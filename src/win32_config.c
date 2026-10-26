/*
 * Contra III: The Alien Wars (USA) - contra3.ini key bindings, gamepad map & preferences
 */

#include "win32_config.h"
#include "snes_ppu.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ButtonDef {
    const char *name;     /* ini key and gamepad target name */
    const char *label;    /* shown while rebinding */
    uint16_t    mask;
    const char *p1;       /* default bindings */
    const char *p2;
} ButtonDef;

static const ButtonDef k_buttons[CFG_BUTTONS] = {
    { "up",     "UP",                   SNES_BTN_UP,     "Up",                "Num8" },
    { "down",   "DOWN",                 SNES_BTN_DOWN,   "Down",              "Num5" },
    { "left",   "LEFT",                 SNES_BTN_LEFT,   "Left",              "Num4" },
    { "right",  "RIGHT",                SNES_BTN_RIGHT,  "Right",             "Num6" },
    { "b",      "B (jump)",             SNES_BTN_B,      "Z, J",              "Num1" },
    { "y",      "Y (shoot)",            SNES_BTN_Y,      "X, K",              "Num2" },
    { "x",      "X (switch weapon)",    SNES_BTN_X,      "A, U",              "Num3" },
    { "a",      "A (mega bomb)",        SNES_BTN_A,      "S, I",              "Num0" },
    { "l",      "L (lock position)",    SNES_BTN_L,      "Q, L",              "Num7" },
    { "r",      "R (dual-wield fire)",  SNES_BTN_R,      "E, O",              "Num9" },
    { "start",  "START",                SNES_BTN_START,  "Enter",             "NumAdd" },
    { "select", "SELECT",               SNES_BTN_SELECT, "RShift, Backspace", "NumSub" },
};

/* XInput inputs that can be remapped; D-pad and left stick always drive the SNES D-pad */
typedef struct PadDef {
    const char *name;
    uint16_t    xinput_mask;   /* wButtons bit, or 0 for a trigger */
    int         trigger;       /* 0 none, 1 left, 2 right */
    const char *def;           /* default SNES target */
} PadDef;

static const PadDef k_pad[CFG_PAD_INPUTS] = {
    { "a",     0x1000u, 0, "b" },      /* Xbox A -> SNES B (jump) */
    { "b",     0x2000u, 0, "a" },      /* Xbox B -> SNES A (mega bomb) */
    { "x",     0x4000u, 0, "y" },      /* Xbox X -> SNES Y (shoot) */
    { "y",     0x8000u, 0, "x" },      /* Xbox Y -> SNES X (switch weapon) */
    { "lb",    0x0100u, 0, "l" },
    { "rb",    0x0200u, 0, "r" },
    { "lt",    0u,      1, "l" },
    { "rt",    0u,      2, "r" },
    { "back",  0x0020u, 0, "select" },
    { "start", 0x0010u, 0, "start" },
};

typedef struct KeyName {
    const char *name;
    uint8_t     vk;
} KeyName;

