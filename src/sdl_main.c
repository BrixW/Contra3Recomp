/*
 * Contra III: The Alien Wars (USA) - Cross-platform SDL2 frontend (Linux, macOS, Windows)
 *
 * Shares everything portable with the Win32 runner: the recompiled game, the preemptible main
 * thread, contra3.ini (c3_config), the in-game overlay (c3_overlay), controller rumble
 * (contra3_events + c3_rumble), the extras and the OpenGL 3.3 presenter (gl_present). SDL
 * provides the window, GL context, audio, keyboard and game controllers. When OpenGL 3.3 is
 * not available the frame is shown through an SDL_Renderer streaming texture instead.
 *
 * Controls are the same as the Win32 build (see README); Esc or Back+Start opens the overlay.
 */

#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "contra3_recomp.h"
#include "snes_ppu.h"
#include "snes_apu.h"
#include "snes_movie.h"
#include "snes_savestate.h"
#include "snes_rom.h"
#include "snes_mainthread.h"
#include "contra3_extras.h"
#include "contra3_events.h"
#include "c3_config.h"
#include "c3_overlay.h"
#include "c3_rumble.h"
#include "gl_present.h"

#define SCREEN_W SNES_SCREEN_WIDTH
#define SCREEN_H SNES_SCREEN_HEIGHT

static SnesCpuContext    g_ctx;
static SnesApu           g_apu;
static uint32_t          g_frame[SCREEN_W * SCREEN_H];
static uint32_t          g_overlay_buf[SCREEN_W * SCREEN_H];
static Contra3Config     g_cfg;
static Contra3Extras     g_extras = { 0u, 0u, false, -1, 0u };
static Contra3EventState g_events;
static C3Rumble          g_rumble;
static C3Overlay         g_overlay;
static char              g_overlay_status[64];

static SDL_Window       *g_window;
static SDL_GLContext     g_gl;
static bool              g_use_gl;
static SDL_Renderer     *g_renderer;
static SDL_Texture      *g_texture;
static SDL_AudioDeviceID g_audio;
static SDL_GameController *g_pads[2];

static bool     g_running = true, g_paused = false, g_step = false, g_muted = false, g_turbo = false;
static bool     g_fullscreen = false, g_vsync = false, g_config_dirty = false;
static unsigned g_cli_extras = 0u;
static int      g_cli_start_stage = -1;
static uint32_t g_lag_cycles = SNES_LAG_BUDGET_DEFAULT;
static char     g_state_path[1024];
static char     g_status[128];
static uint32_t g_status_until;
static unsigned g_shot = 1u;

/* ------------------------------------------------------------------------- */
/* Keyboard: C3Key <-> SDL scancode                                          */
/* ------------------------------------------------------------------------- */

