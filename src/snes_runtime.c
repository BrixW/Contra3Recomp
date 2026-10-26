#include <string.h>
#include "cpu_65816.h"
#include "snes_apu.h"

/* Forward declaration of generated global function dispatcher (src/dispatch.c) */
bool contra3_dispatch_call(SnesCpuContext *ctx, uint32_t addr24);

static uint16_t snes_vram_step(uint8_t vmain) {
    switch (vmain & 0x03u) {
        case 0u: return 1u;
        case 1u: return 32u;
        default: return 128u;
    }
}

static uint16_t snes_vram_translate(uint16_t addr, uint8_t vmain) {
    switch ((vmain >> 2) & 0x03u) {
        case 0u:
            return (uint16_t)(addr & 0x7FFFu);
        case 1u:
            return (uint16_t)((addr & 0x7F00u) | ((addr & 0x00E0u) >> 5) | ((addr & 0x001Fu) << 3));
        case 2u:
            return (uint16_t)((addr & 0x7E00u) | ((addr & 0x01C0u) >> 6) | ((addr & 0x003Fu) << 3));
        default:
            return (uint16_t)((addr & 0x7C00u) | ((addr & 0x0380u) >> 7) | ((addr & 0x007Fu) << 3));
    }
}

void snes_context_init(SnesCpuContext *ctx, const uint8_t *rom, size_t rom_size) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->rom = rom;
    ctx->rom_size = rom_size;
    ctx->sp = 0x01FFu;
    ctx->flag_m = 1u;
    ctx->flag_x = 1u;
    ctx->flag_i = 1u;
    ctx->flag_e = 1u;
    /* Expose IPL ready signature ($BBAA on $2140-$2141) by default for standalone execution */
    ctx->apu.auto_handshake = true;
    ctx->apu.apu_to_cpu[0] = 0xAAu;
    ctx->apu.apu_to_cpu[1] = 0xBBu;
    ctx->cpu_io.rdnmi_status = 0x82u;
}

static void snes_write_bbus(SnesCpuContext *ctx, uint8_t bbad, uint8_t val) {
    uint16_t reg = (uint16_t)(0x2100u | (uint16_t)bbad);
    if (reg <= 0x213Fu) {
        snes_write_ppu(ctx, reg, val);
    } else if (reg <= 0x217Fu) {
        snes_write_apu(ctx, reg, val);
    } else if (reg <= 0x2183u) {
        snes_write_wram_port(ctx, reg, val);
    }
}

static uint8_t snes_read_bbus(SnesCpuContext *ctx, uint8_t bbad) {
    uint16_t reg = (uint16_t)(0x2100u | (uint16_t)bbad);
    if (reg <= 0x213Fu) {
        return snes_read_ppu(ctx, reg);
    } else if (reg <= 0x217Fu) {
        return snes_read_apu(ctx, reg);
    } else if (reg <= 0x2183u) {
        return snes_read_wram_port(ctx, reg);
    }
    return 0u;
}

