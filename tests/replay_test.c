/*
 * Golden Replay Regression Test
 *
 * Replays the input movies listed in tests/golden/manifest.txt from power-on and
 * hashes the complete machine state every CHECKPOINT_INTERVAL frames (WRAM, VRAM,
 * CGRAM + OAM + PPU registers, SPC700 ARAM, and the rendered frame). The hashes are
 * compared with tests/golden/<name>.hash, so any behavioural change in the generated
 * code, runtime, PPU or APU is reported with the first divergent frame and subsystem.
 *
 * Usage: contra3_replay_test [--update] [--golden <dir>] [--rom <path>]
 *   --update   re-record the .hash files (after an intentional behaviour change)
 *
 * Manifest line: <name> <movie.c3m | -> <frames> [poke=<frame>:<hexaddr>=<hexval>]... [infinite-lives]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contra3_recomp.h"
#include "snes_ppu.h"
#include "snes_apu.h"
#include "snes_movie.h"
#include "snes_rom.h"

#define CHECKPOINT_INTERVAL 300u
#define MAX_POKES           8u
#define NUM_COMPONENTS      5u

static const char *const k_component_names[NUM_COMPONENTS] = { "WRAM", "VRAM", "PPU(CGRAM/OAM/regs)", "ARAM", "FRAME" };

typedef struct Scenario {
    char     name[64];
    char     movie[256];
    unsigned frames;
    unsigned num_pokes;
    unsigned poke_frame[MAX_POKES];
    uint32_t poke_addr[MAX_POKES];
    uint16_t poke_val[MAX_POKES];
    bool     infinite_lives;
    bool     lag_model;
} Scenario;

static SnesCpuContext g_ctx;
static SnesApu        g_apu;
static uint32_t       g_frame[SNES_SCREEN_WIDTH * SNES_SCREEN_HEIGHT];

static uint64_t fnv1a(uint64_t h, const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    for (size_t i = 0u; i < len; ++i) {
        h ^= p[i];
        h *= 0x100000001B3ull;
    }
    return h;
}

static void hash_state(uint64_t out[NUM_COMPONENTS]) {
    const uint64_t seed = 0xCBF29CE484222325ull;
    out[0] = fnv1a(seed, g_ctx.wram, sizeof(g_ctx.wram));
    out[1] = fnv1a(seed, g_ctx.ppu.vram, sizeof(g_ctx.ppu.vram));
    uint64_t h = fnv1a(seed, g_ctx.ppu.cgram, sizeof(g_ctx.ppu.cgram));
    h = fnv1a(h, g_ctx.ppu.oam, sizeof(g_ctx.ppu.oam));
    out[2] = fnv1a(h, g_ctx.ppu.regs, sizeof(g_ctx.ppu.regs));
    out[3] = fnv1a(seed, g_apu.ram, sizeof(g_apu.ram));
    out[4] = fnv1a(seed, g_frame, sizeof(g_frame));
}

static bool parse_scenario(const char *line, Scenario *sc) {
    memset(sc, 0, sizeof(*sc));
    char rest[512] = { 0 };
    if (sscanf(line, "%63s %255s %u %511[^\n]", sc->name, sc->movie, &sc->frames, rest) < 3) {
        return false;
    }
    for (char *tok = strtok(rest, " \t\r"); tok != NULL; tok = strtok(NULL, " \t\r")) {
        unsigned f = 0u, a = 0u, v = 0u;
        if (strcmp(tok, "lag-model") == 0) {
            sc->lag_model = true;
        } else if (strcmp(tok, "infinite-lives") == 0) {
            sc->infinite_lives = true;
        } else if (sscanf(tok, "poke=%u:%x=%x", &f, &a, &v) == 3 && sc->num_pokes < MAX_POKES) {
            sc->poke_frame[sc->num_pokes] = f;
            sc->poke_addr[sc->num_pokes]  = a;
            sc->poke_val[sc->num_pokes]   = (uint16_t)v;
            sc->num_pokes++;
        }
    }
    return true;
}

/* Returns 0 = match, 1 = mismatch/missing golden, 2 = setup error */
static int run_scenario(const Scenario *sc, const char *golden_dir, const uint8_t *rom, size_t rom_size, bool update) {
    SnesMovie movie;
    memset(&movie, 0, sizeof(movie));
    if (strcmp(sc->movie, "-") != 0) {
        char path[512], err[256];
        snprintf(path, sizeof(path), "%s/%s", golden_dir, sc->movie);
        if (!snes_movie_load(path, &movie, err, sizeof(err))) {
            fprintf(stderr, "[FAIL] %s: %s\n", sc->name, err);
            return 2;
        }
    }

    char hash_path[512];
    snprintf(hash_path, sizeof(hash_path), "%s/%s.hash", golden_dir, sc->name);
    FILE *golden = update ? fopen(hash_path, "w") : fopen(hash_path, "r");
    if (golden == NULL) {
        fprintf(stderr, "[FAIL] %s: cannot open %s (run with --update to record)\n", sc->name, hash_path);
        snes_movie_free(&movie);
        return update ? 2 : 1;
    }
    if (update) {
        fprintf(golden, "# frame WRAM VRAM PPU ARAM FRAME (FNV-1a 64) - regenerate with contra3_replay_test --update\n");
    }

    snes_context_init(&g_ctx, rom, rom_size);
    snes_apu_init(&g_apu);
    snes_apu_attach(&g_ctx, &g_apu);
    g_ctx.max_main_loop_iters = 1u;
    g_ctx.lag_budget = sc->lag_model ? SNES_LAG_BUDGET_DEFAULT : 0u;
    bank00_func_8000(&g_ctx);

    uint16_t prev_lives[2] = { 0u, 0u };
    int rc = 0;
    unsigned checkpoints = 0u;
    for (unsigned f = 1u; f <= sc->frames && rc == 0; ++f) {
        uint32_t mf = f - 1u;
        uint16_t pad1 = snes_movie_pad(&movie, mf, 0u);
        uint16_t pad2 = snes_movie_pad(&movie, mf, 1u);
        uint8_t flags = snes_movie_flags(&movie, mf);
        if ((flags & SNES_MOVIE_FLAG_POWER) != 0u) {
            snes_context_init(&g_ctx, rom, rom_size);
            snes_apu_init(&g_apu);
            snes_apu_attach(&g_ctx, &g_apu);
            g_ctx.max_main_loop_iters = 1u;
            g_ctx.lag_budget = sc->lag_model ? SNES_LAG_BUDGET_DEFAULT : 0u;
    bank00_func_8000(&g_ctx);
        } else if ((flags & SNES_MOVIE_FLAG_RESET) != 0u) {
            snes_apu_clear_samples(&g_apu);
            g_ctx.main_loop_iters = 0u;
            g_ctx.max_main_loop_iters = 1u;
            bank00_func_80a3(&g_ctx);
        }
        if (sc->infinite_lives) {
            static const uint32_t lives_addr[2] = { 0x001F8Au, 0x001FCAu };
            for (unsigned p = 0u; p < 2u; ++p) {
                uint16_t now = snes_bus_read16(&g_ctx, lives_addr[p]);
                if (now == 0u && prev_lives[p] != 0u) {
                    snes_bus_write16(&g_ctx, lives_addr[p], 2u);
                    now = 2u;
                }
                prev_lives[p] = now;
            }
        }
        for (unsigned k = 0u; k < sc->num_pokes; ++k) {
            if (sc->poke_frame[k] == f) {
                snes_bus_write16(&g_ctx, sc->poke_addr[k], sc->poke_val[k]);
            }
        }

        bool checkpoint = (f % CHECKPOINT_INTERVAL) == 0u || f == sc->frames;
        snes_step_frame(&g_ctx, pad1, pad2, checkpoint ? g_frame : NULL);
        snes_apu_clear_samples(&g_apu);
        if (!checkpoint) {
            continue;
        }

        uint64_t h[NUM_COMPONENTS];
        hash_state(h);
        checkpoints++;
        if (update) {
            fprintf(golden, "%u", f);
            for (unsigned c = 0u; c < NUM_COMPONENTS; ++c) {
                fprintf(golden, " %016llX", (unsigned long long)h[c]);
            }
            fprintf(golden, "\n");
            continue;
        }

        char line[256];
        unsigned gf = 0u;
        unsigned long long g[NUM_COMPONENTS] = { 0u };
        do {
            if (fgets(line, sizeof(line), golden) == NULL) {
                line[0] = '\0';
                break;
            }
        } while (line[0] == '#');
        if (sscanf(line, "%u %llx %llx %llx %llx %llx", &gf, &g[0], &g[1], &g[2], &g[3], &g[4]) != 6 || gf != f) {
            fprintf(stderr, "[FAIL] %s: golden file has no checkpoint for frame %u\n", sc->name, f);
            rc = 1;
            break;
        }
        for (unsigned c = 0u; c < NUM_COMPONENTS; ++c) {
            if (g[c] != h[c]) {
                fprintf(stderr, "[FAIL] %s: first divergence at frame %u in %s (state=$%02X stage=%u)\n",
                        sc->name, f, k_component_names[c],
                        snes_bus_read8(&g_ctx, 0x000032u), snes_bus_read16(&g_ctx, 0x000086u));
                rc = 1;
                break;
            }
        }
    }
    if (rc == 0 && g_ctx.unresolved_jumps != 0u) {
        fprintf(stderr, "[FAIL] %s: %u unresolved indirect jumps (last $%06X)\n",
                sc->name, g_ctx.unresolved_jumps, (unsigned)g_ctx.last_unresolved_addr);
        rc = 1;
    }
    fclose(golden);
    snes_movie_free(&movie);
    if (rc == 0) {
        printf("[%s] %-22s %6u frames, %3u checkpoints, final state=$%02X stage=%u, lag frames=%u\n",
               update ? "REC " : "PASS", sc->name, sc->frames, checkpoints,
               snes_bus_read8(&g_ctx, 0x000032u), snes_bus_read16(&g_ctx, 0x000086u), (unsigned)g_ctx.lag_frames);
    }
    return rc;
}

