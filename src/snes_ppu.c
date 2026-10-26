/*
 * SNES S-PPU1 / S-PPU2 Scanline Software Rasterizer & Frame Runner
 * Supports Contra III's full graphical feature set:
 * - Modes 0, 1 (with BG3 HUD top-priority), 2 (offset-per-tile), 3, 4, and Mode 7
 *   (full 16-bit affine matrix + V-IRQ mid-screen split rasterization for Stages 2 & 5)
 * - 128 OAM Sprites (all OBSEL sizes, 9-bit X wrap, H/V flip, 4 priority tiers)
 * - Per-scanline H-Blank DMA (HDMA, direct & indirect modes 0..7)
 * - Dual Windows (W1/W2 + AND/OR/XOR/XNOR logic), Main/Sub Screen Color Math
 *   (CGWSEL / CGADSUB / COLDATA add, subtract, half-color), Mosaic, and Brightness
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snes_ppu.h"
#include "snes_apu.h"
#include "snes_mainthread.h"

/* Layer IDs matching CGADSUB ($2131) bit positions: 0..3 = BG1..BG4, 4 = OBJ, 5 = BACKDROP */
#define LAYER_BG1       0u
#define LAYER_BG2       1u
#define LAYER_BG3       2u
#define LAYER_BG4       3u
#define LAYER_OBJ       4u
#define LAYER_BACKDROP  5u

typedef struct HdmaRuntimeChan {
    bool     active;
    bool     do_transfer;
    uint16_t table_addr;
    uint8_t  line_counter;
    uint16_t indirect_addr;
} HdmaRuntimeChan;

static void ppu_hdma_write_bbus(SnesCpuContext *ctx, uint8_t bbad, uint8_t val) {
    uint16_t reg = (uint16_t)(0x2100u | (uint16_t)bbad);
    if (reg <= 0x213Fu) {
        snes_write_ppu(ctx, reg, val);
    } else if (reg <= 0x217Fu) {
        snes_write_apu(ctx, reg, val);
    } else if (reg <= 0x2183u) {
        snes_write_wram_port(ctx, reg, val);
    }
}

static void ppu_hdma_transfer_unit(SnesCpuContext *ctx, const SnesDmaChannel *d, HdmaRuntimeChan *rt) {
    static const uint8_t k_unit_len[8] = {1u, 2u, 2u, 4u, 4u, 4u, 2u, 4u};
    static const uint8_t k_unit_off[8][4] = {
        {0u, 0u, 0u, 0u}, /* Mode 0: p */
        {0u, 1u, 0u, 0u}, /* Mode 1: p, p+1 */
        {0u, 0u, 0u, 0u}, /* Mode 2: p, p */
        {0u, 0u, 1u, 1u}, /* Mode 3: p, p, p+1, p+1 */
        {0u, 1u, 2u, 3u}, /* Mode 4: p, p+1, p+2, p+3 */
        {0u, 1u, 0u, 1u}, /* Mode 5: p, p+1, p, p+1 */
        {0u, 0u, 0u, 0u}, /* Mode 6: p, p */
        {0u, 0u, 1u, 1u}  /* Mode 7: p, p, p+1, p+1 */
    };
    uint8_t mode = (uint8_t)(d->dmap & 0x07u);
    bool indirect = (d->dmap & 0x40u) != 0u;
    uint8_t len = k_unit_len[mode];
    for (uint8_t i = 0u; i < len; ++i) {
        uint32_t src_addr;
        if (indirect) {
            src_addr = ((uint32_t)d->dasb << 16) | (uint32_t)rt->indirect_addr;
            rt->indirect_addr = (uint16_t)(rt->indirect_addr + 1u);
        } else {
            src_addr = ((uint32_t)d->a1b << 16) | (uint32_t)rt->table_addr;
            rt->table_addr = (uint16_t)(rt->table_addr + 1u);
        }
        uint8_t val = snes_bus_read8(ctx, src_addr);
        ppu_hdma_write_bbus(ctx, (uint8_t)(d->bbad + k_unit_off[mode][i]), val);
    }
}

static void ppu_hdma_init(SnesCpuContext *ctx, HdmaRuntimeChan rt[8]) {
    uint8_t mask = ctx->cpu_io.hdmaen;
    for (unsigned ch = 0u; ch < 8u; ++ch) {
        memset(&rt[ch], 0, sizeof(rt[ch]));
        if ((mask & (uint8_t)(1u << ch)) == 0u) {
            continue;
        }
        const SnesDmaChannel *d = &ctx->dma[ch];
        rt[ch].table_addr = d->a1t;
        uint8_t lc = snes_bus_read8(ctx, ((uint32_t)d->a1b << 16) | (uint32_t)rt[ch].table_addr);
        rt[ch].table_addr = (uint16_t)(rt[ch].table_addr + 1u);
        rt[ch].line_counter = lc;
        if (lc == 0u) {
            rt[ch].active = false;
            continue;
        }
        rt[ch].active = true;
        if ((d->dmap & 0x40u) != 0u) {
            uint16_t lo = (uint16_t)snes_bus_read8(ctx, ((uint32_t)d->a1b << 16) | (uint32_t)rt[ch].table_addr);
            uint16_t hi = (uint16_t)snes_bus_read8(ctx, ((uint32_t)d->a1b << 16) | (uint32_t)(uint16_t)(rt[ch].table_addr + 1u));
            rt[ch].table_addr = (uint16_t)(rt[ch].table_addr + 2u);
            rt[ch].indirect_addr = (uint16_t)(lo | (uint16_t)(hi << 8));
        }
        ppu_hdma_transfer_unit(ctx, d, &rt[ch]);
    }
}