static void snes_execute_mdma(SnesCpuContext *ctx, uint8_t mask) {
    static const uint8_t k_unit_offsets[8][4] = {
        {0u, 0u, 0u, 0u}, /* Mode 0: 1 reg write once */
        {0u, 1u, 0u, 1u}, /* Mode 1: 2 regs (p, p+1) */
        {0u, 0u, 0u, 0u}, /* Mode 2: 1 reg write twice */
        {0u, 0u, 1u, 1u}, /* Mode 3: 2 regs write twice each */
        {0u, 1u, 2u, 3u}, /* Mode 4: 4 regs (p..p+3) */
        {0u, 1u, 0u, 1u}, /* Mode 5 */
        {0u, 0u, 0u, 0u}, /* Mode 6 */
        {0u, 0u, 1u, 1u}  /* Mode 7 */
    };

    ctx->cpu_io.dma_transfers++;
    for (unsigned ch = 0u; ch < 8u; ++ch) {
        if ((mask & (uint8_t)(1u << ch)) == 0u) {
            continue;
        }
        SnesDmaChannel *d = &ctx->dma[ch];
        uint32_t count = (d->das == 0u) ? 0x10000u : (uint32_t)d->das;
        uint8_t mode = (uint8_t)(d->dmap & 0x07u);
        bool b_to_a = (d->dmap & 0x80u) != 0u;
        bool fixed  = (d->dmap & 0x08u) != 0u;
        bool dec    = (d->dmap & 0x10u) != 0u;

        for (uint32_t i = 0u; i < count; ++i) {
            uint8_t b_reg = (uint8_t)(d->bbad + k_unit_offsets[mode][i & 3u]);
            uint32_t a_addr = ((uint32_t)d->a1b << 16) | (uint32_t)d->a1t;
            if (!b_to_a) {
                uint8_t v = snes_bus_read8(ctx, a_addr);
                snes_write_bbus(ctx, b_reg, v);
            } else {
                uint8_t v = snes_read_bbus(ctx, b_reg);
                snes_bus_write8(ctx, a_addr, v);
            }
            if (!fixed) {
                d->a1t = dec ? (uint16_t)(d->a1t - 1u) : (uint16_t)(d->a1t + 1u);
            }
        }
        d->das = 0u;
    }
}

