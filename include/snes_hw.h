#ifndef CONTRA3_SNES_HW_H
#define CONTRA3_SNES_HW_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "snes_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SnesCpuContext;

/*
 * PPU Hardware Shadow & Memory Backing ($2100 - $213F)
 */
typedef struct SnesPpuState {
    uint8_t  regs[0x40];          /* Raw register write latches ($2100-$213F) */
    uint16_t vram[0x8000];        /* 64 KB Video RAM (32K 16-bit words) */
    uint16_t cgram[0x100];        /* 512 B Palette CGRAM (256 15-bit colors) */
    uint8_t  oam[0x220];          /* 544 B Sprite OAM table */
    uint16_t vmadd;               /* Current VRAM word address ($2116-$2117) */
    uint8_t  vmain;               /* VRAM increment mode ($2115) */
    uint16_t oamadd;              /* Current OAM byte address ($2102-$2103) */
    uint8_t  cgadd;               /* Current CGRAM word address ($2121) */
    uint8_t  cg_latch;            /* CGRAM low-byte flip-flop */
    uint8_t  cg_lsb;              /* Latched CGRAM low byte */
    uint8_t  m7_latch;            /* Mode 7 write-twice latch */
    int16_t  m7a;                 /* Mode 7 matrix A (16-bit signed) */
    int8_t   m7b;                 /* Mode 7 matrix B (8-bit signed, for $2134 MPY) */
    int16_t  m7b_full;            /* Mode 7 matrix B (16-bit signed, for scanline affine) */
    int16_t  m7c;                 /* Mode 7 matrix C (16-bit signed) */
    int16_t  m7d;                 /* Mode 7 matrix D (16-bit signed) */
    int16_t  m7x;                 /* Mode 7 center X (13-bit signed) */
    int16_t  m7y;                 /* Mode 7 center Y (13-bit signed) */
    int16_t  m7hofs;              /* Mode 7 horizontal scroll (13-bit signed) */
    int16_t  m7vofs;              /* Mode 7 vertical scroll (13-bit signed) */
    uint8_t  bg_ofs_latch;        /* Previous byte written to $210D-$2114 */
    uint8_t  bghofs_latch[4];     /* Fine 3-bit latch helper for BG1-BG4 HOFS */
    uint16_t bghofs[4];           /* Latched 10-bit BG1HOFS..BG4HOFS ($210D,$210F,$2111,$2113) */
    uint16_t bgvofs[4];           /* Latched 10-bit BG1VOFS..BG4VOFS ($210E,$2110,$2112,$2114) */
    uint16_t fixed_color;         /* 15-bit BGR555 fixed color built via $2132 COLDATA */
    uint32_t mpy_result;          /* Signed 24-bit PPU multiply product ($2134-$2136) */
    uint32_t write_count;         /* Diagnostic counter of PPU register writes */
} SnesPpuState;

/*
 * CPU On-Chip I/O State ($4200 - $421F)
 */
typedef struct SnesCpuIoState {
    uint8_t  regs[0x20];          /* Register latches ($4200-$421F) */
    uint8_t  nmitimen;            /* $4200 NMI/IRQ/Auto-Joypad enable */
    uint8_t  wrmpya;              /* $4202 Multiplicand A */
    uint8_t  wrmpyb;              /* $4203 Multiplier B */
    uint16_t wrdiv;               /* $4204-$4205 Dividend C */
    uint8_t  wrdivb;              /* $4206 Divisor B */
    uint16_t htime;               /* $4207-$4208 H-IRQ target */
    uint16_t vtime;               /* $4209-$420A V-IRQ target */
    uint8_t  hdmaen;              /* $420C Active HDMA channels */
    uint8_t  memsel;              /* $420D FastROM enable */
    uint16_t rddiv;               /* $4214-$4215 Division quotient */
    uint16_t rdmpy;               /* $4216-$4217 Product / Division remainder */
    uint16_t joy_pads[4];         /* $4218-$421F Auto-read controller states */
    uint8_t  rdnmi_status;        /* $4210 RDNMI status (auto-clears bit 7 on read) */
    uint8_t  timeup_status;       /* $4211 TIMEUP status (auto-clears bit 7 on read) */
    uint8_t  hvbjoy_toggle;       /* $4212 HVBJOY polling helper */
    uint32_t dma_transfers;       /* Diagnostic count of MDMAEN ($420B) bursts */
} SnesCpuIoState;

