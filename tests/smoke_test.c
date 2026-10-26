/*
 * Native C11 Verification & Co-Simulation Smoke Test Suite
 * Executes statically recompiled Contra III 65816 functions against the real ROM.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contra3_recomp.h"
#include "snes_ppu.h"
#include "snes_apu.h"
#include "snes_movie.h"

#define TEST_ASSERT(cond, msg)                                                      \
    do {                                                                            \
        if (!(cond)) {                                                              \
            fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, (msg));       \
            return 1;                                                               \
        }                                                                           \
    } while (0)

static uint8_t *load_rom_image(const char *path, size_t *out_size) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return NULL;
    }
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return NULL;
    }
    long sz = ftell(fp);
    if (sz <= 0 || fseek(fp, 0, SEEK_SET) != 0) {
        fclose(fp);
        return NULL;
    }
    uint8_t *buf = (uint8_t *)malloc((size_t)sz);
    if (buf == NULL) {
        fclose(fp);
        return NULL;
    }
    if (fread(buf, 1u, (size_t)sz, fp) != (size_t)sz) {
        free(buf);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    *out_size = (size_t)sz;
    return buf;
}

/* Dynamic-Huffman raw DEFLATE of a 60-frame BizHawk input log body (zlib level 9, wbits -15) */
static const uint8_t k_bk2_log_deflate[] = {
    0x9D, 0x54, 0x41, 0x8E, 0xC3, 0x20, 0x10, 0xBB, 0xF7, 0x31, 0xFE, 0x43, 0x10, 0x47, 0x4E,
    0x44, 0x91, 0x9A, 0x3F, 0xE4, 0x44, 0xAE, 0x3C, 0x7E, 0x07, 0x86, 0xA4, 0xAD, 0x8A, 0x5D,
    0x69, 0xE7, 0x96, 0x58, 0x33, 0x60, 0x8F, 0x4D, 0x05, 0x2A, 0x22, 0x32, 0xB0, 0xE3, 0x89,
    0xA3, 0x54, 0xBC, 0x55, 0x7D, 0xD4, 0x06, 0x23, 0xE5, 0xFE, 0xB9, 0x60, 0x06, 0x6F, 0x11,
    0x38, 0x3B, 0x4C, 0xBA, 0xAD, 0x56, 0xD8, 0xF0, 0xEB, 0xE3, 0x13, 0xBE, 0xCF, 0x9E, 0xC3,
    0x77, 0x1D, 0x04, 0xF6, 0xEE, 0x65, 0x0A, 0xDF, 0x57, 0xE3, 0xDD, 0xE7, 0xBA, 0xB7, 0x9B,
    0x8B, 0xB3, 0x03, 0xD8, 0xD5, 0x12, 0xAC, 0x3B, 0x10, 0xDE, 0x9B, 0xC1, 0xE8, 0xC3, 0x59,
    0x77, 0x36, 0x59, 0x82, 0xF1, 0x2E, 0x73, 0x59, 0xA0, 0x64, 0x89, 0xFE, 0x45, 0xBA, 0xB7,
    0x98, 0x2E, 0xD5, 0xD8, 0xC6, 0x46, 0x37, 0x23, 0x36, 0xAA, 0x68, 0x51, 0x8B, 0x50, 0x8D,
    0xAD, 0xE4, 0x35, 0x9C, 0xD9, 0xE1, 0x34, 0xDE, 0x52, 0x73, 0xDE, 0xFD, 0xAA, 0xC2, 0xF6,
    0x8D, 0x3D, 0x10, 0xDE, 0xDB, 0x30, 0x13, 0x28, 0xDC, 0x78, 0x07, 0x6A, 0xA6, 0x46, 0xAC,
    0x0D, 0x3F, 0xF8, 0xD9, 0xA6, 0xF9, 0x22, 0x86, 0xF3, 0xB3, 0x63, 0x77, 0x6A, 0x00, 0x75,
    0x4B, 0x3E, 0x57, 0x6A, 0x87, 0x8F, 0x3F, 0x73, 0x2B, 0x2A, 0x59, 0xCC, 0x4C, 0x7E, 0x35,
    0xC5, 0x5B, 0x44, 0x30, 0xEB, 0x7D, 0xFF, 0x48, 0xA8, 0x3B, 0x95, 0x99, 0x49, 0x27, 0xD4,
    0x65, 0x09, 0xCA, 0x2D, 0x5C, 0xD4, 0x9E, 0x92, 0x95, 0x3E, 0x7B, 0x23, 0x25, 0x6D, 0xDF,
    0xDC, 0x4C, 0x3C, 0xA1, 0x6E, 0x26, 0xB1, 0xB1, 0xAC, 0x02, 0xEC, 0x31, 0xD0, 0xC4, 0x78,
    0x42, 0xE3, 0x65, 0x64, 0xBA, 0x6F, 0xE7, 0x4D, 0xBA, 0x93, 0x5A, 0xE8, 0xD8, 0x77, 0x78,
    0xB2, 0x67, 0xCF, 0x63, 0x00, 0x01, 0xAB, 0x7C, 0x27, 0xA8, 0x10, 0xF9, 0xC6, 0xC4, 0x83,
    0x9D, 0x87, 0x5B, 0xCA, 0x7F, 0xE0, 0xF1, 0x32, 0x7D, 0x5B, 0xF1, 0x0F
};