void snes_hw_write_ppu(SnesCpuContext *ctx, uint16_t reg, uint8_t val) {
    if (reg >= 0x2100u && reg <= 0x213Fu) {
        ctx->ppu.regs[reg - 0x2100u] = val;
        ctx->ppu.write_count++;
    }
    switch (reg) {
        case PPU_OAMADDL:
            ctx->ppu.oamadd = (uint16_t)(((ctx->ppu.regs[0x03] & 0x01u) << 9) | ((uint16_t)val << 1));
            break;
        case PPU_OAMADDH:
            ctx->ppu.oamadd = (uint16_t)(((val & 0x01u) << 9) | ((uint16_t)ctx->ppu.regs[0x02] << 1));
            break;
        case PPU_OAMDATA:
            if (ctx->ppu.oamadd < sizeof(ctx->ppu.oam)) {
                ctx->ppu.oam[ctx->ppu.oamadd] = val;
            }
            ctx->ppu.oamadd = (uint16_t)((ctx->ppu.oamadd + 1u) % 0x220u);
            break;
        case PPU_BG1HOFS:
        case PPU_BG2HOFS:
        case PPU_BG3HOFS:
        case PPU_BG4HOFS: {
            unsigned bg = (unsigned)((reg - PPU_BG1HOFS) >> 1);
            ctx->ppu.bghofs[bg] = (uint16_t)(((uint16_t)val << 8) | (uint16_t)(ctx->ppu.bg_ofs_latch & 0xF8u) | (uint16_t)(ctx->ppu.bghofs_latch[bg] & 0x07u));
            ctx->ppu.bg_ofs_latch = val;
            ctx->ppu.bghofs_latch[bg] = val;
            if (reg == PPU_BG1HOFS) {
                uint16_t raw13 = (uint16_t)(((uint16_t)val << 8) | (uint16_t)ctx->ppu.m7_latch);
                ctx->ppu.m7_latch = val;
                ctx->ppu.m7hofs = (int16_t)((int16_t)(raw13 << 3) >> 3);
            }
            break;
        }
        case PPU_BG1VOFS:
        case PPU_BG2VOFS:
        case PPU_BG3VOFS:
        case PPU_BG4VOFS: {
            unsigned bg = (unsigned)((reg - PPU_BG1VOFS) >> 1);
            ctx->ppu.bgvofs[bg] = (uint16_t)(((uint16_t)val << 8) | (uint16_t)ctx->ppu.bg_ofs_latch);
            ctx->ppu.bg_ofs_latch = val;
            if (reg == PPU_BG1VOFS) {
                uint16_t raw13 = (uint16_t)(((uint16_t)val << 8) | (uint16_t)ctx->ppu.m7_latch);
                ctx->ppu.m7_latch = val;
                ctx->ppu.m7vofs = (int16_t)((int16_t)(raw13 << 3) >> 3);
            }
            break;
        }
        case PPU_VMAIN:
            ctx->ppu.vmain = val;
            break;
        case PPU_VMADDL:
            ctx->ppu.vmadd = (uint16_t)((ctx->ppu.vmadd & 0xFF00u) | (uint16_t)val);
            break;
        case PPU_VMADDH:
            ctx->ppu.vmadd = (uint16_t)((ctx->ppu.vmadd & 0x00FFu) | ((uint16_t)val << 8));
            break;
        case PPU_VMDATAL: {
            uint16_t waddr = snes_vram_translate(ctx->ppu.vmadd, ctx->ppu.vmain);
            ctx->ppu.vram[waddr] = (uint16_t)((ctx->ppu.vram[waddr] & 0xFF00u) | (uint16_t)val);
            if ((ctx->ppu.vmain & 0x80u) == 0u) {
                ctx->ppu.vmadd = (uint16_t)(ctx->ppu.vmadd + snes_vram_step(ctx->ppu.vmain));
            }
            break;
        }
        case PPU_VMDATAH: {
            uint16_t waddr = snes_vram_translate(ctx->ppu.vmadd, ctx->ppu.vmain);
            ctx->ppu.vram[waddr] = (uint16_t)((ctx->ppu.vram[waddr] & 0x00FFu) | ((uint16_t)val << 8));
            if ((ctx->ppu.vmain & 0x80u) != 0u) {
                ctx->ppu.vmadd = (uint16_t)(ctx->ppu.vmadd + snes_vram_step(ctx->ppu.vmain));
            }
            break;
        }
        case PPU_M7A:
            ctx->ppu.m7a = (int16_t)(((uint16_t)val << 8) | (uint16_t)ctx->ppu.m7_latch);
            ctx->ppu.m7_latch = val;
            ctx->ppu.mpy_result = (uint32_t)((int32_t)ctx->ppu.m7a * (int32_t)ctx->ppu.m7b) & 0xFFFFFFu;
            break;
        case PPU_M7B:
            ctx->ppu.m7b_full = (int16_t)(((uint16_t)val << 8) | (uint16_t)ctx->ppu.m7_latch);
            ctx->ppu.m7_latch = val;
            ctx->ppu.m7b = (int8_t)val;
            ctx->ppu.mpy_result = (uint32_t)((int32_t)ctx->ppu.m7a * (int32_t)ctx->ppu.m7b) & 0xFFFFFFu;
            break;
        case PPU_M7C:
            ctx->ppu.m7c = (int16_t)(((uint16_t)val << 8) | (uint16_t)ctx->ppu.m7_latch);
            ctx->ppu.m7_latch = val;
            break;
        case PPU_M7D:
            ctx->ppu.m7d = (int16_t)(((uint16_t)val << 8) | (uint16_t)ctx->ppu.m7_latch);
            ctx->ppu.m7_latch = val;
            break;
        case PPU_M7X: {
            uint16_t raw13 = (uint16_t)(((uint16_t)val << 8) | (uint16_t)ctx->ppu.m7_latch);
            ctx->ppu.m7_latch = val;
            ctx->ppu.m7x = (int16_t)((int16_t)(raw13 << 3) >> 3);
            break;
        }
        case PPU_M7Y: {
            uint16_t raw13 = (uint16_t)(((uint16_t)val << 8) | (uint16_t)ctx->ppu.m7_latch);
            ctx->ppu.m7_latch = val;
            ctx->ppu.m7y = (int16_t)((int16_t)(raw13 << 3) >> 3);
            break;
        }
        case PPU_CGADD:
            ctx->ppu.cgadd = val;
            ctx->ppu.cg_latch = 0u;
            break;
        case PPU_CGDATA:
            if (ctx->ppu.cg_latch == 0u) {
                ctx->ppu.cg_lsb = val;
                ctx->ppu.cg_latch = 1u;
            } else {
                ctx->ppu.cgram[ctx->ppu.cgadd] = (uint16_t)(((uint16_t)(val & 0x7Fu) << 8) | (uint16_t)ctx->ppu.cg_lsb);
                ctx->ppu.cgadd = (uint8_t)(ctx->ppu.cgadd + 1u);
                ctx->ppu.cg_latch = 0u;
            }
            break;
        case PPU_COLDATA: {
            uint16_t intensity = (uint16_t)(val & 0x1Fu);
            if ((val & 0x20u) != 0u) {
                ctx->ppu.fixed_color = (uint16_t)((ctx->ppu.fixed_color & ~0x001Fu) | intensity);
            }
            if ((val & 0x40u) != 0u) {
                ctx->ppu.fixed_color = (uint16_t)((ctx->ppu.fixed_color & ~0x03E0u) | (intensity << 5));
            }
            if ((val & 0x80u) != 0u) {
                ctx->ppu.fixed_color = (uint16_t)((ctx->ppu.fixed_color & ~0x7C00u) | (intensity << 10));
            }
            break;
        }
        default:
            break;
    }
    if (ctx->on_write_ppu != NULL) {
        ctx->on_write_ppu(ctx, reg, val);
    }
}

