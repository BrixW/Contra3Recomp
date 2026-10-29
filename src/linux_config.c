/*
 * Contra III: The Alien Wars (USA) - Linux contra3.ini key bindings & preferences
 */

#include "linux_config.h"
#include "snes_ppu.h"

#include <X11/keysym.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ButtonDef {
    const char *name;
    const char *label;
    uint16_t    mask;
    const char *p1;
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

typedef struct KeyName {
    const char *name;
    KeySym      sym;
} KeyName;

static const KeyName k_key_names[] = {
    { "Up", XK_Up }, { "Down", XK_Down }, { "Left", XK_Left }, { "Right", XK_Right },
    { "Enter", XK_Return }, { "Space", XK_space }, { "Tab", XK_Tab }, { "Backspace", XK_BackSpace },
    { "LShift", XK_Shift_L }, { "RShift", XK_Shift_R }, { "LCtrl", XK_Control_L }, { "RCtrl", XK_Control_R },
    { "LAlt", XK_Alt_L }, { "RAlt", XK_Alt_R }, { "Insert", XK_Insert }, { "Delete", XK_Delete },
    { "Home", XK_Home }, { "End", XK_End }, { "PageUp", XK_Prior }, { "PageDown", XK_Next },
    { "Num0", XK_KP_0 }, { "Num1", XK_KP_1 }, { "Num2", XK_KP_2 }, { "Num3", XK_KP_3 },
    { "Num4", XK_KP_4 }, { "Num5", XK_KP_5 }, { "Num6", XK_KP_6 }, { "Num7", XK_KP_7 },
    { "Num8", XK_KP_8 }, { "Num9", XK_KP_9 }, { "NumAdd", XK_KP_Add }, { "NumSub", XK_KP_Subtract },
    { "NumMul", XK_KP_Multiply }, { "NumDiv", XK_KP_Divide }, { "NumDot", XK_KP_Decimal },
    { "Comma", XK_comma }, { "Period", XK_period }, { "Slash", XK_slash }, { "Semicolon", XK_semicolon },
    { "Quote", XK_apostrophe }, { "LBracket", XK_bracketleft }, { "RBracket", XK_bracketright },
    { "Minus", XK_minus }, { "Equals", XK_equal }, { "Backslash", XK_backslash }, { "Grave", XK_grave },
    { "Pause", XK_Pause },
};

static bool name_eq(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a++) != tolower((unsigned char)*b++)) {
            return false;
        }
    }
    return *a == '\0' && *b == '\0';
}

static KeySym key_from_name(const char *name) {
    if (name[0] != '\0' && name[1] == '\0') {
        char c = (char)tolower((unsigned char)name[0]);
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            return (KeySym)(unsigned char)c;
        }
    }
    for (size_t i = 0u; i < sizeof(k_key_names) / sizeof(k_key_names[0]); ++i) {
        if (name_eq(name, k_key_names[i].name)) {
            return k_key_names[i].sym;
        }
    }
    return XStringToKeysym(name); /* allow any other single X11 keysym name in the ini */
}

bool linux_config_key_supported(KeySym sym) {
    return sym != NoSymbol;
}

void linux_config_key_name(KeySym sym, char *out, size_t len) {
    if (sym >= 'a' && sym <= 'z') {
        snprintf(out, len, "%c", (char)toupper((int)sym));
        return;
    }
    if (sym >= '0' && sym <= '9') {
        snprintf(out, len, "%c", (char)sym);
        return;
    }
    for (size_t i = 0u; i < sizeof(k_key_names) / sizeof(k_key_names[0]); ++i) {
        if (k_key_names[i].sym == sym) {
            snprintf(out, len, "%s", k_key_names[i].name);
            return;
        }
    }
    const char *name = XKeysymToString(sym);
    snprintf(out, len, "%s", (name != NULL) ? name : "?");
}

const char *linux_config_button_label(unsigned button) {
    return (button < CFG_BUTTONS) ? k_buttons[button].label : "?";
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

static bool parse_binding(char *value, KeySym out[CFG_KEYS_PER_BTN], char *bad, size_t bad_len) {
    memset(out, 0, CFG_KEYS_PER_BTN * sizeof(*out));
    unsigned n = 0u;
    bool ok = true;
    for (char *tok = strtok(value, ","); tok != NULL; tok = strtok(NULL, ",")) {
        char *name = trim(tok);
        if (name[0] == '\0') {
            continue;
        }
        KeySym sym = key_from_name(name);
        if (sym == NoSymbol) {
            snprintf(bad, bad_len, "%s", name);
            ok = false;
        } else if (n < CFG_KEYS_PER_BTN) {
            out[n++] = sym;
        }
    }
    return ok;
}

static void set_defaults(Contra3LinuxConfig *cfg) {
    char tmp[64], bad[32];
    for (unsigned p = 0u; p < 2u; ++p) {
        for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
            snprintf(tmp, sizeof(tmp), "%s", (p == 0u) ? k_buttons[b].p1 : k_buttons[b].p2);
            parse_binding(tmp, cfg->keys[p][b], bad, sizeof(bad));
        }
    }
    snprintf(cfg->filter, sizeof(cfg->filter), "sharp");
}

