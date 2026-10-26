#ifndef CONTRA3_CPU_65816_H
#define CONTRA3_CPU_65816_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "snes_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SnesApu;

typedef void (*SnesMmioWriteHook)(struct SnesCpuContext *ctx, uint16_t reg, uint8_t val);
typedef uint8_t (*SnesMmioReadHook)(struct SnesCpuContext *ctx, uint16_t reg);
typedef void (*SnesInterpFallbackHook)(struct SnesCpuContext *ctx, uint32_t target_addr24);

/*
 * Portable WDC 65C816 CPU + SNES System Context
 */
typedef struct SnesCpuContext {
    /* 65816 Architectural Registers */
    uint16_t a;          /* 16-bit Accumulator C (B = high byte, A = low byte) */
    uint16_t x;          /* 16-bit Index X (zero-extended to 8-bit when flag_x == 1) */
    uint16_t y;          /* 16-bit Index Y (zero-extended to 8-bit when flag_x == 1) */
    uint16_t sp;         /* 16-bit Stack Pointer S */
    uint16_t dp;         /* 16-bit Direct Page Register D */
    uint8_t  db;         /* 8-bit Data Bank Register DBR */
    uint8_t  pb;         /* 8-bit Program Bank Register PBR (K) */

    /* Processor Status Flags (P) + Emulation Flag (E) */
    uint8_t  flag_n;     /* 0x80 Negative */
    uint8_t  flag_v;     /* 0x40 Overflow */
    uint8_t  flag_m;     /* 0x20 Memory/Accumulator Select (1 = 8-bit, 0 = 16-bit) */
    uint8_t  flag_x;     /* 0x10 Index Register Select     (1 = 8-bit, 0 = 16-bit) */
    uint8_t  flag_d;     /* 0x08 Decimal (BCD) Mode */
    uint8_t  flag_i;     /* 0x04 IRQ Disable */
    uint8_t  flag_z;     /* 0x02 Zero */
    uint8_t  flag_c;     /* 0x01 Carry */
    uint8_t  flag_e;     /* Emulation mode (1 = 6502 emulation, 0 = 65816 native) */

    /* Memory Backing */
    uint8_t        wram[0x20000]; /* 128 KB WRAM ($7E:0000 - $7F:FFFF) */
    uint8_t        sram[0x8000];  /* 32 KB Cartridge SRAM ($70:0000 - $7D:7FFF) */
    uint32_t       wram_port_addr;/* 17-bit address for $2180-$2183 WMDATA port */
    const uint8_t *rom;           /* Pointer to 1 MB LoROM image */
    size_t         rom_size;      /* Size of ROM image in bytes (1,048,576) */

    /* Hardware Subsystem States */
    SnesPpuState    ppu;
    SnesCpuIoState  cpu_io;
    SnesApuState    apu;
    SnesDmaChannel  dma[8];

    /* Optional Host Hooks */
    SnesMmioWriteHook      on_write_ppu;
    SnesMmioReadHook       on_read_ppu;
    SnesMmioWriteHook      on_write_cpu_io;
    SnesMmioReadHook       on_read_cpu_io;
    SnesInterpFallbackHook on_interp_fallback;

    /* Execution Control & Diagnostics */
    bool     stop_main_loop;      /* Allows test harnesses to exit infinite main loops cleanly */
    uint64_t main_loop_iters;     /* Number of iterations of $00:8166 background loop */
    uint64_t max_main_loop_iters; /* If > 0, sets stop_main_loop once reached */
    uint32_t unresolved_jumps;    /* Count of fallback invocations */
    uint32_t last_unresolved_addr;/* Most recent unresolved 24-bit target */
    uint32_t last_unresolved_site;/* 24-bit address of the indirect JMP/JSR/JML that missed */
    bool     strict_fallback;     /* Abort with a register dump on the first unresolved target */
    struct SnesApu *apu_core;     /* Optional live Sony S-SMP (SPC700) + S-DSP audio core */
} SnesCpuContext;

/* Lifecycle & Bus Routines (implemented in src/snes_runtime.c) */
void     snes_context_init(SnesCpuContext *ctx, const uint8_t *rom, size_t rom_size);
uint8_t  snes_bus_read8(SnesCpuContext *ctx, uint32_t addr24);
void     snes_bus_write8(SnesCpuContext *ctx, uint32_t addr24, uint8_t val);
uint16_t snes_bus_read16(SnesCpuContext *ctx, uint32_t addr24);
void     snes_bus_write16(SnesCpuContext *ctx, uint32_t addr24, uint16_t val);
void     snes_block_mvn(SnesCpuContext *ctx, uint8_t dst_bank, uint8_t src_bank);
void     snes_block_mvp(SnesCpuContext *ctx, uint8_t dst_bank, uint8_t src_bank);
/*
 * Called by generated dispatch switches when a runtime target is not a recompiled
 * entry point. Retries the global dispatcher, then records the miss (count, site,
 * target), invokes on_interp_fallback, and aborts if strict_fallback is set.
 */
