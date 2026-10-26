/*
 * Contra III: The Alien Wars (USA) - Input Movie Playback & Recording
 *
 * Portable C11 decoders for Snes9x (.smv), BizHawk (.bk2 zip / "Input Log.txt")
 * and the native .c3m text movie, plus a minimal raw DEFLATE decoder so .bk2
 * archives load with zero third-party dependencies.
 */

#include "snes_movie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define SNES_MOVIE_MAX_FILE   (64u * 1024u * 1024u)
#define SNES_MOVIE_MAX_FRAMES (16u * 1024u * 1024u)

/* SNES_BTN_* values (mirrors include/snes_ppu.h without pulling in the PPU) */
#define MV_BTN_B       0x8000u
#define MV_BTN_Y       0x4000u
#define MV_BTN_SELECT  0x2000u
#define MV_BTN_START   0x1000u
#define MV_BTN_UP      0x0800u
#define MV_BTN_DOWN    0x0400u
#define MV_BTN_LEFT    0x0200u
#define MV_BTN_RIGHT   0x0100u
#define MV_BTN_A       0x0080u
#define MV_BTN_X       0x0040u
#define MV_BTN_L       0x0020u
#define MV_BTN_R       0x0010u

static void set_err(char *err, size_t err_len, const char *fmt, ...) {
    if (err == NULL || err_len == 0u) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(err, err_len, fmt, ap);
    va_end(ap);
}

static inline uint32_t rd_le16(const uint8_t *p) { return (uint32_t)p[0] | ((uint32_t)p[1] << 8); }
static inline uint32_t rd_le32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

/* ------------------------------------------------------------------------- */
/* Growable frame storage                                                    */
/* ------------------------------------------------------------------------- */

typedef struct FrameBuf {
    uint16_t *pads;
    uint8_t  *flags;
    uint32_t  count;
    uint32_t  cap;
} FrameBuf;

static bool framebuf_push(FrameBuf *fb, uint16_t p1, uint16_t p2, uint8_t flags) {
    if (fb->count >= SNES_MOVIE_MAX_FRAMES) {
        return false;
    }
    if (fb->count == fb->cap) {
        uint32_t ncap = (fb->cap == 0u) ? 4096u : fb->cap * 2u;
        uint16_t *np = (uint16_t *)realloc(fb->pads, (size_t)ncap * 2u * sizeof(uint16_t));
        if (np == NULL) {
            return false;
        }
        fb->pads = np;
        uint8_t *nf = (uint8_t *)realloc(fb->flags, (size_t)ncap);
        if (nf == NULL) {
            return false;
        }
        fb->flags = nf;
        fb->cap = ncap;
    }
    fb->pads[fb->count * 2u + 0u] = p1;
    fb->pads[fb->count * 2u + 1u] = p2;
    fb->flags[fb->count] = flags;
    fb->count++;
    return true;
}

static void framebuf_free(FrameBuf *fb) {
    free(fb->pads);
    free(fb->flags);
    memset(fb, 0, sizeof(*fb));
}

static void movie_take_frames(SnesMovie *out, FrameBuf *fb) {
    out->pads = fb->pads;
    out->flags = fb->flags;
    out->frame_count = fb->count;
    memset(fb, 0, sizeof(*fb));
}

/* ------------------------------------------------------------------------- */
/* Raw DEFLATE decoder (RFC 1951)                                            */
/* ------------------------------------------------------------------------- */

typedef struct InflateState {
    const uint8_t *src;
    size_t         src_len;
    size_t         src_pos;
    uint8_t       *dst;
    size_t         dst_cap;
    size_t         dst_pos;
    uint32_t       bitbuf;
    unsigned       bitcnt;
    bool           err;
} InflateState;

typedef struct InflateHuff {
    short count[16];
    short symbol[288];
} InflateHuff;

static unsigned inf_bits(InflateState *s, unsigned need) {
    uint32_t val = s->bitbuf;
    while (s->bitcnt < need) {
        if (s->src_pos >= s->src_len) {
            s->err = true;
            return 0u;
        }
        val |= (uint32_t)s->src[s->src_pos++] << s->bitcnt;
        s->bitcnt += 8u;
    }
    s->bitbuf = (need >= 32u) ? 0u : (val >> need);
    s->bitcnt -= need;
    return (unsigned)(val & ((1u << need) - 1u));
}

