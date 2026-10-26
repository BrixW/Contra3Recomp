/*
 * Sony S-SMP (SPC700) CPU + 64-Byte IPL Boot ROM + Sony S-DSP 32 kHz Stereo Synthesizer
 * Portable zero-dependency C11 implementation for Contra III static recompilation.
 */

#include <stdio.h>
#include <string.h>
#include "snes_apu.h"
#include "cpu_65816.h"

/* SPC700 Processor Status Word (PSW) bit masks */
#define SPC_PSW_N  0x80u
#define SPC_PSW_V  0x40u
#define SPC_PSW_P  0x20u
#define SPC_PSW_B  0x10u
#define SPC_PSW_H  0x08u
#define SPC_PSW_I  0x04u
#define SPC_PSW_Z  0x02u
#define SPC_PSW_C  0x01u

/* Authentic 64-byte Nintendo/Sony SPC700 IPL Boot ROM ($FFC0-$FFFF) */
static const uint8_t k_spc_ipl_rom[64] = {
    0xCDu, 0xEFu,               /* $FFC0: MOV X, #$EF       */
    0xBDu,                      /* $FFC2: MOV SP, X         */
    0xE8u, 0x00u,               /* $FFC3: MOV A, #$00       */
    0xC6u,                      /* $FFC5: MOV (X), A        */
    0x1Du,                      /* $FFC6: DEC X             */
    0xD0u, 0xFCu,               /* $FFC7: BNE $FFC5         */
    0x8Fu, 0xAAu, 0xF4u,        /* $FFC9: MOV $F4, #$AA     */
    0x8Fu, 0xBBu, 0xF5u,        /* $FFCC: MOV $F5, #$BB     */
    0x78u, 0xCCu, 0xF4u,        /* $FFCF: CMP $F4, #$CC     */
    0xD0u, 0xFBu,               /* $FFD2: BNE $FFCF         */
    0x2Fu, 0x19u,               /* $FFD4: BRA $FFEF         */
    0xEBu, 0xF4u,               /* $FFD6: MOV Y, $F4        */
    0xD0u, 0xFCu,               /* $FFD8: BNE $FFD6         */
    0x7Eu, 0xF4u,               /* $FFDA: CMP Y, $F4        */
    0xD0u, 0x0Bu,               /* $FFDC: BNE $FFE9         */
    0xE4u, 0xF5u,               /* $FFDE: MOV A, $F5        */
    0xCBu, 0xF4u,               /* $FFE0: MOV $F4, Y        */
    0xD7u, 0x00u,               /* $FFE2: MOV [$00]+Y, A    */
    0xFCu,                      /* $FFE4: INC Y             */
    0xD0u, 0xF3u,               /* $FFE5: BNE $FFDA         */
    0xABu, 0x01u,               /* $FFE7: INC $01           */
    0x10u, 0xEFu,               /* $FFE9: BPL $FFDA         */
    0x7Eu, 0xF4u,               /* $FFEB: CMP Y, $F4        */
    0x10u, 0xEBu,               /* $FFED: BPL $FFDA         */
    0xBAu, 0xF6u,               /* $FFEF: MOVW YA, $F6      */
    0xDAu, 0x00u,               /* $FFF1: MOVW $00, YA      */
    0xBAu, 0xF4u,               /* $FFF3: MOVW YA, $F4      */
    0xC4u, 0xF4u,               /* $FFF5: MOV $F4, A        */
    0xDDu,                      /* $FFF7: MOV A, Y          */
    0x5Du,                      /* $FFF8: MOV X, A          */
    0xD0u, 0xDBu,               /* $FFF9: BNE $FFD6         */
    0x1Fu, 0x00u, 0x00u,        /* $FFFB: JMP [$0000+X]     */
    0xC0u, 0xFFu                /* $FFFE: Reset Vector $FFC0*/
};

/* Sony S-DSP 32-entry envelope & noise rate period table (all divide 30720) */
static const uint16_t k_dsp_rate_period[32] = {
       0u, 2048u, 1536u, 1280u, 1024u,  768u,  640u,  512u,
     384u,  320u,  256u,  192u,  160u,  128u,   96u,   80u,
      64u,   48u,   40u,   32u,   24u,   20u,   16u,   12u,
      10u,    8u,    6u,    5u,    4u,    3u,    2u,    1u
};

static const uint16_t k_dsp_rate_offset[32] = {
       0u,    0u, 1040u,  536u,    0u, 1040u,  536u,    0u,
    1040u,  536u,    0u, 1040u,  536u,    0u, 1040u,  536u,
       0u, 1040u,  536u,    0u, 1040u,  536u,    0u, 1040u,
     536u,    0u, 1040u,  536u,    0u, 1040u,    0u,    0u
};

/* Normalized 512-entry 4-tap Sony S-DSP Gaussian interpolation table */
static const int16_t k_dsp_gauss[512] = {
       0,    0,    0,    0,    0,    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,    1,
       1,    2,    2,    2,    2,    2,    2,    2,    3,    3,    3,    3,    3,    3,    3,    4,
       4,    4,    4,    4,    5,    5,    5,    5,    5,    6,    6,    6,    6,    6,    7,    7,
       7,    7,    8,    8,    8,    8,    9,    9,    9,   10,   10,   10,   11,   11,   11,   12,
      12,   12,   13,   13,   13,   14,   14,   15,   15,   16,   16,   16,   17,   17,   18,   18,
      19,   19,   20,   20,   21,   21,   22,   22,   23,   23,   24,   25,   25,   26,   26,   27,
      28,   28,   29,   30,   30,   31,   32,   32,   33,   34,   35,   35,   36,   37,   38,   39,
      39,   40,   41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
      55,   56,   57,   58,   59,   60,   61,   62,   64,   65,   66,   67,   69,   70,   71,   72,
      74,   75,   76,   78,   79,   81,   82,   83,   85,   86,   88,   89,   91,   93,   94,   96,
      97,   99,  101,  102,  104,  106,  107,  109,  111,  113,  115,  116,  118,  120,  122,  124,
     126,  128,  130,  132,  134,  136,  138,  140,  142,  144,  147,  149,  151,  153,  156,  158,
     160,  162,  165,  167,  170,  172,  175,  177,  179,  182,  185,  187,  190,  192,  195,  198,
     200,  203,  206,  209,  211,  214,  217,  220,  223,  226,  229,  232,  235,  238,  241,  244,
     247,  250,  253,  256,  259,  263,  266,  269,  272,  276,  279,  282,  286,  289,  293,  296,
     300,  303,  307,  310,  314,  317,  321,  325,  328,  332,  336,  340,  343,  347,  351,  355,
     359,  363,  366,  370,  374,  378,  382,  386,  390,  394,  399,  403,  407,  411,  415,  419,
     423,  428,  432,  436,  441,  445,  449,  454,  458,  462,  467,  471,  476,  480,  485,  489,
     494,  498,  503,  508,  512,  517,  521,  526,  531,  535,  540,  545,  550,  554,  559,  564,
     569,  574,  578,  583,  588,  593,  598,  603,  608,  613,  618,  623,  628,  633,  638,  643,
     648,  653,  658,  663,  668,  673,  678,  683,  688,  693,  698,  703,  708,  713,  719,  724,
     729,  734,  739,  744,  749,  754,  760,  765,  770,  775,  780,  785,  790,  796,  801,  806,
     811,  816,  821,  826,  832,  837,  842,  847,  852,  857,  862,  867,  872,  878,  883,  888,
     893,  898,  903,  908,  913,  918,  923,  928,  933,  938,  943,  948,  953,  958,  963,  967,
     972,  976,  981,  986,  991,  996, 1001, 1006, 1010, 1015, 1020, 1025, 1029, 1034, 1039, 1044,
    1047, 1052, 1057, 1062, 1067, 1070, 1075, 1080, 1084, 1088, 1092, 1097, 1101, 1105, 1110, 1113,
    1118, 1122, 1125, 1131, 1134, 1138, 1143, 1146, 1150, 1153, 1158, 1162, 1166, 1169, 1173, 1176,
    1180, 1183, 1188, 1191, 1195, 1198, 1201, 1205, 1208, 1212, 1214, 1218, 1221, 1224, 1227, 1230,
    1233, 1237, 1239, 1242, 1245, 1248, 1250, 1254, 1257, 1259, 1262, 1265, 1267, 1270, 1272, 1274,
    1277, 1279, 1282, 1283, 1286, 1288, 1290, 1292, 1294, 1296, 1297, 1299, 1301, 1303, 1305, 1306,
    1308, 1310, 1312, 1313, 1315, 1315, 1317, 1318, 1320, 1321, 1322, 1323, 1324, 1325, 1325, 1328,
    1328, 1329, 1329, 1330, 1330, 1331, 1332, 1332, 1333, 1334, 1334, 1334, 1335, 1335, 1334, 1334
};

static inline int16_t sclamp16(int32_t v) {
    if (v > 32767)  return 32767;
    if (v < -32768) return -32768;
    return (int16_t)v;
}

static inline bool dsp_rate_fires(uint16_t counter, uint8_t rate) {
    rate &= 0x1Fu;
    if (rate == 0u) {
        return false;
    }
    uint16_t period = k_dsp_rate_period[rate];
    return ((uint32_t)counter + (uint32_t)k_dsp_rate_offset[rate]) % (uint32_t)period == 0u;
}

/* ============================================================================
 * Sony S-DSP Register Access & 32 kHz Stereo Sample Synthesis
 * ============================================================================ */
static void dsp_reset(SnesDsp *dsp) {
    memset(dsp, 0, sizeof(*dsp));
    dsp->regs[0x6Cu] = 0xE0u; /* Reset + Mute + Disable Echo RAM Writes on boot */
    dsp->noise_sample = 0x4000;
}

static void dsp_write_reg(SnesApu *apu, uint8_t reg, uint8_t val) {
    if ((reg & 0x80u) != 0u) {
        return;
    }
    SnesDsp *dsp = &apu->dsp;
    dsp->regs[reg] = val;
    if (reg == 0x4Cu) {
        dsp->new_kon |= val;
    } else if (reg == 0x7Cu) {
        /* Any write to ENDX clears all 8 voice end flags */
        dsp->regs[0x7Cu] = 0u;
    }
}

