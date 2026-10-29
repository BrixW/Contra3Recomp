/*
 * Contra III: The Alien Wars (USA) - Linux (X11 + ALSA) Interactive Runner & Headless Harness
 *
 * Native Linux frontend for the portable recompilation. Video is presented through
 * a nearest-neighbour integer-scaled X11 window; audio is the 32 kHz S-DSP stream
 * upsampled to 48 kHz and written to ALSA (loaded at runtime, so no ALSA headers or
 * link-time dependency are needed). Keyboard bindings live in contra3.ini.
 *
 * Interactive Controls (Player 1 keyboard + Player 2 numpad):
 *   Arrow Keys        : D-Pad (Move / Aim)
 *   Z or J            : B (Jump)
 *   X or K            : Y (Shoot)
 *   A or U            : X (Switch Weapon)
 *   S or I            : A (Mega Bomb)
 *   Q / E             : L / R Shoulder (Lock Aim / Dual Fire)
 *   Enter             : Start
 *   Right Shift / BS  : Select
 *
 * Hotkeys: Tab/F1 turbo, P pause, Space step, M mute, F3 lag model,
 *          F4 stage select, Shift+F4 infinite lives, F5/F7 quicksave/load,
 *          F6 record, F8 30 lives, F11/Alt+Enter fullscreen, F12 screenshot,
 *          Ctrl+R soft reset, Esc quit.
 */

#define _POSIX_C_SOURCE 200809L

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "contra3_recomp.h"
#include "snes_ppu.h"
#include "snes_apu.h"
#include "snes_movie.h"
#include "snes_savestate.h"
#include "snes_mainthread.h"
#include "snes_rom.h"
#include "contra3_extras.h"
#include "linux_config.h"

/* --- Host audio: ALSA loaded through dlopen, no build-time dependency --- */
#define AUDIO_HOST_SAMPLE_RATE    48000u
#define AUDIO_IN_PAIR_COUNT       341u
#define AUDIO_IN_BLOCK_FRAMES     (AUDIO_IN_PAIR_COUNT * 2u)   /* 682 frames @ 32 kHz */
#define AUDIO_BLOCK_FRAMES        (AUDIO_IN_PAIR_COUNT * 3u)   /* 1023 frames @ 48 kHz */

typedef struct _snd_pcm snd_pcm_t;
typedef long snd_pcm_sframes_t;
typedef unsigned long snd_pcm_uframes_t;

enum {
    SND_PCM_STREAM_PLAYBACK   = 0,
    SND_PCM_NONBLOCK          = 1,
    SND_PCM_FORMAT_S16_LE     = 2,
    SND_PCM_ACCESS_RW_INTERLEAVED = 3
};

typedef struct AlsaApi {
    void *lib;
    int  (*pcm_open)(snd_pcm_t **, const char *, int, int);
    int  (*pcm_set_params)(snd_pcm_t *, int, int, unsigned, unsigned, int, unsigned);
    snd_pcm_sframes_t (*pcm_avail_update)(snd_pcm_t *);
    snd_pcm_sframes_t (*pcm_writei)(snd_pcm_t *, const void *, snd_pcm_uframes_t);
    int  (*pcm_recover)(snd_pcm_t *, int, int);
    int  (*pcm_drop)(snd_pcm_t *);
    int  (*pcm_prepare)(snd_pcm_t *);
    int  (*pcm_close)(snd_pcm_t *);
} AlsaApi;

static AlsaApi     g_alsa;
static snd_pcm_t  *g_pcm         = NULL;
static bool        g_audio_ok    = false;
static bool        g_audio_started = false;
static bool        g_audio_muted = false;
static int16_t     g_resample_hist[3][2]; /* [h0, h1, h2][L/R] @ 32 kHz */
static int16_t     g_audio_buf[AUDIO_BLOCK_FRAMES * 2u];

static SnesCpuContext g_ctx;
static SnesApu        g_apu;
static uint32_t       g_framebuf[SNES_SCREEN_WIDTH * SNES_SCREEN_HEIGHT];

static inline int16_t clamp_pcm16(int32_t v) {
    if (v > 32767)  return 32767;
    if (v < -32768) return -32768;
    return (int16_t)v;
}

static inline int32_t round_div27(int32_t v) {
    return (v >= 0) ? ((v + 13) / 27) : ((v - 13) / 27);
}

static void upsample_32k_to_48k(const int16_t *in_32k, int16_t *out_48k, unsigned pair_count) {
    for (unsigned p = 0u; p < pair_count; ++p) {
        for (unsigned ch = 0u; ch < 2u; ++ch) {
            int32_t h0 = (int32_t)g_resample_hist[0][ch];
            int32_t h1 = (int32_t)g_resample_hist[1][ch];
            int32_t h2 = (int32_t)g_resample_hist[2][ch];
            int32_t s0 = (int32_t)in_32k[(p * 2u + 0u) * 2u + ch];
            int32_t s1 = (int32_t)in_32k[(p * 2u + 1u) * 2u + ch];

            out_48k[(p * 3u + 0u) * 2u + ch] = (int16_t)h1;
            out_48k[(p * 3u + 1u) * 2u + ch] = clamp_pcm16(round_div27(-h0 + 9 * h1 + 21 * h2 - 2 * s0));
            out_48k[(p * 3u + 2u) * 2u + ch] = clamp_pcm16(round_div27(-2 * h1 + 21 * h2 + 9 * s0 - s1));

            g_resample_hist[0][ch] = (int16_t)h2;
            g_resample_hist[1][ch] = (int16_t)s0;
            g_resample_hist[2][ch] = (int16_t)s1;
        }
    }
}

static bool alsa_load(void) {
    static const char *const names[] = { "libasound.so.2", "libasound.so" };
    for (size_t i = 0u; i < sizeof(names) / sizeof(names[0]) && g_alsa.lib == NULL; ++i) {
        g_alsa.lib = dlopen(names[i], RTLD_NOW | RTLD_LOCAL);
    }
    if (g_alsa.lib == NULL) {
        return false;
    }
    void *sym;
#define ALSA_LOAD(field, name)                                                   \
    do {                                                                         \
        sym = dlsym(g_alsa.lib, name);                                           \
        if (sym == NULL) {                                                       \
            dlclose(g_alsa.lib);                                                 \
            memset(&g_alsa, 0, sizeof(g_alsa));                                  \
            return false;                                                        \
        }                                                                        \
        memcpy(&g_alsa.field, &sym, sizeof(sym));                                \
    } while (0)
    ALSA_LOAD(pcm_open, "snd_pcm_open");
    ALSA_LOAD(pcm_set_params, "snd_pcm_set_params");
    ALSA_LOAD(pcm_avail_update, "snd_pcm_avail_update");
    ALSA_LOAD(pcm_writei, "snd_pcm_writei");
    ALSA_LOAD(pcm_recover, "snd_pcm_recover");
    ALSA_LOAD(pcm_drop, "snd_pcm_drop");
    ALSA_LOAD(pcm_prepare, "snd_pcm_prepare");
    ALSA_LOAD(pcm_close, "snd_pcm_close");
#undef ALSA_LOAD
    return true;
}

static void linux_audio_init(void) {
    memset(g_resample_hist, 0, sizeof(g_resample_hist));
    if (!alsa_load()) {
        fprintf(stderr, "[AUDIO] ALSA runtime not found; running without sound\n");
        return;
    }
    static const char *const devices[] = { "default", "pulse", "pipewire", "sysdefault" };
    for (size_t i = 0u; i < sizeof(devices) / sizeof(devices[0]); ++i) {
        if (g_alsa.pcm_open(&g_pcm, devices[i], SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) == 0) {
            if (g_alsa.pcm_set_params(g_pcm, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED,
                                      2u, AUDIO_HOST_SAMPLE_RATE, 1, 120000u) == 0) {
                g_audio_ok = true;
                break;
            }
            g_alsa.pcm_close(g_pcm);
            g_pcm = NULL;
        }
    }
    if (!g_audio_ok) {
        fprintf(stderr, "[AUDIO] no usable ALSA playback device; running without sound\n");
    }
}