void     snes_interp_fallback_at(SnesCpuContext *ctx, uint32_t site_addr24, uint32_t target_addr24);
void     snes_interp_fallback(SnesCpuContext *ctx, uint32_t target_addr24); /* site unknown */

/* --- Status Register (P) Pack / Unpack --- */
static inline uint8_t snes_pack_p(const SnesCpuContext *ctx) {
    return (uint8_t)(
        (ctx->flag_n ? 0x80u : 0u) |
        (ctx->flag_v ? 0x40u : 0u) |
        (ctx->flag_m ? 0x20u : 0u) |
        (ctx->flag_x ? 0x10u : 0u) |
        (ctx->flag_d ? 0x08u : 0u) |
        (ctx->flag_i ? 0x04u : 0u) |
        (ctx->flag_z ? 0x02u : 0u) |
        (ctx->flag_c ? 0x01u : 0u)
    );
}

static inline void snes_unpack_p(SnesCpuContext *ctx, uint8_t p) {
    ctx->flag_n = (uint8_t)((p >> 7) & 1u);
    ctx->flag_v = (uint8_t)((p >> 6) & 1u);
    ctx->flag_m = (uint8_t)((p >> 5) & 1u);
    ctx->flag_x = (uint8_t)((p >> 4) & 1u);
    ctx->flag_d = (uint8_t)((p >> 3) & 1u);
    ctx->flag_i = (uint8_t)((p >> 2) & 1u);
    ctx->flag_z = (uint8_t)((p >> 1) & 1u);
    ctx->flag_c = (uint8_t)(p & 1u);
    if (ctx->flag_x) {
        ctx->x &= 0x00FFu;
        ctx->y &= 0x00FFu;
    }
}

static inline void snes_sep(SnesCpuContext *ctx, uint8_t mask) {
    snes_unpack_p(ctx, (uint8_t)(snes_pack_p(ctx) | mask));
}

static inline void snes_rep(SnesCpuContext *ctx, uint8_t mask) {
    snes_unpack_p(ctx, (uint8_t)(snes_pack_p(ctx) & (uint8_t)(~mask)));
}

/* --- Flag Update Helpers --- */
static inline void snes_set_nz8(SnesCpuContext *ctx, uint8_t val) {
    ctx->flag_z = (uint8_t)(val == 0u ? 1u : 0u);
    ctx->flag_n = (uint8_t)((val >> 7) & 1u);
}

static inline void snes_set_nz16(SnesCpuContext *ctx, uint16_t val) {
    ctx->flag_z = (uint8_t)(val == 0u ? 1u : 0u);
    ctx->flag_n = (uint8_t)((val >> 15) & 1u);
}

/* --- Stack Push / Pull Helpers (Native Mode 16-bit SP in Bank $00) --- */
static inline void snes_push8(SnesCpuContext *ctx, uint8_t val) {
    snes_bus_write8(ctx, (uint32_t)ctx->sp, val);
    ctx->sp = (uint16_t)(ctx->sp - 1u);
}

static inline uint8_t snes_pull8(SnesCpuContext *ctx) {
    ctx->sp = (uint16_t)(ctx->sp + 1u);
    return snes_bus_read8(ctx, (uint32_t)ctx->sp);
}

static inline void snes_push16(SnesCpuContext *ctx, uint16_t val) {
    snes_push8(ctx, (uint8_t)((val >> 8) & 0xFFu));
    snes_push8(ctx, (uint8_t)(val & 0xFFu));
}

