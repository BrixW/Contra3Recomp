/*
 * Contra III: The Alien Wars (USA) - in-game settings overlay
 */

#include "c3_overlay.h"
#include "snes_ppu.h"

#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------------- */
/* 5x7 pixel font (ASCII 32..95; lower case is drawn as upper case)          */
/* ------------------------------------------------------------------------- */

static const char *const k_font[64] = {
    /*   */ "..................................." , /* ! */ "..#....#....#....#....#.........#..",
    /* " */ ".#.#..#.#.........................." , /* # */ ".#.#..#.#.#####.#.#.#####.#.#..#.#.",
    /* $ */ "..#...#####.#...###...#.#####...#.." , /* % */ "##...##..#...#...#...#...#..##...##",
    /* & */ ".##..#..#.#.#...#...#.#.##..#..##.#", /* ' */ "..#....#...#.......................",
    /* ( */ "...#...#...#....#....#.....#.....#." , /* ) */ ".#.....#.....#....#....#...#...#...",
    /* * */ ".......#..#.#.#.###.#.#.#..#......." , /* + */ ".......#....#..#####..#....#.......",
    /* , */ "......................##...#...#..." , /* - */ "...............#####..............." ,
    /* . */ ".........................##...##..." , /* / */ "....#...#....#...#...#....#...#...." ,
    /* 0 */ ".###.#...##..###.#.###..##...#.###." , /* 1 */ "..#...##....#....#....#....#...###.",
    /* 2 */ ".###.#...#....#...#...#...#...#####" , /* 3 */ "#####...#...#.....#.....##...#.###." ,
    /* 4 */ "...#...##..#.#.#..#.#####...#....#." , /* 5 */ "######....####.....#....##...#.###." ,
    /* 6 */ "..##..#...#....####.#...##...#.###." , /* 7 */ "#####....#...#...#...#....#....#..." ,
    /* 8 */ ".###.#...##...#.###.#...##...#.###." , /* 9 */ ".###.#...##...#.####....#...#..##.." ,
    /* : */ "......##...##.........##...##......" , /* ; */ "......##...##........##....#...#..." ,
    /* < */ "...#...#...#...#.....#.....#.....#." , /* = */ "..........#####.....#####.........." ,
    /* > */ ".#.....#.....#.....#...#...#...#..." , /* ? */ ".###.#...#....#...#...#.........#.." ,
    /* @ */ ".###.#...##.####.#.##.####.....###." , /* A */ ".###.#...##...#######...##...##...#",
    /* B */ "####.#...##...#####.#...##...#####." , /* C */ ".###.#...##....#....#....#...#.###." ,
    /* D */ "###..#..#.#...##...##...##..#.###.." , /* E */ "######....#....####.#....#....#####",
    /* F */ "######....#....####.#....#....#....", /* G */ ".###.#...##....#.####...##...#.####",
    /* H */ "#...##...##...#######...##...##...#", /* I */ ".###...#....#....#....#....#...###.",
    /* J */ "..###...#....#....#....#.#..#..##..", /* K */ "#...##..#.#.#..##...#.#..#..#.#...#",
    /* L */ "#....#....#....#....#....#....#####", /* M */ "#...###.###.#.##.#.##...##...##...#",
    /* N */ "#...##...###..##.#.##..###...##...#", /* O */ ".###.#...##...##...##...##...#.###.",
    /* P */ "####.#...##...#####.#....#....#....", /* Q */ ".###.#...##...##...##.#.##..#..##.#",
    /* R */ "####.#...##...#####.#.#..#..#.#...#", /* S */ ".#####....#.....###.....#....#####.",
    /* T */ "#####..#....#....#....#....#....#..", /* U */ "#...##...##...##...##...##...#.###.",
    /* V */ "#...##...##...##...##...#.#.#...#..", /* W */ "#...##...##...##.#.##.#.##.#.#.#.#.",
    /* X */ "#...##...#.#.#...#...#.#.#...##...#", /* Y */ "#...##...#.#.#...#....#....#....#..",
    /* Z */ "#####....#...#...#...#...#....#####", /* [ */ ".###..#....#....#....#....#....###.",
    /* \ */ "#.....#....#.....#.....#....#.....#", /* ] */ ".###....#....#....#....#....#..###.",
    /* ^ */ "..#...#.#.#...#....................", /* _ */ "..............................#####",
};