uint8_t snes_hw_read_ppu(SnesCpuContext *ctx, uint16_t reg) {
    if (ctx->on_read_ppu != NULL) {
        return ctx->on_read_ppu(ctx, reg);
    }
    switch (reg) {
        case PPU_MPYL:   return (uint8_t)(ctx->ppu.mpy_result & 0xFFu);
        case PPU_MPYM:   return (uint8_t)((ctx->ppu.mpy_result >> 8) & 0xFFu);
        case PPU_MPYH:   return (uint8_t)((ctx->ppu.mpy_result >> 16) & 0xFFu);
        case PPU_STAT77: return 0x01u;
        case PPU_STAT78: return 0x01u;
        default:
            if (reg >= 0x2100u && reg <= 0x213Fu) {
                return ctx->ppu.regs[reg - 0x2100u];
            }
            return 0u;
    }
}

void snes_hw_write_cpu_io(SnesCpuContext *ctx, uint16_t reg, uint8_t val) {
    if (reg >= 0x4200u && reg <= 0x421Fu) {
        ctx->cpu_io.regs[reg - 0x4200u] = val;
    }
    switch (reg) {
        case CPUIO_NMITIMEN:
            ctx->cpu_io.nmitimen = val;
            break;
        case CPUIO_WRMPYA:
            ctx->cpu_io.wrmpya = val;
            break;
        case CPUIO_WRMPYB:
            ctx->cpu_io.wrmpyb = val;
            ctx->cpu_io.rdmpy = (uint16_t)((uint16_t)ctx->cpu_io.wrmpya * (uint16_t)val);
            ctx->cpu_io.rddiv = (uint16_t)val;
            break;
        case CPUIO_WRDIVL:
            ctx->cpu_io.wrdiv = (uint16_t)((ctx->cpu_io.wrdiv & 0xFF00u) | (uint16_t)val);
            break;
        case CPUIO_WRDIVH:
            ctx->cpu_io.wrdiv = (uint16_t)((ctx->cpu_io.wrdiv & 0x00FFu) | ((uint16_t)val << 8));
            break;
        case CPUIO_WRDIVB:
            ctx->cpu_io.wrdivb = val;
            if (val == 0u) {
                ctx->cpu_io.rddiv = 0xFFFFu;
                ctx->cpu_io.rdmpy = ctx->cpu_io.wrdiv;
            } else {
                ctx->cpu_io.rddiv = (uint16_t)(ctx->cpu_io.wrdiv / (uint16_t)val);
                ctx->cpu_io.rdmpy = (uint16_t)(ctx->cpu_io.wrdiv % (uint16_t)val);
            }
            break;
        case CPUIO_HTIMEL:
            ctx->cpu_io.htime = (uint16_t)((ctx->cpu_io.htime & 0xFF00u) | (uint16_t)val);
            break;
        case CPUIO_HTIMEH:
            ctx->cpu_io.htime = (uint16_t)((ctx->cpu_io.htime & 0x00FFu) | ((uint16_t)(val & 0x01u) << 8));
            break;
        case CPUIO_VTIMEL:
            ctx->cpu_io.vtime = (uint16_t)((ctx->cpu_io.vtime & 0xFF00u) | (uint16_t)val);
            break;
        case CPUIO_VTIMEH:
            ctx->cpu_io.vtime = (uint16_t)((ctx->cpu_io.vtime & 0x00FFu) | ((uint16_t)(val & 0x01u) << 8));
            break;
        case CPUIO_MDMAEN:
            if (val != 0u) {
                snes_execute_mdma(ctx, val);
            }
            break;
        case CPUIO_HDMAEN:
            ctx->cpu_io.hdmaen = val;
            break;
        case CPUIO_MEMSEL:
            ctx->cpu_io.memsel = val;
            break;
        default:
            break;
    }
    if (ctx->on_write_cpu_io != NULL) {
        ctx->on_write_cpu_io(ctx, reg, val);
    }
}