static void ppu_hdma_step_scanline(SnesCpuContext *ctx, HdmaRuntimeChan rt[8]) {
    uint8_t mask = ctx->cpu_io.hdmaen;
    for (unsigned ch = 0u; ch < 8u; ++ch) {
        if (!rt[ch].active || (mask & (uint8_t)(1u << ch)) == 0u) {
            continue;
        }
        const SnesDmaChannel *d = &ctx->dma[ch];
        /*
         * Exact Ricoh 5A22 8-bit NTRL ($43xA) decrement:
         * When NTRL == 0x80 (128 scanlines non-repeat), NTRL - 1 borrows from bit 7 -> 0x7F,
         * clearing bit 7 so repeat is false for the remaining 127 scanlines!
         */
        rt[ch].line_counter = (uint8_t)(rt[ch].line_counter - 1u);
        rt[ch].do_transfer = (rt[ch].line_counter & 0x80u) != 0u;
        if ((rt[ch].line_counter & 0x7Fu) == 0u) {
            uint8_t lc = snes_bus_read8(ctx, ((uint32_t)d->a1b << 16) | (uint32_t)rt[ch].table_addr);
            rt[ch].table_addr = (uint16_t)(rt[ch].table_addr + 1u);
            rt[ch].line_counter = lc;
            if (lc == 0u) {
                rt[ch].active = false;
                rt[ch].do_transfer = false;
                continue;
            }
            if ((d->dmap & 0x40u) != 0u) {
                uint16_t lo = (uint16_t)snes_bus_read8(ctx, ((uint32_t)d->a1b << 16) | (uint32_t)rt[ch].table_addr);
                uint16_t hi = (uint16_t)snes_bus_read8(ctx, ((uint32_t)d->a1b << 16) | (uint32_t)(uint16_t)(rt[ch].table_addr + 1u));
                rt[ch].table_addr = (uint16_t)(rt[ch].table_addr + 2u);
                rt[ch].indirect_addr = (uint16_t)(lo | (uint16_t)(hi << 8));
            }
            rt[ch].do_transfer = true;
        }
        if (rt[ch].do_transfer) {
            ppu_hdma_transfer_unit(ctx, d, &rt[ch]);
        }
    }
}

static inline bool ppu_eval_window(uint8_t x, uint8_t wsel, uint8_t wlog, uint8_t wh0, uint8_t wh1, uint8_t wh2, uint8_t wh3) {
    bool w1_en = (wsel & 0x02u) != 0u;
    bool w2_en = (wsel & 0x08u) != 0u;
    if (!w1_en && !w2_en) {
        return false;
    }
    bool w1 = false;
    if (w1_en) {
        w1 = (x >= wh0 && x <= wh1);
        if ((wsel & 0x01u) != 0u) {
            w1 = !w1;
        }
    }
    bool w2 = false;
    if (w2_en) {
        w2 = (x >= wh2 && x <= wh3);
        if ((wsel & 0x04u) != 0u) {
            w2 = !w2;
        }
    }
    if (w1_en && !w2_en) return w1;
    if (!w1_en && w2_en) return w2;
    switch (wlog & 0x03u) {
        case 0u: return w1 || w2;          /* OR */
        case 1u: return w1 && w2;          /* AND */
        case 2u: return w1 != w2;          /* XOR */
        default: return w1 == w2;          /* XNOR */
    }
}

static inline uint8_t ppu_fetch_tile_pixel(const uint16_t *vram, uint16_t chr_base_word, uint16_t tile_num,
                                           unsigned bpp, unsigned fx, unsigned fy) {
    uint8_t bit = (uint8_t)(7u - (fx & 7u));
    if (bpp == 2u) {
        uint16_t w0 = vram[(chr_base_word + tile_num * 8u + fy) & 0x7FFFu];
        return (uint8_t)(((w0 >> bit) & 1u) | (((w0 >> (bit + 8u)) & 1u) << 1));
    } else if (bpp == 4u) {
        uint16_t addr = (uint16_t)(chr_base_word + tile_num * 16u + fy);
        uint16_t w0 = vram[addr & 0x7FFFu];
        uint16_t w1 = vram[(addr + 8u) & 0x7FFFu];
        return (uint8_t)(
            ((w0 >> bit) & 1u) |
            (((w0 >> (bit + 8u)) & 1u) << 1) |
            (((w1 >> bit) & 1u) << 2) |
            (((w1 >> (bit + 8u)) & 1u) << 3)
        );
    } else {
        uint16_t addr = (uint16_t)(chr_base_word + tile_num * 32u + fy);
        uint16_t w0 = vram[addr & 0x7FFFu];
        uint16_t w1 = vram[(addr + 8u) & 0x7FFFu];
        uint16_t w2 = vram[(addr + 16u) & 0x7FFFu];
        uint16_t w3 = vram[(addr + 24u) & 0x7FFFu];
        return (uint8_t)(
            ((w0 >> bit) & 1u) |
            (((w0 >> (bit + 8u)) & 1u) << 1) |
            (((w1 >> bit) & 1u) << 2) |
            (((w1 >> (bit + 8u)) & 1u) << 3) |
            (((w2 >> bit) & 1u) << 4) |
            (((w2 >> (bit + 8u)) & 1u) << 5) |
            (((w3 >> bit) & 1u) << 6) |
            (((w3 >> (bit + 8u)) & 1u) << 7)
        );
    }
}

static uint16_t ppu_lookup_tilemap(const uint16_t *vram, uint8_t sc, unsigned map_x, unsigned map_y) {
    uint16_t base = (uint16_t)(((uint16_t)(sc & 0xFCu) << 8) & 0x7FFFu);
    uint8_t size_mode = (uint8_t)(sc & 0x03u);
    unsigned sx = (map_x >> 5) & 1u;
    unsigned sy = (map_y >> 5) & 1u;
    uint16_t screen_off = 0u;
    switch (size_mode) {
        case 0u: screen_off = 0u; break;
        case 1u: screen_off = (uint16_t)(sx * 0x0400u); break;
        case 2u: screen_off = (uint16_t)(sy * 0x0400u); break;
        default: screen_off = (uint16_t)((sy * 2u + sx) * 0x0400u); break;
    }
    uint16_t idx = (uint16_t)(((map_y & 31u) << 5) | (map_x & 31u));
    return vram[(base + screen_off + idx) & 0x7FFFu];
}

