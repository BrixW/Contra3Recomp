/*
 * Contra III: The Alien Wars (USA) - gameplay events for host effects (rumble)
 */

#include "contra3_events.h"

#include <string.h>

#define STATE_GAMEPLAY   0x09u
#define SFX_RING         0x000100u
#define SFX_WRITE_IDX    0x000056u
#define SFX_DEATH        0x3Fu   /* death scream (most stages) */
#define SFX_DEATH_ALT    0x43u   /* death scream variant (e.g. Stage 3) */
#define SFX_MEGA_BOMB    0x5Au
#define SFX_BOSS_BLAST   0x56u
#define SNES_BTN_A_MASK  0x0080u
#define MAX_NEW_SFX      32u    /* more than this in one frame means the ring was reset */

static const uint32_t k_slot[2]  = { 0x000200u, 0x000240u };
static const uint32_t k_lives[2] = { 0x001F8Au, 0x001FCAu };
#define SLOT_STATE       0x12u
#define STATE_DYING      0x0007u

void contra3_events_reset(Contra3EventState *st) {
    memset(st, 0, sizeof(*st));
}

static bool in_death_sequence(uint16_t handler) {
    return (handler & 0xFF00u) == 0xAC00u;
}

unsigned contra3_events_poll(SnesCpuContext *ctx, Contra3EventState *st, uint16_t pad1, uint16_t pad2) {
    unsigned ev = 0u;
    uint8_t write = snes_bus_read8(ctx, SFX_WRITE_IDX);
    uint16_t handler[2], state[2], lives[2];
    for (unsigned p = 0u; p < 2u; ++p) {
        handler[p] = snes_bus_read16(ctx, k_slot[p]);
        state[p]   = snes_bus_read16(ctx, k_slot[p] + SLOT_STATE);
        lives[p]   = snes_bus_read16(ctx, k_lives[p]);
    }
    bool playing = snes_bus_read8(ctx, 0x000032u) == STATE_GAMEPLAY;

    uint8_t pending = (uint8_t)(write - st->sfx_read);
    if (!st->synced || pending > MAX_NEW_SFX || !playing) {
        st->sfx_read = write;   /* first call, ring reset, or not in gameplay: just resync */
        st->synced = true;
        pending = 0u;
    }
    bool death = false, bomb = false;
    for (uint8_t i = 0u; i < pending; ++i) {
        uint8_t id = snes_bus_read8(ctx, SFX_RING + (uint8_t)(st->sfx_read + i));
        if (id == SFX_DEATH || id == SFX_DEATH_ALT) death = true;
        if (id == SFX_MEGA_BOMB)  bomb = true;
        if (id == SFX_BOSS_BLAST) ev |= C3EV_BOSS_BLAST;
    }
    st->sfx_read = write;

    if (death) {
        bool who[2];
        for (unsigned p = 0u; p < 2u; ++p) {
            who[p] = (in_death_sequence(handler[p]) && !in_death_sequence(st->handler[p])) ||
                     (state[p] == STATE_DYING && st->state[p] != STATE_DYING) ||
                     (lives[p] < st->lives[p]);
        }
        bool p1 = who[0], p2 = who[1];
        if (!p1 && !p2) {
            p1 = p2 = true;    /* could not tell who: rumble both */
        }
        ev |= (p1 ? C3EV_P1_DEATH : 0u) | (p2 ? C3EV_P2_DEATH : 0u);
    }
    if (bomb) {
        bool p1 = (pad1 & SNES_BTN_A_MASK) != 0u, p2 = (pad2 & SNES_BTN_A_MASK) != 0u;
        if (!p1 && !p2) {
            p1 = p2 = true;
        }
        ev |= (p1 ? C3EV_P1_BOMB : 0u) | (p2 ? C3EV_P2_BOMB : 0u);
    }
    for (unsigned p = 0u; p < 2u; ++p) {
        st->handler[p] = handler[p];
        st->state[p] = state[p];
        st->lives[p] = lives[p];
    }
    return ev;
}