uint8_t snes_hw_read_cpu_io(SnesCpuContext *ctx, uint16_t reg) {
    if (ctx->on_read_cpu_io != NULL) {
        return ctx->on_read_cpu_io(ctx, reg);
    }
    switch (reg) {
        case CPUIO_RDNMI: {
            uint8_t v = ctx->cpu_io.rdnmi_status;
            ctx->cpu_io.rdnmi_status &= 0x7Fu;
            return (uint8_t)(v | 0x02u);
        }
        case CPUIO_TIMEUP: {
            uint8_t v = ctx->cpu_io.timeup_status;
            ctx->cpu_io.timeup_status = 0u;
            return v;
        }
        case CPUIO_HVBJOY:
            /* Clear auto-joypad busy bit (bit 0) so joypad polling completes cleanly */
            ctx->cpu_io.hvbjoy_toggle ^= 0x80u;
            return (uint8_t)(ctx->cpu_io.hvbjoy_toggle & 0xFEu);
        case CPUIO_RDDIVL:
            return (uint8_t)(ctx->cpu_io.rddiv & 0xFFu);
        case CPUIO_RDDIVH:
            return (uint8_t)((ctx->cpu_io.rddiv >> 8) & 0xFFu);
        case CPUIO_RDMPYL:
            return (uint8_t)(ctx->cpu_io.rdmpy & 0xFFu);
        case CPUIO_RDMPYH:
            return (uint8_t)((ctx->cpu_io.rdmpy >> 8) & 0xFFu);
        case CPUIO_JOY1L: return (uint8_t)(ctx->cpu_io.joy_pads[0] & 0xFFu);
        case CPUIO_JOY1H: return (uint8_t)((ctx->cpu_io.joy_pads[0] >> 8) & 0xFFu);
        case CPUIO_JOY2L: return (uint8_t)(ctx->cpu_io.joy_pads[1] & 0xFFu);
        case CPUIO_JOY2H: return (uint8_t)((ctx->cpu_io.joy_pads[1] >> 8) & 0xFFu);
        case CPUIO_JOY3L: return (uint8_t)(ctx->cpu_io.joy_pads[2] & 0xFFu);
        case CPUIO_JOY3H: return (uint8_t)((ctx->cpu_io.joy_pads[2] >> 8) & 0xFFu);
        case CPUIO_JOY4L: return (uint8_t)(ctx->cpu_io.joy_pads[3] & 0xFFu);
        case CPUIO_JOY4H: return (uint8_t)((ctx->cpu_io.joy_pads[3] >> 8) & 0xFFu);
        default:
            if (reg >= 0x4200u && reg <= 0x421Fu) {
                return ctx->cpu_io.regs[reg - 0x4200u];
            }
            return 0u;
    }
}

void snes_hw_write_apu(SnesCpuContext *ctx, uint16_t reg, uint8_t val) {
    uint8_t port = (uint8_t)((reg - 0x2140u) & 0x03u);
    ctx->apu.cpu_to_apu[port] = val;
    ctx->apu.write_count++;
    if (ctx->apu_core != NULL) {
        snes_apu_write_port(ctx->apu_core, port, val);
        return;
    }
    if (ctx->apu.auto_handshake) {
        /*
         * Cold boot starts with $BBAA in apu_to_cpu[0..1] (set in snes_context_init).
         * Every subsequent SPC700 IPL ($CC / rolling byte index) and Konami driver
         * ($84 / $FF / command ID) handshake polls $2140 until it echoes CPU's write.
         */
        ctx->apu.apu_to_cpu[port] = val;
    }
}

