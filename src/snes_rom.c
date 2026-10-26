/*
 * Contra III: The Alien Wars (USA) - ROM Location & Validation
 */

#include "snes_rom.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void set_err(char *err, size_t err_len, const char *fmt, ...) {
    if (err == NULL || err_len == 0u) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(err, err_len, fmt, ap);
    va_end(ap);
}

uint8_t *snes_rom_load(const char *path, size_t *out_size, char *err, size_t err_len) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        set_err(err, err_len, "cannot open '%s'", path);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (sz != (long)CONTRA3_ROM_SIZE && sz != (long)(CONTRA3_ROM_SIZE + 512u)) {
        fclose(fp);
        set_err(err, err_len, "'%s' is %ld bytes; expected a 1 MB Contra III (USA) image", path, sz);
        return NULL;
    }
    if (sz == (long)(CONTRA3_ROM_SIZE + 512u)) {
        fseek(fp, 512, SEEK_SET); /* copier header */
    }
    uint8_t *rom = (uint8_t *)malloc(CONTRA3_ROM_SIZE);
    if (rom == NULL || fread(rom, 1u, CONTRA3_ROM_SIZE, fp) != CONTRA3_ROM_SIZE) {
        free(rom);
        fclose(fp);
        set_err(err, err_len, "short read on '%s'", path);
        return NULL;
    }
    fclose(fp);

    /* LoROM internal header at $00:FFC0 (file offset $7FC0) */
    static const char k_title[] = "CONTRA3 THE ALIEN WAR";
    if (memcmp(&rom[0x7FC0], k_title, sizeof(k_title) - 1u) != 0) {
        free(rom);
        set_err(err, err_len, "'%s' is not Contra III: The Alien Wars (USA) (internal title mismatch)", path);
        return NULL;
    }
    uint16_t complement = (uint16_t)(rom[0x7FDC] | (rom[0x7FDD] << 8));
    uint16_t checksum   = (uint16_t)(rom[0x7FDE] | (rom[0x7FDF] << 8));
    uint32_t sum = 0u;
    for (size_t i = 0u; i < CONTRA3_ROM_SIZE; ++i) {
        sum += rom[i];
    }
    if ((uint16_t)(complement + checksum) != 0xFFFFu || (uint16_t)sum != checksum) {
        free(rom);
        set_err(err, err_len, "'%s' fails the cartridge checksum (bad dump or patched ROM)", path);
        return NULL;
    }
    *out_size = CONTRA3_ROM_SIZE;
    return rom;
}

static bool file_exists(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (fp != NULL) {
        fclose(fp);
        return true;
    }
    return false;
}

bool snes_rom_find(const char *exe_dir, char *out, size_t out_len) {
    const char *env = getenv("CONTRA3_ROM");
    if (env != NULL && env[0] != '\0' && file_exists(env)) {
        snprintf(out, out_len, "%s", env);
        return true;
    }
    static const char *const k_rel[] = { "", "../", "../ROM/", "../../ROM/" };
    const char *base = (exe_dir != NULL && exe_dir[0] != '\0') ? exe_dir : ".";
    for (size_t i = 0u; i < sizeof(k_rel) / sizeof(k_rel[0]); ++i) {
        snprintf(out, out_len, "%s/%s%s", base, k_rel[i], CONTRA3_ROM_FILENAME);
        if (file_exists(out)) {
            return true;
        }
    }
    snprintf(out, out_len, "%s", CONTRA3_ROM_FILENAME);
    return file_exists(out);
}

void snes_rom_dirname(const char *path, char *out, size_t out_len) {
    const char *slash = NULL;
    for (const char *p = path; p != NULL && *p != '\0'; ++p) {
        if (*p == '/' || *p == '\\') {
            slash = p;
        }
    }
    if (slash == NULL) {
        snprintf(out, out_len, ".");
    } else {
        snprintf(out, out_len, "%.*s", (int)(slash - path), path);
    }
}
