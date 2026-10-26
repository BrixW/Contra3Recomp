/*
 * Contra III: The Alien Wars (USA) - Versioned Save States
 */

#include "snes_savestate.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define HEADER_SIZE 40u

static const uint8_t k_magic[8] = { 'C', '3', 'S', 'A', 'V', 'E', '\r', '\n' };

static uint32_t fnv1a32(uint32_t h, const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    for (size_t i = 0u; i < len; ++i) {
        h ^= p[i];
        h *= 16777619u;
    }
    return h;
}

/* Fingerprint of the field layout: catches reordered or resized fields even when sizeof() is unchanged */
static uint32_t layout_fingerprint(void) {
    const uint32_t offsets[] = {
        (uint32_t)offsetof(SnesCpuContext, a),       (uint32_t)offsetof(SnesCpuContext, flag_e),
        (uint32_t)offsetof(SnesCpuContext, wram),    (uint32_t)offsetof(SnesCpuContext, sram),
        (uint32_t)offsetof(SnesCpuContext, ppu),     (uint32_t)offsetof(SnesCpuContext, cpu_io),
        (uint32_t)offsetof(SnesCpuContext, apu),     (uint32_t)offsetof(SnesCpuContext, dma),
        (uint32_t)offsetof(SnesCpuContext, unresolved_jumps),
        (uint32_t)offsetof(SnesPpuState, vram),      (uint32_t)offsetof(SnesPpuState, cgram),
        (uint32_t)offsetof(SnesPpuState, oam),       (uint32_t)offsetof(SnesPpuState, m7a),
        (uint32_t)offsetof(SnesApu, ram),            (uint32_t)offsetof(SnesApu, pc),
        (uint32_t)offsetof(SnesApu, pcm_ring),       (uint32_t)offsetof(SnesApu, pcm_count),
        (uint32_t)sizeof(SnesDmaChannel),            (uint32_t)sizeof(SnesCpuIoState),
    };
    return fnv1a32(2166136261u, offsets, sizeof(offsets));
}

static uint32_t rom_checksum(const SnesCpuContext *ctx) {
    return (ctx->rom != NULL) ? fnv1a32(2166136261u, ctx->rom, ctx->rom_size) : 0u;
}

static void put32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24);
}

static uint32_t get32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

SnesSavestateResult snes_savestate_save(const char *path, const SnesCpuContext *ctx, const SnesApu *apu) {
    uint32_t payload = fnv1a32(2166136261u, ctx, sizeof(*ctx));
    payload = fnv1a32(payload, apu, sizeof(*apu));

    uint8_t hdr[HEADER_SIZE];
    memset(hdr, 0, sizeof(hdr));
    memcpy(hdr, k_magic, sizeof(k_magic));
    put32(&hdr[8],  SNES_SAVESTATE_VERSION);
    put32(&hdr[12], (uint32_t)sizeof(*ctx));
    put32(&hdr[16], (uint32_t)sizeof(*apu));
    put32(&hdr[20], layout_fingerprint());
    put32(&hdr[24], rom_checksum(ctx));
    put32(&hdr[28], payload);
    /* [32..39] reserved */

    /* Write to a temporary file and swap it in, so a crash never leaves a half-written state */
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    FILE *fp = fopen(tmp, "wb");
    if (fp == NULL) {
        return SNES_SAVESTATE_IO_ERROR;
    }
    bool ok = fwrite(hdr, 1u, sizeof(hdr), fp) == sizeof(hdr) &&
              fwrite(ctx, sizeof(*ctx), 1u, fp) == 1u &&
              fwrite(apu, sizeof(*apu), 1u, fp) == 1u;
    ok = (fclose(fp) == 0) && ok;
    if (!ok) {
        remove(tmp);
        return SNES_SAVESTATE_IO_ERROR;
    }
    remove(path);
    if (rename(tmp, path) != 0) {
        remove(tmp);
        return SNES_SAVESTATE_IO_ERROR;
    }
    return SNES_SAVESTATE_OK;
}