static int inf_decode(InflateState *s, const InflateHuff *h) {
    int code = 0, first = 0, index = 0;
    for (int len = 1; len <= 15; ++len) {
        code |= (int)inf_bits(s, 1u);
        if (s->err) {
            return -1;
        }
        int count = h->count[len];
        if (code - count < first) {
            return h->symbol[index + (code - first)];
        }
        index += count;
        first += count;
        first <<= 1;
        code <<= 1;
    }
    return -1;
}

/* Returns 0 for a complete code, >0 for incomplete, <0 for over-subscribed */
static int inf_construct(InflateHuff *h, const short *length, int n) {
    short offs[16];
    memset(h->count, 0, sizeof(h->count));
    for (int sym = 0; sym < n; ++sym) {
        h->count[length[sym]]++;
    }
    if (h->count[0] == n) {
        return 0;
    }
    int left = 1;
    for (int len = 1; len <= 15; ++len) {
        left <<= 1;
        left -= h->count[len];
        if (left < 0) {
            return left;
        }
    }
    offs[1] = 0;
    for (int len = 1; len < 15; ++len) {
        offs[len + 1] = (short)(offs[len] + h->count[len]);
    }
    for (int sym = 0; sym < n; ++sym) {
        if (length[sym] != 0) {
            h->symbol[offs[length[sym]]++] = (short)sym;
        }
    }
    return left;
}

static bool inf_stored(InflateState *s) {
    s->bitbuf = 0u;
    s->bitcnt = 0u;
    if (s->src_pos + 4u > s->src_len) {
        return false;
    }
    unsigned len  = rd_le16(&s->src[s->src_pos]);
    unsigned nlen = rd_le16(&s->src[s->src_pos + 2u]);
    s->src_pos += 4u;
    if (len != (~nlen & 0xFFFFu) || s->src_pos + len > s->src_len || s->dst_pos + len > s->dst_cap) {
        return false;
    }
    memcpy(&s->dst[s->dst_pos], &s->src[s->src_pos], len);
    s->dst_pos += len;
    s->src_pos += len;
    return true;
}

static bool inf_codes(InflateState *s, const InflateHuff *lencode, const InflateHuff *distcode) {
    static const short lbase[29] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
                                     35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
    static const short lext[29]  = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
                                     3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
    static const short dbase[30] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
                                     257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
                                     8193, 12289, 16385, 24577 };
    static const short dext[30]  = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
                                     7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
    for (;;) {
        int sym = inf_decode(s, lencode);
        if (sym < 0 || s->err) {
            return false;
        }
        if (sym < 256) {
            if (s->dst_pos >= s->dst_cap) {
                return false;
            }
            s->dst[s->dst_pos++] = (uint8_t)sym;
        } else if (sym == 256) {
            return true;
        } else {
            sym -= 257;
            if (sym >= 29) {
                return false;
            }
            size_t len = (size_t)lbase[sym] + inf_bits(s, (unsigned)lext[sym]);
            int dsym = inf_decode(s, distcode);
            if (dsym < 0 || dsym >= 30 || s->err) {
                return false;
            }
            size_t dist = (size_t)dbase[dsym] + inf_bits(s, (unsigned)dext[dsym]);
            if (s->err || dist > s->dst_pos || s->dst_pos + len > s->dst_cap) {
                return false;
            }
            for (size_t i = 0u; i < len; ++i, ++s->dst_pos) {
                s->dst[s->dst_pos] = s->dst[s->dst_pos - dist];
            }
        }
    }
}

static bool inf_fixed(InflateState *s) {
    static InflateHuff lencode, distcode;
    static bool built = false;
    if (!built) {
        short lengths[288];
        int sym = 0;
        for (; sym < 144; ++sym) lengths[sym] = 8;
        for (; sym < 256; ++sym) lengths[sym] = 9;
        for (; sym < 280; ++sym) lengths[sym] = 7;
        for (; sym < 288; ++sym) lengths[sym] = 8;
        inf_construct(&lencode, lengths, 288);
        for (sym = 0; sym < 30; ++sym) lengths[sym] = 5;
        inf_construct(&distcode, lengths, 30);
        built = true;
    }
    return inf_codes(s, &lencode, &distcode);
}