static void linux_audio_reset(void) {
    if (g_pcm != NULL) {
        g_alsa.pcm_drop(g_pcm);
        g_alsa.pcm_prepare(g_pcm);
    }
    snes_apu_clear_samples(&g_apu);
    memset(g_resample_hist, 0, sizeof(g_resample_hist));
    g_audio_started = false;
}

static void linux_audio_pump(bool turbo, bool paused) {
    if (!g_audio_ok || g_audio_muted || paused) {
        snes_apu_clear_samples(&g_apu);
        memset(g_resample_hist, 0, sizeof(g_resample_hist));
        g_audio_started = false;
        return;
    }
    /* Pre-buffer three blocks (~64 ms) so the first write does not underrun */
    if (!g_audio_started) {
        if (g_apu.pcm_count < AUDIO_IN_BLOCK_FRAMES * 3u) {
            return;
        }
        g_audio_started = true;
    }

    snd_pcm_sframes_t avail = g_alsa.pcm_avail_update(g_pcm);
    if (avail < 0) {
        g_alsa.pcm_recover(g_pcm, (int)avail, 1);
        return;
    }
    unsigned max_blocks = turbo ? 2u : 4u;
    for (unsigned written = 0u; written < max_blocks; ++written) {
        if (avail < (snd_pcm_sframes_t)AUDIO_BLOCK_FRAMES || g_apu.pcm_count < AUDIO_IN_BLOCK_FRAMES) {
            break;
        }
        int16_t raw_32k[AUDIO_IN_BLOCK_FRAMES * 2u];
        size_t n = snes_apu_pop_samples(&g_apu, raw_32k, AUDIO_IN_BLOCK_FRAMES);
        if (n < AUDIO_IN_BLOCK_FRAMES) {
            break;
        }
        upsample_32k_to_48k(raw_32k, g_audio_buf, AUDIO_IN_PAIR_COUNT);
        snd_pcm_sframes_t w = g_alsa.pcm_writei(g_pcm, g_audio_buf, AUDIO_BLOCK_FRAMES);
        if (w < 0) {
            g_alsa.pcm_recover(g_pcm, (int)w, 1);
            break;
        }
        avail -= (snd_pcm_sframes_t)AUDIO_BLOCK_FRAMES;
    }

    /* Drop excess only during uncapped turbo or extreme OS sleep stalls */
    if (turbo && g_apu.pcm_count > AUDIO_IN_BLOCK_FRAMES * 2u) {
        snes_apu_clear_samples(&g_apu);
    } else if (!turbo && g_apu.pcm_count > AUDIO_IN_BLOCK_FRAMES * 8u) {
        int16_t discard[512 * 2];
        snes_apu_pop_samples(&g_apu, discard, 512u);
    }
}

static void linux_audio_shutdown(void) {
    if (g_pcm != NULL) {
        g_alsa.pcm_drop(g_pcm);
        g_alsa.pcm_close(g_pcm);
        g_pcm = NULL;
    }
    if (g_alsa.lib != NULL) {
        dlclose(g_alsa.lib);
        memset(&g_alsa, 0, sizeof(g_alsa));
    }
}

/* --- X11 window & software presentation --- */
static Display *g_dpy       = NULL;
static Window   g_win       = 0;
static GC       g_gc        = NULL;
static XImage  *g_ximg      = NULL;
static int      g_win_w     = SNES_SCREEN_WIDTH * 3;
static int      g_win_h     = SNES_SCREEN_HEIGHT * 3;
static bool     g_fullscreen = false;
static bool     g_focused   = true;
static bool     g_running   = true;

static void linux_video_resize(int w, int h) {
    if (w < 1 || h < 1 || (w == g_win_w && h == g_win_h && g_ximg != NULL)) {
        return;
    }
    if (g_ximg != NULL) {
        XDestroyImage(g_ximg); /* also frees g_ximg->data */
        g_ximg = NULL;
    }
    g_win_w = w;
    g_win_h = h;
    char *data = (char *)calloc((size_t)w * (size_t)h, 4u);
    if (data == NULL) {
        return;
    }
    g_ximg = XCreateImage(g_dpy, DefaultVisual(g_dpy, DefaultScreen(g_dpy)),
                          (unsigned)DefaultDepth(g_dpy, DefaultScreen(g_dpy)), ZPixmap, 0,
                          data, (unsigned)w, (unsigned)h, 32, 0);
    if (g_ximg == NULL) {
        free(data);
        return;
    }
    XSetWindowBackground(g_dpy, g_win, BlackPixel(g_dpy, DefaultScreen(g_dpy)));
}

static void linux_video_present(void) {
    if (g_ximg == NULL) {
        return;
    }
    uint32_t *dst    = (uint32_t *)g_ximg->data;
    int       stride = g_ximg->bytes_per_line / 4; /* 32-bit XImage rows may be padded */
    memset(dst, 0, (size_t)stride * (size_t)g_win_h * 4u);

    int scale = g_win_w / SNES_SCREEN_WIDTH;
    int sy = g_win_h / SNES_SCREEN_HEIGHT;
    if (sy < scale) {
        scale = sy;
    }
    if (scale < 1) {
        scale = 1;
    }
    int dst_w = SNES_SCREEN_WIDTH * scale;
    int dst_h = SNES_SCREEN_HEIGHT * scale;
    int dst_x = (g_win_w - dst_w) / 2;
    int dst_y = (g_win_h - dst_h) / 2;
    if (dst_x < 0) dst_x = 0;
    if (dst_y < 0) dst_y = 0;

    for (int y = 0; y < dst_h && dst_y + y < g_win_h; ++y) {
        const uint32_t *src = g_framebuf + (y / scale) * SNES_SCREEN_WIDTH;
        uint32_t *row = dst + (size_t)(dst_y + y) * (size_t)stride + (size_t)dst_x;
        for (int x = 0; x < dst_w && dst_x + x < g_win_w; ++x) {
            row[x] = src[x / scale];
        }
    }
    XPutImage(g_dpy, g_win, g_gc, g_ximg, 0, 0, 0, 0, (unsigned)g_win_w, (unsigned)g_win_h);
    XFlush(g_dpy);
}

static void linux_toggle_fullscreen(void) {
    g_fullscreen = !g_fullscreen;
    Atom state = XInternAtom(g_dpy, "_NET_WM_STATE", False);
    Atom fs    = XInternAtom(g_dpy, "_NET_WM_STATE_FULLSCREEN", False);
    XEvent e;
    memset(&e, 0, sizeof(e));
    e.xclient.type         = ClientMessage;
    e.xclient.window       = g_win;
    e.xclient.message_type = state;
    e.xclient.format       = 32;
    e.xclient.data.l[0]    = g_fullscreen ? 1 : 0; /* _NET_WM_STATE_ADD / _REMOVE */
    e.xclient.data.l[1]    = (long)fs;
    e.xclient.data.l[2]    = 0;
    XSendEvent(g_dpy, DefaultRootWindow(g_dpy), False,
               SubstructureRedirectMask | SubstructureNotifyMask, &e);
    XFlush(g_dpy);
}

/* --- Shared state --- */
static bool     g_paused       = false;
static bool     g_step_once    = false;
static bool     g_turbo_toggle = false;
static unsigned g_shot_index   = 1u;
static bool     g_strict_fallback = false;
static uint32_t g_lag_budget      = 0u;
static uint32_t g_cpu_budget      = 0u;
static Contra3Extras g_extras = { 0u, 0u, false, -1, 0u };

static SnesMovie          g_movie;
static bool               g_movie_active = false;
static uint32_t           g_movie_frame  = 0u;
static SnesMovieRecorder  g_recorder;
static unsigned           g_rec_index    = 1u;
static uint8_t            g_rec_pending_flags = 0u;

static Contra3LinuxConfig g_config;

static char   g_status[192];
static double g_status_until = 0.0;

static uint32_t g_frame_number = 0u;

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static void set_status(const char *msg) {
    snprintf(g_status, sizeof(g_status), "%s", msg);
    g_status_until = now_seconds() + 4.0;
}

static bool key_down(KeySym sym) {
    KeyCode code = XKeysymToKeycode(g_dpy, sym);
    if (code == 0u) {
        return false;
    }
    char keys[32];
    XQueryKeymap(g_dpy, keys);
    return (keys[code / 8u] & (1u << (code % 8u))) != 0;
}

