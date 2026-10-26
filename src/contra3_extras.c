/*
 * Contra III: The Alien Wars (USA) - Restored Contra Spirits unlocks
 */

#include "contra3_extras.h"

#define STATE_TITLE      0x03u
#define ADDR_GAME_STATE  0x000032u
#define ADDR_STAGE_SEL   0x001E60u
#define ADDR_START_LIVES 0x001E3Cu

void contra3_apply_unlocks(SnesCpuContext *ctx, unsigned unlocks) {
    if (unlocks == 0u || snes_bus_read8(ctx, ADDR_GAME_STATE) != STATE_TITLE) {
        return;
    }
    if ((unlocks & CONTRA3_UNLOCK_STAGE_SELECT) != 0u) {
        snes_bus_write16(ctx, ADDR_STAGE_SEL, 0x0001u);
    }
    if ((unlocks & CONTRA3_UNLOCK_LIVES_30) != 0u) {
        snes_bus_write16(ctx, ADDR_START_LIVES, 0x001Eu);
    }
}