static bool inf_dynamic(InflateState *s) {
    static const short order[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
    short lengths[320];
    InflateHuff lencode, distcode;

    int nlen  = (int)inf_bits(s, 5u) + 257;
    int ndist = (int)inf_bits(s, 5u) + 1;
    int ncode = (int)inf_bits(s, 4u) + 4;
    if (s->err || nlen > 286 || ndist > 30) {
        return false;
    }
    int index = 0;
    for (; index < ncode; ++index) {
        lengths[order[index]] = (short)inf_bits(s, 3u);
    }
    for (; index < 19; ++index) {
        lengths[order[index]] = 0;
    }
    if (s->err || inf_construct(&lencode, lengths, 19) != 0) {
        return false;
    }

    index = 0;
    while (index < nlen + ndist) {
        int sym = inf_decode(s, &lencode);
        if (sym < 0 || s->err) {
            return false;
        }
        if (sym < 16) {
            lengths[index++] = (short)sym;
        } else {
            short len = 0;
            if (sym == 16) {
                if (index == 0) {
                    return false;
                }
                len = lengths[index - 1];
                sym = 3 + (int)inf_bits(s, 2u);
            } else if (sym == 17) {
                sym = 3 + (int)inf_bits(s, 3u);
            } else {
                sym = 11 + (int)inf_bits(s, 7u);
            }
            if (s->err || index + sym > nlen + ndist) {
                return false;
            }
            while (sym-- > 0) {
                lengths[index++] = len;
            }
        }
    }
    if (lengths[256] == 0) {
        return false;
    }
    int rc = inf_construct(&lencode, lengths, nlen);
    if (rc < 0 || (rc > 0 && nlen != lencode.count[0] + lencode.count[1])) {
        return false;
    }
    rc = inf_construct(&distcode, lengths + nlen, ndist);
    if (rc < 0 || (rc > 0 && ndist != distcode.count[0] + distcode.count[1])) {
        return false;
    }
    return inf_codes(s, &lencode, &distcode);
}

size_t snes_inflate(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap) {
    InflateState s;
    memset(&s, 0, sizeof(s));
    s.src = src;
    s.src_len = src_len;
    s.dst = dst;
    s.dst_cap = dst_cap;

    unsigned last;
    do {
        last = inf_bits(&s, 1u);
        unsigned type = inf_bits(&s, 2u);
        if (s.err) {
            return (size_t)-1;
        }
        bool ok;
        switch (type) {
            case 0u: ok = inf_stored(&s); break;
            case 1u: ok = inf_fixed(&s); break;
            case 2u: ok = inf_dynamic(&s); break;
            default: ok = false; break;
        }
        if (!ok || s.err) {
            return (size_t)-1;
        }
    } while (!last);
    return s.dst_pos;
}

/* ------------------------------------------------------------------------- */
/* Zip archive member extraction (central directory driven)                  */
/* ------------------------------------------------------------------------- */

static uint8_t *zip_extract(const uint8_t *zip, size_t size, const char *name, size_t *out_len) {
    if (size < 22u) {
        return NULL;
    }
    size_t scan_floor = (size > 22u + 65535u) ? (size - 22u - 65535u) : 0u;
    size_t eocd = (size_t)-1;
    for (size_t pos = size - 22u + 1u; pos-- > scan_floor;) {
        if (rd_le32(&zip[pos]) == 0x06054B50u) {
            eocd = pos;
            break;
        }
    }
    if (eocd == (size_t)-1) {
        return NULL;
    }
    uint32_t entries = rd_le16(&zip[eocd + 10u]);
    size_t   cd_pos  = rd_le32(&zip[eocd + 16u]);
    size_t   name_len_req = strlen(name);

    for (uint32_t e = 0u; e < entries; ++e) {
        if (cd_pos + 46u > size || rd_le32(&zip[cd_pos]) != 0x02014B50u) {
            return NULL;
        }
        uint32_t method   = rd_le16(&zip[cd_pos + 10u]);
        size_t   csize    = rd_le32(&zip[cd_pos + 20u]);
        size_t   usize    = rd_le32(&zip[cd_pos + 24u]);
        size_t   nlen     = rd_le16(&zip[cd_pos + 28u]);
        size_t   xlen     = rd_le16(&zip[cd_pos + 30u]);
        size_t   clen     = rd_le16(&zip[cd_pos + 32u]);
        size_t   loc      = rd_le32(&zip[cd_pos + 42u]);
        if (cd_pos + 46u + nlen > size) {
            return NULL;
        }
        const char *ename = (const char *)&zip[cd_pos + 46u];
        cd_pos += 46u + nlen + xlen + clen;

        if (nlen != name_len_req || memcmp(ename, name, nlen) != 0) {
            continue;
        }
        if (loc + 30u > size || rd_le32(&zip[loc]) != 0x04034B50u) {
            return NULL;
        }
        size_t data = loc + 30u + rd_le16(&zip[loc + 26u]) + rd_le16(&zip[loc + 28u]);
        if (data + csize > size || usize > SNES_MOVIE_MAX_FILE) {
            return NULL;
        }
        uint8_t *out = (uint8_t *)malloc(usize + 1u);
        if (out == NULL) {
            return NULL;
        }
        if (method == 0u) {
            if (csize != usize) {
                free(out);
                return NULL;
            }
            memcpy(out, &zip[data], usize);
        } else if (method == 8u) {
            if (snes_inflate(&zip[data], csize, out, usize) != usize) {
                free(out);
                return NULL;
            }
        } else {
            free(out);
            return NULL;
        }
        out[usize] = 0u;
        *out_len = usize;
        return out;
    }
    return NULL;
}

/* ------------------------------------------------------------------------- */
/* BizHawk "Input Log.txt"                                                   */
/* ------------------------------------------------------------------------- */

#define BK2_MAX_GROUPS   16
#define BK2_MAX_BUTTONS  32

typedef struct Bk2Button {
    uint8_t  port;    /* 0 = P1, 1 = P2, 0xFF = ignored */
    uint16_t mask;
    uint8_t  flag;    /* SNES_MOVIE_FLAG_* for console buttons */
} Bk2Button;

typedef struct Bk2Layout {
    int       num_groups;
    int       num_buttons[BK2_MAX_GROUPS];
    Bk2Button buttons[BK2_MAX_GROUPS][BK2_MAX_BUTTONS];
} Bk2Layout;

static bool str_ieq(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return false;
        }
        ++a;
        ++b;
    }
    return *a == '\0' && *b == '\0';
}