/* Report every unresolved indirect target immediately, with the dispatch site and frame */
static void on_unresolved_jump(SnesCpuContext *ctx, uint32_t target_addr24) {
    static unsigned reported = 0u;
    if (reported >= 32u) {
        return;
    }
    reported++;
    uint32_t site = ctx->last_unresolved_site;
    char msg[160];
    snprintf(msg, sizeof(msg), "[FALLBACK] frame %u: unresolved target $%06X from site $%02X:%04X (state=$%02X stage=%u)%s\n",
             (unsigned)g_frame_number, (unsigned)target_addr24,
             (unsigned)((site >> 16) & 0xFFu), (unsigned)(site & 0xFFFFu),
             snes_bus_read8(ctx, 0x000032u), snes_bus_read16(ctx, 0x000086u),
             (reported == 32u) ? " (further reports suppressed)" : "");
    fputs(msg, stderr);
    FILE *log = fopen("contra3_fallback.log", "a");
    if (log != NULL) {
        fputs(msg, log);
        fclose(log);
    }
}

static void power_on(const uint8_t *rom, size_t rom_size) {
    void (*sink)(void *, int16_t, int16_t) = g_apu.pcm_sink; /* host capture survives power cycles */
    void *sink_user = g_apu.pcm_sink_user;
    snes_context_init(&g_ctx, rom, rom_size);
    snes_apu_init(&g_apu);
    g_apu.pcm_sink = sink;
    g_apu.pcm_sink_user = sink_user;
    snes_apu_attach(&g_ctx, &g_apu);
    g_ctx.on_interp_fallback = on_unresolved_jump;
    g_ctx.strict_fallback = g_strict_fallback;
    g_ctx.lag_budget = g_lag_budget;
    g_ctx.cpu_budget = g_cpu_budget;
    snes_power_on(&g_ctx);
}

static void soft_reset(void) {
    linux_audio_reset();
    snes_soft_reset(&g_ctx);
}

static void toggle_recording(void) {
    if (g_recorder.fp != NULL) {
        char msg[96];
        snprintf(msg, sizeof(msg), "Recording stopped (%u frames)", (unsigned)g_recorder.frames);
        snes_movie_record_end(&g_recorder);
        set_status(msg);
    } else {
        char name[64];
        snprintf(name, sizeof(name), "contra3_rec_%04u.c3m", g_rec_index++);
        if (snes_movie_record_begin(&g_recorder, name, "recorded interactively; starts from power-on")) {
            power_on(g_ctx.rom, g_ctx.rom_size);
            linux_audio_reset();
            g_movie_active = false;
            char msg[96];
            snprintf(msg, sizeof(msg), "Recording to %s (power cycled)", name);
            set_status(msg);
        } else {
            char msg[96];
            snprintf(msg, sizeof(msg), "Could not create %s", name);
            set_status(msg);
        }
    }
}

static void handle_key(KeySym sym) {
    bool shift = key_down(XK_Shift_L) || key_down(XK_Shift_R);
    bool ctrl  = key_down(XK_Control_L) || key_down(XK_Control_R);

    if (sym == XK_Escape) {
        g_running = false;
    } else if (sym == XK_F2) {
        set_status("Key rebinding is not in the Linux frontend yet - edit contra3.ini");
    } else if (sym == XK_Return && (key_down(XK_Alt_L) || key_down(XK_Alt_R))) {
        linux_toggle_fullscreen();
    } else if (sym == XK_p || sym == XK_Pause) {
        g_paused = !g_paused;
        if (g_paused) {
            snes_apu_clear_samples(&g_apu);
            memset(g_resample_hist, 0, sizeof(g_resample_hist));
            g_audio_started = false;
        }
    } else if (sym == XK_m) {
        g_audio_muted = !g_audio_muted;
        if (g_audio_muted) {
            snes_apu_clear_samples(&g_apu);
            memset(g_resample_hist, 0, sizeof(g_resample_hist));
            g_audio_started = false;
        }
    } else if (sym == XK_space && g_paused) {
        g_step_once = true;
    } else if (sym == XK_F1) {
        g_turbo_toggle = !g_turbo_toggle;
    } else if (sym == XK_F3) {
        g_lag_budget = (g_lag_budget != 0u) ? 0u : SNES_LAG_BUDGET_DEFAULT;
        g_ctx.lag_budget = g_lag_budget;
        g_ctx.lag_debt = 0u;
        set_status(g_lag_budget ? "Authentic slowdown ON (lag model)" : "Authentic slowdown OFF");
    } else if (sym == XK_F4 && shift) {
        g_extras.flags ^= CONTRA3_UNLOCK_INFINITE_LIVES;
        set_status((g_extras.flags & CONTRA3_UNLOCK_INFINITE_LIVES) ? "Infinite lives ON" : "Infinite lives OFF");
    } else if (sym == XK_F4) {
        g_extras.flags ^= CONTRA3_UNLOCK_STAGE_SELECT;
        set_status((g_extras.flags & CONTRA3_UNLOCK_STAGE_SELECT)
                       ? "Stage select ON (press START on the title screen)" : "Stage select OFF");
    } else if (sym == XK_F5) {
        SnesSavestateResult r = snes_savestate_save("contra3_quicksave.state", &g_ctx, &g_apu);
        char msg[128];
        snprintf(msg, sizeof(msg), r == SNES_SAVESTATE_OK ? "State saved" : "Save failed: %s",
                 snes_savestate_result_str(r));
        set_status(msg);
    } else if (sym == XK_F7) {
        linux_audio_reset();
        SnesSavestateResult r = snes_savestate_load("contra3_quicksave.state", &g_ctx, &g_apu);
        char msg[128];
        snprintf(msg, sizeof(msg), r == SNES_SAVESTATE_OK ? "State loaded" : "Load refused: %s",
                 snes_savestate_result_str(r));
        set_status(msg);
    } else if (sym == XK_F6) {
        toggle_recording();
    } else if (sym == XK_F8) {
        snes_bus_write16(&g_ctx, 0x001E3Cu, 0x001Eu);
        snes_bus_write16(&g_ctx, 0x001F8Au, 0x001Eu);
        set_status("30 lives granted");
    } else if (sym == XK_F11) {
        linux_toggle_fullscreen();
    } else if (sym == XK_F12) {
        char name[64];
        snprintf(name, sizeof(name), "contra3_shot_%04u.bmp", g_shot_index++);
        snes_write_bmp(name, g_framebuf, SNES_SCREEN_WIDTH, SNES_SCREEN_HEIGHT);
        set_status(name);
    } else if (sym == XK_r && ctrl) {
        soft_reset();
        g_rec_pending_flags |= SNES_MOVIE_FLAG_RESET;
    }
}

static void linux_video_init(void) {
    g_dpy = XOpenDisplay(NULL);
    if (g_dpy == NULL) {
        fprintf(stderr, "Error: cannot open X display (is DISPLAY set?)\n");
        exit(1);
    }
    int screen = DefaultScreen(g_dpy);
    g_win = XCreateSimpleWindow(g_dpy, RootWindow(g_dpy, screen),
                                0, 0, (unsigned)g_win_w, (unsigned)g_win_h, 0,
                                BlackPixel(g_dpy, screen), BlackPixel(g_dpy, screen));
    if (g_win == 0) {
        fprintf(stderr, "Error: cannot create X11 window\n");
        exit(1);
    }
    XSelectInput(g_dpy, g_win, KeyPressMask | KeyReleaseMask | StructureNotifyMask |
                                ExposureMask | FocusChangeMask);
    XStoreName(g_dpy, g_win, "Contra III: The Alien Wars (USA) - Static C11 Recompilation");
    XSetWMProtocols(g_dpy, g_win, &(Atom){ XInternAtom(g_dpy, "WM_DELETE_WINDOW", False) }, 1);

    XSizeHints hints;
    memset(&hints, 0, sizeof(hints));
    hints.flags      = PMinSize;
    hints.min_width  = SNES_SCREEN_WIDTH;
    hints.min_height = SNES_SCREEN_HEIGHT;
    XSetWMNormalHints(g_dpy, g_win, &hints);

    g_gc = XCreateGC(g_dpy, g_win, 0, NULL);
    Bool supported = False;
    XkbSetDetectableAutoRepeat(g_dpy, True, &supported);
    XMapRaised(g_dpy, g_win);
    linux_video_resize(g_win_w, g_win_h);
    XFlush(g_dpy);
}