static void dsp_decode_brr_block(SnesApu *apu, unsigned v_idx) {
    SnesDsp *dsp = &apu->dsp;
    SnesDspVoice *v = &dsp->voices[v_idx];
    uint8_t hdr = apu->ram[v->brr_addr];
    v->brr_header = hdr;

    unsigned range  = (unsigned)(hdr >> 4) & 0x0Fu;
    unsigned filter = (unsigned)(hdr >> 2) & 0x03u;
    int32_t p1 = (int32_t)v->p1;
    int32_t p2 = (int32_t)v->p2;

    for (unsigned i = 0u; i < 8u; ++i) {
        uint8_t byte = apu->ram[(uint16_t)(v->brr_addr + 1u + i)];
        for (unsigned nib_idx = 0u; nib_idx < 2u; ++nib_idx) {
            int32_t nib = (nib_idx == 0u) ? (int32_t)(int8_t)(byte & 0xF0u) >> 4
                                          : (int32_t)(int8_t)((byte & 0x0Fu) << 4) >> 4;
            int32_t s;
            if (range <= 12u) {
                s = (nib << range) >> 1;
            } else {
                s = (nib < 0) ? -2048 : 0;
            }

            switch (filter) {
                case 0u:
                    break;
                case 1u:
                    s += p1 + ((-p1) >> 4);
                    break;
                case 2u:
                    s += (p1 << 1) + ((-p1 * 3) >> 5) - p2 + (p2 >> 4);
                    break;
                default:
                    s += (p1 << 1) + ((-p1 * 13) >> 6) - p2 + ((p2 * 3) >> 4);
                    break;
            }

            /* Hardware 16-bit clamp followed by 15-bit sign wrap */
            int16_t clamped = sclamp16(s);
            int16_t out15   = (int16_t)((int16_t)((uint16_t)clamped << 1) >> 1);
            p2 = p1;
            p1 = (int32_t)out15;
            v->brr_block[i * 2u + nib_idx] = (int16_t)((uint16_t)out15 << 1);
        }
    }

    v->p1 = (int16_t)p1;
    v->p2 = (int16_t)p2;
    v->brr_offset = 0u;
}

static void dsp_step_envelope(SnesDsp *dsp, unsigned v_idx) {
    SnesDspVoice *v = &dsp->voices[v_idx];
    if (v->env_mode == SNES_ENV_RELEASE) {
        v->env -= 8;
        if (v->env <= 0) {
            v->env = 0;
        }
        return;
    }

    uint8_t adsr1 = dsp->regs[(v_idx << 4) + 5u];
    int32_t env = (int32_t)v->env;
    uint8_t rate = 0u;

    if ((adsr1 & 0x80u) != 0u) {
        /* ADSR mode */
        uint8_t adsr2 = dsp->regs[(v_idx << 4) + 6u];
        if (v->env_mode == SNES_ENV_ATTACK) {
            rate = (uint8_t)(((adsr1 & 0x0Fu) << 1) + 1u);
            if (dsp_rate_fires(dsp->rate_counter, rate)) {
                env += (rate == 31u) ? 1024 : 32;
                if (env >= 0x07FF) {
                    env = 0x07FF;
                    v->env_mode = SNES_ENV_DECAY;
                }
            }
        } else if (v->env_mode == SNES_ENV_DECAY) {
            rate = (uint8_t)((((adsr1 >> 4) & 0x07u) << 1) + 16u);
            if (dsp_rate_fires(dsp->rate_counter, rate)) {
                env -= ((env - 1) >> 8) + 1;
            }
            if ((env >> 8) == (int32_t)((adsr2 >> 5) & 0x07u)) {
                v->env_mode = SNES_ENV_SUSTAIN;
            }
        } else {
            rate = (uint8_t)(adsr2 & 0x1Fu);
            if (dsp_rate_fires(dsp->rate_counter, rate)) {
                env -= ((env - 1) >> 8) + 1;
            }
        }
    } else {
        /* GAIN mode */
        uint8_t gain = dsp->regs[(v_idx << 4) + 7u];
        if ((gain & 0x80u) == 0u) {
            env = (int32_t)(gain & 0x7Fu) << 4;
        } else {
            rate = (uint8_t)(gain & 0x1Fu);
            if (dsp_rate_fires(dsp->rate_counter, rate)) {
                switch ((gain >> 5) & 0x03u) {
                    case 0u: env -= 32; break; /* Linear decrease */
                    case 1u: env -= ((env - 1) >> 8) + 1; break; /* Exponential decrease */
                    case 2u: env += 32; break; /* Linear increase */
                    default: env += (env < 0x0600) ? 32 : 8; break; /* Bent-line increase */
                }
            }
        }
    }

    if (env < 0)      env = 0;
    if (env > 0x07FF) env = 0x07FF;
    v->env = (int16_t)env;
}

