/*
 * Contra III: The Alien Wars (USA) - contra3.ini (portable configuration)
 */

#include "c3_config.h"
#include "snes_ppu.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ButtonDef {
    const char *name;     /* ini key and gamepad target name */
    const char *label;    /* shown by the overlay / rebinding */
    uint16_t    mask;
    const char *p1;       /* default bindings */
    const char *p2;
} ButtonDef;

static const ButtonDef k_buttons[C3_BUTTONS] = {
    { "up",     "UP",                   SNES_BTN_UP,     "Up",                "Num8" },
    { "down",   "DOWN",                 SNES_BTN_DOWN,   "Down",              "Num5" },
    { "left",   "LEFT",                 SNES_BTN_LEFT,   "Left",              "Num4" },
    { "right",  "RIGHT",                SNES_BTN_RIGHT,  "Right",             "Num6" },
    { "b",      "B (JUMP)",             SNES_BTN_B,      "Z, J",              "Num1" },
    { "y",      "Y (SHOOT)",            SNES_BTN_Y,      "X, K",              "Num2" },
    { "x",      "X (SWITCH WEAPON)",    SNES_BTN_X,      "A, U",              "Num3" },
    { "a",      "A (MEGA BOMB)",        SNES_BTN_A,      "S, I",              "Num0" },
    { "l",      "L (LOCK POSITION)",    SNES_BTN_L,      "Q, L",              "Num7" },
    { "r",      "R (DUAL FIRE)",        SNES_BTN_R,      "E, O",              "Num9" },
    { "start",  "START",                SNES_BTN_START,  "Enter",             "NumAdd" },
    { "select", "SELECT",               SNES_BTN_SELECT, "RShift, Backspace", "NumSub" },
};

static const struct { const char *name; const char *def; } k_pad[C3P_COUNT] = {
    { "a", "b" }, { "b", "a" }, { "x", "y" }, { "y", "x" },   /* Xbox A -> SNES B (jump) etc. */
    { "lb", "l" }, { "rb", "r" }, { "lt", "l" }, { "rt", "r" },
    { "back", "select" }, { "start", "start" },
};

static const char *const k_key_names[C3K_COUNT] = {
    [C3K_NONE] = "?",
    [C3K_A] = "A", [C3K_B] = "B", [C3K_C] = "C", [C3K_D] = "D", [C3K_E] = "E", [C3K_F] = "F", [C3K_G] = "G",
    [C3K_H] = "H", [C3K_I] = "I", [C3K_J] = "J", [C3K_K] = "K", [C3K_L] = "L", [C3K_M] = "M", [C3K_N] = "N",
    [C3K_O] = "O", [C3K_P] = "P", [C3K_Q] = "Q", [C3K_R] = "R", [C3K_S] = "S", [C3K_T] = "T", [C3K_U] = "U",
    [C3K_V] = "V", [C3K_W] = "W", [C3K_X] = "X", [C3K_Y] = "Y", [C3K_Z] = "Z",
    [C3K_0] = "0", [C3K_1] = "1", [C3K_2] = "2", [C3K_3] = "3", [C3K_4] = "4",
    [C3K_5] = "5", [C3K_6] = "6", [C3K_7] = "7", [C3K_8] = "8", [C3K_9] = "9",
    [C3K_UP] = "Up", [C3K_DOWN] = "Down", [C3K_LEFT] = "Left", [C3K_RIGHT] = "Right",
    [C3K_ENTER] = "Enter", [C3K_SPACE] = "Space", [C3K_TAB] = "Tab", [C3K_BACKSPACE] = "Backspace",
    [C3K_ESCAPE] = "Escape",
    [C3K_LSHIFT] = "LShift", [C3K_RSHIFT] = "RShift", [C3K_LCTRL] = "LCtrl", [C3K_RCTRL] = "RCtrl",
    [C3K_LALT] = "LAlt", [C3K_RALT] = "RAlt",
    [C3K_INSERT] = "Insert", [C3K_DELETE] = "Delete", [C3K_HOME] = "Home", [C3K_END] = "End",
    [C3K_PAGEUP] = "PageUp", [C3K_PAGEDOWN] = "PageDown",
    [C3K_NUM0] = "Num0", [C3K_NUM1] = "Num1", [C3K_NUM2] = "Num2", [C3K_NUM3] = "Num3", [C3K_NUM4] = "Num4",
    [C3K_NUM5] = "Num5", [C3K_NUM6] = "Num6", [C3K_NUM7] = "Num7", [C3K_NUM8] = "Num8", [C3K_NUM9] = "Num9",
    [C3K_NUMADD] = "NumAdd", [C3K_NUMSUB] = "NumSub", [C3K_NUMMUL] = "NumMul", [C3K_NUMDIV] = "NumDiv",
    [C3K_NUMDOT] = "NumDot", [C3K_NUMENTER] = "NumEnter",
    [C3K_COMMA] = "Comma", [C3K_PERIOD] = "Period", [C3K_SLASH] = "Slash", [C3K_SEMICOLON] = "Semicolon",
    [C3K_QUOTE] = "Quote", [C3K_LBRACKET] = "LBracket", [C3K_RBRACKET] = "RBracket", [C3K_MINUS] = "Minus",
    [C3K_EQUALS] = "Equals", [C3K_BACKSLASH] = "Backslash", [C3K_GRAVE] = "Grave",
    [C3K_F1] = "F1", [C3K_F2] = "F2", [C3K_F3] = "F3", [C3K_F4] = "F4", [C3K_F5] = "F5", [C3K_F6] = "F6",
    [C3K_F7] = "F7", [C3K_F8] = "F8", [C3K_F9] = "F9", [C3K_F10] = "F10", [C3K_F11] = "F11", [C3K_F12] = "F12",
};