static uint8_t g_glyph[64][7];
static bool    g_font_ready = false;

static void font_init(void) {
    if (g_font_ready) {
        return;
    }
    for (int c = 0; c < 64; ++c) {
        const char *s = k_font[c];
        size_t len = strlen(s);
        for (int row = 0; row < 7; ++row) {
            uint8_t bits = 0u;
            for (int col = 0; col < 5; ++col) {
                size_t i = (size_t)(row * 5 + col);
                if (i < len && s[i] == '#') {
                    bits |= (uint8_t)(0x10u >> col);
                }
            }
            g_glyph[c][row] = bits;
        }
    }
    g_font_ready = true;
}

int c3_overlay_text_width(const char *text) {
    return (int)strlen(text) * 6;
}

void c3_overlay_text(uint32_t *out, int x, int y, const char *text, uint32_t color) {
    font_init();
    for (const char *p = text; *p != '\0'; ++p, x += 6) {
        int ch = (unsigned char)*p;
        if (ch >= 'a' && ch <= 'z') {
            ch -= 32;
        }
        if (ch < 32 || ch > 95) {
            ch = '?';
        }
        const uint8_t *g = g_glyph[ch - 32];
        for (int row = 0; row < 7; ++row) {
            int py = y + row;
            if (py < 0 || py >= C3_OVERLAY_H) {
                continue;
            }
            for (int col = 0; col < 5; ++col) {
                int px = x + col;
                if ((g[row] & (0x10u >> col)) != 0u && px >= 0 && px < C3_OVERLAY_W) {
                    out[py * C3_OVERLAY_W + px] = color;
                    /* 1-pixel drop shadow for legibility over the dimmed game */
                    if (px + 1 < C3_OVERLAY_W && py + 1 < C3_OVERLAY_H &&
                        out[(py + 1) * C3_OVERLAY_W + px + 1] != color) {
                        out[(py + 1) * C3_OVERLAY_W + px + 1] = 0xFF000000u;
                    }
                }
            }
        }
    }
}

/* ------------------------------------------------------------------------- */
/* Menu model                                                                */
/* ------------------------------------------------------------------------- */

enum { PG_MAIN = C3OV_MAIN, PG_VIDEO = C3OV_VIDEO, PG_AUDIO = C3OV_AUDIO, PG_CONTROLS = C3OV_CONTROLS,
       PG_KEYS_P1 = C3OV_KEYS_P1, PG_KEYS_P2 = C3OV_KEYS_P2, PG_PAD = C3OV_PAD, PG_CHEATS = C3OV_CHEATS };

enum ItemId {
    IT_BACK, IT_RESUME, IT_SUB, IT_SAVE, IT_LOAD, IT_RESET, IT_QUIT,
    IT_FILTER, IT_INTEGER, IT_VSYNC, IT_FULLSCREEN, IT_SLOWDOWN,
    IT_VOLUME, IT_MUTE,
    IT_DEADZONE, IT_RUMBLE, IT_RUMBLE_STRENGTH, IT_RUMBLE_TEST, IT_CONTROLS_DEFAULT,
    IT_BIND_ALL, IT_KEY, IT_PAD,
    IT_STAGE_SELECT, IT_LIVES30, IT_INF_LIVES, IT_START_STAGE
};

typedef struct Item {
    int  id;
    int  arg;          /* sub-page, button index, pad input */
    char label[40];
    char value[32];
    bool enabled;
} Item;

#define MAX_ITEMS 20

static const char *const k_titles[] = {
    "CONTRA III - PAUSED", "VIDEO", "AUDIO", "CONTROLS", "PLAYER 1 KEYBOARD", "PLAYER 2 KEYBOARD",
    "GAMEPAD BUTTONS", "CHEATS & EXTRAS",
};

static void add(Item *items, int *n, int id, int arg, const char *label, const char *value, bool enabled) {
    if (*n >= MAX_ITEMS) {
        return;
    }
    Item *it = &items[(*n)++];
    it->id = id;
    it->arg = arg;
    snprintf(it->label, sizeof(it->label), "%s", label);
    snprintf(it->value, sizeof(it->value), "%s", value != NULL ? value : "");
    it->enabled = enabled;
}