static const KeyName k_key_names[] = {
    { "Up", VK_UP }, { "Down", VK_DOWN }, { "Left", VK_LEFT }, { "Right", VK_RIGHT },
    { "Enter", VK_RETURN }, { "Space", VK_SPACE }, { "Tab", VK_TAB }, { "Backspace", VK_BACK },
    { "LShift", VK_LSHIFT }, { "RShift", VK_RSHIFT }, { "LCtrl", VK_LCONTROL }, { "RCtrl", VK_RCONTROL },
    { "LAlt", VK_LMENU }, { "RAlt", VK_RMENU }, { "Insert", VK_INSERT }, { "Delete", VK_DELETE },
    { "Home", VK_HOME }, { "End", VK_END }, { "PageUp", VK_PRIOR }, { "PageDown", VK_NEXT },
    { "Num0", VK_NUMPAD0 }, { "Num1", VK_NUMPAD1 }, { "Num2", VK_NUMPAD2 }, { "Num3", VK_NUMPAD3 },
    { "Num4", VK_NUMPAD4 }, { "Num5", VK_NUMPAD5 }, { "Num6", VK_NUMPAD6 }, { "Num7", VK_NUMPAD7 },
    { "Num8", VK_NUMPAD8 }, { "Num9", VK_NUMPAD9 }, { "NumAdd", VK_ADD }, { "NumSub", VK_SUBTRACT },
    { "NumMul", VK_MULTIPLY }, { "NumDiv", VK_DIVIDE }, { "NumDot", VK_DECIMAL },
    { "Comma", VK_OEM_COMMA }, { "Period", VK_OEM_PERIOD }, { "Slash", VK_OEM_2 }, { "Semicolon", VK_OEM_1 },
    { "Quote", VK_OEM_7 }, { "LBracket", VK_OEM_4 }, { "RBracket", VK_OEM_6 }, { "Minus", VK_OEM_MINUS },
    { "Equals", VK_OEM_PLUS }, { "Backslash", VK_OEM_5 }, { "Grave", VK_OEM_3 },
};

static bool name_eq(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a++) != tolower((unsigned char)*b++)) {
            return false;
        }
    }
    return *a == '\0' && *b == '\0';
}

static uint8_t vk_from_name(const char *name) {
    if (name[0] != '\0' && name[1] == '\0') {
        char c = (char)toupper((unsigned char)name[0]);
        if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            return (uint8_t)c;
        }
    }
    for (size_t i = 0u; i < sizeof(k_key_names) / sizeof(k_key_names[0]); ++i) {
        if (name_eq(name, k_key_names[i].name)) {
            return k_key_names[i].vk;
        }
    }
    return 0u;
}

bool contra3_config_key_supported(uint8_t vk) {
    if ((vk >= 'A' && vk <= 'Z') || (vk >= '0' && vk <= '9')) {
        return true;
    }
    for (size_t i = 0u; i < sizeof(k_key_names) / sizeof(k_key_names[0]); ++i) {
        if (k_key_names[i].vk == vk) {
            return true;
        }
    }
    return false;
}

void contra3_config_key_name(uint8_t vk, char *out, size_t len) {
    if ((vk >= 'A' && vk <= 'Z') || (vk >= '0' && vk <= '9')) {
        snprintf(out, len, "%c", (char)vk);
        return;
    }
    for (size_t i = 0u; i < sizeof(k_key_names) / sizeof(k_key_names[0]); ++i) {
        if (k_key_names[i].vk == vk) {
            snprintf(out, len, "%s", k_key_names[i].name);
            return;
        }
    }
    snprintf(out, len, "?");
}

const char *contra3_config_button_label(unsigned button) {
    return (button < CFG_BUTTONS) ? k_buttons[button].label : "?";
}

static uint16_t snes_mask_from_name(const char *name, bool *ok) {
    *ok = true;
    if (name_eq(name, "none") || name[0] == '\0') {
        return 0u;
    }
    for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
        if (name_eq(name, k_buttons[b].name)) {
            return k_buttons[b].mask;
        }
    }
    *ok = false;
    return 0u;
}

static const char *snes_name_from_mask(uint16_t mask) {
    for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
        if (k_buttons[b].mask == mask) {
            return k_buttons[b].name;
        }
    }
    return "none";
}

static char *trim(char *s) {
    while (isspace((unsigned char)*s)) {
        s++;
    }
    char *e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1])) {
        *--e = '\0';
    }
    return s;
}

/* Parse "Z, J" into a binding slot list; returns false if any name is unknown */
static bool parse_binding(char *value, uint8_t out[CFG_KEYS_PER_BTN], char *bad, size_t bad_len) {
    memset(out, 0, CFG_KEYS_PER_BTN);
    unsigned n = 0u;
    bool ok = true;
    for (char *tok = strtok(value, ","); tok != NULL; tok = strtok(NULL, ",")) {
        char *name = trim(tok);
        if (name[0] == '\0') {
            continue;
        }
        uint8_t vk = vk_from_name(name);
        if (vk == 0u) {
            snprintf(bad, bad_len, "%s", name);
            ok = false;
        } else if (n < CFG_KEYS_PER_BTN) {
            out[n++] = vk;
        }
    }
    return ok;
}