static Bk2Button bk2_map_button(const char *name) {
    Bk2Button b = { 0xFFu, 0u, 0u };
    if (str_ieq(name, "Reset")) {
        b.flag = SNES_MOVIE_FLAG_RESET;
        return b;
    }
    if (str_ieq(name, "Power")) {
        b.flag = SNES_MOVIE_FLAG_POWER;
        return b;
    }
    if ((name[0] == 'P' || name[0] == 'p') && isdigit((unsigned char)name[1]) && name[2] == ' ') {
        unsigned port = (unsigned)(name[1] - '1');
        const char *btn = name + 3;
        static const struct { const char *n; uint16_t m; } map[] = {
            { "Up", MV_BTN_UP }, { "Down", MV_BTN_DOWN }, { "Left", MV_BTN_LEFT }, { "Right", MV_BTN_RIGHT },
            { "Select", MV_BTN_SELECT }, { "Start", MV_BTN_START },
            { "Y", MV_BTN_Y }, { "B", MV_BTN_B }, { "X", MV_BTN_X }, { "A", MV_BTN_A },
            { "L", MV_BTN_L }, { "R", MV_BTN_R }
        };
        for (size_t i = 0u; i < sizeof(map) / sizeof(map[0]); ++i) {
            if (str_ieq(btn, map[i].n)) {
                if (port < 2u) {
                    b.port = (uint8_t)port;
                    b.mask = map[i].m;
                }
                break;
            }
        }
    }
    return b;
}

static void bk2_default_layout(Bk2Layout *lay) {
    /* BizHawk SNES default: |Reset Power|P1 UDLRsSYBXAlr|P2 UDLRsSYBXAlr| */
    static const char *const pad[12] = { "Up", "Down", "Left", "Right", "Select", "Start",
                                          "Y", "B", "X", "A", "L", "R" };
    char name[16];
    memset(lay, 0, sizeof(*lay));
    lay->num_groups = 3;
    lay->num_buttons[0] = 2;
    lay->buttons[0][0] = bk2_map_button("Reset");
    lay->buttons[0][1] = bk2_map_button("Power");
    for (int p = 0; p < 2; ++p) {
        lay->num_buttons[p + 1] = 12;
        for (int i = 0; i < 12; ++i) {
            snprintf(name, sizeof(name), "P%d %s", p + 1, pad[i]);
            lay->buttons[p + 1][i] = bk2_map_button(name);
        }
    }
}