static const char *onoff(bool v) { return v ? "ON" : "OFF"; }

static void keys_text(const Contra3Config *cfg, unsigned p, unsigned b, char *out, size_t len) {
    out[0] = '\0';
    for (unsigned k = 0u; k < C3_KEYS_PER_BTN && cfg->keys[p][b][k] != 0u; ++k) {
        size_t l = strlen(out);
        snprintf(out + l, len - l, "%s%s", k ? ", " : "", c3_key_name((C3Key)cfg->keys[p][b][k]));
    }
    if (out[0] == '\0') {
        snprintf(out, len, "-");
    }
}

static int build(const C3Overlay *ov, const Contra3Config *cfg, const C3OverlayEnv *env, Item *items) {
    int n = 0;
    char v[32];
    switch (ov->page) {
        case PG_MAIN:
            add(items, &n, IT_RESUME, 0, "RESUME", NULL, true);
            add(items, &n, IT_SUB, PG_VIDEO, "VIDEO", ">", true);
            add(items, &n, IT_SUB, PG_AUDIO, "AUDIO", ">", true);
            add(items, &n, IT_SUB, PG_CONTROLS, "CONTROLS & RUMBLE", ">", true);
            add(items, &n, IT_SUB, PG_CHEATS, "CHEATS & EXTRAS", ">", true);
            add(items, &n, IT_SAVE, 0, "SAVE STATE", NULL, true);
            add(items, &n, IT_LOAD, 0, "LOAD STATE", NULL, true);
            add(items, &n, IT_RESET, 0, "RESET GAME", NULL, true);
            add(items, &n, IT_QUIT, 0, "QUIT", NULL, true);
            break;
        case PG_VIDEO: {
            static const char *const filters[] = { "SHARP", "NEAREST", "CRT" };
            int f = (strcmp(cfg->filter, "crt") == 0) ? 2 : (strcmp(cfg->filter, "nearest") == 0) ? 1 : 0;
            add(items, &n, IT_FILTER, 0, "FILTER", env->gl_available ? filters[f] : "N/A (GDI)", env->gl_available);
            add(items, &n, IT_INTEGER, 0, "INTEGER SCALING", onoff(cfg->integer_scale), env->gl_available);
            add(items, &n, IT_VSYNC, 0, "VSYNC", onoff(cfg->vsync), env->gl_available);
            add(items, &n, IT_FULLSCREEN, 0, "FULLSCREEN", onoff(env->fullscreen), true);
            add(items, &n, IT_SLOWDOWN, 0, "AUTHENTIC SLOWDOWN", onoff(cfg->lag_model), true);
            add(items, &n, IT_BACK, 0, "BACK", NULL, true);
            break;
        }
        case PG_AUDIO:
            snprintf(v, sizeof(v), "%u%%", cfg->volume);
            add(items, &n, IT_VOLUME, 0, "VOLUME", v, true);
            add(items, &n, IT_MUTE, 0, "MUTE", onoff(env->muted), true);
            add(items, &n, IT_BACK, 0, "BACK", NULL, true);
            break;
        case PG_CONTROLS:
            add(items, &n, IT_SUB, PG_KEYS_P1, "PLAYER 1 KEYBOARD", ">", true);
            add(items, &n, IT_SUB, PG_KEYS_P2, "PLAYER 2 KEYBOARD", ">", true);
            add(items, &n, IT_SUB, PG_PAD, "GAMEPAD BUTTONS", ">", true);
            snprintf(v, sizeof(v), "%d", cfg->pad_deadzone);
            add(items, &n, IT_DEADZONE, 0, "STICK DEADZONE", v, true);
            add(items, &n, IT_RUMBLE, 0, "RUMBLE", onoff(cfg->rumble), true);
            snprintf(v, sizeof(v), "%u%%", cfg->rumble_strength);
            add(items, &n, IT_RUMBLE_STRENGTH, 0, "RUMBLE STRENGTH", v, cfg->rumble);
            add(items, &n, IT_RUMBLE_TEST, 0, "TEST RUMBLE", NULL, cfg->rumble);
            add(items, &n, IT_CONTROLS_DEFAULT, 0, "RESET CONTROLS TO DEFAULT", NULL, true);
            add(items, &n, IT_BACK, 0, "BACK", NULL, true);
            break;
        case PG_KEYS_P1:
        case PG_KEYS_P2: {
            unsigned p = (ov->page == PG_KEYS_P1) ? 0u : 1u;
            add(items, &n, IT_BIND_ALL, 0, "BIND ALL BUTTONS", NULL, true);
            for (unsigned b = 0u; b < C3_BUTTONS; ++b) {
                keys_text(cfg, p, b, v, sizeof(v));
                add(items, &n, IT_KEY, (int)b, c3_button_label(b), v, true);
            }
            add(items, &n, IT_BACK, 0, "BACK", NULL, true);
            break;
        }
        case PG_PAD: {
            static const char *const labels[C3P_COUNT] = { "A", "B", "X", "Y", "LB", "RB", "LT", "RT", "BACK", "START" };
            for (int i = 0; i < C3P_COUNT; ++i) {
                char label[24];
                snprintf(label, sizeof(label), "PAD %s", labels[i]);
                const char *t = c3_snes_name_from_mask(cfg->pad_map[i]);
                char up[16];
                size_t k = 0u;
                for (; t[k] != '\0' && k + 1u < sizeof(up); ++k) up[k] = (char)((t[k] >= 'a' && t[k] <= 'z') ? t[k] - 32 : t[k]);
                up[k] = '\0';
                add(items, &n, IT_PAD, i, label, up, true);
            }
            add(items, &n, IT_BACK, 0, "BACK", NULL, true);
            break;
        }
        case PG_CHEATS:
            add(items, &n, IT_STAGE_SELECT, 0, "STAGE SELECT SCREEN", onoff(cfg->stage_select), true);
            add(items, &n, IT_LIVES30, 0, "30 STARTING LIVES", onoff(cfg->lives_30), true);
            add(items, &n, IT_INF_LIVES, 0, "INFINITE LIVES", onoff(cfg->infinite_lives), true);
            if (cfg->start_stage == 0u) snprintf(v, sizeof(v), "NORMAL");
            else snprintf(v, sizeof(v), "STAGE %u", cfg->start_stage);
            add(items, &n, IT_START_STAGE, 0, "START AT", v, true);
            add(items, &n, IT_BACK, 0, "BACK", NULL, true);
            break;
        default:
            break;
    }
    return n;
}

