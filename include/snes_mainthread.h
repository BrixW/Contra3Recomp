#ifndef CONTRA3_SNES_MAINTHREAD_H
#define CONTRA3_SNES_MAINTHREAD_H

#include <stdbool.h>
#include <stdint.h>
#include "cpu_65816.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Preemptible 65816 main thread (src/snes_mainthread.c).
 *
 * snes_power_on / snes_soft_reset start the RESET ($00:8000) or warm-reset ($00:80A3) code
 * on a coroutine that then runs the $00:8166 background loop forever. snes_step_frame runs
 * each NMI on the host and gives the main thread the CPU time up to the next NMI, so long
 * background jobs ($0052 = $FF uploads) span frames exactly as on hardware.
 *
 * The context, ROM and APU must be initialised and attached before snes_power_on. Without
 * it (legacy model) callers run bank00_func_8000 directly and snes_step_frame executes a
 * few background-worker slices synchronously after each NMI.
 */
typedef void (*SnesMainEntry)(SnesCpuContext *ctx);

void snes_power_on(SnesCpuContext *ctx);
void snes_soft_reset(SnesCpuContext *ctx);
void snes_mainthread_release(SnesCpuContext *ctx);
/* Recreate the coroutine directly in the $00:8166 idle loop (used after loading a save state) */
void snes_mainthread_restart_idle(SnesCpuContext *ctx);

bool snes_mainthread_active(const SnesCpuContext *ctx);
bool snes_mainthread_running(const SnesCpuContext *ctx);   /* executing on the coroutine now */
bool snes_mainthread_idle(const SnesCpuContext *ctx);      /* parked in the idle loop (or no coroutine) */

/* Host side: let the main thread run until wall time reaches the deadline */
void snes_mainthread_run_until(SnesCpuContext *ctx, uint64_t wall_deadline);
/* Main-thread side: give the CPU back to the host (next NMI) */
void snes_mainthread_yield(SnesCpuContext *ctx);
void snes_mainthread_preempt(SnesCpuContext *ctx);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_SNES_MAINTHREAD_H */