static void ppu_decode_bg_scanline(const SnesPpuState *ppu, unsigned bg, unsigned bpp, unsigned y,
                                   bool mode0_pal, bool opt_mode2,
                                   uint8_t out_cgram_idx[256], uint8_t out_prio[256]) {
    memset(out_cgram_idx, 0, 256u);
    memset(out_prio, 0, 256u);

    uint8_t mosaic = ppu->regs[0x06];
    unsigned mos_size = 1u;
    if ((mosaic & (uint8_t)(1u << bg)) != 0u) {
        mos_size = (unsigned)((mosaic >> 4) & 0x0Fu) + 1u;
        y = (y / mos_size) * mos_size;
    }

    uint8_t bgmode = ppu->regs[0x05];
    bool tile16 = ((bgmode >> (4u + bg)) & 1u) != 0u;
    uint8_t sc = ppu->regs[0x07u + bg];
    uint8_t nba = (bg < 2u) ? ppu->regs[0x0Bu] : ppu->regs[0x0Cu];
    uint8_t chr_nib = ((bg & 1u) == 0u) ? (uint8_t)(nba & 0x0Fu) : (uint8_t)((nba >> 4) & 0x0Fu);
    uint16_t chr_base = (uint16_t)(((uint16_t)(chr_nib & 0x07u) << 12) & 0x7FFFu);

    uint16_t base_hofs = (uint16_t)(ppu->bghofs[bg] & 0x03FFu);
    uint16_t base_vofs = (uint16_t)(ppu->bgvofs[bg] & 0x03FFu);

    /* Mode 2 Offset-Per-Tile (BG3 provides column H/V scroll overrides for BG1 & BG2) */
    uint8_t bg3_sc = ppu->regs[0x09u];
    uint16_t bg3_hofs = (uint16_t)(ppu->bghofs[2] & 0x03FFu);
    uint16_t bg3_vofs = (uint16_t)(ppu->bgvofs[2] & 0x03FFu);

    for (unsigned x = 0u; x < 256u; ++x) {
        if (mos_size > 1u && (x % mos_size) != 0u) {
            out_cgram_idx[x] = out_cgram_idx[x - 1u];
            out_prio[x] = out_prio[x - 1u];
            continue;
        }

        uint16_t hofs = base_hofs;
        uint16_t vofs = base_vofs;
        if (opt_mode2 && bg < 2u) {
            unsigned fine_x = hofs & 7u;
            int col = (int)((x + fine_x) >> 3) - 1;
            if (col >= 0) {
                unsigned opt_mx = ((unsigned)col + (bg3_hofs >> 3)) & 63u;
                unsigned opt_my0 = (bg3_vofs >> 3) & 63u;
                unsigned opt_my1 = ((bg3_vofs + 8u) >> 3) & 63u;
                uint16_t h_ent = ppu_lookup_tilemap(ppu->vram, bg3_sc, opt_mx, opt_my0);
                uint16_t v_ent = ppu_lookup_tilemap(ppu->vram, bg3_sc, opt_mx, opt_my1);
                uint16_t bg_bit = (bg == 0u) ? 0x2000u : 0x4000u;
                if ((h_ent & bg_bit) != 0u) {
                    hofs = (uint16_t)((h_ent & 0x03F8u) | fine_x);
                }
                if ((v_ent & bg_bit) != 0u) {
                    vofs = (uint16_t)(v_ent & 0x03FFu);
                }
            }
        }

        unsigned px = (x + hofs) & 0x03FFu;
        /* SNES PPU renders scanline 1..224 -> effective Y is (y + 1 + vofs) */
        unsigned py = (y + 1u + vofs) & 0x03FFu;

        unsigned map_x = tile16 ? (px >> 4) : (px >> 3);
        unsigned map_y = tile16 ? (py >> 4) : (py >> 3);
        uint16_t entry = ppu_lookup_tilemap(ppu->vram, sc, map_x, map_y);

        uint16_t tile_num = (uint16_t)(entry & 0x03FFu);
        uint8_t  pal      = (uint8_t)((entry >> 10) & 0x07u);
        uint8_t  prio     = (uint8_t)((entry >> 13) & 0x01u);
        bool     hflip    = (entry & 0x4000u) != 0u;
        bool     vflip    = (entry & 0x8000u) != 0u;

        unsigned fx, fy;
        if (tile16) {
            unsigned sx = (px >> 3) & 1u;
            unsigned sy = (py >> 3) & 1u;
            if (hflip) sx ^= 1u;
            if (vflip) sy ^= 1u;
            tile_num = (uint16_t)((tile_num + sx + (sy << 4)) & 0x03FFu);
            fx = px & 7u;
            fy = py & 7u;
        } else {
            fx = px & 7u;
            fy = py & 7u;
        }
        if (hflip) fx = 7u - fx;
        if (vflip) fy = 7u - fy;

        uint8_t pix = ppu_fetch_tile_pixel(ppu->vram, chr_base, tile_num, bpp, fx, fy);
        if (pix != 0u) {
            uint8_t cgram_idx;
            if (bpp == 2u) {
                uint8_t base = mode0_pal ? (uint8_t)(bg * 32u) : 0u;
                cgram_idx = (uint8_t)(base + pal * 4u + pix);
            } else if (bpp == 4u) {
                cgram_idx = (uint8_t)(pal * 16u + pix);
            } else {
                cgram_idx = pix;
            }
            out_cgram_idx[x] = cgram_idx;
            out_prio[x] = prio;
        }
    }
}

static inline int32_t ppu_clip_m7(int32_t v) {
    /* SNES Mode 7 13-bit signed clip: preserves sign and low 10 bits */
    return (v & 0x2000) ? (v | ~0x03FF) : (v & 0x03FF);
}