void c3_overlay_init(C3Overlay *ov) {
    memset(ov, 0, sizeof(*ov));
    ov->capture_player = -1;
}

void c3_overlay_open(C3Overlay *ov) {
    c3_overlay_init(ov);
    ov->open = true;
}

static void push(C3Overlay *ov, int page);

void c3_overlay_open_at(C3Overlay *ov, C3OverlayPage page) {
    c3_overlay_open(ov);
    if (page == C3OV_KEYS_P1 || page == C3OV_KEYS_P2 || page == C3OV_PAD) {
        push(ov, PG_CONTROLS);          /* BACK walks up through CONTROLS to the main menu */
        ov->cursor = 0;
    }
    if (page != C3OV_MAIN) {
        push(ov, (int)page);
    }
}

void c3_overlay_close(C3Overlay *ov) {
    ov->open = false;
    ov->capture_player = -1;
}

bool c3_overlay_capturing(const C3Overlay *ov) {
    return ov->open && ov->capture_player >= 0;
}

static void push(C3Overlay *ov, int page) {
    if (ov->depth < 8) {
        ov->stack_page[ov->depth] = ov->page;
        ov->stack_cursor[ov->depth] = ov->cursor;
        ov->depth++;
    }
    ov->page = page;
    ov->cursor = 0;
    ov->scroll = 0;
}

static C3OverlayAction pop(C3Overlay *ov) {
    if (ov->depth == 0) {
        c3_overlay_close(ov);
        return C3ACT_RESUME;
    }
    ov->depth--;
    ov->page = ov->stack_page[ov->depth];
    ov->cursor = ov->stack_cursor[ov->depth];
    ov->scroll = 0;
    return C3ACT_NONE;
}

