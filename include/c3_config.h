#ifndef CONTRA3_C3_CONFIG_H
#define CONTRA3_C3_CONFIG_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * contra3.ini (portable): keyboard bindings for both players, gamepad remapping, rumble,
 * video/game preferences, the in-game OPTION MODE settings and the extras.
 *
 * Keys are stored as abstract C3Key values; each frontend maps its native key codes
 * (Win32 virtual keys, SDL scancodes) to C3Key. Created next to the executable with the
 * defaults on first run and rewritten when a setting changes.
 */

typedef enum C3Key {
    C3K_NONE = 0,
    C3K_A, C3K_B, C3K_C, C3K_D, C3K_E, C3K_F, C3K_G, C3K_H, C3K_I, C3K_J, C3K_K, C3K_L, C3K_M,
    C3K_N, C3K_O, C3K_P, C3K_Q, C3K_R, C3K_S, C3K_T, C3K_U, C3K_V, C3K_W, C3K_X, C3K_Y, C3K_Z,
    C3K_0, C3K_1, C3K_2, C3K_3, C3K_4, C3K_5, C3K_6, C3K_7, C3K_8, C3K_9,
    C3K_UP, C3K_DOWN, C3K_LEFT, C3K_RIGHT,
    C3K_ENTER, C3K_SPACE, C3K_TAB, C3K_BACKSPACE, C3K_ESCAPE,
    C3K_LSHIFT, C3K_RSHIFT, C3K_LCTRL, C3K_RCTRL, C3K_LALT, C3K_RALT,
    C3K_INSERT, C3K_DELETE, C3K_HOME, C3K_END, C3K_PAGEUP, C3K_PAGEDOWN,
    C3K_NUM0, C3K_NUM1, C3K_NUM2, C3K_NUM3, C3K_NUM4, C3K_NUM5, C3K_NUM6, C3K_NUM7, C3K_NUM8, C3K_NUM9,
    C3K_NUMADD, C3K_NUMSUB, C3K_NUMMUL, C3K_NUMDIV, C3K_NUMDOT, C3K_NUMENTER,
    C3K_COMMA, C3K_PERIOD, C3K_SLASH, C3K_SEMICOLON, C3K_QUOTE, C3K_LBRACKET, C3K_RBRACKET,
    C3K_MINUS, C3K_EQUALS, C3K_BACKSLASH, C3K_GRAVE,
    C3K_F1, C3K_F2, C3K_F3, C3K_F4, C3K_F5, C3K_F6, C3K_F7, C3K_F8, C3K_F9, C3K_F10, C3K_F11, C3K_F12,
    C3K_COUNT
} C3Key;

/* Gamepad inputs that can be remapped (XInput / SDL GameController layout) */
typedef enum C3PadInput {
    C3P_A = 0, C3P_B, C3P_X, C3P_Y, C3P_LB, C3P_RB, C3P_LT, C3P_RT, C3P_BACK, C3P_START,
    C3P_COUNT
} C3PadInput;

#define C3_BUTTONS      12      /* SNES buttons: up down left right b y x a l r start select */
#define C3_KEYS_PER_BTN 4

typedef struct Contra3Config {
    uint8_t  keys[2][C3_BUTTONS][C3_KEYS_PER_BTN];  /* [player][button][slot] = C3Key, 0 = unused */
    uint16_t pad_map[C3P_COUNT];                      /* pad input -> SNES_BTN_* mask (0 = unused) */
    int      pad_deadzone;                            /* left-stick threshold, 0..32767 */

    bool     rumble;                                  /* controller vibration on deaths, bombs, boss blasts */
    unsigned rumble_strength;                         /* 0..100 % */

    char     filter[16];                              /* sharp | nearest | crt */
    bool     integer_scale;
    bool     vsync;
    bool     lag_model;
    unsigned volume;                                  /* 0..100 % */

    /* In-game OPTION MODE settings, restored after power-on */
    unsigned opt_lives;                               /* 3 / 5 / 7 (any 1..99) */
    unsigned opt_difficulty;                          /* 0 easy, 2 normal, 4 hard */
    unsigned opt_sound;                               /* 0 stereo, 2 monaural */
    bool     have_options;

    /* Extras (include/contra3_extras.h) */
    bool     stage_select;
    bool     lives_30;
    bool     infinite_lives;
    unsigned start_stage;                             /* 0 = off, 1..6 */

    char     path[1024];
} Contra3Config;

void        c3_config_defaults(Contra3Config *cfg);
/* Load <dir>/contra3.ini (written with defaults if missing); unknown names are reported to stderr */
void        c3_config_load(Contra3Config *cfg, const char *dir);
bool        c3_config_save(const Contra3Config *cfg);

/* Keyboard state -> SNES_BTN_* mask for a player; is_down reports whether a C3Key is held */
uint16_t    c3_config_keys_to_pad(const Contra3Config *cfg, unsigned player, bool (*is_down)(C3Key key, void *user), void *user);
/* Gamepad state -> SNES_BTN_* mask through pad_map (directions are handled by the frontend) */
uint16_t    c3_config_map_pad(const Contra3Config *cfg, const bool down[C3P_COUNT]);

/* Names (shared by the ini file and the overlay) */
const char *c3_key_name(C3Key key);              /* "Up", "Num8", "A" ... ("?" if none) */
C3Key       c3_key_from_name(const char *name);  /* case-insensitive; C3K_NONE if unknown */
const char *c3_button_name(unsigned button);     /* ini key: "up", "b", ... */
const char *c3_button_label(unsigned button);    /* "UP", "B (JUMP)", ... */
uint16_t    c3_button_mask(unsigned button);     /* SNES_BTN_* */
const char *c3_pad_input_name(C3PadInput input); /* "a", "lb", ... */
const char *c3_snes_name_from_mask(uint16_t mask);/* "b", "start", "none" ... */
uint16_t    c3_snes_mask_next(uint16_t mask, int dir); /* cycle through none + 12 buttons */

/* Remove a key from every button of a player, then make it the only key of one button */
void        c3_config_bind_key(Contra3Config *cfg, unsigned player, unsigned button, C3Key key);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_C3_CONFIG_H */