static const char *difficulty_name(unsigned d) {
    return (d == 0u) ? "easy" : (d == 2u) ? "normal" : (d == 4u) ? "hard" : NULL;
}

bool linux_config_save(const Contra3LinuxConfig *cfg) {
    FILE *fp = fopen(cfg->path, "w");
    if (fp == NULL) {
        return false;
    }
    fprintf(fp, "; Contra III: The Alien Wars - Linux frontend settings\n");
    fprintf(fp, "; Key names: A-Z, 0-9, Up Down Left Right Enter Space Tab Backspace LShift RShift LCtrl RCtrl\n");
    fprintf(fp, ";   LAlt RAlt Insert Delete Home End PageUp PageDown Num0-Num9 NumAdd NumSub NumMul NumDiv NumDot\n");
    fprintf(fp, ";   Comma Period Slash Semicolon Quote LBracket RBracket Minus Equals Backslash Grave\n");
    fprintf(fp, "; Up to %d keys per button, comma separated. Hotkeys (Tab, P, M, Space, F1-F12, Esc) stay active.\n\n",
            CFG_KEYS_PER_BTN);

    fprintf(fp, "[video]\nfilter = %s        ; Windows OpenGL filter; ignored by the Linux frontend\ninteger = %d\nvsync = %d\n\n",
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

    for (unsigned p = 0u; p < 2u; ++p) {
        fprintf(fp, "[p%u]\n", p + 1u);
        for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
            char line[96] = "";
            for (unsigned k = 0u; k < CFG_KEYS_PER_BTN && cfg->keys[p][b][k] != NoSymbol; ++k) {
                char name[32];
                linux_config_key_name(cfg->keys[p][b][k], name, sizeof(name));
                size_t len = strlen(line);
                snprintf(line + len, sizeof(line) - len, "%s%s", (k == 0u) ? "" : ", ", name);
            }
            fprintf(fp, "%-7s= %-20s ; %s\n", k_buttons[b].name, line, k_buttons[b].label);
        }
        fputc('\n', fp);
    }
    return fclose(fp) == 0;
}

void linux_config_load(Contra3LinuxConfig *cfg, const char *exe_dir) {
    memset(cfg, 0, sizeof(*cfg));
    set_defaults(cfg);
    snprintf(cfg->path, sizeof(cfg->path), "%s/contra3.ini", exe_dir);

    FILE *fp = fopen(cfg->path, "r");
    if (fp == NULL) {
        linux_config_save(cfg);
        return;
    }
    enum { SEC_NONE, SEC_VIDEO, SEC_GAME, SEC_OPTIONS, SEC_EXTRAS, SEC_P1, SEC_P2 } section = SEC_NONE;
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
            section = name_eq(s, "[video]")   ? SEC_VIDEO   : name_eq(s, "[game]")   ? SEC_GAME :
                      name_eq(s, "[options]") ? SEC_OPTIONS : name_eq(s, "[extras]") ? SEC_EXTRAS :
                      name_eq(s, "[p1]")      ? SEC_P1      : name_eq(s, "[p2]")     ? SEC_P2 : SEC_NONE;
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
            case SEC_P1:
            case SEC_P2: {
                unsigned p = (section == SEC_P1) ? 0u : 1u;
                for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
                    if (name_eq(key, k_buttons[b].name)) {
                        char bad[32] = "";
                        KeySym keys[CFG_KEYS_PER_BTN];
                        if (!parse_binding(val, keys, bad, sizeof(bad))) {
                            fprintf(stderr, "[CONFIG] %s:%u: unknown key name '%s' (binding kept for the other keys)\n",
                                    cfg->path, line_no, bad);
                        }
                        memcpy(cfg->keys[p][b], keys, sizeof(keys));
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

uint16_t linux_config_poll(const Contra3LinuxConfig *cfg, unsigned player, Display *dpy) {
    char keys[32];
    XQueryKeymap(dpy, keys);
    uint16_t mask = 0u;
    for (unsigned b = 0u; b < CFG_BUTTONS; ++b) {
        for (unsigned k = 0u; k < CFG_KEYS_PER_BTN && cfg->keys[player][b][k] != NoSymbol; ++k) {
            KeyCode code = XKeysymToKeycode(dpy, cfg->keys[player][b][k]);
            if (code != 0u && (keys[code / 8u] & (1u << (code % 8u))) != 0) {
                mask |= k_buttons[b].mask;
                break;
            }
        }
    }
    if ((mask & SNES_BTN_LEFT) && (mask & SNES_BTN_RIGHT)) mask &= (uint16_t)~(SNES_BTN_LEFT | SNES_BTN_RIGHT);
    if ((mask & SNES_BTN_UP) && (mask & SNES_BTN_DOWN))    mask &= (uint16_t)~(SNES_BTN_UP | SNES_BTN_DOWN);
    return mask;
}