static unsigned cycle_u(unsigned v, int dir, unsigned lo, unsigned hi, unsigned step) {
    if (dir < 0) return (v <= lo) ? hi : ((v - step < lo) ? lo : v - step);
    return (v >= hi) ? lo : ((v + step > hi) ? hi : v + step);
}

static C3OverlayAction activate(C3Overlay *ov, Contra3Config *cfg, const Item *it, int dir) {
    bool ok = (dir == 0);   /* OK pressed (vs left/right) */
    int d = (dir == 0) ? 1 : dir;
    switch (it->id) {
        case IT_BACK:   return ok ? pop(ov) : C3ACT_NONE;
        case IT_RESUME: if (ok) { c3_overlay_close(ov); return C3ACT_RESUME; } return C3ACT_NONE;
        case IT_SUB:    if (ok) push(ov, it->arg); return C3ACT_NONE;
        case IT_SAVE:   return ok ? C3ACT_SAVE_STATE : C3ACT_NONE;
        case IT_LOAD:   return ok ? C3ACT_LOAD_STATE : C3ACT_NONE;
        case IT_RESET:  if (ok) { c3_overlay_close(ov); return C3ACT_RESET; } return C3ACT_NONE;
        case IT_QUIT:   return ok ? C3ACT_QUIT : C3ACT_NONE;
        case IT_FILTER: {
            static const char *const f[] = { "sharp", "nearest", "crt" };
            int cur = (strcmp(cfg->filter, "crt") == 0) ? 2 : (strcmp(cfg->filter, "nearest") == 0) ? 1 : 0;
            cur = (cur + (d > 0 ? 1 : 2)) % 3;
            snprintf(cfg->filter, sizeof(cfg->filter), "%s", f[cur]);
            ov->dirty = true;
            return C3ACT_NONE;
        }
        case IT_INTEGER:  cfg->integer_scale = !cfg->integer_scale; ov->dirty = true; return C3ACT_NONE;
        case IT_VSYNC:    cfg->vsync = !cfg->vsync; ov->dirty = true; return C3ACT_NONE;
        case IT_FULLSCREEN: return C3ACT_TOGGLE_FULLSCREEN;
        case IT_SLOWDOWN: cfg->lag_model = !cfg->lag_model; ov->dirty = true; return C3ACT_NONE;
        case IT_VOLUME:   cfg->volume = cycle_u(cfg->volume, ok ? 1 : d, 0u, 100u, 10u); ov->dirty = true; return C3ACT_NONE;
        case IT_MUTE:     ov->mute_request = true; return C3ACT_NONE;
        case IT_DEADZONE: {
            unsigned dz = cycle_u((unsigned)cfg->pad_deadzone, ok ? 1 : d, 2000u, 24000u, 2000u);
            cfg->pad_deadzone = (int)dz;
            ov->dirty = true;
            return C3ACT_NONE;
        }
        case IT_RUMBLE:   cfg->rumble = !cfg->rumble; ov->dirty = true; return C3ACT_NONE;
        case IT_RUMBLE_STRENGTH:
            cfg->rumble_strength = cycle_u(cfg->rumble_strength, ok ? 1 : d, 10u, 100u, 10u);
            ov->dirty = true;
            return C3ACT_TEST_RUMBLE;
        case IT_RUMBLE_TEST: return ok ? C3ACT_TEST_RUMBLE : C3ACT_NONE;
        case IT_CONTROLS_DEFAULT:
            if (ok) {
                Contra3Config def;
                memset(&def, 0, sizeof(def));
                c3_config_defaults(&def);
                memcpy(cfg->keys, def.keys, sizeof(cfg->keys));
                memcpy(cfg->pad_map, def.pad_map, sizeof(cfg->pad_map));
                cfg->pad_deadzone = def.pad_deadzone;
                snprintf(ov->message, sizeof(ov->message), "CONTROLS RESET TO DEFAULT");
                ov->dirty = true;
            }
            return C3ACT_NONE;
        case IT_BIND_ALL:
            if (ok) {
                ov->capture_player = (ov->page == PG_KEYS_P1) ? 0 : 1;
                ov->capture_button = 0;
                ov->capture_all = true;
            }
            return C3ACT_NONE;
        case IT_KEY:
            if (ok) {
                ov->capture_player = (ov->page == PG_KEYS_P1) ? 0 : 1;
                ov->capture_button = it->arg;
                ov->capture_all = false;
            }
            return C3ACT_NONE;
        case IT_PAD:
            cfg->pad_map[it->arg] = c3_snes_mask_next(cfg->pad_map[it->arg], d);
            ov->dirty = true;
            return C3ACT_NONE;
        case IT_STAGE_SELECT: cfg->stage_select = !cfg->stage_select; ov->dirty = true; return C3ACT_NONE;
        case IT_LIVES30:      cfg->lives_30 = !cfg->lives_30; ov->dirty = true; return C3ACT_NONE;
        case IT_INF_LIVES:    cfg->infinite_lives = !cfg->infinite_lives; ov->dirty = true; return C3ACT_NONE;
        case IT_START_STAGE:
            cfg->start_stage = (unsigned)(((int)cfg->start_stage + (d > 0 ? 1 : 6)) % 7);
            ov->dirty = true;
            return C3ACT_NONE;
        default:
            return C3ACT_NONE;
    }
}

