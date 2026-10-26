/*
 * Contra III: The Alien Wars (USA) - Native Win32 Interactive Runner & Headless Harness
 *
 * Interactive Controls (Player 1 Keyboard + XInput Gamepad 0/1):
 *   Arrow Keys        : D-Pad (Move / Aim)
 *   Z or J            : B (Jump)
 *   X or K            : Y (Shoot)
 *   A or U            : X (Switch Weapon)
 *   S or I            : A (Mega Bomb)
 *   Q / E             : L / R Shoulder (Lock Aim / Dual Fire)
 *   Enter             : Start
 *   Right Shift / BS  : Select
 *
 * Hotkeys:
 *   Tab (hold) / F1   : Fast-Forward (Uncapped Turbo)
 *   P                 : Pause / Resume (Space steps 1 frame while paused)
 *   M                 : Mute / Unmute 32 kHz Stereo Audio
 *   F5 / F7           : Quick Save / Quick Load State (contra3_quicksave.state)
 *   F6                : Start / Stop Input Movie Recording (contra3_rec_XXXX.c3m)
 *   F8                : Grant 30 Lives (1P + default config)
 *   F9                : Cycle OpenGL Filter (Sharp-Bilinear / Nearest / CRT)
 *   F10               : Toggle Integer Scaling (OpenGL)
 *   F11 / Alt+Enter   : Toggle Borderless Fullscreen
 *   F12               : Save BMP Screenshot (contra3_shot_XXXX.bmp)
 *   Ctrl+R            : Soft Reset
 *   Esc               : Quit
 *
 * Presentation: OpenGL 3.3 Core (src/win32_gl.c) by default, GDI StretchDIBits fallback (--gdi).
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <commdlg.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "contra3_recomp.h"
#include "snes_ppu.h"
#include "snes_apu.h"
#include "snes_movie.h"
#include "snes_savestate.h"
#include "snes_mainthread.h"
#include "snes_rom.h"
#include "contra3_extras.h"
#include "win32_gl.h"
#include "win32_config.h"

/* Minimal XInput structures for zero-dependency dynamic controller support */
typedef struct Win32XInputGamepad {
    WORD  wButtons;
    BYTE  bLeftTrigger;
    BYTE  bRightTrigger;
    SHORT sThumbLX;
    SHORT sThumbLY;
    SHORT sThumbRX;
    SHORT sThumbRY;
} Win32XInputGamepad;

typedef struct Win32XInputState {
    DWORD              dwPacketNumber;
    Win32XInputGamepad Gamepad;
} Win32XInputState;

typedef DWORD (WINAPI *PFN_XInputGetState)(DWORD dwUserIndex, Win32XInputState *pState);

#define XINPUT_BTN_DPAD_UP        0x0001u
#define XINPUT_BTN_DPAD_DOWN      0x0002u
#define XINPUT_BTN_DPAD_LEFT      0x0004u
#define XINPUT_BTN_DPAD_RIGHT     0x0008u
#define XINPUT_BTN_START          0x0010u
#define XINPUT_BTN_BACK           0x0020u
#define XINPUT_BTN_LEFT_SHOULDER  0x0100u
#define XINPUT_BTN_RIGHT_SHOULDER 0x0200u
#define XINPUT_BTN_A              0x1000u
#define XINPUT_BTN_B              0x2000u
#define XINPUT_BTN_X              0x4000u
#define XINPUT_BTN_Y              0x8000u

/*
 * Low-latency Win32 waveOut 48 kHz 16-bit Stereo Streaming Ring
 * Upsamples native Sony S-DSP 32,000 Hz stereo by exact 2:3 ratio (682 -> 1023 frames = 21.3 ms/block)
 * via 4-tap Catmull-Rom cubic interpolation to match SNES analog DAC reconstruction and avoid MME aliasing.
 */
#define AUDIO_HOST_SAMPLE_RATE    48000u
#define AUDIO_NUM_BLOCKS          8u
#define AUDIO_IN_PAIR_COUNT       341u
#define AUDIO_IN_BLOCK_FRAMES     (AUDIO_IN_PAIR_COUNT * 2u)   /* 682 frames @ 32 kHz */
#define AUDIO_BLOCK_FRAMES        (AUDIO_IN_PAIR_COUNT * 3u)   /* 1023 frames @ 48 kHz */

static PFN_XInputGetState g_xinput_get_state = NULL;
static SnesCpuContext     g_ctx;
static SnesApu            g_apu;
static uint32_t           g_framebuf[SNES_SCREEN_WIDTH * SNES_SCREEN_HEIGHT];
static HWAVEOUT           g_waveout      = NULL;
static WAVEHDR            g_wave_hdrs[AUDIO_NUM_BLOCKS];
static int16_t            g_wave_bufs[AUDIO_NUM_BLOCKS][AUDIO_BLOCK_FRAMES * 2u];
static int16_t            g_resample_hist[3][2]; /* [h0, h1, h2][L/R] @ 32 kHz */
static unsigned           g_wave_next    = 0u;
static bool               g_audio_muted  = false;
static bool               g_running      = true;
static bool               g_paused       = false;
static bool               g_step_once    = false;
static bool               g_turbo_toggle = false;
static bool               g_fullscreen   = false;
static WINDOWPLACEMENT    g_prev_wp;
static unsigned           g_shot_index   = 1u;

static bool               g_use_gl        = false;
static bool               g_vsync         = false;
static bool               g_integer_scale = false;
static Win32GlFilter      g_gl_filter     = WIN32_GL_FILTER_SHARP;

static SnesMovie          g_movie;
static bool               g_movie_active  = false;
static uint32_t           g_movie_frame   = 0u;
static SnesMovieRecorder  g_recorder;
static unsigned           g_rec_index     = 1u;
static uint8_t            g_rec_pending_flags = 0u; /* Reset flag merged into the next recorded frame */

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

