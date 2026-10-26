/*
 * Contra III: The Alien Wars (USA) - contra3.ini key bindings & preferences
 */

#include "win32_config.h"
#include "snes_ppu.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ButtonDef {
    const char *name;     /* ini key */
    const char *comment;
    uint16_t    mask;
    const char *p1;       /* default bindings */
    const char *p2;
} ButtonDef;

static const ButtonDef k_buttons[CFG_BUTTONS] = {
    { "up",     "D-pad up / aim up",        SNES_BTN_UP,     "Up",             "Num8" },
    { "down",   "D-pad down / prone",       SNES_BTN_DOWN,   "Down",           "Num5" },
    { "left",   "D-pad left",               SNES_BTN_LEFT,   "Left",           "Num4" },
    { "right",  "D-pad right",              SNES_BTN_RIGHT,  "Right",          "Num6" },
    { "b",      "B: jump",                  SNES_BTN_B,      "Z, J",           "Num1" },
    { "y",      "Y: shoot",                 SNES_BTN_Y,      "X, K",           "Num2" },
    { "x",      "X: switch weapon",         SNES_BTN_X,      "A, U",           "Num3" },
    { "a",      "A: mega bomb",             SNES_BTN_A,      "S, I",           "Num0" },
    { "l",      "L: lock position / aim",   SNES_BTN_L,      "Q, L",           "Num7" },
    { "r",      "R: dual-wield fire",       SNES_BTN_R,      "E, O",           "Num9" },
    { "start",  "Start / pause",            SNES_BTN_START,  "Enter",          "NumAdd" },
    { "select", "Select",                   SNES_BTN_SELECT, "RShift, Backspace", "NumSub" },
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

static void name_from_vk(uint8_t vk, char *out, size_t len) {
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
    snprintf(cfg->filter, sizeof(cfg->filter), "sharp");
    cfg->integer_scale = false;
    cfg->vsync = false;
    cfg->lag_model = false;
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
    fprintf(fp, "; Up to %d keys per button, comma separated. Hotkeys (Tab, P, M, Space, F1-F12, Esc) stay active.\n\n",
            CFG_KEYS_PER_BTN);
    fprintf(fp, "[video]\nfilter = %s        ; sharp | nearest | crt\ninteger = %d\nvsync = %d\n\n",
            cfg->filter, cfg->integer_scale ? 1 : 0, cfg->vsync ? 1 : 0);
    fprintf(fp, "[game]\nlag_model = %d     ; authentic slowdown (F3)\n", cfg->lag_model ? 1 : 0);
    for (unsigned p = 0u; p < 2u; ++p) {
        fprintf(fp, "\n[p%u]\n", p + 1u);
        for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
            char line[96] = "";
            for (unsigned k = 0u; k < CFG_KEYS_PER_BTN && cfg->keys[p][b][k] != 0u; ++k) {
                char name[16];
                name_from_vk(cfg->keys[p][b][k], name, sizeof(name));
                size_t len = strlen(line);
                snprintf(line + len, sizeof(line) - len, "%s%s", (k == 0u) ? "" : ", ", name);
            }
            fprintf(fp, "%-7s= %-20s ; %s\n", k_buttons[b].name, line, k_buttons[b].comment);
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
    char line[256];
    int section = -1; /* 0 video, 1 game, 2 p1, 3 p2 */
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
            section = name_eq(s, "[video]") ? 0 : name_eq(s, "[game]") ? 1 : name_eq(s, "[p1]") ? 2 : name_eq(s, "[p2]") ? 3 : -1;
            continue;
        }
        char *eq = strchr(s, '=');
        if (eq == NULL) {
            continue;
        }
        *eq = '\0';
        char *key = trim(s);
        char *val = trim(eq + 1);
        if (section == 0) {
            if (name_eq(key, "filter")) {
                snprintf(cfg->filter, sizeof(cfg->filter), "%s", val);
            } else if (name_eq(key, "integer")) {
                cfg->integer_scale = atoi(val) != 0;
            } else if (name_eq(key, "vsync")) {
                cfg->vsync = atoi(val) != 0;
            }
        } else if (section == 1) {
            if (name_eq(key, "lag_model")) {
                cfg->lag_model = atoi(val) != 0;
            }
        } else if (section >= 2) {
            unsigned p = (unsigned)(section - 2);
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