static void linux_video_shutdown(void) {
    if (g_ximg != NULL) {
        XDestroyImage(g_ximg);
        g_ximg = NULL;
    }
    if (g_gc != NULL && g_dpy != NULL) {
        XFreeGC(g_dpy, g_gc);
        g_gc = NULL;
    }
    if (g_win != 0 && g_dpy != NULL) {
        XDestroyWindow(g_dpy, g_win);
        g_win = 0;
    }
    if (g_dpy != NULL) {
        XCloseDisplay(g_dpy);
        g_dpy = NULL;
    }
}

static void linux_poll_events(void) {
    while (XPending(g_dpy) > 0) {
        XEvent ev;
        XNextEvent(g_dpy, &ev);
        switch (ev.type) {
            case ClientMessage:
                if ((Atom)ev.xclient.data.l[0] == XInternAtom(g_dpy, "WM_DELETE_WINDOW", False)) {
                    g_running = false;
                }
                break;
            case DestroyNotify:
                g_running = false;
                g_win = 0;
                break;
            case ConfigureNotify:
                linux_video_resize(ev.xconfigure.width, ev.xconfigure.height);
                break;
            case Expose:
                if (ev.xexpose.count == 0) {
                    linux_video_present();
                }
                break;
            case FocusIn:
                g_focused = true;
                break;
            case FocusOut:
                g_focused = false;
                break;
            case KeyPress:
                handle_key(XLookupKeysym(&ev.xkey, 0));
                break;
            default:
                break;
        }
    }
}

/* --- Headless Scripted Verification Mode (keep in sync with win32_main.c) --- */
typedef struct ScriptPress {
    unsigned start_frame;
    unsigned end_frame;
    uint16_t mask;
    uint8_t  port;      /* 0 = Player 1, 1 = Player 2 */
} ScriptPress;

typedef struct ScriptDump {
    unsigned    frame;
    const char *path;
} ScriptDump;

typedef struct ScriptPoke {
    unsigned frame;
    uint32_t addr;
    uint16_t value;
} ScriptPoke;

typedef struct HeadlessOptions {
    unsigned           total_frames;
    const ScriptPress *presses;
    unsigned           num_presses;
    const ScriptDump  *dumps;
    unsigned           num_dumps;
    const ScriptPoke  *pokes;
    unsigned           num_pokes;
    const char        *wav_path;
    const SnesMovie   *movie;
    int                movie_offset;
    const uint8_t     *lag;
    uint32_t           lag_count;
    const char        *trace_path;
    const char        *record_path;
    bool               verify;
    bool               infinite_lives;
    bool               movie_logic;
    unsigned           hang_frames;
} HeadlessOptions;

/* WRAM probes shared by --trace, --verify and tools/emu_trace.lua (keep the three in sync) */
#define PROBE_GAME_STATE   0x000032u
#define PROBE_GAME_SUB     0x000034u
#define PROBE_FRAME_CTR    0x00003Au
#define PROBE_NMI_MODE     0x000042u
#define PROBE_PRNG_LO      0x0000E8u
#define PROBE_PRNG_HI      0x0000EAu
#define PROBE_P1_SLOT      0x000200u
#define PROBE_P2_SLOT      0x000240u
#define PROBE_SLOT_X       0x06u
#define PROBE_SLOT_Y       0x0Eu
#define PROBE_CAM_X        0x001380u
#define PROBE_STAGE        0x000086u
#define PROBE_LIVES_P1     0x001F8Au
#define PROBE_LIVES_P2     0x001FCAu

static uint16_t parse_button_spec(const char *spec) {
    if (strncmp(spec, "0x", 2) == 0 || strncmp(spec, "0X", 2) == 0) {
        return (uint16_t)strtoul(spec, NULL, 16);
    }
    uint16_t m = 0u;
    if (strstr(spec, "START") != NULL)  m |= SNES_BTN_START;
    if (strstr(spec, "SELECT") != NULL) m |= SNES_BTN_SELECT;
    if (strstr(spec, "UP") != NULL)     m |= SNES_BTN_UP;
    if (strstr(spec, "DOWN") != NULL)   m |= SNES_BTN_DOWN;
    if (strstr(spec, "LEFT") != NULL)   m |= SNES_BTN_LEFT;
    if (strstr(spec, "RIGHT") != NULL)  m |= SNES_BTN_RIGHT;
    if (strstr(spec, "JUMP") != NULL || strcmp(spec, "B") == 0)  m |= SNES_BTN_B;
    if (strstr(spec, "SHOOT") != NULL || strcmp(spec, "Y") == 0) m |= SNES_BTN_Y;
    if (strstr(spec, "BOMB") != NULL || strcmp(spec, "A") == 0)  m |= SNES_BTN_A;
    if (strcmp(spec, "X") == 0) m |= SNES_BTN_X;
    if (strstr(spec, "LSHOULDER") != NULL || strcmp(spec, "L") == 0) m |= SNES_BTN_L;
    if (strstr(spec, "RSHOULDER") != NULL || strcmp(spec, "R") == 0) m |= SNES_BTN_R;
    return m;
}

static uint8_t *load_lag_file(const char *path, uint32_t *out_count) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return NULL;
    }
    uint32_t cap = 1u << 16, count = 0u;
    uint8_t *lag = (uint8_t *)calloc(cap, 1u);
    char line[512];
    int lag_col = -1;
    bool first = true;
    while (lag != NULL && fgets(line, sizeof(line), fp) != NULL) {
        if (first) {
            first = false;
            if (strstr(line, "lag") != NULL) {
                int col = 0;
                for (char *tok = strtok(line, ",\r\n"); tok != NULL; tok = strtok(NULL, ",\r\n"), ++col) {
                    if (strcmp(tok, "lag") == 0) {
                        lag_col = col;
                    }
                }
                continue;
            }
        }
        uint32_t idx = 0u;
        uint8_t  val = 0u;
        if (lag_col >= 0) {
            int col = 0;
            idx = count;
            for (char *tok = strtok(line, ",\r\n"); tok != NULL; tok = strtok(NULL, ",\r\n"), ++col) {
                if (col == lag_col) {
                    val = (uint8_t)(strtoul(tok, NULL, 10) != 0u);
                }
            }
        } else {
            char *end = NULL;
            unsigned long v = strtoul(line, &end, 10);
            if (end == line) {
                continue;
            }
            idx = (uint32_t)v;
            val = 1u;
        }
        while (idx >= cap) {
            uint8_t *nl = (uint8_t *)realloc(lag, (size_t)cap * 2u);
            if (nl == NULL) {
                free(lag);
                fclose(fp);
                return NULL;
            }
            memset(nl + cap, 0, cap);
            lag = nl;
            cap *= 2u;
        }
        lag[idx] = val;
        if (idx + 1u > count) {
            count = idx + 1u;
        }
    }
    fclose(fp);
    *out_count = count;
    return lag;
}

typedef struct ProbeSample {
    uint8_t  state, sub, nmi_mode;
    uint16_t stage, frame_ctr;
    uint16_t p1_x, p1_y, p2_x, p2_y;
    uint16_t lives1, lives2, cam_x;
    uint16_t rng_lo, rng_hi;
} ProbeSample;