static const SDL_Scancode k_sc[C3K_COUNT] = {
    [C3K_A] = SDL_SCANCODE_A, [C3K_B] = SDL_SCANCODE_B, [C3K_C] = SDL_SCANCODE_C, [C3K_D] = SDL_SCANCODE_D,
    [C3K_E] = SDL_SCANCODE_E, [C3K_F] = SDL_SCANCODE_F, [C3K_G] = SDL_SCANCODE_G, [C3K_H] = SDL_SCANCODE_H,
    [C3K_I] = SDL_SCANCODE_I, [C3K_J] = SDL_SCANCODE_J, [C3K_K] = SDL_SCANCODE_K, [C3K_L] = SDL_SCANCODE_L,
    [C3K_M] = SDL_SCANCODE_M, [C3K_N] = SDL_SCANCODE_N, [C3K_O] = SDL_SCANCODE_O, [C3K_P] = SDL_SCANCODE_P,
    [C3K_Q] = SDL_SCANCODE_Q, [C3K_R] = SDL_SCANCODE_R, [C3K_S] = SDL_SCANCODE_S, [C3K_T] = SDL_SCANCODE_T,
    [C3K_U] = SDL_SCANCODE_U, [C3K_V] = SDL_SCANCODE_V, [C3K_W] = SDL_SCANCODE_W, [C3K_X] = SDL_SCANCODE_X,
    [C3K_Y] = SDL_SCANCODE_Y, [C3K_Z] = SDL_SCANCODE_Z,
    [C3K_0] = SDL_SCANCODE_0, [C3K_1] = SDL_SCANCODE_1, [C3K_2] = SDL_SCANCODE_2, [C3K_3] = SDL_SCANCODE_3,
    [C3K_4] = SDL_SCANCODE_4, [C3K_5] = SDL_SCANCODE_5, [C3K_6] = SDL_SCANCODE_6, [C3K_7] = SDL_SCANCODE_7,
    [C3K_8] = SDL_SCANCODE_8, [C3K_9] = SDL_SCANCODE_9,
    [C3K_UP] = SDL_SCANCODE_UP, [C3K_DOWN] = SDL_SCANCODE_DOWN, [C3K_LEFT] = SDL_SCANCODE_LEFT, [C3K_RIGHT] = SDL_SCANCODE_RIGHT,
    [C3K_ENTER] = SDL_SCANCODE_RETURN, [C3K_SPACE] = SDL_SCANCODE_SPACE, [C3K_TAB] = SDL_SCANCODE_TAB,
    [C3K_BACKSPACE] = SDL_SCANCODE_BACKSPACE, [C3K_ESCAPE] = SDL_SCANCODE_ESCAPE,
    [C3K_LSHIFT] = SDL_SCANCODE_LSHIFT, [C3K_RSHIFT] = SDL_SCANCODE_RSHIFT, [C3K_LCTRL] = SDL_SCANCODE_LCTRL,
    [C3K_RCTRL] = SDL_SCANCODE_RCTRL, [C3K_LALT] = SDL_SCANCODE_LALT, [C3K_RALT] = SDL_SCANCODE_RALT,
    [C3K_INSERT] = SDL_SCANCODE_INSERT, [C3K_DELETE] = SDL_SCANCODE_DELETE, [C3K_HOME] = SDL_SCANCODE_HOME,
    [C3K_END] = SDL_SCANCODE_END, [C3K_PAGEUP] = SDL_SCANCODE_PAGEUP, [C3K_PAGEDOWN] = SDL_SCANCODE_PAGEDOWN,
    [C3K_NUM0] = SDL_SCANCODE_KP_0, [C3K_NUM1] = SDL_SCANCODE_KP_1, [C3K_NUM2] = SDL_SCANCODE_KP_2,
    [C3K_NUM3] = SDL_SCANCODE_KP_3, [C3K_NUM4] = SDL_SCANCODE_KP_4, [C3K_NUM5] = SDL_SCANCODE_KP_5,
    [C3K_NUM6] = SDL_SCANCODE_KP_6, [C3K_NUM7] = SDL_SCANCODE_KP_7, [C3K_NUM8] = SDL_SCANCODE_KP_8,
    [C3K_NUM9] = SDL_SCANCODE_KP_9, [C3K_NUMADD] = SDL_SCANCODE_KP_PLUS, [C3K_NUMSUB] = SDL_SCANCODE_KP_MINUS,
    [C3K_NUMMUL] = SDL_SCANCODE_KP_MULTIPLY, [C3K_NUMDIV] = SDL_SCANCODE_KP_DIVIDE, [C3K_NUMDOT] = SDL_SCANCODE_KP_PERIOD,
    [C3K_NUMENTER] = SDL_SCANCODE_KP_ENTER,
    [C3K_COMMA] = SDL_SCANCODE_COMMA, [C3K_PERIOD] = SDL_SCANCODE_PERIOD, [C3K_SLASH] = SDL_SCANCODE_SLASH,
    [C3K_SEMICOLON] = SDL_SCANCODE_SEMICOLON, [C3K_QUOTE] = SDL_SCANCODE_APOSTROPHE,
    [C3K_LBRACKET] = SDL_SCANCODE_LEFTBRACKET, [C3K_RBRACKET] = SDL_SCANCODE_RIGHTBRACKET,
    [C3K_MINUS] = SDL_SCANCODE_MINUS, [C3K_EQUALS] = SDL_SCANCODE_EQUALS, [C3K_BACKSLASH] = SDL_SCANCODE_BACKSLASH,
    [C3K_GRAVE] = SDL_SCANCODE_GRAVE,
    [C3K_F1] = SDL_SCANCODE_F1, [C3K_F2] = SDL_SCANCODE_F2, [C3K_F3] = SDL_SCANCODE_F3, [C3K_F4] = SDL_SCANCODE_F4,
    [C3K_F5] = SDL_SCANCODE_F5, [C3K_F6] = SDL_SCANCODE_F6, [C3K_F7] = SDL_SCANCODE_F7, [C3K_F8] = SDL_SCANCODE_F8,
    [C3K_F9] = SDL_SCANCODE_F9, [C3K_F10] = SDL_SCANCODE_F10, [C3K_F11] = SDL_SCANCODE_F11, [C3K_F12] = SDL_SCANCODE_F12,
};

static C3Key sc_to_c3(SDL_Scancode sc) {
    for (int k = 1; k < C3K_COUNT; ++k) {
        if (k_sc[k] == sc && sc != SDL_SCANCODE_UNKNOWN) {
            return (C3Key)k;
        }
    }
    return C3K_NONE;
}

static bool sdl_key_down(C3Key key, void *user) {
    const Uint8 *state = (const Uint8 *)user;
    return key > C3K_NONE && key < C3K_COUNT && k_sc[key] != SDL_SCANCODE_UNKNOWN && state[k_sc[key]];
}

