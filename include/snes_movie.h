#ifndef CONTRA3_SNES_MOVIE_H
#define CONTRA3_SNES_MOVIE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Input Movie Playback & Recording (Step 4 co-simulation harness)
 *
 * Supported input formats (all decoded into per-frame 16-bit SNES_BTN_* masks):
 *   .smv  Snes9x 1.43 (v1), 1.51 (v4) and 1.52+ (v5) movies started from power-on/reset
 *   .bk2  BizHawk movie archives (zip; stored or deflate) - "Input Log.txt" is parsed
 *         through its LogKey so any SNES core layout (bsnes / Snes9x / Faust) maps correctly
 *   .txt  A bare BizHawk "Input Log.txt" extracted from a .bk2
 *   .c3m  Native Contra III recompilation text movie written by --record (one line per frame)
 *
 * Joypad masks use the hardware $4218/$421A auto-read layout (SNES_BTN_B = 0x8000 ... SNES_BTN_R = 0x0010).
 */

typedef enum SnesMovieFormat {
    SNES_MOVIE_NONE = 0,
    SNES_MOVIE_SMV,
    SNES_MOVIE_BK2,
    SNES_MOVIE_BK2_LOG,
    SNES_MOVIE_C3M
} SnesMovieFormat;

/* Per-frame command flags */
#define SNES_MOVIE_FLAG_RESET  0x01u   /* Soft reset requested on this frame */
#define SNES_MOVIE_FLAG_POWER  0x02u   /* Power cycle requested on this frame */

typedef struct SnesMovie {
    SnesMovieFormat format;
    uint32_t  frame_count;
    uint32_t  rerecords;
    uint16_t *pads;        /* frame_count * 2 entries: [f*2+0] = P1, [f*2+1] = P2 */
    uint8_t  *flags;       /* frame_count entries of SNES_MOVIE_FLAG_* */
    bool      pal;
    char      author[128];
    char      rom_name[128];
} SnesMovie;

/*
 * Load a movie from disk. The format is detected from the file contents
 * (SMV signature, zip signature, "[Input]"/"LogKey:" text, or "C3M" header).
 * Returns false and writes a human readable reason into err on failure
 * (including movies that start from a savestate, which cannot be co-simulated).
 */
bool snes_movie_load(const char *path, SnesMovie *out, char *err, size_t err_len);
bool snes_movie_load_memory(const uint8_t *data, size_t size, SnesMovie *out, char *err, size_t err_len);
void snes_movie_free(SnesMovie *movie);

static inline uint16_t snes_movie_pad(const SnesMovie *m, uint32_t frame, unsigned port) {
    return (m != NULL && frame < m->frame_count && port < 2u) ? m->pads[frame * 2u + port] : 0u;
}

static inline uint8_t snes_movie_flags(const SnesMovie *m, uint32_t frame) {
    return (m != NULL && frame < m->frame_count) ? m->flags[frame] : 0u;
}

/* --- Native .c3m recorder --- */
typedef struct SnesMovieRecorder {
    void    *fp;
    uint32_t frames;
} SnesMovieRecorder;

bool snes_movie_record_begin(SnesMovieRecorder *rec, const char *path, const char *comment);
void snes_movie_record_frame(SnesMovieRecorder *rec, uint16_t pad1, uint16_t pad2, uint8_t flags);
void snes_movie_record_end(SnesMovieRecorder *rec);

/* --- Raw DEFLATE (RFC 1951) decoder used for .bk2 archives; exposed for tests --- */
/* Returns the number of bytes produced, or (size_t)-1 on a malformed stream / overflow. */
size_t snes_inflate(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_SNES_MOVIE_H */