static inline uint16_t snes_pull16(SnesCpuContext *ctx) {
    uint16_t lo = (uint16_t)snes_pull8(ctx);
    uint16_t hi = (uint16_t)snes_pull8(ctx);
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

/* --- 65816 Effective Address Helpers --- */
static inline uint32_t snes_addr_dp(const SnesCpuContext *ctx, uint8_t d) {
    return (uint32_t)(uint16_t)(ctx->dp + (uint16_t)d);
}

static inline uint32_t snes_addr_dpx(const SnesCpuContext *ctx, uint8_t d) {
    return (uint32_t)(uint16_t)(ctx->dp + (uint16_t)d + ctx->x);
}

static inline uint32_t snes_addr_dpy(const SnesCpuContext *ctx, uint8_t d) {
    return (uint32_t)(uint16_t)(ctx->dp + (uint16_t)d + ctx->y);
}

static inline uint32_t snes_addr_abs(const SnesCpuContext *ctx, uint16_t a16) {
    return ((uint32_t)ctx->db << 16) | (uint32_t)a16;
}

static inline uint32_t snes_addr_absx(const SnesCpuContext *ctx, uint16_t a16) {
    return (((uint32_t)ctx->db << 16) + (uint32_t)a16 + (uint32_t)ctx->x) & 0xFFFFFFu;
}

static inline uint32_t snes_addr_absy(const SnesCpuContext *ctx, uint16_t a16) {
    return (((uint32_t)ctx->db << 16) + (uint32_t)a16 + (uint32_t)ctx->y) & 0xFFFFFFu;
}

static inline uint32_t snes_addr_dpi(SnesCpuContext *ctx, uint8_t d) {
    uint16_t ptr = snes_bus_read16(ctx, (uint32_t)(uint16_t)(ctx->dp + (uint16_t)d));
    return ((uint32_t)ctx->db << 16) | (uint32_t)ptr;
}

static inline uint32_t snes_addr_dpxi(SnesCpuContext *ctx, uint8_t d) {
    uint16_t ptr = snes_bus_read16(ctx, (uint32_t)(uint16_t)(ctx->dp + (uint16_t)d + ctx->x));
    return ((uint32_t)ctx->db << 16) | (uint32_t)ptr;
}

static inline uint32_t snes_addr_dpiy(SnesCpuContext *ctx, uint8_t d) {
    uint16_t ptr = snes_bus_read16(ctx, (uint32_t)(uint16_t)(ctx->dp + (uint16_t)d));
    return (((uint32_t)ctx->db << 16) + (uint32_t)ptr + (uint32_t)ctx->y) & 0xFFFFFFu;
}

static inline uint32_t snes_addr_dpil(SnesCpuContext *ctx, uint8_t d) {
    uint16_t base = (uint16_t)(ctx->dp + (uint16_t)d);
    uint32_t lo16 = (uint32_t)snes_bus_read16(ctx, (uint32_t)base);
    uint32_t bk   = (uint32_t)snes_bus_read8(ctx, (uint32_t)(uint16_t)(base + 2u));
    return (bk << 16) | lo16;
}

static inline uint32_t snes_addr_dpily(SnesCpuContext *ctx, uint8_t d) {
    return (snes_addr_dpil(ctx, d) + (uint32_t)ctx->y) & 0xFFFFFFu;
}

static inline uint32_t snes_addr_longx(const SnesCpuContext *ctx, uint32_t a24) {
    return (a24 + (uint32_t)ctx->x) & 0xFFFFFFu;
}

/* --- 65816 ALU (Binary + BCD Decimal Mode) & Bit/Shift Helpers --- */
static inline void snes_adc8(SnesCpuContext *ctx, uint8_t rhs) {
    uint8_t lhs = (uint8_t)(ctx->a & 0xFFu);
    uint32_t c = (uint32_t)ctx->flag_c;
    uint8_t res;
    if (!ctx->flag_d) {
        uint32_t sum = (uint32_t)lhs + (uint32_t)rhs + c;
        ctx->flag_v = (uint8_t)((~(lhs ^ rhs) & (lhs ^ (uint8_t)sum) & 0x80u) ? 1u : 0u);
        ctx->flag_c = (uint8_t)(sum > 0xFFu ? 1u : 0u);
        res = (uint8_t)(sum & 0xFFu);
    } else {
        uint32_t lo = (uint32_t)(lhs & 0x0Fu) + (uint32_t)(rhs & 0x0Fu) + c;
        if (lo > 0x09u) lo += 0x06u;
        uint32_t hi = (uint32_t)(lhs & 0xF0u) + (uint32_t)(rhs & 0xF0u) + (lo > 0x0Fu ? 0x10u : 0u) + (lo & 0x0Fu);
        ctx->flag_v = (uint8_t)((~(lhs ^ rhs) & (lhs ^ (uint8_t)hi) & 0x80u) ? 1u : 0u);
        if (hi > 0x9Fu) hi += 0x60u;
        ctx->flag_c = (uint8_t)(hi > 0xFFu ? 1u : 0u);
        res = (uint8_t)(hi & 0xFFu);
    }
    ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)res);
    snes_set_nz8(ctx, res);
}