/* ------------------------------------------------------------------------- */
/* Helpers                                                                   */
/* ------------------------------------------------------------------------- */

static void set_status(const char *msg) {
    snprintf(g_status, sizeof(g_status), "%s", msg);
    g_status_until = SDL_GetTicks() + 4000u;
}

static void power_on(const uint8_t *rom, size_t rom_size) {
    snes_mainthread_release(&g_ctx);
    snes_context_init(&g_ctx, rom, rom_size);
    snes_apu_init(&g_apu);
    snes_apu_attach(&g_ctx, &g_apu);
    g_ctx.lag_budget = g_cfg.lag_model ? g_lag_cycles : 0u;
    snes_power_on(&g_ctx);
}

static void apply_config(void) {
    if (g_vsync != g_cfg.vsync) {
        g_vsync = g_cfg.vsync;
        if (g_use_gl) {
            SDL_GL_SetSwapInterval(g_vsync ? 1 : 0);
        }
    }
    uint32_t lag = g_cfg.lag_model ? g_lag_cycles : 0u;
    if (lag != g_ctx.lag_budget) {
        g_ctx.lag_budget = lag;
        g_ctx.lag_debt = 0u;
    }
    g_extras.flags = g_cli_extras |
                     (g_cfg.stage_select   ? CONTRA3_UNLOCK_STAGE_SELECT : 0u) |
                     (g_cfg.lives_30       ? CONTRA3_UNLOCK_LIVES_30 : 0u) |
                     (g_cfg.infinite_lives ? CONTRA3_UNLOCK_INFINITE_LIVES : 0u);
    g_extras.start_stage = (g_cli_start_stage >= 0) ? (unsigned)g_cli_start_stage : g_cfg.start_stage;
}

static C3GlFilter cfg_filter(void) {
    return (strcmp(g_cfg.filter, "crt") == 0) ? C3GL_CRT : (strcmp(g_cfg.filter, "nearest") == 0) ? C3GL_NEAREST : C3GL_SHARP;
}

static void toggle_fullscreen(void) {
    g_fullscreen = !g_fullscreen;
    SDL_SetWindowFullscreen(g_window, g_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

static void rumble_send(bool zero) {
    for (unsigned p = 0u; p < 2u; ++p) {
        if (g_pads[p] != NULL && (zero || g_rumble.changed[p])) {
            Uint16 lo = zero ? 0u : g_rumble.out_low[p], hi = zero ? 0u : g_rumble.out_high[p];
            SDL_GameControllerRumble(g_pads[p], lo, hi, (lo || hi) ? 100u : 0u);
        }
    }
}

static void audio_clear(void) {
    if (g_audio != 0u) {
        SDL_ClearQueuedAudio(g_audio);
    }
    snes_apu_clear_samples(&g_apu);
}

static void audio_pump(bool turbo) {
    static int16_t buf[4096 * 2];
    size_t n;
    if (g_audio == 0u || g_muted || turbo) {
        snes_apu_clear_samples(&g_apu);
        return;
    }
    while ((n = snes_apu_pop_samples(&g_apu, buf, 4096u)) > 0u) {
        if (g_cfg.volume < 100u) {
            for (size_t i = 0u; i < n * 2u; ++i) {
                buf[i] = (int16_t)((int32_t)buf[i] * (int32_t)g_cfg.volume / 100);
            }
        }
        /* Keep latency bounded (~100 ms); long loads produce bursts of real elapsed audio */
        if (SDL_GetQueuedAudioSize(g_audio) < 32000u * 4u / 10u) {
            SDL_QueueAudio(g_audio, buf, (Uint32)(n * 4u));
        }
    }
}

static void open_pads(void) {
    for (int i = 0, slot = 0; i < SDL_NumJoysticks() && slot < 2; ++i) {
        if (!SDL_IsGameController(i)) {
            continue;
        }
        while (slot < 2 && g_pads[slot] != NULL) {
            slot++;
        }
        if (slot >= 2) {
            break;
        }
        bool already = false;
        SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(i);
        for (int p = 0; p < 2; ++p) {
            if (g_pads[p] != NULL && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(g_pads[p])) == id) {
                already = true;
            }
        }
        if (!already) {
            g_pads[slot] = SDL_GameControllerOpen(i);
        }
    }
}

static uint16_t pad_state(unsigned p, bool raw[C3P_COUNT], int *lx, int *ly, bool dpad[4]) {
    SDL_GameController *c = g_pads[p];
    memset(raw, 0, sizeof(bool) * C3P_COUNT);
    memset(dpad, 0, sizeof(bool) * 4u);
    *lx = *ly = 0;
    if (c == NULL) {
        return 0u;
    }
    raw[C3P_A]  = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_A);
    raw[C3P_B]  = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_B);
    raw[C3P_X]  = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_X);
    raw[C3P_Y]  = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_Y);
    raw[C3P_LB] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    raw[C3P_RB] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    raw[C3P_LT] = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 8000;
    raw[C3P_RT] = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 8000;
    raw[C3P_BACK]  = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_BACK);
    raw[C3P_START] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_START);
    *lx = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX);
    *ly = -SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY);   /* SDL: down is positive */
    dpad[0] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_UP);
    dpad[1] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    dpad[2] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    dpad[3] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    uint16_t m = 0u;
    int dz = g_cfg.pad_deadzone;
    if (dpad[0] || *ly >  dz) m |= SNES_BTN_UP;
    if (dpad[1] || *ly < -dz) m |= SNES_BTN_DOWN;
    if (dpad[2] || *lx < -dz) m |= SNES_BTN_LEFT;
    if (dpad[3] || *lx >  dz) m |= SNES_BTN_RIGHT;
    return (uint16_t)(m | c3_config_map_pad(&g_cfg, raw));
}

