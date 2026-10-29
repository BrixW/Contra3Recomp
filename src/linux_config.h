#ifndef CONTRA3_LINUX_CONFIG_H
#define CONTRA3_LINUX_CONFIG_H

#include <X11/Xlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * contra3.ini for the Linux frontend: the same sections and key names as the
 * Win32 build, but bindings are X11 keysyms. Each button accepts up to four
 * comma-separated key names (A-Z, 0-9, Up, Enter, RShift, Num0-Num9, ...).
 */

#define CFG_BUTTONS      12
#define CFG_KEYS_PER_BTN 4

typedef struct Contra3LinuxConfig {
    KeySym   keys[2][CFG_BUTTONS][CFG_KEYS_PER_BTN]; /* [player][button][slot], NoSymbol = unused */

    char     filter[16];                            /* kept for ini compatibility; not used by X11 */
    bool     integer_scale;
    bool     vsync;
    bool     lag_model;

    /* In-game OPTION MODE settings, restored after power-on (0 = leave the game default) */
    unsigned opt_lives;
    unsigned opt_difficulty;
    unsigned opt_sound;
    bool     have_options;

    /* Extras (see include/contra3_extras.h) */
    bool     stage_select;
    bool     lives_30;
    bool     infinite_lives;
    unsigned start_stage;

    char     path[512];
} Contra3LinuxConfig;

void        linux_config_load(Contra3LinuxConfig *cfg, const char *exe_dir);
bool        linux_config_save(const Contra3LinuxConfig *cfg);
uint16_t    linux_config_poll(const Contra3LinuxConfig *cfg, unsigned player, Display *dpy);

const char *linux_config_button_label(unsigned button);
bool        linux_config_key_supported(KeySym sym);
void        linux_config_key_name(KeySym sym, char *out, size_t len);

#endif /* CONTRA3_LINUX_CONFIG_H */