SnesSavestateResult snes_savestate_load(const char *path, SnesCpuContext *ctx, SnesApu *apu) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return SNES_SAVESTATE_IO_ERROR;
    }
    uint8_t hdr[HEADER_SIZE];
    if (fread(hdr, 1u, sizeof(hdr), fp) != sizeof(hdr) || memcmp(hdr, k_magic, sizeof(k_magic)) != 0) {
        fclose(fp);
        return SNES_SAVESTATE_NOT_A_SAVESTATE;
    }
    if (get32(&hdr[8]) != SNES_SAVESTATE_VERSION) {
        fclose(fp);
        return SNES_SAVESTATE_VERSION_MISMATCH;
    }
    if (get32(&hdr[12]) != (uint32_t)sizeof(*ctx) || get32(&hdr[16]) != (uint32_t)sizeof(*apu) ||
        get32(&hdr[20]) != layout_fingerprint()) {
        fclose(fp);
        return SNES_SAVESTATE_LAYOUT_MISMATCH;
    }
    if (get32(&hdr[24]) != rom_checksum(ctx)) {
        fclose(fp);
        return SNES_SAVESTATE_ROM_MISMATCH;
    }

    /* Stage into scratch copies; the live machine is only touched after full validation */
    SnesCpuContext *new_ctx = (SnesCpuContext *)malloc(sizeof(*new_ctx));
    SnesApu        *new_apu = (SnesApu *)malloc(sizeof(*new_apu));
    if (new_ctx == NULL || new_apu == NULL) {
        free(new_ctx);
        free(new_apu);
        fclose(fp);
        return SNES_SAVESTATE_NO_MEMORY;
    }
    bool ok = fread(new_ctx, sizeof(*new_ctx), 1u, fp) == 1u && fread(new_apu, sizeof(*new_apu), 1u, fp) == 1u;
    fclose(fp);
    uint32_t payload = fnv1a32(2166136261u, new_ctx, sizeof(*new_ctx));
    payload = fnv1a32(payload, new_apu, sizeof(*new_apu));
    if (!ok || payload != get32(&hdr[28])) {
        free(new_ctx);
        free(new_apu);
        return SNES_SAVESTATE_CORRUPT;
    }

    /* Keep host-owned pointers, hooks and policy from the running context */
    new_ctx->rom                = ctx->rom;
    new_ctx->rom_size           = ctx->rom_size;
    new_ctx->on_write_ppu       = ctx->on_write_ppu;
    new_ctx->on_read_ppu        = ctx->on_read_ppu;
    new_ctx->on_write_cpu_io    = ctx->on_write_cpu_io;
    new_ctx->on_read_cpu_io     = ctx->on_read_cpu_io;
    new_ctx->on_interp_fallback = ctx->on_interp_fallback;
    new_ctx->strict_fallback    = ctx->strict_fallback;
    new_ctx->apu_core           = ctx->apu_core;
    new_apu->pcm_sink      = apu->pcm_sink;
    new_apu->pcm_sink_user = apu->pcm_sink_user;
    memcpy(ctx, new_ctx, sizeof(*ctx));
    memcpy(apu, new_apu, sizeof(*apu));
    snes_apu_clear_samples(apu);
    free(new_ctx);
    free(new_apu);
    return SNES_SAVESTATE_OK;
}

const char *snes_savestate_result_str(SnesSavestateResult r) {
    switch (r) {
        case SNES_SAVESTATE_OK:               return "ok";
        case SNES_SAVESTATE_IO_ERROR:         return "file could not be read or written";
        case SNES_SAVESTATE_NOT_A_SAVESTATE:  return "not a Contra III save state";
        case SNES_SAVESTATE_VERSION_MISMATCH: return "save state format version is not supported";
        case SNES_SAVESTATE_LAYOUT_MISMATCH:  return "save state was written by an incompatible build";
        case SNES_SAVESTATE_ROM_MISMATCH:     return "save state belongs to a different ROM";
        case SNES_SAVESTATE_CORRUPT:          return "save state is truncated or corrupt";
        case SNES_SAVESTATE_NO_MEMORY:        return "out of memory";
        default:                              return "unknown error";
    }
}
