#ifndef CONTRA3_C3_OVERLAY_H
#define CONTRA3_C3_OVERLAY_H

#include <stdint.h>
#include <stdbool.h>
#include "c3_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * In-game settings overlay (portable). Rendered into the 256x224 game frame with a built-in
 * 5x7 pixel font, so it works with every presenter and filter. The overlay edits a
 * Contra3Config in place; the frontend applies the values every frame and saves the file
 * when the overlay closes with changes.
 *
 * Frontend responsibilities: open/close it (Esc, gamepad Back+Start), translate keyboard and
 * gamepad input to C3Nav (or C3Key while c3_overlay_capturing), pause emulation while open,
 * draw c3_overlay_render's output instead of the game frame, and perform the returned actions.
 */

#define C3_OVERLAY_W 256
#define C3_OVERLAY_H 224

typedef enum C3Nav { C3NAV_UP, C3NAV_DOWN, C3NAV_LEFT, C3NAV_RIGHT, C3NAV_OK, C3NAV_BACK } C3Nav;

typedef enum C3OverlayAction {
    C3ACT_NONE = 0,
    C3ACT_RESUME,       /* overlay closed */
    C3ACT_SAVE_STATE,
    C3ACT_LOAD_STATE,
    C3ACT_RESET,
    C3ACT_QUIT,
    C3ACT_TEST_RUMBLE,  /* pulse both pads */
    C3ACT_TOGGLE_FULLSCREEN
} C3OverlayAction;

typedef struct C3OverlayEnv {
    bool gl_available;     /* filter choice only matters with the OpenGL presenter */
    bool fullscreen;       /* current state, for display */
    bool muted;            /* current state; toggled through *muted_toggle */
    const char *status;    /* optional message shown at the bottom (e.g. "State saved") */
} C3OverlayEnv;

typedef struct C3Overlay {
    bool     open;
    int      page;
    int      cursor;
    int      scroll;
    int      stack_page[8];
    int      stack_cursor[8];
    int      depth;
    int      capture_player;   /* >= 0 while waiting for a key to bind */
    int      capture_button;
    bool     capture_all;      /* "bind all": walk every button in turn */
    bool     dirty;            /* config changed since opening */
    bool     mute_request;     /* frontend should toggle mute */
    char     message[64];
} C3Overlay;

/* Pages that can be opened directly (BACK still returns to the main menu) */
typedef enum C3OverlayPage {
    C3OV_MAIN = 0, C3OV_VIDEO, C3OV_AUDIO, C3OV_CONTROLS, C3OV_KEYS_P1, C3OV_KEYS_P2, C3OV_PAD, C3OV_CHEATS
} C3OverlayPage;

void            c3_overlay_init(C3Overlay *ov);
void            c3_overlay_open(C3Overlay *ov);
void            c3_overlay_open_at(C3Overlay *ov, C3OverlayPage page);
void            c3_overlay_close(C3Overlay *ov);
bool            c3_overlay_capturing(const C3Overlay *ov);
C3OverlayAction c3_overlay_nav(C3Overlay *ov, Contra3Config *cfg, C3Nav nav);
/* While capturing: bind the key (Escape cancels the current button) */
C3OverlayAction c3_overlay_capture_key(C3Overlay *ov, Contra3Config *cfg, C3Key key);
void            c3_overlay_render(const C3Overlay *ov, const Contra3Config *cfg, const C3OverlayEnv *env,
                                  const uint32_t *game, uint32_t *out);

/* Text helpers usable for on-screen messages (out = 256x224 ARGB) */
void c3_overlay_text(uint32_t *out, int x, int y, const char *text, uint32_t color);
int  c3_overlay_text_width(const char *text);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_C3_OVERLAY_H */
