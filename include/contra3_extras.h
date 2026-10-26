#ifndef CONTRA3_EXTRAS_H
#define CONTRA3_EXTRAS_H

#include <stdbool.h>
#include "cpu_65816.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Optional gameplay extras applied by the host after each frame.
 *
 * Restored Contra Spirits (Japan) features whose code-entry detectors were disconnected
 * in the US release ($00:B2F3 / $00:B346 / $00:B39F; their D-pad code tables at $05:86D3+
 * were overwritten by other data, and their $1C08-$1C13 state is reused by other screens).
 * Their effects still work, so they are re-applied on the title screen (the title init at
 * $00:B1CF-$00:B1E1 clears $1E60 every visit):
 *
 *   STAGE_SELECT   $1E60 = 1: START on the title opens Konami's "STAGE SELECT" screen
 *                  (state $07, $00:A73D: UP/DOWN pick 1-6, START launches)
 *   LIVES_30       $1E3C = 30: the starting-lives value the 30-lives code would set
 *
 * Host conveniences:
 *
 *   INFINITE_LIVES during gameplay, lives of every player in the game never drop below 2
 *                  (P2 counts only in 2-player modes, $1E02 = 1 or 2)
 *   start_stage    1..6: the first game started from each title visit begins at that stage
 *                  ($0086 is written during the mode-select fade, state $06)
 */
#define CONTRA3_UNLOCK_STAGE_SELECT   0x01u
#define CONTRA3_UNLOCK_LIVES_30       0x02u
#define CONTRA3_UNLOCK_INFINITE_LIVES 0x04u

typedef struct Contra3Extras {
    unsigned flags;        /* CONTRA3_UNLOCK_* */
    unsigned start_stage;  /* 0 = off, 1..6 */
    bool     stage_armed;  /* internal: title seen since the last start_stage write */
    int      sound_pending;/* internal: stereo/mono command to send once the title settles (-1 none) */
    unsigned title_frames; /* internal: consecutive frames on the title screen */
} Contra3Extras;

void contra3_extras_apply(SnesCpuContext *ctx, Contra3Extras *ex);

/*
 * In-game OPTION MODE settings (state $0D, $04:D878):
 *   PLAYER $1E3C (3/5/7 lives), LEVEL $1E3E (0 easy, 2 normal, 4 hard; raised after an
 *   ending), SOUND $1E54 (0 stereo, 2 monaural). A cold boot resets them, so the host can
 *   restore saved values right after power-on. The sound mode must also reach the SPC700
 *   driver as a command ($00:868F queue, table $05:892A: $7F stereo, $7E mono), but the
 *   driver ignores it until the title music is running (sent on title frame 1, or again by
 *   the game at stage start, it has no effect). contra3_options_restore therefore leaves it
 *   in ex->sound_pending and contra3_extras_apply sends it after 30 frames on the title.
 */
typedef struct Contra3Options {
    unsigned lives;        /* 1..99 */
    unsigned difficulty;   /* 0, 2, 4 (higher values exist after completing the game) */
    unsigned sound;        /* 0 stereo, 2 monaural */
} Contra3Options;

void contra3_options_read(SnesCpuContext *ctx, Contra3Options *out);
void contra3_options_restore(SnesCpuContext *ctx, const Contra3Options *opt, Contra3Extras *ex);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_EXTRAS_H */