static void dsp_tick_sample(SnesApu *apu, int16_t *out_l, int16_t *out_r) {
    SnesDsp *dsp = &apu->dsp;
    uint8_t flg = dsp->regs[0x6Cu];

    /* Soft reset bit ($6C bit 7) forces all voices into immediate release */
    if ((flg & 0x80u) != 0u) {
        for (unsigned v = 0u; v < 8u; ++v) {
            dsp->voices[v].env = 0;
            dsp->voices[v].env_mode = SNES_ENV_RELEASE;
        }
    }

    /* Advance global 30720-sample envelope/noise rate counter */
    dsp->rate_counter = (uint16_t)((dsp->rate_counter + 1u) % 30720u);

    /* Update LFSR white noise generator */
    if (dsp_rate_fires(dsp->rate_counter, (uint8_t)(flg & 0x1Fu))) {
        uint16_t lfsr = (uint16_t)dsp->noise_sample;
        uint16_t fb   = (uint16_t)((lfsr ^ (lfsr >> 1)) & 1u);
        lfsr = (uint16_t)(((lfsr >> 1) & 0x3FFFu) | (fb << 14));
        dsp->noise_sample = (int16_t)((int16_t)((uint16_t)lfsr << 1) >> 1);
    }

    /* Poll KON / KOF every 2 samples matching Sony S-DSP hardware pipeline */
    if ((dsp->sample_index & 1u) == 0u) {
        uint8_t kon = dsp->new_kon;
        uint8_t kof = dsp->regs[0x5Cu];
        dsp->new_kon = 0u;

        for (unsigned i = 0u; i < 8u; ++i) {
            SnesDspVoice *v = &dsp->voices[i];
            uint8_t bit = (uint8_t)(1u << i);
            if ((kon & bit) != 0u) {
                uint8_t srcn = dsp->regs[(i << 4) + 4u];
                uint16_t dir_entry = (uint16_t)(((uint16_t)dsp->regs[0x5Du] << 8) + ((uint16_t)srcn << 2));
                v->brr_addr = (uint16_t)(apu->ram[dir_entry] | ((uint16_t)apu->ram[(uint16_t)(dir_entry + 1u)] << 8));
                v->brr_offset = 0u;
                v->p1 = 0;
                v->p2 = 0;
                memset(v->buf, 0, sizeof(v->buf));
                v->interp = 0u;
                v->env = 0;
                v->env_mode = SNES_ENV_ATTACK;
                v->kon_delay = 5u;
                v->kon_init = true;
                dsp->regs[0x7Cu] &= (uint8_t)(~bit);
                apu->kon_events++;
            } else if (v->kon_delay == 0u && (kof & bit) != 0u) {
                v->env_mode = SNES_ENV_RELEASE;
            }
        }
    }
    dsp->sample_index++;

    uint8_t pmon = dsp->regs[0x2Du];
    uint8_t non  = dsp->regs[0x3Du];
    uint8_t eon  = dsp->regs[0x4Du];

    int32_t main_l = 0, main_r = 0;
    int32_t echo_in_l = 0, echo_in_r = 0;

    for (unsigned i = 0u; i < 8u; ++i) {
        SnesDspVoice *v = &dsp->voices[i];
        uint8_t vbase = (uint8_t)(i << 4);
        uint8_t bit   = (uint8_t)(1u << i);

        if (v->kon_delay > 0u) {
            v->kon_delay--;
            v->prev_out = 0;
            dsp->regs[vbase + 8u] = 0u;
            dsp->regs[vbase + 9u] = 0u;
            continue;
        }

        /* Preload initial 4 BRR samples once right after KON 5-sample startup delay */
        if (v->kon_init) {
            dsp_decode_brr_block(apu, i);
            v->buf[0] = v->brr_block[0];
            v->buf[1] = v->brr_block[1];
            v->buf[2] = v->brr_block[2];
            v->buf[3] = v->brr_block[3];
            v->brr_offset = 4u;
            v->interp = 0u;
            v->kon_init = false;
        }

        /* 4-tap Gaussian interpolation or white noise replacement */
        int32_t sample;
        if ((non & bit) != 0u) {
            sample = (int32_t)(int16_t)((uint16_t)dsp->noise_sample << 1);
        } else {
            unsigned g_idx = (v->interp >> 4) & 0xFFu;
            int32_t g = ((int32_t)k_dsp_gauss[255u - g_idx] * (int32_t)v->buf[0]) >> 11;
            g += ((int32_t)k_dsp_gauss[511u - g_idx] * (int32_t)v->buf[1]) >> 11;
            g += ((int32_t)k_dsp_gauss[256u + g_idx] * (int32_t)v->buf[2]) >> 11;
            g = (int32_t)(int16_t)g;
            g += ((int32_t)k_dsp_gauss[g_idx] * (int32_t)v->buf[3]) >> 11;
            sample = (int32_t)sclamp16(g) & ~1;
        }

        /* Envelope step & scaling */
        dsp_step_envelope(dsp, i);
        int32_t env_out = ((sample * (int32_t)v->env) >> 11) & ~1;
        v->prev_out = (int16_t)env_out;
        dsp->regs[vbase + 8u] = (uint8_t)(v->env >> 4);
        dsp->regs[vbase + 9u] = (uint8_t)(env_out >> 8);

        /* Stereo voice pan & echo send */
        int32_t vl = (int8_t)dsp->regs[vbase + 0u];
        int32_t vr = (int8_t)dsp->regs[vbase + 1u];
        int32_t out_vl = (env_out * vl) >> 7;
        int32_t out_vr = (env_out * vr) >> 7;

        main_l = (int32_t)sclamp16(main_l + out_vl);
        main_r = (int32_t)sclamp16(main_r + out_vr);
        if ((eon & bit) != 0u) {
            echo_in_l = (int32_t)sclamp16(echo_in_l + out_vl);
            echo_in_r = (int32_t)sclamp16(echo_in_r + out_vr);
        }

        /* Advance pitch phase (with optional PMON from voice i-1) */
        int32_t pitch = (int32_t)(dsp->regs[vbase + 2u] | ((uint16_t)(dsp->regs[vbase + 3u] & 0x3Fu) << 8));
        if (i > 0u && (pmon & bit) != 0u) {
            pitch += (((int32_t)dsp->voices[i - 1u].prev_out >> 5) * pitch) >> 10;
            if (pitch < 0)      pitch = 0;
            if (pitch > 0x3FFF) pitch = 0x3FFF;
        }

        uint32_t next_interp = (uint32_t)v->interp + (uint32_t)pitch;
        if (next_interp > 0x7FFFu) {
            next_interp = 0x7FFFu;
        }
        while (next_interp >= 0x1000u) {
            next_interp -= 0x1000u;
            if (v->brr_offset >= 16u) {
                if ((v->brr_header & 0x01u) != 0u) {
                    dsp->regs[0x7Cu] |= bit;
                    if ((v->brr_header & 0x02u) != 0u) {
                        uint8_t srcn = dsp->regs[vbase + 4u];
                        uint16_t dir_loop = (uint16_t)(((uint16_t)dsp->regs[0x5Du] << 8) + ((uint16_t)srcn << 2) + 2u);
                        v->brr_addr = (uint16_t)(apu->ram[dir_loop] | ((uint16_t)apu->ram[(uint16_t)(dir_loop + 1u)] << 8));
                        dsp_decode_brr_block(apu, i);
                    } else {
                        v->env = 0;
                        v->env_mode = SNES_ENV_RELEASE;
                        memset(v->brr_block, 0, sizeof(v->brr_block));
                        v->brr_offset = 0u;
                    }
                } else {
                    v->brr_addr = (uint16_t)(v->brr_addr + 9u);
                    dsp_decode_brr_block(apu, i);
                }
            }
            v->buf[0] = v->buf[1];
            v->buf[1] = v->buf[2];
            v->buf[2] = v->buf[3];
            v->buf[3] = v->brr_block[v->brr_offset++];
        }
        v->interp = (uint16_t)next_interp;
    }

    /* Echo buffer read & 8-tap FIR filter */
    uint16_t echo_addr = (uint16_t)(((uint16_t)dsp->regs[0x6Du] << 8) + dsp->echo_offset);
    int16_t echo_rd_l = (int16_t)((uint16_t)(apu->ram[echo_addr] | ((uint16_t)apu->ram[(uint16_t)(echo_addr + 1u)] << 8)) & 0xFFFEu);
    int16_t echo_rd_r = (int16_t)((uint16_t)(apu->ram[(uint16_t)(echo_addr + 2u)] | ((uint16_t)apu->ram[(uint16_t)(echo_addr + 3u)] << 8)) & 0xFFFEu);

    dsp->fir_l[dsp->fir_pos] = (int16_t)(echo_rd_l >> 1);
    dsp->fir_r[dsp->fir_pos] = (int16_t)(echo_rd_r >> 1);

    int32_t fir_sum_l = 0, fir_sum_r = 0;
    for (unsigned t = 0u; t < 7u; ++t) {
        unsigned idx = (dsp->fir_pos + 1u + t) & 7u;
        int32_t c = (int8_t)dsp->regs[(t << 4) + 0x0Fu];
        fir_sum_l += ((int32_t)dsp->fir_l[idx] * c) >> 6;
        fir_sum_r += ((int32_t)dsp->fir_r[idx] * c) >> 6;
    }
    fir_sum_l = (int32_t)(int16_t)fir_sum_l;
    fir_sum_r = (int32_t)(int16_t)fir_sum_r;
    {
        int32_t c7 = (int8_t)dsp->regs[0x7Fu];
        fir_sum_l = (int32_t)sclamp16(fir_sum_l + (((int32_t)dsp->fir_l[dsp->fir_pos] * c7) >> 6)) & ~1;
        fir_sum_r = (int32_t)sclamp16(fir_sum_r + (((int32_t)dsp->fir_r[dsp->fir_pos] * c7) >> 6)) & ~1;
    }
    dsp->fir_pos = (uint8_t)((dsp->fir_pos + 1u) & 7u);

    /* Master + Echo output mix */
    int32_t mvol_l = (int8_t)dsp->regs[0x0Cu];
    int32_t mvol_r = (int8_t)dsp->regs[0x1Cu];
    int32_t evol_l = (int8_t)dsp->regs[0x2Cu];
    int32_t evol_r = (int8_t)dsp->regs[0x3Cu];

    int32_t mix_l = (int32_t)sclamp16((main_l * mvol_l) >> 7) + (int32_t)sclamp16((fir_sum_l * evol_l) >> 7);
    int32_t mix_r = (int32_t)sclamp16((main_r * mvol_r) >> 7) + (int32_t)sclamp16((fir_sum_r * evol_r) >> 7);

    /* Write echo feedback into ARAM ONLY if $6C bit 5 (ECEN disable) is 0 */
    if ((flg & 0x20u) == 0u) {
        int32_t efb = (int8_t)dsp->regs[0x0Du];
        uint16_t fb_l = (uint16_t)((int32_t)sclamp16(echo_in_l + ((fir_sum_l * efb) >> 7)) & ~1);
        uint16_t fb_r = (uint16_t)((int32_t)sclamp16(echo_in_r + ((fir_sum_r * efb) >> 7)) & ~1);
        apu->ram[echo_addr]                  = (uint8_t)(fb_l & 0xFFu);
        apu->ram[(uint16_t)(echo_addr + 1u)] = (uint8_t)(fb_l >> 8);
        apu->ram[(uint16_t)(echo_addr + 2u)] = (uint8_t)(fb_r & 0xFFu);
        apu->ram[(uint16_t)(echo_addr + 3u)] = (uint8_t)(fb_r >> 8);
    }

    if (dsp->echo_offset == 0u) {
        uint16_t edl = (uint16_t)(dsp->regs[0x7Du] & 0x0Fu);
        dsp->echo_length = (edl == 0u) ? 4u : (uint16_t)(edl * 2048u);
    }
    dsp->echo_offset = (uint16_t)(dsp->echo_offset + 4u);
    if (dsp->echo_offset >= dsp->echo_length) {
        dsp->echo_offset = 0u;
    }

    if ((flg & 0x40u) != 0u) {
        *out_l = 0;
        *out_r = 0;
    } else {
        *out_l = sclamp16(mix_l);
        *out_r = sclamp16(mix_r);
    }
}

/* ============================================================================
 * Sony SPC700 Bus, Timers, & Complete 256-Opcode Interpreter
 * ============================================================================ */
static uint8_t spc_read8(SnesApu *apu, uint16_t addr) {
    if (addr >= 0x00F0u && addr <= 0x00FFu) {
        switch (addr) {
            case 0x00F0u: return 0u;
            case 0x00F1u: return 0u;
            case 0x00F2u: return apu->dsp_addr;
            case 0x00F3u: return apu->dsp.regs[apu->dsp_addr & 0x7Fu];
            case 0x00F4u:
            case 0x00F5u:
            case 0x00F6u:
            case 0x00F7u: return apu->cpu_to_apu[addr - 0x00F4u];
            case 0x00F8u:
            case 0x00F9u: return apu->aux_io[addr - 0x00F8u];
            case 0x00FAu:
            case 0x00FBu:
            case 0x00FCu: return 0u;
            default: {
                unsigned t = (unsigned)(addr - 0x00FDu);
                uint8_t v = (uint8_t)(apu->timer_out[t] & 0x0Fu);
                apu->timer_out[t] = 0u;
                return v;
            }
        }
    }
    if (addr >= 0xFFC0u && (apu->f1_control & 0x80u) != 0u) {
        return k_spc_ipl_rom[addr - 0xFFC0u];
    }
    return apu->ram[addr];
}

static void spc_write8(SnesApu *apu, uint16_t addr, uint8_t val) {
    apu->ram[addr] = val;
    if (addr >= 0x00F0u && addr <= 0x00FFu) {
        switch (addr) {
            case 0x00F0u:
                apu->f0_test = val;
                break;
            case 0x00F1u: {
                uint8_t rising = (uint8_t)((~apu->f1_control) & val);
                for (unsigned t = 0u; t < 3u; ++t) {
                    if ((rising & (uint8_t)(1u << t)) != 0u) {
                        apu->timer_stage1[t] = 0u;
                        apu->timer_stage2[t] = 0u;
                        apu->timer_out[t] = 0u;
                    }
                }
                if ((val & 0x10u) != 0u) {
                    apu->cpu_to_apu[0] = 0u;
                    apu->cpu_to_apu[1] = 0u;
                }
                if ((val & 0x20u) != 0u) {
                    apu->cpu_to_apu[2] = 0u;
                    apu->cpu_to_apu[3] = 0u;
                }
                apu->f1_control = val;
                break;
            }
            case 0x00F2u:
                apu->dsp_addr = val;
                break;
            case 0x00F3u:
                dsp_write_reg(apu, apu->dsp_addr, val);
                break;
            case 0x00F4u:
            case 0x00F5u:
            case 0x00F6u:
            case 0x00F7u:
                apu->apu_to_cpu[addr - 0x00F4u] = val;
                break;
            case 0x00F8u:
            case 0x00F9u:
                apu->aux_io[addr - 0x00F8u] = val;
                break;
            case 0x00FAu:
            case 0x00FBu:
            case 0x00FCu:
                apu->timer_target[addr - 0x00FAu] = val;
                break;
            default:
                break;
        }
    }
}

static inline uint16_t spc_dp_addr(const SnesApu *apu, uint8_t off) {
    return (uint16_t)(((apu->psw & SPC_PSW_P) != 0u ? 0x0100u : 0x0000u) | (uint16_t)off);
}