static void win32_audio_init(void) {
    WAVEFORMATEX wfx;
    memset(&wfx, 0, sizeof(wfx));
    wfx.wFormatTag      = WAVE_FORMAT_PCM;
    wfx.nChannels       = 2u;
    wfx.nSamplesPerSec  = AUDIO_HOST_SAMPLE_RATE;
    wfx.wBitsPerSample  = 16u;
    wfx.nBlockAlign     = (WORD)((wfx.nChannels * wfx.wBitsPerSample) / 8u);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * (DWORD)wfx.nBlockAlign;

    memset(g_resample_hist, 0, sizeof(g_resample_hist));
    if (waveOutOpen(&g_waveout, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
        g_waveout = NULL;
        return;
    }

    for (unsigned i = 0u; i < AUDIO_NUM_BLOCKS; ++i) {
        memset(&g_wave_hdrs[i], 0, sizeof(WAVEHDR));
        g_wave_hdrs[i].lpData         = (LPSTR)g_wave_bufs[i];
        g_wave_hdrs[i].dwBufferLength = (DWORD)(AUDIO_BLOCK_FRAMES * 4u);
        waveOutPrepareHeader(g_waveout, &g_wave_hdrs[i], sizeof(WAVEHDR));
    }
    g_wave_next = 0u;
}

static void win32_audio_pump(bool turbo, bool paused) {
    if (g_waveout == NULL || g_audio_muted || paused) {
        snes_apu_clear_samples(&g_apu);
        memset(g_resample_hist, 0, sizeof(g_resample_hist));
        return;
    }

    unsigned in_queue = 0u;
    for (unsigned i = 0u; i < AUDIO_NUM_BLOCKS; ++i) {
        if ((g_wave_hdrs[i].dwFlags & WHDR_INQUEUE) != 0u) {
            in_queue++;
        }
    }

    /* Pre-buffer 3 blocks (~64 ms) on startup/unpause to prevent WASAPI shared-mode underruns */
    if (in_queue == 0u && !turbo && g_apu.pcm_count < AUDIO_IN_BLOCK_FRAMES * 3u) {
        return;
    }

    unsigned max_queued = turbo ? 3u : 6u;
    int16_t raw_32k[AUDIO_IN_BLOCK_FRAMES * 2u];
    while (in_queue < max_queued && g_apu.pcm_count >= AUDIO_IN_BLOCK_FRAMES) {
        WAVEHDR *hdr = &g_wave_hdrs[g_wave_next];
        if ((hdr->dwFlags & WHDR_INQUEUE) != 0u) {
            break;
        }
        size_t n = snes_apu_pop_samples(&g_apu, raw_32k, AUDIO_IN_BLOCK_FRAMES);
        if (n < AUDIO_IN_BLOCK_FRAMES) {
            break;
        }
        upsample_32k_to_48k(raw_32k, g_wave_bufs[g_wave_next], AUDIO_IN_PAIR_COUNT);
        hdr->dwBufferLength = (DWORD)(AUDIO_BLOCK_FRAMES * 4u);
        hdr->dwFlags &= ~(DWORD)WHDR_DONE;
        if (waveOutWrite(g_waveout, hdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
            break;
        }
        g_wave_next = (g_wave_next + 1u) % AUDIO_NUM_BLOCKS;
        in_queue++;
    }

    /* Drop excess only during uncapped turbo or extreme OS sleep stalls */
    if (turbo && g_apu.pcm_count > AUDIO_IN_BLOCK_FRAMES * 2u) {
        snes_apu_clear_samples(&g_apu);
    } else if (!turbo && g_apu.pcm_count > AUDIO_IN_BLOCK_FRAMES * 6u) {
        int16_t discard[512 * 2];
        snes_apu_pop_samples(&g_apu, discard, 512u);
    }
}

static void win32_audio_shutdown(void) {
    if (g_waveout != NULL) {
        waveOutReset(g_waveout);
        for (unsigned i = 0u; i < AUDIO_NUM_BLOCKS; ++i) {
            if ((g_wave_hdrs[i].dwFlags & WHDR_PREPARED) != 0u) {
                waveOutUnprepareHeader(g_waveout, &g_wave_hdrs[i], sizeof(WAVEHDR));
            }
        }
        waveOutClose(g_waveout);
        g_waveout = NULL;
    }
}

static void init_xinput(void) {
    static const char *const dll_names[] = {
        "xinput1_4.dll",
        "xinput1_3.dll",
        "xinput9_1_0.dll"
    };
    for (size_t i = 0u; i < sizeof(dll_names) / sizeof(dll_names[0]); ++i) {
        HMODULE mod = LoadLibraryA(dll_names[i]);
        if (mod != NULL) {
            FARPROC proc = GetProcAddress(mod, "XInputGetState");
            if (proc != NULL) {
                memcpy(&g_xinput_get_state, &proc, sizeof(g_xinput_get_state));
                return;
            }
        }
    }
}

/* Ask for the ROM interactively when none was given and none was found (desktop launch) */
static bool prompt_for_rom(char *out, size_t out_len) {
    OPENFILENAMEA ofn;
    memset(&ofn, 0, sizeof(ofn));
    out[0] = '\0';
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "SNES ROM (*.sfc;*.smc)\0*.sfc;*.smc\0All files\0*.*\0";
    ofn.lpstrFile   = out;
    ofn.nMaxFile    = (DWORD)out_len;
    ofn.lpstrTitle  = "Select Contra III: The Alien Wars (USA) ROM";
    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    return GetOpenFileNameA(&ofn) != 0;
}

/* Short-lived status line shown in the title bar (save/load results, recording) */
static char  g_status[128];
static DWORD g_status_until = 0u;

static void set_status(HWND hwnd, const char *msg) {
    snprintf(g_status, sizeof(g_status), "%s", msg);
    g_status_until = GetTickCount() + 4000u;
    SetWindowTextA(hwnd, msg);
}

static Contra3Config g_config;

static uint16_t poll_xinput_pad(DWORD index) {
    if (g_xinput_get_state == NULL) {
        return 0u;
    }
    Win32XInputState st;
    memset(&st, 0, sizeof(st));
    if (g_xinput_get_state(index, &st) != ERROR_SUCCESS) {
        return 0u;
    }
    /* D-pad and left stick always steer; face buttons, shoulders, triggers, Back/Start follow [gamepad] */
    uint16_t mask = 0u;
    WORD b = st.Gamepad.wButtons;
    int dz = g_config.pad_deadzone;
    if ((b & XINPUT_BTN_DPAD_UP)    || st.Gamepad.sThumbLY >  dz) mask |= SNES_BTN_UP;
    if ((b & XINPUT_BTN_DPAD_DOWN)  || st.Gamepad.sThumbLY < -dz) mask |= SNES_BTN_DOWN;
    if ((b & XINPUT_BTN_DPAD_LEFT)  || st.Gamepad.sThumbLX < -dz) mask |= SNES_BTN_LEFT;
    if ((b & XINPUT_BTN_DPAD_RIGHT) || st.Gamepad.sThumbLX >  dz) mask |= SNES_BTN_RIGHT;
    mask |= contra3_config_map_pad(&g_config, b, st.Gamepad.bLeftTrigger, st.Gamepad.bRightTrigger);
    return mask;
}

static uint16_t poll_keyboard_pad(HWND hwnd, unsigned player) {
    if (GetForegroundWindow() != hwnd) {
        return 0u;
    }
    return contra3_config_poll(&g_config, player);
}

static void toggle_fullscreen(HWND hwnd) {
    DWORD style = (DWORD)GetWindowLongPtrA(hwnd, GWL_STYLE);
    if (!g_fullscreen) {
        MONITORINFO mi;
        memset(&mi, 0, sizeof(mi));
        mi.cbSize = sizeof(mi);
        g_prev_wp.length = sizeof(g_prev_wp);
        if (GetWindowPlacement(hwnd, &g_prev_wp) &&
            GetMonitorInfoA(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
            SetWindowLongPtrA(hwnd, GWL_STYLE, (LONG_PTR)(style & ~WS_OVERLAPPEDWINDOW));
            SetWindowPos(hwnd, HWND_TOP,
                         mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            g_fullscreen = true;
        }
    } else {
        SetWindowLongPtrA(hwnd, GWL_STYLE, (LONG_PTR)(style | WS_OVERLAPPEDWINDOW));
        SetWindowPlacement(hwnd, &g_prev_wp);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        g_fullscreen = false;
    }
}

static void present_framebuffer(HWND hwnd, HDC hdc) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    int win_w = rc.right - rc.left;
    int win_h = rc.bottom - rc.top;
    if (win_w <= 0 || win_h <= 0) {
        return;
    }

    if (g_use_gl) {
        win32_gl_present(hdc, g_framebuf, SNES_SCREEN_WIDTH, SNES_SCREEN_HEIGHT,
                         win_w, win_h, g_integer_scale, g_gl_filter);
        return;
    }

    /* Preserve crisp 4:3 CRT / 8:7 aspect ratio centered inside window */
    int dst_w = win_w;
    int dst_h = (win_w * SNES_SCREEN_HEIGHT) / SNES_SCREEN_WIDTH;
    if (dst_h > win_h) {
        dst_h = win_h;
        dst_w = (win_h * SNES_SCREEN_WIDTH) / SNES_SCREEN_HEIGHT;
    }
    int dst_x = (win_w - dst_w) / 2;
    int dst_y = (win_h - dst_h) / 2;

    if (dst_x > 0) {
        PatBlt(hdc, 0, 0, dst_x, win_h, BLACKNESS);
        PatBlt(hdc, dst_x + dst_w, 0, win_w - (dst_x + dst_w), win_h, BLACKNESS);
    }
    if (dst_y > 0) {
        PatBlt(hdc, 0, 0, win_w, dst_y, BLACKNESS);
        PatBlt(hdc, 0, dst_y + dst_h, win_w, win_h - (dst_y + dst_h), BLACKNESS);
    }

    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = SNES_SCREEN_WIDTH;
    bmi.bmiHeader.biHeight      = -SNES_SCREEN_HEIGHT; /* Top-down DIB */
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    SetStretchBltMode(hdc, COLORONCOLOR);
    StretchDIBits(hdc, dst_x, dst_y, dst_w, dst_h,
                  0, 0, SNES_SCREEN_WIDTH, SNES_SCREEN_HEIGHT,
                  g_framebuf, &bmi, DIB_RGB_COLORS, SRCCOPY);
}

static void soft_reset(SnesCpuContext *ctx, SnesApu *apu) {
    snes_apu_clear_samples(apu);
    snes_soft_reset(ctx);
}

static bool     g_strict_fallback = false;
static uint32_t g_lag_budget      = 0u;   /* CPU cycles per frame for the lag model (0 = off) */
static uint32_t g_cpu_budget      = 0u;   /* timing-only CPU cycles per frame override (0 = default) */
static Contra3Extras g_extras = { 0u, 0u, false, -1, 0u }; /* restored Contra Spirits features + host extras */

/* Interactive key rebinding (F2 = Player 1, Shift+F2 = Player 2) */
static int      g_rebind_player = -1;
static unsigned g_rebind_button = 0u;
static uint8_t  g_rebind_backup[CFG_BUTTONS][CFG_KEYS_PER_BTN];
static char     g_rebind_prompt[200];

static void rebind_prompt(HWND hwnd) {
    char cur[48] = "";
    for (unsigned k = 0u; k < CFG_KEYS_PER_BTN && g_config.keys[g_rebind_player][g_rebind_button][k] != 0u; ++k) {
        char name[16];
        contra3_config_key_name(g_config.keys[g_rebind_player][g_rebind_button][k], name, sizeof(name));
        size_t len = strlen(cur);
        snprintf(cur + len, sizeof(cur) - len, "%s%s", k ? ", " : "", name);
    }
    snprintf(g_rebind_prompt, sizeof(g_rebind_prompt),
             "Player %d controls %u/%u: press a key for %s  [now: %s]  (Esc = keep, F2 = cancel)",
             g_rebind_player + 1, g_rebind_button + 1u, (unsigned)CFG_BUTTONS,
             contra3_config_button_label(g_rebind_button), cur[0] ? cur : "none");
    SetWindowTextA(hwnd, g_rebind_prompt);
}

static void rebind_start(HWND hwnd, int player) {
    g_rebind_player = player;
    g_rebind_button = 0u;
    memcpy(g_rebind_backup, g_config.keys[player], sizeof(g_rebind_backup));
    rebind_prompt(hwnd);
}

/* Distinguish left/right Shift, Ctrl and Alt, which WM_KEYDOWN reports generically */
static uint8_t resolve_vk(WPARAM wparam, LPARAM lparam) {
    bool extended = (lparam & (1 << 24)) != 0;
    if (wparam == VK_SHIFT) {
        return (uint8_t)MapVirtualKeyA((UINT)((lparam >> 16) & 0xFF), MAPVK_VSC_TO_VK_EX);
    }
    if (wparam == VK_CONTROL) {
        return extended ? VK_RCONTROL : VK_LCONTROL;
    }
    if (wparam == VK_MENU) {
        return extended ? VK_RMENU : VK_LMENU;
    }
    return (uint8_t)wparam;
}

/* Returns true when the key was consumed by the rebinding flow */
static bool rebind_handle_key(HWND hwnd, WPARAM wparam, LPARAM lparam) {
    if (g_rebind_player < 0) {
        return false;
    }
    if (wparam == VK_F2) {
        memcpy(g_config.keys[g_rebind_player], g_rebind_backup, sizeof(g_rebind_backup));
        g_rebind_player = -1;
        set_status(hwnd, "Key rebinding cancelled");
        return true;
    }
    if (wparam != VK_ESCAPE) {
        uint8_t vk = resolve_vk(wparam, lparam);
        if (!contra3_config_key_supported(vk) || (vk >= VK_F1 && vk <= VK_F24)) {
            SetWindowTextA(hwnd, "That key can't be bound - press another (Esc = keep, F2 = cancel)");
            return true;
        }
        /* One key per button from the wizard; a key moves away from any other button of this player */
        for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
            for (unsigned k = 0u; k < CFG_KEYS_PER_BTN; ++k) {
                if (g_config.keys[g_rebind_player][b][k] == vk) {
                    memmove(&g_config.keys[g_rebind_player][b][k], &g_config.keys[g_rebind_player][b][k + 1u],
                            CFG_KEYS_PER_BTN - 1u - k);
                    g_config.keys[g_rebind_player][b][CFG_KEYS_PER_BTN - 1u] = 0u;
                    k--;
                }
            }
        }
        memset(g_config.keys[g_rebind_player][g_rebind_button], 0, CFG_KEYS_PER_BTN);
        g_config.keys[g_rebind_player][g_rebind_button][0] = vk;
    }
    if (++g_rebind_button >= CFG_BUTTONS) {
        char msg[96];
        bool saved = contra3_config_save(&g_config);
        snprintf(msg, sizeof(msg), "Player %d controls %s", g_rebind_player + 1,
                 saved ? "saved to contra3.ini" : "updated (contra3.ini could not be written)");
        g_rebind_player = -1;
        set_status(hwnd, msg);
    } else {
        rebind_prompt(hwnd);
    }
    return true;
}
static uint32_t g_frame_number    = 0u;   /* Frames stepped since launch (for diagnostics) */

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

static void power_on(SnesCpuContext *ctx, SnesApu *apu, const uint8_t *rom, size_t rom_size) {
    void (*sink)(void *, int16_t, int16_t) = apu->pcm_sink; /* host capture survives power cycles */
    void *sink_user = apu->pcm_sink_user;
    snes_context_init(ctx, rom, rom_size);
    snes_apu_init(apu);
    apu->pcm_sink = sink;
    apu->pcm_sink_user = sink_user;
    snes_apu_attach(ctx, apu);
    ctx->on_interp_fallback = on_unresolved_jump;
    ctx->strict_fallback = g_strict_fallback;
    ctx->lag_budget = g_lag_budget;
    ctx->cpu_budget = g_cpu_budget;
    snes_power_on(ctx);
}

static void toggle_recording(HWND hwnd) {
    char msg[96];
    if (g_recorder.fp != NULL) {
        snprintf(msg, sizeof(msg), "Recording stopped (%u frames)", (unsigned)g_recorder.frames);
        snes_movie_record_end(&g_recorder);
    } else {
        char name[64];
        snprintf(name, sizeof(name), "contra3_rec_%04u.c3m", g_rec_index++);
        /* Movies must be replayable from power-on, so recording always starts with a power cycle */
        if (snes_movie_record_begin(&g_recorder, name, "recorded interactively; starts from power-on")) {
            if (g_waveout != NULL) {
                waveOutReset(g_waveout);
            }
            power_on(&g_ctx, &g_apu, g_ctx.rom, g_ctx.rom_size);
            g_movie_active = false;
            snprintf(msg, sizeof(msg), "Recording to %s (power cycled)", name);
        } else {
            snprintf(msg, sizeof(msg), "Could not create %s", name);
        }
    }
    set_status(hwnd, msg);
}

static LRESULT CALLBACK contra3_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_CLOSE:
            g_running = false;
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            g_running = false;
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            present_framebuffer(hwnd, hdc);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_SYSKEYDOWN:
            if ((lparam & 0x40000000) == 0 && rebind_handle_key(hwnd, wparam, lparam)) {
                return 0;
            }
            if (wparam == VK_RETURN && (lparam & (1 << 29)) != 0) {
                toggle_fullscreen(hwnd);
                return 0;
            }
            break;
        case WM_KEYDOWN: {
            bool repeat = (lparam & 0x40000000) != 0;
            if (!repeat && rebind_handle_key(hwnd, wparam, lparam)) {
                return 0;
            }
            if (!repeat) {
                bool shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
                if (wparam == VK_F2) {
                    rebind_start(hwnd, shift ? 1 : 0);
                } else if (wparam == VK_ESCAPE) {
                    g_running = false;
                    DestroyWindow(hwnd);
                    return 0;
                } else if (wparam == 'P' || wparam == VK_PAUSE) {
                    g_paused = !g_paused;
                    if (g_paused && g_waveout != NULL) {
                        waveOutReset(g_waveout);
                        snes_apu_clear_samples(&g_apu);
                    }
                } else if (wparam == 'M') {
                    g_audio_muted = !g_audio_muted;
                    if (g_audio_muted && g_waveout != NULL) {
                        waveOutReset(g_waveout);
                        snes_apu_clear_samples(&g_apu);
                    }
                } else if (wparam == VK_SPACE && g_paused) {
                    g_step_once = true;
                } else if (wparam == VK_F4 && shift) {
                    g_extras.flags ^= CONTRA3_UNLOCK_INFINITE_LIVES;
                    set_status(hwnd, (g_extras.flags & CONTRA3_UNLOCK_INFINITE_LIVES)
                                         ? "Infinite lives ON" : "Infinite lives OFF");
                } else if (wparam == VK_F4) {
                    g_extras.flags ^= CONTRA3_UNLOCK_STAGE_SELECT;
                    set_status(hwnd, (g_extras.flags & CONTRA3_UNLOCK_STAGE_SELECT)
                                         ? "Stage select ON (press START on the title screen)" : "Stage select OFF");
                } else if (wparam == VK_F3) {
                    g_lag_budget = (g_lag_budget != 0u) ? 0u : SNES_LAG_BUDGET_DEFAULT;
                    g_ctx.lag_budget = g_lag_budget;
                    g_ctx.lag_debt = 0u;
                    set_status(hwnd, g_lag_budget ? "Authentic slowdown ON (lag model)" : "Authentic slowdown OFF");
                } else if (wparam == VK_F1) {
                    g_turbo_toggle = !g_turbo_toggle;
                } else if (wparam == VK_F5) {
                    SnesSavestateResult r = snes_savestate_save("contra3_quicksave.state", &g_ctx, &g_apu);
                    char msg[128];
                    snprintf(msg, sizeof(msg), r == SNES_SAVESTATE_OK ? "State saved" : "Save failed: %s",
                             snes_savestate_result_str(r));
                    set_status(hwnd, msg);
                } else if (wparam == VK_F7) {
                    if (g_waveout != NULL) {
                        waveOutReset(g_waveout);
                    }
                    SnesSavestateResult r = snes_savestate_load("contra3_quicksave.state", &g_ctx, &g_apu);
                    char msg[128];
                    snprintf(msg, sizeof(msg), r == SNES_SAVESTATE_OK ? "State loaded" : "Load refused: %s",
                             snes_savestate_result_str(r));
                    set_status(hwnd, msg);
                } else if (wparam == VK_F8) {
                    /* Grant 30 lives to active 1P ($1F8A) & config ($1E3C) */
                    snes_bus_write16(&g_ctx, 0x001E3Cu, 0x001Eu);
                    snes_bus_write16(&g_ctx, 0x001F8Au, 0x001Eu);
                } else if (wparam == VK_F6) {
                    toggle_recording(hwnd);
                } else if (wparam == VK_F9 && g_use_gl) {
                    g_gl_filter = (Win32GlFilter)((g_gl_filter + 1) % WIN32_GL_FILTER_COUNT);
                } else if (wparam == VK_F10 && g_use_gl) {
                    g_integer_scale = !g_integer_scale;
                } else if (wparam == VK_F11) {
                    toggle_fullscreen(hwnd);
                } else if (wparam == VK_F12) {
                    char name[64];
                    snprintf(name, sizeof(name), "contra3_shot_%04u.bmp", g_shot_index++);
                    snes_write_bmp(name, g_framebuf, SNES_SCREEN_WIDTH, SNES_SCREEN_HEIGHT);
                } else if (wparam == 'R' && (GetKeyState(VK_CONTROL) & 0x8000) != 0) {
                    if (g_waveout != NULL) {
                        waveOutReset(g_waveout);
                    }
                    soft_reset(&g_ctx, &g_apu);
                    g_rec_pending_flags |= SNES_MOVIE_FLAG_RESET;
                }
            }
            break;
        }
        default:
            break;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

/* --- Headless Scripted Verification Mode --- */
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
    unsigned           total_frames;     /* 0 with a movie = run until the movie is exhausted */
    const ScriptPress *presses;
    unsigned           num_presses;
    const ScriptDump  *dumps;
    unsigned           num_dumps;
    const ScriptPoke  *pokes;
    unsigned           num_pokes;
    const char        *wav_path;
    const SnesMovie   *movie;
    int                movie_offset;     /* >0 skips leading movie frames, <0 inserts idle frames */
    const uint8_t     *lag;              /* Emulator lag flag per movie frame (skipped on playback) */
    uint32_t           lag_count;
    const char        *trace_path;
    const char        *record_path;
    bool               verify;
    bool               infinite_lives;   /* Soak testing: keep both players' lives topped up */
    bool               movie_logic;      /* advance the movie (and key pokes) on logic frames only */
    unsigned           hang_frames;
} HeadlessOptions;

/* WRAM probes shared by --trace, --verify and tools/emu_trace.lua (keep the three in sync) */
#define PROBE_GAME_STATE   0x000032u  /* MainModule_Dispatch state ($00:A38A, 0x00..0x0E) */
#define PROBE_GAME_SUB     0x000034u  /* Per-state sub-step */
#define PROBE_FRAME_CTR    0x00003Au  /* Logic frame counter (advances once per serviced NMI) */
#define PROBE_NMI_MODE     0x000042u  /* 0 = menus, 2 = side-scroll, 4 = Mode 7 overhead */
#define PROBE_PRNG_LO      0x0000E8u
#define PROBE_PRNG_HI      0x0000EAu
#define PROBE_P1_SLOT      0x000200u  /* Player 1 actor slot (64 bytes) */
#define PROBE_P2_SLOT      0x000240u  /* Player 2 actor slot (64 bytes) */
#define PROBE_SLOT_X       0x06u      /* Screen X */
#define PROBE_SLOT_Y       0x0Eu      /* Screen Y */
#define PROBE_CAM_X        0x001380u  /* BG1 scroll descriptor (camera X) */
#define PROBE_STAGE        0x000086u  /* Stage number 1..6 (stage select $00:A7C8, intro text $00:A836) */
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

/*
 * Load per-frame emulator lag flags: either a trace CSV with a "lag" column
 * (as written by tools/emu_trace.lua) or a plain list of 0-based lag frame numbers.
 */
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

/* Growable 32 kHz stereo capture fed directly by the S-DSP (snes_apu pcm_sink) */
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
    power_on(&g_ctx, &g_apu, rom, rom_size);

    const SnesMovie *mv = opt->movie;
    unsigned total_frames = opt->total_frames;
    if (total_frames == 0u && mv != NULL) {
        /* Run until the movie is consumed; logic-frame playback needs extra video frames for
           boot, loads and lag, so allow up to 4x the movie length */
        int64_t n = (int64_t)mv->frame_count - (int64_t)opt->movie_offset;
        total_frames = (n > 0) ? (unsigned)n * (opt->movie_logic ? 4u : 1u) : 0u;
    }

    /* Lossless capture: long loads can produce seconds of audio inside a single frame */
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

    LARGE_INTEGER freq, t0, t1;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t0);

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
            power_on(&g_ctx, &g_apu, rom, rom_size);
        } else if ((flags & SNES_MOVIE_FLAG_RESET) != 0u) {
            soft_reset(&g_ctx, &g_apu);
        }
        if (opt->infinite_lives) {
            /* Refill only a player who is in the game (lives dropping 1 -> 0); topping up an
               absent P2 would spawn an idle second player that pins the camera. */
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
        /* A logic frame ran iff NMI_Handler reached MainModule_Dispatch, which increments $3A
           (or, in a few init frames, resets it with STZ $3A at $00:ADBF); NMIs are atomic here */
        bool logic_ran = snes_bus_read16(&g_ctx, PROBE_FRAME_CTR) != ctr_before;
        if (logic_ran) {
            logic_done++;
        } else if (opt->movie_logic && this_movie_frame >= 0) {
            movie_pos--;          /* the game did not read this input: offer it again next frame */
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
                        s.rng_lo, s.rng_hi, logic_ran ? 0u : 1u, (unsigned)g_ctx.last_nmi_cycles, (unsigned)g_ctx.last_nmi_apu_polls);
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

    QueryPerformanceCounter(&t1);
    double elapsed_s = (double)(t1.QuadPart - t0.QuadPart) / (double)freq.QuadPart;
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
            /* 99th percentile from a 1024-cycle histogram */
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
    return hang ? 4 : 0; /* 3 is reserved: abort() from --strict exits with 3 on Windows */
}