static void bk2_parse_logkey(Bk2Layout *lay, const char *key, size_t len) {
    memset(lay, 0, sizeof(*lay));
    int group = -1;
    size_t i = 0u;
    while (i < len) {
        if (key[i] == '#') {
            if (group + 1 >= BK2_MAX_GROUPS) {
                break;
            }
            group++;
            lay->num_groups = group + 1;
            i++;
            continue;
        }
        size_t start = i;
        while (i < len && key[i] != '|' && key[i] != '#') {
            i++;
        }
        if (i > start && group >= 0 && lay->num_buttons[group] < BK2_MAX_BUTTONS) {
            char name[64];
            size_t n = i - start;
            if (n >= sizeof(name)) {
                n = sizeof(name) - 1u;
            }
            memcpy(name, &key[start], n);
            name[n] = '\0';
            lay->buttons[group][lay->num_buttons[group]++] = bk2_map_button(name);
        }
        if (i < len && key[i] == '|') {
            i++;
        }
    }
}

static bool parse_bk2_log(const char *text, size_t len, FrameBuf *fb, char *err, size_t err_len) {
    Bk2Layout lay;
    bk2_default_layout(&lay);

    size_t pos = 0u;
    while (pos < len) {
        size_t eol = pos;
        while (eol < len && text[eol] != '\n') {
            eol++;
        }
        size_t line_end = eol;
        if (line_end > pos && text[line_end - 1u] == '\r') {
            line_end--;
        }
        const char *line = &text[pos];
        size_t llen = line_end - pos;

        if (llen >= 7u && memcmp(line, "LogKey:", 7u) == 0) {
            bk2_parse_logkey(&lay, line + 7, llen - 7u);
        } else if (llen > 0u && line[0] == '|') {
            uint16_t pads[2] = { 0u, 0u };
            uint8_t  flags   = 0u;
            int      group   = 0;
            int      btn     = 0;
            for (size_t c = 1u; c < llen; ++c) {
                char ch = line[c];
                if (ch == '|') {
                    group++;
                    btn = 0;
                    continue;
                }
                if (group < lay.num_groups && btn < lay.num_buttons[group]) {
                    if (ch != '.' && ch != ' ') {
                        const Bk2Button *b = &lay.buttons[group][btn];
                        if (b->port < 2u) {
                            pads[b->port] |= b->mask;
                        }
                        flags |= b->flag;
                    }
                }
                btn++;
            }
            if (!framebuf_push(fb, pads[0], pads[1], flags)) {
                set_err(err, err_len, "out of memory / too many frames in input log");
                return false;
            }
        }
        pos = eol + 1u;
    }
    if (fb->count == 0u) {
        set_err(err, err_len, "input log contains no frames");
        return false;
    }
    return true;
}

static void bk2_parse_header(const char *text, size_t len, SnesMovie *out, bool *from_state) {
    size_t pos = 0u;
    while (pos < len) {
        size_t eol = pos;
        while (eol < len && text[eol] != '\n') {
            eol++;
        }
        size_t line_end = eol;
        if (line_end > pos && text[line_end - 1u] == '\r') {
            line_end--;
        }
        const char *line = &text[pos];
        size_t sp = 0u;
        while (pos + sp < line_end && line[sp] != ' ') {
            sp++;
        }
        char key[48] = { 0 };
        char val[128] = { 0 };
        size_t kn = (sp < sizeof(key) - 1u) ? sp : sizeof(key) - 1u;
        memcpy(key, line, kn);
        if (pos + sp < line_end) {
            size_t vn = line_end - (pos + sp + 1u);
            if (vn >= sizeof(val)) {
                vn = sizeof(val) - 1u;
            }
            memcpy(val, line + sp + 1u, vn);
        }
        if (str_ieq(key, "StartsFromSavestate") && str_ieq(val, "True")) {
            *from_state = true;
        } else if (str_ieq(key, "rerecordCount")) {
            out->rerecords = (uint32_t)strtoul(val, NULL, 10);
        } else if (str_ieq(key, "Author")) {
            snprintf(out->author, sizeof(out->author), "%s", val);
        } else if (str_ieq(key, "GameName")) {
            snprintf(out->rom_name, sizeof(out->rom_name), "%s", val);
        }
        pos = eol + 1u;
    }
}

