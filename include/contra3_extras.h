#ifndef CONTRA3_EXTRAS_H
#define CONTRA3_EXTRAS_H

#include "cpu_65816.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Leftover Contra Spirits (Japan) features whose code-entry detectors were disconnected
 * in the US release ($00:B2F3 / $00:B346 / $00:B39F; their D-pad code tables at $05:86D3+
 * were overwritten by other data, and their $1C08-$1C13 state is reused by other screens).
 * Their effects still work, so the host re-applies them on the title screen:
 *
 *   STAGE_SELECT  $1E60 = 1: pressing START on the title opens Konami's "STAGE SELECT"
 *                 screen (state $07, $00:A73D: UP/DOWN pick 1-6, START launches)
 *   LIVES_30      $1E3C = 30: the starting-lives option the 30-lives code would set
 *
 * The title-screen init ($00:B1CF-$00:B1E1) clears $1E60, so this must run every frame.
 */
#define CONTRA3_UNLOCK_STAGE_SELECT 0x01u
#define CONTRA3_UNLOCK_LIVES_30     0x02u

void contra3_apply_unlocks(SnesCpuContext *ctx, unsigned unlocks);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_EXTRAS_H */