static void overlay_open(C3OverlayPage page) {
    c3_overlay_open_at(&g_overlay, page);
    g_overlay_status[0] = '\0';
    audio_clear();
    rumble_send(true);
}

static void overlay_closed(void) {
    if (g_overlay.dirty) {
        c3_config_save(&g_cfg);
        g_overlay.dirty = false;
    }
}

static void soft_reset(void) {
    audio_clear();
    snes_soft_reset(&g_ctx);
}

static void overlay_action(C3OverlayAction act) {
    switch (act) {
        case C3ACT_RESUME: overlay_closed(); break;
        case C3ACT_SAVE_STATE: {
            SnesSavestateResult r = snes_savestate_save(g_state_path, &g_ctx, &g_apu);
            snprintf(g_overlay_status, sizeof(g_overlay_status), "%s", r == SNES_SAVESTATE_OK ? "STATE SAVED" : "SAVE FAILED - TRY AGAIN");
            break;
        }
        case C3ACT_LOAD_STATE:
            if (snes_savestate_load(g_state_path, &g_ctx, &g_apu) == SNES_SAVESTATE_OK) {
                c3_overlay_close(&g_overlay);
                overlay_closed();
                set_status("State loaded");
            } else {
                snprintf(g_overlay_status, sizeof(g_overlay_status), "NO USABLE SAVE STATE");
            }
            break;
        case C3ACT_RESET: overlay_closed(); soft_reset(); break;
        case C3ACT_QUIT: overlay_closed(); g_running = false; break;
        case C3ACT_TEST_RUMBLE: c3_rumble_test(&g_rumble, 0u); c3_rumble_test(&g_rumble, 1u); break;
        case C3ACT_TOGGLE_FULLSCREEN: toggle_fullscreen(); break;
        default: break;
    }
    if (g_overlay.mute_request) {
        g_overlay.mute_request = false;
        g_muted = !g_muted;
        audio_clear();
    }
}