static void probe_sample(SnesCpuContext *ctx, ProbeSample *s) {
    s->state     = snes_bus_read8(ctx, PROBE_GAME_STATE);
    s->sub       = snes_bus_read8(ctx, PROBE_GAME_SUB);
    s->nmi_mode  = snes_bus_read8(ctx, PROBE_NMI_MODE);
    s->stage     = snes_bus_read16(ctx, PROBE_STAGE);
    s->frame_ctr = snes_bus_read16(ctx, PROBE_FRAME_CTR);
    s->p1_x      = snes_bus_read16(ctx, PROBE_P1_SLOT + PROBE_SLOT_X);
    s->p1_y      = snes_bus_read16(ctx, PROBE_P1_SLOT + PROBE_SLOT_Y);
    s->p2_x      = snes_bus_read16(ctx, PROBE_P2_SLOT + PROBE_SLOT_X);
    s->p2_y      = snes_bus_read16(ctx, PROBE_P2_SLOT + PROBE_SLOT_Y);
    s->lives1    = snes_bus_read16(ctx, PROBE_LIVES_P1);
    s->lives2    = snes_bus_read16(ctx, PROBE_LIVES_P2);
    s->cam_x     = snes_bus_read16(ctx, PROBE_CAM_X);
    s->rng_lo    = snes_bus_read16(ctx, PROBE_PRNG_LO);
    s->rng_hi    = snes_bus_read16(ctx, PROBE_PRNG_HI);
}

typedef struct WavCapture {
    int16_t *data;
    size_t   frames;
    size_t   cap;
    bool     failed;
} WavCapture;

static WavCapture g_wav;

static void wav_sink(void *user, int16_t l, int16_t r) {
    WavCapture *w = (WavCapture *)user;
    if (w->failed) {
        return;
    }
    if (w->frames == w->cap) {
        size_t ncap = (w->cap == 0u) ? (size_t)1u << 20 : w->cap * 2u;
        int16_t *nd = (int16_t *)realloc(w->data, ncap * 2u * sizeof(int16_t));
        if (nd == NULL) {
            w->failed = true;
            return;
        }
        w->data = nd;
        w->cap = ncap;
    }
    w->data[w->frames * 2u + 0u] = l;
    w->data[w->frames * 2u + 1u] = r;
    w->frames++;
}