static bool load_bk2(const uint8_t *data, size_t size, SnesMovie *out, char *err, size_t err_len) {
    size_t log_len = 0u;
    uint8_t *log = zip_extract(data, size, "Input Log.txt", &log_len);
    if (log == NULL) {
        set_err(err, err_len, "bk2: could not extract 'Input Log.txt' (unsupported zip or corrupt archive)");
        return false;
    }
    size_t hdr_len = 0u;
    uint8_t *hdr = zip_extract(data, size, "Header.txt", &hdr_len);
    bool from_state = false;
    if (hdr != NULL) {
        bk2_parse_header((const char *)hdr, hdr_len, out, &from_state);
        free(hdr);
    }
    if (from_state) {
        free(log);
        set_err(err, err_len, "bk2: movie starts from a savestate; only power-on movies can be co-simulated");
        return false;
    }
    FrameBuf fb;
    memset(&fb, 0, sizeof(fb));
    bool ok = parse_bk2_log((const char *)log, log_len, &fb, err, err_len);
    free(log);
    if (!ok) {
        framebuf_free(&fb);
        return false;
    }
    out->format = SNES_MOVIE_BK2;
    movie_take_frames(out, &fb);
    return true;
}

/* ------------------------------------------------------------------------- */
/* Snes9x .smv                                                               */
/* ------------------------------------------------------------------------- */

static bool load_smv(const uint8_t *data, size_t size, SnesMovie *out, char *err, size_t err_len) {
    if (size < 32u) {
        set_err(err, err_len, "smv: truncated header");
        return false;
    }
    uint32_t version   = rd_le32(&data[4]);
    uint32_t rerecords = rd_le32(&data[12]);
    uint32_t frames    = rd_le32(&data[16]);
    uint8_t  ctl_mask  = data[20];
    uint8_t  options   = data[21];
    uint8_t  sync2     = data[23];
    uint32_t state_off = rd_le32(&data[24]);
    uint32_t input_off = rd_le32(&data[28]);

    if (version != 1u && version != 4u && version != 5u) {
        set_err(err, err_len, "smv: unsupported version %u", (unsigned)version);
        return false;
    }
    if ((options & 0x01u) == 0u) {
        set_err(err, err_len, "smv: movie starts from a savestate; only power-on/reset movies can be co-simulated");
        return false;
    }
    size_t hdr_size = (version == 1u) ? 32u : 64u;
    if (size < hdr_size || input_off > size || state_off > size) {
        set_err(err, err_len, "smv: corrupt offsets");
        return false;
    }

    uint32_t samples = frames + 1u;
    uint8_t  port_type[2] = { 1u, 1u };
    if (version >= 4u) {
        samples = rd_le32(&data[32]);
        port_type[0] = data[36];
        port_type[1] = data[37];
    }

    unsigned num_ctl = 0u;
    for (unsigned i = 0u; i < 8u; ++i) {
        if ((ctl_mask & (1u << i)) != 0u) {
            num_ctl++;
        }
    }
    size_t bytes = 2u * num_ctl;
    if (version >= 5u) {
        for (unsigned p = 0u; p < 2u; ++p) {
            if (port_type[p] == 2u)      bytes += 5u;   /* Mouse */
            else if (port_type[p] == 3u) bytes += 6u;   /* Super Scope */
            else if (port_type[p] == 4u) bytes += 11u;  /* Justifier */
        }
    }
    if (bytes == 0u) {
        set_err(err, err_len, "smv: no controllers enabled");
        return false;
    }
    size_t avail = (size - input_off) / bytes;
    if (samples > avail) {
        samples = (uint32_t)avail;
    }

    /* Author metadata (UTF-16LE) between the header and the ROM info / savestate */
    size_t meta_end = state_off;
    if ((sync2 & 0x40u) != 0u && meta_end >= hdr_size + 30u) {
        size_t rominfo = meta_end - 30u;
        size_t n = 0u;
        for (size_t i = 0u; i < 23u && data[rominfo + 7u + i] != 0u && n + 1u < sizeof(out->rom_name); ++i) {
            out->rom_name[n++] = (char)data[rominfo + 7u + i];
        }
        out->rom_name[n] = '\0';
        meta_end = rominfo;
    }
    if (meta_end > hdr_size && meta_end <= size) {
        size_t n = 0u;
        for (size_t i = hdr_size; i + 1u < meta_end && n + 1u < sizeof(out->author); i += 2u) {
            uint32_t ch = rd_le16(&data[i]);
            if (ch == 0u) {
                break;
            }
            out->author[n++] = (ch < 0x80u) ? (char)ch : '?';
        }
        out->author[n] = '\0';
    }

    FrameBuf fb;
    memset(&fb, 0, sizeof(fb));
    for (uint32_t s = 0u; s < samples; ++s) {
        const uint8_t *p = &data[input_off + (size_t)s * bytes];
        uint16_t pads[2] = { 0u, 0u };
        bool all_ff = true;
        unsigned slot = 0u;
        for (unsigned c = 0u; c < 8u; ++c) {
            if ((ctl_mask & (1u << c)) == 0u) {
                continue;
            }
            uint16_t v = (uint16_t)rd_le16(&p[slot * 2u]);
            if (v != 0xFFFFu) {
                all_ff = false;
            }
            if (c < 2u) {
                pads[c] = (uint16_t)(v & 0xFFF0u);
            }
            slot++;
        }
        uint8_t flags = 0u;
        if (all_ff) {
            flags = SNES_MOVIE_FLAG_RESET;
            pads[0] = pads[1] = 0u;
        }
        if (!framebuf_push(&fb, pads[0], pads[1], flags)) {
            framebuf_free(&fb);
            set_err(err, err_len, "smv: out of memory");
            return false;
        }
    }
    if (fb.count == 0u) {
        framebuf_free(&fb);
        set_err(err, err_len, "smv: movie contains no input samples");
        return false;
    }
    out->format = SNES_MOVIE_SMV;
    out->rerecords = rerecords;
    out->pal = (options & 0x02u) != 0u;
    movie_take_frames(out, &fb);
    return true;
}