static void ppu_decode_mode7_scanline(const SnesPpuState *ppu, unsigned y,
                                      uint8_t out_cgram_idx[256], uint8_t out_extbg_idx[256], uint8_t out_extbg_prio[256]) {
    memset(out_cgram_idx, 0, 256u);
    memset(out_extbg_idx, 0, 256u);
    memset(out_extbg_prio, 0, 256u);

    uint8_t m7sel = ppu->regs[0x1Au];
    bool hflip = (m7sel & 0x01u) != 0u;
    bool vflip = (m7sel & 0x02u) != 0u;
    uint8_t screen_over = (uint8_t)((m7sel >> 6) & 0x03u);

    uint8_t mosaic = ppu->regs[0x06u];
    unsigned mos_size = ((mosaic & 0x01u) != 0u) ? ((unsigned)((mosaic >> 4) & 0x0Fu) + 1u) : 1u;
    unsigned my = (mos_size > 1u) ? ((y / mos_size) * mos_size) : y;
    int32_t sy = (int32_t)(vflip ? (255u - (my + 1u)) : (my + 1u));

    int32_t a = (int32_t)ppu->m7a;
    int32_t b = (int32_t)ppu->m7b_full;
    int32_t c = (int32_t)ppu->m7c;
    int32_t d = (int32_t)ppu->m7d;
    int32_t cx = (int32_t)ppu->m7x;
    int32_t cy = (int32_t)ppu->m7y;
    int32_t hofs = (int32_t)ppu->m7hofs;
    int32_t vofs = (int32_t)ppu->m7vofs;

    int32_t clip_x = ppu_clip_m7(hofs - cx);
    int32_t clip_y = ppu_clip_m7(vofs - cy);

    int32_t base_x = ((a * clip_x) & ~63) + ((b * clip_y) & ~63) + ((b * sy) & ~63) + (cx << 8);
    int32_t base_y = ((c * clip_x) & ~63) + ((d * clip_y) & ~63) + ((d * sy) & ~63) + (cy << 8);
    bool extbg = (ppu->regs[0x33u] & 0x40u) != 0u;

    for (unsigned x = 0u; x < 256u; ++x) {
        if (mos_size > 1u && (x % mos_size) != 0u) {
            out_cgram_idx[x] = out_cgram_idx[x - 1u];
            out_extbg_idx[x] = out_extbg_idx[x - 1u];
            out_extbg_prio[x] = out_extbg_prio[x - 1u];
            continue;
        }
        int32_t sx = (int32_t)(hflip ? (255u - x) : x);
        int32_t vx = (base_x + a * sx) >> 8;
        int32_t vy = (base_y + c * sx) >> 8;

        bool out_of_bounds = (vx < 0 || vx >= 1024 || vy < 0 || vy >= 1024);
        uint8_t tile_id = 0u;
        if (out_of_bounds) {
            if (screen_over == 2u) {
                continue; /* Transparent outside 1024x1024 map */
            } else if (screen_over == 3u) {
                tile_id = 0u; /* Fill with tile 0 */
            } else {
                uint16_t map_addr = (uint16_t)((((uint32_t)vy & 0x3F8u) << 4) | (((uint32_t)vx & 0x3F8u) >> 3));
                tile_id = (uint8_t)(ppu->vram[map_addr & 0x3FFFu] & 0xFFu);
            }
        } else {
            uint16_t map_addr = (uint16_t)((((uint32_t)vy & 0x3F8u) << 4) | (((uint32_t)vx & 0x3F8u) >> 3));
            tile_id = (uint8_t)(ppu->vram[map_addr & 0x3FFFu] & 0xFFu);
        }

        uint16_t chr_addr = (uint16_t)(((uint16_t)tile_id << 6) | (uint16_t)(((uint32_t)vy & 7u) << 3) | (uint16_t)((uint32_t)vx & 7u));
        uint8_t pix = (uint8_t)((ppu->vram[chr_addr & 0x3FFFu] >> 8) & 0xFFu);
        out_cgram_idx[x] = pix;
        if (extbg) {
            out_extbg_idx[x] = (uint8_t)(pix & 0x7Fu);
            out_extbg_prio[x] = (uint8_t)((pix >> 7) & 1u);
        }
    }
}

static void ppu_decode_sprites_scanline(const SnesPpuState *ppu, unsigned y,
                                        uint8_t out_cgram_idx[256], uint8_t out_prio[256], uint8_t out_pal[256]) {
    static const uint8_t k_spr_w[8][2] = {
        {8u, 16u}, {8u, 32u}, {8u, 64u}, {16u, 32u},
        {16u, 64u}, {32u, 64u}, {16u, 32u}, {16u, 32u}
    };
    static const uint8_t k_spr_h[8][2] = {
        {8u, 16u}, {8u, 32u}, {8u, 64u}, {16u, 32u},
        {16u, 64u}, {32u, 64u}, {32u, 64u}, {32u, 32u}
    };

    memset(out_cgram_idx, 0, 256u);
    memset(out_prio, 0, 256u);
    memset(out_pal, 0, 256u);

    uint8_t obsel = ppu->regs[0x01u];
    uint8_t size_sel = (uint8_t)((obsel >> 5) & 0x07u);
    uint16_t base0 = (uint16_t)(((uint16_t)(obsel & 0x07u) << 13) & 0x7FFFu);
    uint16_t gap   = (uint16_t)((((uint16_t)((obsel >> 3) & 0x03u)) + 1u) << 12);
    uint16_t base1 = (uint16_t)((base0 + gap) & 0x7FFFu);

    /* OAM Priority rotation ($2103 bit 7) or standard sprite 0..127 evaluation */
    unsigned first_spr = ((ppu->regs[0x03u] & 0x80u) != 0u) ? ((ppu->oamadd >> 2) & 0x7Fu) : 0u;
    unsigned spr_on_line = 0u;

    for (unsigned n = 0u; n < 128u; ++n) {
        unsigned idx = (first_spr + n) & 0x7Fu;
        const uint8_t *o = &ppu->oam[idx * 4u];
        uint8_t hi_bits = (uint8_t)((ppu->oam[0x200u + (idx >> 2)] >> ((idx & 3u) * 2u)) & 0x03u);

        int sx = (int)o[0] | ((hi_bits & 1u) ? -256 : 0);
        uint8_t size_bit = (uint8_t)((hi_bits >> 1) & 1u);
        unsigned sw = k_spr_w[size_sel][size_bit];
        unsigned sh = k_spr_h[size_sel][size_bit];

        /* Off-screen X cull (note: x == -256 participates in 32-sprite count on hardware, skip hidden X=-256) */
        if (sx <= -(int)sw || sx >= 256) {
            continue;
        }

        /* SNES OAM Y is offset by +1 scanline (wraps mod 256) */
        unsigned sy_start = (unsigned)(o[1] + 1u) & 0xFFu;
        unsigned rel_y = (y - sy_start) & 0xFFu;
        if (rel_y >= sh) {
            continue;
        }

        if (++spr_on_line > 34u) {
            /* Respect hardware per-scanline sprite limit */
            break;
        }

        uint8_t tile = o[2];
        uint8_t attr = o[3];
        uint16_t chr_base = ((attr & 0x01u) != 0u) ? base1 : base0;
        uint8_t pal  = (uint8_t)((attr >> 1) & 0x07u);
        uint8_t prio = (uint8_t)((attr >> 4) & 0x03u);
        bool hflip   = (attr & 0x40u) != 0u;
        bool vflip   = (attr & 0x80u) != 0u;

        unsigned eff_y = vflip ? (sh - 1u - rel_y) : rel_y;
        unsigned tile_row = (eff_y >> 3);
        unsigned fy = eff_y & 7u;

        for (unsigned dx = 0u; dx < sw; ++dx) {
            int px = sx + (int)dx;
            if (px < 0 || px >= 256) {
                continue;
            }
            /* Lower OAM index wins pixel ownership regardless of priority tier! */
            if (out_cgram_idx[px] != 0u) {
                continue;
            }
            unsigned eff_x = hflip ? (sw - 1u - dx) : dx;
            unsigned tile_col = (eff_x >> 3);
            unsigned fx = eff_x & 7u;

            /* SNES OBJ 16x16 character sheet wraps X within 16-tile row, Y across 16-tile rows */
            uint8_t tx = (uint8_t)(((tile & 0x0Fu) + tile_col) & 0x0Fu);
            uint8_t ty = (uint8_t)((((tile >> 4) & 0x0Fu) + tile_row) & 0x0Fu);
            uint16_t tnum = (uint16_t)((ty << 4) | tx);

            uint8_t pix = ppu_fetch_tile_pixel(ppu->vram, chr_base, tnum, 4u, fx, fy);
            if (pix != 0u) {
                out_cgram_idx[px] = (uint8_t)(128u + pal * 16u + pix);
                out_prio[px] = prio;
                out_pal[px] = pal;
            }
        }
    }
}

