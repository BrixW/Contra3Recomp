/*
 * Contra III: The Alien Wars (USA) - Preemptible 65816 main thread
 *
 * On hardware the RESET code ends in the background loop at $00:8166 (JSL $00995A; BRA),
 * which performs long jobs requested by NMI code ($0052 = $FF: SPC700 streaming uploads,
 * $00:98E2) while NMIs keep interrupting it every frame. The game logic in those NMIs waits
 * for the job (it polls $0052), so a job that takes N frames of CPU time delays the game by
 * N logic frames.
 *
 * The recompiled main thread runs as a coroutine (Win32 fiber, or POSIX ucontext). Generated
 * code charges cycles before every instruction through SNES_CYCLES, which switches back to
 * the host once the CPU's wall time reaches the next NMI (ctx->yield_at). snes_step_frame
 * then runs the NMI on the host and resumes the main thread until the following NMI. Main
 * thread VBlank waits ($00:85DF spinning at $00:85EC) simply yield until the next NMI, which
 * takes Konami's busy path ($003C != 0) exactly as on hardware.
 */

#include "snes_mainthread.h"
#include "contra3_recomp.h"

#include <stdlib.h>
#include <string.h>

#define MAIN_THREAD_STACK (4u * 1024u * 1024u)

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct MainThread {
    SnesCpuContext *ctx;
    void           *host_fiber;
    void           *fiber;
    SnesMainEntry   entry;
    bool            running;      /* currently executing on the main fiber */
    bool            finished;     /* entry returned (never happens for the real main loop) */
} MainThread;

static void switch_to_host(MainThread *mt) {
    mt->running = false;
    SwitchToFiber(mt->host_fiber);
}

static void switch_to_main(MainThread *mt) {
    if (mt->host_fiber == NULL) {
        mt->host_fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(NULL);
    }
    mt->running = true;
    SwitchToFiber(mt->fiber);
}

static void main_body(MainThread *mt);

static VOID CALLBACK fiber_proc(LPVOID param) {
    main_body((MainThread *)param);
}

static bool create_fiber(MainThread *mt) {
    if (mt->host_fiber == NULL) {
        mt->host_fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(NULL);
        if (mt->host_fiber == NULL) {
            return false;
        }
    }
    mt->fiber = CreateFiber(MAIN_THREAD_STACK, fiber_proc, mt);
    return mt->fiber != NULL;
}

static void destroy_fiber(MainThread *mt) {
    if (mt->fiber != NULL) {
        DeleteFiber(mt->fiber);
        mt->fiber = NULL;
    }
}

#else
#include <ucontext.h>

typedef struct MainThread {
    SnesCpuContext *ctx;
    ucontext_t      host_uc;
    ucontext_t      main_uc;
    void           *stack;
    SnesMainEntry   entry;
    bool            running;
    bool            finished;
} MainThread;

static void switch_to_host(MainThread *mt) {
    mt->running = false;
    swapcontext(&mt->main_uc, &mt->host_uc);
}

static void switch_to_main(MainThread *mt) {
    mt->running = true;
    swapcontext(&mt->host_uc, &mt->main_uc);
}

static void main_body(MainThread *mt);
static MainThread *g_uc_start;

static void uc_proc(void) {
    main_body(g_uc_start);
}

static bool create_fiber(MainThread *mt) {
    mt->stack = malloc(MAIN_THREAD_STACK);
    if (mt->stack == NULL || getcontext(&mt->main_uc) != 0) {
        return false;
    }
    mt->main_uc.uc_stack.ss_sp = mt->stack;
    mt->main_uc.uc_stack.ss_size = MAIN_THREAD_STACK;
    mt->main_uc.uc_link = &mt->host_uc;
    g_uc_start = mt;
    makecontext(&mt->main_uc, uc_proc, 0);
    return true;
}

static void destroy_fiber(MainThread *mt) {
    free(mt->stack);
    mt->stack = NULL;
}
#endif

static void set_deadline(SnesCpuContext *ctx, uint64_t wall_deadline) {
    ctx->yield_at = wall_deadline;
    ctx->yield_cycles = (wall_deadline == UINT64_MAX || wall_deadline < ctx->idle_cycles)
                            ? UINT64_MAX : wall_deadline - ctx->idle_cycles;
}