static inline uint16_t spc_read16(SnesApu *apu, uint16_t addr) {
    uint16_t lo = (uint16_t)spc_read8(apu, addr);
    uint16_t hi = (uint16_t)spc_read8(apu, (uint16_t)(addr + 1u));
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

static inline uint16_t spc_read_dp16(SnesApu *apu, uint8_t off) {
    uint16_t lo = (uint16_t)spc_read8(apu, spc_dp_addr(apu, off));
    uint16_t hi = (uint16_t)spc_read8(apu, spc_dp_addr(apu, (uint8_t)(off + 1u)));
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

static inline uint8_t spc_fetch8(SnesApu *apu) {
    uint8_t v = spc_read8(apu, apu->pc);
    apu->pc = (uint16_t)(apu->pc + 1u);
    return v;
}

static inline uint16_t spc_fetch16(SnesApu *apu) {
    uint16_t lo = (uint16_t)spc_fetch8(apu);
    uint16_t hi = (uint16_t)spc_fetch8(apu);
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

static inline void spc_push8(SnesApu *apu, uint8_t v) {
    apu->ram[0x0100u | (uint16_t)apu->sp] = v;
    apu->sp = (uint8_t)(apu->sp - 1u);
}

static inline uint8_t spc_pop8(SnesApu *apu) {
    apu->sp = (uint8_t)(apu->sp + 1u);
    return apu->ram[0x0100u | (uint16_t)apu->sp];
}

static inline void spc_push16(SnesApu *apu, uint16_t v) {
    spc_push8(apu, (uint8_t)(v >> 8));
    spc_push8(apu, (uint8_t)(v & 0xFFu));
}

static inline uint16_t spc_pop16(SnesApu *apu) {
    uint16_t lo = (uint16_t)spc_pop8(apu);
    uint16_t hi = (uint16_t)spc_pop8(apu);
    return (uint16_t)(lo | (uint16_t)(hi << 8));
}

static inline void spc_set_nz8(SnesApu *apu, uint8_t v) {
    apu->psw = (uint8_t)((apu->psw & ~(SPC_PSW_N | SPC_PSW_Z)) |
                         (v & 0x80u) |
                         (v == 0u ? SPC_PSW_Z : 0u));
}

static inline void spc_set_flag(SnesApu *apu, uint8_t mask, bool cond) {
    if (cond) apu->psw |= mask;
    else      apu->psw &= (uint8_t)(~mask);
}

static inline uint8_t spc_op_adc(SnesApu *apu, uint8_t a, uint8_t b) {
    int cin = (apu->psw & SPC_PSW_C) != 0u ? 1 : 0;
    int sum = (int)a + (int)b + cin;
    uint8_t res = (uint8_t)sum;
    spc_set_flag(apu, SPC_PSW_C, sum > 0xFF);
    spc_set_flag(apu, SPC_PSW_V, ((~(a ^ b)) & (a ^ res) & 0x80u) != 0u);
    spc_set_flag(apu, SPC_PSW_H, ((int)(a & 0x0Fu) + (int)(b & 0x0Fu) + cin) > 0x0F);
    spc_set_nz8(apu, res);
    return res;
}

static inline uint8_t spc_op_sbc(SnesApu *apu, uint8_t a, uint8_t b) {
    int borrow = (apu->psw & SPC_PSW_C) != 0u ? 0 : 1;
    int diff = (int)a - (int)b - borrow;
    uint8_t res = (uint8_t)diff;
    spc_set_flag(apu, SPC_PSW_C, diff >= 0);
    spc_set_flag(apu, SPC_PSW_V, ((a ^ b) & (a ^ res) & 0x80u) != 0u);
    spc_set_flag(apu, SPC_PSW_H, ((int)(a & 0x0Fu) - (int)(b & 0x0Fu) - borrow) >= 0);
    spc_set_nz8(apu, res);
    return res;
}

static inline void spc_op_cmp(SnesApu *apu, uint8_t a, uint8_t b) {
    int diff = (int)a - (int)b;
    spc_set_flag(apu, SPC_PSW_C, diff >= 0);
    spc_set_nz8(apu, (uint8_t)diff);
}

static inline uint8_t spc_op_asl(SnesApu *apu, uint8_t v) {
    spc_set_flag(apu, SPC_PSW_C, (v & 0x80u) != 0u);
    uint8_t r = (uint8_t)(v << 1);
    spc_set_nz8(apu, r);
    return r;
}

static inline uint8_t spc_op_lsr(SnesApu *apu, uint8_t v) {
    spc_set_flag(apu, SPC_PSW_C, (v & 0x01u) != 0u);
    uint8_t r = (uint8_t)(v >> 1);
    spc_set_nz8(apu, r);
    return r;
}

static inline uint8_t spc_op_rol(SnesApu *apu, uint8_t v) {
    uint8_t c = (uint8_t)(apu->psw & SPC_PSW_C);
    spc_set_flag(apu, SPC_PSW_C, (v & 0x80u) != 0u);
    uint8_t r = (uint8_t)((v << 1) | c);
    spc_set_nz8(apu, r);
    return r;
}

static inline uint8_t spc_op_ror(SnesApu *apu, uint8_t v) {
    uint8_t c = (uint8_t)((apu->psw & SPC_PSW_C) << 7);
    spc_set_flag(apu, SPC_PSW_C, (v & 0x01u) != 0u);
    uint8_t r = (uint8_t)((v >> 1) | c);
    spc_set_nz8(apu, r);
    return r;
}

static inline uint32_t spc_branch(SnesApu *apu, bool cond) {
    int8_t rel = (int8_t)spc_fetch8(apu);
    if (cond) {
        apu->pc = (uint16_t)((int32_t)apu->pc + (int32_t)rel);
        return 4u;
    }
    return 2u;
}

/* Execute 1 SPC700 instruction and return nominal 1.024 MHz cycle count */
static uint32_t spc_step_instruction(SnesApu *apu) {
    if (apu->halted) {
        return 4u;
    }

    uint8_t op = spc_fetch8(apu);

    /* Group 1: TCALL 0..15 (low nibble == 0x01) */
    if ((op & 0x0Fu) == 0x01u) {
        unsigned n = (unsigned)(op >> 4);
        spc_push16(apu, apu->pc);
        apu->pc = spc_read16(apu, (uint16_t)(0xFFDEu - (n * 2u)));
        return 8u;
    }

    /* Group 2: SET1 / CLR1 dp.bit (low nibble == 0x02) */
    if ((op & 0x0Fu) == 0x02u) {
        uint8_t bit = (uint8_t)(1u << (op >> 5));
        uint16_t a  = spc_dp_addr(apu, spc_fetch8(apu));
        uint8_t v   = spc_read8(apu, a);
        if ((op & 0x10u) == 0u) spc_write8(apu, a, (uint8_t)(v | bit));
        else                    spc_write8(apu, a, (uint8_t)(v & (uint8_t)(~bit)));
        return 4u;
    }

    /* Group 3: BBS / BBC dp.bit, rel (low nibble == 0x03) */
    if ((op & 0x0Fu) == 0x03u) {
        uint8_t bit = (uint8_t)(1u << (op >> 5));
        uint8_t v   = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu)));
        int8_t rel  = (int8_t)spc_fetch8(apu);
        bool set    = (v & bit) != 0u;
        bool take   = ((op & 0x10u) == 0u) ? set : !set;
        if (take) {
            apu->pc = (uint16_t)((int32_t)apu->pc + (int32_t)rel);
            return 7u;
        }
        return 5u;
    }

    switch (op) {
        case 0x00u: return 2u; /* NOP */
        case 0xEFu: case 0xFFu: apu->halted = true; return 3u; /* SLEEP / STOP */

        /* Flag instructions */
        case 0x60u: apu->psw &= (uint8_t)(~SPC_PSW_C); return 2u; /* CLRC */
        case 0x80u: apu->psw |= SPC_PSW_C; return 2u;             /* SETC */
        case 0xEDu: apu->psw ^= SPC_PSW_C; return 3u;             /* NOTC */
        case 0xE0u: apu->psw &= (uint8_t)(~(SPC_PSW_V | SPC_PSW_H)); return 2u; /* CLRV */
        case 0x20u: apu->psw &= (uint8_t)(~SPC_PSW_P); return 2u; /* CLRP */
        case 0x40u: apu->psw |= SPC_PSW_P; return 2u;             /* SETP */
        case 0xA0u: apu->psw |= SPC_PSW_I; return 3u;             /* EI */
        case 0xC0u: apu->psw &= (uint8_t)(~SPC_PSW_I); return 3u; /* DI */

        /* Conditional & unconditional relative branches */
        case 0x10u: return spc_branch(apu, (apu->psw & SPC_PSW_N) == 0u); /* BPL */
        case 0x30u: return spc_branch(apu, (apu->psw & SPC_PSW_N) != 0u); /* BMI */
        case 0x50u: return spc_branch(apu, (apu->psw & SPC_PSW_V) == 0u); /* BVC */
        case 0x70u: return spc_branch(apu, (apu->psw & SPC_PSW_V) != 0u); /* BVS */
        case 0x90u: return spc_branch(apu, (apu->psw & SPC_PSW_C) == 0u); /* BCC */
        case 0xB0u: return spc_branch(apu, (apu->psw & SPC_PSW_C) != 0u); /* BCS */
        case 0xD0u: return spc_branch(apu, (apu->psw & SPC_PSW_Z) == 0u); /* BNE */
        case 0xF0u: return spc_branch(apu, (apu->psw & SPC_PSW_Z) != 0u); /* BEQ */
        case 0x2Fu: return spc_branch(apu, true);                         /* BRA */

        /* Compare/Decrement & Branch */
        case 0x2Eu: { /* CBNE dp, rel */
            uint8_t v = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu)));
            return 3u + spc_branch(apu, apu->a != v);
        }
        case 0xDEu: { /* CBNE dp+X, rel */
            uint8_t v = spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)));
            return 4u + spc_branch(apu, apu->a != v);
        }
        case 0x6Eu: { /* DBNZ dp, rel */
            uint16_t a = spc_dp_addr(apu, spc_fetch8(apu));
            uint8_t v  = (uint8_t)(spc_read8(apu, a) - 1u);
            spc_write8(apu, a, v);
            return 3u + spc_branch(apu, v != 0u);
        }
        case 0xFEu: { /* DBNZ Y, rel */
            apu->y = (uint8_t)(apu->y - 1u);
            return 2u + spc_branch(apu, apu->y != 0u);
        }

        /* Jumps, Calls, Returns, Stack */
        case 0x5Fu: apu->pc = spc_fetch16(apu); return 3u; /* JMP !abs */
        case 0x1Fu: { /* JMP [!abs+X] */
            uint16_t ptr = (uint16_t)(spc_fetch16(apu) + apu->x);
            apu->pc = spc_read16(apu, ptr);
            return 6u;
        }
        case 0x3Fu: { /* CALL !abs */
            uint16_t dst = spc_fetch16(apu);
            spc_push16(apu, apu->pc);
            apu->pc = dst;
            return 8u;
        }
        case 0x4Fu: { /* PCALL up */
            uint8_t up = spc_fetch8(apu);
            spc_push16(apu, apu->pc);
            apu->pc = (uint16_t)(0xFF00u | (uint16_t)up);
            return 6u;
        }
        case 0x0Fu: { /* BRK */
            spc_push16(apu, apu->pc);
            spc_push8(apu, apu->psw);
            apu->psw = (uint8_t)((apu->psw | SPC_PSW_B) & ~SPC_PSW_I);
            apu->pc = spc_read16(apu, 0xFFDEu);
            return 8u;
        }
        case 0x6Fu: apu->pc = spc_pop16(apu); return 5u; /* RET */
        case 0x7Fu: apu->psw = spc_pop8(apu); apu->pc = spc_pop16(apu); return 6u; /* RETI */

        case 0x2Du: spc_push8(apu, apu->a);   return 4u; /* PUSH A */
        case 0x4Du: spc_push8(apu, apu->x);   return 4u; /* PUSH X */
        case 0x6Du: spc_push8(apu, apu->y);   return 4u; /* PUSH Y */
        case 0x0Du: spc_push8(apu, apu->psw); return 4u; /* PUSH PSW */
        case 0xAEu: apu->a   = spc_pop8(apu); return 4u; /* POP A (no flags!) */
        case 0xCEu: apu->x   = spc_pop8(apu); return 4u; /* POP X (no flags!) */
        case 0xEEu: apu->y   = spc_pop8(apu); return 4u; /* POP Y (no flags!) */
        case 0x8Eu: apu->psw = spc_pop8(apu); return 4u; /* POP PSW */

        /* Register-to-Register Transfers */
        case 0x7Du: apu->a = apu->x; spc_set_nz8(apu, apu->a); return 2u; /* MOV A, X */
        case 0xDDu: apu->a = apu->y; spc_set_nz8(apu, apu->a); return 2u; /* MOV A, Y */
        case 0x5Du: apu->x = apu->a; spc_set_nz8(apu, apu->x); return 2u; /* MOV X, A */
        case 0xFDu: apu->y = apu->a; spc_set_nz8(apu, apu->y); return 2u; /* MOV Y, A */
        case 0x9Du: apu->x = apu->sp; spc_set_nz8(apu, apu->x); return 2u;/* MOV X, SP */
        case 0xBDu: apu->sp = apu->x; return 2u;                          /* MOV SP, X */

        /* MOV A, ... (all set N, Z) */
        case 0xE8u: apu->a = spc_fetch8(apu); spc_set_nz8(apu, apu->a); return 2u;
        case 0xE6u: apu->a = spc_read8(apu, spc_dp_addr(apu, apu->x)); spc_set_nz8(apu, apu->a); return 3u;
        case 0xBFu: apu->a = spc_read8(apu, spc_dp_addr(apu, apu->x)); apu->x++; spc_set_nz8(apu, apu->a); return 4u;
        case 0xE4u: apu->a = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); spc_set_nz8(apu, apu->a); return 3u;
        case 0xF4u: apu->a = spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x))); spc_set_nz8(apu, apu->a); return 4u;
        case 0xE5u: apu->a = spc_read8(apu, spc_fetch16(apu)); spc_set_nz8(apu, apu->a); return 4u;
        case 0xF5u: apu->a = spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->x)); spc_set_nz8(apu, apu->a); return 5u;
        case 0xF6u: apu->a = spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->y)); spc_set_nz8(apu, apu->a); return 5u;
        case 0xE7u: apu->a = spc_read8(apu, spc_read_dp16(apu, (uint8_t)(spc_fetch8(apu) + apu->x))); spc_set_nz8(apu, apu->a); return 6u;
        case 0xF7u: apu->a = spc_read8(apu, (uint16_t)(spc_read_dp16(apu, spc_fetch8(apu)) + apu->y)); spc_set_nz8(apu, apu->a); return 6u;

        /* MOV X, ... & MOV Y, ... */
        case 0xCDu: apu->x = spc_fetch8(apu); spc_set_nz8(apu, apu->x); return 2u;
        case 0xF8u: apu->x = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); spc_set_nz8(apu, apu->x); return 3u;
        case 0xF9u: apu->x = spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->y))); spc_set_nz8(apu, apu->x); return 4u;
        case 0xE9u: apu->x = spc_read8(apu, spc_fetch16(apu)); spc_set_nz8(apu, apu->x); return 4u;
        case 0x8Du: apu->y = spc_fetch8(apu); spc_set_nz8(apu, apu->y); return 2u;
        case 0xEBu: apu->y = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); spc_set_nz8(apu, apu->y); return 3u;
        case 0xFBu: apu->y = spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x))); spc_set_nz8(apu, apu->y); return 4u;
        case 0xECu: apu->y = spc_read8(apu, spc_fetch16(apu)); spc_set_nz8(apu, apu->y); return 4u;

        /* MOV mem, reg / imm / dp (no flags affected) */
        case 0xC6u: spc_write8(apu, spc_dp_addr(apu, apu->x), apu->a); return 4u;
        case 0xAFu: spc_write8(apu, spc_dp_addr(apu, apu->x), apu->a); apu->x++; return 4u;
        case 0xC4u: spc_write8(apu, spc_dp_addr(apu, spc_fetch8(apu)), apu->a); return 4u;
        case 0xD4u: spc_write8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)), apu->a); return 5u;
        case 0xC5u: spc_write8(apu, spc_fetch16(apu), apu->a); return 5u;
        case 0xD5u: spc_write8(apu, (uint16_t)(spc_fetch16(apu) + apu->x), apu->a); return 6u;
        case 0xD6u: spc_write8(apu, (uint16_t)(spc_fetch16(apu) + apu->y), apu->a); return 6u;
        case 0xC7u: spc_write8(apu, spc_read_dp16(apu, (uint8_t)(spc_fetch8(apu) + apu->x)), apu->a); return 7u;
        case 0xD7u: spc_write8(apu, (uint16_t)(spc_read_dp16(apu, spc_fetch8(apu)) + apu->y), apu->a); return 7u;

        case 0xD8u: spc_write8(apu, spc_dp_addr(apu, spc_fetch8(apu)), apu->x); return 4u;
        case 0xD9u: spc_write8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->y)), apu->x); return 5u;
        case 0xC9u: spc_write8(apu, spc_fetch16(apu), apu->x); return 5u;
        case 0xCBu: spc_write8(apu, spc_dp_addr(apu, spc_fetch8(apu)), apu->y); return 4u;
        case 0xDBu: spc_write8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)), apu->y); return 5u;
        case 0xCCu: spc_write8(apu, spc_fetch16(apu), apu->y); return 5u;

        case 0xFAu: { /* MOV dp(d), dp(s) */
            uint8_t s = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu)));
            uint8_t d = spc_fetch8(apu);
            spc_write8(apu, spc_dp_addr(apu, d), s);
            return 5u;
        }
        case 0x8Fu: { /* MOV dp, #imm */
            uint8_t imm = spc_fetch8(apu);
            uint8_t d   = spc_fetch8(apu);
            spc_write8(apu, spc_dp_addr(apu, d), imm);
            return 5u;
        }

        /* OR */
        case 0x08u: apu->a |= spc_fetch8(apu); spc_set_nz8(apu, apu->a); return 2u;
        case 0x06u: apu->a |= spc_read8(apu, spc_dp_addr(apu, apu->x)); spc_set_nz8(apu, apu->a); return 3u;
        case 0x04u: apu->a |= spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); spc_set_nz8(apu, apu->a); return 3u;
        case 0x14u: apu->a |= spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x))); spc_set_nz8(apu, apu->a); return 4u;
        case 0x05u: apu->a |= spc_read8(apu, spc_fetch16(apu)); spc_set_nz8(apu, apu->a); return 4u;
        case 0x15u: apu->a |= spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->x)); spc_set_nz8(apu, apu->a); return 5u;
        case 0x16u: apu->a |= spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->y)); spc_set_nz8(apu, apu->a); return 5u;
        case 0x07u: apu->a |= spc_read8(apu, spc_read_dp16(apu, (uint8_t)(spc_fetch8(apu) + apu->x))); spc_set_nz8(apu, apu->a); return 6u;
        case 0x17u: apu->a |= spc_read8(apu, (uint16_t)(spc_read_dp16(apu, spc_fetch8(apu)) + apu->y)); spc_set_nz8(apu, apu->a); return 6u;
        case 0x09u: { uint8_t s = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = (uint8_t)(spc_read8(apu, da) | s); spc_write8(apu, da, r); spc_set_nz8(apu, r); return 6u; }
        case 0x18u: { uint8_t imm = spc_fetch8(apu); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = (uint8_t)(spc_read8(apu, da) | imm); spc_write8(apu, da, r); spc_set_nz8(apu, r); return 5u; }
        case 0x19u: { uint16_t xa = spc_dp_addr(apu, apu->x); uint8_t r = (uint8_t)(spc_read8(apu, xa) | spc_read8(apu, spc_dp_addr(apu, apu->y))); spc_write8(apu, xa, r); spc_set_nz8(apu, r); return 5u; }

        /* AND */
        case 0x28u: apu->a &= spc_fetch8(apu); spc_set_nz8(apu, apu->a); return 2u;
        case 0x26u: apu->a &= spc_read8(apu, spc_dp_addr(apu, apu->x)); spc_set_nz8(apu, apu->a); return 3u;
        case 0x24u: apu->a &= spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); spc_set_nz8(apu, apu->a); return 3u;
        case 0x34u: apu->a &= spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x))); spc_set_nz8(apu, apu->a); return 4u;
        case 0x25u: apu->a &= spc_read8(apu, spc_fetch16(apu)); spc_set_nz8(apu, apu->a); return 4u;
        case 0x35u: apu->a &= spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->x)); spc_set_nz8(apu, apu->a); return 5u;
        case 0x36u: apu->a &= spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->y)); spc_set_nz8(apu, apu->a); return 5u;
        case 0x27u: apu->a &= spc_read8(apu, spc_read_dp16(apu, (uint8_t)(spc_fetch8(apu) + apu->x))); spc_set_nz8(apu, apu->a); return 6u;
        case 0x37u: apu->a &= spc_read8(apu, (uint16_t)(spc_read_dp16(apu, spc_fetch8(apu)) + apu->y)); spc_set_nz8(apu, apu->a); return 6u;
        case 0x29u: { uint8_t s = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = (uint8_t)(spc_read8(apu, da) & s); spc_write8(apu, da, r); spc_set_nz8(apu, r); return 6u; }
        case 0x38u: { uint8_t imm = spc_fetch8(apu); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = (uint8_t)(spc_read8(apu, da) & imm); spc_write8(apu, da, r); spc_set_nz8(apu, r); return 5u; }
        case 0x39u: { uint16_t xa = spc_dp_addr(apu, apu->x); uint8_t r = (uint8_t)(spc_read8(apu, xa) & spc_read8(apu, spc_dp_addr(apu, apu->y))); spc_write8(apu, xa, r); spc_set_nz8(apu, r); return 5u; }

        /* EOR */
        case 0x48u: apu->a ^= spc_fetch8(apu); spc_set_nz8(apu, apu->a); return 2u;
        case 0x46u: apu->a ^= spc_read8(apu, spc_dp_addr(apu, apu->x)); spc_set_nz8(apu, apu->a); return 3u;
        case 0x44u: apu->a ^= spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); spc_set_nz8(apu, apu->a); return 3u;
        case 0x54u: apu->a ^= spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x))); spc_set_nz8(apu, apu->a); return 4u;
        case 0x45u: apu->a ^= spc_read8(apu, spc_fetch16(apu)); spc_set_nz8(apu, apu->a); return 4u;
        case 0x55u: apu->a ^= spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->x)); spc_set_nz8(apu, apu->a); return 5u;
        case 0x56u: apu->a ^= spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->y)); spc_set_nz8(apu, apu->a); return 5u;
        case 0x47u: apu->a ^= spc_read8(apu, spc_read_dp16(apu, (uint8_t)(spc_fetch8(apu) + apu->x))); spc_set_nz8(apu, apu->a); return 6u;
        case 0x57u: apu->a ^= spc_read8(apu, (uint16_t)(spc_read_dp16(apu, spc_fetch8(apu)) + apu->y)); spc_set_nz8(apu, apu->a); return 6u;
        case 0x49u: { uint8_t s = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = (uint8_t)(spc_read8(apu, da) ^ s); spc_write8(apu, da, r); spc_set_nz8(apu, r); return 6u; }
        case 0x58u: { uint8_t imm = spc_fetch8(apu); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = (uint8_t)(spc_read8(apu, da) ^ imm); spc_write8(apu, da, r); spc_set_nz8(apu, r); return 5u; }
        case 0x59u: { uint16_t xa = spc_dp_addr(apu, apu->x); uint8_t r = (uint8_t)(spc_read8(apu, xa) ^ spc_read8(apu, spc_dp_addr(apu, apu->y))); spc_write8(apu, xa, r); spc_set_nz8(apu, r); return 5u; }

        /* CMP A / X / Y / mem */
        case 0x68u: spc_op_cmp(apu, apu->a, spc_fetch8(apu)); return 2u;
        case 0x66u: spc_op_cmp(apu, apu->a, spc_read8(apu, spc_dp_addr(apu, apu->x))); return 3u;
        case 0x64u: spc_op_cmp(apu, apu->a, spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu)))); return 3u;
        case 0x74u: spc_op_cmp(apu, apu->a, spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)))); return 4u;
        case 0x65u: spc_op_cmp(apu, apu->a, spc_read8(apu, spc_fetch16(apu))); return 4u;
        case 0x75u: spc_op_cmp(apu, apu->a, spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->x))); return 5u;
        case 0x76u: spc_op_cmp(apu, apu->a, spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->y))); return 5u;
        case 0x67u: spc_op_cmp(apu, apu->a, spc_read8(apu, spc_read_dp16(apu, (uint8_t)(spc_fetch8(apu) + apu->x)))); return 6u;
        case 0x77u: spc_op_cmp(apu, apu->a, spc_read8(apu, (uint16_t)(spc_read_dp16(apu, spc_fetch8(apu)) + apu->y))); return 6u;
        case 0x69u: { uint8_t s = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); uint8_t d = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); spc_op_cmp(apu, d, s); return 6u; }
        case 0x78u: { uint8_t imm = spc_fetch8(apu); uint8_t d = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); spc_op_cmp(apu, d, imm); return 5u; }
        case 0x79u: spc_op_cmp(apu, spc_read8(apu, spc_dp_addr(apu, apu->x)), spc_read8(apu, spc_dp_addr(apu, apu->y))); return 5u;

        case 0xC8u: spc_op_cmp(apu, apu->x, spc_fetch8(apu)); return 2u;
        case 0x3Eu: spc_op_cmp(apu, apu->x, spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu)))); return 3u;
        case 0x1Eu: spc_op_cmp(apu, apu->x, spc_read8(apu, spc_fetch16(apu))); return 4u;
        case 0xADu: spc_op_cmp(apu, apu->y, spc_fetch8(apu)); return 2u;
        case 0x7Eu: spc_op_cmp(apu, apu->y, spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu)))); return 3u;
        case 0x5Eu: spc_op_cmp(apu, apu->y, spc_read8(apu, spc_fetch16(apu))); return 4u;

        /* ADC */
        case 0x88u: apu->a = spc_op_adc(apu, apu->a, spc_fetch8(apu)); return 2u;
        case 0x86u: apu->a = spc_op_adc(apu, apu->a, spc_read8(apu, spc_dp_addr(apu, apu->x))); return 3u;
        case 0x84u: apu->a = spc_op_adc(apu, apu->a, spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu)))); return 3u;
        case 0x94u: apu->a = spc_op_adc(apu, apu->a, spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)))); return 4u;
        case 0x85u: apu->a = spc_op_adc(apu, apu->a, spc_read8(apu, spc_fetch16(apu))); return 4u;
        case 0x95u: apu->a = spc_op_adc(apu, apu->a, spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->x))); return 5u;
        case 0x96u: apu->a = spc_op_adc(apu, apu->a, spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->y))); return 5u;
        case 0x87u: apu->a = spc_op_adc(apu, apu->a, spc_read8(apu, spc_read_dp16(apu, (uint8_t)(spc_fetch8(apu) + apu->x)))); return 6u;
        case 0x97u: apu->a = spc_op_adc(apu, apu->a, spc_read8(apu, (uint16_t)(spc_read_dp16(apu, spc_fetch8(apu)) + apu->y))); return 6u;
        case 0x89u: { uint8_t s = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = spc_op_adc(apu, spc_read8(apu, da), s); spc_write8(apu, da, r); return 6u; }
        case 0x98u: { uint8_t imm = spc_fetch8(apu); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = spc_op_adc(apu, spc_read8(apu, da), imm); spc_write8(apu, da, r); return 5u; }
        case 0x99u: { uint16_t xa = spc_dp_addr(apu, apu->x); uint8_t r = spc_op_adc(apu, spc_read8(apu, xa), spc_read8(apu, spc_dp_addr(apu, apu->y))); spc_write8(apu, xa, r); return 5u; }

        /* SBC */
        case 0xA8u: apu->a = spc_op_sbc(apu, apu->a, spc_fetch8(apu)); return 2u;
        case 0xA6u: apu->a = spc_op_sbc(apu, apu->a, spc_read8(apu, spc_dp_addr(apu, apu->x))); return 3u;
        case 0xA4u: apu->a = spc_op_sbc(apu, apu->a, spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu)))); return 3u;
        case 0xB4u: apu->a = spc_op_sbc(apu, apu->a, spc_read8(apu, spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)))); return 4u;
        case 0xA5u: apu->a = spc_op_sbc(apu, apu->a, spc_read8(apu, spc_fetch16(apu))); return 4u;
        case 0xB5u: apu->a = spc_op_sbc(apu, apu->a, spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->x))); return 5u;
        case 0xB6u: apu->a = spc_op_sbc(apu, apu->a, spc_read8(apu, (uint16_t)(spc_fetch16(apu) + apu->y))); return 5u;
        case 0xA7u: apu->a = spc_op_sbc(apu, apu->a, spc_read8(apu, spc_read_dp16(apu, (uint8_t)(spc_fetch8(apu) + apu->x)))); return 6u;
        case 0xB7u: apu->a = spc_op_sbc(apu, apu->a, spc_read8(apu, (uint16_t)(spc_read_dp16(apu, spc_fetch8(apu)) + apu->y))); return 6u;
        case 0xA9u: { uint8_t s = spc_read8(apu, spc_dp_addr(apu, spc_fetch8(apu))); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = spc_op_sbc(apu, spc_read8(apu, da), s); spc_write8(apu, da, r); return 6u; }
        case 0xB8u: { uint8_t imm = spc_fetch8(apu); uint16_t da = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = spc_op_sbc(apu, spc_read8(apu, da), imm); spc_write8(apu, da, r); return 5u; }
        case 0xB9u: { uint16_t xa = spc_dp_addr(apu, apu->x); uint8_t r = spc_op_sbc(apu, spc_read8(apu, xa), spc_read8(apu, spc_dp_addr(apu, apu->y))); spc_write8(apu, xa, r); return 5u; }

        /* INC / DEC / Shifts / Rotates */
        case 0xBCu: apu->a++; spc_set_nz8(apu, apu->a); return 2u; /* INC A */
        case 0x3Du: apu->x++; spc_set_nz8(apu, apu->x); return 2u; /* INC X */
        case 0xFCu: apu->y++; spc_set_nz8(apu, apu->y); return 2u; /* INC Y */
        case 0xABu: { uint16_t a = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = (uint8_t)(spc_read8(apu, a) + 1u); spc_write8(apu, a, r); spc_set_nz8(apu, r); return 4u; }
        case 0xBBu: { uint16_t a = spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)); uint8_t r = (uint8_t)(spc_read8(apu, a) + 1u); spc_write8(apu, a, r); spc_set_nz8(apu, r); return 5u; }
        case 0xACu: { uint16_t a = spc_fetch16(apu); uint8_t r = (uint8_t)(spc_read8(apu, a) + 1u); spc_write8(apu, a, r); spc_set_nz8(apu, r); return 5u; }

        case 0x9Cu: apu->a--; spc_set_nz8(apu, apu->a); return 2u; /* DEC A */
        case 0x1Du: apu->x--; spc_set_nz8(apu, apu->x); return 2u; /* DEC X */
        case 0xDCu: apu->y--; spc_set_nz8(apu, apu->y); return 2u; /* DEC Y */
        case 0x8Bu: { uint16_t a = spc_dp_addr(apu, spc_fetch8(apu)); uint8_t r = (uint8_t)(spc_read8(apu, a) - 1u); spc_write8(apu, a, r); spc_set_nz8(apu, r); return 4u; }
        case 0x9Bu: { uint16_t a = spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)); uint8_t r = (uint8_t)(spc_read8(apu, a) - 1u); spc_write8(apu, a, r); spc_set_nz8(apu, r); return 5u; }
        case 0x8Cu: { uint16_t a = spc_fetch16(apu); uint8_t r = (uint8_t)(spc_read8(apu, a) - 1u); spc_write8(apu, a, r); spc_set_nz8(apu, r); return 5u; }

        case 0x1Cu: apu->a = spc_op_asl(apu, apu->a); return 2u;
        case 0x0Bu: { uint16_t a = spc_dp_addr(apu, spc_fetch8(apu)); spc_write8(apu, a, spc_op_asl(apu, spc_read8(apu, a))); return 4u; }
        case 0x1Bu: { uint16_t a = spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)); spc_write8(apu, a, spc_op_asl(apu, spc_read8(apu, a))); return 5u; }
        case 0x0Cu: { uint16_t a = spc_fetch16(apu); spc_write8(apu, a, spc_op_asl(apu, spc_read8(apu, a))); return 5u; }

        case 0x3Cu: apu->a = spc_op_rol(apu, apu->a); return 2u;
        case 0x2Bu: { uint16_t a = spc_dp_addr(apu, spc_fetch8(apu)); spc_write8(apu, a, spc_op_rol(apu, spc_read8(apu, a))); return 4u; }
        case 0x3Bu: { uint16_t a = spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)); spc_write8(apu, a, spc_op_rol(apu, spc_read8(apu, a))); return 5u; }
        case 0x2Cu: { uint16_t a = spc_fetch16(apu); spc_write8(apu, a, spc_op_rol(apu, spc_read8(apu, a))); return 5u; }

        case 0x5Cu: apu->a = spc_op_lsr(apu, apu->a); return 2u;
        case 0x4Bu: { uint16_t a = spc_dp_addr(apu, spc_fetch8(apu)); spc_write8(apu, a, spc_op_lsr(apu, spc_read8(apu, a))); return 4u; }
        case 0x5Bu: { uint16_t a = spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)); spc_write8(apu, a, spc_op_lsr(apu, spc_read8(apu, a))); return 5u; }
        case 0x4Cu: { uint16_t a = spc_fetch16(apu); spc_write8(apu, a, spc_op_lsr(apu, spc_read8(apu, a))); return 5u; }

        case 0x7Cu: apu->a = spc_op_ror(apu, apu->a); return 2u;
        case 0x6Bu: { uint16_t a = spc_dp_addr(apu, spc_fetch8(apu)); spc_write8(apu, a, spc_op_ror(apu, spc_read8(apu, a))); return 4u; }
        case 0x7Bu: { uint16_t a = spc_dp_addr(apu, (uint8_t)(spc_fetch8(apu) + apu->x)); spc_write8(apu, a, spc_op_ror(apu, spc_read8(apu, a))); return 5u; }
        case 0x6Cu: { uint16_t a = spc_fetch16(apu); spc_write8(apu, a, spc_op_ror(apu, spc_read8(apu, a))); return 5u; }

        case 0x9Fu: /* XCN A */
            apu->a = (uint8_t)((apu->a << 4) | (apu->a >> 4));
            spc_set_nz8(apu, apu->a);
            return 5u;

        /* 16-bit Word Operations (YA & dp) */
        case 0xBAu: { /* MOVW YA, dp */
            uint16_t w = spc_read_dp16(apu, spc_fetch8(apu));
            apu->a = (uint8_t)(w & 0xFFu);
            apu->y = (uint8_t)(w >> 8);
            spc_set_flag(apu, SPC_PSW_N, (w & 0x8000u) != 0u);
            spc_set_flag(apu, SPC_PSW_Z, w == 0u);
            return 5u;
        }
        case 0xDAu: { /* MOVW dp, YA */
            uint8_t d = spc_fetch8(apu);
            spc_write8(apu, spc_dp_addr(apu, d), apu->a);
            spc_write8(apu, spc_dp_addr(apu, (uint8_t)(d + 1u)), apu->y);
            return 5u;
        }
        case 0x3Au: { /* INCW dp */
            uint8_t d = spc_fetch8(apu);
            uint16_t w = (uint16_t)(spc_read_dp16(apu, d) + 1u);
            spc_write8(apu, spc_dp_addr(apu, d), (uint8_t)(w & 0xFFu));
            spc_write8(apu, spc_dp_addr(apu, (uint8_t)(d + 1u)), (uint8_t)(w >> 8));
            spc_set_flag(apu, SPC_PSW_N, (w & 0x8000u) != 0u);
            spc_set_flag(apu, SPC_PSW_Z, w == 0u);
            return 6u;
        }
        case 0x1Au: { /* DECW dp */
            uint8_t d = spc_fetch8(apu);
            uint16_t w = (uint16_t)(spc_read_dp16(apu, d) - 1u);
            spc_write8(apu, spc_dp_addr(apu, d), (uint8_t)(w & 0xFFu));
            spc_write8(apu, spc_dp_addr(apu, (uint8_t)(d + 1u)), (uint8_t)(w >> 8));
            spc_set_flag(apu, SPC_PSW_N, (w & 0x8000u) != 0u);
            spc_set_flag(apu, SPC_PSW_Z, w == 0u);
            return 6u;
        }
        case 0x7Au: { /* ADDW YA, dp */
            uint16_t ya = (uint16_t)(((uint16_t)apu->y << 8) | (uint16_t)apu->a);
            uint16_t m  = spc_read_dp16(apu, spc_fetch8(apu));
            uint32_t s  = (uint32_t)ya + (uint32_t)m;
            uint16_t r  = (uint16_t)s;
            spc_set_flag(apu, SPC_PSW_C, s > 0xFFFFu);
            spc_set_flag(apu, SPC_PSW_V, ((~(ya ^ m)) & (ya ^ r) & 0x8000u) != 0u);
            spc_set_flag(apu, SPC_PSW_H, ((ya & 0x0FFFu) + (m & 0x0FFFu)) > 0x0FFFu);
            spc_set_flag(apu, SPC_PSW_N, (r & 0x8000u) != 0u);
            spc_set_flag(apu, SPC_PSW_Z, r == 0u);
            apu->a = (uint8_t)(r & 0xFFu);
            apu->y = (uint8_t)(r >> 8);
            return 5u;
        }
        case 0x9Au: { /* SUBW YA, dp */
            uint16_t ya = (uint16_t)(((uint16_t)apu->y << 8) | (uint16_t)apu->a);
            uint16_t m  = spc_read_dp16(apu, spc_fetch8(apu));
            int32_t  d  = (int32_t)ya - (int32_t)m;
            uint16_t r  = (uint16_t)d;
            spc_set_flag(apu, SPC_PSW_C, d >= 0);
            spc_set_flag(apu, SPC_PSW_V, ((ya ^ m) & (ya ^ r) & 0x8000u) != 0u);
            spc_set_flag(apu, SPC_PSW_H, ((int)(ya & 0x0FFFu) - (int)(m & 0x0FFFu)) >= 0);
            spc_set_flag(apu, SPC_PSW_N, (r & 0x8000u) != 0u);
            spc_set_flag(apu, SPC_PSW_Z, r == 0u);
            apu->a = (uint8_t)(r & 0xFFu);
            apu->y = (uint8_t)(r >> 8);
            return 5u;
        }
        case 0x5Au: { /* CMPW YA, dp */
            uint16_t ya = (uint16_t)(((uint16_t)apu->y << 8) | (uint16_t)apu->a);
            uint16_t m  = spc_read_dp16(apu, spc_fetch8(apu));
            int32_t  d  = (int32_t)ya - (int32_t)m;
            spc_set_flag(apu, SPC_PSW_C, d >= 0);
            spc_set_flag(apu, SPC_PSW_N, ((uint16_t)d & 0x8000u) != 0u);
            spc_set_flag(apu, SPC_PSW_Z, (uint16_t)d == 0u);
            return 4u;
        }
        case 0xCFu: { /* MUL YA */
            uint16_t prod = (uint16_t)((uint16_t)apu->y * (uint16_t)apu->a);
            apu->a = (uint8_t)(prod & 0xFFu);
            apu->y = (uint8_t)(prod >> 8);
            spc_set_nz8(apu, apu->y);
            return 9u;
        }
        case 0x9Eu: { /* DIV YA, X */
            uint16_t ya = (uint16_t)(((uint16_t)apu->y << 8) | (uint16_t)apu->a);
            spc_set_flag(apu, SPC_PSW_H, (apu->y & 0x0Fu) >= (apu->x & 0x0Fu));
            spc_set_flag(apu, SPC_PSW_V, apu->y >= apu->x);
            if (apu->y < (uint16_t)(apu->x << 1)) {
                apu->a = (uint8_t)(ya / apu->x);
                apu->y = (uint8_t)(ya % apu->x);
            } else {
                apu->a = (uint8_t)(255u - (ya - ((uint16_t)apu->x << 9)) / (256u - apu->x));
                apu->y = (uint8_t)(apu->x + (ya - ((uint16_t)apu->x << 9)) % (256u - apu->x));
            }
            spc_set_nz8(apu, apu->a);
            return 12u;
        }

        /* TSET1 / TCLR1 !abs */
        case 0x0Eu: {
            uint16_t a = spc_fetch16(apu);
            uint8_t  v = spc_read8(apu, a);
            spc_set_nz8(apu, (uint8_t)(apu->a - v));
            spc_write8(apu, a, (uint8_t)(v | apu->a));
            return 6u;
        }
        case 0x4Eu: {
            uint16_t a = spc_fetch16(apu);
            uint8_t  v = spc_read8(apu, a);
            spc_set_nz8(apu, (uint8_t)(apu->a - v));
            spc_write8(apu, a, (uint8_t)(v & (uint8_t)(~apu->a)));
            return 6u;
        }

        /* 1-bit Carry / mem.bit Operations */
        case 0x0Au: case 0x2Au: case 0x4Au: case 0x6Au:
        case 0x8Au: case 0xAAu: case 0xCAu: case 0xEAu: {
            uint16_t mb  = spc_fetch16(apu);
            uint16_t a   = (uint16_t)(mb & 0x1FFFu);
            uint8_t  bit = (uint8_t)(1u << (mb >> 13));
            uint8_t  v   = spc_read8(apu, a);
            bool     m   = (v & bit) != 0u;
            bool     c   = (apu->psw & SPC_PSW_C) != 0u;
            switch (op) {
                case 0x0Au: spc_set_flag(apu, SPC_PSW_C, c || m);  return 5u;
                case 0x2Au: spc_set_flag(apu, SPC_PSW_C, c || !m); return 5u;
                case 0x4Au: spc_set_flag(apu, SPC_PSW_C, c && m);  return 4u;
                case 0x6Au: spc_set_flag(apu, SPC_PSW_C, c && !m); return 4u;
                case 0x8Au: spc_set_flag(apu, SPC_PSW_C, c != m);  return 5u;
                case 0xAAu: spc_set_flag(apu, SPC_PSW_C, m);       return 4u;
                case 0xCAu:
                    spc_write8(apu, a, c ? (uint8_t)(v | bit) : (uint8_t)(v & (uint8_t)(~bit)));
                    return 6u;
                default: /* 0xEA NOT1 */
                    spc_write8(apu, a, (uint8_t)(v ^ bit));
                    return 5u;
            }
        }

        /* BCD Adjust (DAA / DAS) */
        case 0xDFu: { /* DAA A */
            if ((apu->psw & SPC_PSW_C) != 0u || apu->a > 0x99u) {
                apu->a = (uint8_t)(apu->a + 0x60u);
                apu->psw |= SPC_PSW_C;
            }
            if ((apu->psw & SPC_PSW_H) != 0u || (apu->a & 0x0Fu) > 0x09u) {
                apu->a = (uint8_t)(apu->a + 0x06u);
            }
            spc_set_nz8(apu, apu->a);
            return 3u;
        }
        case 0xBEu: { /* DAS A */
            if ((apu->psw & SPC_PSW_C) == 0u || apu->a > 0x99u) {
                apu->a = (uint8_t)(apu->a - 0x60u);
                apu->psw &= (uint8_t)(~SPC_PSW_C);
            }
            if ((apu->psw & SPC_PSW_H) == 0u || (apu->a & 0x0Fu) > 0x09u) {
                apu->a = (uint8_t)(apu->a - 0x06u);
            }
            spc_set_nz8(apu, apu->a);
            return 3u;
        }

        default:
            return 2u;
    }
}