uint8_t snes_hw_read_apu(SnesCpuContext *ctx, uint16_t reg) {
    uint8_t port = (uint8_t)((reg - 0x2140u) & 0x03u);
    if (ctx->apu_core != NULL) {
        uint8_t v = snes_apu_read_port(ctx->apu_core, port);
        ctx->apu.apu_to_cpu[port] = v;
        return v;
    }
    return ctx->apu.apu_to_cpu[port];
}

void snes_hw_write_wram_port(SnesCpuContext *ctx, uint16_t reg, uint8_t val) {
    switch (reg) {
        case WRAM_WMDATA:
            ctx->wram[ctx->wram_port_addr & 0x1FFFFu] = val;
            ctx->wram_port_addr = (ctx->wram_port_addr + 1u) & 0x1FFFFu;
            break;
        case WRAM_WMADDL:
            ctx->wram_port_addr = (ctx->wram_port_addr & 0x1FF00u) | (uint32_t)val;
            break;
        case WRAM_WMADDM:
            ctx->wram_port_addr = (ctx->wram_port_addr & 0x100FFu) | ((uint32_t)val << 8);
            break;
        case WRAM_WMADDH:
            ctx->wram_port_addr = (ctx->wram_port_addr & 0x0FFFFu) | (((uint32_t)val & 1u) << 16);
            break;
        default:
            break;
    }
}

uint8_t snes_hw_read_wram_port(SnesCpuContext *ctx, uint16_t reg) {
    if (reg == WRAM_WMDATA) {
        uint8_t v = ctx->wram[ctx->wram_port_addr & 0x1FFFFu];
        ctx->wram_port_addr = (ctx->wram_port_addr + 1u) & 0x1FFFFu;
        return v;
    }
    return 0u;
}

void snes_hw_write_joy_io(SnesCpuContext *ctx, uint16_t reg, uint8_t val) {
    (void)ctx;
    (void)reg;
    (void)val;
}

uint8_t snes_hw_read_joy_io(SnesCpuContext *ctx, uint16_t reg) {
    (void)ctx;
    (void)reg;
    return 0u;
}

void snes_hw_write_dma(SnesCpuContext *ctx, uint16_t reg, uint8_t val) {
    unsigned ch = ((unsigned)(reg - 0x4300u) >> 4) & 7u;
    unsigned off = (unsigned)(reg - 0x4300u) & 0x0Fu;
    SnesDmaChannel *d = &ctx->dma[ch];
    switch (off) {
        case 0x0u: d->dmap = val; break;
        case 0x1u: d->bbad = val; break;
        case 0x2u: d->a1t = (uint16_t)((d->a1t & 0xFF00u) | (uint16_t)val); break;
        case 0x3u: d->a1t = (uint16_t)((d->a1t & 0x00FFu) | ((uint16_t)val << 8)); break;
        case 0x4u: d->a1b = val; break;
        case 0x5u: d->das = (uint16_t)((d->das & 0xFF00u) | (uint16_t)val); break;
        case 0x6u: d->das = (uint16_t)((d->das & 0x00FFu) | ((uint16_t)val << 8)); break;
        case 0x7u: d->dasb = val; break;
        case 0x8u: d->a2a = (uint16_t)((d->a2a & 0xFF00u) | (uint16_t)val); break;
        case 0x9u: d->a2a = (uint16_t)((d->a2a & 0x00FFu) | ((uint16_t)val << 8)); break;
        case 0xAu: d->ntrl = val; break;
        default:   d->unused_b = val; break;
    }
}