static void set_defaults(Contra3Config *cfg) {
    char tmp[64], bad[32];
    for (unsigned p = 0u; p < 2u; ++p) {
        for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
            snprintf(tmp, sizeof(tmp), "%s", (p == 0u) ? k_buttons[b].p1 : k_buttons[b].p2);
            parse_binding(tmp, cfg->keys[p][b], bad, sizeof(bad));
        }
    }
    for (unsigned i = 0u; i < CFG_PAD_INPUTS; ++i) {
        bool ok;
        cfg->pad_map[i] = snes_mask_from_name(k_pad[i].def, &ok);
    }
    cfg->pad_deadzone = 12000;
    snprintf(cfg->filter, sizeof(cfg->filter), "sharp");
}

static const char *difficulty_name(unsigned d) {
    return (d == 0u) ? "easy" : (d == 2u) ? "normal" : (d == 4u) ? "hard" : NULL;
}

bool contra3_config_save(const Contra3Config *cfg) {
    FILE *fp = fopen(cfg->path, "w");
    if (fp == NULL) {
        return false;
    }
    fprintf(fp, "; Contra III: The Alien Wars - static recompilation settings\n");
    fprintf(fp, "; Key names: A-Z, 0-9, Up Down Left Right Enter Space Tab Backspace LShift RShift LCtrl RCtrl\n");
    fprintf(fp, ";   LAlt RAlt Insert Delete Home End PageUp PageDown Num0-Num9 NumAdd NumSub NumMul NumDiv NumDot\n");
    fprintf(fp, ";   Comma Period Slash Semicolon Quote LBracket RBracket Minus Equals Backslash Grave\n");
    fprintf(fp, "; Up to %d keys per button, comma separated. Hotkeys (Tab, P, M, Space, F1-F12, Esc) stay active.\n",
            CFG_KEYS_PER_BTN);
    fprintf(fp, "; F2 / Shift+F2 rebind Player 1 / Player 2 from inside the game window.\n\n");

    fprintf(fp, "[video]\nfilter = %s        ; sharp | nearest | crt\ninteger = %d\nvsync = %d\n\n",
            cfg->filter, cfg->integer_scale ? 1 : 0, cfg->vsync ? 1 : 0);

    fprintf(fp, "[game]\nlag_model = %d     ; authentic slowdown (F3)\n\n", cfg->lag_model ? 1 : 0);

    fprintf(fp, "[options]          ; in-game OPTION MODE, restored at power-on (saved on exit)\n");
    if (cfg->have_options) {
        const char *dn = difficulty_name(cfg->opt_difficulty);
        fprintf(fp, "lives = %u          ; PLAYER: 3 / 5 / 7\n", cfg->opt_lives);
        if (dn != NULL) {
            fprintf(fp, "difficulty = %s    ; LEVEL: easy / normal / hard\n", dn);
        } else {
            fprintf(fp, "difficulty = %u    ; LEVEL: easy / normal / hard (raised after an ending)\n", cfg->opt_difficulty);
        }
        fprintf(fp, "sound = %s       ; stereo / mono\n\n", (cfg->opt_sound == 2u) ? "mono" : "stereo");
    } else {
        fprintf(fp, "; lives = 3\n; difficulty = normal\n; sound = stereo\n\n");
    }

    fprintf(fp, "[extras]\n");
    fprintf(fp, "stage_select = %d   ; restored STAGE SELECT screen on START (F4)\n", cfg->stage_select ? 1 : 0);
    fprintf(fp, "lives_30 = %d       ; 30 starting lives (restored Contra Spirits code)\n", cfg->lives_30 ? 1 : 0);
    fprintf(fp, "infinite_lives = %d ; lives never drop below 2 (Shift+F4)\n", cfg->infinite_lives ? 1 : 0);
    fprintf(fp, "start_stage = %u    ; 0 = normal, 1-6 = first game from the title starts at that stage\n\n",
            cfg->start_stage);

    fprintf(fp, "[gamepad]          ; XInput pads 1 & 2; targets: b y x a l r start select up down left right none\n");
    fprintf(fp, "deadzone = %d\n", cfg->pad_deadzone);
    for (unsigned i = 0u; i < CFG_PAD_INPUTS; ++i) {
        fprintf(fp, "%-6s = %s\n", k_pad[i].name, snes_name_from_mask(cfg->pad_map[i]));
    }

    for (unsigned p = 0u; p < 2u; ++p) {
        fprintf(fp, "\n[p%u]\n", p + 1u);
        for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
            char line[96] = "";
            for (unsigned k = 0u; k < CFG_KEYS_PER_BTN && cfg->keys[p][b][k] != 0u; ++k) {
                char name[16];
                contra3_config_key_name(cfg->keys[p][b][k], name, sizeof(name));
                size_t len = strlen(line);
                snprintf(line + len, sizeof(line) - len, "%s%s", (k == 0u) ? "" : ", ", name);
            }
            fprintf(fp, "%-7s= %-20s ; %s\n", k_buttons[b].name, line, k_buttons[b].label);
        }
    }
    return fclose(fp) == 0;
}