static void spc_step_timers(SnesApu *apu, uint32_t cycles) {
    static const uint16_t k_prescale[3] = { 128u, 128u, 16u };
    for (unsigned t = 0u; t < 3u; ++t) {
        if ((apu->f1_control & (uint8_t)(1u << t)) == 0u) {
            continue;
        }
        apu->timer_stage1[t] = (uint16_t)(apu->timer_stage1[t] + cycles);
        uint16_t pre = k_prescale[t];
        uint16_t tgt = (apu->timer_target[t] == 0u) ? 256u : (uint16_t)apu->timer_target[t];
        while (apu->timer_stage1[t] >= pre) {
            apu->timer_stage1[t] = (uint16_t)(apu->timer_stage1[t] - pre);
            apu->timer_stage2[t] = (uint16_t)(apu->timer_stage2[t] + 1u);
            if (apu->timer_stage2[t] >= tgt) {
                apu->timer_stage2[t] = 0u;
                apu->timer_out[t] = (uint8_t)((apu->timer_out[t] + 1u) & 0x0Fu);
            }
        }
    }
}

static void apu_push_stereo_sample(SnesApu *apu, int16_t l, int16_t r) {
    apu->total_samples_emitted++;
    if (apu->pcm_sink != NULL) {
        apu->pcm_sink(apu->pcm_sink_user, l, r);
        return;
    }

    if (apu->pcm_count >= SNES_APU_RING_FRAMES) {
        /* Drop oldest sample on ring overflow so latency never grows unbounded */
        apu->pcm_read_pos = (apu->pcm_read_pos + 1u) % SNES_APU_RING_FRAMES;
        apu->pcm_count--;
    }
    apu->pcm_ring[apu->pcm_write_pos * 2u + 0u] = l;
    apu->pcm_ring[apu->pcm_write_pos * 2u + 1u] = r;
    apu->pcm_write_pos = (apu->pcm_write_pos + 1u) % SNES_APU_RING_FRAMES;
    apu->pcm_count++;
}