C3OverlayAction c3_overlay_nav(C3Overlay *ov, Contra3Config *cfg, C3Nav nav) {
    if (!ov->open || c3_overlay_capturing(ov)) {
        return C3ACT_NONE;
    }
    static const C3OverlayEnv dummy = { true, false, false, NULL };
    Item items[MAX_ITEMS];
    int n = build(ov, cfg, &dummy, items);
    if (n == 0) {
        return C3ACT_NONE;
    }
    ov->message[0] = '\0';
    switch (nav) {
        case C3NAV_UP:
            do { ov->cursor = (ov->cursor + n - 1) % n; } while (!items[ov->cursor].enabled && n > 1);
            return C3ACT_NONE;
        case C3NAV_DOWN:
            do { ov->cursor = (ov->cursor + 1) % n; } while (!items[ov->cursor].enabled && n > 1);
            return C3ACT_NONE;
        case C3NAV_BACK:
            return pop(ov);
        case C3NAV_LEFT:
        case C3NAV_RIGHT:
        case C3NAV_OK: {
            if (ov->cursor >= n || !items[ov->cursor].enabled) {
                return C3ACT_NONE;
            }
            int dir = (nav == C3NAV_OK) ? 0 : (nav == C3NAV_LEFT) ? -1 : 1;
            return activate(ov, cfg, &items[ov->cursor], dir);
        }
    }
    return C3ACT_NONE;
}

C3OverlayAction c3_overlay_capture_key(C3Overlay *ov, Contra3Config *cfg, C3Key key) {
    if (!c3_overlay_capturing(ov)) {
        return C3ACT_NONE;
    }
    if (key != C3K_ESCAPE && key != C3K_NONE) {
        c3_config_bind_key(cfg, (unsigned)ov->capture_player, (unsigned)ov->capture_button, key);
        ov->dirty = true;
    }
    if (ov->capture_all && key != C3K_ESCAPE && ++ov->capture_button < C3_BUTTONS) {
        return C3ACT_NONE;
    }
    if (ov->capture_all && key == C3K_ESCAPE && ++ov->capture_button < C3_BUTTONS) {
        return C3ACT_NONE;   /* Escape keeps this button and moves on */
    }
    ov->capture_player = -1;
    ov->capture_all = false;
    return C3ACT_NONE;
}

/* ------------------------------------------------------------------------- */
/* Rendering                                                                 */
/* ------------------------------------------------------------------------- */

static void fill(uint32_t *out, int x0, int y0, int x1, int y1, uint32_t color) {
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            if (x >= 0 && x < C3_OVERLAY_W && y >= 0 && y < C3_OVERLAY_H) {
                out[y * C3_OVERLAY_W + x] = color;
            }
        }
    }
}

