#ifndef CONTRA3_EVENTS_H
#define CONTRA3_EVENTS_H

#include <stdint.h>
#include <stdbool.h>
#include "cpu_65816.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Gameplay events for host effects (controller rumble), derived from the game's own state
 * after each frame:
 *
 *   - Sound effect commands queued for the SPC700 driver: ring at $0100, write index $56
 *     (enqueued by $00:868F). $3F / $43 = player death screams, $5A = Mega Bomb, $56 = boss
 *     explosion. The ring is reset during sample uploads, so large jumps resynchronise.
 *   - Who died, in the frame the scream is queued: the player whose actor slot ($0200 P1,
 *     $0240 P2) state handler at +$00 enters the death sequence ($ACxx, side-scrolling), whose
 *     slot state word +$12 becomes 7 (dying), or whose lives ($1F8A / $1FCA) drop in that
 *     frame (Mode 7 overhead stages deduct the life immediately).
 *   - Mega Bombs are credited to the player who pressed A this frame (fallback: both).
 */

#define C3EV_P1_DEATH      0x0001u
#define C3EV_P2_DEATH      0x0002u
#define C3EV_P1_BOMB       0x0004u
#define C3EV_P2_BOMB       0x0008u
#define C3EV_BOSS_BLAST    0x0010u   /* boss / large explosion (both pads, light) */

typedef struct Contra3EventState {
    uint8_t  sfx_read;        /* next ring index to inspect */
    bool     synced;
    uint16_t handler[2];      /* previous P1/P2 state handler words */
    uint16_t state[2];        /* previous P1/P2 slot state words (+$12) */
    uint16_t lives[2];        /* previous P1/P2 lives */
} Contra3EventState;

void     contra3_events_reset(Contra3EventState *st);
/* pads = the joypad masks fed to this frame; returns C3EV_* flags */
unsigned contra3_events_poll(SnesCpuContext *ctx, Contra3EventState *st, uint16_t pad1, uint16_t pad2);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_EVENTS_H */