static uint32_t crc32_bytes(const uint8_t *p, size_t n) {
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0u; i < n; ++i) {
        crc ^= p[i];
        for (unsigned k = 0u; k < 8u; ++k) {
            crc = (crc >> 1) ^ (0xEDB88320u & (0u - (crc & 1u)));
        }
    }
    return ~crc;
}

int main(int argc, char **argv) {
    const char *rom_path = (argc > 1)
        ? argv[1]
        : "Z:/REA/ROM/Contra III - The Alien Wars (USA).sfc";

    size_t rom_size = 0u;
    uint8_t *rom = load_rom_image(rom_path, &rom_size);
    TEST_ASSERT(rom != NULL && rom_size == 1048576u, "Loaded 1 MB Contra III (USA) LoROM image");

    static SnesCpuContext ctx;
    snes_context_init(&ctx, rom, rom_size);

    /* 1. Verify Bank Manifest Integrity */
    uint32_t total_manifest_funcs = 0u;
    uint32_t total_manifest_insns = 0u;
    for (unsigned b = 0u; b < 32u; ++b) {
        TEST_ASSERT(g_contra3_bank_manifest[b].bank == b, "Bank index matches manifest");
        TEST_ASSERT(g_contra3_bank_manifest[b].byte_length == 32768u, "32 KB LoROM bank size");
        total_manifest_funcs += g_contra3_bank_manifest[b].function_count;
        total_manifest_insns += g_contra3_bank_manifest[b].decoded_insn_count;
    }
    TEST_ASSERT(total_manifest_funcs >= 2280u, "All reachable + SDK functions registered in manifest");
    TEST_ASSERT(total_manifest_insns >= 56900u, "All unique instructions accounted for");
    printf("[PASS] Bank manifest verified: %u functions, %u instructions across 32 banks.\n",
           total_manifest_funcs, total_manifest_insns);

    /* 2. Execute RESET ($00:8000) through cold-boot init + 3 background worker iterations */
    ctx.max_main_loop_iters = 3u;
    bank00_func_8000(&ctx);

    TEST_ASSERT(ctx.stop_main_loop, "RESET entered background worker loop at $00:8166 cleanly");
    TEST_ASSERT(ctx.flag_e == 0u, "CPU switched to 65816 native mode (CLC; XCE)");
    /* Note: At $00:80D3 PHB pushes $05 to $00:1DFF, then MVN $00,$00 ($00:80E3) zeroes $0000-$1DFF, so PLB pulls $00! */
    TEST_ASSERT(ctx.db == 0x00u, "Post-MVN stack clear PLB sets background loop DB=$00 ($05 set per-NMI at $00:81A9)");
    TEST_ASSERT(ctx.dp == 0x0000u, "Direct Page DP=$0000 established");

    /* Verify KONAMI cold-boot signature written to $00:1E0A-$1E10 */
    TEST_ASSERT(snes_bus_read16(&ctx, 0x001E0Au) == 0x4A3Eu, "KONAMI signature word 0 ($4A3E)");
    TEST_ASSERT(snes_bus_read16(&ctx, 0x001E0Cu) == 0x4E49u, "KONAMI signature word 1 ($4E49)");
    TEST_ASSERT(snes_bus_read16(&ctx, 0x001E0Eu) == 0x3C4Cu, "KONAMI signature word 2 ($3C4C)");
    TEST_ASSERT(snes_bus_read16(&ctx, 0x001E10u) == 0x0400u, "KONAMI signature word 3 ($0400)");
    TEST_ASSERT(snes_bus_read16(&ctx, 0x001E3Cu) == 0x0003u, "Default starting lives = 3 ($1E3C)");
    TEST_ASSERT(snes_bus_read16(&ctx, 0x001E82u) == 0x0081u, "NMITIMEN shadow $1E82 = $0081");
    TEST_ASSERT(ctx.cpu_io.nmitimen == 0x81u, "Hardware $4200 NMITIMEN = $81 via snes_write_cpu_io");
    TEST_ASSERT(ctx.ppu.write_count > 50u, "PPU hardware registers initialized via snes_write_ppu");
    printf("[PASS] RESET ($00:8000) cold-boot passed: PPU writes=%u, APU writes=%u, DMA bursts=%u.\n",
           ctx.ppu.write_count, ctx.apu.write_count, ctx.cpu_io.dma_transfers);

    /* 3. Verify PRNG ($00:816C) determinism */
    snes_bus_write16(&ctx, 0x00003Au, 0x1234u);
    snes_bus_write16(&ctx, 0x0000E8u, 0x0000u);
    snes_bus_write16(&ctx, 0x0000EAu, 0x5678u);
    bank00_func_816c(&ctx);
    uint16_t r0 = snes_bus_read16(&ctx, 0x0000E8u);
    uint16_t r1 = snes_bus_read16(&ctx, 0x0000EAu);
    bank00_func_816c(&ctx);
    uint16_t r2 = snes_bus_read16(&ctx, 0x0000E8u);
    TEST_ASSERT(r0 != 0u && r2 != r0, "PRNG ($00:816C) advances 32-bit state ($E8:$EA)");
    printf("[PASS] PRNG ($00:816C) state verified: ($%04X:%04X) -> $%04X.\n", r1, r0, r2);

    /* 4. Verify 16x16 Hardware Multiply ($00:8ADB) using $4202/$4203 -> $4216 */
    snes_rep(&ctx, 0x30u);
    snes_bus_write16(&ctx, 0x000000u, 300u);  /* Multiplicand in $00 */
    snes_bus_write16(&ctx, 0x000002u, 45u);   /* Multiplier in $02 */
    bank00_func_8adb(&ctx);
    uint32_t prod = (uint32_t)snes_bus_read16(&ctx, 0x000004u) |
                    ((uint32_t)snes_bus_read16(&ctx, 0x000006u) << 16);
    TEST_ASSERT(prod == 300u * 45u, "Math_Mul16x16 ($00:8ADB) computed exact product 13500");
    printf("[PASS] Math_Mul16x16 ($00:8ADB) verified: 300 * 45 = %u.\n", prod);

    /* 5. Verify Bank $1D Math_Atan2_256 ($1D:ABA0) cross-bank execution */
    snes_rep(&ctx, 0x30u);
    snes_bus_write16(&ctx, 0x000008u, 0x0010u);
    snes_bus_write16(&ctx, 0x00000Au, 0x0010u);
    TEST_ASSERT(contra3_dispatch_call(&ctx, 0x1DABA0u), "Dispatched $1D:ABA0 via contra3_dispatch_call");
    TEST_ASSERT(ctx.flag_m == 0u && ctx.flag_x == 0u, "$1D:ABA0 restored [M=16-bit, X=16-bit] via REP #$30");
    printf("[PASS] Cross-bank $1D:ABA0 (Math_Atan2_256) returned angle=0x%04X.\n", ctx.a);

    /* 6. Execute 60 full 60Hz VBlank frames (NMI_Handler $00:819B + Background Worker $00:995A) */
    uint32_t prev_dma = ctx.cpu_io.dma_transfers;
    for (unsigned frame = 0u; frame < 60u; ++frame) {
        ctx.cpu_io.rdnmi_status = 0x82u;
        bank00_func_819b(&ctx);
        bank00_func_995a(&ctx);
    }
    unsigned nonzero_vram = 0u;
    for (unsigned i = 0u; i < 32768u; ++i) {
        if (ctx.ppu.vram[i] != 0u) {
            nonzero_vram++;
        }
    }
    unsigned nonzero_cgram = 0u;
    for (unsigned i = 0u; i < 256u; ++i) {
        if (ctx.ppu.cgram[i] != 0u) {
            nonzero_cgram++;
        }
    }
    TEST_ASSERT(ctx.cpu_io.dma_transfers > prev_dma + 50u, "NMI_Handler executed 60 frames of VBlank DMA");
    TEST_ASSERT(nonzero_vram > 100u, "Konami LZSS + DMA populated VRAM tiles during startup sequence");
    TEST_ASSERT(nonzero_cgram > 0u, "Palette engine + CGRAM DMA populated non-zero colors");
    TEST_ASSERT(ctx.unresolved_jumps == 0u, "Zero unresolved indirect jumps across 60 live frames");
    printf("[PASS] 60 VBlank frames executed cleanly: DMA bursts=%u, VRAM words=%u, CGRAM colors=%u, unresolved=%u.\n",
           ctx.cpu_io.dma_transfers, nonzero_vram, nonzero_cgram, ctx.unresolved_jumps);

    /* 7. Verify S-PPU1/S-PPU2 Scanline Software Rasterizer during Konami Logo display */
    static uint32_t framebuf[SNES_SCREEN_WIDTH * SNES_SCREEN_HEIGHT];
    for (unsigned frame = 0u; frame < 45u; ++frame) {
        snes_step_frame(&ctx, 0u, 0u, (frame == 44u) ? framebuf : NULL);
    }
    unsigned nonblack_pixels = 0u;
    for (unsigned px = 0u; px < SNES_SCREEN_WIDTH * SNES_SCREEN_HEIGHT; ++px) {
        if ((framebuf[px] & 0x00FFFFFFu) != 0u) {
            nonblack_pixels++;
        }
    }
    TEST_ASSERT(nonblack_pixels > 500u, "S-PPU scanline rasterizer rendered visible Konami logo pixels");
    TEST_ASSERT(ctx.unresolved_jumps == 0u, "Zero unresolved indirect jumps after 105 frames + PPU scanout");
    printf("[PASS] S-PPU scanline rasterizer verified: %u/%u non-black pixels rendered cleanly.\n",
           nonblack_pixels, (unsigned)(SNES_SCREEN_WIDTH * SNES_SCREEN_HEIGHT));

    /* 8. Verify Sony S-SMP (SPC700 + 64-byte IPL ROM) & S-DSP 32 kHz Stereo Audio Synthesis */
    static SnesCpuContext apu_ctx;
    static SnesApu        apu_core;
    static int16_t        pcm_chunk[2048 * 2];

    snes_context_init(&apu_ctx, rom, rom_size);
    snes_apu_init(&apu_core);
    TEST_ASSERT(apu_core.apu_to_cpu[0] == 0xAAu && apu_core.apu_to_cpu[1] == 0xBBu,
                "64-byte Sony IPL ROM booted and posted $BBAA ready signature");
    snes_apu_attach(&apu_ctx, &apu_core);

    apu_ctx.max_main_loop_iters = 1u;
    bank00_func_8000(&apu_ctx);
    TEST_ASSERT(apu_core.ram[0x0200] == 0x5Fu,
                "Real IPL ROM uploaded Konami SPC700 audio driver to ARAM $0200");
    TEST_ASSERT(apu_core.pc < 0xFFC0u && !apu_core.halted,
                "Konami SPC700 audio driver booted and is actively executing");

    size_t   total_pcm_frames = 0u;
    unsigned nonzero_samples  = 0u;
    int32_t  peak_abs         = 0;
    for (unsigned frame = 0u; frame < 220u; ++frame) {
        snes_step_frame(&apu_ctx, 0u, 0u, NULL);
        size_t n = snes_apu_pop_samples(&apu_core, pcm_chunk, 2048u);
        total_pcm_frames += n;
        for (size_t i = 0u; i < n * 2u; ++i) {
            int32_t s = (int32_t)pcm_chunk[i];
            if (s != 0) {
                nonzero_samples++;
                int32_t a = (s < 0) ? -s : s;
                if (a > peak_abs) {
                    peak_abs = a;
                }
            }
        }
    }
    TEST_ASSERT(total_pcm_frames >= 115000u, "S-DSP synthesized 32 kHz stereo stream across 220 frames");
    TEST_ASSERT(apu_core.kon_events > 0u, "Konami SPC700 driver keyed on S-DSP BRR voices");
    TEST_ASSERT(nonzero_samples > 1000u && peak_abs > 100,
                "S-DSP BRR + Gaussian + Envelope pipeline produced audible stereo waveform");
    TEST_ASSERT(apu_ctx.unresolved_jumps == 0u, "Zero unresolved jumps with live SPC700 + S-DSP attached");
    printf("[PASS] S-SMP/S-DSP audio verified: %zu stereo frames, KON=%u, active_samples=%u, peak=%d.\n",
           total_pcm_frames, apu_core.kon_events, nonzero_samples, (int)peak_abs);

    /* 9. Input movie decoding: raw DEFLATE, BizHawk input log mapping, native .c3m */
    {
        static uint8_t log_buf[8 + 4096];
        memcpy(log_buf, "[Input]\n", 8u);
        size_t n = snes_inflate(k_bk2_log_deflate, sizeof(k_bk2_log_deflate), log_buf + 8u, 4096u);
        TEST_ASSERT(n == 1860u, "Dynamic-Huffman DEFLATE stream inflated to exact length");
        TEST_ASSERT(crc32_bytes(log_buf + 8u, n) == 0x8E784788u, "Inflated input log CRC32 matches zlib reference");

        SnesMovie mv;
        char err[160];
        TEST_ASSERT(snes_movie_load_memory(log_buf, 8u + n, &mv, err, sizeof(err)), "BizHawk input log parsed");
        uint32_t sum = 0u;
        for (uint32_t f = 0u; f < mv.frame_count; ++f) {
            sum += snes_movie_pad(&mv, f, 0u);
        }
        TEST_ASSERT(mv.format == SNES_MOVIE_BK2_LOG && mv.frame_count == 60u, "60 input log frames decoded");
        TEST_ASSERT(snes_movie_pad(&mv, 0u, 0u) == 0x4570u && snes_movie_pad(&mv, 59u, 0u) == 0x61A0u &&
                    sum == 1265472u, "UDLRsSYBXAlr columns mapped to $4218 joypad bits");
        snes_movie_free(&mv);

        static const char c3m[] = "C3M 1\n# author tester\n1000 0000\n8100 0040 1\n0000 FFF0\n";
        TEST_ASSERT(snes_movie_load_memory((const uint8_t *)c3m, sizeof(c3m) - 1u, &mv, err, sizeof(err)),
                    "Native .c3m movie parsed");
        TEST_ASSERT(mv.frame_count == 3u && snes_movie_pad(&mv, 1u, 0u) == 0x8100u &&
                    snes_movie_pad(&mv, 1u, 1u) == 0x0040u && snes_movie_flags(&mv, 1u) == SNES_MOVIE_FLAG_RESET &&
                    snes_movie_pad(&mv, 2u, 1u) == 0xFFF0u && strcmp(mv.author, "tester") == 0,
                    ".c3m pads, reset flag and author decoded");
        snes_movie_free(&mv);
        printf("[PASS] Input movie decoders verified: DEFLATE %zu -> %zu bytes, 60-frame BK2 log, C3M reset flag.\n",
               sizeof(k_bk2_log_deflate), n);
    }

    free(rom);
    printf("ALL CONTRA III STATIC RECOMPILATION SMOKE TESTS PASSED!\n");
    return 0;
}