#define COL_TITLE   0xFFFFD040u
#define COL_TEXT    0xFFB8C0D0u
#define COL_SEL     0xFFFFFFFFu
#define COL_VALUE   0xFF80E0FFu
#define COL_OFF     0xFF5A6070u
#define COL_HINT    0xFF8890A0u
#define COL_PANEL   0xFF101828u
#define COL_BAR     0xFF2A4878u
#define COL_FRAME   0xFF5070A8u

void c3_overlay_render(const C3Overlay *ov, const Contra3Config *cfg, const C3OverlayEnv *env,
                       const uint32_t *game, uint32_t *out) {
    /* Dimmed game behind the panel */
    for (int i = 0; i < C3_OVERLAY_W * C3_OVERLAY_H; ++i) {
        uint32_t c = game[i];
        uint32_t r = ((c >> 16) & 0xFFu) * 3u / 10u, g = ((c >> 8) & 0xFFu) * 3u / 10u, b = (c & 0xFFu) * 3u / 10u;
        out[i] = 0xFF000000u | (r << 16) | (g << 8) | b;
    }
    if (!ov->open) {
        return;
    }
    const int px0 = 14, px1 = 242, py0 = 10, py1 = 214;
    fill(out, px0, py0, px1, py1, COL_PANEL);
    fill(out, px0, py0, px1, py0 + 1, COL_FRAME);
    fill(out, px0, py1 - 1, px1, py1, COL_FRAME);
    fill(out, px0, py0, px0 + 1, py1, COL_FRAME);
    fill(out, px1 - 1, py0, px1, py1, COL_FRAME);

    const char *title = k_titles[ov->page];
    c3_overlay_text(out, (C3_OVERLAY_W - c3_overlay_text_width(title)) / 2, py0 + 6, title, COL_TITLE);
    fill(out, px0 + 8, py0 + 17, px1 - 8, py0 + 18, COL_FRAME);

    Item items[MAX_ITEMS];
    int n = build(ov, cfg, env, items);
    const int row_h = 11, visible = 14;
    int first = 0;
    if (ov->cursor >= visible) {
        first = ov->cursor - visible + 1;
    }
    int y = py0 + 24;
    for (int i = first; i < n && i < first + visible; ++i, y += row_h) {
        const Item *it = &items[i];
        bool sel = (i == ov->cursor);
        bool capturing_this = c3_overlay_capturing(ov) && it->id == IT_KEY && it->arg == ov->capture_button;
        if (sel || capturing_this) {
            fill(out, px0 + 4, y - 2, px1 - 4, y + 9, COL_BAR);
        }
        uint32_t col = !it->enabled ? COL_OFF : (sel ? COL_SEL : COL_TEXT);
        c3_overlay_text(out, px0 + 10, y, it->label, col);
        const char *value = capturing_this ? "PRESS A KEY..." : it->value;
        if (value[0] != '\0') {
            uint32_t vcol = !it->enabled ? COL_OFF : (capturing_this ? COL_TITLE : COL_VALUE);
            bool cyc = sel && it->enabled && it->id != IT_SUB && it->id != IT_KEY && it->id != IT_BIND_ALL && value[0] != '>';
            char buf[40];
            snprintf(buf, sizeof(buf), cyc ? "< %s >" : "%s", value);
            int w = c3_overlay_text_width(buf);
            c3_overlay_text(out, px1 - 10 - w, y, buf, vcol);
        }
    }

    const char *hint;
    if (c3_overlay_capturing(ov)) {
        hint = ov->capture_all ? "PRESS A KEY  (ESC SKIPS)" : "PRESS A KEY  (ESC CANCELS)";
    } else if (ov->message[0] != '\0') {
        hint = ov->message;
    } else if (env->status != NULL && env->status[0] != '\0') {
        hint = env->status;
    } else if (ov->page == PG_KEYS_P1 || ov->page == PG_KEYS_P2) {
        hint = "ENTER: REBIND   ESC: BACK";
    } else {
        hint = "ARROWS: MOVE / CHANGE   ESC: BACK";
    }
    char hbuf[48];
    snprintf(hbuf, sizeof(hbuf), "%.40s", hint);
    c3_overlay_text(out, (C3_OVERLAY_W - c3_overlay_text_width(hbuf)) / 2, py1 - 13, hbuf, COL_HINT);
}