/* ------------------------------------------------------------------------- */
/* Native .c3m text movie                                                    */
/* ------------------------------------------------------------------------- */

static bool load_c3m(const char *text, size_t len, SnesMovie *out, char *err, size_t err_len) {
    FrameBuf fb;
    memset(&fb, 0, sizeof(fb));
    size_t pos = 0u;
    unsigned line_no = 0u;
    while (pos < len) {
        size_t eol = pos;
        while (eol < len && text[eol] != '\n') {
            eol++;
        }
        char line[128];
        size_t n = eol - pos;
        if (n >= sizeof(line)) {
            n = sizeof(line) - 1u;
        }
        memcpy(line, &text[pos], n);
        line[n] = '\0';
        pos = eol + 1u;
        line_no++;

        if (line_no == 1u) {
            if (strncmp(line, "C3M", 3u) != 0) {
                framebuf_free(&fb);
                set_err(err, err_len, "c3m: missing 'C3M' header");
                return false;
            }
            continue;
        }
        if (line[0] == '#') {
            if (strncmp(line, "# author ", 9u) == 0) {
                snprintf(out->author, sizeof(out->author), "%s", line + 9);
                size_t al = strlen(out->author);
                if (al > 0u && out->author[al - 1u] == '\r') {
                    out->author[al - 1u] = '\0';
                }
            }
            continue;
        }
        unsigned p1 = 0u, p2 = 0u, fl = 0u;
        int got = sscanf(line, "%x %x %x", &p1, &p2, &fl);
        if (got < 2) {
            continue;
        }
        if (!framebuf_push(&fb, (uint16_t)p1, (uint16_t)p2, (uint8_t)((got >= 3) ? fl : 0u))) {
            framebuf_free(&fb);
            set_err(err, err_len, "c3m: out of memory");
            return false;
        }
    }
    if (fb.count == 0u) {
        framebuf_free(&fb);
        set_err(err, err_len, "c3m: movie contains no frames");
        return false;
    }
    out->format = SNES_MOVIE_C3M;
    movie_take_frames(out, &fb);
    return true;
}

/* ------------------------------------------------------------------------- */
/* Public API                                                                */
/* ------------------------------------------------------------------------- */

