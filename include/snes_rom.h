#ifndef CONTRA3_SNES_ROM_H
#define CONTRA3_SNES_ROM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CONTRA3_ROM_FILENAME "Contra III - The Alien Wars (USA).sfc"
#define CONTRA3_ROM_SIZE     1048576u

/*
 * Load and validate a Contra III (USA) image. A 512-byte copier header is stripped,
 * then the LoROM internal header is checked (title "CONTRA3 THE ALIEN WAR",
 * checksum + complement == $FFFF, and the stored checksum matches the image).
 * Returns a malloc'd 1 MB buffer, or NULL with a reason in err.
 */
uint8_t *snes_rom_load(const char *path, size_t *out_size, char *err, size_t err_len);

/*
 * Locate the ROM when none was given on the command line. Search order:
 *   1. $CONTRA3_ROM
 *   2. <exe_dir>/<CONTRA3_ROM_FILENAME>
 *   3. <exe_dir>/../<CONTRA3_ROM_FILENAME>
 *   4. <exe_dir>/../ROM/... and <exe_dir>/../../ROM/... (project-sibling ROM folder)
 * exe_dir may be NULL (only $CONTRA3_ROM and the current directory are tried).
 * Returns true and writes the first existing candidate into out.
 */
bool snes_rom_find(const char *exe_dir, char *out, size_t out_len);

/* Directory part of a path such as argv[0] ("." when there is none) */
void snes_rom_dirname(const char *path, char *out, size_t out_len);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_SNES_ROM_H */
