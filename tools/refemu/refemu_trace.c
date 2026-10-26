/*
 * Contra III: The Alien Wars (USA) - Reference Emulator Trace Harness (libretro)
 *
 * Runs a libretro SNES core (built from upstream snes9x or bsnes) headlessly on the same
 * input movie as `contra3.exe --headless 0 --movie <m> --trace ours.csv`, and writes a
 * trace CSV with identical columns for tools/compare_trace.py.
 *
 * Logic-frame alignment: Contra III runs all game logic in NMI, and an overrunning frame
 * makes the next NMI hit Konami's $003C busy guard (a lag frame: no logic, joypad unread,
 * logic frame counter $3A unchanged). The recompilation services every NMI, so its movie
 * frames are logic frames. This harness therefore advances the movie only when $3A advanced,
 * holding the same input across lag frames (where the game ignores it anyway), and marks
 * those frames lag=1. Pokes and the --infinite-lives rule are keyed to logic frames too.
 *
 * Usage:
 *   refemu_trace <core.dll> <rom.sfc> <movie> <out.csv> [--frames N] [--poke F:ADDR=VAL]... [--infinite-lives]
 *     --frames N   stop after N logic frames (default: movie length)
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "libretro.h"
#include "snes_movie.h"

typedef void   (*PFN_set_env)(retro_environment_t);
typedef void   (*PFN_set_video)(retro_video_refresh_t);
typedef void   (*PFN_set_audio)(retro_audio_sample_t);
typedef void   (*PFN_set_audio_batch)(retro_audio_sample_batch_t);
typedef void   (*PFN_set_poll)(retro_input_poll_t);
typedef void   (*PFN_set_state)(retro_input_state_t);
typedef void   (*PFN_void)(void);
typedef bool   (*PFN_load)(const struct retro_game_info *);
typedef void  *(*PFN_mem_data)(unsigned);
typedef size_t (*PFN_mem_size)(unsigned);
typedef void   (*PFN_port_dev)(unsigned, unsigned);

static uint16_t g_pads[2];

/* SNES_BTN_* ($4218 layout) -> libretro joypad ids */
static const struct { uint16_t mask; unsigned id; } k_map[12] = {
    { 0x8000u, RETRO_DEVICE_ID_JOYPAD_B },     { 0x4000u, RETRO_DEVICE_ID_JOYPAD_Y },
    { 0x2000u, RETRO_DEVICE_ID_JOYPAD_SELECT },{ 0x1000u, RETRO_DEVICE_ID_JOYPAD_START },
    { 0x0800u, RETRO_DEVICE_ID_JOYPAD_UP },    { 0x0400u, RETRO_DEVICE_ID_JOYPAD_DOWN },
    { 0x0200u, RETRO_DEVICE_ID_JOYPAD_LEFT },  { 0x0100u, RETRO_DEVICE_ID_JOYPAD_RIGHT },
    { 0x0080u, RETRO_DEVICE_ID_JOYPAD_A },     { 0x0040u, RETRO_DEVICE_ID_JOYPAD_X },
    { 0x0020u, RETRO_DEVICE_ID_JOYPAD_L },     { 0x0010u, RETRO_DEVICE_ID_JOYPAD_R },
};