typedef struct PixelHit {
    uint16_t bgr555;
    uint8_t  layer;
    uint8_t  obj_pal;
    bool     present;
} PixelHit;

static inline PixelHit ppu_pick_screen_pixel(
    const SnesPpuState *ppu, uint8_t bgmode, uint8_t screen_mask, unsigned x,
    const uint8_t bg_pix[4][256], const uint8_t bg_prio[4][256],
    const uint8_t obj_pix[256], const uint8_t obj_prio[256], const uint8_t obj_pal[256])
{
    PixelHit hit = { ppu->cgram[0], LAYER_BACKDROP, 0u, false };
    uint8_t mode = (uint8_t)(bgmode & 0x07u);

    #define TRY_OBJ(p_req) \
        do { \
            if ((screen_mask & 0x10u) != 0u && obj_pix[x] != 0u && obj_prio[x] == (p_req)) { \
                hit.bgr555 = ppu->cgram[obj_pix[x]]; hit.layer = LAYER_OBJ; hit.obj_pal = obj_pal[x]; hit.present = true; return hit; \
            } \
        } while (0)

    #define TRY_BG(b_idx, p_req) \
        do { \
            if ((screen_mask & (uint8_t)(1u << (b_idx))) != 0u && bg_pix[b_idx][x] != 0u && bg_prio[b_idx][x] == (p_req)) { \
                hit.bgr555 = ppu->cgram[bg_pix[b_idx][x]]; hit.layer = (uint8_t)(b_idx); hit.present = true; return hit; \
            } \
        } while (0)

    if (mode == 0u) {
        TRY_OBJ(3u); TRY_BG(0u, 1u); TRY_BG(1u, 1u);
        TRY_OBJ(2u); TRY_BG(0u, 0u); TRY_BG(1u, 0u);
        TRY_OBJ(1u); TRY_BG(2u, 1u); TRY_BG(3u, 1u);
        TRY_OBJ(0u); TRY_BG(2u, 0u); TRY_BG(3u, 0u);
    } else if (mode == 1u) {
        if ((bgmode & 0x08u) != 0u) {
            TRY_BG(2u, 1u);
        }
        TRY_OBJ(3u); TRY_BG(0u, 1u); TRY_BG(1u, 1u);
        TRY_OBJ(2u); TRY_BG(0u, 0u); TRY_BG(1u, 0u);
        TRY_OBJ(1u);
        if ((bgmode & 0x08u) == 0u) {
            TRY_BG(2u, 1u);
        }
        TRY_OBJ(0u); TRY_BG(2u, 0u);
    } else if (mode == 7u) {
        bool extbg = (ppu->regs[0x33u] & 0x40u) != 0u;
        TRY_OBJ(3u);
        TRY_OBJ(2u);
        if (extbg) TRY_BG(1u, 1u);
        TRY_OBJ(1u);
        if ((screen_mask & 0x01u) != 0u && bg_pix[0][x] != 0u) {
            hit.bgr555 = ppu->cgram[bg_pix[0][x]]; hit.layer = LAYER_BG1; hit.present = true; return hit;
        }
        TRY_OBJ(0u);
        if (extbg) TRY_BG(1u, 0u);
    } else {
        /* Modes 2, 3, 4, 5, 6 */
        TRY_OBJ(3u); TRY_BG(0u, 1u);
        TRY_OBJ(2u); TRY_BG(1u, 1u);
        TRY_OBJ(1u); TRY_BG(0u, 0u);
        TRY_OBJ(0u); TRY_BG(1u, 0u);
    }

    #undef TRY_OBJ
    #undef TRY_BG
    return hit;
}