static inline void snes_adc16(SnesCpuContext *ctx, uint16_t rhs) {
    uint16_t lhs = ctx->a;
    uint32_t c = (uint32_t)ctx->flag_c;
    uint16_t res;
    if (!ctx->flag_d) {
        uint32_t sum = (uint32_t)lhs + (uint32_t)rhs + c;
        ctx->flag_v = (uint8_t)((~(lhs ^ rhs) & (lhs ^ (uint16_t)sum) & 0x8000u) ? 1u : 0u);
        ctx->flag_c = (uint8_t)(sum > 0xFFFFu ? 1u : 0u);
        res = (uint16_t)(sum & 0xFFFFu);
    } else {
        uint32_t n0 = (uint32_t)(lhs & 0x000Fu) + (uint32_t)(rhs & 0x000Fu) + c;
        if (n0 > 0x0009u) n0 += 0x0006u;
        uint32_t n1 = (uint32_t)(lhs & 0x00F0u) + (uint32_t)(rhs & 0x00F0u) + (n0 > 0x000Fu ? 0x0010u : 0u) + (n0 & 0x000Fu);
        if (n1 > 0x009Fu) n1 += 0x0060u;
        uint32_t n2 = (uint32_t)(lhs & 0x0F00u) + (uint32_t)(rhs & 0x0F00u) + (n1 > 0x00FFu ? 0x0100u : 0u) + (n1 & 0x00FFu);
        if (n2 > 0x09FFu) n2 += 0x0600u;
        uint32_t n3 = (uint32_t)(lhs & 0xF000u) + (uint32_t)(rhs & 0xF000u) + (n2 > 0x0FFFu ? 0x1000u : 0u) + (n2 & 0x0FFFu);
        ctx->flag_v = (uint8_t)((~(lhs ^ rhs) & (lhs ^ (uint16_t)n3) & 0x8000u) ? 1u : 0u);
        if (n3 > 0x9FFFu) n3 += 0x6000u;
        ctx->flag_c = (uint8_t)(n3 > 0xFFFFu ? 1u : 0u);
        res = (uint16_t)(n3 & 0xFFFFu);
    }
    ctx->a = res;
    snes_set_nz16(ctx, res);
}

static inline void snes_sbc8(SnesCpuContext *ctx, uint8_t rhs) {
    uint8_t lhs = (uint8_t)(ctx->a & 0xFFu);
    uint32_t c = (uint32_t)ctx->flag_c;
    uint8_t inv = (uint8_t)(rhs ^ 0xFFu);
    uint32_t bin = (uint32_t)lhs + (uint32_t)inv + c;
    ctx->flag_v = (uint8_t)((~(lhs ^ inv) & (lhs ^ (uint8_t)bin) & 0x80u) ? 1u : 0u);
    ctx->flag_c = (uint8_t)(bin > 0xFFu ? 1u : 0u);
    uint8_t res;
    if (!ctx->flag_d) {
        res = (uint8_t)(bin & 0xFFu);
    } else {
        int32_t lo = (int32_t)(lhs & 0x0Fu) + (int32_t)(inv & 0x0Fu) + (int32_t)c;
        if (lo <= 0x0F) lo -= 0x06;
        int32_t hi = (int32_t)(lhs & 0xF0u) + (int32_t)(inv & 0xF0u) + (lo > 0x0F ? 0x10 : 0) + (lo & 0x0F);
        if (hi <= 0xFF) hi -= 0x60;
        res = (uint8_t)(hi & 0xFF);
    }
    ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)res);
    snes_set_nz8(ctx, res);
}

static inline void snes_sbc16(SnesCpuContext *ctx, uint16_t rhs) {
    uint16_t lhs = ctx->a;
    uint32_t c = (uint32_t)ctx->flag_c;
    uint16_t inv = (uint16_t)(rhs ^ 0xFFFFu);
    uint32_t bin = (uint32_t)lhs + (uint32_t)inv + c;
    ctx->flag_v = (uint8_t)((~(lhs ^ inv) & (lhs ^ (uint16_t)bin) & 0x8000u) ? 1u : 0u);
    ctx->flag_c = (uint8_t)(bin > 0xFFFFu ? 1u : 0u);
    uint16_t res;
    if (!ctx->flag_d) {
        res = (uint16_t)(bin & 0xFFFFu);
    } else {
        int32_t n0 = (int32_t)(lhs & 0x000Fu) + (int32_t)(inv & 0x000Fu) + (int32_t)c;
        if (n0 <= 0x000F) n0 -= 0x0006;
        int32_t n1 = (int32_t)(lhs & 0x00F0u) + (int32_t)(inv & 0x00F0u) + (n0 > 0x000F ? 0x0010 : 0) + (n0 & 0x000F);
        if (n1 <= 0x00FF) n1 -= 0x0060;
        int32_t n2 = (int32_t)(lhs & 0x0F00u) + (int32_t)(inv & 0x0F00u) + (n1 > 0x00FF ? 0x0100 : 0) + (n1 & 0x00FF);
        if (n2 <= 0x0FFF) n2 -= 0x0600;
        int32_t n3 = (int32_t)(lhs & 0xF000u) + (int32_t)(inv & 0xF000u) + (n2 > 0x0FFF ? 0x1000 : 0) + (n2 & 0x0FFF);
        if (n3 <= 0xFFFF) n3 -= 0x6000;
        res = (uint16_t)(n3 & 0xFFFF);
    }
    ctx->a = res;
    snes_set_nz16(ctx, res);
}

