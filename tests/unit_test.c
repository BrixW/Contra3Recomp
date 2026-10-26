/*
 * Contra III Recompilation - ROM-free unit tests
 *
 * Covers the portable frontend pieces that do not need the game: contra3.ini round-trip and
 * key names, keyboard/gamepad mapping, the overlay (navigation, cheat toggles, key capture,
 * rendering), rumble envelopes, the .c3m movie recorder/loader and the DEFLATE decoder.
 * Runs on every platform in CI, where no ROM is available.
 *
 * Usage: contra3_unit_test [scratch-dir]   (default ".")
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snes_ppu.h"
#include "snes_movie.h"
#include "c3_config.h"
#include "c3_overlay.h"
#include "c3_rumble.h"
#include "contra3_events.h"

static int g_failures = 0;
static int g_checks = 0;

#define CHECK(cond, msg)                                                         \
    do {                                                                         \
        g_checks++;                                                              \
        if (!(cond)) {                                                           \
            fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, (msg));    \
            g_failures++;                                                        \
        }                                                                        \
    } while (0)

/* ------------------------------------------------------------------------- */

static void test_key_names(void) {
    for (int k = 1; k < C3K_COUNT; ++k) {
        const char *name = c3_key_name((C3Key)k);
        CHECK(name != NULL && strcmp(name, "?") != 0, "every key has a name");
        CHECK(c3_key_from_name(name) == (C3Key)k, "key name round-trips");
    }
    CHECK(c3_key_from_name("not-a-key") == C3K_NONE, "unknown key name");
    for (uint16_t m = 0u, i = 0u; i < 13u; ++i) {
        uint16_t next = c3_snes_mask_next(m, 1);
        CHECK(c3_snes_mask_next(next, -1) == m, "SNES mask cycling is reversible");
        m = next;
    }
}

static bool fake_down(C3Key key, void *user) {
    const C3Key *held = (const C3Key *)user;
    for (; *held != C3K_NONE; ++held) {
        if (*held == key) {
            return true;
        }
    }
    return false;
}

static void test_mapping(void) {
    Contra3Config cfg;
    memset(&cfg, 0, sizeof(cfg));
    c3_config_defaults(&cfg);

    C3Key none[] = { C3K_NONE };
    CHECK(c3_config_keys_to_pad(&cfg, 0u, fake_down, none) == 0u, "no keys -> no buttons");

    c3_config_bind_key(&cfg, 0u, 0u, C3K_W);   /* button 0 = UP */
    C3Key w[] = { C3K_W, C3K_NONE };
    CHECK(c3_config_keys_to_pad(&cfg, 0u, fake_down, w) == c3_button_mask(0u), "bound key presses UP");
    CHECK(c3_config_keys_to_pad(&cfg, 1u, fake_down, w) == 0u, "P1 binding does not leak to P2");

    c3_config_bind_key(&cfg, 0u, 1u, C3K_W);   /* rebinding W to DOWN removes it from UP */
    CHECK(c3_config_keys_to_pad(&cfg, 0u, fake_down, w) == c3_button_mask(1u), "rebinding moves the key");

    bool pad[C3P_COUNT];
    memset(pad, 0, sizeof(pad));
    CHECK(c3_config_map_pad(&cfg, pad) == 0u, "idle pad");
    pad[C3P_START] = true;
    CHECK(c3_config_map_pad(&cfg, pad) == SNES_BTN_START, "pad START -> SNES START by default");
    cfg.pad_map[C3P_START] = SNES_BTN_SELECT;
    CHECK(c3_config_map_pad(&cfg, pad) == SNES_BTN_SELECT, "pad remap");
}