static void handle_key(const SDL_KeyboardEvent *e) {
    bool repeat = e->repeat != 0;
    SDL_Scancode sc = e->keysym.scancode;
    if (g_overlay.open) {
        if (c3_overlay_capturing(&g_overlay)) {
            if (!repeat) c3_overlay_capture_key(&g_overlay, &g_cfg, sc_to_c3(sc));
            return;
        }
        int nav = -1;
        switch (sc) {
            case SDL_SCANCODE_UP:     nav = C3NAV_UP; break;
            case SDL_SCANCODE_DOWN:   nav = C3NAV_DOWN; break;
            case SDL_SCANCODE_LEFT:   nav = C3NAV_LEFT; break;
            case SDL_SCANCODE_RIGHT:  nav = C3NAV_RIGHT; break;
            case SDL_SCANCODE_RETURN: case SDL_SCANCODE_KP_ENTER: case SDL_SCANCODE_SPACE: nav = repeat ? -1 : C3NAV_OK; break;
            case SDL_SCANCODE_ESCAPE: case SDL_SCANCODE_BACKSPACE: nav = repeat ? -1 : C3NAV_BACK; break;
            default: break;
        }
        if (nav >= 0) overlay_action(c3_overlay_nav(&g_overlay, &g_cfg, (C3Nav)nav));
        return;
    }
    if (repeat) {
        return;
    }
    bool shift = (e->keysym.mod & KMOD_SHIFT) != 0, ctrl = (e->keysym.mod & KMOD_CTRL) != 0;
    switch (sc) {
        case SDL_SCANCODE_ESCAPE: overlay_open(C3OV_MAIN); break;
        case SDL_SCANCODE_F2: overlay_open(shift ? C3OV_KEYS_P2 : C3OV_KEYS_P1); break;
        case SDL_SCANCODE_P: case SDL_SCANCODE_PAUSE: g_paused = !g_paused; audio_clear(); break;
        case SDL_SCANCODE_M: g_muted = !g_muted; audio_clear(); break;
        case SDL_SCANCODE_SPACE: if (g_paused) g_step = true; break;
        case SDL_SCANCODE_F1: g_turbo = !g_turbo; break;
        case SDL_SCANCODE_F3:
            g_cfg.lag_model = !g_cfg.lag_model; g_config_dirty = true;
            set_status(g_cfg.lag_model ? "Authentic slowdown ON" : "Authentic slowdown OFF");
            break;
        case SDL_SCANCODE_F4:
            if (shift) { g_cfg.infinite_lives = !g_cfg.infinite_lives; set_status(g_cfg.infinite_lives ? "Infinite lives ON" : "Infinite lives OFF"); }
            else { g_cfg.stage_select = !g_cfg.stage_select; set_status(g_cfg.stage_select ? "Stage select ON" : "Stage select OFF"); }
            g_config_dirty = true;
            break;
        case SDL_SCANCODE_F5: {
            SnesSavestateResult r = snes_savestate_save(g_state_path, &g_ctx, &g_apu);
            set_status(r == SNES_SAVESTATE_OK ? "State saved" : snes_savestate_result_str(r));
            break;
        }
        case SDL_SCANCODE_F7: {
            audio_clear();
            SnesSavestateResult r = snes_savestate_load(g_state_path, &g_ctx, &g_apu);
            set_status(r == SNES_SAVESTATE_OK ? "State loaded" : snes_savestate_result_str(r));
            break;
        }
        case SDL_SCANCODE_F8:
            snes_bus_write16(&g_ctx, 0x001E3Cu, 0x001Eu);
            snes_bus_write16(&g_ctx, 0x001F8Au, 0x001Eu);
            break;
        case SDL_SCANCODE_F9: {
            static const char *const next[] = { "nearest", "crt", "sharp" };
            snprintf(g_cfg.filter, sizeof(g_cfg.filter), "%s", next[cfg_filter()]);
            g_config_dirty = true;
            break;
        }
        case SDL_SCANCODE_F10: g_cfg.integer_scale = !g_cfg.integer_scale; g_config_dirty = true; break;
        case SDL_SCANCODE_F11: toggle_fullscreen(); break;
        case SDL_SCANCODE_RETURN: if (e->keysym.mod & KMOD_ALT) toggle_fullscreen(); break;
        case SDL_SCANCODE_F12: {
            char name[64];
            snprintf(name, sizeof(name), "contra3_shot_%04u.bmp", g_shot++);
            snes_write_bmp(name, g_frame, SCREEN_W, SCREEN_H);
            set_status(name);
            break;
        }
        case SDL_SCANCODE_R: if (ctrl) soft_reset(); break;
        default: break;
    }
}

static void present(void) {
    const uint32_t *src = g_overlay.open ? g_overlay_buf : g_frame;
    if (g_use_gl) {
        int w, h;
        SDL_GL_GetDrawableSize(g_window, &w, &h);
        c3gl_present(src, SCREEN_W, SCREEN_H, w, h, g_cfg.integer_scale, cfg_filter());
        SDL_GL_SwapWindow(g_window);
    } else {
        SDL_UpdateTexture(g_texture, NULL, src, SCREEN_W * 4);
        SDL_RenderClear(g_renderer);
        SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);
        SDL_RenderPresent(g_renderer);
    }
}

static void *sdl_gl_proc(const char *name) {
    return SDL_GL_GetProcAddress(name);
}

static bool dir_writable(const char *dir) {
    char probe[1100];
    snprintf(probe, sizeof(probe), "%s/.contra3_write_test", dir);
    FILE *fp = fopen(probe, "w");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);
    remove(probe);
    return true;
}

static void usage(void) {
    printf("Usage: contra3 [rom.sfc] [options]\n"
           "  --rom <path>             ROM image (default: $CONTRA3_ROM, next to the executable, ../ROM)\n"
           "  --filter <sharp|nearest|crt>  --integer  --vsync  --fullscreen  --software (no OpenGL)\n"
           "  --lag-model [cycles]     authentic slowdown\n"
           "  --stage-select  --30-lives  --infinite-lives-cheat  --start-stage <1-6>\n"
           "  --movie <file>           play a .smv / .bk2 / .c3m movie from power-on\n"
           "Headless verification tools (--headless, --trace, --verify, ...) are in the Win32 runner and tests/.\n");
}