/*
 * --gl-selftest <prefix>: render a real Stage 1 frame through every OpenGL filter,
 * read the back buffer back, save <prefix>_<filter>.bmp and check that the
 * Nearest filter at exact 3x integer scale reproduces the CPU frame buffer bit-exactly.
 */
static int run_gl_selftest(const uint8_t *rom, size_t rom_size, const char *prefix) {
    power_on(&g_ctx, &g_apu, rom, rom_size);
    for (unsigned f = 1u; f <= 1350u; ++f) {
        uint16_t pad = 0u;
        if ((f >= 620u && f <= 625u) || (f >= 760u && f <= 765u)) pad |= SNES_BTN_START;
        if (f >= 1200u) pad |= SNES_BTN_RIGHT;
        if (f >= 1250u) pad |= SNES_BTN_Y;
        snes_step_frame(&g_ctx, pad, 0u, (f == 1350u) ? g_framebuf : NULL);
        snes_apu_clear_samples(&g_apu);
    }

    WNDCLASSEXA wc;
    memset(&wc, 0, sizeof(wc));
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = DefWindowProcA;
    wc.hInstance     = GetModuleHandleA(NULL);
    wc.lpszClassName = "Contra3GlSelfTest";
    RegisterClassExA(&wc);

    static const struct { int w, h; bool integer; Win32GlFilter filter; const char *tag; } cases[] = {
        { 768, 672, true,  WIN32_GL_FILTER_NEAREST, "nearest_3x" },
        { 768, 672, true,  WIN32_GL_FILTER_SHARP,   "sharp_3x"   },
        { 768, 672, true,  WIN32_GL_FILTER_CRT,     "crt_3x"     },
        { 1000, 700, false, WIN32_GL_FILTER_SHARP,  "sharp_fit_1000x700" },
        { 1000, 700, false, WIN32_GL_FILTER_CRT,    "crt_fit_1000x700"   },
    };

    int rc = 0;
    for (size_t c = 0u; c < sizeof(cases) / sizeof(cases[0]); ++c) {
        RECT wr = { 0, 0, cases[c].w, cases[c].h };
        AdjustWindowRect(&wr, WS_POPUP, FALSE);
        HWND hwnd = CreateWindowExA(WS_EX_TOOLWINDOW, wc.lpszClassName, "contra3 gl selftest", WS_POPUP,
                                    0, 0, wr.right - wr.left, wr.bottom - wr.top,
                                    NULL, NULL, wc.hInstance, NULL);
        if (hwnd == NULL) {
            fprintf(stderr, "[GLTEST] CreateWindow failed\n");
            return 1;
        }
        ShowWindow(hwnd, SW_SHOWNOACTIVATE);
        HDC hdc = GetDC(hwnd);
        if (!win32_gl_init(hwnd, hdc, false)) {
            fprintf(stderr, "[GLTEST] OpenGL 3.3 initialization failed (GDI fallback would be used)\n");
            ReleaseDC(hwnd, hdc);
            DestroyWindow(hwnd);
            return 1;
        }
        if (c == 0u) {
            printf("[GLTEST] %s\n", win32_gl_renderer());
        }

        size_t npx = (size_t)cases[c].w * (size_t)cases[c].h;
        uint32_t *cap = (uint32_t *)calloc(npx, sizeof(uint32_t));
        win32_gl_capture_next(cap);
        win32_gl_present(hdc, g_framebuf, SNES_SCREEN_WIDTH, SNES_SCREEN_HEIGHT,
                         cases[c].w, cases[c].h, cases[c].integer, cases[c].filter);

        unsigned lit = 0u;
        for (size_t i = 0u; i < npx; ++i) {
            if ((cap[i] & 0x00FFFFFFu) != 0u) {
                lit++;
            }
        }
        char path[512];
        snprintf(path, sizeof(path), "%s_%s.bmp", prefix, cases[c].tag);
        snes_write_bmp(path, cap, cases[c].w, cases[c].h);

        if (cases[c].filter == WIN32_GL_FILTER_NEAREST) {
            unsigned diff = 0u;
            for (int y = 0; y < cases[c].h; ++y) {
                for (int x = 0; x < cases[c].w; ++x) {
                    uint32_t want = g_framebuf[(y / 3) * SNES_SCREEN_WIDTH + (x / 3)] & 0x00FFFFFFu;
                    if ((cap[(size_t)y * (size_t)cases[c].w + (size_t)x] & 0x00FFFFFFu) != want) {
                        diff++;
                    }
                }
            }
            printf("[GLTEST] %-20s -> %s | lit=%u/%zu | mismatches vs CPU 3x nearest=%u\n",
                   cases[c].tag, path, lit, npx, diff);
            if (diff != 0u) {
                rc = 1;
            }
        } else {
            printf("[GLTEST] %-20s -> %s | lit=%u/%zu\n", cases[c].tag, path, lit, npx);
            if (lit < npx / 4u) {
                rc = 1;
            }
        }
        free(cap);
        win32_gl_shutdown(hwnd, hdc);
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
    }
    printf("[GLTEST] %s\n", rc == 0 ? "PASS" : "FAIL");
    return rc;
}