uint8_t snes_hw_read_dma(SnesCpuContext *ctx, uint16_t reg) {
    unsigned ch = ((unsigned)(reg - 0x4300u) >> 4) & 7u;
    unsigned off = (unsigned)(reg - 0x4300u) & 0x0Fu;
    const SnesDmaChannel *d = &ctx->dma[ch];
    switch (off) {
        case 0x0u: return d->dmap;
        case 0x1u: return d->bbad;
        case 0x2u: return (uint8_t)(d->a1t & 0xFFu);
        case 0x3u: return (uint8_t)((d->a1t >> 8) & 0xFFu);
        case 0x4u: return d->a1b;
        case 0x5u: return (uint8_t)(d->das & 0xFFu);
        case 0x6u: return (uint8_t)((d->das >> 8) & 0xFFu);
        case 0x7u: return d->dasb;
        case 0x8u: return (uint8_t)(d->a2a & 0xFFu);
        case 0x9u: return (uint8_t)((d->a2a >> 8) & 0xFFu);
        case 0xAu: return d->ntrl;
        default:   return d->unused_b;
    }
}

uint8_t snes_bus_read8(SnesCpuContext *ctx, uint32_t addr24) {
    uint8_t bank = (uint8_t)((addr24 >> 16) & 0xFFu);
    uint16_t off = (uint16_t)(addr24 & 0xFFFFu);

    if (bank == 0x7Eu || bank == 0x7Fu) {
        return ctx->wram[((uint32_t)(bank - 0x7Eu) << 16) | (uint32_t)off];
    }

    uint8_t bank_lo = (uint8_t)(bank & 0x7Fu);
    if (off < 0x8000u) {
        if (bank_lo < 0x40u) {
            if (off < 0x2000u) {
                return ctx->wram[off];
            }
            if (off >= 0x2100u && off <= 0x213Fu) {
                return snes_read_ppu(ctx, off);
            }
            if (off >= 0x2140u && off <= 0x217Fu) {
                return snes_read_apu(ctx, off);
            }
            if (off >= 0x2180u && off <= 0x2183u) {
                return snes_read_wram_port(ctx, off);
            }
            if (off == 0x4016u || off == 0x4017u) {
                return snes_read_joy_io(ctx, off);
            }
            if (off >= 0x4200u && off <= 0x421Fu) {
                return snes_read_cpu_io(ctx, off);
            }
            if (off >= 0x4300u && off <= 0x437Fu) {
                return snes_read_dma(ctx, off);
            }
            return 0u;
        }
        if (bank_lo >= 0x70u) {
            return ctx->sram[off & 0x7FFFu];
        }
        return 0u;
    }

    /* LoROM cartridge ROM mapping ($8000-$FFFF across 32 banks of 32 KB = 1 MB) */
    if (ctx->rom != NULL && ctx->rom_size > 0u) {
        size_t rom_off = ((size_t)(bank & 0x1Fu) * 0x8000u) + (size_t)(off - 0x8000u);
        if (rom_off < ctx->rom_size) {
            return ctx->rom[rom_off];
        }
    }
    return 0xFFu;
}

void snes_bus_write8(SnesCpuContext *ctx, uint32_t addr24, uint8_t val) {
    uint8_t bank = (uint8_t)((addr24 >> 16) & 0xFFu);
    uint16_t off = (uint16_t)(addr24 & 0xFFFFu);

    if (bank == 0x7Eu || bank == 0x7Fu) {
        ctx->wram[((uint32_t)(bank - 0x7Eu) << 16) | (uint32_t)off] = val;
        return;
    }

    uint8_t bank_lo = (uint8_t)(bank & 0x7Fu);
    if (off < 0x8000u) {
        if (bank_lo < 0x40u) {
            if (off < 0x2000u) {
                ctx->wram[off] = val;
                return;
            }
            if (off >= 0x2100u && off <= 0x213Fu) {
                snes_write_ppu(ctx, off, val);
                return;
            }
            if (off >= 0x2140u && off <= 0x217Fu) {
                snes_write_apu(ctx, off, val);
                return;
            }
            if (off >= 0x2180u && off <= 0x2183u) {
                snes_write_wram_port(ctx, off, val);
                return;
            }
            if (off == 0x4016u || off == 0x4017u) {
                snes_write_joy_io(ctx, off, val);
                return;
            }
            if (off >= 0x4200u && off <= 0x421Fu) {
                snes_write_cpu_io(ctx, off, val);
                return;
            }
            if (off >= 0x4300u && off <= 0x437Fu) {
                snes_write_dma(ctx, off, val);
                return;
            }
            return;
        }
        if (bank_lo >= 0x70u) {
            ctx->sram[off & 0x7FFFu] = val;
            return;
        }
    }
}