static bool name_eq(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a++) != tolower((unsigned char)*b++)) {
            return false;
        }
    }
    return *a == '\0' && *b == '\0';
}

const char *c3_key_name(C3Key key) {
    return (key > C3K_NONE && key < C3K_COUNT && k_key_names[key] != NULL) ? k_key_names[key] : "?";
}

C3Key c3_key_from_name(const char *name) {
    for (int k = 1; k < C3K_COUNT; ++k) {
        if (k_key_names[k] != NULL && name_eq(name, k_key_names[k])) {
            return (C3Key)k;
        }
    }
    return C3K_NONE;
}

const char *c3_button_name(unsigned button)  { return (button < C3_BUTTONS) ? k_buttons[button].name : "?"; }
const char *c3_button_label(unsigned button) { return (button < C3_BUTTONS) ? k_buttons[button].label : "?"; }
uint16_t    c3_button_mask(unsigned button)  { return (button < C3_BUTTONS) ? k_buttons[button].mask : 0u; }
const char *c3_pad_input_name(C3PadInput in) { return ((unsigned)in < C3P_COUNT) ? k_pad[in].name : "?"; }

static uint16_t snes_mask_from_name(const char *name, bool *ok) {
    *ok = true;
    if (name_eq(name, "none") || name[0] == '\0') {
        return 0u;
    }
    for (unsigned b = 0u; b < C3_BUTTONS; ++b) {
        if (name_eq(name, k_buttons[b].name)) {
            return k_buttons[b].mask;
        }
    }
    *ok = false;
    return 0u;
}

const char *c3_snes_name_from_mask(uint16_t mask) {
    for (unsigned b = 0u; b < C3_BUTTONS; ++b) {
        if (k_buttons[b].mask == mask) {
            return k_buttons[b].name;
        }
    }
    return "none";
}