void snes_ppu_render_frame(SnesCpuContext *ctx, uint32_t *argb_out) {
    if (argb_out == NULL) {
        return;
    }

    HdmaRuntimeChan hdma_rt[8];
    ppu_hdma_init(ctx, hdma_rt);

    uint8_t bg_pix[4][256];
    uint8_t bg_prio[4][256];
    uint8_t obj_pix[256];
    uint8_t obj_prio[256];
    uint8_t obj_pal[256];

    for (unsigned y = 0u; y < SNES_SCREEN_HEIGHT; ++y) {
        /* Fire Mode 7 / scanline V-IRQ ($00:8BE2) if armed at this scanline */
        if ((ctx->cpu_io.nmitimen & 0x20u) != 0u && y == ctx->cpu_io.vtime) {
            uint8_t saved_pb = ctx->pb;
            ctx->pb = 0x00u;
            ctx->cpu_io.timeup_status = 0x80u;
            bank00_func_8be2(ctx);
            ctx->pb = saved_pb;
        }

        const SnesPpuState *ppu = &ctx->ppu;
        uint8_t inidisp = ppu->regs[0x00u];
        uint32_t *row_out = &argb_out[y * SNES_SCREEN_WIDTH];

        if ((inidisp & 0x80u) != 0u || (inidisp & 0x0Fu) == 0u) {
            for (unsigned x = 0u; x < SNES_SCREEN_WIDTH; ++x) {
                row_out[x] = 0xFF000000u;
            }
            ppu_hdma_step_scanline(ctx, hdma_rt);
            continue;
        }

        uint8_t bgmode = ppu->regs[0x05u];
        uint8_t mode = (uint8_t)(bgmode & 0x07u);
        uint8_t tm = ppu->regs[0x2Cu];
        uint8_t ts = ppu->regs[0x2Du];
        uint8_t cgwsel = ppu->regs[0x30u];
        uint8_t cgadsub = ppu->regs[0x31u];
        uint8_t need_layers = (uint8_t)(tm | (((cgwsel & 0x02u) != 0u) ? ts : 0u));

        memset(bg_pix, 0, sizeof(bg_pix));
        memset(bg_prio, 0, sizeof(bg_prio));

        if (mode == 0u) {
            for (unsigned b = 0u; b < 4u; ++b) {
                if ((need_layers & (uint8_t)(1u << b)) != 0u) {
                    ppu_decode_bg_scanline(ppu, b, 2u, y, true, false, bg_pix[b], bg_prio[b]);
                }
            }
        } else if (mode == 1u) {
            if ((need_layers & 0x01u) != 0u) ppu_decode_bg_scanline(ppu, 0u, 4u, y, false, false, bg_pix[0], bg_prio[0]);
            if ((need_layers & 0x02u) != 0u) ppu_decode_bg_scanline(ppu, 1u, 4u, y, false, false, bg_pix[1], bg_prio[1]);
            if ((need_layers & 0x04u) != 0u) ppu_decode_bg_scanline(ppu, 2u, 2u, y, false, false, bg_pix[2], bg_prio[2]);
        } else if (mode == 2u) {
            if ((need_layers & 0x01u) != 0u) ppu_decode_bg_scanline(ppu, 0u, 4u, y, false, true, bg_pix[0], bg_prio[0]);
            if ((need_layers & 0x02u) != 0u) ppu_decode_bg_scanline(ppu, 1u, 4u, y, false, true, bg_pix[1], bg_prio[1]);
        } else if (mode == 3u) {
            if ((need_layers & 0x01u) != 0u) ppu_decode_bg_scanline(ppu, 0u, 8u, y, false, false, bg_pix[0], bg_prio[0]);
            if ((need_layers & 0x02u) != 0u) ppu_decode_bg_scanline(ppu, 1u, 4u, y, false, false, bg_pix[1], bg_prio[1]);
        } else if (mode == 4u) {
            if ((need_layers & 0x01u) != 0u) ppu_decode_bg_scanline(ppu, 0u, 8u, y, false, false, bg_pix[0], bg_prio[0]);
            if ((need_layers & 0x02u) != 0u) ppu_decode_bg_scanline(ppu, 1u, 2u, y, false, false, bg_pix[1], bg_prio[1]);
        } else if (mode == 7u) {
            if ((need_layers & 0x03u) != 0u) {
                ppu_decode_mode7_scanline(ppu, y, bg_pix[0], bg_pix[1], bg_prio[1]);
            }
        } else {
            if ((need_layers & 0x01u) != 0u) ppu_decode_bg_scanline(ppu, 0u, 4u, y, false, false, bg_pix[0], bg_prio[0]);
            if ((need_layers & 0x02u) != 0u) ppu_decode_bg_scanline(ppu, 1u, 2u, y, false, false, bg_pix[1], bg_prio[1]);
        }

        if ((need_layers & 0x10u) != 0u) {
            ppu_decode_sprites_scanline(ppu, y, obj_pix, obj_prio, obj_pal);
        } else {
            memset(obj_pix, 0, sizeof(obj_pix));
            memset(obj_prio, 0, sizeof(obj_prio));
            memset(obj_pal, 0, sizeof(obj_pal));
        }

        uint8_t w12sel  = ppu->regs[0x23u];
        uint8_t w34sel  = ppu->regs[0x24u];
        uint8_t wobjsel = ppu->regs[0x25u];
        uint8_t wh0     = ppu->regs[0x26u];
        uint8_t wh1     = ppu->regs[0x27u];
        uint8_t wh2     = ppu->regs[0x28u];
        uint8_t wh3     = ppu->regs[0x29u];
        uint8_t wbglog  = ppu->regs[0x2Au];
        uint8_t wobjlog = ppu->regs[0x2Bu];
        uint8_t tmw     = ppu->regs[0x2Eu];
        uint8_t tsw     = ppu->regs[0x2Fu];
        uint32_t bright = (uint32_t)(inidisp & 0x0Fu);

        for (unsigned x = 0u; x < SNES_SCREEN_WIDTH; ++x) {
            uint8_t main_mask = tm;
            uint8_t sub_mask  = ts;

            if ((tmw | tsw) != 0u) {
                uint8_t wmask = 0u;
                if (ppu_eval_window((uint8_t)x, (uint8_t)(w12sel & 0x0Fu), (uint8_t)(wbglog & 0x03u), wh0, wh1, wh2, wh3)) wmask |= 0x01u;
                if (ppu_eval_window((uint8_t)x, (uint8_t)((w12sel >> 4) & 0x0Fu), (uint8_t)((wbglog >> 2) & 0x03u), wh0, wh1, wh2, wh3)) wmask |= 0x02u;
                if (ppu_eval_window((uint8_t)x, (uint8_t)(w34sel & 0x0Fu), (uint8_t)((wbglog >> 4) & 0x03u), wh0, wh1, wh2, wh3)) wmask |= 0x04u;
                if (ppu_eval_window((uint8_t)x, (uint8_t)((w34sel >> 4) & 0x0Fu), (uint8_t)((wbglog >> 6) & 0x03u), wh0, wh1, wh2, wh3)) wmask |= 0x08u;
                if (ppu_eval_window((uint8_t)x, (uint8_t)(wobjsel & 0x0Fu), (uint8_t)(wobjlog & 0x03u), wh0, wh1, wh2, wh3)) wmask |= 0x10u;

                main_mask &= (uint8_t)(~(tmw & wmask));
                sub_mask  &= (uint8_t)(~(tsw & wmask));
            }

            PixelHit main_hit = ppu_pick_screen_pixel(ppu, bgmode, main_mask, x, bg_pix, bg_prio, obj_pix, obj_prio, obj_pal);

            /* Evaluate Color Window ($2130 bits 4..7) */
            bool col_win_in = ppu_eval_window((uint8_t)x, (uint8_t)((wobjsel >> 4) & 0x0Fu), (uint8_t)((wobjlog >> 2) & 0x03u), wh0, wh1, wh2, wh3);
            bool clip_black = false;
            switch ((cgwsel >> 6) & 0x03u) {
                case 0u: clip_black = false; break;
                case 1u: clip_black = !col_win_in; break;
                case 2u: clip_black = col_win_in; break;
                default: clip_black = true; break;
            }
            bool allow_math = true;
            switch ((cgwsel >> 4) & 0x03u) {
                case 0u: allow_math = true; break;
                case 1u: allow_math = col_win_in; break;
                case 2u: allow_math = !col_win_in; break;
                default: allow_math = false; break;
            }

            int mr = clip_black ? 0 : (int)(main_hit.bgr555 & 0x1Fu);
            int mg = clip_black ? 0 : (int)((main_hit.bgr555 >> 5) & 0x1Fu);
            int mb = clip_black ? 0 : (int)((main_hit.bgr555 >> 10) & 0x1Fu);

            /* Apply CGADSUB ($2131) color addition/subtraction (OBJ palettes 0..3 never participate) */
            bool layer_math = ((cgadsub & (uint8_t)(1u << main_hit.layer)) != 0u) &&
                              (main_hit.layer != LAYER_OBJ || main_hit.obj_pal >= 4u);

            if (allow_math && layer_math) {
                uint16_t sub_col = ppu->fixed_color;
                bool sub_backdrop_only = false;
                if ((cgwsel & 0x02u) != 0u) {
                    PixelHit sub_hit = ppu_pick_screen_pixel(ppu, bgmode, sub_mask, x, bg_pix, bg_prio, obj_pix, obj_prio, obj_pal);
                    if (sub_hit.present) {
                        sub_col = sub_hit.bgr555;
                    } else {
                        sub_backdrop_only = true;
                    }
                }
                int sr = (int)(sub_col & 0x1Fu);
                int sg = (int)((sub_col >> 5) & 0x1Fu);
                int sb = (int)((sub_col >> 10) & 0x1Fu);

                if ((cgadsub & 0x80u) != 0u) {
                    mr -= sr; mg -= sg; mb -= sb;
                } else {
                    mr += sr; mg += sg; mb += sb;
                }
                if ((cgadsub & 0x40u) != 0u && !clip_black && !sub_backdrop_only) {
                    mr >>= 1; mg >>= 1; mb >>= 1;
                }
                if (mr < 0) mr = 0; else if (mr > 31) mr = 31;
                if (mg < 0) mg = 0; else if (mg > 31) mg = 31;
                if (mb < 0) mb = 0; else if (mb > 31) mb = 31;
            }

            /* Expand 5-bit BGR555 -> 8-bit RGB888 scaled by master brightness (0..15) */
            uint32_t r8 = (((uint32_t)mr << 3) | ((uint32_t)mr >> 2)) * bright / 15u;
            uint32_t g8 = (((uint32_t)mg << 3) | ((uint32_t)mg >> 2)) * bright / 15u;
            uint32_t b8 = (((uint32_t)mb << 3) | ((uint32_t)mb >> 2)) * bright / 15u;
            row_out[x] = 0xFF000000u | (r8 << 16) | (g8 << 8) | b8;
        }

        ppu_hdma_step_scanline(ctx, hdma_rt);
    }
}