void snes_apu_step_cycles(SnesApu *apu, uint32_t cycles) {
    uint32_t ran = 0u;
    while (ran < cycles) {
        uint32_t c = spc_step_instruction(apu);
        ran += c;
        apu->total_cycles += c;
        spc_step_timers(apu, c);

        apu->dsp_cycle_accum += c;
        while (apu->dsp_cycle_accum >= 32u) {
            apu->dsp_cycle_accum -= 32u;
            int16_t l = 0, r = 0;
            dsp_tick_sample(apu, &l, &r);
            apu_push_stereo_sample(apu, l, r);
        }
    }
}

void snes_apu_init(SnesApu *apu) {
    memset(apu, 0, sizeof(*apu));
    apu->f1_control = 0x80u; /* Enable 64-byte IPL Boot ROM at $FFC0-$FFFF */
    apu->sp         = 0xEFu;
    apu->pc         = spc_read16(apu, 0xFFFEu);
    dsp_reset(&apu->dsp);

    /* Step IPL ROM through zero-page clear ($FFC0-$FFC8) until $BBAA ready signature */
    for (unsigned guard = 0u; guard < 512u; ++guard) {
        snes_apu_step_cycles(apu, 16u);
        if (apu->apu_to_cpu[0] == 0xAAu && apu->apu_to_cpu[1] == 0xBBu) {
            break;
        }
    }
    /* SPC700 time zero is the CPU's power-on; the IPL boot above happens "before" it */
    apu->dsp_cycle_accum = 0u;
    apu->total_cycles = 0u;
    apu->pcm_read_pos = apu->pcm_write_pos = apu->pcm_count = 0u;
}