static int run_headless(const uint8_t *rom, size_t rom_size, const HeadlessOptions *opt) {
    power_on(rom, rom_size);

    const SnesMovie *mv = opt->movie;
    unsigned total_frames = opt->total_frames;
    if (total_frames == 0u && mv != NULL) {
        int64_t n = (int64_t)mv->frame_count - (int64_t)opt->movie_offset;
        total_frames = (n > 0) ? (unsigned)n * (opt->movie_logic ? 4u : 1u) : 0u;
    }

    if (opt->wav_path != NULL) {
        g_apu.pcm_sink = wav_sink;
        g_apu.pcm_sink_user = &g_wav;
    }

    FILE *trace = NULL;
    if (opt->trace_path != NULL) {
        trace = fopen(opt->trace_path, "w");
        if (trace == NULL) {
            fprintf(stderr, "Error: cannot create trace '%s'\n", opt->trace_path);
        } else {
            fprintf(trace, "frame,movie_frame,pad1,pad2,state,sub,nmi_mode,stage,frame_ctr,"
                           "p1_x,p1_y,p2_x,p2_y,lives1,lives2,cam_x,rng_lo,rng_hi,lag,nmi_cycles,apu_polls\n");
        }
    }
    SnesMovieRecorder rec;
    memset(&rec, 0, sizeof(rec));
    if (opt->record_path != NULL && !snes_movie_record_begin(&rec, opt->record_path, "recorded by --headless")) {
        fprintf(stderr, "Error: cannot create movie '%s'\n", opt->record_path);
    }

    int64_t  movie_pos       = (int64_t)opt->movie_offset;
    uint32_t lag_skipped     = 0u;
    bool     movie_done      = (mv == NULL);
    unsigned movie_end_frame = 0u;

    ProbeSample prev;
    probe_sample(&g_ctx, &prev);
    uint16_t max_stage       = 0u;
    unsigned gameplay_frames = 0u;
    uint64_t nmi_sum = 0u, nmi_samples = 0u;
    uint32_t nmi_max = 0u, nmi_over = 0u;
    static uint32_t nmi_hist[256];
    memset(nmi_hist, 0, sizeof(nmi_hist));
    unsigned last_ctr_change = 0u;
    bool     hang            = false;
    unsigned state_seen      = 0u;

    double t0 = now_seconds();

    uint16_t prev_lives[2] = { 0u, 0u };
    unsigned logic_done = 0u;
    unsigned f = 1u;
    for (; f <= total_frames; ++f) {
        uint16_t pad1 = 0u, pad2 = 0u;
        uint8_t  flags = 0u;
        int64_t  this_movie_frame = -1;

        if (!movie_done) {
            if (movie_pos < 0) {
                movie_pos++;
            } else {
                while ((uint64_t)movie_pos < mv->frame_count && opt->lag != NULL &&
                       (uint64_t)movie_pos < opt->lag_count && opt->lag[movie_pos] != 0u) {
                    movie_pos++;
                    lag_skipped++;
                }
                if ((uint64_t)movie_pos < mv->frame_count) {
                    this_movie_frame = movie_pos;
                    pad1  = snes_movie_pad(mv, (uint32_t)movie_pos, 0u);
                    pad2  = snes_movie_pad(mv, (uint32_t)movie_pos, 1u);
                    flags = snes_movie_flags(mv, (uint32_t)movie_pos);
                    movie_pos++;
                } else {
                    movie_done = true;
                    movie_end_frame = f;
                    if (opt->total_frames == 0u) {
                        break;
                    }
                }
            }
        }
        for (unsigned p = 0u; p < opt->num_presses; ++p) {
            if (f >= opt->presses[p].start_frame && f <= opt->presses[p].end_frame) {
                if (opt->presses[p].port == 0u) {
                    pad1 |= opt->presses[p].mask;
                } else {
                    pad2 |= opt->presses[p].mask;
                }
            }
        }

        if ((flags & SNES_MOVIE_FLAG_POWER) != 0u) {
            power_on(rom, rom_size);
        } else if ((flags & SNES_MOVIE_FLAG_RESET) != 0u) {
            soft_reset();
        }
        if (opt->infinite_lives) {
            static const uint32_t lives_addr[2] = { PROBE_LIVES_P1, PROBE_LIVES_P2 };
            for (unsigned p = 0u; p < 2u; ++p) {
                uint16_t now = snes_bus_read16(&g_ctx, lives_addr[p]);
                if (now == 0u && prev_lives[p] != 0u) {
                    snes_bus_write16(&g_ctx, lives_addr[p], 2u);
                    now = 2u;
                }
                prev_lives[p] = now;
            }
        }
        for (unsigned k = 0u; k < opt->num_pokes; ++k) {
            if (opt->pokes[k].frame == (opt->movie_logic ? logic_done + 1u : f)) {
                snes_bus_write16(&g_ctx, opt->pokes[k].addr, opt->pokes[k].value);
            }
        }

        bool need_render = (f == total_frames);
        for (unsigned d = 0u; d < opt->num_dumps; ++d) {
            if (opt->dumps[d].frame == f) {
                need_render = true;
                break;
            }
        }

        g_frame_number++;
        uint16_t ctr_before = snes_bus_read16(&g_ctx, PROBE_FRAME_CTR);
        snes_step_frame(&g_ctx, pad1, pad2, need_render ? g_framebuf : NULL);
        bool logic_ran = snes_bus_read16(&g_ctx, PROBE_FRAME_CTR) != ctr_before;
        if (logic_ran) {
            logic_done++;
        } else if (opt->movie_logic && this_movie_frame >= 0) {
            movie_pos--;
            this_movie_frame = -1;
        }
        contra3_extras_apply(&g_ctx, &g_extras);
        snes_movie_record_frame(&rec, pad1, pad2, flags);

        snes_apu_clear_samples(&g_apu);

        for (unsigned d = 0u; d < opt->num_dumps; ++d) {
            if (opt->dumps[d].frame == f) {
                if (snes_write_bmp(opt->dumps[d].path, g_framebuf, SNES_SCREEN_WIDTH, SNES_SCREEN_HEIGHT)) {
                    unsigned nonblack = 0u;
                    for (unsigned px = 0u; px < SNES_SCREEN_WIDTH * SNES_SCREEN_HEIGHT; ++px) {
                        if ((g_framebuf[px] & 0x00FFFFFFu) != 0u) {
                            nonblack++;
                        }
                    }
                    printf("[DUMP] Frame %4u -> %s (state=$%02X, NMI_mode=$%02X, nonblack_px=%u/%u)\n",
                           f, opt->dumps[d].path,
                           snes_bus_read8(&g_ctx, PROBE_GAME_STATE),
                           snes_bus_read8(&g_ctx, PROBE_NMI_MODE),
                           nonblack, (unsigned)(SNES_SCREEN_WIDTH * SNES_SCREEN_HEIGHT));
                }
            }
        }

        if (trace != NULL || opt->verify) {
            ProbeSample s;
            probe_sample(&g_ctx, &s);
            if (trace != NULL) {
                fprintf(trace, "%u,%lld,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
                        f, (long long)this_movie_frame, pad1, pad2, s.state, s.sub, s.nmi_mode, s.stage,
                        s.frame_ctr, s.p1_x, s.p1_y, s.p2_x, s.p2_y, s.lives1, s.lives2, s.cam_x,
                        s.rng_lo, s.rng_hi, logic_ran ? 0u : 1u, (unsigned)g_ctx.last_nmi_cycles,
                        (unsigned)g_ctx.last_nmi_apu_polls);
            }
            if (opt->verify) {
                if (s.state < 32u) {
                    state_seen |= 1u << s.state;
                }
                if (s.state == 0x09u) {
                    gameplay_frames++;
                    if (!g_ctx.last_frame_lagged && prev.state == 0x09u) {
                        uint32_t c = g_ctx.last_nmi_cycles;
                        nmi_sum += c;
                        nmi_samples++;
                        nmi_max = (c > nmi_max) ? c : nmi_max;
                        nmi_hist[(c / 1024u < 255u) ? c / 1024u : 255u]++;
                        if (c > (g_lag_budget ? g_lag_budget : SNES_LAG_BUDGET_DEFAULT)) {
                            nmi_over++;
                        }
                    }
                    if (s.stage > max_stage) {
                        max_stage = s.stage;
                    }
                }
                if (s.state != prev.state || s.stage != prev.stage || s.nmi_mode != prev.nmi_mode) {
                    printf("[PROGRESS] frame %6u (movie %6lld): state $%02X->$%02X  stage %u  nmi_mode %u  lives %u/%u  unresolved=%u\n",
                           f, (long long)this_movie_frame, prev.state, s.state, s.stage, s.nmi_mode,
                           s.lives1, s.lives2, g_ctx.unresolved_jumps);
                }
                if (s.frame_ctr != prev.frame_ctr) {
                    last_ctr_change = f;
                } else if (opt->hang_frames > 0u && f - last_ctr_change >= opt->hang_frames) {
                    printf("[HANG] logic frame counter $%04X frozen for %u frames at frame %u (state $%02X sub $%02X)\n",
                           s.frame_ctr, opt->hang_frames, f, s.state, s.sub);
                    hang = true;
                    break;
                }
            }
            prev = s;
        }
    }
    unsigned frames_run = (f > total_frames) ? total_frames : (f - 1u);
    if (!movie_done && movie_pos >= 0 && (uint64_t)movie_pos >= mv->frame_count) {
        movie_done = true;
    }
    if (hang) {
        frames_run = f;
    }

    double elapsed_s = now_seconds() - t0;
    double avg_fps   = (elapsed_s > 0.0) ? ((double)frames_run / elapsed_s) : 0.0;

    if (trace != NULL) {
        fclose(trace);
        printf("[TRACE] %s (%u frames)\n", opt->trace_path, frames_run);
    }
    if (rec.fp != NULL) {
        printf("[RECORD] %s (%u frames)\n", opt->record_path, (unsigned)rec.frames);
        snes_movie_record_end(&rec);
    }

    if (opt->wav_path != NULL) {
        g_apu.pcm_sink = NULL;
        int32_t peak_abs = 0;
        size_t  nonzero  = 0u;
        for (size_t i = 0u; i < g_wav.frames * 2u; ++i) {
            int32_t smp = (int32_t)g_wav.data[i];
            if (smp != 0) {
                nonzero++;
                int32_t a = (smp < 0) ? -smp : smp;
                if (a > peak_abs) {
                    peak_abs = a;
                }
            }
        }
        if (g_wav.failed) {
            fprintf(stderr, "Error: out of memory while capturing audio\n");
        } else if (snes_write_wav(opt->wav_path, g_wav.data, g_wav.frames, SNES_AUDIO_SAMPLE_RATE)) {
            printf("[WAV] %s | %zu stereo frames (%.2f s @ %u Hz) | KON=%u | active=%zu | peak=%d\n",
                   opt->wav_path, g_wav.frames,
                   (double)g_wav.frames / (double)SNES_AUDIO_SAMPLE_RATE,
                   SNES_AUDIO_SAMPLE_RATE,
                   g_apu.kon_events, nonzero, (int)peak_abs);
        }
        free(g_wav.data);
        memset(&g_wav, 0, sizeof(g_wav));
    }

    if (mv != NULL) {
        printf("[MOVIE] consumed %lld/%u movie frames (%u emulator lag frames skipped)%s\n",
               (long long)((movie_pos < 0) ? 0 : movie_pos), (unsigned)mv->frame_count, (unsigned)lag_skipped,
               movie_done ? "" : " - movie NOT finished");
        if (movie_done && movie_end_frame != 0u) {
            printf("[MOVIE] input exhausted at frame %u\n", movie_end_frame);
        }
    }
    if (opt->verify) {
        printf("[VERIFY] gameplay frames=%u | highest stage=%u | states visited mask=$%04X | ending/credits state ($0E) %s\n",
               gameplay_frames, max_stage, state_seen, (state_seen & (1u << 0x0Eu)) ? "REACHED" : "not reached");
        if (nmi_samples > 0u) {
            uint64_t target = (nmi_samples * 99u + 99u) / 100u, acc = 0u;
            unsigned p99 = 0u;
            for (unsigned b = 0u; b < 256u; ++b) {
                acc += nmi_hist[b];
                if (acc >= target) {
                    p99 = (b + 1u) * 1024u;
                    break;
                }
            }
            uint32_t budget = g_lag_budget ? g_lag_budget : SNES_LAG_BUDGET_DEFAULT;
            printf("[VERIFY] gameplay CPU load: avg %.0f | p99 <=%u | max %u cycles per NMI | budget %u | over budget %u frames (%.2f%%) | lag frames %u%s\n",
                   (double)nmi_sum / (double)nmi_samples, p99, nmi_max, budget, nmi_over,
                   100.0 * (double)nmi_over / (double)nmi_samples, (unsigned)g_ctx.lag_frames,
                   g_lag_budget ? "" : " (lag model off)");
        }
    }

    printf("[HEADLESS] %u frames in %.3f s (%.1f FPS) | state=$%02X | DMA=%u | KON=%u | unresolved=%u%s\n",
           frames_run, elapsed_s, avg_fps,
           snes_bus_read8(&g_ctx, PROBE_GAME_STATE),
           g_ctx.cpu_io.dma_transfers,
           g_apu.kon_events,
           g_ctx.unresolved_jumps,
           hang ? " | HANG" : "");
    if (g_ctx.unresolved_jumps != 0u) {
        printf("[HEADLESS] last unresolved target: $%06X\n", (unsigned)g_ctx.last_unresolved_addr);
        return 2;
    }
    return hang ? 4 : 0;
}