/*
 * APU / SPC700 Communication Port State ($2140 - $2143)
 */
typedef struct SnesApuState {
    uint8_t  cpu_to_apu[4];       /* Last bytes written by 65816 to $2140-$2143 */
    uint8_t  apu_to_cpu[4];       /* Bytes exposed by SPC700 to 65816 on $2140-$2143 */
    bool     auto_handshake;      /* Emulate Konami SPC700 IPL ($AA/$BB) + echo protocol */
    uint8_t  boot_phase;          /* IPL boot handshake state tracker */
    uint32_t write_count;         /* Diagnostic count of APU port writes */
} SnesApuState;

/*
 * DMA / HDMA Channel State ($4300 - $437F)
 */
typedef struct SnesDmaChannel {
    uint8_t  dmap;                /* $43x0 Transfer mode / direction */
    uint8_t  bbad;                /* $43x1 B-Bus register offset ($2100 + bbad) */
    uint16_t a1t;                 /* $43x2-$43x3 A-Bus source 16-bit address */
    uint8_t  a1b;                 /* $43x4 A-Bus source bank */
    uint16_t das;                 /* $43x5-$43x6 Byte count / indirect HDMA addr */
    uint8_t  dasb;                /* $43x7 Indirect HDMA bank */
    uint16_t a2a;                 /* $43x8-$43x9 HDMA table current address */
    uint8_t  ntrl;                /* $43xA HDMA line counter */
    uint8_t  unused_b;            /* $43xB/$43xF scratch byte */
} SnesDmaChannel;

/* Underlying hardware implementation functions (src/snes_runtime.c) */
void    snes_hw_write_ppu(struct SnesCpuContext *ctx, uint16_t reg, uint8_t val);
uint8_t snes_hw_read_ppu(struct SnesCpuContext *ctx, uint16_t reg);

void    snes_hw_write_cpu_io(struct SnesCpuContext *ctx, uint16_t reg, uint8_t val);
uint8_t snes_hw_read_cpu_io(struct SnesCpuContext *ctx, uint16_t reg);

void    snes_hw_write_apu(struct SnesCpuContext *ctx, uint16_t reg, uint8_t val);
uint8_t snes_hw_read_apu(struct SnesCpuContext *ctx, uint16_t reg);

void    snes_hw_write_wram_port(struct SnesCpuContext *ctx, uint16_t reg, uint8_t val);
uint8_t snes_hw_read_wram_port(struct SnesCpuContext *ctx, uint16_t reg);

void    snes_hw_write_joy_io(struct SnesCpuContext *ctx, uint16_t reg, uint8_t val);
uint8_t snes_hw_read_joy_io(struct SnesCpuContext *ctx, uint16_t reg);

void    snes_hw_write_dma(struct SnesCpuContext *ctx, uint16_t reg, uint8_t val);
uint8_t snes_hw_read_dma(struct SnesCpuContext *ctx, uint16_t reg);

/*
 * ============================================================================
 * Abstracted Project Hardware Interface Macros
 * All translated 65816 assembly accesses to SNES MMIO ($2100-$213F, $4200-$421F,
 * plus APU $2140-$217F, WRAM port $2180-$2183, Joypad $4016-$4017, DMA $4300-$437F)
 * route exclusively through these macros.
 * ============================================================================
 */
#define snes_write_ppu(ctx, reg, val)        snes_hw_write_ppu((ctx), (uint16_t)(reg), (uint8_t)(val))
#define snes_read_ppu(ctx, reg)              snes_hw_read_ppu((ctx), (uint16_t)(reg))

#define snes_write_cpu_io(ctx, reg, val)     snes_hw_write_cpu_io((ctx), (uint16_t)(reg), (uint8_t)(val))
#define snes_read_cpu_io(ctx, reg)           snes_hw_read_cpu_io((ctx), (uint16_t)(reg))