void snes_apu_run_to(SnesApu *apu, uint64_t spc_time) {
    if (apu == NULL || spc_time <= apu->total_cycles) {
        return;
    }
    /* Large gaps (long loads) are stepped in bounded slices to keep the 32-bit step API */
    while (apu->total_cycles < spc_time) {
        uint64_t gap = spc_time - apu->total_cycles;
        snes_apu_step_cycles(apu, (gap > 0x10000000u) ? 0x10000000u : (uint32_t)gap);
    }
}

void snes_apu_attach(SnesCpuContext *ctx, SnesApu *apu) {
    ctx->apu_core = apu;
    if (apu != NULL) {
        ctx->apu.auto_handshake = false;
        for (unsigned i = 0u; i < 4u; ++i) {
            ctx->apu.apu_to_cpu[i] = apu->apu_to_cpu[i];
        }
    } else {
        ctx->apu.auto_handshake = true;
    }
}

void snes_apu_write_port(SnesApu *apu, uint8_t port, uint8_t val) {
    /* The caller has already run the SPC700 up to the 65816's current time (snes_apu_sync) */
    apu->cpu_to_apu[port & 3u] = val;
}

uint8_t snes_apu_read_port(SnesApu *apu, uint8_t port) {
    return apu->apu_to_cpu[port & 3u];
}

