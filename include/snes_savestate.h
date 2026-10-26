#ifndef CONTRA3_SNES_SAVESTATE_H
#define CONTRA3_SNES_SAVESTATE_H

#include <stddef.h>
#include <stdbool.h>
#include "cpu_65816.h"
#include "snes_apu.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Versioned save states (CPU/PPU/DMA context + SPC700/S-DSP core).
 *
 * File layout: 40-byte header, then the raw SnesCpuContext and SnesApu images.
 *   magic "C3SAVE\r\n", format version, struct sizes, a layout fingerprint built
 *   from field offsets, an FNV-1a checksum of the ROM, and an FNV-1a checksum of
 *   the payload. Loading validates everything into a scratch buffer first, so a
 *   rejected or corrupt file never touches the running machine. Host-side pointers
 *   and hooks (ROM, MMIO hooks, fallback hook, APU link) are preserved across loads.
 */

#define SNES_SAVESTATE_VERSION 1u

typedef enum SnesSavestateResult {
    SNES_SAVESTATE_OK = 0,
    SNES_SAVESTATE_IO_ERROR,         /* cannot open / read / write the file */
    SNES_SAVESTATE_NOT_A_SAVESTATE,  /* bad magic or truncated header */
    SNES_SAVESTATE_VERSION_MISMATCH, /* written by an incompatible format version */
    SNES_SAVESTATE_LAYOUT_MISMATCH,  /* struct sizes / field layout changed since it was written */
    SNES_SAVESTATE_ROM_MISMATCH,     /* written for a different ROM image */
    SNES_SAVESTATE_CORRUPT,          /* payload truncated or checksum mismatch */
    SNES_SAVESTATE_NO_MEMORY,
    SNES_SAVESTATE_BUSY              /* the 65816 main thread is mid-job (loading); try again in a moment */
} SnesSavestateResult;

SnesSavestateResult snes_savestate_save(const char *path, const SnesCpuContext *ctx, const SnesApu *apu);
SnesSavestateResult snes_savestate_load(const char *path, SnesCpuContext *ctx, SnesApu *apu);
const char         *snes_savestate_result_str(SnesSavestateResult r);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_SNES_SAVESTATE_H */