uint16_t c3_snes_mask_next(uint16_t mask, int dir) {
    int idx = -1; /* -1 = none */
    for (unsigned b = 0u; b < C3_BUTTONS; ++b) {
        if (k_buttons[b].mask == mask) {
            idx = (int)b;
        }
    }
    idx += (dir < 0) ? -1 : 1;
    if (idx < -1) idx = C3_BUTTONS - 1;
    if (idx >= (int)C3_BUTTONS) idx = -1;
    return (idx < 0) ? 0u : k_buttons[idx].mask;
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
static bool parse_binding(char *value, uint8_t out[C3_KEYS_PER_BTN], char *bad, size_t bad_len) {
    memset(out, 0, C3_KEYS_PER_BTN);
    unsigned n = 0u;
    bool ok = true;
    for (char *tok = strtok(value, ","); tok != NULL; tok = strtok(NULL, ",")) {
        char *name = trim(tok);
        if (name[0] == '\0') {
            continue;
        }
        C3Key key = c3_key_from_name(name);
        if (key == C3K_NONE) {
            snprintf(bad, bad_len, "%s", name);
            ok = false;
        } else if (n < C3_KEYS_PER_BTN) {
            out[n++] = (uint8_t)key;
        }
    }
    return ok;
}

void c3_config_defaults(Contra3Config *cfg) {
    char tmp[64], bad[32];
    char path[sizeof(cfg->path)];
    memcpy(path, cfg->path, sizeof(path));
    memset(cfg, 0, sizeof(*cfg));
    memcpy(cfg->path, path, sizeof(path));
    for (unsigned p = 0u; p < 2u; ++p) {
        for (unsigned b = 0u; b < C3_BUTTONS; ++b) {
            snprintf(tmp, sizeof(tmp), "%s", (p == 0u) ? k_buttons[b].p1 : k_buttons[b].p2);
            parse_binding(tmp, cfg->keys[p][b], bad, sizeof(bad));
        }
    }
    for (unsigned i = 0u; i < C3P_COUNT; ++i) {
        bool ok;
        cfg->pad_map[i] = snes_mask_from_name(k_pad[i].def, &ok);
    }
    cfg->pad_deadzone = 12000;
    cfg->rumble = true;
    cfg->rumble_strength = 100u;
    cfg->volume = 100u;
    snprintf(cfg->filter, sizeof(cfg->filter), "sharp");
}

static const char *difficulty_name(unsigned d) {
    return (d == 0u) ? "easy" : (d == 2u) ? "normal" : (d == 4u) ? "hard" : NULL;
}

bool c3_config_save(const Contra3Config *cfg) {
    FILE *fp = fopen(cfg->path, "w");
    if (fp == NULL) {
        return false;
    }
    fprintf(fp, "; Contra III: The Alien Wars - static recompilation settings\n");
    fprintf(fp, "; Most of these can be changed in-game from the overlay (Esc, or Back+Start on a gamepad).\n");
    fprintf(fp, "; Key names: A-Z, 0-9, Up Down Left Right Enter Space Tab Backspace LShift RShift LCtrl RCtrl\n");
    fprintf(fp, ";   LAlt RAlt Insert Delete Home End PageUp PageDown Num0-Num9 NumAdd NumSub NumMul NumDiv NumDot\n");
    fprintf(fp, ";   NumEnter Comma Period Slash Semicolon Quote LBracket RBracket Minus Equals Backslash Grave\n");
    fprintf(fp, "; Up to %d keys per button, comma separated.\n\n", C3_KEYS_PER_BTN);

    fprintf(fp, "[video]\nfilter = %s        ; sharp | nearest | crt\ninteger = %d\nvsync = %d\n\n",
            cfg->filter, cfg->integer_scale ? 1 : 0, cfg->vsync ? 1 : 0);
    fprintf(fp, "[audio]\nvolume = %u        ; 0-100\n\n", cfg->volume);
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

    fprintf(fp, "[rumble]           ; controller vibration on player deaths, Mega Bombs and boss explosions\n");
    fprintf(fp, "enabled = %d\nstrength = %u      ; 0-100\n\n", cfg->rumble ? 1 : 0, cfg->rumble_strength);

    fprintf(fp, "[gamepad]          ; pads 1 & 2; targets: b y x a l r start select up down left right none\n");
    fprintf(fp, "deadzone = %d\n", cfg->pad_deadzone);
    for (unsigned i = 0u; i < C3P_COUNT; ++i) {
        fprintf(fp, "%-6s = %s\n", k_pad[i].name, c3_snes_name_from_mask(cfg->pad_map[i]));
    }

    for (unsigned p = 0u; p < 2u; ++p) {
        fprintf(fp, "\n[p%u]\n", p + 1u);
        for (unsigned b = 0u; b < C3_BUTTONS; ++b) {
            char line[96] = "";
            for (unsigned k = 0u; k < C3_KEYS_PER_BTN && cfg->keys[p][b][k] != 0u; ++k) {
                size_t len = strlen(line);
                snprintf(line + len, sizeof(line) - len, "%s%s", (k == 0u) ? "" : ", ", c3_key_name((C3Key)cfg->keys[p][b][k]));
            }
            fprintf(fp, "%-7s= %-20s ; %s\n", k_buttons[b].name, line, k_buttons[b].label);
        }
    }
    return fclose(fp) == 0;
}

void c3_config_load(Contra3Config *cfg, const char *dir) {
    memset(cfg, 0, sizeof(*cfg));
    snprintf(cfg->path, sizeof(cfg->path), "%s/contra3.ini", (dir != NULL && dir[0] != '\0') ? dir : ".");
    c3_config_defaults(cfg);

    FILE *fp = fopen(cfg->path, "r");
    if (fp == NULL) {
        c3_config_save(cfg); /* first run: write the defaults so they can be edited */
        return;
    }
    enum { SEC_NONE, SEC_VIDEO, SEC_AUDIO, SEC_GAME, SEC_OPTIONS, SEC_EXTRAS, SEC_RUMBLE, SEC_PAD, SEC_P1, SEC_P2 } section = SEC_NONE;
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
            section = name_eq(s, "[video]")   ? SEC_VIDEO   : name_eq(s, "[audio]")   ? SEC_AUDIO :
                      name_eq(s, "[game]")    ? SEC_GAME    : name_eq(s, "[options]") ? SEC_OPTIONS :
                      name_eq(s, "[extras]")  ? SEC_EXTRAS  : name_eq(s, "[rumble]")  ? SEC_RUMBLE :
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
        int iv = atoi(val);
        switch (section) {
            case SEC_VIDEO:
                if (name_eq(key, "filter"))       snprintf(cfg->filter, sizeof(cfg->filter), "%s", val);
                else if (name_eq(key, "integer")) cfg->integer_scale = iv != 0;
                else if (name_eq(key, "vsync"))   cfg->vsync = iv != 0;
                break;
            case SEC_AUDIO:
                if (name_eq(key, "volume")) cfg->volume = (iv < 0) ? 0u : (iv > 100) ? 100u : (unsigned)iv;
                break;
            case SEC_GAME:
                if (name_eq(key, "lag_model")) cfg->lag_model = iv != 0;
                break;
            case SEC_OPTIONS:
                if (name_eq(key, "lives")) {
                    cfg->opt_lives = (unsigned)iv;
                } else if (name_eq(key, "difficulty")) {
                    cfg->opt_difficulty = name_eq(val, "easy") ? 0u : name_eq(val, "normal") ? 2u :
                                          name_eq(val, "hard") ? 4u : (unsigned)iv;
                } else if (name_eq(key, "sound")) {
                    cfg->opt_sound = (name_eq(val, "mono") || name_eq(val, "monaural")) ? 2u : 0u;
                }
                cfg->have_options = cfg->opt_lives != 0u;
                break;
            case SEC_EXTRAS:
                if (name_eq(key, "stage_select"))        cfg->stage_select = iv != 0;
                else if (name_eq(key, "lives_30"))       cfg->lives_30 = iv != 0;
                else if (name_eq(key, "infinite_lives")) cfg->infinite_lives = iv != 0;
                else if (name_eq(key, "start_stage"))    cfg->start_stage = (iv >= 1 && iv <= 6) ? (unsigned)iv : 0u;
                break;
            case SEC_RUMBLE:
                if (name_eq(key, "enabled"))       cfg->rumble = iv != 0;
                else if (name_eq(key, "strength")) cfg->rumble_strength = (iv < 0) ? 0u : (iv > 100) ? 100u : (unsigned)iv;
                break;
            case SEC_PAD:
                if (name_eq(key, "deadzone")) {
                    cfg->pad_deadzone = (iv < 0) ? 0 : (iv > 32767) ? 32767 : iv;
                } else {
                    bool found = false;
                    for (unsigned i = 0u; i < C3P_COUNT; ++i) {
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
                for (unsigned b = 0u; b < C3_BUTTONS; ++b) {
                    if (name_eq(key, k_buttons[b].name)) {
                        char bad[32] = "";
                        uint8_t keys[C3_KEYS_PER_BTN];
                        if (!parse_binding(val, keys, bad, sizeof(bad))) {
                            fprintf(stderr, "[CONFIG] %s:%u: unknown key name '%s' (binding kept for the other keys)\n",
                                    cfg->path, line_no, bad);
                        }
                        memcpy(cfg->keys[p][b], keys, C3_KEYS_PER_BTN);
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

uint16_t c3_config_keys_to_pad(const Contra3Config *cfg, unsigned player, bool (*is_down)(C3Key key, void *user), void *user) {
    uint16_t mask = 0u;
    for (unsigned b = 0u; b < C3_BUTTONS; ++b) {
        for (unsigned k = 0u; k < C3_KEYS_PER_BTN && cfg->keys[player][b][k] != 0u; ++k) {
            if (is_down((C3Key)cfg->keys[player][b][k], user)) {
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

uint16_t c3_config_map_pad(const Contra3Config *cfg, const bool down[C3P_COUNT]) {
    uint16_t mask = 0u;
    for (unsigned i = 0u; i < C3P_COUNT; ++i) {
        if (down[i]) {
            mask |= cfg->pad_map[i];
        }
    }
    return mask;
}

void c3_config_bind_key(Contra3Config *cfg, unsigned player, unsigned button, C3Key key) {
    if (player > 1u || button >= C3_BUTTONS || key == C3K_NONE) {
        return;
    }
    for (unsigned b = 0u; b < C3_BUTTONS; ++b) {
        for (unsigned k = 0u; k < C3_KEYS_PER_BTN; ++k) {
            if (cfg->keys[player][b][k] == (uint8_t)key) {
                memmove(&cfg->keys[player][b][k], &cfg->keys[player][b][k + 1u], C3_KEYS_PER_BTN - 1u - k);
                cfg->keys[player][b][C3_KEYS_PER_BTN - 1u] = 0u;
                k--;
            }
        }
    }
    memset(cfg->keys[player][button], 0, C3_KEYS_PER_BTN);
    cfg->keys[player][button][0] = (uint8_t)key;
}