/* Background loop at $00:8166: JSL $00995A; BRA $8166 (11 cycles of loop overhead) */
static void background_loop(SnesCpuContext *ctx) {
    for (;;) {
        if (snes_bus_read16(ctx, 0x000052u) == 0x00FFu) {
            SNES_CYCLES(ctx, 11u);
            bank00_func_995a(ctx);
        } else {
            /* Nothing requested: the loop just spins until the next NMI */
            uint64_t wall = snes_wall_cycles(ctx);
            if (wall < ctx->yield_at) {
                ctx->idle_cycles += ctx->yield_at - wall;
            }
            ctx->main_idle = true;
            snes_mainthread_yield(ctx);
            ctx->main_idle = false;
        }
    }
}

static void main_body(MainThread *mt) {
    SnesCpuContext *ctx = mt->ctx;
    /* The RESET / warm-reset code returns at its first pass through $00:8166 */
    ctx->main_loop_iters = 0u;
    ctx->max_main_loop_iters = 1u;
    if (mt->entry != NULL) {
        mt->entry(ctx);
    }
    background_loop(ctx);
    mt->finished = true;
    switch_to_host(mt);
}

static void free_main_thread(SnesCpuContext *ctx) {
    MainThread *mt = (MainThread *)ctx->main_thread;
    if (mt == NULL) {
        return;
    }
    destroy_fiber(mt);
    free(mt);
    ctx->main_thread = NULL;
}

static void start(SnesCpuContext *ctx, SnesMainEntry entry) {
    free_main_thread(ctx);
    MainThread *mt = (MainThread *)calloc(1u, sizeof(*mt));
    if (mt == NULL) {
        if (entry != NULL) {
            entry(ctx);   /* out of memory: fall back to the synchronous model */
        }
        return;
    }
    mt->ctx = ctx;
    mt->entry = entry;
    if (!create_fiber(mt)) {
        free(mt);
        if (entry != NULL) {
            entry(ctx);
        }
        return;
    }
    ctx->main_thread = mt;
    ctx->main_idle = false;
    /* Run the boot code until it first waits for an NMI (or reaches the next NMI time) */
    uint64_t now = snes_wall_cycles(ctx);
    set_deadline(ctx, (ctx->nmi_wall > now) ? ctx->nmi_wall : now + snes_timing_budget(ctx));
    switch_to_main(mt);
    set_deadline(ctx, UINT64_MAX);
}

void snes_power_on(SnesCpuContext *ctx) {
    start(ctx, bank00_func_8000);
}

void snes_soft_reset(SnesCpuContext *ctx) {
    start(ctx, bank00_func_80a3);
}

void snes_mainthread_restart_idle(SnesCpuContext *ctx) {
    start(ctx, NULL);
}

void snes_mainthread_release(SnesCpuContext *ctx) {
    free_main_thread(ctx);
}

bool snes_mainthread_active(const SnesCpuContext *ctx) {
    return ctx->main_thread != NULL;
}

bool snes_mainthread_idle(const SnesCpuContext *ctx) {
    return ctx->main_thread == NULL || ctx->main_idle;
}

void snes_mainthread_run_until(SnesCpuContext *ctx, uint64_t wall_deadline) {
    MainThread *mt = (MainThread *)ctx->main_thread;
    if (mt == NULL || mt->finished || snes_wall_cycles(ctx) >= wall_deadline) {
        return;
    }
    set_deadline(ctx, wall_deadline);
    switch_to_main(mt);
    set_deadline(ctx, UINT64_MAX);
}

void snes_mainthread_yield(SnesCpuContext *ctx) {
    MainThread *mt = (MainThread *)ctx->main_thread;
    if (mt != NULL && mt->running) {
        switch_to_host(mt);
    }
}

void snes_mainthread_preempt(SnesCpuContext *ctx) {
    snes_mainthread_yield(ctx);
}

bool snes_mainthread_running(const SnesCpuContext *ctx) {
    const MainThread *mt = (const MainThread *)ctx->main_thread;
    return mt != NULL && mt->running;
}