bool snes_movie_load_memory(const uint8_t *data, size_t size, SnesMovie *out, char *err, size_t err_len) {
    if (out == NULL) {
        return false;
    }
    memset(out, 0, sizeof(*out));
    if (data == NULL || size < 4u) {
        set_err(err, err_len, "movie is empty");
        return false;
    }
    if (memcmp(data, "SMV\x1A", 4u) == 0) {
        return load_smv(data, size, out, err, err_len);
    }
    if (rd_le32(data) == 0x04034B50u) {
        return load_bk2(data, size, out, err, err_len);
    }
    if (memcmp(data, "C3M", 3u) == 0) {
        return load_c3m((const char *)data, size, out, err, err_len);
    }
    /* Bare BizHawk Input Log ("[Input]" / "LogKey:" / "|..." lines) */
    size_t probe = (size < 4096u) ? size : 4096u;
    for (size_t i = 0u; i + 7u <= probe; ++i) {
        if (memcmp(&data[i], "LogKey:", 7u) == 0 || memcmp(&data[i], "[Input]", 7u) == 0) {
            FrameBuf fb;
            memset(&fb, 0, sizeof(fb));
            if (!parse_bk2_log((const char *)data, size, &fb, err, err_len)) {
                framebuf_free(&fb);
                return false;
            }
            out->format = SNES_MOVIE_BK2_LOG;
            movie_take_frames(out, &fb);
            return true;
        }
    }
    set_err(err, err_len, "unrecognized movie format (expected .smv, .bk2, Input Log.txt or .c3m)");
    return false;
}

bool snes_movie_load(const char *path, SnesMovie *out, char *err, size_t err_len) {
    if (out != NULL) {
        memset(out, 0, sizeof(*out));
    }
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        set_err(err, err_len, "cannot open '%s'", path);
        return false;
    }
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        set_err(err, err_len, "cannot seek '%s'", path);
        return false;
    }
    long sz = ftell(fp);
    if (sz <= 0 || (unsigned long)sz > SNES_MOVIE_MAX_FILE || fseek(fp, 0, SEEK_SET) != 0) {
        fclose(fp);
        set_err(err, err_len, "'%s' is empty or larger than 64 MB", path);
        return false;
    }
    uint8_t *buf = (uint8_t *)malloc((size_t)sz + 1u);
    if (buf == NULL) {
        fclose(fp);
        set_err(err, err_len, "out of memory");
        return false;
    }
    size_t got = fread(buf, 1u, (size_t)sz, fp);
    fclose(fp);
    if (got != (size_t)sz) {
        free(buf);
        set_err(err, err_len, "short read on '%s'", path);
        return false;
    }
    buf[sz] = 0u;
    bool ok = snes_movie_load_memory(buf, (size_t)sz, out, err, err_len);
    free(buf);
    return ok;
}

void snes_movie_free(SnesMovie *movie) {
    if (movie == NULL) {
        return;
    }
    free(movie->pads);
    free(movie->flags);
    memset(movie, 0, sizeof(*movie));
}

bool snes_movie_record_begin(SnesMovieRecorder *rec, const char *path, const char *comment) {
    memset(rec, 0, sizeof(*rec));
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        return false;
    }
    fprintf(fp, "C3M 1\n");
    fprintf(fp, "# Contra III: The Alien Wars (USA) static recompilation input movie\n");
    fprintf(fp, "# columns: P1 P2 [flags]  (hex $4218 masks; flags 1=reset 2=power)\n");
    if (comment != NULL && comment[0] != '\0') {
        fprintf(fp, "# %s\n", comment);
    }
    rec->fp = fp;
    return true;
}

void snes_movie_record_frame(SnesMovieRecorder *rec, uint16_t pad1, uint16_t pad2, uint8_t flags) {
    if (rec == NULL || rec->fp == NULL) {
        return;
    }
    if (flags != 0u) {
        fprintf((FILE *)rec->fp, "%04X %04X %X\n", pad1, pad2, flags);
    } else {
        fprintf((FILE *)rec->fp, "%04X %04X\n", pad1, pad2);
    }
    rec->frames++;
}

void snes_movie_record_end(SnesMovieRecorder *rec) {
    if (rec != NULL && rec->fp != NULL) {
        fprintf((FILE *)rec->fp, "# end (%u frames)\n", (unsigned)rec->frames);
        fclose((FILE *)rec->fp);
        rec->fp = NULL;
    }
}
