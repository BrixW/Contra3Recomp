/*
 * Contra III: The Alien Wars (USA) - Preemptible 65816 main thread
 *
 * On hardware the RESET code ends in the background loop at $00:8166 (JSL $00995A; BRA),
 * which performs long jobs requested by NMI code ($0052 = $FF: SPC700 streaming uploads,
 * $00:98E2) while NMIs keep interrupting it every frame. The game logic in those NMIs waits
 * for the job (it polls $0052), so a job that takes N frames of CPU time delays the game by
 * N logic frames.
 *
 * The recompiled main thread runs as a coroutine (Win32 fiber, or a pthread with strict handoff). Generated
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

#if defined(_WIN32) && !defined(C3_MAINTHREAD_PTHREAD)
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
/*
 * Portable backend (Linux, macOS, or Windows built with -DC3_MAINTHREAD_PTHREAD): the main
 * thread is a real pthread, but only one side ever runs - control is handed back and forth
 * with a mutex + condition variable, so execution stays strictly sequential and
 * deterministic, exactly like the fiber backend.
 */
#include <pthread.h>

typedef struct MainThread {
    SnesCpuContext *ctx;
    pthread_t       thread;
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    int             turn;         /* 0 = host runs, 1 = main thread runs */
    bool            quit;
    bool            started;
    SnesMainEntry   entry;
    bool            running;
    bool            finished;
} MainThread;

static void main_body(MainThread *mt);

static void switch_to_host(MainThread *mt) {
    pthread_mutex_lock(&mt->lock);
    mt->running = false;
    mt->turn = 0;
    pthread_cond_broadcast(&mt->cond);
    while (mt->turn == 0 && !mt->quit) {
        pthread_cond_wait(&mt->cond, &mt->lock);
    }
    bool quit = mt->quit;
    pthread_mutex_unlock(&mt->lock);
    if (quit) {
        pthread_exit(NULL);   /* the coroutine is being destroyed; never resume it */
    }
}

static void switch_to_main(MainThread *mt) {
    pthread_mutex_lock(&mt->lock);
    mt->running = true;
    mt->turn = 1;
    pthread_cond_broadcast(&mt->cond);
    while (mt->turn == 1) {
        pthread_cond_wait(&mt->cond, &mt->lock);
    }
    pthread_mutex_unlock(&mt->lock);
}

static void *thread_proc(void *param) {
    MainThread *mt = (MainThread *)param;
    pthread_mutex_lock(&mt->lock);
    while (mt->turn == 0 && !mt->quit) {
        pthread_cond_wait(&mt->cond, &mt->lock);
    }
    bool quit = mt->quit;
    pthread_mutex_unlock(&mt->lock);
    if (!quit) {
        main_body(mt);
    }
    return NULL;
}

static bool create_fiber(MainThread *mt) {
    pthread_mutex_init(&mt->lock, NULL);
    pthread_cond_init(&mt->cond, NULL);
    mt->turn = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, MAIN_THREAD_STACK);
    int rc = pthread_create(&mt->thread, &attr, thread_proc, mt);
    pthread_attr_destroy(&attr);
    mt->started = (rc == 0);
    return mt->started;
}

static void destroy_fiber(MainThread *mt) {
    if (!mt->started) {
        return;
    }
    pthread_mutex_lock(&mt->lock);
    mt->quit = true;
    pthread_cond_broadcast(&mt->cond);
    pthread_mutex_unlock(&mt->lock);
    pthread_join(mt->thread, NULL);
    pthread_cond_destroy(&mt->cond);
    pthread_mutex_destroy(&mt->lock);
    mt->started = false;
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