size_t snes_apu_pop_samples(SnesApu *apu, int16_t *stereo_out, size_t max_frames) {
    if (apu == NULL || stereo_out == NULL || max_frames == 0u) {
        return 0u;
    }
    size_t n = (apu->pcm_count < max_frames) ? (size_t)apu->pcm_count : max_frames;
    for (size_t i = 0u; i < n; ++i) {
        stereo_out[i * 2u + 0u] = apu->pcm_ring[apu->pcm_read_pos * 2u + 0u];
        stereo_out[i * 2u + 1u] = apu->pcm_ring[apu->pcm_read_pos * 2u + 1u];
        apu->pcm_read_pos = (apu->pcm_read_pos + 1u) % SNES_APU_RING_FRAMES;
    }
    apu->pcm_count -= (uint32_t)n;
    return n;
}

void snes_apu_clear_samples(SnesApu *apu) {
    if (apu != NULL) {
        apu->pcm_read_pos  = 0u;
        apu->pcm_write_pos = 0u;
        apu->pcm_count     = 0u;
    }
}

bool snes_write_wav(const char *path, const int16_t *stereo_pcm, size_t num_frames, uint32_t sample_rate) {
    if (path == NULL || stereo_pcm == NULL) {
        return false;
    }
    FILE *fp = fopen(path, "wb");
    if (fp == NULL) {
        return false;
    }

    uint32_t data_bytes = (uint32_t)(num_frames * 4u);
    uint32_t riff_size  = 36u + data_bytes;
    uint32_t byte_rate  = sample_rate * 4u;
    uint8_t  hdr[44];

    memcpy(&hdr[0],  "RIFF", 4u);
    hdr[4]  = (uint8_t)(riff_size & 0xFFu);
    hdr[5]  = (uint8_t)((riff_size >> 8) & 0xFFu);
    hdr[6]  = (uint8_t)((riff_size >> 16) & 0xFFu);
    hdr[7]  = (uint8_t)((riff_size >> 24) & 0xFFu);
    memcpy(&hdr[8],  "WAVEfmt ", 8u);
    hdr[16] = 16u; hdr[17] = 0u; hdr[18] = 0u; hdr[19] = 0u; /* PCM chunk size = 16 */
    hdr[20] = 1u;  hdr[21] = 0u;                             /* AudioFormat = 1 (PCM) */
    hdr[22] = 2u;  hdr[23] = 0u;                             /* NumChannels = 2 (Stereo) */
    hdr[24] = (uint8_t)(sample_rate & 0xFFu);
    hdr[25] = (uint8_t)((sample_rate >> 8) & 0xFFu);
    hdr[26] = (uint8_t)((sample_rate >> 16) & 0xFFu);
    hdr[27] = (uint8_t)((sample_rate >> 24) & 0xFFu);
    hdr[28] = (uint8_t)(byte_rate & 0xFFu);
    hdr[29] = (uint8_t)((byte_rate >> 8) & 0xFFu);
    hdr[30] = (uint8_t)((byte_rate >> 16) & 0xFFu);
    hdr[31] = (uint8_t)((byte_rate >> 24) & 0xFFu);
    hdr[32] = 4u;  hdr[33] = 0u;                             /* BlockAlign = 4 */
    hdr[34] = 16u; hdr[35] = 0u;                             /* BitsPerSample = 16 */
    memcpy(&hdr[36], "data", 4u);
    hdr[40] = (uint8_t)(data_bytes & 0xFFu);
    hdr[41] = (uint8_t)((data_bytes >> 8) & 0xFFu);
    hdr[42] = (uint8_t)((data_bytes >> 16) & 0xFFu);
    hdr[43] = (uint8_t)((data_bytes >> 24) & 0xFFu);

    bool ok = (fwrite(hdr, 1u, sizeof(hdr), fp) == sizeof(hdr)) &&
              (num_frames == 0u || fwrite(stereo_pcm, 4u, num_frames, fp) == num_frames);
    fclose(fp);
    return ok;
}