void contra3_config_load(Contra3Config *cfg, const char *exe_dir) {
    memset(cfg, 0, sizeof(*cfg));
    set_defaults(cfg);
    snprintf(cfg->path, sizeof(cfg->path), "%s\\contra3.ini", exe_dir);

    FILE *fp = fopen(cfg->path, "r");
    if (fp == NULL) {
        contra3_config_save(cfg); /* first run: write the defaults so they can be edited */
        return;
    }
    enum { SEC_NONE, SEC_VIDEO, SEC_GAME, SEC_OPTIONS, SEC_EXTRAS, SEC_PAD, SEC_P1, SEC_P2 } section = SEC_NONE;
    char line[256];
    unsigned line_no = 0u;
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_no++;
        char *semi = strchr(line, ';');
        if (semi != NULL) {
            *semi = '\0';
        }
        char *s = trim(line);
        if (s[0] == '\0') {
            continue;
        }
        if (s[0] == '[') {
            section = name_eq(s, "[video]")   ? SEC_VIDEO   : name_eq(s, "[game]")    ? SEC_GAME :
                      name_eq(s, "[options]") ? SEC_OPTIONS : name_eq(s, "[extras]")  ? SEC_EXTRAS :
                      name_eq(s, "[gamepad]") ? SEC_PAD     : name_eq(s, "[p1]")      ? SEC_P1 :
                      name_eq(s, "[p2]")      ? SEC_P2      : SEC_NONE;
            continue;
        }
        char *eq = strchr(s, '=');
        if (eq == NULL) {
            continue;
        }
        *eq = '\0';
        char *key = trim(s);
        char *val = trim(eq + 1);
        switch (section) {
            case SEC_VIDEO:
                if (name_eq(key, "filter"))       snprintf(cfg->filter, sizeof(cfg->filter), "%s", val);
                else if (name_eq(key, "integer")) cfg->integer_scale = atoi(val) != 0;
                else if (name_eq(key, "vsync"))   cfg->vsync = atoi(val) != 0;
                break;
            case SEC_GAME:
                if (name_eq(key, "lag_model")) cfg->lag_model = atoi(val) != 0;
                break;
            case SEC_OPTIONS:
                if (name_eq(key, "lives")) {
                    cfg->opt_lives = (unsigned)atoi(val);
                } else if (name_eq(key, "difficulty")) {
                    cfg->opt_difficulty = name_eq(val, "easy") ? 0u : name_eq(val, "normal") ? 2u :
                                          name_eq(val, "hard") ? 4u : (unsigned)atoi(val);
                } else if (name_eq(key, "sound")) {
                    cfg->opt_sound = (name_eq(val, "mono") || name_eq(val, "monaural")) ? 2u : 0u;
                }
                cfg->have_options = cfg->opt_lives != 0u;
                break;
            case SEC_EXTRAS:
                if (name_eq(key, "stage_select"))        cfg->stage_select = atoi(val) != 0;
                else if (name_eq(key, "lives_30"))       cfg->lives_30 = atoi(val) != 0;
                else if (name_eq(key, "infinite_lives")) cfg->infinite_lives = atoi(val) != 0;
                else if (name_eq(key, "start_stage")) {
                    int st = atoi(val);
                    cfg->start_stage = (st >= 1 && st <= 6) ? (unsigned)st : 0u;
                }
                break;
            case SEC_PAD:
                if (name_eq(key, "deadzone")) {
                    int dz = atoi(val);
                    cfg->pad_deadzone = (dz < 0) ? 0 : (dz > 32767) ? 32767 : dz;
                } else {
                    bool found = false;
                    for (unsigned i = 0u; i < CFG_PAD_INPUTS; ++i) {
                        if (name_eq(key, k_pad[i].name)) {
                            bool ok;
                            uint16_t m = snes_mask_from_name(val, &ok);
                            if (ok) {
                                cfg->pad_map[i] = m;
                            } else {
                                fprintf(stderr, "[CONFIG] %s:%u: unknown SNES button '%s'\n", cfg->path, line_no, val);
                            }
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        fprintf(stderr, "[CONFIG] %s:%u: unknown gamepad input '%s'\n", cfg->path, line_no, key);
                    }
                }
                break;
            case SEC_P1:
            case SEC_P2: {
                unsigned p = (section == SEC_P1) ? 0u : 1u;
                for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
                    if (name_eq(key, k_buttons[b].name)) {
                        char bad[32] = "";
                        uint8_t keys[CFG_KEYS_PER_BTN];
                        if (!parse_binding(val, keys, bad, sizeof(bad))) {
                            fprintf(stderr, "[CONFIG] %s:%u: unknown key name '%s' (binding kept for the other keys)\n",
                                    cfg->path, line_no, bad);
                        }
                        memcpy(cfg->keys[p][b], keys, CFG_KEYS_PER_BTN);
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    fclose(fp);
}

uint16_t contra3_config_poll(const Contra3Config *cfg, unsigned player) {
    uint16_t mask = 0u;
    for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
        for (unsigned k = 0u; k < CFG_KEYS_PER_BTN && cfg->keys[player][b][k] != 0u; ++k) {
            if ((GetAsyncKeyState(cfg->keys[player][b][k]) & 0x8000) != 0) {
                mask |= k_buttons[b].mask;
                break;
            }
        }
    }
    /* Opposing directions cancel, as on a real D-pad */
    if ((mask & SNES_BTN_LEFT) && (mask & SNES_BTN_RIGHT)) mask &= (uint16_t)~(SNES_BTN_LEFT | SNES_BTN_RIGHT);
    if ((mask & SNES_BTN_UP) && (mask & SNES_BTN_DOWN))    mask &= (uint16_t)~(SNES_BTN_UP | SNES_BTN_DOWN);
    return mask;
}

uint16_t contra3_config_map_pad(const Contra3Config *cfg, uint16_t xinput_buttons,
                                uint8_t left_trigger, uint8_t right_trigger) {
    uint16_t mask = 0u;
    for (unsigned i = 0u; i < CFG_PAD_INPUTS; ++i) {
        bool down = (k_pad[i].trigger == 1) ? (left_trigger > 64u) :
                    (k_pad[i].trigger == 2) ? (right_trigger > 64u) :
                    ((xinput_buttons & k_pad[i].xinput_mask) != 0u);
        if (down) {
            mask |= cfg->pad_map[i];
        }
    }
    return mask;
}
