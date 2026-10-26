/*
 * Contra III: The Alien Wars (USA) - controller rumble envelopes
 */

#include "c3_rumble.h"
#include "contra3_events.h"

#include <string.h>

void c3_rumble_reset(C3Rumble *r) {
    memset(r, 0, sizeof(*r));
    r->changed[0] = r->changed[1] = true;
}

static void kick(C3RumbleMotor *m, float level, int frames) {
    /* strongest effect wins: only replace a weaker or nearly finished one */
    float now = (m->total > 0) ? m->level * (float)m->frames / (float)m->total : 0.0f;
    if (level >= now) {
        m->level = level;
        m->frames = m->total = frames;
    }
}

static uint16_t motor_out(C3RumbleMotor *m, float scale) {
    if (m->frames <= 0 || m->total <= 0) {
        return 0u;
    }
    float v = m->level * scale * (float)m->frames / (float)m->total;
    m->frames--;
    if (v > 1.0f) v = 1.0f;
    return (uint16_t)(v * 65535.0f);
}

void c3_rumble_frame(C3Rumble *r, unsigned events, unsigned strength, bool enabled) {
    if (enabled) {
        if (events & C3EV_P1_DEATH) { kick(&r->low[0], 0.90f, 24); kick(&r->high[0], 0.60f, 18); }
        if (events & C3EV_P2_DEATH) { kick(&r->low[1], 0.90f, 24); kick(&r->high[1], 0.60f, 18); }
        for (unsigned p = 0u; p < 2u; ++p) {
            unsigned mine = (p == 0u) ? C3EV_P1_BOMB : C3EV_P2_BOMB;
            unsigned other = (p == 0u) ? C3EV_P2_BOMB : C3EV_P1_BOMB;
            if (events & mine)  { kick(&r->low[p], 1.00f, 45); kick(&r->high[p], 0.40f, 30); }
            if (events & other) { kick(&r->low[p], 0.40f, 30); }
            if (events & C3EV_BOSS_BLAST) { kick(&r->low[p], 0.50f, 10); kick(&r->high[p], 0.20f, 8); }
        }
    } else {
        memset(r->low, 0, sizeof(r->low));
        memset(r->high, 0, sizeof(r->high));
    }
    float scale = (float)strength / 100.0f;
    for (unsigned p = 0u; p < 2u; ++p) {
        uint16_t lo = motor_out(&r->low[p], scale);
        uint16_t hi = motor_out(&r->high[p], scale);
        r->changed[p] = (lo != r->out_low[p]) || (hi != r->out_high[p]);
        r->out_low[p] = lo;
        r->out_high[p] = hi;
    }
}

void c3_rumble_test(C3Rumble *r, unsigned pad) {
    if (pad < 2u) {
        kick(&r->low[pad], 1.0f, 30);
        kick(&r->high[pad], 0.6f, 20);
    }
}