static bool env_cb(unsigned cmd, void *data) {
    switch (cmd) {
        case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
        case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
            *(const char **)data = ".";
            return true;
        case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
        case RETRO_ENVIRONMENT_GET_CAN_DUPE:
            if (cmd == RETRO_ENVIRONMENT_GET_CAN_DUPE) {
                *(bool *)data = true;
            }
            return true;
        default:
            return false;   /* core options keep their defaults */
    }
}
static void video_cb(const void *d, unsigned w, unsigned h, size_t p) { (void)d; (void)w; (void)h; (void)p; }
static void audio_cb(int16_t l, int16_t r) { (void)l; (void)r; }
static size_t audio_batch_cb(const int16_t *d, size_t n) { (void)d; return n; }
static void poll_cb(void) {}
static int16_t state_cb(unsigned port, unsigned device, unsigned index, unsigned id) {
    (void)index;
    if (port > 1u || device != RETRO_DEVICE_JOYPAD) {
        return 0;
    }
    if (id == RETRO_DEVICE_ID_JOYPAD_MASK) {
        int16_t m = 0;
        for (unsigned i = 0u; i < 12u; ++i) {
            if (g_pads[port] & k_map[i].mask) m |= (int16_t)(1 << k_map[i].id);
        }
        return m;
    }
    for (unsigned i = 0u; i < 12u; ++i) {
        if (k_map[i].id == id) {
            return (g_pads[port] & k_map[i].mask) ? 1 : 0;
        }
    }
    return 0;
}

static FARPROC sym(HMODULE m, const char *name) {
    FARPROC p = GetProcAddress(m, name);
    if (p == NULL) {
        fprintf(stderr, "core is missing %s\n", name);
        exit(2);
    }
    return p;
}

#define LOAD(type, var, name) type var; { FARPROC p_ = sym(core, name); memcpy(&var, &p_, sizeof(var)); }

static inline uint16_t r16(const uint8_t *w, uint32_t a) { return (uint16_t)(w[a] | (w[a + 1u] << 8)); }