/* Thread-local/static render hook state used during snes_step_frame */
static uint32_t *g_active_render_buf = NULL;
static bool      g_frame_rendered    = false;

static uint8_t ppu_frame_read_cpu_io_hook(SnesCpuContext *ctx, uint16_t reg) {
    /*
     * In all three Contra III NMI handlers ($00:821D, $00:8367, $00:842B),
     * Input_ReadJoypads ($00:8B19) polls $4212 (CPUIO_HVBJOY) immediately after
     * 100% of VBlank DMA, scroll latches, PPU shadows, HDMA setup, and INIDISP
     * unblanking are finished, and right before MainModule_Dispatch ($00:A377)
     * mutates next-frame WRAM buffers. Rendering active display here matches
     * real hardware scanout timing cleanly.
     */
    if (reg == CPUIO_HVBJOY && !g_frame_rendered && g_active_render_buf != NULL) {
        g_frame_rendered = true;
        SnesMmioReadHook saved_hook = ctx->on_read_cpu_io;
        ctx->on_read_cpu_io = NULL;
        snes_ppu_render_frame(ctx, g_active_render_buf);
        ctx->on_read_cpu_io = saved_hook;
    }
    SnesMmioReadHook saved = ctx->on_read_cpu_io;
    ctx->on_read_cpu_io = NULL;
    uint8_t val = snes_hw_read_cpu_io(ctx, reg);
    ctx->on_read_cpu_io = saved;
    return val;
}