static void test_config_roundtrip(const char *dir) {
    Contra3Config a, b;
    memset(&a, 0, sizeof(a));
    char ini[1100];
    snprintf(ini, sizeof(ini), "%s/contra3.ini", dir);
    remove(ini);

    c3_config_load(&a, dir);                     /* creates the file with defaults */
    FILE *fp = fopen(ini, "r");
    CHECK(fp != NULL, "load writes a default contra3.ini");
    if (fp != NULL) {
        fclose(fp);
    }

    c3_config_bind_key(&a, 1u, 4u, C3K_NUMDOT);
    a.pad_map[C3P_LT] = SNES_BTN_L;
    a.pad_deadzone = 12000;
    a.rumble = false;
    a.rumble_strength = 40u;
    snprintf(a.filter, sizeof(a.filter), "crt");
    a.integer_scale = true;
    a.vsync = true;
    a.lag_model = true;
    a.volume = 70u;
    a.opt_lives = 7u;
    a.opt_difficulty = 4u;
    a.opt_sound = 2u;
    a.have_options = true;
    a.stage_select = true;
    a.lives_30 = true;
    a.infinite_lives = true;
    a.start_stage = 5u;
    CHECK(c3_config_save(&a), "save");

    memset(&b, 0x5A, sizeof(b));
    c3_config_load(&b, dir);
    CHECK(memcmp(a.keys, b.keys, sizeof(a.keys)) == 0, "keys round-trip");
    CHECK(memcmp(a.pad_map, b.pad_map, sizeof(a.pad_map)) == 0, "pad map round-trips");
    CHECK(b.pad_deadzone == 12000 && !b.rumble && b.rumble_strength == 40u, "gamepad/rumble round-trip");
    CHECK(strcmp(b.filter, "crt") == 0 && b.integer_scale && b.vsync && b.lag_model && b.volume == 70u, "video/audio/game round-trip");
    CHECK(b.have_options && b.opt_lives == 7u && b.opt_difficulty == 4u && b.opt_sound == 2u, "options round-trip");
    CHECK(b.stage_select && b.lives_30 && b.infinite_lives && b.start_stage == 5u, "extras round-trip");
    remove(ini);
}