int main(int argc, char **argv) {
    if (argc < 5) {
        fprintf(stderr, "usage: %s <core.dll> <rom.sfc> <movie> <out.csv> [--frames N] [--poke F:ADDR=VAL]... [--infinite-lives]\n", argv[0]);
        return 2;
    }
    unsigned max_logic = 0u, npokes = 0u;
    unsigned pk_f[32]; uint32_t pk_a[32]; uint16_t pk_v[32];
    bool inf_lives = false;
    unsigned dump_at = 0u;
    const char *dump_path = NULL;
    for (int i = 5; i < argc; ++i) {
        unsigned f, a, v;
        if (strcmp(argv[i], "--frames") == 0 && i + 1 < argc) {
            max_logic = (unsigned)strtoul(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "--poke") == 0 && i + 1 < argc && sscanf(argv[++i], "%u:%x=%x", &f, &a, &v) == 3 && npokes < 32u) {
            pk_f[npokes] = f; pk_a[npokes] = a; pk_v[npokes] = (uint16_t)v; npokes++;
        } else if (strcmp(argv[i], "--dump-wram") == 0 && i + 1 < argc) {
            /* --dump-wram F:path writes the 128 KB WRAM as it stands once logic frame F is complete */
            char *colon = strchr(argv[++i], ':');
            if (colon != NULL) {
                dump_at = (unsigned)strtoul(argv[i], NULL, 10);
                dump_path = colon + 1;
            }
        } else if (strcmp(argv[i], "--infinite-lives") == 0) {
            inf_lives = true;
        }
    }

    SnesMovie mv;
    char err[256];
    if (!snes_movie_load(argv[3], &mv, err, sizeof(err))) {
        fprintf(stderr, "movie: %s\n", err);
        return 2;
    }
    if (max_logic == 0u) {
        max_logic = mv.frame_count;
    }

    HMODULE core = LoadLibraryA(argv[1]);
    if (core == NULL) {
        fprintf(stderr, "cannot load core %s\n", argv[1]);
        return 2;
    }
    LOAD(PFN_set_env, set_env, "retro_set_environment");
    LOAD(PFN_set_video, set_video, "retro_set_video_refresh");
    LOAD(PFN_set_audio, set_audio, "retro_set_audio_sample");
    LOAD(PFN_set_audio_batch, set_audio_batch, "retro_set_audio_sample_batch");
    LOAD(PFN_set_poll, set_poll, "retro_set_input_poll");
    LOAD(PFN_set_state, set_state, "retro_set_input_state");
    LOAD(PFN_void, r_init, "retro_init");
    LOAD(PFN_void, r_run, "retro_run");
    LOAD(PFN_void, r_deinit, "retro_deinit");
    LOAD(PFN_load, r_load, "retro_load_game");
    LOAD(PFN_mem_data, r_mem_data, "retro_get_memory_data");
    LOAD(PFN_mem_size, r_mem_size, "retro_get_memory_size");
    LOAD(PFN_port_dev, r_port_dev, "retro_set_controller_port_device");

    set_env(env_cb);
    r_init();
    set_video(video_cb);
    set_audio(audio_cb);
    set_audio_batch(audio_batch_cb);
    set_poll(poll_cb);
    set_state(state_cb);

    FILE *rf = fopen(argv[2], "rb");
    if (rf == NULL) {
        fprintf(stderr, "cannot open ROM\n");
        return 2;
    }
    static uint8_t rom[1u << 21];
    size_t rom_size = fread(rom, 1u, sizeof(rom), rf);
    fclose(rf);
    struct retro_game_info gi = { argv[2], rom, rom_size, NULL };
    if (!r_load(&gi)) {
        fprintf(stderr, "core rejected the ROM\n");
        return 2;
    }
    r_port_dev(0u, RETRO_DEVICE_JOYPAD);
    r_port_dev(1u, RETRO_DEVICE_JOYPAD);

    uint8_t *wram = (uint8_t *)r_mem_data(RETRO_MEMORY_SYSTEM_RAM);
    if (wram == NULL || r_mem_size(RETRO_MEMORY_SYSTEM_RAM) < 0x20000u) {
        fprintf(stderr, "core does not expose 128 KB WRAM\n");
        return 2;
    }

    FILE *out = fopen(argv[4], "w");
    fprintf(out, "frame,movie_frame,pad1,pad2,state,sub,nmi_mode,stage,frame_ctr,"
                 "p1_x,p1_y,p2_x,p2_y,lives1,lives2,cam_x,rng_lo,rng_hi,lag,nmi_cycles,apu_polls\n");

    unsigned logic = 0u, video = 0u, lag_total = 0u;
    char pending[96] = "", pending_probes[192] = "";
    static uint8_t prev_wram[0x20000];
    uint16_t prev_ctr = r16(wram, 0x3Au), prev_lives[2] = { 0u, 0u };
    bool counted_in_progress = false;
    while (logic < max_logic && video < max_logic * 4u + 2000u) {
        uint32_t mf = logic;  /* movie row for the next logic frame */
        g_pads[0] = snes_movie_pad(&mv, mf, 0u);
        g_pads[1] = snes_movie_pad(&mv, mf, 1u);
        /* Host-side rules before the next logic frame, as contra3.exe --headless applies them */
        /* Host rules run only between logic frames, like the recomp: Konami's busy flag $3C is
           non-zero while an NMI's game logic is still executing (it overran into this frame) */
        bool between_logic = r16(wram, 0x3Cu) == 0u;
        if (inf_lives && logic > 0u && between_logic) {  /* track lives only once the game is running */
            static const uint32_t la[2] = { 0x1F8Au, 0x1FCAu };
            for (unsigned p = 0u; p < 2u; ++p) {
                uint16_t now = r16(wram, la[p]);
                if (now == 0u && prev_lives[p] != 0u) {
                    wram[la[p]] = 2u; wram[la[p] + 1u] = 0u; now = 2u;
                }
                prev_lives[p] = now;
            }
        }
        for (unsigned k = 0u; k < npokes; ++k) {
            if (pk_f[k] != 0u && pk_f[k] <= logic + 1u && between_logic) {
                wram[pk_a[k] & 0x1FFFFu] = (uint8_t)pk_v[k];
                wram[(pk_a[k] + 1u) & 0x1FFFFu] = (uint8_t)(pk_v[k] >> 8);
                pk_f[k] = 0u;  /* fire once even if the frame lags */
            }
        }

        r_run();
        video++;
        uint16_t ctr = r16(wram, 0x3Au);
        /*
         * Count logic frames from $3A, which MainModule_Dispatch increments at the start of each
         * logic frame - i.e. in the video frame whose NMI read the joypad. An increment of n means
         * n logic frames. A few init frames also reset it (STZ $3A at $00:ADBF): a reset counts
         * as one logic frame unless that NMI was still running at the previous boundary ($3C != 0)
         * and was already counted through its increment. The boot-time clear of the power-on
         * $55 fill ($5555 -> 0) is not a logic frame.
         */
        unsigned started = 0u;
        uint16_t delta = (uint16_t)(ctr - prev_ctr);
        if (ctr != prev_ctr) {
            if (prev_ctr == 0x5555u) {
                started = 0u;
            } else if (delta >= 1u && delta <= 8u) {
                started = delta;
            } else {
                started = counted_in_progress ? 0u : 1u;
            }
            prev_ctr = ctr;
        }
        counted_in_progress = (started > 0u || counted_in_progress) && r16(wram, 0x3Cu) != 0u;
        bool lag = (started == 0u);
        if (!lag) {
            logic += started;
        } else {
            lag_total++;
        }
        /*
         * A logic frame that overruns its video frame is still executing when retro_run returns,
         * so its WRAM is only final at the end of the last video frame before the next logic
         * frame starts. Lag rows are written immediately; the logic row is held back and its
         * probe values refreshed on every following lag frame, then written once complete.
         */
        char probes[192];
        snprintf(probes, sizeof(probes), "%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
                 wram[0x34], wram[0x42], r16(wram, 0x86), ctr,
                 r16(wram, 0x206), r16(wram, 0x20E), r16(wram, 0x246), r16(wram, 0x24E),
                 r16(wram, 0x1F8A), r16(wram, 0x1FCA), r16(wram, 0x1380),
                 r16(wram, 0xE8), r16(wram, 0xEA), 0u);
        /* Logic frame dump_at is complete at the end of the video frame before logic dump_at+1 began */
        if (!lag && dump_path != NULL && logic == dump_at + 1u) {
            FILE *df = fopen(dump_path, "wb");
            if (df != NULL) {
                fwrite(prev_wram, 1u, sizeof(prev_wram), df);
                fclose(df);
            }
            dump_path = NULL;
        }
        if (dump_path != NULL) {
            memcpy(prev_wram, wram, sizeof(prev_wram));
        }
        if (!lag) {
            if (pending[0] != '\0') {
                fprintf(out, "%s,%s,0,0\n", pending, pending_probes);
            }
            snprintf(pending, sizeof(pending), "%u,%lld,%u,%u,%u", video, (long long)mf,
                     g_pads[0], g_pads[1], wram[0x32]);
            snprintf(pending_probes, sizeof(pending_probes), "%s", probes);
        } else {
            fprintf(out, "%u,-1,%u,%u,%u,%.*s1,0,0\n", video, g_pads[0], g_pads[1], wram[0x32],
                    (int)(strlen(probes) - 1u), probes);
            if (pending[0] != '\0') {
                /* the held logic frame keeps its row identity but takes the newer, more complete state */
                char *comma = strrchr(pending, ',');
                snprintf(comma + 1, sizeof(pending) - (size_t)(comma + 1 - pending), "%u", wram[0x32]);
                snprintf(pending_probes, sizeof(pending_probes), "%s", probes);
            }
        }
    }
    if (pending[0] != '\0') {
        fprintf(out, "%s,%s,0,0\n", pending, pending_probes);
    }
    fclose(out);
    printf("[REFEMU] %u video frames, %u logic frames, %u lag frames -> %s\n", video, logic, lag_total, argv[4]);
    r_deinit();
    snes_movie_free(&mv);
    return 0;
}