void snes_step_frame(SnesCpuContext *ctx, uint16_t pad1, uint16_t pad2, uint32_t *argb_out) {
    ctx->cpu_io.joy_pads[0] = pad1;
    ctx->cpu_io.joy_pads[1] = pad2;
    ctx->cpu_io.rdnmi_status = 0x82u;

    /*
     * Shared timeline: ctx->nmi_wall is when this frame's NMI fires. If the CPU is still
     * running last frame's logic at that moment, Konami's re-entrancy guard ($00:81C6,
     * $003C != 0) returns at once: a lag frame (no game logic, joypad not read, the PPU
     * scans out unchanged state). Without the lag model the overrun is absorbed instead:
     * the frame grid slides to "now", so video never drops frames but the APU still
     * experiences the real elapsed time.
     */
    uint64_t wall = snes_wall_cycles(ctx);
    bool lagged = false;
    if (wall > ctx->nmi_wall) {
        if (ctx->lag_budget != 0u) {
            lagged = true;
        } else {
            ctx->spc_at_nmi = snes_spc_time_now(ctx);
            ctx->nmi_wall = wall;
        }
    }
    uint64_t over = (wall > ctx->nmi_wall) ? wall - ctx->nmi_wall : 0u;
    ctx->lag_debt = (over > 0xFFFFFFFFull) ? 0xFFFFFFFFu : (uint32_t)over;
    if (wall < ctx->nmi_wall) {
        ctx->idle_cycles += ctx->nmi_wall - wall; /* background loop idles until VBlank */
    }
    snes_apu_sync(ctx);
    ctx->last_frame_lagged = lagged;

    SnesMmioReadHook prev_hook = ctx->on_read_cpu_io;
    g_active_render_buf = argb_out;
    g_frame_rendered = false;
    if (argb_out != NULL) {
        ctx->on_read_cpu_io = ppu_frame_read_cpu_io_hook;
    }

    if (lagged) {
        ctx->lag_frames++;
    } else if ((ctx->cpu_io.nmitimen & 0x80u) != 0u) {
        uint8_t saved_pb = ctx->pb;
        uint64_t start = ctx->cycles;
        uint32_t polls_start = ctx->apu.read_count;
        ctx->pb = 0x00u;
        bank00_func_819b(ctx);
        ctx->pb = saved_pb;
        uint64_t used = ctx->cycles - start;
        ctx->last_nmi_cycles = (used > 0xFFFFFFFFull) ? 0xFFFFFFFFu : (uint32_t)used;
        ctx->last_nmi_apu_polls = ctx->apu.read_count - polls_start;
    }

    ctx->on_read_cpu_io = prev_hook;
    if (argb_out != NULL && !g_frame_rendered) {
        snes_ppu_render_frame(ctx, argb_out);
    }
    g_active_render_buf = NULL;

    /* Next NMI; the SPC700 runs on to it (or to "now" if the CPU has already passed it) */
    snes_timeline_next_frame(ctx);

    if (snes_mainthread_active(ctx)) {
        /* The main thread (background loop / boot code) owns the CPU until the next NMI */
        snes_mainthread_run_until(ctx, ctx->nmi_wall);
    } else {
        /* Legacy model: run background decompression / SPC700 streaming worker slices
           (none while a lag frame is still running game logic) */
        for (unsigned slice = 0u; slice < (lagged ? 0u : 8u); ++slice) {
            bank00_func_995a(ctx);
            if (snes_bus_read16(ctx, 0x000052u) != 0x00FFu) {
                break;
            }
        }
    }
    if (ctx->apu_core != NULL) {
        uint64_t target = snes_spc_time_now(ctx);
        snes_apu_run_to(ctx->apu_core, (target > ctx->spc_at_nmi) ? target : ctx->spc_at_nmi);
    }
}

bool snes_write_bmp(const char *path, const uint32_t *argb_buf, int width, int height) {
    if (path == NULL || argb_buf == NULL || width <= 0 || height <= 0) {
        return false;
    }
    FILE *fp = fopen(path, "wb");
    if (fp == NULL) {
        return false;
    }

    uint32_t row_stride = (uint32_t)((width * 3 + 3) & ~3);
    uint32_t img_bytes  = row_stride * (uint32_t)height;
    uint32_t file_size  = 54u + img_bytes;

    uint8_t hdr[54];
    memset(hdr, 0, sizeof(hdr));
    hdr[0] = 'B';
    hdr[1] = 'M';
    hdr[2] = (uint8_t)(file_size & 0xFFu);
    hdr[3] = (uint8_t)((file_size >> 8) & 0xFFu);
    hdr[4] = (uint8_t)((file_size >> 16) & 0xFFu);
    hdr[5] = (uint8_t)((file_size >> 24) & 0xFFu);
    hdr[10] = 54u;
    hdr[14] = 40u;
    hdr[18] = (uint8_t)(width & 0xFF);
    hdr[19] = (uint8_t)((width >> 8) & 0xFF);
    hdr[22] = (uint8_t)(height & 0xFF);
    hdr[23] = (uint8_t)((height >> 8) & 0xFF);
    hdr[26] = 1u;
    hdr[28] = 24u;
    hdr[34] = (uint8_t)(img_bytes & 0xFFu);
    hdr[35] = (uint8_t)((img_bytes >> 8) & 0xFFu);
    hdr[36] = (uint8_t)((img_bytes >> 16) & 0xFFu);
    hdr[37] = (uint8_t)((img_bytes >> 24) & 0xFFu);

    if (fwrite(hdr, 1u, sizeof(hdr), fp) != sizeof(hdr)) {
        fclose(fp);
        return false;
    }

    uint8_t *row_buf = (uint8_t *)malloc(row_stride);
    if (row_buf == NULL) {
        fclose(fp);
        return false;
    }
    for (int y = height - 1; y >= 0; --y) {
        const uint32_t *src = &argb_buf[y * width];
        memset(row_buf, 0, row_stride);
        for (int x = 0; x < width; ++x) {
            uint32_t px = src[x];
            row_buf[x * 3 + 0] = (uint8_t)(px & 0xFFu);         /* Blue */
            row_buf[x * 3 + 1] = (uint8_t)((px >> 8) & 0xFFu);  /* Green */
            row_buf[x * 3 + 2] = (uint8_t)((px >> 16) & 0xFFu); /* Red */
        }
        if (fwrite(row_buf, 1u, row_stride, fp) != row_stride) {
            free(row_buf);
            fclose(fp);
            return false;
        }
    }

    free(row_buf);
    fclose(fp);
    return true;
}