static void print_usage(void) {
    printf(
        "Usage: contra3.exe [rom.sfc] [options]\n"
        "  --rom <path>              ROM image (default: $CONTRA3_ROM, then next to the exe, then ../ROM or ../../ROM)\n"
        "  --gdi                     Force the GDI presenter instead of OpenGL 3.3\n"
        "  --vsync                   Enable OpenGL swap-interval VSync\n"
        "  --filter <sharp|nearest|crt>  OpenGL filter (F9 cycles at runtime)\n"
        "  --integer                 Integer scaling (F10 toggles at runtime)\n"
        "  --movie <file>            Play a .smv / .bk2 / Input Log.txt / .c3m movie from power-on\n"
        "  --movie-logic             Advance the movie and --poke frames on logic frames only (tools/refemu equivalence runs)\n"
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
        "  --infinite-lives          Refill a player's lives when they reach the last one (soak testing)\n"
        "  --gl-selftest <prefix>    Render a Stage 1 frame through every OpenGL filter and verify readback\n");
}

int main(int argc, char **argv) {
    const char *rom_path = NULL;
    const char *movie_path = NULL;
    const char *lag_path = NULL;
    bool headless = false;
    bool force_gdi = false;
    bool cli_filter = false, cli_integer = false, cli_vsync = false, cli_lag = false, cli_start_stage = false;
    const char *gl_selftest = NULL;
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
            /* Optional numeric budget may follow: --lag-model 45642 */
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
        } else if (strcmp(argv[i], "--gl-selftest") == 0 && i + 1 < argc) {
            gl_selftest = argv[++i];
        } else if (strcmp(argv[i], "--gdi") == 0) {
            force_gdi = true;
        } else if (strcmp(argv[i], "--vsync") == 0) {
            g_vsync = true;
            cli_vsync = true;
        } else if (strcmp(argv[i], "--integer") == 0) {
            g_integer_scale = true;
            cli_integer = true;
        } else if (strcmp(argv[i], "--filter") == 0 && i + 1 < argc) {
            const char *fl = argv[++i];
            cli_filter = true;
            g_gl_filter = (strcmp(fl, "crt") == 0)     ? WIN32_GL_FILTER_CRT :
                          (strcmp(fl, "nearest") == 0) ? WIN32_GL_FILTER_NEAREST : WIN32_GL_FILTER_SHARP;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage();
            return 0;
        } else if (argv[i][0] != '-') {
            rom_path = argv[i];
        } else {
            fprintf(stderr, "Warning: ignoring unknown option '%s' (see --help)\n", argv[i]);
        }
    }

    /* Resolve the ROM: argument / drag-and-drop, $CONTRA3_ROM, standard locations, then a file dialog */
    static char found_rom[MAX_PATH];
    char exe_path[MAX_PATH], exe_dir[MAX_PATH];
    DWORD exe_len = GetModuleFileNameA(NULL, exe_path, (DWORD)sizeof(exe_path));
    snes_rom_dirname((exe_len > 0u && exe_len < sizeof(exe_path)) ? exe_path : argv[0], exe_dir, sizeof(exe_dir));
    if (rom_path == NULL) {
        bool interactive = !headless && gl_selftest == NULL;
        if (snes_rom_find(exe_dir, found_rom, sizeof(found_rom)) ||
            (interactive && prompt_for_rom(found_rom, sizeof(found_rom)))) {
            rom_path = found_rom;
        } else {
            fprintf(stderr, "Error: Contra III ROM not found. Pass it as an argument, set CONTRA3_ROM, or place\n"
                            "       '%s' next to contra3.exe.\n", CONTRA3_ROM_FILENAME);
            return 1;
        }
    }
    size_t rom_size = 0u;
    char rom_err[512];
    uint8_t *rom = snes_rom_load(rom_path, &rom_size, rom_err, sizeof(rom_err));
    if (rom == NULL) {
        fprintf(stderr, "Error: %s\n", rom_err);
        if (!headless && gl_selftest == NULL) {
            MessageBoxA(NULL, rom_err, "Contra III - ROM error", MB_OK | MB_ICONERROR);
        }
        return 1;
    }

    if (gl_selftest != NULL) {
        int rc = run_gl_selftest(rom, rom_size, gl_selftest);
        free(rom);
        return rc;
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

    contra3_config_load(&g_config, exe_dir);
    if (!cli_filter) {
        g_gl_filter = (strcmp(g_config.filter, "crt") == 0)     ? WIN32_GL_FILTER_CRT :
                      (strcmp(g_config.filter, "nearest") == 0) ? WIN32_GL_FILTER_NEAREST : WIN32_GL_FILTER_SHARP;
    }
    if (!cli_integer) {
        g_integer_scale = g_config.integer_scale;
    }
    if (!cli_vsync) {
        g_vsync = g_config.vsync;
    }
    if (!cli_lag) {
        g_lag_budget = g_config.lag_model ? SNES_LAG_BUDGET_DEFAULT : 0u;
    }
    if (g_config.stage_select)   g_extras.flags |= CONTRA3_UNLOCK_STAGE_SELECT;
    if (g_config.lives_30)       g_extras.flags |= CONTRA3_UNLOCK_LIVES_30;
    if (g_config.infinite_lives) g_extras.flags |= CONTRA3_UNLOCK_INFINITE_LIVES;
    if (!cli_start_stage)        g_extras.start_stage = g_config.start_stage;
    const Contra3Config config_at_start = g_config;
    const unsigned extras_at_start = g_extras.flags;
    const Win32GlFilter start_filter = g_gl_filter;
    const bool start_integer = g_integer_scale;
    const uint32_t start_lag = g_lag_budget;
    init_xinput();
    win32_audio_init();
    power_on(&g_ctx, &g_apu, rom, rom_size);
    g_movie_frame = (opt.movie_offset > 0) ? (uint32_t)opt.movie_offset : 0u;
    /* Saved OPTION MODE settings; skipped for movies, which must start from a pristine power-on */
    if (g_config.have_options && !g_movie_active) {
        Contra3Options o = { g_config.opt_lives, g_config.opt_difficulty, g_config.opt_sound };
        contra3_options_restore(&g_ctx, &o, &g_extras);
    }

    const char *cls_name = "Contra3RecompWndClass";
    WNDCLASSEXA wc;
    memset(&wc, 0, sizeof(wc));
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = contra3_wndproc;
    wc.hInstance     = GetModuleHandleA(NULL);
    wc.hCursor       = LoadCursorA(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = cls_name;
    RegisterClassExA(&wc);

    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT wr = { 0, 0, SNES_SCREEN_WIDTH * 3, SNES_SCREEN_HEIGHT * 3 };
    AdjustWindowRect(&wr, style, FALSE);

    HWND hwnd = CreateWindowExA(
        0, cls_name,
        "Contra III: The Alien Wars (USA) - Static C11 Recompilation",
        style, CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, wc.hInstance, NULL
    );
    if (hwnd == NULL) {
        win32_audio_shutdown();
        free(lag);
        snes_movie_free(&g_movie);
        free(rom);
        return 1;
    }

    HDC hdc = GetDC(hwnd);
    if (!force_gdi) {
        g_use_gl = win32_gl_init(hwnd, hdc, g_vsync);
        if (g_use_gl) {
            printf("[GL] %s\n", win32_gl_renderer());
        }
    }
    /* With VSync on a ~60 Hz display, SwapBuffers paces frames; otherwise QPC pacing stays in charge */
    int refresh_hz = GetDeviceCaps(hdc, VREFRESH);
    bool swap_paced = g_use_gl && g_vsync && win32_gl_vsync_supported() && refresh_hz >= 59 && refresh_hz <= 61;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    timeBeginPeriod(1u);
    LARGE_INTEGER qpc_freq, next_tick, fps_timer;
    QueryPerformanceFrequency(&qpc_freq);
    QueryPerformanceCounter(&next_tick);
    fps_timer = next_tick;
    const LONGLONG frame_ticks = (LONGLONG)((double)qpc_freq.QuadPart / 60.09881389744051);
    unsigned frames_this_sec = 0u;

    while (g_running) {
        MSG msg;
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                g_running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        if (!g_running) {
            break;
        }

        bool turbo = g_turbo_toggle || ((GetForegroundWindow() == hwnd) && ((GetAsyncKeyState(VK_TAB) & 0x8000) != 0));

        if ((!g_paused || g_step_once) && g_rebind_player < 0) {
            uint16_t pad1, pad2;
            uint8_t  flags = g_rec_pending_flags;
            g_rec_pending_flags = 0u;
            if (g_movie_active && g_movie_frame < g_movie.frame_count) {
                pad1   = snes_movie_pad(&g_movie, g_movie_frame, 0u);
                pad2   = snes_movie_pad(&g_movie, g_movie_frame, 1u);
                uint8_t mflags = snes_movie_flags(&g_movie, g_movie_frame);
                flags |= mflags;
                g_movie_frame++;
                if ((mflags & SNES_MOVIE_FLAG_POWER) != 0u) {
                    power_on(&g_ctx, &g_apu, rom, rom_size);
                } else if ((mflags & SNES_MOVIE_FLAG_RESET) != 0u) {
                    soft_reset(&g_ctx, &g_apu);
                }
            } else {
                g_movie_active = false; /* Movie finished (or none): input comes from the player */
                pad1 = (uint16_t)(poll_keyboard_pad(hwnd, 0u) | poll_xinput_pad(0u));
                pad2 = (uint16_t)(poll_keyboard_pad(hwnd, 1u) | poll_xinput_pad(1u));
            }
            g_frame_number++;
            snes_step_frame(&g_ctx, pad1, pad2, g_framebuf);
            contra3_extras_apply(&g_ctx, &g_extras);
            snes_movie_record_frame(&g_recorder, pad1, pad2, flags);
            win32_audio_pump(turbo, false);
            g_step_once = false;
            frames_this_sec++;
        } else {
            win32_audio_pump(false, true);
        }

        present_framebuffer(hwnd, hdc);

        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        if (now.QuadPart - fps_timer.QuadPart >= qpc_freq.QuadPart) {
            double sec = (double)(now.QuadPart - fps_timer.QuadPart) / (double)qpc_freq.QuadPart;
            double fps = (double)frames_this_sec / sec;
            char movie_note[48] = "";
            if (g_movie_active) {
                snprintf(movie_note, sizeof(movie_note), " [MOVIE %u/%u]",
                         (unsigned)g_movie_frame, (unsigned)g_movie.frame_count);
            } else if (g_recorder.fp != NULL) {
                snprintf(movie_note, sizeof(movie_note), " [REC %u]", (unsigned)g_recorder.frames);
            }
            char title[384];
            snprintf(title, sizeof(title),
                     "Contra III: The Alien Wars [C11 Recomp] | %.1f FPS%s%s%s | %s%s | State:$%02X | Unresolved:%u",
                     fps,
                     g_paused ? " [PAUSED]" : (turbo ? " [TURBO]" : ""),
                     g_audio_muted ? " [MUTED]" : "",
                     movie_note,
                     g_use_gl ? win32_gl_filter_name(g_gl_filter) : "GDI",
                     (g_use_gl && g_integer_scale) ? " Int" : "",
                     snes_bus_read8(&g_ctx, PROBE_GAME_STATE),
                     g_ctx.unresolved_jumps);
            if (g_status[0] != '\0' && (LONG)(GetTickCount() - g_status_until) < 0) {
                size_t len = strlen(title);
                snprintf(title + len, sizeof(title) - len, " | %s", g_status);
            }
            SetWindowTextA(hwnd, (g_rebind_player >= 0) ? g_rebind_prompt : title);
            fps_timer = now;
            frames_this_sec = 0u;
        }

        if (!turbo && !swap_paced) {
            next_tick.QuadPart += frame_ticks;
            QueryPerformanceCounter(&now);
            if (now.QuadPart < next_tick.QuadPart) {
                LONGLONG remain_ms = ((next_tick.QuadPart - now.QuadPart) * 1000LL) / qpc_freq.QuadPart;
                if (remain_ms > 2LL) {
                    Sleep((DWORD)(remain_ms - 1LL));
                }
                do {
                    QueryPerformanceCounter(&now);
                } while (now.QuadPart < next_tick.QuadPart);
            } else if (now.QuadPart - next_tick.QuadPart > frame_ticks * 4LL) {
                next_tick = now;
            }
        } else {
            QueryPerformanceCounter(&next_tick);
        }
    }

    timeEndPeriod(1u);
    {
        /* Persist runtime changes: video/lag toggles, extras toggles, and the OPTION MODE values */
        if (g_gl_filter != start_filter || g_integer_scale != start_integer || (g_lag_budget != 0u) != (start_lag != 0u)) {
            static const char *const filter_names[] = { "sharp", "nearest", "crt" };
            snprintf(g_config.filter, sizeof(g_config.filter), "%s", filter_names[g_gl_filter]);
            g_config.integer_scale = g_integer_scale;
            g_config.lag_model = g_lag_budget != 0u;
        }
        if (g_extras.flags != extras_at_start) {
            g_config.stage_select   = (g_extras.flags & CONTRA3_UNLOCK_STAGE_SELECT) != 0u;
            g_config.infinite_lives = (g_extras.flags & CONTRA3_UNLOCK_INFINITE_LIVES) != 0u;
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
            contra3_config_save(&g_config);
        }
    }
    snes_movie_record_end(&g_recorder);
    win32_audio_shutdown();
    if (g_use_gl) {
        win32_gl_shutdown(hwnd, hdc);
    }
    ReleaseDC(hwnd, hdc);
    free(lag);
    snes_movie_free(&g_movie);
    free(rom);
    return 0;
}