#define snes_write_apu(ctx, reg, val)        snes_hw_write_apu((ctx), (uint16_t)(reg), (uint8_t)(val))
#define snes_read_apu(ctx, reg)              snes_hw_read_apu((ctx), (uint16_t)(reg))

#define snes_write_wram_port(ctx, reg, val)  snes_hw_write_wram_port((ctx), (uint16_t)(reg), (uint8_t)(val))
#define snes_read_wram_port(ctx, reg)        snes_hw_read_wram_port((ctx), (uint16_t)(reg))

#define snes_write_joy_io(ctx, reg, val)     snes_hw_write_joy_io((ctx), (uint16_t)(reg), (uint8_t)(val))
#define snes_read_joy_io(ctx, reg)           snes_hw_read_joy_io((ctx), (uint16_t)(reg))

#define snes_write_dma(ctx, reg, val)        snes_hw_write_dma((ctx), (uint16_t)(reg), (uint8_t)(val))
#define snes_read_dma(ctx, reg)              snes_hw_read_dma((ctx), (uint16_t)(reg))

/*
 * 16-bit MMIO Bus Helpers:
 * On real 65816 hardware, a 16-bit access to MMIO performs two sequential 8-bit
 * bus cycles: low byte at (reg), followed by high byte at (reg + 1).
 */
static inline void snes_write_ppu16(struct SnesCpuContext *ctx, uint16_t reg, uint16_t val) {
    snes_write_ppu(ctx, reg, (uint8_t)(val & 0xFFu));
    snes_write_ppu(ctx, (uint16_t)(reg + 1u), (uint8_t)((val >> 8) & 0xFFu));
}

static inline uint16_t snes_read_ppu16(struct SnesCpuContext *ctx, uint16_t reg) {
    uint16_t lo = (uint16_t)snes_read_ppu(ctx, reg);
    uint16_t hi = (uint16_t)snes_read_ppu(ctx, (uint16_t)(reg + 1u));
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

static inline void snes_write_cpu_io16(struct SnesCpuContext *ctx, uint16_t reg, uint16_t val) {
    snes_write_cpu_io(ctx, reg, (uint8_t)(val & 0xFFu));
    snes_write_cpu_io(ctx, (uint16_t)(reg + 1u), (uint8_t)((val >> 8) & 0xFFu));
}

static inline uint16_t snes_read_cpu_io16(struct SnesCpuContext *ctx, uint16_t reg) {
    uint16_t lo = (uint16_t)snes_read_cpu_io(ctx, reg);
    uint16_t hi = (uint16_t)snes_read_cpu_io(ctx, (uint16_t)(reg + 1u));
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

static inline void snes_write_apu16(struct SnesCpuContext *ctx, uint16_t reg, uint16_t val) {
    snes_write_apu(ctx, reg, (uint8_t)(val & 0xFFu));
    snes_write_apu(ctx, (uint16_t)(reg + 1u), (uint8_t)((val >> 8) & 0xFFu));
}

static inline uint16_t snes_read_apu16(struct SnesCpuContext *ctx, uint16_t reg) {
    uint16_t lo = (uint16_t)snes_read_apu(ctx, reg);
    uint16_t hi = (uint16_t)snes_read_apu(ctx, (uint16_t)(reg + 1u));
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

static inline void snes_write_dma16(struct SnesCpuContext *ctx, uint16_t reg, uint16_t val) {
    snes_write_dma(ctx, reg, (uint8_t)(val & 0xFFu));
    snes_write_dma(ctx, (uint16_t)(reg + 1u), (uint8_t)((val >> 8) & 0xFFu));
}

static inline uint16_t snes_read_dma16(struct SnesCpuContext *ctx, uint16_t reg) {
    uint16_t lo = (uint16_t)snes_read_dma(ctx, reg);
    uint16_t hi = (uint16_t)snes_read_dma(ctx, (uint16_t)(reg + 1u));
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_SNES_HW_H */
