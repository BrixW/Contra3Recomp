#ifndef CONTRA3_WIN32_CONFIG_H
#define CONTRA3_WIN32_CONFIG_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * contra3.ini: keyboard bindings for both players, gamepad remapping, video/game
 * preferences, the in-game OPTION MODE settings and the extras. Created next to
 * contra3.exe with the defaults on first run; rewritten on exit when anything changed.
 * Each key binding accepts up to four comma-separated key names (A-Z, 0-9, Up, Enter,
 * RShift, Num0-Num9, NumAdd, ...).
 */

#define CFG_BUTTONS      12
#define CFG_KEYS_PER_BTN 4
#define CFG_PAD_INPUTS   10   /* XInput a b x y lb rb lt rt back start */

typedef struct Contra3Config {
    uint8_t  keys[2][CFG_BUTTONS][CFG_KEYS_PER_BTN]; /* [player][button][slot] = VK code, 0 = unused */
    uint16_t pad_map[CFG_PAD_INPUTS];                /* XInput input -> SNES_BTN_* mask (0 = unused) */
    int      pad_deadzone;                           /* left-stick threshold, 0..32767 */

    char     filter[16];                             /* sharp | nearest | crt */
    bool     integer_scale;
    bool     vsync;
    bool     lag_model;

    /* In-game OPTION MODE settings, restored after power-on (0 = leave the game default) */
    unsigned opt_lives;                              /* 3 / 5 / 7 (any 1..99) */
    unsigned opt_difficulty;                         /* 0 easy, 2 normal, 4 hard */
    unsigned opt_sound;                              /* 0 stereo, 2 monaural */
    bool     have_options;

    /* Extras (see include/contra3_extras.h) */
    bool     stage_select;
    bool     lives_30;
    bool     infinite_lives;
    unsigned start_stage;                            /* 0 = off, 1..6 */

    char     path[MAX_PATH];
} Contra3Config;

/* Load (or create with defaults) <exe_dir>/contra3.ini; unknown names are reported to stderr */
void        contra3_config_load(Contra3Config *cfg, const char *exe_dir);
bool        contra3_config_save(const Contra3Config *cfg);
uint16_t    contra3_config_poll(const Contra3Config *cfg, unsigned player); /* keyboard -> SNES_BTN_* */

/* Gamepad mapping helpers: XInput wButtons / triggers -> SNES_BTN_* using pad_map */
uint16_t    contra3_config_map_pad(const Contra3Config *cfg, uint16_t xinput_buttons,
                                   uint8_t left_trigger, uint8_t right_trigger);

/* Interactive rebinding support */
const char *contra3_config_button_label(unsigned button);       /* "UP", "B (jump)", ... */
bool        contra3_config_key_supported(uint8_t vk);           /* has a name the ini can store */
void        contra3_config_key_name(uint8_t vk, char *out, size_t len);

#endif /* CONTRA3_WIN32_CONFIG_H */