uint16_t snes_bus_read16(SnesCpuContext *ctx, uint32_t addr24) {
    uint16_t lo = (uint16_t)snes_bus_read8(ctx, addr24);
    uint16_t hi = (uint16_t)snes_bus_read8(ctx, (addr24 + 1u) & 0xFFFFFFu);
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

void snes_bus_write16(SnesCpuContext *ctx, uint32_t addr24, uint16_t val) {
    snes_bus_write8(ctx, addr24, (uint8_t)(val & 0xFFu));
    snes_bus_write8(ctx, (addr24 + 1u) & 0xFFFFFFu, (uint8_t)((val >> 8) & 0xFFu));
}

void snes_block_mvn(SnesCpuContext *ctx, uint8_t dst_bank, uint8_t src_bank) {
    ctx->db = dst_bank;
    uint32_t count = (uint32_t)ctx->a + 1u;
    for (uint32_t i = 0u; i < count; ++i) {
        uint8_t b = snes_bus_read8(ctx, ((uint32_t)src_bank << 16) | (uint32_t)ctx->x);
        snes_bus_write8(ctx, ((uint32_t)dst_bank << 16) | (uint32_t)ctx->y, b);
        if (ctx->flag_x) {
            ctx->x = (uint16_t)((ctx->x + 1u) & 0x00FFu);
            ctx->y = (uint16_t)((ctx->y + 1u) & 0x00FFu);
        } else {
            ctx->x = (uint16_t)(ctx->x + 1u);
            ctx->y = (uint16_t)(ctx->y + 1u);
        }
    }
    ctx->a = 0xFFFFu;
}

void snes_block_mvp(SnesCpuContext *ctx, uint8_t dst_bank, uint8_t src_bank) {
    ctx->db = dst_bank;
    uint32_t count = (uint32_t)ctx->a + 1u;
    for (uint32_t i = 0u; i < count; ++i) {
        uint8_t b = snes_bus_read8(ctx, ((uint32_t)src_bank << 16) | (uint32_t)ctx->x);
        snes_bus_write8(ctx, ((uint32_t)dst_bank << 16) | (uint32_t)ctx->y, b);
        if (ctx->flag_x) {
            ctx->x = (uint16_t)((ctx->x - 1u) & 0x00FFu);
            ctx->y = (uint16_t)((ctx->y - 1u) & 0x00FFu);
        } else {
            ctx->x = (uint16_t)(ctx->x - 1u);
            ctx->y = (uint16_t)(ctx->y - 1u);
        }
    }
    ctx->a = 0xFFFFu;
}

void snes_interp_fallback(SnesCpuContext *ctx, uint32_t target_addr24) {
    /* Normalize LoROM mirror bank ($80-$9F -> $00-$1F) and check null slot ($0000) */
    uint8_t bk = (uint8_t)((target_addr24 >> 16) & 0x1Fu);
    uint16_t pc = (uint16_t)(target_addr24 & 0xFFFFu);
    if (pc == 0x0000u) {
        /* Konami null-slot convention ($00:BD57[0], $00:C215[0], $05:A0B4[0], $05:A48A[0]) */
        return;
    }
    uint32_t norm24 = ((uint32_t)bk << 16) | (uint32_t)pc;
    if (contra3_dispatch_call(ctx, norm24)) {
        return;
    }
    ctx->unresolved_jumps++;
    ctx->last_unresolved_addr = target_addr24;
    if (ctx->on_interp_fallback != NULL) {
        ctx->on_interp_fallback(ctx, target_addr24);
    }
}