static void test_overlay(void) {
    Contra3Config cfg;
    memset(&cfg, 0, sizeof(cfg));
    c3_config_defaults(&cfg);
    C3Overlay ov;
    c3_overlay_init(&ov);
    CHECK(!ov.open, "overlay starts closed");

    /* Cheats page: OK toggles stage select and marks the config dirty */
    c3_overlay_open_at(&ov, C3OV_CHEATS);
    CHECK(ov.open && !ov.dirty, "open at cheats");
    bool before = cfg.stage_select;
    c3_overlay_nav(&ov, &cfg, C3NAV_OK);
    CHECK(cfg.stage_select != before && ov.dirty, "stage select toggled");
    c3_overlay_nav(&ov, &cfg, C3NAV_DOWN);
    c3_overlay_nav(&ov, &cfg, C3NAV_DOWN);
    c3_overlay_nav(&ov, &cfg, C3NAV_DOWN);
    c3_overlay_nav(&ov, &cfg, C3NAV_RIGHT);
    CHECK(cfg.start_stage == 1u, "start stage advances");
    c3_overlay_nav(&ov, &cfg, C3NAV_LEFT);
    c3_overlay_nav(&ov, &cfg, C3NAV_LEFT);
    CHECK(cfg.start_stage == 6u, "start stage wraps");

    /* BACK walks to the main menu, then closes */
    CHECK(c3_overlay_nav(&ov, &cfg, C3NAV_BACK) == C3ACT_NONE && ov.open, "back to main");
    CHECK(c3_overlay_nav(&ov, &cfg, C3NAV_BACK) == C3ACT_RESUME && !ov.open, "back closes");
    CHECK(c3_overlay_nav(&ov, &cfg, C3NAV_OK) == C3ACT_NONE, "closed overlay ignores input");

    /* Main menu: the first entry resumes */
    c3_overlay_open(&ov);
    CHECK(c3_overlay_nav(&ov, &cfg, C3NAV_OK) == C3ACT_RESUME && !ov.open, "resume");

    /* Key capture on the P2 keys page: row 0 = bind all, row 1 = UP */
    c3_overlay_open_at(&ov, C3OV_KEYS_P2);
    c3_overlay_nav(&ov, &cfg, C3NAV_DOWN);
    c3_overlay_nav(&ov, &cfg, C3NAV_OK);
    CHECK(c3_overlay_capturing(&ov), "capturing a key");
    CHECK(c3_overlay_nav(&ov, &cfg, C3NAV_DOWN) == C3ACT_NONE && c3_overlay_capturing(&ov), "nav ignored while capturing");
    c3_overlay_capture_key(&ov, &cfg, C3K_I);
    CHECK(!c3_overlay_capturing(&ov), "capture done");
    CHECK(cfg.keys[1][0][0] == C3K_I && cfg.keys[1][0][1] == C3K_NONE, "P2 UP bound to I");

    /* Bind all: every button in turn, Escape keeps a button */
    c3_overlay_nav(&ov, &cfg, C3NAV_UP);
    c3_overlay_nav(&ov, &cfg, C3NAV_OK);
    static const C3Key seq[C3_BUTTONS] = { C3K_ESCAPE, C3K_K, C3K_J, C3K_L, C3K_Z, C3K_X, C3K_C, C3K_V,
                                           C3K_Q, C3K_E, C3K_ENTER, C3K_RSHIFT };
    for (unsigned i = 0u; i < C3_BUTTONS; ++i) {
        CHECK(c3_overlay_capturing(&ov), "bind all still capturing");
        c3_overlay_capture_key(&ov, &cfg, seq[i]);
    }
    CHECK(!c3_overlay_capturing(&ov), "bind all finished");
    CHECK(cfg.keys[1][0][0] == C3K_I, "Escape kept UP");
    for (unsigned i = 1u; i < C3_BUTTONS; ++i) {
        CHECK(cfg.keys[1][i][0] == seq[i], "bind all assigned the key");
    }

    /* Rendering: draws a panel over the dimmed frame and only writes opaque pixels */
    static uint32_t game[C3_OVERLAY_W * C3_OVERLAY_H], out[C3_OVERLAY_W * C3_OVERLAY_H];
    for (int i = 0; i < C3_OVERLAY_W * C3_OVERLAY_H; ++i) {
        game[i] = 0xFFFFFFFFu;
        out[i] = 0u;
    }
    C3OverlayEnv env = { true, false, false, "STATUS" };
    for (int page = C3OV_MAIN; page <= C3OV_CHEATS; ++page) {
        c3_overlay_open_at(&ov, (C3OverlayPage)page);
        c3_overlay_render(&ov, &cfg, &env, game, out);
        unsigned opaque = 0u, white = 0u;
        for (int i = 0; i < C3_OVERLAY_W * C3_OVERLAY_H; ++i) {
            opaque += (out[i] >> 24) == 0xFFu;
            white += out[i] == 0xFFFFFFFFu;
        }
        CHECK(opaque == (unsigned)(C3_OVERLAY_W * C3_OVERLAY_H), "overlay frame fully opaque");
        CHECK(white < (unsigned)(C3_OVERLAY_W * C3_OVERLAY_H / 8), "game frame dimmed behind the panel");
    }
    CHECK(c3_overlay_text_width("ABC") > 0, "text width");
}

static void test_rumble(void) {
    C3Rumble r;
    c3_rumble_reset(&r);
    c3_rumble_frame(&r, 0u, 100u, true);
    CHECK(r.out_low[0] == 0u && r.out_high[0] == 0u && r.out_low[1] == 0u, "idle motors");

    c3_rumble_frame(&r, C3EV_P1_DEATH, 100u, true);
    CHECK(r.out_low[0] > 0u && r.out_high[0] > 0u && r.changed[0], "P1 death rumbles pad 1");
    CHECK(r.out_low[1] == 0u, "P1 death leaves pad 2 alone");
    uint16_t first = r.out_low[0];
    c3_rumble_frame(&r, 0u, 100u, true);
    CHECK(r.out_low[0] < first, "envelope decays");
    for (int i = 0; i < 120; ++i) {
        c3_rumble_frame(&r, 0u, 100u, true);
    }
    CHECK(r.out_low[0] == 0u && r.out_high[0] == 0u, "envelope ends");

    c3_rumble_frame(&r, C3EV_P2_BOMB, 100u, true);
    CHECK(r.out_low[1] > r.out_low[0] && r.out_low[0] > 0u, "bomb: thrower strongest, partner light");

    C3Rumble half;
    c3_rumble_reset(&half);
    c3_rumble_frame(&half, C3EV_P2_BOMB, 50u, true);
    CHECK(half.out_low[1] < r.out_low[1] && half.out_low[1] > 0u, "strength scales");

    c3_rumble_frame(&r, C3EV_BOSS_BLAST, 100u, false);
    CHECK(r.out_low[0] == 0u && r.out_low[1] == 0u && r.out_high[0] == 0u && r.out_high[1] == 0u, "disabled silences");
}