static inline void snes_cmp8(SnesCpuContext *ctx, uint8_t lhs, uint8_t rhs) {
    uint8_t diff = (uint8_t)(lhs - rhs);
    ctx->flag_c = (uint8_t)(lhs >= rhs ? 1u : 0u);
    snes_set_nz8(ctx, diff);
}

static inline void snes_cmp16(SnesCpuContext *ctx, uint16_t lhs, uint16_t rhs) {
    uint16_t diff = (uint16_t)(lhs - rhs);
    ctx->flag_c = (uint8_t)(lhs >= rhs ? 1u : 0u);
    snes_set_nz16(ctx, diff);
}

static inline void snes_bit8(SnesCpuContext *ctx, uint8_t val) {
    ctx->flag_z = (uint8_t)(((ctx->a & 0xFFu) & val) == 0u ? 1u : 0u);
    ctx->flag_n = (uint8_t)((val >> 7) & 1u);
    ctx->flag_v = (uint8_t)((val >> 6) & 1u);
}

static inline void snes_bit16(SnesCpuContext *ctx, uint16_t val) {
    ctx->flag_z = (uint8_t)((ctx->a & val) == 0u ? 1u : 0u);
    ctx->flag_n = (uint8_t)((val >> 15) & 1u);
    ctx->flag_v = (uint8_t)((val >> 14) & 1u);
}

static inline uint8_t snes_asl8(SnesCpuContext *ctx, uint8_t v) {
    ctx->flag_c = (uint8_t)((v >> 7) & 1u);
    uint8_t r = (uint8_t)(v << 1);
    snes_set_nz8(ctx, r);
    return r;
}

static inline uint16_t snes_asl16(SnesCpuContext *ctx, uint16_t v) {
    ctx->flag_c = (uint8_t)((v >> 15) & 1u);
    uint16_t r = (uint16_t)(v << 1);
    snes_set_nz16(ctx, r);
    return r;
}

static inline uint8_t snes_lsr8(SnesCpuContext *ctx, uint8_t v) {
    ctx->flag_c = (uint8_t)(v & 1u);
    uint8_t r = (uint8_t)(v >> 1);
    snes_set_nz8(ctx, r);
    return r;
}

static inline uint16_t snes_lsr16(SnesCpuContext *ctx, uint16_t v) {
    ctx->flag_c = (uint8_t)(v & 1u);
    uint16_t r = (uint16_t)(v >> 1);
    snes_set_nz16(ctx, r);
    return r;
}

static inline uint8_t snes_rol8(SnesCpuContext *ctx, uint8_t v) {
    uint8_t oc = ctx->flag_c;
    ctx->flag_c = (uint8_t)((v >> 7) & 1u);
    uint8_t r = (uint8_t)((uint8_t)(v << 1) | oc);
    snes_set_nz8(ctx, r);
    return r;
}

static inline uint16_t snes_rol16(SnesCpuContext *ctx, uint16_t v) {
    uint16_t oc = (uint16_t)ctx->flag_c;
    ctx->flag_c = (uint8_t)((v >> 15) & 1u);
    uint16_t r = (uint16_t)((uint16_t)(v << 1) | oc);
    snes_set_nz16(ctx, r);
    return r;
}

static inline uint8_t snes_ror8(SnesCpuContext *ctx, uint8_t v) {
    uint8_t oc = ctx->flag_c;
    ctx->flag_c = (uint8_t)(v & 1u);
    uint8_t r = (uint8_t)((v >> 1) | (uint8_t)(oc << 7));
    snes_set_nz8(ctx, r);
    return r;
}

static inline uint16_t snes_ror16(SnesCpuContext *ctx, uint16_t v) {
    uint16_t oc = (uint16_t)ctx->flag_c;
    ctx->flag_c = (uint8_t)(v & 1u);
    uint16_t r = (uint16_t)((v >> 1) | (uint16_t)(oc << 15));
    snes_set_nz16(ctx, r);
    return r;
}

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_CPU_65816_H */