static void print_usage(void) {
    printf(
        "Usage: contra3 [rom.sfc] [options]\n"
        "  --rom <path>              ROM image (default: $CONTRA3_ROM, then next to the binary, then ../ROM or ../../ROM)\n"
        "  --movie <file>            Play a .smv / .bk2 / Input Log.txt / .c3m movie from power-on\n"
        "  --movie-logic             Advance the movie and --poke frames on logic frames only\n"
        "  --movie-offset <n>        Skip n leading movie frames (negative: insert n idle frames)\n"
        "  --lag-from <file>         Emulator lag flags (trace CSV 'lag' column or frame list) to skip\n"
        "  --record <file.c3m>       Record the input actually fed to the game (headless)\n"
        "  --headless <frames>       Run without a window (0 with --movie = until the movie ends)\n"
        "  --press <a-b:BUTTONS>     Scripted P1 input (START, SELECT, UP, DOWN, LEFT, RIGHT, JUMP, SHOOT, BOMB, X, L, R, 0xMASK)\n"
        "  --press2 <a-b:BUTTONS>    Scripted P2 input\n"
        "  --dump <frame:file.bmp>   Save a rendered frame\n"
        "  --poke <frame:addr=val>   Write a 16-bit hex value to the bus before a frame (debug / stage warp)\n"
        "  --wav <file.wav>          Capture 32 kHz S-DSP output\n"
        "  --trace <file.csv>        Per-frame WRAM probe trace (compare with tools/compare_trace.py)\n"
        "  --verify                  Print state/stage progress, detect hangs, summarize the run\n"
        "  --hang-frames <n>         Frames without a logic-frame tick that count as a hang (default 600)\n"
        "  --strict                  Abort with a register dump on the first unresolved indirect jump\n"
        "  --stage-select            Restore Konami's STAGE SELECT screen (START on the title; F4 toggles)\n"
        "  --start-stage <1-6>       First game started from the title begins at that stage\n"
        "  --infinite-lives-cheat    Lives never drop below 2 during play (Shift+F4)\n"
        "  --30-lives                Start with 30 lives (restored Contra Spirits code effect)\n"
        "  --lag-model [cycles]      Authentic slowdown: drop logic frames when an NMI overruns its CPU budget (F3)\n"
        "  --infinite-lives          Refill a player's lives when they reach the last one (soak testing)\n");
}