int main(int argc, char **argv) {
    const char *rom_path = NULL, *movie_path = NULL;
    bool force_software = false, start_fullscreen = false;
    const char *cli_filter = NULL;
    bool cli_integer = false, cli_vsync = false, cli_lag = false;
    for (int i = 1; i < argc; ++i) {
        const char *a = argv[i];
        if (!strcmp(a, "--rom") && i + 1 < argc) rom_path = argv[++i];
        else if (!strcmp(a, "--filter") && i + 1 < argc) cli_filter = argv[++i];
        else if (!strcmp(a, "--integer")) cli_integer = true;
        else if (!strcmp(a, "--vsync")) cli_vsync = true;
        else if (!strcmp(a, "--fullscreen")) start_fullscreen = true;
        else if (!strcmp(a, "--software")) force_software = true;
        else if (!strcmp(a, "--lag-model")) {
            cli_lag = true;
            if (i + 1 < argc && argv[i + 1][0] >= '0' && argv[i + 1][0] <= '9') g_lag_cycles = (uint32_t)strtoul(argv[++i], NULL, 10);
        }
        else if (!strcmp(a, "--stage-select")) g_cli_extras |= CONTRA3_UNLOCK_STAGE_SELECT;
        else if (!strcmp(a, "--30-lives")) g_cli_extras |= CONTRA3_UNLOCK_LIVES_30;
        else if (!strcmp(a, "--infinite-lives-cheat")) g_cli_extras |= CONTRA3_UNLOCK_INFINITE_LIVES;
        else if (!strcmp(a, "--start-stage") && i + 1 < argc) { int s = atoi(argv[++i]); g_cli_start_stage = (s >= 1 && s <= 6) ? s : 0; }
        else if (!strcmp(a, "--movie") && i + 1 < argc) movie_path = argv[++i];
        else if (!strcmp(a, "--help") || !strcmp(a, "-h")) { usage(); return 0; }
        else if (a[0] != '-') rom_path = a;
        else fprintf(stderr, "Warning: ignoring unknown option '%s'\n", a);
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    /* Executable folder for ROM discovery; settings/states there if writable, else the user pref dir */
    char base[1024] = ".";
    char *bp = SDL_GetBasePath();
    if (bp != NULL) {
        snprintf(base, sizeof(base), "%s", bp);
        size_t l = strlen(base);
        if (l > 1u && (base[l - 1] == '/' || base[l - 1] == '\\')) base[l - 1] = '\0';
        SDL_free(bp);
    }
    char data_dir[1024];
    snprintf(data_dir, sizeof(data_dir), "%s", base);
    if (!dir_writable(base)) {
        char *pp = SDL_GetPrefPath("Contra3Recomp", "Contra3");
        if (pp != NULL) {
            snprintf(data_dir, sizeof(data_dir), "%s", pp);
            size_t l = strlen(data_dir);
            if (l > 1u && (data_dir[l - 1] == '/' || data_dir[l - 1] == '\\')) data_dir[l - 1] = '\0';
            SDL_free(pp);
        }
    }
    snprintf(g_state_path, sizeof(g_state_path), "%s/contra3_quicksave.state", data_dir);

    static char found[1024];
    if (rom_path == NULL) {
        if (!snes_rom_find(base, found, sizeof(found))) {
            const char *msg = "Contra III ROM not found.\nPass it as an argument, set CONTRA3_ROM, or place\n"
                              "'Contra III - The Alien Wars (USA).sfc' next to the executable.";
            fprintf(stderr, "%s\n", msg);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Contra III", msg, NULL);
            SDL_Quit();
            return 1;
        }
        rom_path = found;
    }
    size_t rom_size = 0u;
    char err[512];
    uint8_t *rom = snes_rom_load(rom_path, &rom_size, err, sizeof(err));
    if (rom == NULL) {
        fprintf(stderr, "Error: %s\n", err);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Contra III - ROM error", err, NULL);
        SDL_Quit();
        return 1;
    }

    c3_config_load(&g_cfg, data_dir);
    if (cli_filter != NULL) snprintf(g_cfg.filter, sizeof(g_cfg.filter), "%s", cli_filter);
    if (cli_integer) g_cfg.integer_scale = true;
    if (cli_vsync) g_cfg.vsync = true;
    if (cli_lag) g_cfg.lag_model = true;
    const Contra3Config cfg_at_start = g_cfg;

    SnesMovie movie;
    memset(&movie, 0, sizeof(movie));
    bool movie_active = false;
    uint32_t movie_frame = 0u;
    if (movie_path != NULL) {
        if (!snes_movie_load(movie_path, &movie, err, sizeof(err))) {
            fprintf(stderr, "Error: movie '%s': %s\n", movie_path, err);
            free(rom);
            SDL_Quit();
            return 1;
        }
        movie_active = true;
    }

    /* Window + OpenGL 3.3 Core, falling back to SDL_Renderer */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);   /* required for 3.3 Core on macOS */
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    Uint32 wflags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | (force_software ? 0u : SDL_WINDOW_OPENGL);
    g_window = SDL_CreateWindow("Contra III: The Alien Wars", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_W * 3, SCREEN_H * 3, wflags);
    if (g_window == NULL && !force_software) {
        force_software = true;
        g_window = SDL_CreateWindow("Contra III: The Alien Wars", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    SCREEN_W * 3, SCREEN_H * 3, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    }
    if (g_window == NULL) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        free(rom);
        SDL_Quit();
        return 1;
    }
    if (!force_software) {
        g_gl = SDL_GL_CreateContext(g_window);
        g_use_gl = g_gl != NULL && c3gl_init(sdl_gl_proc);
        if (g_use_gl) {
            printf("[GL] %s\n", c3gl_renderer());
        } else if (g_gl != NULL) {
            SDL_GL_DeleteContext(g_gl);
            g_gl = NULL;
        }
    }
    if (!g_use_gl) {
        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_PRESENTVSYNC);
        if (g_renderer == NULL) g_renderer = SDL_CreateRenderer(g_window, -1, 0);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        SDL_RenderSetLogicalSize(g_renderer, SCREEN_W, SCREEN_H);
        g_texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_W, SCREEN_H);
        printf("[VIDEO] SDL_Renderer fallback (%s)\n", SDL_GetCurrentVideoDriver());
    }
    g_vsync = !g_cfg.vsync;  /* force apply_config to set the swap interval */
    if (start_fullscreen) toggle_fullscreen();

    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = 32000;   /* the S-DSP's native rate; SDL resamples if the device needs it */
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 1024;
    g_audio = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (g_audio != 0u) SDL_PauseAudioDevice(g_audio, 0);

    open_pads();
    power_on(rom, rom_size);
    if (g_cfg.have_options && !movie_active) {
        Contra3Options o = { g_cfg.opt_lives, g_cfg.opt_difficulty, g_cfg.opt_sound };
        contra3_options_restore(&g_ctx, &o, &g_extras);
    }
    contra3_events_reset(&g_events);
    c3_rumble_reset(&g_rumble);
    c3_overlay_init(&g_overlay);

    const double frame_s = 1.0 / 60.09881389744051;
    const Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 next = SDL_GetPerformanceCounter();
    Uint64 fps_t0 = next;
    unsigned fps_frames = 0u;
    bool prev_chord[2] = { false, false };
    int held_dir = -1, repeat_timer = 0;
    bool prev_ok[2] = { false, false }, prev_back[2] = { false, false };

    while (g_running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) g_running = false;
            else if (e.type == SDL_KEYDOWN) handle_key(&e.key);
            else if (e.type == SDL_CONTROLLERDEVICEADDED) open_pads();
            else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
                for (int p = 0; p < 2; ++p) {
                    if (g_pads[p] != NULL && !SDL_GameControllerGetAttached(g_pads[p])) {
                        SDL_GameControllerClose(g_pads[p]);
                        g_pads[p] = NULL;
                    }
                }
            }
        }
        if (!g_running) break;

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        bool turbo = g_turbo || keys[SDL_SCANCODE_TAB];
        bool focused = (SDL_GetWindowFlags(g_window) & SDL_WINDOW_INPUT_FOCUS) != 0;

        uint16_t pads[2];
        int dir = -1;
        for (unsigned p = 0u; p < 2u; ++p) {
            bool raw[C3P_COUNT], dpad[4];
            int lx, ly;
            pads[p] = pad_state(p, raw, &lx, &ly, dpad);
            bool chord = raw[C3P_BACK] && raw[C3P_START];
            if (chord && !prev_chord[p]) {
                if (g_overlay.open) { c3_overlay_close(&g_overlay); overlay_closed(); }
                else overlay_open(C3OV_MAIN);
            }
            prev_chord[p] = chord;
            if (g_overlay.open && !c3_overlay_capturing(&g_overlay)) {
                if (raw[C3P_A] && !prev_ok[p]) overlay_action(c3_overlay_nav(&g_overlay, &g_cfg, C3NAV_OK));
                if (raw[C3P_B] && !prev_back[p]) overlay_action(c3_overlay_nav(&g_overlay, &g_cfg, C3NAV_BACK));
                if (dpad[0] || ly >  16000) dir = C3NAV_UP;
                if (dpad[1] || ly < -16000) dir = C3NAV_DOWN;
                if (dpad[2] || lx < -16000) dir = C3NAV_LEFT;
                if (dpad[3] || lx >  16000) dir = C3NAV_RIGHT;
            }
            prev_ok[p] = raw[C3P_A];
            prev_back[p] = raw[C3P_B];
        }
        if (g_overlay.open && dir >= 0) {
            if (dir != held_dir) { held_dir = dir; repeat_timer = 18; overlay_action(c3_overlay_nav(&g_overlay, &g_cfg, (C3Nav)dir)); }
            else if (--repeat_timer <= 0) { repeat_timer = 5; overlay_action(c3_overlay_nav(&g_overlay, &g_cfg, (C3Nav)dir)); }
        } else {
            held_dir = -1;
        }

        apply_config();
        unsigned events = 0u;
        if (g_overlay.open) {
            C3OverlayEnv env = { g_use_gl, g_fullscreen, g_muted, g_overlay_status };
            c3_overlay_render(&g_overlay, &g_cfg, &env, g_frame, g_overlay_buf);
            snes_apu_clear_samples(&g_apu);
        } else if (!g_paused || g_step) {
            uint16_t p1, p2;
            if (movie_active && movie_frame < movie.frame_count) {
                p1 = snes_movie_pad(&movie, movie_frame, 0u);
                p2 = snes_movie_pad(&movie, movie_frame, 1u);
                movie_frame++;
            } else {
                movie_active = false;
                p1 = (uint16_t)((focused ? c3_config_keys_to_pad(&g_cfg, 0u, sdl_key_down, (void *)keys) : 0u) | pads[0]);
                p2 = (uint16_t)((focused ? c3_config_keys_to_pad(&g_cfg, 1u, sdl_key_down, (void *)keys) : 0u) | pads[1]);
            }
            snes_step_frame(&g_ctx, p1, p2, g_frame);
            contra3_extras_apply(&g_ctx, &g_extras);
            events = contra3_events_poll(&g_ctx, &g_events, p1, p2);
            audio_pump(turbo);
            g_step = false;
            fps_frames++;
        } else {
            snes_apu_clear_samples(&g_apu);
        }
        c3_rumble_frame(&g_rumble, events, g_cfg.rumble_strength, g_cfg.rumble && focused);
        rumble_send(false);
        present();

        Uint64 now = SDL_GetPerformanceCounter();
        if (now - fps_t0 >= freq) {
            char title[256];
            snprintf(title, sizeof(title), "Contra III: The Alien Wars | %.1f FPS%s%s | %s%s",
                     (double)fps_frames * (double)freq / (double)(now - fps_t0),
                     g_overlay.open ? " [MENU]" : g_paused ? " [PAUSED]" : (turbo ? " [TURBO]" : ""), g_muted ? " [MUTED]" : "",
                     g_use_gl ? c3gl_filter_name(cfg_filter()) : "SDL renderer",
                     (SDL_TICKS_PASSED(SDL_GetTicks(), g_status_until) || g_status[0] == '\0') ? "" : " | ");
            if (!SDL_TICKS_PASSED(SDL_GetTicks(), g_status_until) && g_status[0] != '\0') {
                size_t l = strlen(title);
                snprintf(title + l, sizeof(title) - l, "%s", g_status);
            }
            SDL_SetWindowTitle(g_window, title);
            fps_t0 = now;
            fps_frames = 0u;
        }
        if (!turbo) {
            next += (Uint64)(frame_s * (double)freq);
            now = SDL_GetPerformanceCounter();
            if (now < next) {
                Uint64 ms = (next - now) * 1000u / freq;
                if (ms > 2u) SDL_Delay((Uint32)(ms - 1u));
                while (SDL_GetPerformanceCounter() < next) {
                }
            } else if (now - next > (Uint64)(4.0 * frame_s * (double)freq)) {
                next = now;
            }
        } else {
            next = SDL_GetPerformanceCounter();
        }
    }

    /* Persist OPTION MODE values and changed settings */
    if (!movie_active) {
        Contra3Options o;
        contra3_options_read(&g_ctx, &o);
        if (o.lives >= 1u && o.lives <= 99u && (o.sound == 0u || o.sound == 2u) && o.difficulty <= 0x0Eu) {
            if (!(g_extras.flags & CONTRA3_UNLOCK_LIVES_30)) g_cfg.opt_lives = o.lives;
            else if (!g_cfg.have_options) g_cfg.opt_lives = 3u;
            g_cfg.opt_difficulty = o.difficulty;
            g_cfg.opt_sound = o.sound;
            g_cfg.have_options = true;
        }
    }
    if (g_config_dirty || memcmp(&g_cfg, &cfg_at_start, sizeof(g_cfg)) != 0) {
        c3_config_save(&g_cfg);
    }
    rumble_send(true);
    snes_mainthread_release(&g_ctx);
    if (g_audio != 0u) SDL_CloseAudioDevice(g_audio);
    for (int p = 0; p < 2; ++p) if (g_pads[p] != NULL) SDL_GameControllerClose(g_pads[p]);
    if (g_use_gl) { c3gl_shutdown(); SDL_GL_DeleteContext(g_gl); }
    if (g_texture != NULL) SDL_DestroyTexture(g_texture);
    if (g_renderer != NULL) SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    snes_movie_free(&movie);
    free(rom);
    SDL_Quit();
    return 0;
}
