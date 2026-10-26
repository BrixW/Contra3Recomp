/*
 * Contra III: The Alien Wars (USA) - Restored Contra Spirits unlocks, host extras, options
 */

#include "contra3_extras.h"
#include "contra3_recomp.h"

#define STATE_TITLE       0x03u
#define STATE_MODE_FADE   0x06u
#define STATE_GAMEPLAY    0x09u
#define ADDR_GAME_STATE   0x000032u
#define ADDR_STAGE        0x000086u
#define ADDR_PLAYER_MODE  0x001E02u   /* 0 1P, 1 2P-A, 2 2P-B, 3 option */
#define ADDR_STAGE_SEL    0x001E60u
#define ADDR_START_LIVES  0x001E3Cu
#define ADDR_DIFFICULTY   0x001E3Eu
#define ADDR_SOUND_MODE   0x001E54u
#define ADDR_LIVES_P1     0x001F8Au
#define ADDR_LIVES_P2     0x001FCAu
#define SOUND_CMD_TABLE   0x05892Au   /* LDA $892A,Y with DB = $05 at $00:BC25 */

static void send_sound_mode(SnesCpuContext *ctx, unsigned sound);

void contra3_extras_apply(SnesCpuContext *ctx, Contra3Extras *ex) {
    if (ex == NULL) {
        return;
    }
    uint8_t state = snes_bus_read8(ctx, ADDR_GAME_STATE);
    if (state != STATE_TITLE) {
        ex->title_frames = 0u;
    }
    if (state == STATE_TITLE) {
        if ((ex->flags & CONTRA3_UNLOCK_STAGE_SELECT) != 0u) {
            snes_bus_write16(ctx, ADDR_STAGE_SEL, 0x0001u);
        }
        if ((ex->flags & CONTRA3_UNLOCK_LIVES_30) != 0u) {
            snes_bus_write16(ctx, ADDR_START_LIVES, 0x001Eu);
        }
        ex->stage_armed = true;
        if (++ex->title_frames >= 30u && ex->sound_pending >= 0) {
            send_sound_mode(ctx, (unsigned)ex->sound_pending);
            ex->sound_pending = -1;
        }
    } else if (state == STATE_MODE_FADE) {
        /* The fade's own init sets $0086 = 1 on its first frames, so hold the value throughout */
        if (ex->stage_armed && ex->start_stage >= 1u && ex->start_stage <= 6u) {
            snes_bus_write16(ctx, ADDR_STAGE, (uint16_t)ex->start_stage);
        }
    } else if (state != STATE_TITLE) {
        ex->stage_armed = false; /* only the first game from each title visit */
    }
    if (state == STATE_GAMEPLAY && (ex->flags & CONTRA3_UNLOCK_INFINITE_LIVES) != 0u) {
        uint16_t mode = snes_bus_read16(ctx, ADDR_PLAYER_MODE);
        if (snes_bus_read16(ctx, ADDR_LIVES_P1) < 2u) {
            snes_bus_write16(ctx, ADDR_LIVES_P1, 2u);
        }
        if ((mode == 1u || mode == 2u) && snes_bus_read16(ctx, ADDR_LIVES_P2) < 2u) {
            snes_bus_write16(ctx, ADDR_LIVES_P2, 2u);
        }
    }
}

void contra3_options_read(SnesCpuContext *ctx, Contra3Options *out) {
    out->lives      = snes_bus_read16(ctx, ADDR_START_LIVES);
    out->difficulty = snes_bus_read16(ctx, ADDR_DIFFICULTY);
    out->sound      = snes_bus_read16(ctx, ADDR_SOUND_MODE);
}

static void send_sound_mode(SnesCpuContext *ctx, unsigned sound) {
    /* Same command OPTION MODE sends ($00:BC22) */
    uint16_t saved_a = ctx->a, saved_dp = ctx->dp;
    uint8_t  saved_m = ctx->flag_m, saved_x = ctx->flag_x;
    ctx->flag_m = 0u;   /* $00:868F is entered with 16-bit M/X and D = $0000 */
    ctx->flag_x = 0u;
    ctx->dp = 0u;
    ctx->a = snes_bus_read16(ctx, SOUND_CMD_TABLE + sound);
    bank00_func_868f(ctx);
    ctx->a = saved_a;
    ctx->dp = saved_dp;
    ctx->flag_m = saved_m;
    ctx->flag_x = saved_x;
}

void contra3_options_restore(SnesCpuContext *ctx, const Contra3Options *opt, Contra3Extras *ex) {
    if (opt->lives >= 1u && opt->lives <= 99u) {
        snes_bus_write16(ctx, ADDR_START_LIVES, (uint16_t)opt->lives);
    }
    if (opt->difficulty <= 0x0Eu && (opt->difficulty & 1u) == 0u) {
        snes_bus_write16(ctx, ADDR_DIFFICULTY, (uint16_t)opt->difficulty);
    }
    if (opt->sound == 0u || opt->sound == 2u) {
        snes_bus_write16(ctx, ADDR_SOUND_MODE, (uint16_t)opt->sound);
        if (opt->sound != 0u && ex != NULL) {
            ex->sound_pending = (int)opt->sound; /* stereo is the driver default */
        }
    }
}
