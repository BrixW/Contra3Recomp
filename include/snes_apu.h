#ifndef CONTRA3_SNES_APU_H
#define CONTRA3_SNES_APU_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct SnesCpuContext;

#define SNES_AUDIO_SAMPLE_RATE  32000u
#define SNES_APU_RING_FRAMES    16384u

typedef enum SnesDspEnvMode {
    SNES_ENV_RELEASE = 0,
    SNES_ENV_ATTACK  = 1,
    SNES_ENV_DECAY   = 2,
    SNES_ENV_SUSTAIN = 3
} SnesDspEnvMode;

typedef struct SnesDspVoice {
    uint16_t       brr_addr;        /* Current 9-byte BRR block address in ARAM */
    uint8_t        brr_header;      /* Header byte of current BRR block */
    uint8_t        brr_offset;      /* Next sample index within decoded 16-sample block (0..16) */
    int16_t        brr_block[16];   /* Decoded 16-bit samples of active BRR block */
    int16_t        p1;              /* BRR IIR filter previous sample 1 (15-bit signed) */
    int16_t        p2;              /* BRR IIR filter previous sample 2 (15-bit signed) */
    int16_t        buf[4];          /* 4-tap Gaussian interpolation history window */
    uint16_t       interp;          /* 12-bit fractional pitch phase (0..0x0FFF) */
    int16_t        env;             /* 11-bit envelope level (0..0x07FF) */
    SnesDspEnvMode env_mode;        /* ADSR / Release envelope state */
    uint8_t        kon_delay;       /* 5-sample hardware startup delay after KON */
    bool           kon_init;        /* Pending initial 4-sample BRR preload after KON */
    int16_t        prev_out;        /* Last post-envelope 16-bit sample (for PMON & OUTX) */
} SnesDspVoice;

typedef struct SnesDsp {
    uint8_t      regs[0x80];        /* $00-$7F S-DSP register file */
    SnesDspVoice voices[8];         /* 8 hardware BRR voices */
    uint8_t      new_kon;           /* Latched KON ($4C) bits pending startup */
    uint16_t     rate_counter;      /* Global 0..30719 envelope/noise rate counter */
    int16_t      noise_sample;      /* 15-bit signed LFSR white-noise output */
    uint16_t     echo_offset;       /* Byte offset inside echo ring buffer */
    uint16_t     echo_length;       /* Total byte length of echo buffer (EDL * 2048) */
    int16_t      fir_l[8];          /* 8-tap left echo FIR history */
    int16_t      fir_r[8];          /* 8-tap right echo FIR history */
    uint8_t      fir_pos;           /* Current circular index in fir_l / fir_r (0..7) */
    uint32_t     sample_index;      /* Total 32 kHz samples ticked */
} SnesDsp;

typedef struct SnesApu {
    /* 64 KB Audio RAM + Sony SPC700 CPU Architectural State */
    uint8_t  ram[0x10000];
    uint16_t pc;
    uint8_t  a;
    uint8_t  x;
    uint8_t  y;
    uint8_t  sp;
    uint8_t  psw;
    bool     halted;                /* Set on SLEEP ($EF) or STOP ($FF) */

    /* S-SMP Internal I/O Latches ($00F0-$00FF) */
    uint8_t  f0_test;
    uint8_t  f1_control;            /* Bit 7 = IPL ROM enable, Bits 0..2 = Timers */
    uint8_t  dsp_addr;              /* $00F2 DSP register address */
    uint8_t  cpu_to_apu[4];         /* Written by 65816 ($2140-$2143), read by SPC ($F4-$F7) */
    uint8_t  apu_to_cpu[4];         /* Written by SPC ($F4-$F7), read by 65816 ($2140-$2143) */
    uint8_t  aux_io[2];             /* $00F8-$00F9 R/W registers */
    uint8_t  timer_target[3];       /* $00FA-$00FC (0 == 256) */
    uint16_t timer_stage1[3];       /* SPC cycle prescaler (128, 128, 16 cycles) */
    uint16_t timer_stage2[3];       /* 8-bit divider counting toward timer_target */
    uint8_t  timer_out[3];          /* $00FD-$00FF 4-bit counter (cleared on read) */

    /* Sony S-DSP Synthesizer */
    SnesDsp  dsp;

    /* Cycle Synchronization & Stereo 16-bit 32 kHz PCM Ring Buffer */
    uint64_t total_cycles;          /* SPC700 time; driven by the CPU timeline via snes_apu_run_to */
    uint32_t dsp_cycle_accum;       /* 32 SPC cycles == 1 S-DSP 32 kHz sample */

    int16_t  pcm_ring[SNES_APU_RING_FRAMES * 2u];
    uint32_t pcm_read_pos;
    uint32_t pcm_write_pos;
    uint32_t pcm_count;
    uint64_t total_samples_emitted;
    /* Optional host sink: when set, every sample goes here instead of the ring (lossless capture) */
    void   (*pcm_sink)(void *user, int16_t l, int16_t r);
    void    *pcm_sink_user;
    uint32_t kon_events;            /* Diagnostic count of S-DSP voice key-ons */
} SnesApu;

/* Initialize SPC700 + 64-byte IPL ROM + S-DSP to cold-boot state (runs IPL to $BBAA ready) */
void    snes_apu_init(SnesApu *apu);

/* Attach live SPC700 + S-DSP instance to SnesCpuContext ($2140-$2143) */
void    snes_apu_attach(struct SnesCpuContext *ctx, SnesApu *apu);

/* 65816 Bus Port Accessors ($2140-$2143) */
void    snes_apu_write_port(SnesApu *apu, uint8_t port, uint8_t val);
uint8_t snes_apu_read_port(SnesApu *apu, uint8_t port);

/* Step SPC700 + Timers + S-DSP by N 1.024 MHz cycles */
void    snes_apu_step_cycles(SnesApu *apu, uint32_t cycles);

/*
 * Run the SPC700 + S-DSP until total_cycles reaches spc_time (instruction granular; never
 * runs backwards). The 65816 side calls this through snes_apu_sync() before every
 * $2140-$2143 access and at every frame boundary, so both chips share one timeline and
 * exactly one 32 kHz sample is produced per 32 SPC cycles.
 */
void    snes_apu_run_to(SnesApu *apu, uint64_t spc_time);

/* Pop interleaved 16-bit stereo PCM frames (L, R) from ring buffer; returns frames popped */
size_t  snes_apu_pop_samples(SnesApu *apu, int16_t *stereo_out, size_t max_frames);
void    snes_apu_clear_samples(SnesApu *apu);

/* Write interleaved 16-bit stereo PCM buffer to standard RIFF WAVE (.wav) file */
bool    snes_write_wav(const char *path, const int16_t *stereo_pcm, size_t num_frames, uint32_t sample_rate);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_SNES_APU_H */
