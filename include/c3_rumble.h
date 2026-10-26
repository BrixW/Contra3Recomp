#ifndef CONTRA3_C3_RUMBLE_H
#define CONTRA3_C3_RUMBLE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Controller rumble envelopes driven by contra3_events (C3EV_*). Portable: the frontend
 * calls c3_rumble_frame once per emulated frame and sends the resulting motor levels to
 * XInput / SDL. Effects (strongest wins, linear decay):
 *   death       on the player's pad   heavy + buzz,   ~0.4 s
 *   Mega Bomb   thrower               heavy + light,  ~0.75 s   (other pad: light, ~0.5 s)
 *   boss blast  both pads             light,          ~0.17 s
 */

typedef struct C3RumbleMotor {
    float level;     /* 0..1 at the start of the effect */
    int   frames;    /* remaining frames */
    int   total;
} C3RumbleMotor;

typedef struct C3Rumble {
    C3RumbleMotor low[2], high[2];   /* low = heavy (left) motor, high = buzz (right) motor */
    uint16_t      out_low[2], out_high[2];
    bool          changed[2];
} C3Rumble;

void c3_rumble_reset(C3Rumble *r);
/* Feed this frame's events and advance one frame; strength 0..100 scales every effect */
void c3_rumble_frame(C3Rumble *r, unsigned events, unsigned strength, bool enabled);
/* Manual test pulse (overlay "test rumble") */
void c3_rumble_test(C3Rumble *r, unsigned pad);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_C3_RUMBLE_H */
