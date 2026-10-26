#ifndef CONTRA3_SNES_PPU_H
#define CONTRA3_SNES_PPU_H

#include <stdint.h>
#include <stdbool.h>
#include "contra3_recomp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SNES_SCREEN_WIDTH   256
#define SNES_SCREEN_HEIGHT  224

/* Standard SNES Auto-Joypad ($4218-$4219) 16-bit Button Masks */
#define SNES_BTN_B       0x8000u
#define SNES_BTN_Y       0x4000u
#define SNES_BTN_SELECT  0x2000u
#define SNES_BTN_START   0x1000u
#define SNES_BTN_UP      0x0800u
#define SNES_BTN_DOWN    0x0400u
#define SNES_BTN_LEFT    0x0200u
#define SNES_BTN_RIGHT   0x0100u
#define SNES_BTN_A       0x0080u
#define SNES_BTN_X       0x0040u
#define SNES_BTN_L       0x0020u
#define SNES_BTN_R       0x0010u

/*
 * Rasterize the current SNES PPU state (VRAM, CGRAM, OAM, Mode 0-7, Windows,
 * Color Math, Per-Scanline HDMA, and Per-Scanline Mode 7 V-IRQ splits) into a
 * 256x224 ARGB8888 (0xFFRRGGBB) pixel buffer.
 */
void snes_ppu_render_frame(SnesCpuContext *ctx, uint32_t *argb_out);

/*
 * Advance the recompiled Contra III engine by one complete 60 Hz frame:
 * 1. Latches Player 1 & Player 2 joypad masks ($4218-$421B).
 * 2. Executes NMI_Handler ($00:819B), rasterizing active display (V=0..223) at
 *    the exact hardware VBlank -> Active-Display transition ($4212 poll) before
 *    MainModule_Dispatch mutates next-frame WRAM buffers.
 * 3. Steps Background_DecompressWorker ($00:995A) slices to service async LZSS
 *    and SPC700 streaming requests between frames.
 */
void snes_step_frame(SnesCpuContext *ctx, uint16_t pad1, uint16_t pad2, uint32_t *argb_out);

/*
 * Write a 256x224 ARGB8888 (0xFFRRGGBB) framebuffer to a standard 24-bit .bmp file.
 */
bool snes_write_bmp(const char *path, const uint32_t *argb_buf, int width, int height);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_SNES_PPU_H */