static void test_movie(const char *dir) {
    char path[1100];
    snprintf(path, sizeof(path), "%s/unit_test.c3m", dir);
    SnesMovieRecorder rec;
    memset(&rec, 0, sizeof(rec));
    CHECK(snes_movie_record_begin(&rec, path, "unit test"), "record begin");
    for (uint32_t f = 0u; f < 300u; ++f) {
        uint16_t p1 = (uint16_t)((f * 0x1F0u) & 0xFFF0u), p2 = (uint16_t)(f % 7u == 0u ? SNES_BTN_START : 0u);
        snes_movie_record_frame(&rec, p1, p2, f == 200u ? SNES_MOVIE_FLAG_RESET : 0u);
    }
    snes_movie_record_end(&rec);

    SnesMovie m;
    memset(&m, 0, sizeof(m));
    char err[256];
    bool ok = snes_movie_load(path, &m, err, sizeof(err));
    CHECK(ok, err);
    if (ok) {
        CHECK(m.format == SNES_MOVIE_C3M && m.frame_count == 300u, "c3m header");
        bool same = true;
        for (uint32_t f = 0u; f < 300u; ++f) {
            uint16_t p1 = (uint16_t)((f * 0x1F0u) & 0xFFF0u), p2 = (uint16_t)(f % 7u == 0u ? SNES_BTN_START : 0u);
            same = same && snes_movie_pad(&m, f, 0u) == p1 && snes_movie_pad(&m, f, 1u) == p2 &&
                   snes_movie_flags(&m, f) == (f == 200u ? SNES_MOVIE_FLAG_RESET : 0u);
        }
        CHECK(same, "c3m frames round-trip");
        snes_movie_free(&m);
    }
    remove(path);

    const uint8_t junk[] = "definitely not a movie";
    CHECK(!snes_movie_load_memory(junk, sizeof(junk), &m, err, sizeof(err)), "garbage rejected");
}

static void test_inflate(void) {
    /* zlib.compressobj(9, DEFLATED, -15) of "Contra III Contra III Contra III" (fixed Huffman + matches) */
    static const uint8_t fixed[] = { 0x73, 0xCE, 0xCF, 0x2B, 0x29, 0x4A, 0x54, 0xF0, 0xF4, 0xF4, 0x54,
                                     0x70, 0xC6, 0xC6, 0x04, 0x00 };
    static const char expect[] = "Contra III Contra III Contra III";
    uint8_t out[64];
    size_t n = snes_inflate(fixed, sizeof(fixed), out, sizeof(out));
    CHECK(n == sizeof(expect) - 1u && memcmp(out, expect, n) == 0, "fixed-Huffman inflate");
    CHECK(snes_inflate(fixed, sizeof(fixed), out, 8u) == (size_t)-1, "inflate overflow detected");

    /* Stored block: BFINAL=1 BTYPE=00, LEN=5, NLEN=~5 */
    static const uint8_t stored[] = { 0x01, 0x05, 0x00, 0xFA, 0xFF, 'S', 'N', 'E', 'S', '!' };
    n = snes_inflate(stored, sizeof(stored), out, sizeof(out));
    CHECK(n == 5u && memcmp(out, "SNES!", 5u) == 0, "stored-block inflate");
}

int main(int argc, char **argv) {
    const char *dir = (argc > 1) ? argv[1] : ".";
    test_key_names();
    test_mapping();
    test_config_roundtrip(dir);
    test_overlay();
    test_rumble();
    test_movie(dir);
    test_inflate();
    if (g_failures != 0) {
        fprintf(stderr, "%d of %d checks failed\n", g_failures, g_checks);
        return 1;
    }
    printf("[PASS] %d unit checks\n", g_checks);
    return 0;
}
