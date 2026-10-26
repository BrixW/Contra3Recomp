#ifndef CONTRA3_WIN32_CONFIG_H
#define CONTRA3_WIN32_CONFIG_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * contra3.ini: keyboard bindings for both players plus persisted video preferences.
 * Created next to contra3.exe with the defaults on first run; rewritten on exit only
 * when a preference changed at runtime (F3/F9/F10). Each binding accepts up to four
 * comma-separated key names (A-Z, 0-9, Up, Enter, RShift, Num0-Num9, NumAdd, ...).
 */

#define CFG_BUTTONS      12
#define CFG_KEYS_PER_BTN 4

typedef struct Contra3Config {
    uint8_t keys[2][CFG_BUTTONS][CFG_KEYS_PER_BTN]; /* [player][button][slot] = VK code, 0 = unused */
    char    filter[16];                             /* sharp | nearest | crt */
    bool    integer_scale;
    bool    vsync;
    bool    lag_model;
    char    path[MAX_PATH];
} Contra3Config;

/* Load (or create with defaults) <exe_dir>/contra3.ini; unknown keys are reported to stderr */
void     contra3_config_load(Contra3Config *cfg, const char *exe_dir);
bool     contra3_config_save(const Contra3Config *cfg);
uint16_t contra3_config_poll(const Contra3Config *cfg, unsigned player); /* SNES_BTN_* mask */

#endif /* CONTRA3_WIN32_CONFIG_H */