int main(int argc, char **argv) {
    const char *rom_path = NULL;
    const char *movie_path = NULL;
    const char *lag_path = NULL;
    bool headless = false;
    bool cli_lag = false, cli_start_stage = false;
    ScriptPress presses[64];
    ScriptDump dumps[64];
    ScriptPoke pokes[64];
    HeadlessOptions opt;
    memset(&opt, 0, sizeof(opt));
    opt.presses = presses;
    opt.dumps = dumps;
    opt.pokes = pokes;
    opt.hang_frames = 600u;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--headless") == 0 && i + 1 < argc) {
            headless = true;
            opt.total_frames = (unsigned)strtoul(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "--rom") == 0 && i + 1 < argc) {
            rom_path = argv[++i];
        } else if (strcmp(argv[i], "--wav") == 0 && i + 1 < argc) {
            opt.wav_path = argv[++i];
        } else if ((strcmp(argv[i], "--press") == 0 || strcmp(argv[i], "--press2") == 0) && i + 1 < argc) {
            uint8_t port = (argv[i][7] == '2') ? 1u : 0u;
            const char *arg = argv[++i];
            unsigned s = 0u, e = 0u;
            char bspec[64] = {0};
            if (opt.num_presses < 64u && sscanf(arg, "%u-%u:%63s", &s, &e, bspec) == 3) {
                presses[opt.num_presses].start_frame = s;
                presses[opt.num_presses].end_frame   = e;
                presses[opt.num_presses].mask        = parse_button_spec(bspec);
                presses[opt.num_presses].port        = port;
                opt.num_presses++;
            }
        } else if (strcmp(argv[i], "--dump") == 0 && i + 1 < argc) {
            const char *arg = argv[++i];
            const char *colon = strchr(arg, ':');
            if (colon != NULL && opt.num_dumps < 64u) {
                dumps[opt.num_dumps].frame = (unsigned)strtoul(arg, NULL, 10);
                dumps[opt.num_dumps].path  = colon + 1;
                opt.num_dumps++;
            }
        } else if (strcmp(argv[i], "--poke") == 0 && i + 1 < argc) {
            unsigned pf = 0u, pa = 0u, pv = 0u;
            if (opt.num_pokes < 64u && sscanf(argv[++i], "%u:%x=%x", &pf, &pa, &pv) == 3) {
                pokes[opt.num_pokes].frame = pf;
                pokes[opt.num_pokes].addr  = pa;
                pokes[opt.num_pokes].value = (uint16_t)pv;
                opt.num_pokes++;
            }
        } else if (strcmp(argv[i], "--movie") == 0 && i + 1 < argc) {
            movie_path = argv[++i];
        } else if (strcmp(argv[i], "--cpu-budget") == 0 && i + 1 < argc) {
            g_cpu_budget = (uint32_t)strtoul(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "--movie-logic") == 0) {
            opt.movie_logic = true;
        } else if (strcmp(argv[i], "--movie-offset") == 0 && i + 1 < argc) {
            opt.movie_offset = (int)strtol(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "--lag-from") == 0 && i + 1 < argc) {
            lag_path = argv[++i];
        } else if (strcmp(argv[i], "--record") == 0 && i + 1 < argc) {
            opt.record_path = argv[++i];
        } else if (strcmp(argv[i], "--trace") == 0 && i + 1 < argc) {
            opt.trace_path = argv[++i];
        } else if (strcmp(argv[i], "--verify") == 0) {
            opt.verify = true;
        } else if (strcmp(argv[i], "--stage-select") == 0) {
            g_extras.flags |= CONTRA3_UNLOCK_STAGE_SELECT;
        } else if (strcmp(argv[i], "--30-lives") == 0) {
            g_extras.flags |= CONTRA3_UNLOCK_LIVES_30;
        } else if (strcmp(argv[i], "--infinite-lives-cheat") == 0) {
            g_extras.flags |= CONTRA3_UNLOCK_INFINITE_LIVES;
        } else if (strcmp(argv[i], "--start-stage") == 0 && i + 1 < argc) {
            unsigned st = (unsigned)strtoul(argv[++i], NULL, 10);
            g_extras.start_stage = (st >= 1u && st <= 6u) ? st : 0u;
            cli_start_stage = true;
        } else if (strcmp(argv[i], "--lag-model") == 0) {
            g_lag_budget = SNES_LAG_BUDGET_DEFAULT;
            cli_lag = true;
            if (i + 1 < argc && argv[i + 1][0] >= '0' && argv[i + 1][0] <= '9') {
                g_lag_budget = (uint32_t)strtoul(argv[++i], NULL, 10);
            }
        } else if (strcmp(argv[i], "--strict") == 0) {
            g_strict_fallback = true;
        } else if (strcmp(argv[i], "--infinite-lives") == 0) {
            opt.infinite_lives = true;
        } else if (strcmp(argv[i], "--hang-frames") == 0 && i + 1 < argc) {
            opt.hang_frames = (unsigned)strtoul(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage();
            return 0;
        } else if (argv[i][0] != '-') {
            rom_path = argv[i];
        } else {
            fprintf(stderr, "Warning: ignoring unknown option '%s' (see --help)\n", argv[i]);
        }
    }

    /* Resolve the ROM: argument, $CONTRA3_ROM, executable-relative locations */
    static char found_rom[512];
    char exe_path[512] = "";
    char exe_dir[512] = ".";
    ssize_t exe_len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1u);
    if (exe_len > 0) {
        exe_path[exe_len] = '\0';
        snes_rom_dirname(exe_path, exe_dir, sizeof(exe_dir));
    } else {
        snes_rom_dirname(argv[0], exe_dir, sizeof(exe_dir));
    }
    if (rom_path == NULL) {
        if (snes_rom_find(exe_dir, found_rom, sizeof(found_rom))) {
            rom_path = found_rom;
        } else {
            fprintf(stderr, "Error: Contra III ROM not found. Pass it as an argument, set CONTRA3_ROM, or place\n"
                            "       '%s' next to the contra3 binary.\n", CONTRA3_ROM_FILENAME);
            return 1;
        }
    }
    size_t rom_size = 0u;
    char rom_err[512];
    uint8_t *rom = snes_rom_load(rom_path, &rom_size, rom_err, sizeof(rom_err));
    if (rom == NULL) {
        fprintf(stderr, "Error: %s\n", rom_err);
        return 1;
    }

    if (movie_path != NULL) {
        char err[256];
        if (!snes_movie_load(movie_path, &g_movie, err, sizeof(err))) {
            fprintf(stderr, "Error: movie '%s': %s\n", movie_path, err);
            free(rom);
            return 1;
        }
        static const char *const fmt_names[] = { "?", "Snes9x SMV", "BizHawk BK2", "BizHawk Input Log", "C3M" };
        printf("[MOVIE] %s: %s, %u frames, %u rerecords%s%s%s\n",
               movie_path, fmt_names[g_movie.format], (unsigned)g_movie.frame_count, (unsigned)g_movie.rerecords,
               g_movie.author[0] ? ", author: " : "", g_movie.author, g_movie.pal ? " [PAL movie on NTSC game!]" : "");
        opt.movie = &g_movie;
        g_movie_active = true;
    }
    uint8_t *lag = NULL;
    if (lag_path != NULL) {
        lag = load_lag_file(lag_path, &opt.lag_count);
        if (lag == NULL) {
            fprintf(stderr, "Error: cannot read lag file '%s'\n", lag_path);
            free(rom);
            snes_movie_free(&g_movie);
            return 1;
        }
        unsigned n = 0u;
        for (uint32_t k = 0u; k < opt.lag_count; ++k) {
            n += lag[k];
        }
        printf("[MOVIE] %u lag frames loaded from %s\n", n, lag_path);
        opt.lag = lag;
    }

    if (headless) {
        int rc = run_headless(rom, rom_size, &opt);
        free(lag);
        snes_movie_free(&g_movie);
        free(rom);
        return rc;
    }

    linux_config_load(&g_config, exe_dir);
    if (!cli_lag) {
        g_lag_budget = g_config.lag_model ? SNES_LAG_BUDGET_DEFAULT : 0u;
    }
    if (g_config.stage_select)   g_extras.flags |= CONTRA3_UNLOCK_STAGE_SELECT;
    if (g_config.lives_30)       g_extras.flags |= CONTRA3_UNLOCK_LIVES_30;
    if (g_config.infinite_lives) g_extras.flags |= CONTRA3_UNLOCK_INFINITE_LIVES;
    if (!cli_start_stage)        g_extras.start_stage = g_config.start_stage;
    const Contra3LinuxConfig config_at_start = g_config;
    const unsigned extras_at_start = g_extras.flags;
    const uint32_t start_lag = g_lag_budget;

    linux_audio_init();
    power_on(rom, rom_size);
    g_movie_frame = (opt.movie_offset > 0) ? (uint32_t)opt.movie_offset : 0u;
    if (g_config.have_options && !g_movie_active) {
        Contra3Options o = { g_config.opt_lives, g_config.opt_difficulty, g_config.opt_sound };
        contra3_options_restore(&g_ctx, &o, &g_extras);
    }

    linux_video_init();

    const long frame_ns = (long)(1e9 / 60.09881389744051);
    struct timespec next_tick;
    clock_gettime(CLOCK_MONOTONIC, &next_tick);
    double fps_timer = now_seconds();
    unsigned frames_this_sec = 0u;

    while (g_running) {
        linux_poll_events();
        if (!g_running) {
            break;
        }

        bool turbo = g_turbo_toggle || (g_focused && key_down(XK_Tab));

        if ((!g_paused || g_step_once)) {
            uint16_t pad1, pad2;
            uint8_t  flags = g_rec_pending_flags;
            g_rec_pending_flags = 0u;
            if (g_movie_active && g_movie_frame < g_movie.frame_count) {
                pad1 = snes_movie_pad(&g_movie, g_movie_frame, 0u);
                pad2 = snes_movie_pad(&g_movie, g_movie_frame, 1u);
                uint8_t mflags = snes_movie_flags(&g_movie, g_movie_frame);
                flags |= mflags;
                g_movie_frame++;
                if ((mflags & SNES_MOVIE_FLAG_POWER) != 0u) {
                    power_on(rom, rom_size);
                    linux_audio_reset();
                } else if ((mflags & SNES_MOVIE_FLAG_RESET) != 0u) {
                    soft_reset();
                }
            } else {
                g_movie_active = false;
                pad1 = g_focused ? linux_config_poll(&g_config, 0u, g_dpy) : 0u;
                pad2 = g_focused ? linux_config_poll(&g_config, 1u, g_dpy) : 0u;
            }
            g_frame_number++;
            snes_step_frame(&g_ctx, pad1, pad2, g_framebuf);
            contra3_extras_apply(&g_ctx, &g_extras);
            snes_movie_record_frame(&g_recorder, pad1, pad2, flags);
            linux_audio_pump(turbo, false);
            g_step_once = false;
            frames_this_sec++;
        } else {
            linux_audio_pump(false, true);
        }

        linux_video_present();

        double now = now_seconds();
        if (now - fps_timer >= 1.0) {
            double fps = (double)frames_this_sec / (now - fps_timer);
            char movie_note[48] = "";
            if (g_movie_active) {
                snprintf(movie_note, sizeof(movie_note), " [MOVIE %u/%u]",
                         (unsigned)g_movie_frame, (unsigned)g_movie.frame_count);
            } else if (g_recorder.fp != NULL) {
                snprintf(movie_note, sizeof(movie_note), " [REC %u]", (unsigned)g_recorder.frames);
            }
            char title[512];
            snprintf(title, sizeof(title),
                     "Contra III: The Alien Wars [C11 Recomp] | %.1f FPS%s%s%s%s | State:$%02X | Unresolved:%u",
                     fps,
                     g_paused ? " [PAUSED]" : (turbo ? " [TURBO]" : ""),
                     g_audio_muted ? " [MUTED]" : "",
                     g_audio_ok ? "" : " [NO AUDIO]",
                     movie_note,
                     snes_bus_read8(&g_ctx, PROBE_GAME_STATE),
                     g_ctx.unresolved_jumps);
            if (g_status[0] != '\0' && now < g_status_until) {
                size_t len = strlen(title);
                snprintf(title + len, sizeof(title) - len, " | %s", g_status);
            } else {
                g_status[0] = '\0';
            }
            XStoreName(g_dpy, g_win, title);
            fps_timer = now;
            frames_this_sec = 0u;
        }

        if (!turbo) {
            next_tick.tv_nsec += frame_ns;
            while (next_tick.tv_nsec >= 1000000000L) {
                next_tick.tv_nsec -= 1000000000L;
                next_tick.tv_sec++;
            }
            struct timespec current;
            clock_gettime(CLOCK_MONOTONIC, &current);
            if (current.tv_sec > next_tick.tv_sec ||
                (current.tv_sec == next_tick.tv_sec && current.tv_nsec >= next_tick.tv_nsec)) {
                double drift = (double)(current.tv_sec - next_tick.tv_sec) +
                               (double)(current.tv_nsec - next_tick.tv_nsec) / 1e9;
                if (drift > 4.0 * (double)frame_ns / 1e9) {
                    next_tick = current;
                }
            } else {
                clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_tick, NULL);
            }
        } else {
            clock_gettime(CLOCK_MONOTONIC, &next_tick);
        }
    }

    {
        if (g_extras.flags != extras_at_start) {
            g_config.stage_select   = (g_extras.flags & CONTRA3_UNLOCK_STAGE_SELECT) != 0u;
            g_config.infinite_lives = (g_extras.flags & CONTRA3_UNLOCK_INFINITE_LIVES) != 0u;
        }
        if ((g_lag_budget != 0u) != (start_lag != 0u)) {
            g_config.lag_model = g_lag_budget != 0u;
        }
        if (g_recorder.fp == NULL && !g_movie_active) {
            Contra3Options o;
            contra3_options_read(&g_ctx, &o);
            bool lives_cheat = (g_extras.flags & CONTRA3_UNLOCK_LIVES_30) != 0u;
            if (o.lives >= 1u && o.lives <= 99u && (o.sound == 0u || o.sound == 2u) && o.difficulty <= 0x0Eu) {
                if (!lives_cheat) {
                    g_config.opt_lives = o.lives;
                } else if (!g_config.have_options) {
                    g_config.opt_lives = 3u;
                }
                g_config.opt_difficulty = o.difficulty;
                g_config.opt_sound = o.sound;
                g_config.have_options = true;
            }
        }
        if (memcmp(&g_config, &config_at_start, sizeof(g_config)) != 0) {
            linux_config_save(&g_config);
        }
    }
    snes_movie_record_end(&g_recorder);
    linux_audio_shutdown();
    linux_video_shutdown();
    free(lag);
    snes_movie_free(&g_movie);
    free(rom);
    return 0;
}