int main(int argc, char **argv) {
    const char *golden_dir = "tests/golden";
    const char *rom_path = NULL;
    bool update = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--update") == 0) {
            update = true;
        } else if (strcmp(argv[i], "--golden") == 0 && i + 1 < argc) {
            golden_dir = argv[++i];
        } else if (strcmp(argv[i], "--rom") == 0 && i + 1 < argc) {
            rom_path = argv[++i];
        }
    }
    static char found[1024];
    if (rom_path == NULL) {
        char exe_dir[1024];
        snes_rom_dirname(argv[0], exe_dir, sizeof(exe_dir));
        if (!snes_rom_find(exe_dir, found, sizeof(found))) {
            fprintf(stderr, "[FAIL] Contra III ROM not found (set CONTRA3_ROM or pass --rom)\n");
            return 2;
        }
        rom_path = found;
    }

    size_t rom_size = 0u;
    char err[512];
    uint8_t *rom = snes_rom_load(rom_path, &rom_size, err, sizeof(err));
    if (rom == NULL) {
        fprintf(stderr, "[FAIL] %s\n", err);
        return 2;
    }

    char manifest_path[512];
    snprintf(manifest_path, sizeof(manifest_path), "%s/manifest.txt", golden_dir);
    FILE *mf = fopen(manifest_path, "r");
    if (mf == NULL) {
        fprintf(stderr, "[FAIL] cannot open %s\n", manifest_path);
        free(rom);
        return 2;
    }

    int worst = 0;
    unsigned total = 0u, failed = 0u;
    char line[1024];
    while (fgets(line, sizeof(line), mf) != NULL) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }
        Scenario sc;
        if (!parse_scenario(line, &sc)) {
            continue;
        }
        total++;
        int rc = run_scenario(&sc, golden_dir, rom, rom_size, update);
        if (rc != 0) {
            failed++;
        }
        if (rc > worst) {
            worst = rc;
        }
    }
    fclose(mf);
    free(rom);

    if (failed == 0u) {
        printf("%s %u GOLDEN REPLAY SCENARIOS\n", update ? "RECORDED" : "ALL PASSED:", total);
    } else {
        printf("%u/%u golden replay scenarios FAILED\n", failed, total);
    }
    return worst;
}
