/*
 * Contra III: The Alien Wars (USA) - Statically Recompiled Bank $1D
 * SNES LoROM Range : $1D:8000 - $1D:FFFF (32,768 bytes)
 * Ghidra Base Addr : 0x1d8000
 * REA Evidence ID  : ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Bank SHA-256     : cdf795a1cf5bcc8fe020038cf920bb2a6a70268a7fcddd7f48c3ef761af7a7bd
 * Functions        : 155
 */

#include "contra3_recomp.h"

/* ==========================================================================
 * Function: bank1d_func_aba0  (SNES $1D:ABA0 | Ghidra 0x1daba0)
 * Role: Math_Atan2_256 - Computes 8-bit 256-step angle from delta X/Y
 * Discovery Provenance: JSL $1DABA0 @ $02:F7C1
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_aba0(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:ABA0: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:ABA1: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $1D:ABA2: 20 AD AB    | JSR $ABAD          [M=16-bit, X=16-bit] */
    bank1d_func_abad(ctx);
    /* $1D:ABA5: C2 30       | REP #$30           [M=16-bit, X=16-bit] */
    /* REP #$30 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x30u);
    /* $1D:ABA7: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ABAA: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:ABAB: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:ABAC: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_abad  (SNES $1D:ABAD | Ghidra 0x1dabad)
 * Discovery Provenance: JSR $ABAD @ $1D:ABA2
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_abad(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:ABAD: E2 30       | SEP #$30           [M=16-bit, X=16-bit] */
    /* SEP #$30 -> Register Width Transition: [M=8-bit, X=8-bit] */
    snes_sep(ctx, 0x30u);
    /* $1D:ABAF: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABB1: D0 08       | BNE $ABBB          [M=8-bit, X=8-bit] */
    if (!ctx->flag_z) goto L_abbb;
    /* $1D:ABB3: A4 0B       | LDY $0B            [M=8-bit, X=8-bit] */
    ctx->y = (uint16_t)(snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Bu)));
    snes_set_nz8(ctx, (uint8_t)ctx->y);
    /* $1D:ABB5: 10 01       | BPL $ABB8          [M=8-bit, X=8-bit] */
    if (!ctx->flag_n) goto L_abb8;
    /* $1D:ABB7: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_abb8: /* [M=8-bit, X=8-bit] */
    /* $1D:ABB8: A9 80       | LDA #$80           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0x80u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABBA: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_abbb: /* [M=8-bit, X=8-bit] */
    /* $1D:ABBB: A5 0A       | LDA $0A            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Au)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABBD: D0 0A       | BNE $ABC9          [M=8-bit, X=8-bit] */
    if (!ctx->flag_z) goto L_abc9;
    /* $1D:ABBF: A5 09       | LDA $09            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x09u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABC1: 30 03       | BMI $ABC6          [M=8-bit, X=8-bit] */
    if (ctx->flag_n) goto L_abc6;
    /* $1D:ABC3: A9 40       | LDA #$40           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0x40u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABC5: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_abc6: /* [M=8-bit, X=8-bit] */
    /* $1D:ABC6: A9 C0       | LDA #$C0           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0xC0u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABC8: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_abc9: /* [M=8-bit, X=8-bit] */
    /* $1D:ABC9: A5 09       | LDA $09            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x09u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABCB: 30 77       | BMI $AC44          [M=8-bit, X=8-bit] */
    if (ctx->flag_n) goto L_ac44;
    /* $1D:ABCD: A5 0B       | LDA $0B            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Bu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABCF: 10 2C       | BPL $ABFD          [M=8-bit, X=8-bit] */
    if (!ctx->flag_n) goto L_abfd;
    /* $1D:ABD1: A5 0A       | LDA $0A            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Au)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABD3: 49 FF       | EOR #$FF           [M=8-bit, X=8-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) ^ (0xFFu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABD5: 1A          | INC A              [M=8-bit, X=8-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) + 1u); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABD6: 85 0A       | STA $0A            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Au), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:ABD8: C5 08       | CMP $08            [M=8-bit, X=8-bit] */
    snes_cmp8(ctx, (uint8_t)(ctx->a & 0xFFu), snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:ABDA: D0 03       | BNE $ABDF          [M=8-bit, X=8-bit] */
    if (!ctx->flag_z) goto L_abdf;
    /* $1D:ABDC: A9 20       | LDA #$20           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0x20u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABDE: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_abdf: /* [M=8-bit, X=8-bit] */
    /* $1D:ABDF: 90 0E       | BCC $ABEF          [M=8-bit, X=8-bit] */
    if (!ctx->flag_c) goto L_abef;
    /* $1D:ABE1: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABE3: 8D 05 42    | STA $4205          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVH, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:ABE6: 9C 04 42    | STZ $4204          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVL, (uint8_t)(0u));
    /* $1D:ABE9: 64 0C       | STZ $0C            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Cu), (uint8_t)(0u));
    /* $1D:ABEB: A5 0A       | LDA $0A            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Au)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABED: 80 44       | BRA $AC33          [M=8-bit, X=8-bit] */
    goto L_ac33;
L_abef: /* [M=8-bit, X=8-bit] */
    /* $1D:ABEF: 8D 05 42    | STA $4205          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVH, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:ABF2: 9C 04 42    | STZ $4204          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVL, (uint8_t)(0u));
    /* $1D:ABF5: A9 40       | LDA #$40           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0x40u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABF7: 85 0C       | STA $0C            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Cu), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:ABF9: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABFB: 80 19       | BRA $AC16          [M=8-bit, X=8-bit] */
    goto L_ac16;
L_abfd: /* [M=8-bit, X=8-bit] */
    /* $1D:ABFD: A5 0A       | LDA $0A            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Au)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ABFF: C5 08       | CMP $08            [M=8-bit, X=8-bit] */
    snes_cmp8(ctx, (uint8_t)(ctx->a & 0xFFu), snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:AC01: D0 03       | BNE $AC06          [M=8-bit, X=8-bit] */
    if (!ctx->flag_z) goto L_ac06;
    /* $1D:AC03: A9 60       | LDA #$60           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0x60u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC05: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_ac06: /* [M=8-bit, X=8-bit] */
    /* $1D:AC06: 90 1F       | BCC $AC27          [M=8-bit, X=8-bit] */
    if (!ctx->flag_c) goto L_ac27;
    /* $1D:AC08: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC0A: 8D 05 42    | STA $4205          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVH, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC0D: 9C 04 42    | STZ $4204          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVL, (uint8_t)(0u));
    /* $1D:AC10: A9 80       | LDA #$80           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0x80u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC12: 85 0C       | STA $0C            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Cu), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC14: A5 0A       | LDA $0A            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Au)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
L_ac16: /* [M=8-bit, X=8-bit] */
    /* $1D:AC16: 8D 06 42    | STA $4206          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC19: EA          | NOP                [M=8-bit, X=8-bit] */
    /* NOP */
    /* $1D:AC1A: EA          | NOP                [M=8-bit, X=8-bit] */
    /* NOP */
    /* $1D:AC1B: EA          | NOP                [M=8-bit, X=8-bit] */
    /* NOP */
    /* $1D:AC1C: EA          | NOP                [M=8-bit, X=8-bit] */
    /* NOP */
    /* $1D:AC1D: 38          | SEC                [M=8-bit, X=8-bit] */
    ctx->flag_c = 1u;
    /* $1D:AC1E: A5 0C       | LDA $0C            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Cu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC20: AC 14 42    | LDY $4214          [M=8-bit, X=8-bit] */
    ctx->y = (uint16_t)(snes_read_cpu_io(ctx, CPUIO_RDDIVL));
    snes_set_nz8(ctx, (uint8_t)ctx->y);
    /* $1D:AC23: F9 94 F6    | SBC $F694,Y        [M=8-bit, X=8-bit] */
    snes_sbc8(ctx, snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF694u)));
    /* $1D:AC26: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_ac27: /* [M=8-bit, X=8-bit] */
    /* $1D:AC27: 8D 05 42    | STA $4205          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVH, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC2A: 9C 04 42    | STZ $4204          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVL, (uint8_t)(0u));
    /* $1D:AC2D: A9 40       | LDA #$40           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0x40u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC2F: 85 0C       | STA $0C            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Cu), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC31: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
L_ac33: /* [M=8-bit, X=8-bit] */
    /* $1D:AC33: 8D 06 42    | STA $4206          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC36: EA          | NOP                [M=8-bit, X=8-bit] */
    /* NOP */
    /* $1D:AC37: EA          | NOP                [M=8-bit, X=8-bit] */
    /* NOP */
    /* $1D:AC38: EA          | NOP                [M=8-bit, X=8-bit] */
    /* NOP */
    /* $1D:AC39: EA          | NOP                [M=8-bit, X=8-bit] */
    /* NOP */
    /* $1D:AC3A: 18          | CLC                [M=8-bit, X=8-bit] */
    ctx->flag_c = 0u;
    /* $1D:AC3B: A5 0C       | LDA $0C            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Cu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC3D: AC 14 42    | LDY $4214          [M=8-bit, X=8-bit] */
    ctx->y = (uint16_t)(snes_read_cpu_io(ctx, CPUIO_RDDIVL));
    snes_set_nz8(ctx, (uint8_t)ctx->y);
    /* $1D:AC40: 79 94 F6    | ADC $F694,Y        [M=8-bit, X=8-bit] */
    snes_adc8(ctx, snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF694u)));
    /* $1D:AC43: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_ac44: /* [M=8-bit, X=8-bit] */
    /* $1D:AC44: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC46: 49 FF       | EOR #$FF           [M=8-bit, X=8-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) ^ (0xFFu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC48: 1A          | INC A              [M=8-bit, X=8-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) + 1u); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC49: 85 08       | STA $08            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x08u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC4B: A5 0B       | LDA $0B            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Bu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC4D: 30 29       | BMI $AC78          [M=8-bit, X=8-bit] */
    if (ctx->flag_n) goto L_ac78;
    /* $1D:AC4F: A5 0A       | LDA $0A            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Au)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC51: C5 08       | CMP $08            [M=8-bit, X=8-bit] */
    snes_cmp8(ctx, (uint8_t)(ctx->a & 0xFFu), snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:AC53: D0 03       | BNE $AC58          [M=8-bit, X=8-bit] */
    if (!ctx->flag_z) goto L_ac58;
    /* $1D:AC55: A9 A0       | LDA #$A0           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0xA0u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC57: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_ac58: /* [M=8-bit, X=8-bit] */
    /* $1D:AC58: 90 10       | BCC $AC6A          [M=8-bit, X=8-bit] */
    if (!ctx->flag_c) goto L_ac6a;
    /* $1D:AC5A: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC5C: 8D 05 42    | STA $4205          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVH, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC5F: 9C 04 42    | STZ $4204          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVL, (uint8_t)(0u));
    /* $1D:AC62: A9 80       | LDA #$80           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0x80u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC64: 85 0C       | STA $0C            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Cu), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC66: A5 0A       | LDA $0A            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Au)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC68: 80 C9       | BRA $AC33          [M=8-bit, X=8-bit] */
    goto L_ac33;
L_ac6a: /* [M=8-bit, X=8-bit] */
    /* $1D:AC6A: 8D 05 42    | STA $4205          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVH, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC6D: 9C 04 42    | STZ $4204          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVL, (uint8_t)(0u));
    /* $1D:AC70: A9 C0       | LDA #$C0           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0xC0u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC72: 85 0C       | STA $0C            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Cu), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC74: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC76: 80 9E       | BRA $AC16          [M=8-bit, X=8-bit] */
    goto L_ac16;
L_ac78: /* [M=8-bit, X=8-bit] */
    /* $1D:AC78: A5 0A       | LDA $0A            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Au)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC7A: 49 FF       | EOR #$FF           [M=8-bit, X=8-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) ^ (0xFFu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC7C: 1A          | INC A              [M=8-bit, X=8-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) + 1u); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC7D: 85 0A       | STA $0A            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Au), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC7F: C5 08       | CMP $08            [M=8-bit, X=8-bit] */
    snes_cmp8(ctx, (uint8_t)(ctx->a & 0xFFu), snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:AC81: D0 03       | BNE $AC86          [M=8-bit, X=8-bit] */
    if (!ctx->flag_z) goto L_ac86;
    /* $1D:AC83: A9 E0       | LDA #$E0           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0xE0u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC85: 60          | RTS                [M=8-bit, X=8-bit] */
    return;
L_ac86: /* [M=8-bit, X=8-bit] */
    /* $1D:AC86: 90 0E       | BCC $AC96          [M=8-bit, X=8-bit] */
    if (!ctx->flag_c) goto L_ac96;
    /* $1D:AC88: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC8A: 8D 05 42    | STA $4205          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVH, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC8D: 9C 04 42    | STZ $4204          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVL, (uint8_t)(0u));
    /* $1D:AC90: 64 0C       | STZ $0C            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Cu), (uint8_t)(0u));
    /* $1D:AC92: A5 0A       | LDA $0A            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x0Au)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC94: 80 80       | BRA $AC16          [M=8-bit, X=8-bit] */
    goto L_ac16;
L_ac96: /* [M=8-bit, X=8-bit] */
    /* $1D:AC96: 8D 05 42    | STA $4205          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVH, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AC99: 9C 04 42    | STZ $4204          [M=8-bit, X=8-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVL, (uint8_t)(0u));
    /* $1D:AC9C: A9 C0       | LDA #$C0           [M=8-bit, X=8-bit] */
    { uint8_t v8 = 0xC0u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AC9E: 85 0C       | STA $0C            [M=8-bit, X=8-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Cu), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:ACA0: A5 08       | LDA $08            [M=8-bit, X=8-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x08u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ACA2: 80 8F       | BRA $AC33          [M=8-bit, X=8-bit] */
    goto L_ac33;
}

/* ==========================================================================
 * Function: bank1d_func_aca4  (SNES $1D:ACA4 | Ghidra 0x1daca4)
 * Discovery Provenance: JSL $1DACA4 @ $02:B6EB
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_aca4(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:ACA4: 8C 02 42    | STY $4202          [M=16-bit, X=16-bit] */
    snes_write_cpu_io16(ctx, CPUIO_WRMPYA, (uint16_t)(ctx->y));
    /* $1D:ACA7: 89 80 00    | BIT #$0080         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0080u)) == 0u ? 1u : 0u);
    /* $1D:ACAA: D0 1D       | BNE $ACC9          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_acc9;
    /* $1D:ACAC: 89 40 00    | BIT #$0040         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0040u)) == 0u ? 1u : 0u);
    /* $1D:ACAF: D0 0C       | BNE $ACBD          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_acbd;
    /* $1D:ACB1: 20 EF AC    | JSR $ACEF          [M=16-bit, X=16-bit] */
    bank1d_func_acef(ctx);
    /* $1D:ACB4: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ACB7: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:ACB8: E5 0A       | SBC $0A            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $1D:ACBA: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:ACBC: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_acbd: /* [M=16-bit, X=16-bit] */
    /* $1D:ACBD: 20 EF AC    | JSR $ACEF          [M=16-bit, X=16-bit] */
    bank1d_func_acef(ctx);
    /* $1D:ACC0: A5 08       | LDA $08            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ACC2: A4 0A       | LDY $0A            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:ACC4: 84 08       | STY $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->y));
    /* $1D:ACC6: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:ACC8: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_acc9: /* [M=16-bit, X=16-bit] */
    /* $1D:ACC9: 89 40 00    | BIT #$0040         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0040u)) == 0u ? 1u : 0u);
    /* $1D:ACCC: D0 0C       | BNE $ACDA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_acda;
    /* $1D:ACCE: 20 EF AC    | JSR $ACEF          [M=16-bit, X=16-bit] */
    bank1d_func_acef(ctx);
    /* $1D:ACD1: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ACD4: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:ACD5: E5 08       | SBC $08            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:ACD7: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:ACD9: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_acda: /* [M=16-bit, X=16-bit] */
    /* $1D:ACDA: 20 EF AC    | JSR $ACEF          [M=16-bit, X=16-bit] */
    bank1d_func_acef(ctx);
    /* $1D:ACDD: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ACE0: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:ACE1: E5 08       | SBC $08            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:ACE3: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:ACE4: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ACE7: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:ACE8: E5 0A       | SBC $0A            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $1D:ACEA: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:ACEC: 84 0A       | STY $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->y));
    /* $1D:ACEE: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_acef  (SNES $1D:ACEF | Ghidra 0x1dacef)
 * Discovery Provenance: JSR $ACEF @ $1D:ACDA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_acef(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:ACEF: 29 3F 00    | AND #$003F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ACF2: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:ACF3: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:ACF5: B9 94 F7    | LDA $F794,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF794u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ACF8: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:ACFB: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:ACFC: 64 09       | STZ $09            [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x09u), (uint8_t)(0u));
    /* $1D:ACFE: AD 17 42    | LDA $4217          [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_read_cpu_io(ctx, CPUIO_RDMPYH); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AD01: 85 08       | STA $08            [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x08u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD03: B9 14 F8    | LDA $F814,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF814u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AD06: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD09: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AD0B: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:AD0C: A5 08       | LDA $08            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD0E: 6D 16 42    | ADC $4216          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_read_cpu_io16(ctx, CPUIO_RDMPYL));
    /* $1D:AD11: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $1D:AD12: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD15: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:AD17: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:AD19: B9 D4 F7    | LDA $F7D4,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF7D4u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AD1C: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD1F: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:AD20: 64 0B       | STZ $0B            [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Bu), (uint8_t)(0u));
    /* $1D:AD22: AD 17 42    | LDA $4217          [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_read_cpu_io(ctx, CPUIO_RDMPYH); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AD25: 85 0A       | STA $0A            [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0x0Au), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD27: B9 54 F8    | LDA $F854,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF854u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AD2A: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD2D: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AD2F: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:AD30: A5 0A       | LDA $0A            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD32: 6D 16 42    | ADC $4216          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_read_cpu_io16(ctx, CPUIO_RDMPYL));
    /* $1D:AD35: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $1D:AD36: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD39: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:AD3B: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ad3c  (SNES $1D:AD3C | Ghidra 0x1dad3c)
 * Discovery Provenance: JSL $1DAD3C @ $02:B1CC
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ad3c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:AD3C: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $1D:AD3D: A5 08       | LDA $08            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD3F: 10 04       | BPL $AD45          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_ad45;
    /* $1D:AD41: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD44: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
L_ad45: /* [M=16-bit, X=16-bit] */
    /* $1D:AD45: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $1D:AD48: B0 31       | BCS $AD7B          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ad7b;
    /* $1D:AD4A: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:AD4C: 8D 02 42    | STA $4202          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYA, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD4F: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD52: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AD54: A5 0A       | LDA $0A            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD56: 10 04       | BPL $AD5C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_ad5c;
    /* $1D:AD58: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD5B: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
L_ad5c: /* [M=16-bit, X=16-bit] */
    /* $1D:AD5C: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $1D:AD5F: B0 1A       | BCS $AD7B          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ad7b;
    /* $1D:AD61: AC 16 42    | LDY $4216          [M=16-bit, X=16-bit] */
    ctx->y = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AD64: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:AD66: 8D 02 42    | STA $4202          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYA, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD69: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD6C: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AD6E: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD6F: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AD70: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $1D:AD72: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD75: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AD76: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:AD77: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $1D:AD79: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AD7A: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_ad7b: /* [M=16-bit, X=16-bit] */
    /* $1D:AD7B: A9 FE FF    | LDA #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AD7E: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AD7F: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ad80  (SNES $1D:AD80 | Ghidra 0x1dad80)
 * Discovery Provenance: JSL $1DAD80 @ $02:A273
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ad80(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:AD80: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:AD81: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $1D:AD82: BB          | TYX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->y;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:AD83: 22 8A AD 1D | JSL $1DAD8A        [M=16-bit, X=16-bit] */
    bank1d_func_ad8a(ctx);
    /* $1D:AD87: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AD88: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:AD89: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ad8a  (SNES $1D:AD8A | Ghidra 0x1dad8a)
 * Discovery Provenance: JSL $1DAD8A @ $02:93AB
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ad8a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:AD8A: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:AD8C: 8D 02 42    | STA $4202          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYA, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AD8F: B5 04       | LDA $04,X          [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dpx(ctx, 0x04u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AD91: 89 80       | BIT #$80           [M=8-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)(((ctx->a & 0xFFu) & (0x80u)) == 0u ? 1u : 0u);
    /* $1D:AD93: F0 03       | BEQ $AD98          [M=8-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ad98;
    /* $1D:AD95: 82 92 00    | BRL $AE2A          [M=8-bit, X=16-bit] */
    goto L_ae2a;
L_ad98: /* [M=8-bit, X=16-bit] */
    /* $1D:AD98: 89 40       | BIT #$40           [M=8-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)(((ctx->a & 0xFFu) & (0x40u)) == 0u ? 1u : 0u);
    /* $1D:AD9A: D0 4C       | BNE $ADE8          [M=8-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ade8;
    /* $1D:AD9C: 29 3F       | AND #$3F           [M=8-bit, X=16-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) & (0x3Fu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AD9E: A8          | TAY                [M=8-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AD9F: B9 14 F8    | LDA $F814,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF814u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ADA2: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:ADA5: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:ADA6: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:ADA8: AD 17 42    | LDA $4217          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYH);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADAB: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADAC: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADAD: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADAE: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADAF: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADB2: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:ADB4: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADB7: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:ADB8: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:ADB9: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:ADBA: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:ADBB: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:ADBD: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:ADBF: B9 54 F8    | LDA $F854,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF854u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ADC2: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:ADC5: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:ADC6: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:ADC8: AD 17 42    | LDA $4217          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYH);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADCB: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADCC: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADCD: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADCE: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADCF: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADD2: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADD5: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:ADD7: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADDA: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:ADDB: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:ADDC: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:ADDD: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:ADDE: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADE1: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:ADE3: D0 02       | BNE $ADE7          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ade7;
    /* $1D:ADE5: F6 1E       | INC $1E,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(v16)); }
L_ade7: /* [M=16-bit, X=16-bit] */
    /* $1D:ADE7: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_ade8: /* [M=8-bit, X=16-bit] */
    /* $1D:ADE8: 29 3F       | AND #$3F           [M=8-bit, X=16-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) & (0x3Fu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ADEA: A8          | TAY                [M=8-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:ADEB: B9 54 F8    | LDA $F854,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF854u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:ADEE: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:ADF1: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:ADF2: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:ADF4: AD 17 42    | LDA $4217          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYH);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADF7: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADF8: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADF9: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADFA: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:ADFB: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:ADFE: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:AE00: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE03: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE04: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE05: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE06: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE07: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:AE09: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:AE0B: B9 14 F8    | LDA $F814,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF814u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AE0E: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AE11: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:AE12: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AE14: AD 17 42    | LDA $4217          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYH);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE17: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE18: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE19: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE1A: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE1B: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE1E: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:AE20: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE23: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE24: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE25: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE26: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE27: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:AE29: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_ae2a: /* [M=8-bit, X=16-bit] */
    /* $1D:AE2A: 89 40       | BIT #$40           [M=8-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)(((ctx->a & 0xFFu) & (0x40u)) == 0u ? 1u : 0u);
    /* $1D:AE2C: D0 4C       | BNE $AE7A          [M=8-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ae7a;
    /* $1D:AE2E: 29 3F       | AND #$3F           [M=8-bit, X=16-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) & (0x3Fu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AE30: A8          | TAY                [M=8-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AE31: B9 14 F8    | LDA $F814,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF814u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AE34: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AE37: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:AE38: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AE3A: AD 17 42    | LDA $4217          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYH);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE3D: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE3E: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE3F: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE40: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE41: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE44: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE47: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:AE49: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE4C: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE4D: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE4E: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE4F: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE50: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE53: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:AE55: D0 02       | BNE $AE59          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ae59;
    /* $1D:AE57: F6 1A       | INC $1A,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(v16)); }
L_ae59: /* [M=16-bit, X=16-bit] */
    /* $1D:AE59: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:AE5B: B9 54 F8    | LDA $F854,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF854u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AE5E: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AE61: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:AE62: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AE64: AD 17 42    | LDA $4217          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYH);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE67: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE68: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE69: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE6A: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE6B: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE6E: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:AE70: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE73: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE74: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE75: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE76: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE77: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:AE79: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_ae7a: /* [M=8-bit, X=16-bit] */
    /* $1D:AE7A: 29 3F       | AND #$3F           [M=8-bit, X=16-bit] */
    { uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) & (0x3Fu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AE7C: A8          | TAY                [M=8-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AE7D: B9 54 F8    | LDA $F854,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF854u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AE80: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AE83: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:AE84: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AE86: AD 17 42    | LDA $4217          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYH);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE89: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE8A: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE8B: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE8C: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AE8D: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE90: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE93: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:AE95: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE98: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE99: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE9A: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE9B: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AE9C: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AE9F: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:AEA1: D0 02       | BNE $AEA5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_aea5;
    /* $1D:AEA3: F6 1A       | INC $1A,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(v16)); }
L_aea5: /* [M=16-bit, X=16-bit] */
    /* $1D:AEA5: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:AEA7: B9 14 F8    | LDA $F814,Y        [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_absy(ctx, 0xF814u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $1D:AEAA: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AEAD: EA          | NOP                [M=8-bit, X=16-bit] */
    /* NOP */
    /* $1D:AEAE: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AEB0: AD 17 42    | LDA $4217          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYH);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AEB3: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AEB4: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AEB5: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AEB6: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AEB7: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AEBA: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AEBD: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:AEBF: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AEC2: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AEC3: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AEC4: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AEC5: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AEC6: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AEC9: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:AECB: D0 02       | BNE $AECF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_aecf;
    /* $1D:AECD: F6 1E       | INC $1E,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(v16)); }
L_aecf: /* [M=16-bit, X=16-bit] */
    /* $1D:AECF: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_aed0  (SNES $1D:AED0 | Ghidra 0x1daed0)
 * Discovery Provenance: JSL $1DAED0 @ $02:CC32
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_aed0(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:AED0: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $1D:AED1: A5 08       | LDA $08            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AED3: 10 04       | BPL $AED9          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_aed9;
    /* $1D:AED5: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AED8: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
L_aed9: /* [M=16-bit, X=16-bit] */
    /* $1D:AED9: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $1D:AEDC: B0 32       | BCS $AF10          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_af10;
    /* $1D:AEDE: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:AEE0: 8D 02 42    | STA $4202          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYA, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AEE3: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AEE6: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AEE8: A5 0A       | LDA $0A            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AEEA: 10 04       | BPL $AEF0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_aef0;
    /* $1D:AEEC: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AEEF: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
L_aef0: /* [M=16-bit, X=16-bit] */
    /* $1D:AEF0: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $1D:AEF3: B0 1B       | BCS $AF10          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_af10;
    /* $1D:AEF5: AC 16 42    | LDY $4216          [M=16-bit, X=16-bit] */
    ctx->y = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AEF8: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $1D:AEFA: 8D 02 42    | STA $4202          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYA, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AEFD: 8D 03 42    | STA $4203          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRMPYB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $1D:AF00: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $1D:AF02: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF03: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $1D:AF05: AD 16 42    | LDA $4216          [M=16-bit, X=16-bit] */
    ctx->a = snes_read_cpu_io16(ctx, CPUIO_RDMPYL);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF08: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:AF09: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $1D:AF0B: B0 03       | BCS $AF10          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_af10;
    /* $1D:AF0D: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AF0E: 80 05       | BRA $AF15          [M=16-bit, X=16-bit] */
    goto L_af15;
L_af10: /* [M=16-bit, X=16-bit] */
    /* $1D:AF10: A9 FF 00    | LDA #$00FF         [M=16-bit, X=16-bit] */
    ctx->a = 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF13: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AF14: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_af15: /* [M=16-bit, X=16-bit] */
    /* $1D:AF15: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $1D:AF16: A0 FF FF    | LDY #$FFFF         [M=16-bit, X=16-bit] */
    ctx->y = 0xFFFFu;
    snes_set_nz16(ctx, ctx->y);
L_af19: /* [M=16-bit, X=16-bit] */
    /* $1D:AF19: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AF1A: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AF1B: 84 FA       | STY $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->y));
    /* $1D:AF1D: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:AF1E: E5 FA       | SBC $FA            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $1D:AF20: B0 F7       | BCS $AF19          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_af19;
    /* $1D:AF22: A5 FA       | LDA $FA            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF24: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:AF25: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:AF26: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_af27  (SNES $1D:AF27 | Ghidra 0x1daf27)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_af27(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:AF27: B5 12       | LDA $12,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF29: C9 09 00    | CMP #$0009         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0009u);
    /* $1D:AF2C: B0 0A       | BCS $AF38          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_af38;
    /* $1D:AF2E: A9 8A 91    | LDA #$918A         [M=16-bit, X=16-bit] */
    ctx->a = 0x918Au;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF31: 85 9A       | STA $9A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x9Au), (uint16_t)(ctx->a));
    /* $1D:AF33: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF36: 85 98       | STA $98            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x98u), (uint16_t)(ctx->a));
L_af38: /* [M=16-bit, X=16-bit] */
    /* $1D:AF38: B5 26       | LDA $26,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x26u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF3A: F0 0A       | BEQ $AF46          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_af46;
    /* $1D:AF3C: A9 2A 91    | LDA #$912A         [M=16-bit, X=16-bit] */
    ctx->a = 0x912Au;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF3F: 85 9A       | STA $9A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x9Au), (uint16_t)(ctx->a));
    /* $1D:AF41: A9 30 B0    | LDA #$B030         [M=16-bit, X=16-bit] */
    ctx->a = 0xB030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF44: 85 98       | STA $98            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x98u), (uint16_t)(ctx->a));
L_af46: /* [M=16-bit, X=16-bit] */
    /* $1D:AF46: B5 06       | LDA $06,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF48: 10 28       | BPL $AF72          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_af72;
    /* $1D:AF4A: 74 06       | STZ $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(0u));
    /* $1D:AF4C: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF4F: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:AF51: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF54: 8D 88 16    | STA $1688          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1688u), (uint16_t)(ctx->a));
    /* $1D:AF57: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:AF59: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:AF5B: A9 0F 00    | LDA #$000F         [M=16-bit, X=16-bit] */
    ctx->a = 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF5E: 95 12       | STA $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $1D:AF60: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF63: 8D 56 09    | STA $0956          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0956u), (uint16_t)(ctx->a));
    /* $1D:AF66: A9 6F 07    | LDA #$076F         [M=16-bit, X=16-bit] */
    ctx->a = 0x076Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF69: 8F 00 E8 7E | STA $7EE800        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE800u, (uint16_t)(ctx->a));
    /* $1D:AF6D: A9 00 02    | LDA #$0200         [M=16-bit, X=16-bit] */
    ctx->a = 0x0200u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF70: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
L_af72: /* [M=16-bit, X=16-bit] */
    /* $1D:AF72: B5 12       | LDA $12,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF74: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:AF75: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:AF76: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:AF77: BF 81 AF 1D | LDA $1DAF81,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x1DAF81u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AF7B: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:AF7C: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:AF7E: 6C 00 00    | JMP ($0000)        [M=16-bit, X=16-bit] */
    {
        uint16_t jmp_tgt = snes_bus_read16(ctx, 0x000000u);
        switch (jmp_tgt) {
            case 0xAFD7u: bank1d_func_afd7(ctx); return;
            case 0xAFFEu: bank1d_func_affe(ctx); return;
            case 0xB06Bu: bank1d_func_b06b(ctx); return;
            case 0xB087u: bank1d_func_b087(ctx); return;
            case 0xB0ACu: bank1d_func_b0ac(ctx); return;
            case 0xB0C2u: bank1d_func_b0c2(ctx); return;
            case 0xB0DDu: bank1d_func_b0dd(ctx); return;
            case 0xB0E9u: bank1d_func_b0e9(ctx); return;
            case 0xB113u: bank1d_func_b113(ctx); return;
            case 0xB175u: bank1d_func_b175(ctx); return;
            case 0xB19Du: bank1d_func_b19d(ctx); return;
            case 0xB1C1u: bank1d_func_b1c1(ctx); return;
            case 0xB205u: bank1d_func_b205(ctx); return;
            case 0xB220u: bank1d_func_b220(ctx); return;
            case 0xB24Au: bank1d_func_b24a(ctx); return;
            case 0xB269u: bank1d_func_b269(ctx); return;
            case 0xB2C5u: bank1d_func_b2c5(ctx); return;
            case 0xB306u: bank1d_func_b306(ctx); return;
            case 0xB338u: bank1d_func_b338(ctx); return;
            case 0xB377u: bank1d_func_b377(ctx); return;
            case 0xB3CBu: bank1d_func_b3cb(ctx); return;
            case 0xB3FDu: bank1d_func_b3fd(ctx); return;
            case 0xB409u: bank1d_func_b409(ctx); return;
            case 0xBA8Cu: bank1d_func_ba8c(ctx); return;
            case 0xBAEAu: bank1d_func_baea(ctx); return;
            case 0xBB2Eu: bank1d_func_bb2e(ctx); return;
            case 0xBB6Bu: bank1d_func_bb6b(ctx); return;
            case 0xBB84u: bank1d_func_bb84(ctx); return;
            case 0xBB90u: bank1d_func_bb90(ctx); return;
            case 0xBBDAu: bank1d_func_bbda(ctx); return;
            case 0xBC16u: bank1d_func_bc16(ctx); return;
            case 0xBC2Au: bank1d_func_bc2a(ctx); return;
            case 0xBC59u: bank1d_func_bc59(ctx); return;
            case 0xBC97u: bank1d_func_bc97(ctx); return;
            case 0xBCBFu: bank1d_func_bcbf(ctx); return;
            case 0xBCCDu: bank1d_func_bccd(ctx); return;
            case 0xBCE3u: bank1d_func_bce3(ctx); return;
            case 0xBD1Cu: bank1d_func_bd1c(ctx); return;
            case 0xBD32u: bank1d_func_bd32(ctx); return;
            case 0xBDFBu: bank1d_func_bdfb(ctx); return;
            case 0xBE2Cu: bank1d_func_be2c(ctx); return;
            default:
                // REQUIRES INTERPRETER FALLBACK
                snes_interp_fallback_at(ctx, 0x1DAF7Eu, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);
                return;
        }
    }
}

/* ==========================================================================
 * Function: bank1d_func_afd7  (SNES $1D:AFD7 | Ghidra 0x1dafd7)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_afd7(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:AFD7: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:AFD9: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AFDC: 8D B6 16    | STA $16B6          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16B6u), (uint16_t)(ctx->a));
    /* $1D:AFDF: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AFE2: 8D D6 08    | STA $08D6          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x08D6u), (uint16_t)(ctx->a));
    /* $1D:AFE5: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AFE8: 8D FA 13    | STA $13FA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13FAu), (uint16_t)(ctx->a));
    /* $1D:AFEB: A9 80 04    | LDA #$0480         [M=16-bit, X=16-bit] */
    ctx->a = 0x0480u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AFEE: 8D FC 13    | STA $13FC          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13FCu), (uint16_t)(ctx->a));
    /* $1D:AFF1: 9C FE 13    | STZ $13FE          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13FEu), (uint16_t)(0u));
    /* $1D:AFF4: A9 F8 01    | LDA #$01F8         [M=16-bit, X=16-bit] */
    ctx->a = 0x01F8u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:AFF7: 8D C2 13    | STA $13C2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13C2u), (uint16_t)(ctx->a));
    /* $1D:AFFA: 9C F2 13    | STZ $13F2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13F2u), (uint16_t)(0u));
    /* $1D:AFFD: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_affe  (SNES $1D:AFFE | Ghidra 0x1daffe)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_affe(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:AFFE: AD FE 13    | LDA $13FE          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x13FEu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B001: F0 67       | BEQ $B06A          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b06a;
    /* $1D:B003: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B005: 9C FA 13    | STZ $13FA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13FAu), (uint16_t)(0u));
    /* $1D:B008: A9 65 00    | LDA #$0065         [M=16-bit, X=16-bit] */
    ctx->a = 0x0065u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B00B: 8D 90 02    | STA $0290          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0290u), (uint16_t)(ctx->a));
    /* $1D:B00E: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B011: 8D 96 02    | STA $0296          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0296u), (uint16_t)(ctx->a));
    /* $1D:B014: A9 78 00    | LDA #$0078         [M=16-bit, X=16-bit] */
    ctx->a = 0x0078u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B017: 8D A8 02    | STA $02A8          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x02A8u), (uint16_t)(ctx->a));
    /* $1D:B01A: A9 F8 FF    | LDA #$FFF8         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFF8u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B01D: 8D AA 02    | STA $02AA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x02AAu), (uint16_t)(ctx->a));
    /* $1D:B020: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B023: 8D 92 02    | STA $0292          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0292u), (uint16_t)(ctx->a));
    /* $1D:B026: A9 F0 FF    | LDA #$FFF0         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFF0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B029: 8D 8E 02    | STA $028E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x028Eu), (uint16_t)(ctx->a));
    /* $1D:B02C: A9 42 00    | LDA #$0042         [M=16-bit, X=16-bit] */
    ctx->a = 0x0042u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B02F: 8D 8A 02    | STA $028A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x028Au), (uint16_t)(ctx->a));
    /* $1D:B032: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B035: 8D 9C 02    | STA $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(ctx->a));
    /* $1D:B038: A9 6F 07    | LDA #$076F         [M=16-bit, X=16-bit] */
    ctx->a = 0x076Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B03B: 8F 00 E8 7E | STA $7EE800        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE800u, (uint16_t)(ctx->a));
    /* $1D:B03F: A9 E8 7F    | LDA #$7FE8         [M=16-bit, X=16-bit] */
    ctx->a = 0x7FE8u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B042: 8F 02 E8 7E | STA $7EE802        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE802u, (uint16_t)(ctx->a));
    /* $1D:B046: A9 09 E8    | LDA #$E809         [M=16-bit, X=16-bit] */
    ctx->a = 0xE809u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B049: 8F 04 E8 7E | STA $7EE804        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE804u, (uint16_t)(ctx->a));
    /* $1D:B04D: A9 00 7F    | LDA #$7F00         [M=16-bit, X=16-bit] */
    ctx->a = 0x7F00u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B050: 8F 06 E8 7E | STA $7EE806        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE806u, (uint16_t)(ctx->a));
    /* $1D:B054: 8F 08 E8 7E | STA $7EE808        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE808u, (uint16_t)(ctx->a));
    /* $1D:B058: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B05B: 8F 0A E8 7E | STA $7EE80A        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE80Au, (uint16_t)(ctx->a));
    /* $1D:B05F: A9 11 00    | LDA #$0011         [M=16-bit, X=16-bit] */
    ctx->a = 0x0011u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B062: 85 46       | STA $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(ctx->a));
    /* $1D:B064: A9 07 00    | LDA #$0007         [M=16-bit, X=16-bit] */
    ctx->a = 0x0007u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B067: 8D 72 16    | STA $1672          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1672u), (uint16_t)(ctx->a));
L_b06a: /* [M=16-bit, X=16-bit] */
    /* $1D:B06A: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b06b  (SNES $1D:B06B | Ghidra 0x1db06b)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b06b(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B06B: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:B06E: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B071: 30 13       | BMI $B086          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b086;
    /* $1D:B073: C9 6C 00    | CMP #$006C         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x006Cu);
    /* $1D:B076: 90 0E       | BCC $B086          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b086;
    /* $1D:B078: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B07A: 9C 9C 02    | STZ $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(0u));
    /* $1D:B07D: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B080: 8D B4 16    | STA $16B4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16B4u), (uint16_t)(ctx->a));
    /* $1D:B083: 9C 66 16    | STZ $1666          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1666u), (uint16_t)(0u));
L_b086: /* [M=16-bit, X=16-bit] */
    /* $1D:B086: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b087  (SNES $1D:B087 | Ghidra 0x1db087)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b087(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B087: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:B08A: AD 8A 1F    | LDA $1F8A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1F8Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B08D: F0 08       | BEQ $B097          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b097;
    /* $1D:B08F: AD 12 02    | LDA $0212          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0212u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B092: C9 01 00    | CMP #$0001         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0001u);
    /* $1D:B095: D0 14       | BNE $B0AB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b0ab;
L_b097: /* [M=16-bit, X=16-bit] */
    /* $1D:B097: AD CA 1F    | LDA $1FCA          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1FCAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B09A: F0 08       | BEQ $B0A4          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b0a4;
    /* $1D:B09C: AD 52 02    | LDA $0252          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0252u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B09F: C9 01 00    | CMP #$0001         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0001u);
    /* $1D:B0A2: D0 07       | BNE $B0AB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b0ab;
L_b0a4: /* [M=16-bit, X=16-bit] */
    /* $1D:B0A4: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B0A6: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B0A9: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
L_b0ab: /* [M=16-bit, X=16-bit] */
    /* $1D:B0AB: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b0ac  (SNES $1D:B0AC | Ghidra 0x1db0ac)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b0ac(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B0AC: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:B0AF: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:B0B1: 10 0E       | BPL $B0C1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b0c1;
    /* $1D:B0B3: A9 FF FF    | LDA #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B0B6: 8D 9E 02    | STA $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(ctx->a));
    /* $1D:B0B9: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B0BC: 8D 9C 02    | STA $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(ctx->a));
    /* $1D:B0BF: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_b0c1: /* [M=16-bit, X=16-bit] */
    /* $1D:B0C1: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b0c2  (SNES $1D:B0C2 | Ghidra 0x1db0c2)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b0c2(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B0C2: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:B0C5: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B0C8: 10 12       | BPL $B0DC          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b0dc;
    /* $1D:B0CA: C9 DD FF    | CMP #$FFDD         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFDDu);
    /* $1D:B0CD: B0 0D       | BCS $B0DC          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b0dc;
    /* $1D:B0CF: 9C 9C 02    | STZ $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(0u));
    /* $1D:B0D2: 9C 9E 02    | STZ $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(0u));
    /* $1D:B0D5: A9 80 00    | LDA #$0080         [M=16-bit, X=16-bit] */
    ctx->a = 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B0D8: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:B0DA: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_b0dc: /* [M=16-bit, X=16-bit] */
    /* $1D:B0DC: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b0dd  (SNES $1D:B0DD | Ghidra 0x1db0dd)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b0dd(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B0DD: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:B0DF: 10 07       | BPL $B0E8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b0e8;
    /* $1D:B0E1: A9 80 00    | LDA #$0080         [M=16-bit, X=16-bit] */
    ctx->a = 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B0E4: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:B0E6: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_b0e8: /* [M=16-bit, X=16-bit] */
    /* $1D:B0E8: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b0e9  (SNES $1D:B0E9 | Ghidra 0x1db0e9)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b0e9(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B0E9: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:B0EB: 10 15       | BPL $B102          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b102;
    /* $1D:B0ED: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B0EF: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:B0F2: F0 0B       | BEQ $B0FF          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b0ff;
    /* $1D:B0F4: A9 00 01    | LDA #$0100         [M=16-bit, X=16-bit] */
    ctx->a = 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B0F7: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:B0F9: A9 25 00    | LDA #$0025         [M=16-bit, X=16-bit] */
    ctx->a = 0x0025u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B0FC: 95 12       | STA $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $1D:B0FE: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_b0ff: /* [M=16-bit, X=16-bit] */
    /* $1D:B0FF: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B101: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_b102: /* [M=16-bit, X=16-bit] */
    /* $1D:B102: A0 59 FF    | LDY #$FF59         [M=16-bit, X=16-bit] */
    ctx->y = 0xFF59u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B105: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B107: 89 08 00    | BIT #$0008         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0008u)) == 0u ? 1u : 0u);
    /* $1D:B10A: D0 03       | BNE $B10F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b10f;
    /* $1D:B10C: A0 5D FF    | LDY #$FF5D         [M=16-bit, X=16-bit] */
    ctx->y = 0xFF5Du;
    snes_set_nz16(ctx, ctx->y);
L_b10f: /* [M=16-bit, X=16-bit] */
    /* $1D:B10F: 20 1C CC    | JSR $CC1C          [M=16-bit, X=16-bit] */
    bank1d_func_cc1c(ctx);
    /* $1D:B112: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b113  (SNES $1D:B113 | Ghidra 0x1db113)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b113(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B113: A9 4A 00    | LDA #$004A         [M=16-bit, X=16-bit] */
    ctx->a = 0x004Au;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B116: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:B11A: 9C 36 16    | STZ $1636          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1636u), (uint16_t)(0u));
    /* $1D:B11D: A9 10 00    | LDA #$0010         [M=16-bit, X=16-bit] */
    ctx->a = 0x0010u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B120: 8D 38 16    | STA $1638          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1638u), (uint16_t)(ctx->a));
    /* $1D:B123: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B125: 20 1A B8    | JSR $B81A          [M=16-bit, X=16-bit] */
    bank1d_func_b81a(ctx);
    /* $1D:B128: 74 30       | STZ $30,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x30u), (uint16_t)(0u));
    /* $1D:B12A: A9 1D 40    | LDA #$401D         [M=16-bit, X=16-bit] */
    ctx->a = 0x401Du;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B12D: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:B12F: A9 2C 00    | LDA #$002C         [M=16-bit, X=16-bit] */
    ctx->a = 0x002Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B132: 95 28       | STA $28,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x28u), (uint16_t)(ctx->a));
    /* $1D:B134: A9 B0 FF    | LDA #$FFB0         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFB0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B137: 95 2A       | STA $2A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Au), (uint16_t)(ctx->a));
    /* $1D:B139: A9 6A 00    | LDA #$006A         [M=16-bit, X=16-bit] */
    ctx->a = 0x006Au;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B13C: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:B13E: A9 C0 00    | LDA #$00C0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00C0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B141: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:B143: A9 80 01    | LDA #$0180         [M=16-bit, X=16-bit] */
    ctx->a = 0x0180u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B146: 95 06       | STA $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $1D:B148: 74 34       | STZ $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(0u));
    /* $1D:B14A: 74 36       | STZ $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(0u));
    /* $1D:B14C: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B14F: 95 3A       | STA $3A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(ctx->a));
    /* $1D:B151: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B152: A2 40 09    | LDX #$0940         [M=16-bit, X=16-bit] */
    ctx->x = 0x0940u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B155: A9 53 00    | LDA #$0053         [M=16-bit, X=16-bit] */
    ctx->a = 0x0053u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B158: 95 10       | STA $10,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x10u), (uint16_t)(ctx->a));
    /* $1D:B15A: A9 3C 00    | LDA #$003C         [M=16-bit, X=16-bit] */
    ctx->a = 0x003Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B15D: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:B15F: A9 7C 00    | LDA #$007C         [M=16-bit, X=16-bit] */
    ctx->a = 0x007Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B162: 95 28       | STA $28,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x28u), (uint16_t)(ctx->a));
    /* $1D:B164: A9 D8 FF    | LDA #$FFD8         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFD8u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B167: 95 2A       | STA $2A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Au), (uint16_t)(ctx->a));
    /* $1D:B169: A9 42 00    | LDA #$0042         [M=16-bit, X=16-bit] */
    ctx->a = 0x0042u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B16C: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:B16E: A9 C0 00    | LDA #$00C0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00C0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B171: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:B173: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B174: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b175  (SNES $1D:B175 | Ghidra 0x1db175)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b175(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B175: 64 00       | STZ $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(0u));
    /* $1D:B177: A9 FF 00    | LDA #$00FF         [M=16-bit, X=16-bit] */
    ctx->a = 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B17A: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $1D:B17C: 20 68 BA    | JSR $BA68          [M=16-bit, X=16-bit] */
    bank1d_func_ba68(ctx);
    /* $1D:B17F: 20 72 B4    | JSR $B472          [M=16-bit, X=16-bit] */
    bank1d_func_b472(ctx);
    /* $1D:B182: AD 36 16    | LDA $1636          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B185: C9 12 00    | CMP #$0012         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0012u);
    /* $1D:B188: 90 12       | BCC $B19C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b19c;
    /* $1D:B18A: A9 49 00    | LDA #$0049         [M=16-bit, X=16-bit] */
    ctx->a = 0x0049u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B18D: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:B191: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B193: 20 42 B6    | JSR $B642          [M=16-bit, X=16-bit] */
    bank1d_func_b642(ctx);
    /* $1D:B196: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B199: 8D B6 16    | STA $16B6          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16B6u), (uint16_t)(ctx->a));
L_b19c: /* [M=16-bit, X=16-bit] */
    /* $1D:B19C: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b19d  (SNES $1D:B19D | Ghidra 0x1db19d)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b19d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B19D: AD 98 13    | LDA $1398          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1398u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1A0: 89 FF 00    | BIT #$00FF         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x00FFu)) == 0u ? 1u : 0u);
    /* $1D:B1A3: F0 2B       | BEQ $B1D0          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b1d0;
    /* $1D:B1A5: 89 80 00    | BIT #$0080         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0080u)) == 0u ? 1u : 0u);
    /* $1D:B1A8: D0 26       | BNE $B1D0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b1d0;
    /* $1D:B1AA: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B1AC: 9C 9C 02    | STZ $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(0u));
    /* $1D:B1AF: 9C 9E 02    | STZ $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(0u));
    /* $1D:B1B2: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:B1B4: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:B1B6: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1B9: 8D BA 13    | STA $13BA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13BAu), (uint16_t)(ctx->a));
    /* $1D:B1BC: A9 00 01    | LDA #$0100         [M=16-bit, X=16-bit] */
    ctx->a = 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1BF: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* Fallthrough into function $1D:B1C1 */
    bank1d_func_b1c1(ctx);
    return;
L_b1d0: /* [M=16-bit, X=16-bit] */
    /* $1D:B1D0: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1D2: D0 1E       | BNE $B1F2          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b1f2;
    /* $1D:B1D4: AD A2 16    | LDA $16A2          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x16A2u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1D7: C9 0A 00    | CMP #$000A         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x000Au);
    /* $1D:B1DA: B0 07       | BCS $B1E3          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b1e3;
    /* $1D:B1DC: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1DD: 8D A2 16    | STA $16A2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A2u), (uint16_t)(ctx->a));
    /* $1D:B1E0: 8D A4 16    | STA $16A4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A4u), (uint16_t)(ctx->a));
L_b1e3: /* [M=16-bit, X=16-bit] */
    /* $1D:B1E3: AD A6 16    | LDA $16A6          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x16A6u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1E6: C9 0C 00    | CMP #$000C         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x000Cu);
    /* $1D:B1E9: B0 07       | BCS $B1F2          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b1f2;
    /* $1D:B1EB: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1EC: 8D A6 16    | STA $16A6          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A6u), (uint16_t)(ctx->a));
    /* $1D:B1EF: 8D A8 16    | STA $16A8          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A8u), (uint16_t)(ctx->a));
L_b1f2: /* [M=16-bit, X=16-bit] */
    /* $1D:B1F2: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1F5: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:B1F8: 20 4A B4    | JSR $B44A          [M=16-bit, X=16-bit] */
    bank1d_func_b44a(ctx);
    /* $1D:B1FB: 20 E2 B4    | JSR $B4E2          [M=16-bit, X=16-bit] */
    bank1d_func_b4e2(ctx);
    /* $1D:B1FE: 20 F5 B4    | JSR $B4F5          [M=16-bit, X=16-bit] */
    bank1d_func_b4f5(ctx);
    /* $1D:B201: 20 ED B5    | JSR $B5ED          [M=16-bit, X=16-bit] */
    bank1d_func_b5ed(ctx);
    /* $1D:B204: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b1c1  (SNES $1D:B1C1 | Ghidra 0x1db1c1)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b1c1(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B1C1: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:B1C4: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:B1C6: 10 08       | BPL $B1D0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b1d0;
    /* $1D:B1C8: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B1CA: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1CD: 8D 9C 02    | STA $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(ctx->a));
L_b1d0: /* [M=16-bit, X=16-bit] */
    /* $1D:B1D0: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1D2: D0 1E       | BNE $B1F2          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b1f2;
    /* $1D:B1D4: AD A2 16    | LDA $16A2          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x16A2u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1D7: C9 0A 00    | CMP #$000A         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x000Au);
    /* $1D:B1DA: B0 07       | BCS $B1E3          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b1e3;
    /* $1D:B1DC: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1DD: 8D A2 16    | STA $16A2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A2u), (uint16_t)(ctx->a));
    /* $1D:B1E0: 8D A4 16    | STA $16A4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A4u), (uint16_t)(ctx->a));
L_b1e3: /* [M=16-bit, X=16-bit] */
    /* $1D:B1E3: AD A6 16    | LDA $16A6          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x16A6u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1E6: C9 0C 00    | CMP #$000C         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x000Cu);
    /* $1D:B1E9: B0 07       | BCS $B1F2          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b1f2;
    /* $1D:B1EB: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1EC: 8D A6 16    | STA $16A6          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A6u), (uint16_t)(ctx->a));
    /* $1D:B1EF: 8D A8 16    | STA $16A8          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A8u), (uint16_t)(ctx->a));
L_b1f2: /* [M=16-bit, X=16-bit] */
    /* $1D:B1F2: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B1F5: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:B1F8: 20 4A B4    | JSR $B44A          [M=16-bit, X=16-bit] */
    bank1d_func_b44a(ctx);
    /* $1D:B1FB: 20 E2 B4    | JSR $B4E2          [M=16-bit, X=16-bit] */
    bank1d_func_b4e2(ctx);
    /* $1D:B1FE: 20 F5 B4    | JSR $B4F5          [M=16-bit, X=16-bit] */
    bank1d_func_b4f5(ctx);
    /* $1D:B201: 20 ED B5    | JSR $B5ED          [M=16-bit, X=16-bit] */
    bank1d_func_b5ed(ctx);
    /* $1D:B204: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b205  (SNES $1D:B205 | Ghidra 0x1db205)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b205(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B205: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B208: 30 2D       | BMI $B237          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b237;
    /* $1D:B20A: C9 40 00    | CMP #$0040         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0040u);
    /* $1D:B20D: 90 28       | BCC $B237          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b237;
    /* $1D:B20F: 9C B4 16    | STZ $16B4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16B4u), (uint16_t)(0u));
    /* $1D:B212: A0 00 02    | LDY #$0200         [M=16-bit, X=16-bit] */
    ctx->y = 0x0200u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B215: 20 41 B7    | JSR $B741          [M=16-bit, X=16-bit] */
    bank1d_func_b741(ctx);
    /* $1D:B218: A0 40 02    | LDY #$0240         [M=16-bit, X=16-bit] */
    ctx->y = 0x0240u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B21B: 20 41 B7    | JSR $B741          [M=16-bit, X=16-bit] */
    bank1d_func_b741(ctx);
    /* $1D:B21E: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* Fallthrough into function $1D:B220 */
    bank1d_func_b220(ctx);
    return;
L_b237: /* [M=16-bit, X=16-bit] */
    /* $1D:B237: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:B23A: 20 4A B4    | JSR $B44A          [M=16-bit, X=16-bit] */
    bank1d_func_b44a(ctx);
    /* $1D:B23D: 20 E2 B4    | JSR $B4E2          [M=16-bit, X=16-bit] */
    bank1d_func_b4e2(ctx);
    /* $1D:B240: 20 F5 B4    | JSR $B4F5          [M=16-bit, X=16-bit] */
    bank1d_func_b4f5(ctx);
    /* $1D:B243: 20 ED B5    | JSR $B5ED          [M=16-bit, X=16-bit] */
    bank1d_func_b5ed(ctx);
    /* $1D:B246: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:B249: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b220  (SNES $1D:B220 | Ghidra 0x1db220)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b220(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B220: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B222: 89 FF 01    | BIT #$01FF         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x01FFu)) == 0u ? 1u : 0u);
    /* $1D:B225: D0 07       | BNE $B22E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b22e;
    /* $1D:B227: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B229: A9 00 04    | LDA #$0400         [M=16-bit, X=16-bit] */
    ctx->a = 0x0400u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B22C: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
L_b22e: /* [M=16-bit, X=16-bit] */
    /* $1D:B22E: 20 A2 B5    | JSR $B5A2          [M=16-bit, X=16-bit] */
    bank1d_func_b5a2(ctx);
    /* $1D:B231: 20 58 B7    | JSR $B758          [M=16-bit, X=16-bit] */
    bank1d_func_b758(ctx);
    /* $1D:B234: 20 0F B7    | JSR $B70F          [M=16-bit, X=16-bit] */
    bank1d_func_b70f(ctx);
    /* $1D:B237: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:B23A: 20 4A B4    | JSR $B44A          [M=16-bit, X=16-bit] */
    bank1d_func_b44a(ctx);
    /* $1D:B23D: 20 E2 B4    | JSR $B4E2          [M=16-bit, X=16-bit] */
    bank1d_func_b4e2(ctx);
    /* $1D:B240: 20 F5 B4    | JSR $B4F5          [M=16-bit, X=16-bit] */
    bank1d_func_b4f5(ctx);
    /* $1D:B243: 20 ED B5    | JSR $B5ED          [M=16-bit, X=16-bit] */
    bank1d_func_b5ed(ctx);
    /* $1D:B246: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:B249: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b24a  (SNES $1D:B24A | Ghidra 0x1db24a)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b24a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B24A: 20 58 B7    | JSR $B758          [M=16-bit, X=16-bit] */
    bank1d_func_b758(ctx);
    /* $1D:B24D: 20 0F B7    | JSR $B70F          [M=16-bit, X=16-bit] */
    bank1d_func_b70f(ctx);
    /* $1D:B250: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:B253: 20 4A B4    | JSR $B44A          [M=16-bit, X=16-bit] */
    bank1d_func_b44a(ctx);
    /* $1D:B256: 20 E2 B4    | JSR $B4E2          [M=16-bit, X=16-bit] */
    bank1d_func_b4e2(ctx);
    /* $1D:B259: 20 09 B5    | JSR $B509          [M=16-bit, X=16-bit] */
    bank1d_func_b509(ctx);
    /* $1D:B25C: 20 ED B5    | JSR $B5ED          [M=16-bit, X=16-bit] */
    bank1d_func_b5ed(ctx);
    /* $1D:B25F: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:B262: D6 36       | DEC $36,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(v16)); }
    /* $1D:B264: 10 02       | BPL $B268          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b268;
    /* $1D:B266: D6 12       | DEC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_b268: /* [M=16-bit, X=16-bit] */
    /* $1D:B268: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b269  (SNES $1D:B269 | Ghidra 0x1db269)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b269(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B269: AD 98 13    | LDA $1398          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1398u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B26C: 29 FF 01    | AND #$01FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x01FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B26F: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:B272: B0 47       | BCS $B2BB          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b2bb;
    /* $1D:B274: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B275: 69 00 02    | ADC #$0200         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0200u);
    /* $1D:B278: 8D 98 13    | STA $1398          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1398u), (uint16_t)(ctx->a));
    /* $1D:B27B: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B27C: 69 00 01    | ADC #$0100         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0100u);
    /* $1D:B27F: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B282: 8D B0 13    | STA $13B0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13B0u), (uint16_t)(ctx->a));
    /* $1D:B285: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B287: A9 06 00    | LDA #$0006         [M=16-bit, X=16-bit] */
    ctx->a = 0x0006u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B28A: 8D BA 13    | STA $13BA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13BAu), (uint16_t)(ctx->a));
    /* $1D:B28D: A9 12 00    | LDA #$0012         [M=16-bit, X=16-bit] */
    ctx->a = 0x0012u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B290: 85 46       | STA $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(ctx->a));
    /* $1D:B292: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B295: 8D 9C 1E    | STA $1E9C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E9Cu), (uint16_t)(ctx->a));
    /* $1D:B298: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B29B: 8D B4 16    | STA $16B4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16B4u), (uint16_t)(ctx->a));
    /* $1D:B29E: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B2A1: 8D 5E 1E    | STA $1E5E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E5Eu), (uint16_t)(ctx->a));
    /* $1D:B2A4: A9 00 60    | LDA #$6000         [M=16-bit, X=16-bit] */
    ctx->a = 0x6000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B2A7: 8D 36 16    | STA $1636          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1636u), (uint16_t)(ctx->a));
    /* $1D:B2AA: A9 FF FF    | LDA #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B2AD: 8D 38 16    | STA $1638          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1638u), (uint16_t)(ctx->a));
    /* $1D:B2B0: 22 06 B3 1D | JSL $1DB306        [M=16-bit, X=16-bit] */
    bank1d_func_b306(ctx);
    /* $1D:B2B4: AD D8 13    | LDA $13D8          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x13D8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B2B7: 8D E2 13    | STA $13E2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13E2u), (uint16_t)(ctx->a));
    /* $1D:B2BA: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_b2bb: /* [M=16-bit, X=16-bit] */
    /* $1D:B2BB: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:B2BE: 20 58 B7    | JSR $B758          [M=16-bit, X=16-bit] */
    bank1d_func_b758(ctx);
    /* $1D:B2C1: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:B2C4: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b2c5  (SNES $1D:B2C5 | Ghidra 0x1db2c5)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b2c5(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B2C5: 20 4A B4    | JSR $B44A          [M=16-bit, X=16-bit] */
    bank1d_func_b44a(ctx);
    /* $1D:B2C8: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:B2CA: 10 25       | BPL $B2F1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b2f1;
    /* $1D:B2CC: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B2CE: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B2CF: A2 C3 F8    | LDX #$F8C3         [M=16-bit, X=16-bit] */
    ctx->x = 0xF8C3u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B2D2: 22 A8 90 00 | JSL $0090A8        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_90a8(ctx); ctx->pb = saved_pb; }
    /* $1D:B2D6: A0 DA F8    | LDY #$F8DA         [M=16-bit, X=16-bit] */
    ctx->y = 0xF8DAu;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B2D9: 22 82 99 00 | JSL $009982        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9982(ctx); ctx->pb = saved_pb; }
    /* $1D:B2DD: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B2DE: 74 0A       | STZ $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(0u));
    /* $1D:B2E0: A9 80 01    | LDA #$0180         [M=16-bit, X=16-bit] */
    ctx->a = 0x0180u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B2E3: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:B2E5: A9 00 02    | LDA #$0200         [M=16-bit, X=16-bit] */
    ctx->a = 0x0200u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B2E8: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:B2EA: A9 00 01    | LDA #$0100         [M=16-bit, X=16-bit] */
    ctx->a = 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B2ED: 8F 09 E8 7E | STA $7EE809        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE809u, (uint16_t)(ctx->a));
L_b2f1: /* [M=16-bit, X=16-bit] */
    /* $1D:B2F1: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:B2F4: 20 58 B7    | JSR $B758          [M=16-bit, X=16-bit] */
    bank1d_func_b758(ctx);
    /* $1D:B2F7: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:B2FA: AD 36 16    | LDA $1636          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B2FD: C9 14 00    | CMP #$0014         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0014u);
    /* $1D:B300: B0 03       | BCS $B305          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b305;
    /* $1D:B302: 20 72 B4    | JSR $B472          [M=16-bit, X=16-bit] */
    bank1d_func_b472(ctx);
L_b305: /* [M=16-bit, X=16-bit] */
    /* $1D:B305: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b306  (SNES $1D:B306 | Ghidra 0x1db306)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b306(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B306: 20 DE B7    | JSR $B7DE          [M=16-bit, X=16-bit] */
    bank1d_func_b7de(ctx);
    /* $1D:B309: 20 B6 B6    | JSR $B6B6          [M=16-bit, X=16-bit] */
    bank1d_func_b6b6(ctx);
    /* $1D:B30C: A9 64 00    | LDA #$0064         [M=16-bit, X=16-bit] */
    ctx->a = 0x0064u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B30F: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:B311: A9 80 00    | LDA #$0080         [M=16-bit, X=16-bit] */
    ctx->a = 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B314: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:B316: 20 62 B7    | JSR $B762          [M=16-bit, X=16-bit] */
    bank1d_func_b762(ctx);
    /* $1D:B319: 20 3F B4    | JSR $B43F          [M=16-bit, X=16-bit] */
    bank1d_func_b43f(ctx);
    /* $1D:B31C: AD 98 13    | LDA $1398          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1398u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B31F: 10 16       | BPL $B337          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b337;
    /* $1D:B321: 9C 98 13    | STZ $1398          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1398u), (uint16_t)(0u));
    /* $1D:B324: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B327: 8D BA 13    | STA $13BA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13BAu), (uint16_t)(ctx->a));
    /* $1D:B32A: A9 C0 00    | LDA #$00C0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00C0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B32D: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:B32F: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B331: 9C 3A 16    | STZ $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(0u));
    /* $1D:B334: 9C 3C 16    | STZ $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(0u));
L_b337: /* [M=16-bit, X=16-bit] */
    /* $1D:B337: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b338  (SNES $1D:B338 | Ghidra 0x1db338)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b338(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B338: 20 F1 B6    | JSR $B6F1          [M=16-bit, X=16-bit] */
    bank1d_func_b6f1(ctx);
    /* $1D:B33B: AD 3E 16    | LDA $163E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B33E: 30 20       | BMI $B360          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b360;
    /* $1D:B340: C9 46 00    | CMP #$0046         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0046u);
    /* $1D:B343: 90 1B       | BCC $B360          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b360;
    /* $1D:B345: 9C 3A 16    | STZ $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(0u));
    /* $1D:B348: 9C 3C 16    | STZ $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(0u));
    /* $1D:B34B: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B34D: A9 06 00    | LDA #$0006         [M=16-bit, X=16-bit] */
    ctx->a = 0x0006u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B350: 8D BA 13    | STA $13BA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13BAu), (uint16_t)(ctx->a));
    /* $1D:B353: A9 00 09    | LDA #$0900         [M=16-bit, X=16-bit] */
    ctx->a = 0x0900u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B356: 8D BC 13    | STA $13BC          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13BCu), (uint16_t)(ctx->a));
    /* $1D:B359: 8D B0 13    | STA $13B0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13B0u), (uint16_t)(ctx->a));
    /* $1D:B35C: 22 1B E2 04 | JSL $04E21B        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x04u; bank04_func_e21b(ctx); ctx->pb = saved_pb; }
L_b360: /* [M=16-bit, X=16-bit] */
    /* $1D:B360: 20 DE B7    | JSR $B7DE          [M=16-bit, X=16-bit] */
    bank1d_func_b7de(ctx);
    /* $1D:B363: 20 8D B9    | JSR $B98D          [M=16-bit, X=16-bit] */
    bank1d_func_b98d(ctx);
    /* $1D:B366: A9 74 00    | LDA #$0074         [M=16-bit, X=16-bit] */
    ctx->a = 0x0074u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B369: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:B36B: A9 80 00    | LDA #$0080         [M=16-bit, X=16-bit] */
    ctx->a = 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B36E: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:B370: 20 62 B7    | JSR $B762          [M=16-bit, X=16-bit] */
    bank1d_func_b762(ctx);
    /* $1D:B373: 20 3F B4    | JSR $B43F          [M=16-bit, X=16-bit] */
    bank1d_func_b43f(ctx);
    /* $1D:B376: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b377  (SNES $1D:B377 | Ghidra 0x1db377)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b377(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B377: 20 D3 B6    | JSR $B6D3          [M=16-bit, X=16-bit] */
    bank1d_func_b6d3(ctx);
    /* $1D:B37A: 20 68 B6    | JSR $B668          [M=16-bit, X=16-bit] */
    bank1d_func_b668(ctx);
    /* $1D:B37D: 20 8D B9    | JSR $B98D          [M=16-bit, X=16-bit] */
    bank1d_func_b98d(ctx);
    /* $1D:B380: A9 88 00    | LDA #$0088         [M=16-bit, X=16-bit] */
    ctx->a = 0x0088u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B383: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:B385: A9 90 00    | LDA #$0090         [M=16-bit, X=16-bit] */
    ctx->a = 0x0090u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B388: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:B38A: 20 62 B7    | JSR $B762          [M=16-bit, X=16-bit] */
    bank1d_func_b762(ctx);
    /* $1D:B38D: 20 3F B4    | JSR $B43F          [M=16-bit, X=16-bit] */
    bank1d_func_b43f(ctx);
    /* $1D:B390: 20 DE B7    | JSR $B7DE          [M=16-bit, X=16-bit] */
    bank1d_func_b7de(ctx);
    /* $1D:B393: A9 00 01    | LDA #$0100         [M=16-bit, X=16-bit] */
    ctx->a = 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B396: CD 98 13    | CMP $1398          [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1398u)));
    /* $1D:B399: B0 2F       | BCS $B3CA          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b3ca;
    /* $1D:B39B: 8D 98 13    | STA $1398          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1398u), (uint16_t)(ctx->a));
    /* $1D:B39E: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B3A0: AD 80 13    | LDA $1380          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1380u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3A3: 29 00 01    | AND #$0100         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3A6: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:B3A7: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:B3A8: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:B3A9: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B3AA: 69 40 46    | ADC #$4640         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x4640u);
    /* $1D:B3AD: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B3AE: A9 02 20    | LDA #$2002         [M=16-bit, X=16-bit] */
    ctx->a = 0x2002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3B1: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:B3B3: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3B6: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $1D:B3B8: A9 20 00    | LDA #$0020         [M=16-bit, X=16-bit] */
    ctx->a = 0x0020u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3BB: 22 AF CF 00 | JSL $00CFAF        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_cfaf(ctx); ctx->pb = saved_pb; }
    /* $1D:B3BF: A9 60 00    | LDA #$0060         [M=16-bit, X=16-bit] */
    ctx->a = 0x0060u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3C2: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:B3C4: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3C7: 8D B4 16    | STA $16B4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16B4u), (uint16_t)(ctx->a));
L_b3ca: /* [M=16-bit, X=16-bit] */
    /* $1D:B3CA: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b3cb  (SNES $1D:B3CB | Ghidra 0x1db3cb)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b3cb(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B3CB: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:B3CD: 10 3A       | BPL $B409          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) { bank1d_func_b409(ctx); return; }
    /* $1D:B3CF: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:B3D1: AD 8A 1F    | LDA $1F8A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1F8Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3D4: F0 0C       | BEQ $B3E2          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b3e2;
    /* $1D:B3D6: A9 13 00    | LDA #$0013         [M=16-bit, X=16-bit] */
    ctx->a = 0x0013u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3D9: 8D 12 02    | STA $0212          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0212u), (uint16_t)(ctx->a));
    /* $1D:B3DC: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3DF: 8D 34 02    | STA $0234          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0234u), (uint16_t)(ctx->a));
L_b3e2: /* [M=16-bit, X=16-bit] */
    /* $1D:B3E2: AD CA 1F    | LDA $1FCA          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1FCAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3E5: F0 0C       | BEQ $B3F3          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b3f3;
    /* $1D:B3E7: A9 13 00    | LDA #$0013         [M=16-bit, X=16-bit] */
    ctx->a = 0x0013u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3EA: 8D 52 02    | STA $0252          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0252u), (uint16_t)(ctx->a));
    /* $1D:B3ED: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3F0: 8D 74 02    | STA $0274          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0274u), (uint16_t)(ctx->a));
L_b3f3: /* [M=16-bit, X=16-bit] */
    /* $1D:B3F3: 9C B4 16    | STZ $16B4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16B4u), (uint16_t)(0u));
    /* $1D:B3F6: A9 F0 01    | LDA #$01F0         [M=16-bit, X=16-bit] */
    ctx->a = 0x01F0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B3F9: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:B3FB: 80 0C       | BRA $B409          [M=16-bit, X=16-bit] */
    bank1d_func_b409(ctx);
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b3fd  (SNES $1D:B3FD | Ghidra 0x1db3fd)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b3fd(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B3FD: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:B3FF: 10 08       | BPL $B409          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) { bank1d_func_b409(ctx); return; }
    /* $1D:B401: A9 00 01    | LDA #$0100         [M=16-bit, X=16-bit] */
    ctx->a = 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B404: 8D 86 16    | STA $1686          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1686u), (uint16_t)(ctx->a));
    /* $1D:B407: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* Fallthrough into function $1D:B409 */
    bank1d_func_b409(ctx);
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b409  (SNES $1D:B409 | Ghidra 0x1db409)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b409(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B409: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B40C: 10 04       | BPL $B412          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b412;
    /* $1D:B40E: 22 CB FC 02 | JSL $02FCCB        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x02u; bank02_func_fccb(ctx); ctx->pb = saved_pb; }
L_b412: /* [M=16-bit, X=16-bit] */
    /* $1D:B412: 20 D3 B6    | JSR $B6D3          [M=16-bit, X=16-bit] */
    bank1d_func_b6d3(ctx);
    /* $1D:B415: 20 B9 B7    | JSR $B7B9          [M=16-bit, X=16-bit] */
    bank1d_func_b7b9(ctx);
    /* $1D:B418: 20 3F B4    | JSR $B43F          [M=16-bit, X=16-bit] */
    bank1d_func_b43f(ctx);
    /* $1D:B41B: 20 DE B7    | JSR $B7DE          [M=16-bit, X=16-bit] */
    bank1d_func_b7de(ctx);
    /* $1D:B41E: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b41f  (SNES $1D:B41F | Ghidra 0x1db41f)
 * Discovery Provenance: JSR $B41F @ $1D:B06B
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b41f(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B41F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B420: A9 77 00    | LDA #$0077         [M=16-bit, X=16-bit] */
    ctx->a = 0x0077u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B423: ED 8E 02    | SBC $028E          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu)));
    /* $1D:B426: 8F 07 E8 7E | STA $7EE807        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE807u, (uint16_t)(ctx->a));
    /* $1D:B42A: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B42D: 30 05       | BMI $B434          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b434;
    /* $1D:B42F: C9 08 00    | CMP #$0008         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0008u);
    /* $1D:B432: B0 03       | BCS $B437          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b437;
L_b434: /* [M=16-bit, X=16-bit] */
    /* $1D:B434: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
L_b437: /* [M=16-bit, X=16-bit] */
    /* $1D:B437: 09 00 07    | ORA #$0700         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0700u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B43A: 8F 00 E8 7E | STA $7EE800        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE800u, (uint16_t)(ctx->a));
    /* $1D:B43E: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b43f  (SNES $1D:B43F | Ghidra 0x1db43f)
 * Discovery Provenance: JSR $B43F @ $1D:B418
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b43f(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B43F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B440: A9 78 00    | LDA #$0078         [M=16-bit, X=16-bit] */
    ctx->a = 0x0078u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B443: ED 8E 02    | SBC $028E          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu)));
    /* $1D:B446: 8D D8 13    | STA $13D8          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13D8u), (uint16_t)(ctx->a));
    /* $1D:B449: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b44a  (SNES $1D:B44A | Ghidra 0x1db44a)
 * Discovery Provenance: JSR $B44A @ $1D:B2C5
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b44a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B44A: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B44B: A9 41 01    | LDA #$0141         [M=16-bit, X=16-bit] */
    ctx->a = 0x0141u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B44E: F5 0E       | SBC $0E,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu)));
    /* $1D:B450: 8F 09 E8 7E | STA $7EE809        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, 0x7EE809u, (uint16_t)(ctx->a));
    /* $1D:B454: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B455: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B457: 69 F0 FF    | ADC #$FFF0         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFF0u);
    /* $1D:B45A: 8D 4E 09    | STA $094E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x094Eu), (uint16_t)(ctx->a));
    /* $1D:B45D: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B45E: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $1D:B461: 8D 9E 16    | STA $169E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x169Eu), (uint16_t)(ctx->a));
    /* $1D:B464: 8D A0 16    | STA $16A0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A0u), (uint16_t)(ctx->a));
    /* $1D:B467: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B468: 69 E0 FF    | ADC #$FFE0         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE0u);
    /* $1D:B46B: 8D 9A 16    | STA $169A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x169Au), (uint16_t)(ctx->a));
    /* $1D:B46E: 8D 9C 16    | STA $169C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x169Cu), (uint16_t)(ctx->a));
    /* $1D:B471: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b472  (SNES $1D:B472 | Ghidra 0x1db472)
 * Discovery Provenance: JSR $B472 @ $1D:B17F
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b472(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    goto L_b472;
L_b43e: /* [M=16-bit, X=16-bit] */
    /* $1D:B43E: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b472: /* [M=16-bit, X=16-bit] */
    /* $1D:B472: AC 36 16    | LDY $1636          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B475: 30 C7       | BMI $B43E          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b43e;
    /* $1D:B477: C0 14 00    | CPY #$0014         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->y, 0x0014u);
    /* $1D:B47A: B0 C2       | BCS $B43E          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b43e;
    /* $1D:B47C: CE 38 16    | DEC $1638          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1638u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1638u), (uint16_t)(v16)); }
    /* $1D:B47F: 10 BD       | BPL $B43E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b43e;
    /* $1D:B481: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B483: F0 B9       | BEQ $B43E          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b43e;
    /* $1D:B485: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B486: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B489: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:B48B: A9 18 00    | LDA #$0018         [M=16-bit, X=16-bit] */
    ctx->a = 0x0018u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B48E: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:B490: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B491: B9 EC F8    | LDA $F8EC,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF8ECu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B494: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B497: 69 80 4B    | ADC #$4B80         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x4B80u);
    /* $1D:B49A: 85 FC       | STA $FC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(ctx->a));
    /* $1D:B49C: 49 1C 00    | EOR #$001C         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x001Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B49F: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $1D:B4A1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B4A2: B9 00 F9    | LDA $F900,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF900u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4A5: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4A8: 69 14 F9    | ADC #$F914         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xF914u);
    /* $1D:B4AB: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B4AC: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B4AD: 69 06 00    | ADC #$0006         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0006u);
    /* $1D:B4B0: 85 FE       | STA $FE            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFEu), (uint16_t)(ctx->a));
    /* $1D:B4B2: A5 FC       | LDA $FC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4B4: 22 A3 FE 00 | JSL $00FEA3        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_fea3(ctx); ctx->pb = saved_pb; }
    /* $1D:B4B8: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4BB: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:B4BD: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4C0: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:B4C2: A4 FE       | LDY $FE            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFEu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B4C4: A5 FA       | LDA $FA            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4C6: A2 18 00    | LDX #$0018         [M=16-bit, X=16-bit] */
    ctx->x = 0x0018u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B4C9: 22 AA FE 00 | JSL $00FEAA        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_feaa(ctx); ctx->pb = saved_pb; }
    /* $1D:B4CD: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B4CE: AC 36 16    | LDY $1636          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B4D1: B9 EC F8    | LDA $F8EC,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF8ECu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4D4: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4D7: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:B4D8: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:B4D9: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:B4DA: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:B4DB: 8D 38 16    | STA $1638          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1638u), (uint16_t)(ctx->a));
    /* $1D:B4DE: EE 36 16    | INC $1636          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1636u), (uint16_t)(v16)); }
    /* $1D:B4E1: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b4e2  (SNES $1D:B4E2 | Ghidra 0x1db4e2)
 * Discovery Provenance: JSR $B4E2 @ $1D:B23D
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b4e2(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B4E2: 20 B6 B5    | JSR $B5B6          [M=16-bit, X=16-bit] */
    bank1d_func_b5b6(ctx);
    /* $1D:B4E5: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B4E6: B5 32       | LDA $32,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x32u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4E8: 8D 44 16    | STA $1644          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1644u), (uint16_t)(ctx->a));
    /* $1D:B4EB: 69 64 00    | ADC #$0064         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0064u);
    /* $1D:B4EE: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4F1: 8D 42 16    | STA $1642          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(ctx->a));
    /* $1D:B4F4: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b4f5  (SNES $1D:B4F5 | Ghidra 0x1db4f5)
 * Discovery Provenance: JSR $B4F5 @ $1D:B240
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b4f5(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B4F5: 49 7F 00    | EOR #$007F         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x007Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4F8: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B4FB: 8D 3E 16    | STA $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->a));
    /* $1D:B4FE: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B4FF: E9 94 00    | SBC #$0094         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0094u);
    /* $1D:B502: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B505: 8D 40 16    | STA $1640          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1640u), (uint16_t)(ctx->a));
    /* $1D:B508: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b509  (SNES $1D:B509 | Ghidra 0x1db509)
 * Discovery Provenance: JSR $B509 @ $1D:B259
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b509(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B509: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B50A: A2 00 03    | LDX #$0300         [M=16-bit, X=16-bit] */
    ctx->x = 0x0300u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B50D: A0 00 00    | LDY #$0000         [M=16-bit, X=16-bit] */
    ctx->y = 0x0000u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B510: 20 1E B5    | JSR $B51E          [M=16-bit, X=16-bit] */
    bank1d_func_b51e(ctx);
    /* $1D:B513: A2 40 04    | LDX #$0440         [M=16-bit, X=16-bit] */
    ctx->x = 0x0440u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B516: A0 02 00    | LDY #$0002         [M=16-bit, X=16-bit] */
    ctx->y = 0x0002u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B519: 20 1E B5    | JSR $B51E          [M=16-bit, X=16-bit] */
    bank1d_func_b51e(ctx);
    /* $1D:B51C: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B51D: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b51e  (SNES $1D:B51E | Ghidra 0x1db51e)
 * Discovery Provenance: JSR $B51E @ $1D:B510
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b51e(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B51E: AD 36 09    | LDA $0936          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0936u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B521: C9 C0 03    | CMP #$03C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x03C0u);
    /* $1D:B524: B0 35       | BCS $B55B          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b55b;
    /* $1D:B526: C9 40 00    | CMP #$0040         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0040u);
    /* $1D:B529: 90 30       | BCC $B55B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b55b;
    /* $1D:B52B: 29 40 00    | AND #$0040         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B52E: 59 94 F8    | EOR $F894,Y        [M=16-bit, X=16-bit] */
    ctx->a ^= snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF894u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B531: F0 28       | BEQ $B55B          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b55b;
    /* $1D:B533: AD 36 09    | LDA $0936          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0936u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B536: 29 3F 00    | AND #$003F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B539: C9 3F 00    | CMP #$003F         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x003Fu);
    /* $1D:B53C: D0 18       | BNE $B556          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b556;
    /* $1D:B53E: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $1D:B53F: A9 20 00    | LDA #$0020         [M=16-bit, X=16-bit] */
    ctx->a = 0x0020u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B542: 22 5D A1 01 | JSL $01A15D        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a15d(ctx); ctx->pb = saved_pb; }
    /* $1D:B546: 22 C8 A0 01 | JSL $01A0C8        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a0c8(ctx); ctx->pb = saved_pb; }
    /* $1D:B54A: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B54B: 8D 38 09    | STA $0938          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0938u), (uint16_t)(ctx->a));
    /* $1D:B54E: A9 5C 00    | LDA #$005C         [M=16-bit, X=16-bit] */
    ctx->a = 0x005Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B551: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:B555: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b556: /* [M=16-bit, X=16-bit] */
    /* $1D:B556: C9 20 00    | CMP #$0020         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0020u);
    /* $1D:B559: B0 1A       | BCS $B575          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b575;
L_b55b: /* [M=16-bit, X=16-bit] */
    /* $1D:B55B: B9 A2 16    | LDA $16A2,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x16A2u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B55E: C9 01 00    | CMP #$0001         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0001u);
    /* $1D:B561: 90 11       | BCC $B574          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b574;
    /* $1D:B563: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B564: 99 A2 16    | STA $16A2,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x16A2u), (uint16_t)(ctx->a));
    /* $1D:B567: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B568: B9 3E 16    | LDA $163E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B56B: 79 98 F8    | ADC $F898,Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF898u)));
    /* $1D:B56E: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B571: 99 3E 16    | STA $163E,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x163Eu), (uint16_t)(ctx->a));
L_b574: /* [M=16-bit, X=16-bit] */
    /* $1D:B574: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b575: /* [M=16-bit, X=16-bit] */
    /* $1D:B575: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B576: AD 38 09    | LDA $0938          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0938u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B579: F9 3E 16    | SBC $163E,Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0x163Eu)));
    /* $1D:B57C: 10 04       | BPL $B582          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b582;
    /* $1D:B57E: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B581: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
L_b582: /* [M=16-bit, X=16-bit] */
    /* $1D:B582: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:B585: 90 0D       | BCC $B594          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b594;
    /* $1D:B587: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B588: B9 3E 16    | LDA $163E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B58B: 79 9C F8    | ADC $F89C,Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF89Cu)));
    /* $1D:B58E: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B591: 99 3E 16    | STA $163E,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x163Eu), (uint16_t)(ctx->a));
L_b594: /* [M=16-bit, X=16-bit] */
    /* $1D:B594: B9 A2 16    | LDA $16A2,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x16A2u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B597: C9 20 00    | CMP #$0020         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0020u);
    /* $1D:B59A: B0 05       | BCS $B5A1          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b5a1;
    /* $1D:B59C: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B59D: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B59E: 99 A2 16    | STA $16A2,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x16A2u), (uint16_t)(ctx->a));
L_b5a1: /* [M=16-bit, X=16-bit] */
    /* $1D:B5A1: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b5a2  (SNES $1D:B5A2 | Ghidra 0x1db5a2)
 * Discovery Provenance: JSR $B5A2 @ $1D:B22E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b5a2(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B5A2: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B5A4: D0 0F       | BNE $B5B5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b5b5;
    /* $1D:B5A6: AD A2 16    | LDA $16A2          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x16A2u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B5A9: C9 0A 00    | CMP #$000A         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x000Au);
    /* $1D:B5AC: B0 07       | BCS $B5B5          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b5b5;
    /* $1D:B5AE: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B5AF: 8D A2 16    | STA $16A2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A2u), (uint16_t)(ctx->a));
    /* $1D:B5B2: 8D A4 16    | STA $16A4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16A4u), (uint16_t)(ctx->a));
L_b5b5: /* [M=16-bit, X=16-bit] */
    /* $1D:B5B5: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b5b6  (SNES $1D:B5B6 | Ghidra 0x1db5b6)
 * Discovery Provenance: JSR $B5B6 @ $1D:B4E2
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b5b6(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B5B6: B5 32       | LDA $32,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x32u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B5B8: B4 30       | LDY $30,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x30u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B5BA: D0 11       | BNE $B5CD          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b5cd;
    /* $1D:B5BC: B4 34       | LDY $34,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B5BE: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B5BF: F9 BB F8    | SBC $F8BB,Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF8BBu)));
    /* $1D:B5C2: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B5C5: 95 32       | STA $32,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x32u), (uint16_t)(ctx->a));
    /* $1D:B5C7: C9 18 00    | CMP #$0018         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0018u);
    /* $1D:B5CA: 90 12       | BCC $B5DE          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b5de;
    /* $1D:B5CC: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b5cd: /* [M=16-bit, X=16-bit] */
    /* $1D:B5CD: B4 34       | LDY $34,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B5CF: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B5D0: 79 BB F8    | ADC $F8BB,Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF8BBu)));
    /* $1D:B5D3: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B5D6: 95 32       | STA $32,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x32u), (uint16_t)(ctx->a));
    /* $1D:B5D8: C9 7C 00    | CMP #$007C         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x007Cu);
    /* $1D:B5DB: B0 08       | BCS $B5E5          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b5e5;
    /* $1D:B5DD: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b5de: /* [M=16-bit, X=16-bit] */
    /* $1D:B5DE: A9 5E 00    | LDA #$005E         [M=16-bit, X=16-bit] */
    ctx->a = 0x005Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B5E1: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
L_b5e5: /* [M=16-bit, X=16-bit] */
    /* $1D:B5E5: B5 30       | LDA $30,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x30u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B5E7: 49 02 00    | EOR #$0002         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B5EA: 95 30       | STA $30,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x30u), (uint16_t)(ctx->a));
    /* $1D:B5EC: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b5ed  (SNES $1D:B5ED | Ghidra 0x1db5ed)
 * Discovery Provenance: JSR $B5ED @ $1D:B243
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b5ed(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B5ED: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B5EE: A2 00 03    | LDX #$0300         [M=16-bit, X=16-bit] */
    ctx->x = 0x0300u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B5F1: A0 00 00    | LDY #$0000         [M=16-bit, X=16-bit] */
    ctx->y = 0x0000u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B5F4: 20 14 B6    | JSR $B614          [M=16-bit, X=16-bit] */
    bank1d_func_b614(ctx);
    /* $1D:B5F7: A2 40 04    | LDX #$0440         [M=16-bit, X=16-bit] */
    ctx->x = 0x0440u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B5FA: A0 02 00    | LDY #$0002         [M=16-bit, X=16-bit] */
    ctx->y = 0x0002u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B5FD: 20 14 B6    | JSR $B614          [M=16-bit, X=16-bit] */
    bank1d_func_b614(ctx);
    /* $1D:B600: A2 80 05    | LDX #$0580         [M=16-bit, X=16-bit] */
    ctx->x = 0x0580u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B603: A0 04 00    | LDY #$0004         [M=16-bit, X=16-bit] */
    ctx->y = 0x0004u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B606: 20 14 B6    | JSR $B614          [M=16-bit, X=16-bit] */
    bank1d_func_b614(ctx);
    /* $1D:B609: A2 40 06    | LDX #$0640         [M=16-bit, X=16-bit] */
    ctx->x = 0x0640u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B60C: A0 06 00    | LDY #$0006         [M=16-bit, X=16-bit] */
    ctx->y = 0x0006u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B60F: 20 14 B6    | JSR $B614          [M=16-bit, X=16-bit] */
    bank1d_func_b614(ctx);
    /* $1D:B612: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B613: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b614  (SNES $1D:B614 | Ghidra 0x1db614)
 * Discovery Provenance: JSR $B614 @ $1D:B5F4
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b614(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B614: B9 A2 16    | LDA $16A2,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x16A2u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B617: 95 30       | STA $30,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x30u), (uint16_t)(ctx->a));
    /* $1D:B619: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B61A: B5 32       | LDA $32,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x32u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B61C: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $1D:B61F: C9 10 00    | CMP #$0010         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0010u);
    /* $1D:B622: 90 04       | BCC $B628          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b628;
    /* $1D:B624: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B625: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
L_b628: /* [M=16-bit, X=16-bit] */
    /* $1D:B628: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B629: 75 34       | ADC $34,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u)));
    /* $1D:B62B: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B62C: B9 B3 F8    | LDA $F8B3,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF8B3u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B62F: 9F 00 E0 7E | STA $7EE000,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE000u), (uint16_t)(ctx->a));
    /* $1D:B633: B9 9A 16    | LDA $169A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x169Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B636: 9F 00 E1 7E | STA $7EE100,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE100u), (uint16_t)(ctx->a));
    /* $1D:B63A: B9 3E 16    | LDA $163E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B63D: 9F 00 E2 7E | STA $7EE200,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE200u), (uint16_t)(ctx->a));
    /* $1D:B641: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b642  (SNES $1D:B642 | Ghidra 0x1db642)
 * Discovery Provenance: JSR $B642 @ $1D:B193
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b642(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B642: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B645: 8D BA 13    | STA $13BA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13BAu), (uint16_t)(ctx->a));
    /* $1D:B648: 8D B4 13    | STA $13B4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13B4u), (uint16_t)(ctx->a));
    /* $1D:B64B: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B64E: 8D B6 13    | STA $13B6          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13B6u), (uint16_t)(ctx->a));
    /* $1D:B651: A9 00 20    | LDA #$2000         [M=16-bit, X=16-bit] */
    ctx->a = 0x2000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B654: 8D 9C 02    | STA $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(ctx->a));
    /* $1D:B657: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:B65A: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:B65C: A9 FF FF    | LDA #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B65F: 8D 9E 02    | STA $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(ctx->a));
    /* $1D:B662: 8D 3C 16    | STA $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(ctx->a));
    /* $1D:B665: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:B667: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b668  (SNES $1D:B668 | Ghidra 0x1db668)
 * Discovery Provenance: JSR $B668 @ $1D:B37A
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b668(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B668: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B66B: C9 03 00    | CMP #$0003         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0003u);
    /* $1D:B66E: B0 32       | BCS $B6A2          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b6a2;
    /* $1D:B670: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B671: AD 3A 16    | LDA $163A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B674: 69 00 18    | ADC #$1800         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x1800u);
    /* $1D:B677: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:B67A: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B67D: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:B680: 8D 3C 16    | STA $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(ctx->a));
    /* $1D:B683: 80 1D       | BRA $B6A2          [M=16-bit, X=16-bit] */
    goto L_b6a2;
L_b6a2: /* [M=16-bit, X=16-bit] */
    /* $1D:B6A2: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B6A3: AD 9C 13    | LDA $139C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x139Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6A6: 6D 3A 16    | ADC $163A          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au)));
    /* $1D:B6A9: 8D 9C 13    | STA $139C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x139Cu), (uint16_t)(ctx->a));
    /* $1D:B6AC: AD 98 13    | LDA $1398          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1398u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6AF: 6D 3C 16    | ADC $163C          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu)));
    /* $1D:B6B2: 8D 98 13    | STA $1398          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1398u), (uint16_t)(ctx->a));
    /* $1D:B6B5: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b685  (SNES $1D:B685 | Ghidra 0x1db685)
 * Discovery Provenance: JSR $B685 @ $1D:B1C1
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b685(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B685: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B688: 10 05       | BPL $B68F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b68f;
    /* $1D:B68A: C9 FA FF    | CMP #$FFFA         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFFAu);
    /* $1D:B68D: 90 13       | BCC $B6A2          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b6a2;
L_b68f: /* [M=16-bit, X=16-bit] */
    /* $1D:B68F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B690: AD 3A 16    | LDA $163A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B693: E9 00 10    | SBC #$1000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x1000u);
    /* $1D:B696: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:B699: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B69C: E9 00 00    | SBC #$0000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0000u);
    /* $1D:B69F: 8D 3C 16    | STA $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(ctx->a));
L_b6a2: /* [M=16-bit, X=16-bit] */
    /* $1D:B6A2: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B6A3: AD 9C 13    | LDA $139C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x139Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6A6: 6D 3A 16    | ADC $163A          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au)));
    /* $1D:B6A9: 8D 9C 13    | STA $139C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x139Cu), (uint16_t)(ctx->a));
    /* $1D:B6AC: AD 98 13    | LDA $1398          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1398u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6AF: 6D 3C 16    | ADC $163C          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu)));
    /* $1D:B6B2: 8D 98 13    | STA $1398          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1398u), (uint16_t)(ctx->a));
    /* $1D:B6B5: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b6b6  (SNES $1D:B6B6 | Ghidra 0x1db6b6)
 * Discovery Provenance: JSR $B6B6 @ $1D:B309
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b6b6(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    goto L_b6b6;
L_b6a2: /* [M=16-bit, X=16-bit] */
    /* $1D:B6A2: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B6A3: AD 9C 13    | LDA $139C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x139Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6A6: 6D 3A 16    | ADC $163A          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au)));
    /* $1D:B6A9: 8D 9C 13    | STA $139C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x139Cu), (uint16_t)(ctx->a));
    /* $1D:B6AC: AD 98 13    | LDA $1398          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1398u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6AF: 6D 3C 16    | ADC $163C          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu)));
    /* $1D:B6B2: 8D 98 13    | STA $1398          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1398u), (uint16_t)(ctx->a));
    /* $1D:B6B5: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b6b6: /* [M=16-bit, X=16-bit] */
    /* $1D:B6B6: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6B9: C9 FF FF    | CMP #$FFFF         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFFFu);
    /* $1D:B6BC: B0 E4       | BCS $B6A2          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b6a2;
    /* $1D:B6BE: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B6BF: AD 3A 16    | LDA $163A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6C2: 69 00 10    | ADC #$1000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x1000u);
    /* $1D:B6C5: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:B6C8: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6CB: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:B6CE: 8D 3C 16    | STA $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(ctx->a));
    /* $1D:B6D1: 80 CF       | BRA $B6A2          [M=16-bit, X=16-bit] */
    goto L_b6a2;
}

/* ==========================================================================
 * Function: bank1d_func_b6d3  (SNES $1D:B6D3 | Ghidra 0x1db6d3)
 * Discovery Provenance: JSR $B6D3 @ $1D:B377
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b6d3(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B6D3: AD 38 16    | LDA $1638          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1638u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6D6: 10 05       | BPL $B6DD          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b6dd;
    /* $1D:B6D8: C9 FF FF    | CMP #$FFFF         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFFFu);
    /* $1D:B6DB: 90 31       | BCC $B70E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b70e;
L_b6dd: /* [M=16-bit, X=16-bit] */
    /* $1D:B6DD: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B6DE: AD 36 16    | LDA $1636          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6E1: E9 00 C8    | SBC #$C800         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0xC800u);
    /* $1D:B6E4: 8D 36 16    | STA $1636          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1636u), (uint16_t)(ctx->a));
    /* $1D:B6E7: AD 38 16    | LDA $1638          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1638u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6EA: E9 00 00    | SBC #$0000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0000u);
    /* $1D:B6ED: 8D 38 16    | STA $1638          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1638u), (uint16_t)(ctx->a));
    /* $1D:B6F0: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b70e: /* [M=16-bit, X=16-bit] */
    /* $1D:B70E: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b6f1  (SNES $1D:B6F1 | Ghidra 0x1db6f1)
 * Discovery Provenance: JSR $B6F1 @ $1D:BA8C
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b6f1(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B6F1: AD 38 16    | LDA $1638          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1638u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6F4: C9 06 00    | CMP #$0006         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0006u);
    /* $1D:B6F7: 30 02       | BMI $B6FB          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b6fb;
    /* $1D:B6F9: B0 13       | BCS $B70E          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b70e;
L_b6fb: /* [M=16-bit, X=16-bit] */
    /* $1D:B6FB: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B6FC: AD 36 16    | LDA $1636          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B6FF: 69 00 10    | ADC #$1000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x1000u);
    /* $1D:B702: 8D 36 16    | STA $1636          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1636u), (uint16_t)(ctx->a));
    /* $1D:B705: AD 38 16    | LDA $1638          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1638u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B708: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:B70B: 8D 38 16    | STA $1638          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1638u), (uint16_t)(ctx->a));
L_b70e: /* [M=16-bit, X=16-bit] */
    /* $1D:B70E: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b70f  (SNES $1D:B70F | Ghidra 0x1db70f)
 * Discovery Provenance: JSR $B70F @ $1D:B24D
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b70f(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B70F: B5 34       | LDA $34,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B711: F0 0F       | BEQ $B722          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b722;
    /* $1D:B713: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:B715: A9 FE FF    | LDA #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B718: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:B71A: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B71C: C9 C4 00    | CMP #$00C4         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00C4u);
    /* $1D:B71F: 90 18       | BCC $B739          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b739;
L_b721: /* [M=16-bit, X=16-bit] */
    /* $1D:B721: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b722: /* [M=16-bit, X=16-bit] */
    /* $1D:B722: A9 00 C0    | LDA #$C000         [M=16-bit, X=16-bit] */
    ctx->a = 0xC000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B725: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:B727: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:B729: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B72B: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $1D:B72E: 90 F1       | BCC $B721          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b721;
    /* $1D:B730: A9 5D 00    | LDA #$005D         [M=16-bit, X=16-bit] */
    ctx->a = 0x005Du;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B733: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:B737: 80 00       | BRA $B739          [M=16-bit, X=16-bit] */
    goto L_b739;
L_b739: /* [M=16-bit, X=16-bit] */
    /* $1D:B739: B5 34       | LDA $34,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B73B: 49 02 00    | EOR #$0002         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B73E: 95 34       | STA $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(ctx->a));
    /* $1D:B740: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b741  (SNES $1D:B741 | Ghidra 0x1db741)
 * Discovery Provenance: JSR $B741 @ $1D:B215
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b741(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B741: B9 8A 1D    | LDA $1D8A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x1D8Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B744: F0 11       | BEQ $B757          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b757;
    /* $1D:B746: B9 16 00    | LDA $0016,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0016u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B749: 30 0C       | BMI $B757          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b757;
    /* $1D:B74B: A9 60 00    | LDA #$0060         [M=16-bit, X=16-bit] */
    ctx->a = 0x0060u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B74E: 99 24 00    | STA $0024,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0024u), (uint16_t)(ctx->a));
    /* $1D:B751: A9 03 00    | LDA #$0003         [M=16-bit, X=16-bit] */
    ctx->a = 0x0003u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B754: 99 16 00    | STA $0016,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0016u), (uint16_t)(ctx->a));
L_b757: /* [M=16-bit, X=16-bit] */
    /* $1D:B757: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b758  (SNES $1D:B758 | Ghidra 0x1db758)
 * Discovery Provenance: JSR $B758 @ $1D:B24A
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b758(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B758: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B75B: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:B75D: A9 60 00    | LDA #$0060         [M=16-bit, X=16-bit] */
    ctx->a = 0x0060u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B760: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* Fallthrough into function $1D:B762 */
    bank1d_func_b762(ctx);
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b762  (SNES $1D:B762 | Ghidra 0x1db762)
 * Discovery Provenance: JSR $B762 @ $1D:B316
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b762(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B762: B4 3A       | LDY $3A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Au));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B764: D0 2A       | BNE $B790          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b790;
    /* $1D:B766: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B769: 30 25       | BMI $B790          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b790;
    /* $1D:B76B: C5 00       | CMP $00            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u)));
    /* $1D:B76D: B0 02       | BCS $B771          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b771;
    /* $1D:B76F: F6 3A       | INC $3A,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Au))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(v16)); }
L_b771: /* [M=16-bit, X=16-bit] */
    /* $1D:B771: AD 9D 02    | LDA $029D          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x029Du));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B774: 10 05       | BPL $B77B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b77b;
    /* $1D:B776: C9 80 FF    | CMP #$FF80         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFF80u);
    /* $1D:B779: 90 3D       | BCC $B7B8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b7b8;
L_b77b: /* [M=16-bit, X=16-bit] */
    /* $1D:B77B: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B77C: AD 9C 02    | LDA $029C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x029Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B77F: E9 00 04    | SBC #$0400         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0400u);
    /* $1D:B782: 8D 9C 02    | STA $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(ctx->a));
    /* $1D:B785: AD 9E 02    | LDA $029E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x029Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B788: E9 00 00    | SBC #$0000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0000u);
    /* $1D:B78B: 8D 9E 02    | STA $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(ctx->a));
    /* $1D:B78E: 80 28       | BRA $B7B8          [M=16-bit, X=16-bit] */
    goto L_b7b8;
L_b790: /* [M=16-bit, X=16-bit] */
    /* $1D:B790: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B793: 30 06       | BMI $B79B          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b79b;
    /* $1D:B795: C5 02       | CMP $02            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u)));
    /* $1D:B797: 90 02       | BCC $B79B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b79b;
    /* $1D:B799: 74 3A       | STZ $3A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(0u));
L_b79b: /* [M=16-bit, X=16-bit] */
    /* $1D:B79B: AD 9D 02    | LDA $029D          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x029Du));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B79E: 30 05       | BMI $B7A5          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_b7a5;
    /* $1D:B7A0: C9 80 00    | CMP #$0080         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0080u);
    /* $1D:B7A3: B0 13       | BCS $B7B8          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b7b8;
L_b7a5: /* [M=16-bit, X=16-bit] */
    /* $1D:B7A5: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B7A6: AD 9C 02    | LDA $029C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x029Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7A9: 69 00 04    | ADC #$0400         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0400u);
    /* $1D:B7AC: 8D 9C 02    | STA $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(ctx->a));
    /* $1D:B7AF: AD 9E 02    | LDA $029E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x029Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7B2: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:B7B5: 8D 9E 02    | STA $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(ctx->a));
L_b7b8: /* [M=16-bit, X=16-bit] */
    /* $1D:B7B8: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b7b9  (SNES $1D:B7B9 | Ghidra 0x1db7b9)
 * Discovery Provenance: JSR $B7B9 @ $1D:B415
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b7b9(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B7B9: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7BC: 10 0C       | BPL $B7CA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b7ca;
    /* $1D:B7BE: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $1D:B7C1: B0 07       | BCS $B7CA          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_b7ca;
    /* $1D:B7C3: 9C 9C 02    | STZ $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(0u));
    /* $1D:B7C6: 9C 9E 02    | STZ $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(0u));
    /* $1D:B7C9: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b7ca: /* [M=16-bit, X=16-bit] */
    /* $1D:B7CA: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B7CB: AD 9C 02    | LDA $029C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x029Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7CE: E9 00 04    | SBC #$0400         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0400u);
    /* $1D:B7D1: 8D 9C 02    | STA $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(ctx->a));
    /* $1D:B7D4: AD 9E 02    | LDA $029E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x029Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7D7: E9 00 00    | SBC #$0000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0000u);
    /* $1D:B7DA: 8D 9E 02    | STA $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(ctx->a));
    /* $1D:B7DD: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b7de  (SNES $1D:B7DE | Ghidra 0x1db7de)
 * Discovery Provenance: JSR $B7DE @ $1D:B306
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b7de(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B7DE: A9 03 00    | LDA #$0003         [M=16-bit, X=16-bit] */
    ctx->a = 0x0003u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7E1: 8D 76 16    | STA $1676          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1676u), (uint16_t)(ctx->a));
    /* $1D:B7E4: AD 98 13    | LDA $1398          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1398u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7E7: 8D 74 16    | STA $1674          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1674u), (uint16_t)(ctx->a));
    /* $1D:B7EA: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B7EB: A2 06 00    | LDX #$0006         [M=16-bit, X=16-bit] */
    ctx->x = 0x0006u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B7EE: AD 36 16    | LDA $1636          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7F1: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:B7F3: AD 38 16    | LDA $1638          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1638u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7F6: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
L_b7f8: /* [M=16-bit, X=16-bit] */
    /* $1D:B7F8: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B7F9: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B7FB: 7F 02 E4 7E | ADC $7EE402,X      [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_longx(ctx, 0x7EE402u)));
    /* $1D:B7FF: 9F 02 E4 7E | STA $7EE402,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE402u), (uint16_t)(ctx->a));
    /* $1D:B803: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B805: 7F 02 E0 7E | ADC $7EE002,X      [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_longx(ctx, 0x7EE002u)));
    /* $1D:B809: 9F 02 E0 7E | STA $7EE002,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE002u), (uint16_t)(ctx->a));
    /* $1D:B80D: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B80F: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:B810: 66 02       | ROR $02            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(v16)); }
    /* $1D:B812: 66 00       | ROR $00            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(v16)); }
    /* $1D:B814: CA          | DEX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x - 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B815: CA          | DEX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x - 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B816: D0 E0       | BNE $B7F8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b7f8;
    /* $1D:B818: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B819: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b81a  (SNES $1D:B81A | Ghidra 0x1db81a)
 * Discovery Provenance: JSR $B81A @ $1D:B125
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b81a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B81A: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B81B: A0 0C 00    | LDY #$000C         [M=16-bit, X=16-bit] */
    ctx->y = 0x000Cu;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B81E: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B821: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:B823: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B826: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:B828: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B82B: A2 00 03    | LDX #$0300         [M=16-bit, X=16-bit] */
    ctx->x = 0x0300u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B82E: 20 5C B8    | JSR $B85C          [M=16-bit, X=16-bit] */
    bank1d_func_b85c(ctx);
    /* $1D:B831: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B834: A0 0C 00    | LDY #$000C         [M=16-bit, X=16-bit] */
    ctx->y = 0x000Cu;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B837: A2 40 04    | LDX #$0440         [M=16-bit, X=16-bit] */
    ctx->x = 0x0440u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B83A: 20 5C B8    | JSR $B85C          [M=16-bit, X=16-bit] */
    bank1d_func_b85c(ctx);
    /* $1D:B83D: A9 0E 00    | LDA #$000E         [M=16-bit, X=16-bit] */
    ctx->a = 0x000Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B840: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:B842: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B845: A0 00 00    | LDY #$0000         [M=16-bit, X=16-bit] */
    ctx->y = 0x0000u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B848: A2 80 05    | LDX #$0580         [M=16-bit, X=16-bit] */
    ctx->x = 0x0580u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B84B: 20 5C B8    | JSR $B85C          [M=16-bit, X=16-bit] */
    bank1d_func_b85c(ctx);
    /* $1D:B84E: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B851: A0 00 00    | LDY #$0000         [M=16-bit, X=16-bit] */
    ctx->y = 0x0000u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B854: A2 40 06    | LDX #$0640         [M=16-bit, X=16-bit] */
    ctx->x = 0x0640u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B857: 20 5C B8    | JSR $B85C          [M=16-bit, X=16-bit] */
    bank1d_func_b85c(ctx);
    /* $1D:B85A: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B85B: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b85c  (SNES $1D:B85C | Ghidra 0x1db85c)
 * Discovery Provenance: JSR $B85C @ $1D:B82E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b85c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B85C: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:B85E: 84 04       | STY $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->y));
    /* $1D:B860: A0 00 00    | LDY #$0000         [M=16-bit, X=16-bit] */
    ctx->y = 0x0000u;
    snes_set_nz16(ctx, ctx->y);
L_b863: /* [M=16-bit, X=16-bit] */
    /* $1D:B863: A9 96 00    | LDA #$0096         [M=16-bit, X=16-bit] */
    ctx->a = 0x0096u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B866: 95 10       | STA $10,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x10u), (uint16_t)(ctx->a));
    /* $1D:B868: A5 04       | LDA $04            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B86A: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:B86C: A9 03 C5    | LDA #$C503         [M=16-bit, X=16-bit] */
    ctx->a = 0xC503u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B86F: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:B871: A9 00 06    | LDA #$0600         [M=16-bit, X=16-bit] */
    ctx->a = 0x0600u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B874: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:B876: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B879: 95 28       | STA $28,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x28u), (uint16_t)(ctx->a));
    /* $1D:B87B: A9 F8 FF    | LDA #$FFF8         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFF8u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B87E: 95 2A       | STA $2A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Au), (uint16_t)(ctx->a));
    /* $1D:B880: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B881: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $1D:B882: A9 80 01    | LDA #$0180         [M=16-bit, X=16-bit] */
    ctx->a = 0x0180u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B885: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:B887: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B889: A0 08 00    | LDY #$0008         [M=16-bit, X=16-bit] */
    ctx->y = 0x0008u;
    snes_set_nz16(ctx, ctx->y);
L_b88c: /* [M=16-bit, X=16-bit] */
    /* $1D:B88C: 9F 00 E0 7E | STA $7EE000,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE000u), (uint16_t)(ctx->a));
    /* $1D:B890: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B891: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B892: 88          | DEY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y - 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B893: 10 F7       | BPL $B88C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b88c;
    /* $1D:B895: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B896: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B897: A5 08       | LDA $08            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B899: 95 34       | STA $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(ctx->a));
    /* $1D:B89B: A5 0A       | LDA $0A            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B89D: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:B89F: 74 30       | STZ $30,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x30u), (uint16_t)(0u));
    /* $1D:B8A1: 74 32       | STZ $32,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x32u), (uint16_t)(0u));
    /* $1D:B8A3: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B8A4: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8A5: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $1D:B8A8: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
    /* $1D:B8AA: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B8AB: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B8AC: A5 08       | LDA $08            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8AE: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $1D:B8B1: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:B8B3: C6 02       | DEC $02            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(v16)); }
    /* $1D:B8B5: 10 AC       | BPL $B863          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_b863;
    /* $1D:B8B7: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B8B8: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8B9: E9 40 00    | SBC #$0040         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0040u);
    /* $1D:B8BC: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B8BD: 74 36       | STZ $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(0u));
    /* $1D:B8BF: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b8c0  (SNES $1D:B8C0 | Ghidra 0x1db8c0)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b8c0(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B8C0: AD 88 16    | LDA $1688          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1688u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8C3: F0 06       | BEQ $B8CB          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b8cb;
    /* $1D:B8C5: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8C8: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:B8CA: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_b8cb: /* [M=16-bit, X=16-bit] */
    /* $1D:B8CB: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B8CC: B5 32       | LDA $32,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x32u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8CE: 75 34       | ADC $34,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u)));
    /* $1D:B8D0: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B8D1: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B8D2: BF 00 E0 7E | LDA $7EE000,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x7EE000u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8D6: 85 FC       | STA $FC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(ctx->a));
    /* $1D:B8D8: BF 00 E1 7E | LDA $7EE100,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x7EE100u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8DC: 85 FE       | STA $FE            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFEu), (uint16_t)(ctx->a));
    /* $1D:B8DE: BF 00 E2 7E | LDA $7EE200,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x7EE200u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8E2: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $1D:B8E4: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B8E5: B4 30       | LDY $30,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x30u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B8E7: 22 A4 AC 1D | JSL $1DACA4        [M=16-bit, X=16-bit] */
    bank1d_func_aca4(ctx);
    /* $1D:B8EB: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B8EC: A5 FC       | LDA $FC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8EE: 65 08       | ADC $08            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:B8F0: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:B8F2: 85 FC       | STA $FC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(ctx->a));
    /* $1D:B8F4: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B8F5: A5 FE       | LDA $FE            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFEu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B8F7: 65 0A       | ADC $0A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $1D:B8F9: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:B8FB: 85 FE       | STA $FE            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFEu), (uint16_t)(ctx->a));
    /* $1D:B8FD: B4 36       | LDY $36,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B8FF: F0 3F       | BEQ $B940          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b940;
    /* $1D:B901: B5 30       | LDA $30,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x30u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B903: 99 30 00    | STA $0030,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0030u), (uint16_t)(ctx->a));
    /* $1D:B906: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B907: B5 32       | LDA $32,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x32u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B909: 75 38       | ADC $38,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u)));
    /* $1D:B90B: C9 10 00    | CMP #$0010         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0010u);
    /* $1D:B90E: 90 04       | BCC $B914          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b914;
    /* $1D:B910: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B911: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
L_b914: /* [M=16-bit, X=16-bit] */
    /* $1D:B914: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B915: 75 34       | ADC $34,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u)));
    /* $1D:B917: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:B918: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $1D:B91B: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B91C: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B91D: A5 FC       | LDA $FC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B91F: 9F 00 E0 7E | STA $7EE000,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE000u), (uint16_t)(ctx->a));
    /* $1D:B923: A5 FE       | LDA $FE            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFEu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B925: 9F 00 E1 7E | STA $7EE100,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE100u), (uint16_t)(ctx->a));
    /* $1D:B929: A5 FA       | LDA $FA            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B92B: 9F 00 E2 7E | STA $7EE200,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE200u), (uint16_t)(ctx->a));
    /* $1D:B92F: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_b930: /* [M=16-bit, X=16-bit] */
    /* $1D:B930: B5 32       | LDA $32,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x32u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B932: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B933: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B934: C9 10 00    | CMP #$0010         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0010u);
    /* $1D:B937: 90 04       | BCC $B93D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b93d;
    /* $1D:B939: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:B93A: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
L_b93d: /* [M=16-bit, X=16-bit] */
    /* $1D:B93D: 95 32       | STA $32,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x32u), (uint16_t)(ctx->a));
    /* $1D:B93F: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_b940: /* [M=16-bit, X=16-bit] */
    /* $1D:B940: A5 FA       | LDA $FA            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B942: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:B943: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:B944: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:B945: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:B946: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:B947: 29 06 00    | AND #$0006         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0006u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B94A: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B94B: B9 AB F8    | LDA $F8AB,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF8ABu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B94E: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:B950: 80 DE       | BRA $B930          [M=16-bit, X=16-bit] */
    goto L_b930;
}

/* ==========================================================================
 * Function: bank1d_func_b952  (SNES $1D:B952 | Ghidra 0x1db952)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b952(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:B952: 9C 44 16    | STZ $1644          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1644u), (uint16_t)(0u));
    /* $1D:B955: A9 FF FF    | LDA #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B958: 8D 3E 16    | STA $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->a));
    /* $1D:B95B: 9C 42 16    | STZ $1642          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(0u));
    /* $1D:B95E: A9 10 00    | LDA #$0010         [M=16-bit, X=16-bit] */
    ctx->a = 0x0010u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B961: 8D 40 16    | STA $1640          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1640u), (uint16_t)(ctx->a));
    /* $1D:B964: 22 30 FF 00 | JSL $00FF30        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_ff30(ctx); ctx->pb = saved_pb; }
    /* $1D:B968: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B96B: 8D 94 1E    | STA $1E94          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E94u), (uint16_t)(ctx->a));
    /* $1D:B96E: A9 13 00    | LDA #$0013         [M=16-bit, X=16-bit] */
    ctx->a = 0x0013u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B971: 8D 96 1E    | STA $1E96          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E96u), (uint16_t)(ctx->a));
    /* $1D:B974: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B977: 8D 84 1E    | STA $1E84          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E84u), (uint16_t)(ctx->a));
    /* $1D:B97A: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B97B: A0 E3 F8    | LDY #$F8E3         [M=16-bit, X=16-bit] */
    ctx->y = 0xF8E3u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B97E: 22 82 99 00 | JSL $009982        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9982(ctx); ctx->pb = saved_pb; }
    /* $1D:B982: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B983: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B984: A2 D1 F8    | LDX #$F8D1         [M=16-bit, X=16-bit] */
    ctx->x = 0xF8D1u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B987: 22 A8 90 00 | JSL $0090A8        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_90a8(ctx); ctx->pb = saved_pb; }
    /* $1D:B98B: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B98C: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_b98d  (SNES $1D:B98D | Ghidra 0x1db98d)
 * Discovery Provenance: JSR $B98D @ $1D:B37D
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_b98d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    goto L_b98d;
L_b98c: /* [M=16-bit, X=16-bit] */
    /* $1D:B98C: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b98d: /* [M=16-bit, X=16-bit] */
    /* $1D:B98D: AC 9A 16    | LDY $169A          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x169Au));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B990: AD 40 16    | LDA $1640          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1640u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B993: D0 0A       | BNE $B99F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b99f;
    /* $1D:B995: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B998: 89 10 00    | BIT #$0010         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0010u)) == 0u ? 1u : 0u);
    /* $1D:B99B: D0 72       | BNE $BA0F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ba0f;
    /* $1D:B99D: F0 55       | BEQ $B9F4          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b9f4;
L_b99f: /* [M=16-bit, X=16-bit] */
    /* $1D:B99F: CE 40 16    | DEC $1640          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1640u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1640u), (uint16_t)(v16)); }
    /* $1D:B9A2: D0 E8       | BNE $B98C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b98c;
    /* $1D:B9A4: AC 3E 16    | LDY $163E          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B9A7: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B9A8: 8C 3E 16    | STY $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->y));
    /* $1D:B9AB: B9 D0 F9    | LDA $F9D0,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF9D0u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B9AE: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B9B1: C9 FE 00    | CMP #$00FE         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00FEu);
    /* $1D:B9B4: 90 0F       | BCC $B9C5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_b9c5;
    /* $1D:B9B6: A0 40 00    | LDY #$0040         [M=16-bit, X=16-bit] */
    ctx->y = 0x0040u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B9B9: C9 FE 00    | CMP #$00FE         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00FEu);
    /* $1D:B9BC: F0 03       | BEQ $B9C1          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_b9c1;
    /* $1D:B9BE: A0 60 01    | LDY #$0160         [M=16-bit, X=16-bit] */
    ctx->y = 0x0160u;
    snes_set_nz16(ctx, ctx->y);
L_b9c1: /* [M=16-bit, X=16-bit] */
    /* $1D:B9C1: 8C 40 16    | STY $1640          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1640u), (uint16_t)(ctx->y));
    /* $1D:B9C4: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b9c5: /* [M=16-bit, X=16-bit] */
    /* $1D:B9C5: A0 20 00    | LDY #$0020         [M=16-bit, X=16-bit] */
    ctx->y = 0x0020u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B9C8: 8D 44 16    | STA $1644          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1644u), (uint16_t)(ctx->a));
    /* $1D:B9CB: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B9CE: 8D 9C 16    | STA $169C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x169Cu), (uint16_t)(ctx->a));
    /* $1D:B9D1: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:B9D2: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B9D5: 89 10 00    | BIT #$0010         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0010u)) == 0u ? 1u : 0u);
    /* $1D:B9D8: D0 15       | BNE $B9EF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_b9ef;
    /* $1D:B9DA: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B9DB: AC 42 16    | LDY $1642          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B9DE: B9 1A FA    | LDA $FA1A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFA1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B9E1: 9F 00 E9 7E | STA $7EE900,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE900u), (uint16_t)(ctx->a));
    /* $1D:B9E5: B9 52 FA    | LDA $FA52,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFA52u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:B9E8: 9F 10 E9 7E | STA $7EE910,X      [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_longx(ctx, 0x7EE910u), (uint16_t)(ctx->a));
    /* $1D:B9EC: A0 00 00    | LDY #$0000         [M=16-bit, X=16-bit] */
    ctx->y = 0x0000u;
    snes_set_nz16(ctx, ctx->y);
L_b9ef: /* [M=16-bit, X=16-bit] */
    /* $1D:B9EF: 8C 9A 16    | STY $169A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x169Au), (uint16_t)(ctx->y));
    /* $1D:B9F2: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:B9F3: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_b9f4: /* [M=16-bit, X=16-bit] */
    /* $1D:B9F4: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B9F5: C0 20 00    | CPY #$0020         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->y, 0x0020u);
    /* $1D:B9F8: 90 3C       | BCC $BA36          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ba36;
    /* $1D:B9FA: A0 60 00    | LDY #$0060         [M=16-bit, X=16-bit] */
    ctx->y = 0x0060u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:B9FD: AD 9C 16    | LDA $169C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x169Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA00: F0 03       | BEQ $BA05          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ba05;
    /* $1D:BA02: A0 20 00    | LDY #$0020         [M=16-bit, X=16-bit] */
    ctx->y = 0x0020u;
    snes_set_nz16(ctx, ctx->y);
L_ba05: /* [M=16-bit, X=16-bit] */
    /* $1D:BA05: 8C 40 16    | STY $1640          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1640u), (uint16_t)(ctx->y));
    /* $1D:BA08: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
    /* $1D:BA0B: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
    /* $1D:BA0E: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_ba0f: /* [M=16-bit, X=16-bit] */
    /* $1D:BA0F: 88          | DEY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y - 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BA10: D0 24       | BNE $BA36          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ba36;
    /* $1D:BA12: A0 20 00    | LDY #$0020         [M=16-bit, X=16-bit] */
    ctx->y = 0x0020u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BA15: AD 9C 16    | LDA $169C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x169Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA18: F0 03       | BEQ $BA1D          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ba1d;
    /* $1D:BA1A: A0 01 00    | LDY #$0001         [M=16-bit, X=16-bit] */
    ctx->y = 0x0001u;
    snes_set_nz16(ctx, ctx->y);
L_ba1d: /* [M=16-bit, X=16-bit] */
    /* $1D:BA1D: 8C 40 16    | STY $1640          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1640u), (uint16_t)(ctx->y));
    /* $1D:BA20: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA21: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA22: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $1D:BA23: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:BA24: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:BA25: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:BA26: 69 20 5C    | ADC #$5C20         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x5C20u);
    /* $1D:BA29: 85 B0       | STA $B0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xB0u), (uint16_t)(ctx->a));
    /* $1D:BA2B: A9 00 A0    | LDA #$A000         [M=16-bit, X=16-bit] */
    ctx->a = 0xA000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA2E: 85 B4       | STA $B4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xB4u), (uint16_t)(ctx->a));
    /* $1D:BA30: A9 C0 00    | LDA #$00C0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00C0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA33: 85 B2       | STA $B2            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xB2u), (uint16_t)(ctx->a));
    /* $1D:BA35: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_ba36: /* [M=16-bit, X=16-bit] */
    /* $1D:BA36: 8C 9A 16    | STY $169A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x169Au), (uint16_t)(ctx->y));
    /* $1D:BA39: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BA3A: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA3B: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:BA3C: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:BA3D: 29 06 00    | AND #$0006         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0006u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA40: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BA41: B9 C8 F9    | LDA $F9C8,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xF9C8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA44: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $1D:BA45: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:BA46: E9 00 04    | SBC #$0400         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0400u);
    /* $1D:BA49: 85 FC       | STA $FC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(ctx->a));
    /* $1D:BA4B: AE 9C 16    | LDX $169C          [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x169Cu));
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BA4E: BF 00 E9 7E | LDA $7EE900,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x7EE900u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA52: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BA53: 68          | PLA                [M=16-bit, X=16-bit] */
    ctx->a = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA54: 22 15 90 00 | JSL $009015        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9015(ctx); ctx->pb = saved_pb; }
    /* $1D:BA58: AE 9C 16    | LDX $169C          [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x169Cu));
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BA5B: BF 10 E9 7E | LDA $7EE910,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x7EE910u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA5F: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BA60: A5 FC       | LDA $FC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA62: 22 15 90 00 | JSL $009015        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9015(ctx); ctx->pb = saved_pb; }
    /* $1D:BA66: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BA67: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ba68  (SNES $1D:BA68 | Ghidra 0x1dba68)
 * Discovery Provenance: JSR $BA68 @ $1D:B17C
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ba68(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BA68: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA6B: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:BA6E: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA70: F0 19       | BEQ $BA8B          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ba8b;
    /* $1D:BA72: 64 06       | STZ $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(0u));
    /* $1D:BA74: A9 C0 00    | LDA #$00C0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00C0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA77: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:BA79: A0 00 07    | LDY #$0700         [M=16-bit, X=16-bit] */
    ctx->y = 0x0700u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BA7C: A9 9C C6    | LDA #$C69C         [M=16-bit, X=16-bit] */
    ctx->a = 0xC69Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA7F: 22 3C CF 01 | JSL $01CF3C        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf3c(ctx); ctx->pb = saved_pb; }
    /* $1D:BA83: B0 06       | BCS $BA8B          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ba8b;
    /* $1D:BA85: A9 FC FF    | LDA #$FFFC         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFCu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA88: 99 1E 00    | STA $001E,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x001Eu), (uint16_t)(ctx->a));
L_ba8b: /* [M=16-bit, X=16-bit] */
    /* $1D:BA8B: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ba8c  (SNES $1D:BA8C | Ghidra 0x1dba8c)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ba8c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BA8C: 20 F1 B6    | JSR $B6F1          [M=16-bit, X=16-bit] */
    bank1d_func_b6f1(ctx);
    /* $1D:BA8F: 20 DE B7    | JSR $B7DE          [M=16-bit, X=16-bit] */
    bank1d_func_b7de(ctx);
    /* $1D:BA92: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA95: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:BA97: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BA9A: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:BA9C: 20 62 B7    | JSR $B762          [M=16-bit, X=16-bit] */
    bank1d_func_b762(ctx);
    /* $1D:BA9F: 20 3F B4    | JSR $B43F          [M=16-bit, X=16-bit] */
    bank1d_func_b43f(ctx);
    /* $1D:BAA2: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BAA4: 30 04       | BMI $BAAA          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_baaa;
    /* $1D:BAA6: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BAA8: 10 3F       | BPL $BAE9          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bae9;
L_baaa: /* [M=16-bit, X=16-bit] */
    /* $1D:BAAA: CE 80 1E    | DEC $1E80          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E80u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E80u), (uint16_t)(v16)); }
    /* $1D:BAAD: CE 80 1E    | DEC $1E80          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E80u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E80u), (uint16_t)(v16)); }
    /* $1D:BAB0: F0 02       | BEQ $BAB4          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bab4;
    /* $1D:BAB2: 10 35       | BPL $BAE9          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bae9;
L_bab4: /* [M=16-bit, X=16-bit] */
    /* $1D:BAB4: 9C 9C 02    | STZ $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(0u));
    /* $1D:BAB7: 9C 9E 02    | STZ $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(0u));
    /* $1D:BABA: 9C 9A 1E    | STZ $1E9A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E9Au), (uint16_t)(0u));
    /* $1D:BABD: 64 46       | STZ $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(0u));
    /* $1D:BABF: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BAC1: A9 F0 00    | LDA #$00F0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00F0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BAC4: 8D D8 13    | STA $13D8          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13D8u), (uint16_t)(ctx->a));
    /* $1D:BAC7: A9 24 00    | LDA #$0024         [M=16-bit, X=16-bit] */
    ctx->a = 0x0024u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BACA: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BACC: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BACD: A0 CD FE    | LDY #$FECD         [M=16-bit, X=16-bit] */
    ctx->y = 0xFECDu;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BAD0: 22 82 99 00 | JSL $009982        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9982(ctx); ctx->pb = saved_pb; }
    /* $1D:BAD4: A2 10 12    | LDX #$1210         [M=16-bit, X=16-bit] */
    ctx->x = 0x1210u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BAD7: A0 D9 FE    | LDY #$FED9         [M=16-bit, X=16-bit] */
    ctx->y = 0xFED9u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BADA: 22 85 99 00 | JSL $009985        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9985(ctx); ctx->pb = saved_pb; }
    /* $1D:BADE: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BADF: 9C 3E 16    | STZ $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(0u));
    /* $1D:BAE2: A9 81 00    | LDA #$0081         [M=16-bit, X=16-bit] */
    ctx->a = 0x0081u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BAE5: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
L_bae9: /* [M=16-bit, X=16-bit] */
    /* $1D:BAE9: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_baea  (SNES $1D:BAEA | Ghidra 0x1dbaea)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_baea(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BAEA: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BAEC: 10 3F       | BPL $BB2D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bb2d;
    /* $1D:BAEE: A9 80 00    | LDA #$0080         [M=16-bit, X=16-bit] */
    ctx->a = 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BAF1: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BAF3: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BAF4: A4 84       | LDY $84            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BAF6: BE 9B FD    | LDX $FD9B,Y        [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFD9Bu));
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BAF9: 22 A8 90 00 | JSL $0090A8        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_90a8(ctx); ctx->pb = saved_pb; }
    /* $1D:BAFD: A0 E8 FE    | LDY #$FEE8         [M=16-bit, X=16-bit] */
    ctx->y = 0xFEE8u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BB00: 22 82 99 00 | JSL $009982        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9982(ctx); ctx->pb = saved_pb; }
    /* $1D:BB04: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BB05: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BB07: A9 81 00    | LDA #$0081         [M=16-bit, X=16-bit] */
    ctx->a = 0x0081u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB0A: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:BB0E: 9C 9C 02    | STZ $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(0u));
    /* $1D:BB11: 9C 9E 02    | STZ $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(0u));
    /* $1D:BB14: 9C BA 02    | STZ $02BA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x02BAu), (uint16_t)(0u));
    /* $1D:BB17: A9 DD FF    | LDA #$FFDD         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFDDu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB1A: 8D 8E 02    | STA $028E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x028Eu), (uint16_t)(ctx->a));
    /* $1D:BB1D: AC 8A 1F    | LDY $1F8A          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1F8Au));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BB20: F0 03       | BEQ $BB25          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bb25;
    /* $1D:BB22: 8D 0E 02    | STA $020E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x020Eu), (uint16_t)(ctx->a));
L_bb25: /* [M=16-bit, X=16-bit] */
    /* $1D:BB25: AC CA 1F    | LDY $1FCA          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1FCAu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BB28: F0 03       | BEQ $BB2D          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bb2d;
    /* $1D:BB2A: 8D 4E 02    | STA $024E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x024Eu), (uint16_t)(ctx->a));
L_bb2d: /* [M=16-bit, X=16-bit] */
    /* $1D:BB2D: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bb2e  (SNES $1D:BB2E | Ghidra 0x1dbb2e)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bb2e(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BB2E: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BB30: 10 38       | BPL $BB6A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bb6a;
    /* $1D:BB32: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB35: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BB37: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB39: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:BB3C: F0 07       | BEQ $BB45          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bb45;
    /* $1D:BB3E: A9 29 00    | LDA #$0029         [M=16-bit, X=16-bit] */
    ctx->a = 0x0029u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB41: 95 12       | STA $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $1D:BB43: 80 02       | BRA $BB47          [M=16-bit, X=16-bit] */
    goto L_bb47;
L_bb45: /* [M=16-bit, X=16-bit] */
    /* $1D:BB45: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_bb47: /* [M=16-bit, X=16-bit] */
    /* $1D:BB47: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BB48: A2 C3 F4    | LDX #$F4C3         [M=16-bit, X=16-bit] */
    ctx->x = 0xF4C3u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BB4B: 22 D4 98 00 | JSL $0098D4        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_98d4(ctx); ctx->pb = saved_pb; }
    /* $1D:BB4F: A9 71 00    | LDA #$0071         [M=16-bit, X=16-bit] */
    ctx->a = 0x0071u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB52: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:BB56: A0 C7 FE    | LDY #$FEC7         [M=16-bit, X=16-bit] */
    ctx->y = 0xFEC7u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BB59: 22 82 99 00 | JSL $009982        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9982(ctx); ctx->pb = saved_pb; }
    /* $1D:BB5D: A2 DF FE    | LDX #$FEDF         [M=16-bit, X=16-bit] */
    ctx->x = 0xFEDFu;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BB60: 22 A8 90 00 | JSL $0090A8        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_90a8(ctx); ctx->pb = saved_pb; }
    /* $1D:BB64: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BB65: A9 13 00    | LDA #$0013         [M=16-bit, X=16-bit] */
    ctx->a = 0x0013u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB68: 85 46       | STA $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(ctx->a));
L_bb6a: /* [M=16-bit, X=16-bit] */
    /* $1D:BB6A: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bb6b  (SNES $1D:BB6B | Ghidra 0x1dbb6b)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bb6b(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BB6B: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BB6D: 10 14       | BPL $BB83          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bb83;
    /* $1D:BB6F: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BB71: A9 80 00    | LDA #$0080         [M=16-bit, X=16-bit] */
    ctx->a = 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB74: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BB76: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BB77: AC 3E 16    | LDY $163E          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BB7A: B9 BC FD    | LDA $FDBC,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFDBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB7D: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BB7E: 22 A8 90 00 | JSL $0090A8        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_90a8(ctx); ctx->pb = saved_pb; }
    /* $1D:BB82: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_bb83: /* [M=16-bit, X=16-bit] */
    /* $1D:BB83: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bb84  (SNES $1D:BB84 | Ghidra 0x1dbb84)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bb84(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BB84: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BB86: 10 07       | BPL $BB8F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bb8f;
    /* $1D:BB88: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB8B: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BB8D: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_bb8f: /* [M=16-bit, X=16-bit] */
    /* $1D:BB8F: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bb90  (SNES $1D:BB90 | Ghidra 0x1dbb90)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bb90(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BB90: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BB92: 10 14       | BPL $BBA8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bba8;
    /* $1D:BB94: A9 E0 00    | LDA #$00E0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00E0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BB97: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BB99: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BB9B: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BB9C: A2 10 12    | LDX #$1210         [M=16-bit, X=16-bit] */
    ctx->x = 0x1210u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BB9F: A0 D3 FE    | LDY #$FED3         [M=16-bit, X=16-bit] */
    ctx->y = 0xFED3u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BBA2: 22 85 99 00 | JSL $009985        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9985(ctx); ctx->pb = saved_pb; }
    /* $1D:BBA6: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BBA7: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_bba8: /* [M=16-bit, X=16-bit] */
    /* $1D:BBA8: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBAA: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:BBAD: D0 2A       | BNE $BBD9          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bbd9;
    /* $1D:BBAF: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBB1: F0 26       | BEQ $BBD9          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bbd9;
    /* $1D:BBB3: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBB5: 29 02 00    | AND #$0002         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBB8: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BBB9: B9 F7 FD    | LDA $FDF7,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFDF7u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBBC: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $1D:BBBD: B9 F3 FD    | LDA $FDF3,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFDF3u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBC0: AC 3E 16    | LDY $163E          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BBC3: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:BBC4: 79 FB FD    | ADC $FDFB,Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFDFBu)));
    /* $1D:BBC7: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BBC8: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBCB: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:BBCD: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBD0: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:BBD2: 68          | PLA                [M=16-bit, X=16-bit] */
    ctx->a = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBD3: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BBD4: 22 A3 FE 00 | JSL $00FEA3        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_fea3(ctx); ctx->pb = saved_pb; }
    /* $1D:BBD8: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_bbd9: /* [M=16-bit, X=16-bit] */
    /* $1D:BBD9: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bbda  (SNES $1D:BBDA | Ghidra 0x1dbbda)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bbda(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BBDA: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BBDC: 10 37       | BPL $BC15          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bc15;
    /* $1D:BBDE: A9 14 00    | LDA #$0014         [M=16-bit, X=16-bit] */
    ctx->a = 0x0014u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBE1: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BBE3: A9 19 00    | LDA #$0019         [M=16-bit, X=16-bit] */
    ctx->a = 0x0019u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBE6: 95 12       | STA $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $1D:BBE8: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BBE9: A2 10 12    | LDX #$1210         [M=16-bit, X=16-bit] */
    ctx->x = 0x1210u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BBEC: A0 D9 FE    | LDY #$FED9         [M=16-bit, X=16-bit] */
    ctx->y = 0xFED9u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BBEF: 22 85 99 00 | JSL $009985        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9985(ctx); ctx->pb = saved_pb; }
    /* $1D:BBF3: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BBF4: EE 3E 16    | INC $163E          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(v16)); }
    /* $1D:BBF7: EE 3E 16    | INC $163E          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(v16)); }
    /* $1D:BBFA: AD 3E 16    | LDA $163E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBFD: C9 0A 00    | CMP #$000A         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x000Au);
    /* $1D:BC00: 90 13       | BCC $BC15          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_bc15;
    /* $1D:BC02: A9 54 00    | LDA #$0054         [M=16-bit, X=16-bit] */
    ctx->a = 0x0054u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC05: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BC07: A9 1D 00    | LDA #$001D         [M=16-bit, X=16-bit] */
    ctx->a = 0x001Du;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC0A: 95 12       | STA $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $1D:BC0C: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BC0D: A0 CD FE    | LDY #$FECD         [M=16-bit, X=16-bit] */
    ctx->y = 0xFECDu;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BC10: 22 82 99 00 | JSL $009982        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9982(ctx); ctx->pb = saved_pb; }
    /* $1D:BC14: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_bc15: /* [M=16-bit, X=16-bit] */
    /* $1D:BC15: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bc16  (SNES $1D:BC16 | Ghidra 0x1dbc16)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bc16(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    goto L_bc16;
L_bba8: /* [M=16-bit, X=16-bit] */
    /* $1D:BBA8: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBAA: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:BBAD: D0 2A       | BNE $BBD9          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bbd9;
    /* $1D:BBAF: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBB1: F0 26       | BEQ $BBD9          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bbd9;
    /* $1D:BBB3: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBB5: 29 02 00    | AND #$0002         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBB8: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BBB9: B9 F7 FD    | LDA $FDF7,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFDF7u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBBC: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $1D:BBBD: B9 F3 FD    | LDA $FDF3,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFDF3u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBC0: AC 3E 16    | LDY $163E          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BBC3: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:BBC4: 79 FB FD    | ADC $FDFB,Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFDFBu)));
    /* $1D:BBC7: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BBC8: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBCB: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:BBCD: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBD0: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:BBD2: 68          | PLA                [M=16-bit, X=16-bit] */
    ctx->a = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BBD3: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BBD4: 22 A3 FE 00 | JSL $00FEA3        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_fea3(ctx); ctx->pb = saved_pb; }
    /* $1D:BBD8: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_bbd9: /* [M=16-bit, X=16-bit] */
    /* $1D:BBD9: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_bc16: /* [M=16-bit, X=16-bit] */
    /* $1D:BC16: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BC18: 30 03       | BMI $BC1D          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_bc1d;
    /* $1D:BC1A: 82 8B FF    | BRL $BBA8          [M=16-bit, X=16-bit] */
    goto L_bba8;
L_bc1d: /* [M=16-bit, X=16-bit] */
    /* $1D:BC1D: A9 80 03    | LDA #$0380         [M=16-bit, X=16-bit] */
    ctx->a = 0x0380u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC20: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BC22: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BC24: 64 46       | STZ $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(0u));
    /* $1D:BC26: 20 52 B9    | JSR $B952          [M=16-bit, X=16-bit] */
    bank1d_func_b952(ctx);
    /* $1D:BC29: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bc2a  (SNES $1D:BC2A | Ghidra 0x1dbc2a)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bc2a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BC2A: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC2C: F0 2A       | BEQ $BC58          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bc58;
    /* $1D:BC2E: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:BC2F: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC31: E9 80 00    | SBC #$0080         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0080u);
    /* $1D:BC34: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BC36: D0 11       | BNE $BC49          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bc49;
    /* $1D:BC38: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BC3A: A9 12 00    | LDA #$0012         [M=16-bit, X=16-bit] */
    ctx->a = 0x0012u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC3D: 85 46       | STA $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(ctx->a));
    /* $1D:BC3F: A9 1F 00    | LDA #$001F         [M=16-bit, X=16-bit] */
    ctx->a = 0x001Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC42: 8D 9A 1E    | STA $1E9A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E9Au), (uint16_t)(ctx->a));
    /* $1D:BC45: 9C 80 1E    | STZ $1E80          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E80u), (uint16_t)(0u));
    /* $1D:BC48: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_bc49: /* [M=16-bit, X=16-bit] */
    /* $1D:BC49: 69 00 5C    | ADC #$5C00         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x5C00u);
    /* $1D:BC4C: 85 B0       | STA $B0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xB0u), (uint16_t)(ctx->a));
    /* $1D:BC4E: A9 00 A0    | LDA #$A000         [M=16-bit, X=16-bit] */
    ctx->a = 0xA000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC51: 85 B4       | STA $B4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xB4u), (uint16_t)(ctx->a));
    /* $1D:BC53: A9 FE 00    | LDA #$00FE         [M=16-bit, X=16-bit] */
    ctx->a = 0x00FEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC56: 85 B2       | STA $B2            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xB2u), (uint16_t)(ctx->a));
L_bc58: /* [M=16-bit, X=16-bit] */
    /* $1D:BC58: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bc59  (SNES $1D:BC59 | Ghidra 0x1dbc59)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bc59(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BC59: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BC5B: 10 2C       | BPL $BC89          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bc89;
    /* $1D:BC5D: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC5F: F0 28       | BEQ $BC89          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bc89;
    /* $1D:BC61: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC64: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:BC66: A9 20 00    | LDA #$0020         [M=16-bit, X=16-bit] */
    ctx->a = 0x0020u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC69: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:BC6B: A0 6A 94    | LDY #$946A         [M=16-bit, X=16-bit] */
    ctx->y = 0x946Au;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BC6E: A9 0C 4C    | LDA #$4C0C         [M=16-bit, X=16-bit] */
    ctx->a = 0x4C0Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC71: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BC72: 22 A3 FE 00 | JSL $00FEA3        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_fea3(ctx); ctx->pb = saved_pb; }
    /* $1D:BC76: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BC77: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BC79: 20 42 B6    | JSR $B642          [M=16-bit, X=16-bit] */
    bank1d_func_b642(ctx);
    /* $1D:BC7C: A9 12 00    | LDA #$0012         [M=16-bit, X=16-bit] */
    ctx->a = 0x0012u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC7F: 8D 36 16    | STA $1636          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1636u), (uint16_t)(ctx->a));
    /* $1D:BC82: A9 59 00    | LDA #$0059         [M=16-bit, X=16-bit] */
    ctx->a = 0x0059u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC85: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
L_bc89: /* [M=16-bit, X=16-bit] */
    /* $1D:BC89: A9 58 00    | LDA #$0058         [M=16-bit, X=16-bit] */
    ctx->a = 0x0058u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC8C: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:BC8E: A9 3F 00    | LDA #$003F         [M=16-bit, X=16-bit] */
    ctx->a = 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC91: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $1D:BC93: 20 68 BA    | JSR $BA68          [M=16-bit, X=16-bit] */
    bank1d_func_ba68(ctx);
    /* $1D:BC96: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bc97  (SNES $1D:BC97 | Ghidra 0x1dbc97)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bc97(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BC97: AD 98 13    | LDA $1398          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1398u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BC9A: 89 FF 00    | BIT #$00FF         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x00FFu)) == 0u ? 1u : 0u);
    /* $1D:BC9D: F0 1C       | BEQ $BCBB          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bcbb;
    /* $1D:BC9F: 89 80 00    | BIT #$0080         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0080u)) == 0u ? 1u : 0u);
    /* $1D:BCA2: D0 17       | BNE $BCBB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bcbb;
    /* $1D:BCA4: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BCA6: 9C 9C 02    | STZ $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(0u));
    /* $1D:BCA9: 9C 9E 02    | STZ $029E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Eu), (uint16_t)(0u));
    /* $1D:BCAC: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:BCAE: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:BCB0: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BCB3: 8D BA 13    | STA $13BA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13BAu), (uint16_t)(ctx->a));
    /* $1D:BCB6: A9 00 01    | LDA #$0100         [M=16-bit, X=16-bit] */
    ctx->a = 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BCB9: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
L_bcbb: /* [M=16-bit, X=16-bit] */
    /* $1D:BCBB: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:BCBE: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bcbf  (SNES $1D:BCBF | Ghidra 0x1dbcbf)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bcbf(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    goto L_bcbf;
L_bcbb: /* [M=16-bit, X=16-bit] */
    /* $1D:BCBB: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:BCBE: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_bcbf: /* [M=16-bit, X=16-bit] */
    /* $1D:BCBF: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BCC1: 10 F8       | BPL $BCBB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bcbb;
    /* $1D:BCC3: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BCC5: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BCC8: 8D 9C 02    | STA $029C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x029Cu), (uint16_t)(ctx->a));
    /* $1D:BCCB: 80 EE       | BRA $BCBB          [M=16-bit, X=16-bit] */
    goto L_bcbb;
}

/* ==========================================================================
 * Function: bank1d_func_bccd  (SNES $1D:BCCD | Ghidra 0x1dbccd)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bccd(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    goto L_bccd;
L_bcbb: /* [M=16-bit, X=16-bit] */
    /* $1D:BCBB: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:BCBE: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_bccd: /* [M=16-bit, X=16-bit] */
    /* $1D:BCCD: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BCD0: 30 E9       | BMI $BCBB          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_bcbb;
    /* $1D:BCD2: C9 10 00    | CMP #$0010         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0010u);
    /* $1D:BCD5: 90 E4       | BCC $BCBB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_bcbb;
    /* $1D:BCD7: A9 10 00    | LDA #$0010         [M=16-bit, X=16-bit] */
    ctx->a = 0x0010u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BCDA: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BCDC: A9 10 00    | LDA #$0010         [M=16-bit, X=16-bit] */
    ctx->a = 0x0010u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BCDF: 95 12       | STA $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $1D:BCE1: 80 D8       | BRA $BCBB          [M=16-bit, X=16-bit] */
    goto L_bcbb;
}

/* ==========================================================================
 * Function: bank1d_func_bce3  (SNES $1D:BCE3 | Ghidra 0x1dbce3)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bce3(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BCE3: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BCE5: C9 08 01    | CMP #$0108         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0108u);
    /* $1D:BCE8: 90 0B       | BCC $BCF5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_bcf5;
    /* $1D:BCEA: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:BCEC: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:BCEE: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BCF0: A9 10 00    | LDA #$0010         [M=16-bit, X=16-bit] */
    ctx->a = 0x0010u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BCF3: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
L_bcf5: /* [M=16-bit, X=16-bit] */
    /* $1D:BCF5: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BCF7: 30 04       | BMI $BCFD          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_bcfd;
    /* $1D:BCF9: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BCFB: 80 05       | BRA $BD02          [M=16-bit, X=16-bit] */
    goto L_bd02;
L_bcfd: /* [M=16-bit, X=16-bit] */
    /* $1D:BCFD: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD00: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
L_bd02: /* [M=16-bit, X=16-bit] */
    /* $1D:BD02: 20 58 B7    | JSR $B758          [M=16-bit, X=16-bit] */
    bank1d_func_b758(ctx);
    /* $1D:BD05: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:BD08: 20 4A B4    | JSR $B44A          [M=16-bit, X=16-bit] */
    bank1d_func_b44a(ctx);
    /* $1D:BD0B: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:BD0E: 20 6A BD    | JSR $BD6A          [M=16-bit, X=16-bit] */
    bank1d_func_bd6a(ctx);
    /* $1D:BD11: A9 3F 00    | LDA #$003F         [M=16-bit, X=16-bit] */
    ctx->a = 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD14: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $1D:BD16: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $1D:BD18: 20 A0 BD    | JSR $BDA0          [M=16-bit, X=16-bit] */
    bank1d_func_bda0(ctx);
    /* $1D:BD1B: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bd1c  (SNES $1D:BD1C | Ghidra 0x1dbd1c)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bd1c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BD1C: 20 58 B7    | JSR $B758          [M=16-bit, X=16-bit] */
    bank1d_func_b758(ctx);
    /* $1D:BD1F: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:BD22: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:BD25: 20 89 BD    | JSR $BD89          [M=16-bit, X=16-bit] */
    bank1d_func_bd89(ctx);
    /* $1D:BD28: 20 6A BD    | JSR $BD6A          [M=16-bit, X=16-bit] */
    bank1d_func_bd6a(ctx);
    /* $1D:BD2B: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BD2D: 10 02       | BPL $BD31          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bd31;
    /* $1D:BD2F: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_bd31: /* [M=16-bit, X=16-bit] */
    /* $1D:BD31: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bd32  (SNES $1D:BD32 | Ghidra 0x1dbd32)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bd32(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BD32: 20 58 B7    | JSR $B758          [M=16-bit, X=16-bit] */
    bank1d_func_b758(ctx);
    /* $1D:BD35: 20 85 B6    | JSR $B685          [M=16-bit, X=16-bit] */
    bank1d_func_b685(ctx);
    /* $1D:BD38: 20 1F B4    | JSR $B41F          [M=16-bit, X=16-bit] */
    bank1d_func_b41f(ctx);
    /* $1D:BD3B: 20 89 BD    | JSR $BD89          [M=16-bit, X=16-bit] */
    bank1d_func_bd89(ctx);
    /* $1D:BD3E: 20 6A BD    | JSR $BD6A          [M=16-bit, X=16-bit] */
    bank1d_func_bd6a(ctx);
    /* $1D:BD41: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BD43: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD46: 8D BC 16    | STA $16BC          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16BCu), (uint16_t)(ctx->a));
    /* $1D:BD49: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bd4a  (SNES $1D:BD4A | Ghidra 0x1dbd4a)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bd4a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BD4A: B4 20       | LDY $20,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BD4C: D0 06       | BNE $BD54          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bd54;
    /* $1D:BD4E: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD51: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:BD53: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_bd54: /* [M=16-bit, X=16-bit] */
    /* $1D:BD54: B9 04 00    | LDA $0004,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD57: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:BD59: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:BD5A: B5 1C       | LDA $1C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD5C: 69 00 80    | ADC #$8000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x8000u);
    /* $1D:BD5F: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:BD61: B5 1E       | LDA $1E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD63: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:BD66: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:BD68: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bd69  (SNES $1D:BD69 | Ghidra 0x1dbd69)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bd69(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BD69: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bd6a  (SNES $1D:BD6A | Ghidra 0x1dbd6a)
 * Discovery Provenance: JSR $BD6A @ $1D:BD28
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bd6a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BD6A: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD6D: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:BD70: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD72: 89 01 00    | BIT #$0001         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0001u)) == 0u ? 1u : 0u);
    /* $1D:BD75: D0 11       | BNE $BD88          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bd88;
    /* $1D:BD77: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD79: 29 03 00    | AND #$0003         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0003u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD7C: D0 0A       | BNE $BD88          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bd88;
    /* $1D:BD7E: A9 2A 91    | LDA #$912A         [M=16-bit, X=16-bit] */
    ctx->a = 0x912Au;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD81: 85 9A       | STA $9A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x9Au), (uint16_t)(ctx->a));
    /* $1D:BD83: A9 40 30    | LDA #$3040         [M=16-bit, X=16-bit] */
    ctx->a = 0x3040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD86: 85 98       | STA $98            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x98u), (uint16_t)(ctx->a));
L_bd88: /* [M=16-bit, X=16-bit] */
    /* $1D:BD88: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bd89  (SNES $1D:BD89 | Ghidra 0x1dbd89)
 * Discovery Provenance: JSR $BD89 @ $1D:BD25
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bd89(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BD89: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD8B: C9 E0 00    | CMP #$00E0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00E0u);
    /* $1D:BD8E: 90 06       | BCC $BD96          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_bd96;
    /* $1D:BD90: A5 BE       | LDA $BE            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBEu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD92: D0 02       | BNE $BD96          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bd96;
    /* $1D:BD94: D6 0E       | DEC $0E,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(v16)); }
L_bd96: /* [M=16-bit, X=16-bit] */
    /* $1D:BD96: A9 FF 00    | LDA #$00FF         [M=16-bit, X=16-bit] */
    ctx->a = 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD99: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $1D:BD9B: A9 7F 00    | LDA #$007F         [M=16-bit, X=16-bit] */
    ctx->a = 0x007Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BD9E: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* Fallthrough into function $1D:BDA0 */
    bank1d_func_bda0(ctx);
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bda0  (SNES $1D:BDA0 | Ghidra 0x1dbda0)
 * Discovery Provenance: JSR $BDA0 @ $1D:BD18
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bda0(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BDA0: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDA2: 69 F0 FF    | ADC #$FFF0         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFF0u);
    /* $1D:BDA5: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:BDA7: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDA9: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:BDAB: A0 C0 02    | LDY #$02C0         [M=16-bit, X=16-bit] */
    ctx->y = 0x02C0u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BDAE: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDB1: 85 FC       | STA $FC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(ctx->a));
L_bdb3: /* [M=16-bit, X=16-bit] */
    /* $1D:BDB3: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDB5: 65 EA       | ADC $EA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xEAu)));
    /* $1D:BDB7: 65 C0       | ADC $C0            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC0u)));
    /* $1D:BDB9: 85 E8       | STA $E8            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xE8u), (uint16_t)(ctx->a));
    /* $1D:BDBB: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDBC: 65 3A       | ADC $3A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x3Au)));
    /* $1D:BDBE: 65 EA       | ADC $EA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xEAu)));
    /* $1D:BDC0: 85 EA       | STA $EA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xEAu), (uint16_t)(ctx->a));
    /* $1D:BDC2: A9 58 C5    | LDA #$C558         [M=16-bit, X=16-bit] */
    ctx->a = 0xC558u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDC5: 22 3C CF 01 | JSL $01CF3C        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf3c(ctx); ctx->pb = saved_pb; }
    /* $1D:BDC9: B0 2B       | BCS $BDF6          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_bdf6;
    /* $1D:BDCB: A9 98 00    | LDA #$0098         [M=16-bit, X=16-bit] */
    ctx->a = 0x0098u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDCE: 99 10 00    | STA $0010,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0010u), (uint16_t)(ctx->a));
    /* $1D:BDD1: A9 FE FF    | LDA #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDD4: 99 1E 00    | STA $001E,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x001Eu), (uint16_t)(ctx->a));
    /* $1D:BDD7: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BDD8: A2 00 00    | LDX #$0000         [M=16-bit, X=16-bit] */
    ctx->x = 0x0000u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BDDB: B9 0A 00    | LDA $000A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDDE: C9 80 00    | CMP #$0080         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0080u);
    /* $1D:BDE1: 90 02       | BCC $BDE5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_bde5;
    /* $1D:BDE3: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BDE4: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
L_bde5: /* [M=16-bit, X=16-bit] */
    /* $1D:BDE5: BD F1 FE    | LDA $FEF1,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0xFEF1u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDE8: 99 18 00    | STA $0018,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0018u), (uint16_t)(ctx->a));
    /* $1D:BDEB: BD F5 FE    | LDA $FEF5,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0xFEF5u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDEE: 99 1A 00    | STA $001A,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x001Au), (uint16_t)(ctx->a));
    /* $1D:BDF1: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BDF2: C6 FC       | DEC $FC            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(v16)); }
    /* $1D:BDF4: 10 BD       | BPL $BDB3          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bdb3;
L_bdf6: /* [M=16-bit, X=16-bit] */
    /* $1D:BDF6: 22 8D FD 00 | JSL $00FD8D        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_fd8d(ctx); ctx->pb = saved_pb; }
    /* $1D:BDFA: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bdfb  (SNES $1D:BDFB | Ghidra 0x1dbdfb)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bdfb(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BDFB: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BDFD: 30 04       | BMI $BE03          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_be03;
    /* $1D:BDFF: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BE01: 10 29       | BPL $BE2C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) { bank1d_func_be2c(ctx); return; }
L_be03: /* [M=16-bit, X=16-bit] */
    /* $1D:BE03: A5 28       | LDA $28            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x28u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE05: 05 2A       | ORA $2A            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x2Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE07: 89 00 10    | BIT #$1000         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x1000u)) == 0u ? 1u : 0u);
    /* $1D:BE0A: D0 05       | BNE $BE11          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_be11;
    /* $1D:BE0C: 20 61 BE    | JSR $BE61          [M=16-bit, X=16-bit] */
    bank1d_func_be61(ctx);
    /* $1D:BE0F: 80 1B       | BRA $BE2C          [M=16-bit, X=16-bit] */
    bank1d_func_be2c(ctx);
    return;
L_be11: /* [M=16-bit, X=16-bit] */
    /* $1D:BE11: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BE12: A2 0A FF    | LDX #$FF0A         [M=16-bit, X=16-bit] */
    ctx->x = 0xFF0Au;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BE15: A9 00 04    | LDA #$0400         [M=16-bit, X=16-bit] */
    ctx->a = 0x0400u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE18: 22 15 90 00 | JSL $009015        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9015(ctx); ctx->pb = saved_pb; }
    /* $1D:BE1C: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BE1D: A9 00 01    | LDA #$0100         [M=16-bit, X=16-bit] */
    ctx->a = 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE20: 8D 86 16    | STA $1686          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1686u), (uint16_t)(ctx->a));
    /* $1D:BE23: A9 84 00    | LDA #$0084         [M=16-bit, X=16-bit] */
    ctx->a = 0x0084u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE26: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:BE2A: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* Fallthrough into function $1D:BE2C */
    bank1d_func_be2c(ctx);
    return;
}

/* ==========================================================================
 * Function: bank1d_func_be2c  (SNES $1D:BE2C | Ghidra 0x1dbe2c)
 * Discovery Provenance: JMP ($0000) table $1D:AF81 @ $1D:AF7E
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_be2c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BE2C: 20 30 BE    | JSR $BE30          [M=16-bit, X=16-bit] */
    bank1d_func_be30(ctx);
    /* $1D:BE2F: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_be30  (SNES $1D:BE30 | Ghidra 0x1dbe30)
 * Discovery Provenance: JSR $BE30 @ $1D:BE2C
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_be30(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BE30: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE32: F0 2C       | BEQ $BE60          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_be60;
    /* $1D:BE34: A9 FF 00    | LDA #$00FF         [M=16-bit, X=16-bit] */
    ctx->a = 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE37: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $1D:BE39: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $1D:BE3B: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:BE3D: A9 10 00    | LDA #$0010         [M=16-bit, X=16-bit] */
    ctx->a = 0x0010u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE40: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:BE42: A9 EA C6    | LDA #$C6EA         [M=16-bit, X=16-bit] */
    ctx->a = 0xC6EAu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE45: A4 BC       | LDY $BC            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BE47: D0 03       | BNE $BE4C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_be4c;
    /* $1D:BE49: A9 02 C7    | LDA #$C702         [M=16-bit, X=16-bit] */
    ctx->a = 0xC702u;
    snes_set_nz16(ctx, ctx->a);
L_be4c: /* [M=16-bit, X=16-bit] */
    /* $1D:BE4C: A0 00 03    | LDY #$0300         [M=16-bit, X=16-bit] */
    ctx->y = 0x0300u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BE4F: 22 3C CF 01 | JSL $01CF3C        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf3c(ctx); ctx->pb = saved_pb; }
    /* $1D:BE53: B0 0B       | BCS $BE60          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_be60;
    /* $1D:BE55: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE57: 29 07 00    | AND #$0007         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0007u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE5A: 69 FA FF    | ADC #$FFFA         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFFAu);
    /* $1D:BE5D: 99 1A 00    | STA $001A,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x001Au), (uint16_t)(ctx->a));
L_be60: /* [M=16-bit, X=16-bit] */
    /* $1D:BE60: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_be61  (SNES $1D:BE61 | Ghidra 0x1dbe61)
 * Discovery Provenance: JSR $BE61 @ $1D:BE0C
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_be61(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BE61: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BE62: A2 F9 FE    | LDX #$FEF9         [M=16-bit, X=16-bit] */
    ctx->x = 0xFEF9u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BE65: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE67: 89 10 00    | BIT #$0010         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0010u)) == 0u ? 1u : 0u);
    /* $1D:BE6A: D0 03       | BNE $BE6F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_be6f;
    /* $1D:BE6C: A2 0A FF    | LDX #$FF0A         [M=16-bit, X=16-bit] */
    ctx->x = 0xFF0Au;
    snes_set_nz16(ctx, ctx->x);
L_be6f: /* [M=16-bit, X=16-bit] */
    /* $1D:BE6F: A9 00 04    | LDA #$0400         [M=16-bit, X=16-bit] */
    ctx->a = 0x0400u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE72: 22 15 90 00 | JSL $009015        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_9015(ctx); ctx->pb = saved_pb; }
    /* $1D:BE76: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BE77: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_be78  (SNES $1D:BE78 | Ghidra 0x1dbe78)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_be78(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BE78: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE7B: 30 03       | BMI $BE80          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_be80;
    /* $1D:BE7D: CE 44 16    | DEC $1644          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1644u), (uint16_t)(v16)); }
L_be80: /* [M=16-bit, X=16-bit] */
    /* $1D:BE80: B5 26       | LDA $26,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x26u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE82: F0 21       | BEQ $BEA5          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_bea5;
    /* $1D:BE84: A9 2A 91    | LDA #$912A         [M=16-bit, X=16-bit] */
    ctx->a = 0x912Au;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE87: 85 9A       | STA $9A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x9Au), (uint16_t)(ctx->a));
    /* $1D:BE89: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE8C: 85 98       | STA $98            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x98u), (uint16_t)(ctx->a));
    /* $1D:BE8E: AD BA 16    | LDA $16BA          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x16BAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE91: 30 05       | BMI $BE98          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_be98;
    /* $1D:BE93: CE BA 16    | DEC $16BA          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x16BAu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16BAu), (uint16_t)(v16)); }
    /* $1D:BE96: 80 0D       | BRA $BEA5          [M=16-bit, X=16-bit] */
    goto L_bea5;
L_be98: /* [M=16-bit, X=16-bit] */
    /* $1D:BE98: A9 3C 00    | LDA #$003C         [M=16-bit, X=16-bit] */
    ctx->a = 0x003Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BE9B: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:BE9F: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEA2: 8D BA 16    | STA $16BA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16BAu), (uint16_t)(ctx->a));
L_bea5: /* [M=16-bit, X=16-bit] */
    /* $1D:BEA5: B5 06       | LDA $06,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEA7: 10 2F       | BPL $BED8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bed8;
    /* $1D:BEA9: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEAC: 8D 88 16    | STA $1688          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1688u), (uint16_t)(ctx->a));
    /* $1D:BEAF: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEB2: 8D 38 16    | STA $1638          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1638u), (uint16_t)(ctx->a));
    /* $1D:BEB5: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEB8: 95 12       | STA $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $1D:BEBA: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEBD: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:BEBF: A9 00 01    | LDA #$0100         [M=16-bit, X=16-bit] */
    ctx->a = 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEC2: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BEC4: 74 06       | STZ $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(0u));
    /* $1D:BEC6: A9 3F 00    | LDA #$003F         [M=16-bit, X=16-bit] */
    ctx->a = 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEC9: 95 34       | STA $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(ctx->a));
    /* $1D:BECB: A9 1F 00    | LDA #$001F         [M=16-bit, X=16-bit] */
    ctx->a = 0x001Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BECE: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
    /* $1D:BED0: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:BED2: 74 1A       | STZ $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(0u));
    /* $1D:BED4: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:BED6: 74 18       | STZ $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(0u));
L_bed8: /* [M=16-bit, X=16-bit] */
    /* $1D:BED8: B5 12       | LDA $12,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEDA: F0 12       | BEQ $BEEE          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_beee;
    /* $1D:BEDC: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:BEDD: A9 60 01    | LDA #$0160         [M=16-bit, X=16-bit] */
    ctx->a = 0x0160u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEE0: F5 0A       | SBC $0A,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au)));
    /* $1D:BEE2: 8D C0 13    | STA $13C0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13C0u), (uint16_t)(ctx->a));
    /* $1D:BEE5: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:BEE6: A9 40 01    | LDA #$0140         [M=16-bit, X=16-bit] */
    ctx->a = 0x0140u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEE9: F5 0E       | SBC $0E,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu)));
    /* $1D:BEEB: 8D D8 13    | STA $13D8          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x13D8u), (uint16_t)(ctx->a));
L_beee: /* [M=16-bit, X=16-bit] */
    /* $1D:BEEE: B5 12       | LDA $12,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEF0: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BEF1: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:BEF2: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BEF3: BF FD BE 1D | LDA $1DBEFD,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x1DBEFDu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BEF7: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BEF8: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:BEFA: 6C 00 00    | JMP ($0000)        [M=16-bit, X=16-bit] */
    {
        uint16_t jmp_tgt = snes_bus_read16(ctx, 0x000000u);
        switch (jmp_tgt) {
            case 0xBF13u: bank1d_func_bf13(ctx); return;
            case 0xBF5Du: bank1d_func_bf5d(ctx); return;
            case 0xBF76u: bank1d_func_bf76(ctx); return;
            case 0xBF89u: bank1d_func_bf89(ctx); return;
            case 0xBFA3u: bank1d_func_bfa3(ctx); return;
            case 0xC03Bu: bank1d_func_c03b(ctx); return;
            case 0xC05Bu: bank1d_func_c05b(ctx); return;
            case 0xC089u: bank1d_func_c089(ctx); return;
            case 0xC125u: bank1d_func_c125(ctx); return;
            case 0xC145u: bank1d_func_c145(ctx); return;
            case 0xC14Cu: bank1d_func_c14c(ctx); return;
            default:
                // REQUIRES INTERPRETER FALLBACK
                snes_interp_fallback_at(ctx, 0x1DBEFAu, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);
                return;
        }
    }
}

/* ==========================================================================
 * Function: bank1d_func_bf13  (SNES $1D:BF13 | Ghidra 0x1dbf13)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bf13(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BF13: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BF15: A4 84       | LDY $84            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BF17: A9 31 00    | LDA #$0031         [M=16-bit, X=16-bit] */
    ctx->a = 0x0031u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF1A: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:BF1C: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF1F: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BF21: B9 9B FF    | LDA $FF9B,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF9Bu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF24: 95 06       | STA $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $1D:BF26: A9 38 00    | LDA #$0038         [M=16-bit, X=16-bit] */
    ctx->a = 0x0038u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF29: 95 28       | STA $28,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x28u), (uint16_t)(ctx->a));
    /* $1D:BF2B: A9 D0 FF    | LDA #$FFD0         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFD0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF2E: 95 2A       | STA $2A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Au), (uint16_t)(ctx->a));
    /* $1D:BF30: A9 4B 01    | LDA #$014B         [M=16-bit, X=16-bit] */
    ctx->a = 0x014Bu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF33: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
    /* $1D:BF35: 74 38       | STZ $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(0u));
    /* $1D:BF37: 74 34       | STZ $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(0u));
    /* $1D:BF39: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:BF3A: A9 40 01    | LDA #$0140         [M=16-bit, X=16-bit] */
    ctx->a = 0x0140u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF3D: ED D8 13    | SBC $13D8          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x13D8u)));
    /* $1D:BF40: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:BF42: A9 60 00    | LDA #$0060         [M=16-bit, X=16-bit] */
    ctx->a = 0x0060u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF45: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:BF47: 9C 38 16    | STZ $1638          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1638u), (uint16_t)(0u));
    /* $1D:BF4A: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:BF4B: A2 27 FF    | LDX #$FF27         [M=16-bit, X=16-bit] */
    ctx->x = 0xFF27u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BF4E: 22 A8 90 00 | JSL $0090A8        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_90a8(ctx); ctx->pb = saved_pb; }
    /* $1D:BF52: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:BF53: 9C 42 16    | STZ $1642          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(0u));
    /* $1D:BF56: 9C BA 16    | STZ $16BA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x16BAu), (uint16_t)(0u));
    /* $1D:BF59: 5C 33 E7 00 | JML $00E733        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_e733(ctx); ctx->pb = saved_pb; }
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bf5d  (SNES $1D:BF5D | Ghidra 0x1dbf5d)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bf5d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BF5D: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:BF5F: 10 11       | BPL $BF72          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_bf72;
    /* $1D:BF61: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF64: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:BF66: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BF68: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF6B: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BF6D: A9 10 00    | LDA #$0010         [M=16-bit, X=16-bit] */
    ctx->a = 0x0010u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF70: 85 46       | STA $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(ctx->a));
L_bf72: /* [M=16-bit, X=16-bit] */
    /* $1D:BF72: 5C 33 E7 00 | JML $00E733        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_e733(ctx); ctx->pb = saved_pb; }
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bf76  (SNES $1D:BF76 | Ghidra 0x1dbf76)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bf76(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BF76: 20 48 CB    | JSR $CB48          [M=16-bit, X=16-bit] */
    bank1d_func_cb48(ctx);
    /* $1D:BF79: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF7B: 30 0B       | BMI $BF88          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_bf88;
    /* $1D:BF7D: C9 70 00    | CMP #$0070         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0070u);
    /* $1D:BF80: 90 06       | BCC $BF88          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_bf88;
    /* $1D:BF82: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:BF84: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BF86: 64 46       | STZ $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(0u));
L_bf88: /* [M=16-bit, X=16-bit] */
    /* $1D:BF88: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bf89  (SNES $1D:BF89 | Ghidra 0x1dbf89)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bf89(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BF89: 20 58 CC    | JSR $CC58          [M=16-bit, X=16-bit] */
    bank1d_func_cc58(ctx);
    /* $1D:BF8C: 20 C0 CC    | JSR $CCC0          [M=16-bit, X=16-bit] */
    bank1d_func_ccc0(ctx);
    /* $1D:BF8F: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF91: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF94: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:BF97: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BF9A: 8D 3C 16    | STA $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(ctx->a));
    /* $1D:BF9D: 9C 3E 16    | STZ $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(0u));
    /* $1D:BFA0: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:BFA2: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_bfa3  (SNES $1D:BFA3 | Ghidra 0x1dbfa3)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_bfa3(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:BFA3: AD 3E 16    | LDA $163E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFA6: C9 01 00    | CMP #$0001         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0001u);
    /* $1D:BFA9: B0 29       | BCS $BFD4          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_bfd4;
    /* $1D:BFAB: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFAD: 89 3F 00    | BIT #$003F         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x003Fu)) == 0u ? 1u : 0u);
    /* $1D:BFB0: D0 03       | BNE $BFB5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bfb5;
    /* $1D:BFB2: 20 C0 CC    | JSR $CCC0          [M=16-bit, X=16-bit] */
    bank1d_func_ccc0(ctx);
L_bfb5: /* [M=16-bit, X=16-bit] */
    /* $1D:BFB5: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFB8: 8D 60 16    | STA $1660          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1660u), (uint16_t)(ctx->a));
    /* $1D:BFBB: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFBE: C9 30 00    | CMP #$0030         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0030u);
    /* $1D:BFC1: B0 07       | BCS $BFCA          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_bfca;
    /* $1D:BFC3: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFC5: D0 03       | BNE $BFCA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bfca;
    /* $1D:BFC7: EE 3C 16    | INC $163C          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(v16)); }
L_bfca: /* [M=16-bit, X=16-bit] */
    /* $1D:BFCA: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:BFCB: AD 3A 16    | LDA $163A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFCE: 75 3E       | ADC $3E,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Eu)));
    /* $1D:BFD0: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:BFD3: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_bfd4: /* [M=16-bit, X=16-bit] */
    /* $1D:BFD4: C9 02 00    | CMP #$0002         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0002u);
    /* $1D:BFD7: F0 2C       | BEQ $C005          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c005;
    /* $1D:BFD9: A9 60 00    | LDA #$0060         [M=16-bit, X=16-bit] */
    ctx->a = 0x0060u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFDC: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:BFDE: A0 00 03    | LDY #$0300         [M=16-bit, X=16-bit] */
    ctx->y = 0x0300u;
    snes_set_nz16(ctx, ctx->y);
L_bfe1: /* [M=16-bit, X=16-bit] */
    /* $1D:BFE1: B9 06 00    | LDA $0006,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0006u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFE4: D0 08       | BNE $BFEE          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_bfee;
    /* $1D:BFE6: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFE7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:BFE8: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $1D:BFEB: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:BFEC: 80 F3       | BRA $BFE1          [M=16-bit, X=16-bit] */
    goto L_bfe1;
L_bfee: /* [M=16-bit, X=16-bit] */
    /* $1D:BFEE: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFF1: 99 16 00    | STA $0016,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0016u), (uint16_t)(ctx->a));
    /* $1D:BFF4: B9 24 00    | LDA $0024,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0024u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:BFF7: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:BFF8: 8D 36 16    | STA $1636          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1636u), (uint16_t)(ctx->a));
    /* $1D:BFFB: EE 3E 16    | INC $163E          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(v16)); }
    /* $1D:BFFE: A0 5D FF    | LDY #$FF5D         [M=16-bit, X=16-bit] */
    ctx->y = 0xFF5Du;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C001: 20 1C CC    | JSR $CC1C          [M=16-bit, X=16-bit] */
    bank1d_func_cc1c(ctx);
L_c004: /* [M=16-bit, X=16-bit] */
    /* $1D:C004: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c005: /* [M=16-bit, X=16-bit] */
    /* $1D:C005: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:C007: 10 FB       | BPL $C004          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c004;
    /* $1D:C009: A9 11 00    | LDA #$0011         [M=16-bit, X=16-bit] */
    ctx->a = 0x0011u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C00C: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:C00E: A5 EA       | LDA $EA            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xEAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C010: 29 3F 00    | AND #$003F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C013: 69 00 02    | ADC #$0200         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0200u);
    /* $1D:C016: 8D 44 16    | STA $1644          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1644u), (uint16_t)(ctx->a));
    /* $1D:C019: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:C01B: AD 36 16    | LDA $1636          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C01E: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C01F: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:C020: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C021: BF 2B C0 1D | LDA $1DC02B,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x1DC02Bu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C025: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C026: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C028: 6C 00 00    | JMP ($0000)        [M=16-bit, X=16-bit] */
    {
        uint16_t jmp_tgt = snes_bus_read16(ctx, 0x000000u);
        switch (jmp_tgt) {
            case 0xC559u: bank1d_func_c559(ctx); return;
            case 0xC57Au: bank1d_func_c57a(ctx); return;
            case 0xC580u: bank1d_func_c580(ctx); return;
            case 0xC586u: bank1d_func_c586(ctx); return;
            case 0xC5A7u: bank1d_func_c5a7(ctx); return;
            case 0xC5B7u: bank1d_func_c5b7(ctx); return;
            case 0xC5C9u: bank1d_func_c5c9(ctx); return;
            case 0xC5FFu: bank1d_func_c5ff(ctx); return;
            default:
                // REQUIRES INTERPRETER FALLBACK
                snes_interp_fallback_at(ctx, 0x1DC028u, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);
                return;
        }
    }
}

/* ==========================================================================
 * Function: bank1d_func_c03b  (SNES $1D:C03B | Ghidra 0x1dc03b)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c03b(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C03B: AD 36 16    | LDA $1636          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C03E: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C03F: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:C040: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C041: BF 4B C0 1D | LDA $1DC04B,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x1DC04Bu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C045: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C046: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C048: 6C 00 00    | JMP ($0000)        [M=16-bit, X=16-bit] */
    {
        uint16_t jmp_tgt = snes_bus_read16(ctx, 0x000000u);
        switch (jmp_tgt) {
            case 0xC1B8u: bank1d_func_c1b8(ctx); return;
            case 0xC23Du: bank1d_func_c23d(ctx); return;
            case 0xC2ABu: bank1d_func_c2ab(ctx); return;
            case 0xC31Bu: bank1d_func_c31b(ctx); return;
            case 0xC35Au: bank1d_func_c35a(ctx); return;
            case 0xC372u: bank1d_func_c372(ctx); return;
            case 0xC3FCu: bank1d_func_c3fc(ctx); return;
            case 0xC51Bu: bank1d_func_c51b(ctx); return;
            default:
                // REQUIRES INTERPRETER FALLBACK
                snes_interp_fallback_at(ctx, 0x1DC048u, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);
                return;
        }
    }
}

/* ==========================================================================
 * Function: bank1d_func_c05b  (SNES $1D:C05B | Ghidra 0x1dc05b)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c05b(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C05B: B5 06       | LDA $06,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C05D: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $1D:C060: 90 03       | BCC $C065          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c065;
    /* $1D:C062: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:C063: 95 06       | STA $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(ctx->a));
L_c065: /* [M=16-bit, X=16-bit] */
    /* $1D:C065: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:C067: 74 3A       | STZ $3A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(0u));
    /* $1D:C069: A9 10 00    | LDA #$0010         [M=16-bit, X=16-bit] */
    ctx->a = 0x0010u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C06C: 85 46       | STA $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(ctx->a));
    /* $1D:C06E: A9 4B 01    | LDA #$014B         [M=16-bit, X=16-bit] */
    ctx->a = 0x014Bu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C071: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
    /* $1D:C073: A9 31 00    | LDA #$0031         [M=16-bit, X=16-bit] */
    ctx->a = 0x0031u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C076: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:C078: 74 18       | STZ $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(0u));
    /* $1D:C07A: 74 1A       | STZ $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(0u));
    /* $1D:C07C: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:C07E: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:C080: 74 3E       | STZ $3E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(0u));
    /* $1D:C082: 20 48 CB    | JSR $CB48          [M=16-bit, X=16-bit] */
    bank1d_func_cb48(ctx);
    /* $1D:C085: 9C 42 16    | STZ $1642          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(0u));
    /* $1D:C088: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c089  (SNES $1D:C089 | Ghidra 0x1dc089)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c089(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C089: B5 36       | LDA $36,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C08B: B4 3A       | LDY $3A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Au));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C08D: D0 52       | BNE $C0E1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c0e1;
    /* $1D:C08F: C9 11 01    | CMP #$0111         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0111u);
    /* $1D:C092: B0 47       | BCS $C0DB          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c0db;
    /* $1D:C094: F6 3A       | INC $3A,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Au))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(v16)); }
    /* $1D:C096: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C099: 85 FC       | STA $FC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(ctx->a));
L_c09b: /* [M=16-bit, X=16-bit] */
    /* $1D:C09B: 22 AB 9E 01 | JSL $019EAB        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_9eab(ctx); ctx->pb = saved_pb; }
    /* $1D:C09F: B0 25       | BCS $C0C6          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c0c6;
    /* $1D:C0A1: A9 DE C6    | LDA #$C6DE         [M=16-bit, X=16-bit] */
    ctx->a = 0xC6DEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0A4: 22 66 CF 01 | JSL $01CF66        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf66(ctx); ctx->pb = saved_pb; }
    /* $1D:C0A8: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0AA: 69 E4 FF    | ADC #$FFE4         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE4u);
    /* $1D:C0AD: 99 0E 00    | STA $000E,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x000Eu), (uint16_t)(ctx->a));
    /* $1D:C0B0: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0B2: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:C0B5: 99 0A 00    | STA $000A,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x000Au), (uint16_t)(ctx->a));
    /* $1D:C0B8: A5 FC       | LDA $FC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0BA: 99 1A 00    | STA $001A,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x001Au), (uint16_t)(ctx->a));
    /* $1D:C0BD: C6 FC       | DEC $FC            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(v16)); }
    /* $1D:C0BF: A5 FC       | LDA $FC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0C1: C9 FB FF    | CMP #$FFFB         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFFBu);
    /* $1D:C0C4: D0 D5       | BNE $C09B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c09b;
L_c0c6: /* [M=16-bit, X=16-bit] */
    /* $1D:C0C6: A9 80 01    | LDA #$0180         [M=16-bit, X=16-bit] */
    ctx->a = 0x0180u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0C9: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C0CB: A9 05 01    | LDA #$0105         [M=16-bit, X=16-bit] */
    ctx->a = 0x0105u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0CE: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
    /* $1D:C0D0: A9 20 00    | LDA #$0020         [M=16-bit, X=16-bit] */
    ctx->a = 0x0020u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0D3: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:C0D5: A9 03 00    | LDA #$0003         [M=16-bit, X=16-bit] */
    ctx->a = 0x0003u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0D8: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
L_c0db: /* [M=16-bit, X=16-bit] */
    /* $1D:C0DB: D6 36       | DEC $36,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(v16)); }
    /* $1D:C0DD: 20 66 CB    | JSR $CB66          [M=16-bit, X=16-bit] */
    bank1d_func_cb66(ctx);
    /* $1D:C0E0: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c0e1: /* [M=16-bit, X=16-bit] */
    /* $1D:C0E1: C0 02 00    | CPY #$0002         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->y, 0x0002u);
    /* $1D:C0E4: F0 24       | BEQ $C10A          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c10a;
    /* $1D:C0E6: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:C0E8: 10 1C       | BPL $C106          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c106;
    /* $1D:C0EA: F6 3A       | INC $3A,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Au))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(v16)); }
    /* $1D:C0EC: A9 60 00    | LDA #$0060         [M=16-bit, X=16-bit] */
    ctx->a = 0x0060u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0EF: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C0F1: AC 38 16    | LDY $1638          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1638u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C0F4: B9 1B FF    | LDA $FF1B,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF1Bu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C0F7: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C0F9: A0 59 FF    | LDY #$FF59         [M=16-bit, X=16-bit] */
    ctx->y = 0xFF59u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C0FC: 20 1C CC    | JSR $CC1C          [M=16-bit, X=16-bit] */
    bank1d_func_cc1c(ctx);
    /* $1D:C0FF: A9 59 00    | LDA #$0059         [M=16-bit, X=16-bit] */
    ctx->a = 0x0059u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C102: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
L_c106: /* [M=16-bit, X=16-bit] */
    /* $1D:C106: 20 66 CB    | JSR $CB66          [M=16-bit, X=16-bit] */
    bank1d_func_cb66(ctx);
    /* $1D:C109: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c10a: /* [M=16-bit, X=16-bit] */
    /* $1D:C10A: C9 4B 01    | CMP #$014B         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x014Bu);
    /* $1D:C10D: 90 0A       | BCC $C119          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c119;
    /* $1D:C10F: AC 38 16    | LDY $1638          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1638u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C112: B9 1F FF    | LDA $FF1F,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF1Fu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C115: 95 12       | STA $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $1D:C117: 64 46       | STZ $46            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x46u), (uint16_t)(0u));
L_c119: /* [M=16-bit, X=16-bit] */
    /* $1D:C119: F6 36       | INC $36,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(v16)); }
    /* $1D:C11B: 20 66 CB    | JSR $CB66          [M=16-bit, X=16-bit] */
    bank1d_func_cb66(ctx);
    /* $1D:C11E: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C121: 8D 60 16    | STA $1660          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1660u), (uint16_t)(ctx->a));
    /* $1D:C124: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c125  (SNES $1D:C125 | Ghidra 0x1dc125)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c125(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C125: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C128: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:C12B: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:C12D: 10 0E       | BPL $C13D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c13d;
    /* $1D:C12F: A9 06 00    | LDA #$0006         [M=16-bit, X=16-bit] */
    ctx->a = 0x0006u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C132: 95 12       | STA $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $1D:C134: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C135: A2 30 FF    | LDX #$FF30         [M=16-bit, X=16-bit] */
    ctx->x = 0xFF30u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C138: 22 A8 90 00 | JSL $0090A8        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_90a8(ctx); ctx->pb = saved_pb; }
    /* $1D:C13C: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_c13d: /* [M=16-bit, X=16-bit] */
    /* $1D:C13D: 22 92 FD 00 | JSL $00FD92        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_fd92(ctx); ctx->pb = saved_pb; }
    /* $1D:C141: 5C 3E D8 01 | JML $01D83E        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_d83e(ctx); ctx->pb = saved_pb; }
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c145  (SNES $1D:C145 | Ghidra 0x1dc145)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c145(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C145: A9 00 02    | LDA #$0200         [M=16-bit, X=16-bit] */
    ctx->a = 0x0200u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C148: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:C14A: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* Fallthrough into function $1D:C14C */
    bank1d_func_c14c(ctx);
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c14c  (SNES $1D:C14C | Ghidra 0x1dc14c)
 * Discovery Provenance: JMP ($0000) table $1D:BEFD @ $1D:BEFA
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c14c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C14C: A9 8A 91    | LDA #$918A         [M=16-bit, X=16-bit] */
    ctx->a = 0x918Au;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C14F: 85 9A       | STA $9A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x9Au), (uint16_t)(ctx->a));
    /* $1D:C151: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C154: 85 98       | STA $98            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x98u), (uint16_t)(ctx->a));
    /* $1D:C156: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:C158: 10 1B       | BPL $C175          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c175;
    /* $1D:C15A: A9 95 00    | LDA #$0095         [M=16-bit, X=16-bit] */
    ctx->a = 0x0095u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C15D: 95 10       | STA $10,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x10u), (uint16_t)(ctx->a));
    /* $1D:C15F: 74 12       | STZ $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(0u));
    /* $1D:C161: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C162: AD 80 13    | LDA $1380          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1380u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C165: 29 00 01    | AND #$0100         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C168: 49 00 00    | EOR #$0000         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C16B: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $1D:C16C: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:C16D: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:C16E: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C16F: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $1D:C172: 8D 8C 1E    | STA $1E8C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1E8Cu), (uint16_t)(ctx->a));
L_c175: /* [M=16-bit, X=16-bit] */
    /* $1D:C175: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C177: C9 80 01    | CMP #$0180         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0180u);
    /* $1D:C17A: D0 06       | BNE $C182          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c182;
    /* $1D:C17C: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
    /* $1D:C17F: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
L_c182: /* [M=16-bit, X=16-bit] */
    /* $1D:C182: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C184: C9 80 01    | CMP #$0180         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0180u);
    /* $1D:C187: B0 04       | BCS $C18D          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c18d;
    /* $1D:C189: 22 8D FD 00 | JSL $00FD8D        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_fd8d(ctx); ctx->pb = saved_pb; }
L_c18d: /* [M=16-bit, X=16-bit] */
    /* $1D:C18D: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C190: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:C193: A9 FF 00    | LDA #$00FF         [M=16-bit, X=16-bit] */
    ctx->a = 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C196: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C198: 64 00       | STZ $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(0u));
    /* $1D:C19A: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $1D:C19C: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $1D:C19E: AC 42 16    | LDY $1642          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C1A1: B9 A1 FF    | LDA $FFA1,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFFA1u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1A4: 85 FC       | STA $FC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(ctx->a));
    /* $1D:C1A6: B9 A5 FF    | LDA $FFA5,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFFA5u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1A9: A0 C0 02    | LDY #$02C0         [M=16-bit, X=16-bit] */
    ctx->y = 0x02C0u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C1AC: 22 3C CF 01 | JSL $01CF3C        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf3c(ctx); ctx->pb = saved_pb; }
    /* $1D:C1B0: B0 05       | BCS $C1B7          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c1b7;
    /* $1D:C1B2: A5 FC       | LDA $FC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1B4: 99 1E 00    | STA $001E,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x001Eu), (uint16_t)(ctx->a));
L_c1b7: /* [M=16-bit, X=16-bit] */
    /* $1D:C1B7: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c1b8  (SNES $1D:C1B8 | Ghidra 0x1dc1b8)
 * Discovery Provenance: JMP ($0000) table $1D:C04B @ $1D:C048
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c1b8(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C1B8: 20 7A CB    | JSR $CB7A          [M=16-bit, X=16-bit] */
    bank1d_func_cb7a(ctx);
    /* $1D:C1BB: A9 70 00    | LDA #$0070         [M=16-bit, X=16-bit] */
    ctx->a = 0x0070u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1BE: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C1C0: A9 A0 00    | LDA #$00A0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00A0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1C3: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C1C5: 20 D1 CB    | JSR $CBD1          [M=16-bit, X=16-bit] */
    bank1d_func_cbd1(ctx);
    /* $1D:C1C8: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1CB: 10 03       | BPL $C1D0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c1d0;
    /* $1D:C1CD: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:C1CF: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c1d0: /* [M=16-bit, X=16-bit] */
    /* $1D:C1D0: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1D2: 89 1F 00    | BIT #$001F         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x001Fu)) == 0u ? 1u : 0u);
    /* $1D:C1D5: D0 0E       | BNE $C1E5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c1e5;
    /* $1D:C1D7: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1D9: 89 01 00    | BIT #$0001         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0001u)) == 0u ? 1u : 0u);
    /* $1D:C1DC: D0 07       | BNE $C1E5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c1e5;
    /* $1D:C1DE: B5 36       | LDA $36,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1E0: 49 02 00    | EOR #$0002         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1E3: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
L_c1e5: /* [M=16-bit, X=16-bit] */
    /* $1D:C1E5: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1E8: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C1EA: A9 FF FF    | LDA #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1ED: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C1EF: 20 04 CB    | JSR $CB04          [M=16-bit, X=16-bit] */
    bank1d_func_cb04(ctx);
    /* $1D:C1F2: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1F4: 89 3F 00    | BIT #$003F         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x003Fu)) == 0u ? 1u : 0u);
    /* $1D:C1F7: D0 0F       | BNE $C208          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c208;
    /* $1D:C1F9: A5 EA       | LDA $EA            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xEAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C1FB: 89 01 00    | BIT #$0001         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0001u)) == 0u ? 1u : 0u);
    /* $1D:C1FE: D0 08       | BNE $C208          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c208;
    /* $1D:C200: B5 38       | LDA $38,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C202: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C205: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C206: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
L_c208: /* [M=16-bit, X=16-bit] */
    /* $1D:C208: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C20B: C9 24 00    | CMP #$0024         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0024u);
    /* $1D:C20E: 90 07       | BCC $C217          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c217;
    /* $1D:C210: A9 FF FF    | LDA #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C213: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:C215: 80 0A       | BRA $C221          [M=16-bit, X=16-bit] */
    goto L_c221;
L_c217: /* [M=16-bit, X=16-bit] */
    /* $1D:C217: C9 14 00    | CMP #$0014         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0014u);
    /* $1D:C21A: B0 05       | BCS $C221          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c221;
    /* $1D:C21C: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C21F: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
L_c221: /* [M=16-bit, X=16-bit] */
    /* $1D:C221: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C222: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C225: 75 38       | ADC $38,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u)));
    /* $1D:C227: 8D 3C 16    | STA $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(ctx->a));
    /* $1D:C22A: AD 3E 16    | LDA $163E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C22D: C9 E8 FF    | CMP #$FFE8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFE8u);
    /* $1D:C230: 90 0A       | BCC $C23C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c23c;
    /* $1D:C232: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C233: AD 3E 16    | LDA $163E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C236: E9 04 00    | SBC #$0004         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0004u);
    /* $1D:C239: 8D 3E 16    | STA $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->a));
L_c23c: /* [M=16-bit, X=16-bit] */
    /* $1D:C23C: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c23d  (SNES $1D:C23D | Ghidra 0x1dc23d)
 * Discovery Provenance: JMP ($0000) table $1D:C04B @ $1D:C048
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c23d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C23D: AD 42 16    | LDA $1642          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C240: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C241: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:C242: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C243: BF 4D C2 1D | LDA $1DC24D,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x1DC24Du));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C247: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C248: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C24A: 6C 00 00    | JMP ($0000)        [M=16-bit, X=16-bit] */
    {
        uint16_t jmp_tgt = snes_bus_read16(ctx, 0x000000u);
        switch (jmp_tgt) {
            case 0xC259u: bank1d_func_c259(ctx); return;
            case 0xC282u: bank1d_func_c282(ctx); return;
            case 0xC28Fu: bank1d_func_c28f(ctx); return;
            case 0xC29Du: bank1d_func_c29d(ctx); return;
            case 0xC2AAu: bank1d_func_c2aa(ctx); return;
            default:
                // REQUIRES INTERPRETER FALLBACK
                snes_interp_fallback_at(ctx, 0x1DC24Au, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);
                return;
        }
    }
}

/* ==========================================================================
 * Function: bank1d_func_c259  (SNES $1D:C259 | Ghidra 0x1dc259)
 * Discovery Provenance: JMP ($0000) table $1D:C24D @ $1D:C24A
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c259(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C259: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C25B: C9 BC 00    | CMP #$00BC         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00BCu);
    /* $1D:C25E: 90 21       | BCC $C281          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c281;
    /* $1D:C260: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:C262: A9 03 00    | LDA #$0003         [M=16-bit, X=16-bit] */
    ctx->a = 0x0003u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C265: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:C268: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
    /* $1D:C26B: AD 42 16    | LDA $1642          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C26E: C9 05 00    | CMP #$0005         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0005u);
    /* $1D:C271: F0 0E       | BEQ $C281          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c281;
    /* $1D:C273: A9 50 00    | LDA #$0050         [M=16-bit, X=16-bit] */
    ctx->a = 0x0050u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C276: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:C278: A9 8C 00    | LDA #$008C         [M=16-bit, X=16-bit] */
    ctx->a = 0x008Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C27B: A0 40 03    | LDY #$0340         [M=16-bit, X=16-bit] */
    ctx->y = 0x0340u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C27E: 20 52 CA    | JSR $CA52          [M=16-bit, X=16-bit] */
    bank1d_func_ca52(ctx);
L_c281: /* [M=16-bit, X=16-bit] */
    /* $1D:C281: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c282  (SNES $1D:C282 | Ghidra 0x1dc282)
 * Discovery Provenance: JMP ($0000) table $1D:C24D @ $1D:C24A
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c282(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C282: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:C284: 10 08       | BPL $C28E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c28e;
    /* $1D:C286: A9 FE FF    | LDA #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C289: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:C28B: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
L_c28e: /* [M=16-bit, X=16-bit] */
    /* $1D:C28E: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c28f  (SNES $1D:C28F | Ghidra 0x1dc28f)
 * Discovery Provenance: JMP ($0000) table $1D:C24D @ $1D:C24A
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c28f(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C28F: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C292: 10 08       | BPL $C29C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c29c;
    /* $1D:C294: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C297: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:C299: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
L_c29c: /* [M=16-bit, X=16-bit] */
    /* $1D:C29C: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c29d  (SNES $1D:C29D | Ghidra 0x1dc29d)
 * Discovery Provenance: JMP ($0000) table $1D:C24D @ $1D:C24A
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c29d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C29D: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C29F: C9 70 00    | CMP #$0070         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0070u);
    /* $1D:C2A2: B0 05       | BCS $C2A9          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c2a9;
    /* $1D:C2A4: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:C2A6: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
L_c2a9: /* [M=16-bit, X=16-bit] */
    /* $1D:C2A9: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c2aa  (SNES $1D:C2AA | Ghidra 0x1dc2aa)
 * Discovery Provenance: JMP ($0000) table $1D:C24D @ $1D:C24A
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c2aa(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C2AA: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c2ab  (SNES $1D:C2AB | Ghidra 0x1dc2ab)
 * Discovery Provenance: JMP ($0000) table $1D:C04B @ $1D:C048
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c2ab(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C2AB: AD 42 16    | LDA $1642          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2AE: D0 10       | BNE $C2C0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c2c0;
    /* $1D:C2B0: AC 84 00    | LDY $0084          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0084u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C2B3: B9 61 FF    | LDA $FF61,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF61u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2B6: D5 0E       | CMP $0E,X          [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu)));
    /* $1D:C2B8: 90 05       | BCC $C2BF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c2bf;
    /* $1D:C2BA: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
    /* $1D:C2BD: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
L_c2bf: /* [M=16-bit, X=16-bit] */
    /* $1D:C2BF: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c2c0: /* [M=16-bit, X=16-bit] */
    /* $1D:C2C0: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2C3: 8D 60 16    | STA $1660          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1660u), (uint16_t)(ctx->a));
    /* $1D:C2C6: 20 7A CB    | JSR $CB7A          [M=16-bit, X=16-bit] */
    bank1d_func_cb7a(ctx);
    /* $1D:C2C9: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2CC: 10 03       | BPL $C2D1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c2d1;
    /* $1D:C2CE: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:C2D0: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c2d1: /* [M=16-bit, X=16-bit] */
    /* $1D:C2D1: A5 C0       | LDA $C0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC0u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2D3: D0 45       | BNE $C31A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c31a;
    /* $1D:C2D5: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2D7: 29 01 00    | AND #$0001         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2DA: D0 14       | BNE $C2F0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c2f0;
    /* $1D:C2DC: A9 8E 00    | LDA #$008E         [M=16-bit, X=16-bit] */
    ctx->a = 0x008Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2DF: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C2E0: 20 73 CA    | JSR $CA73          [M=16-bit, X=16-bit] */
    bank1d_func_ca73(ctx);
    /* $1D:C2E3: B0 35       | BCS $C31A          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c31a;
    /* $1D:C2E5: A9 00 34    | LDA #$3400         [M=16-bit, X=16-bit] */
    ctx->a = 0x3400u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2E8: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C2EA: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C2EB: 20 30 CC    | JSR $CC30          [M=16-bit, X=16-bit] */
    bank1d_func_cc30(ctx);
    /* $1D:C2EE: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C2EF: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c2f0: /* [M=16-bit, X=16-bit] */
    /* $1D:C2F0: A0 00 08    | LDY #$0800         [M=16-bit, X=16-bit] */
    ctx->y = 0x0800u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C2F3: 22 C1 9E 01 | JSL $019EC1        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_9ec1(ctx); ctx->pb = saved_pb; }
    /* $1D:C2F7: 90 21       | BCC $C31A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c31a;
    /* $1D:C2F9: A9 0F 00    | LDA #$000F         [M=16-bit, X=16-bit] */
    ctx->a = 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C2FC: 99 10 00    | STA $0010,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0010u), (uint16_t)(ctx->a));
    /* $1D:C2FF: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C302: 99 3E 00    | STA $003E,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x003Eu), (uint16_t)(ctx->a));
    /* $1D:C305: 20 30 CC    | JSR $CC30          [M=16-bit, X=16-bit] */
    bank1d_func_cc30(ctx);
    /* $1D:C308: A5 EA       | LDA $EA            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xEAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C30A: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $1D:C30B: 29 07 00    | AND #$0007         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0007u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C30E: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C30F: C9 06 00    | CMP #$0006         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0006u);
    /* $1D:C312: 90 03       | BCC $C317          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c317;
    /* $1D:C314: A9 07 00    | LDA #$0007         [M=16-bit, X=16-bit] */
    ctx->a = 0x0007u;
    snes_set_nz16(ctx, ctx->a);
L_c317: /* [M=16-bit, X=16-bit] */
    /* $1D:C317: 99 3A 00    | STA $003A,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x003Au), (uint16_t)(ctx->a));
L_c31a: /* [M=16-bit, X=16-bit] */
    /* $1D:C31A: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c31b  (SNES $1D:C31B | Ghidra 0x1dc31b)
 * Discovery Provenance: JMP ($0000) table $1D:C04B @ $1D:C048
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c31b(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C31B: 20 7A CB    | JSR $CB7A          [M=16-bit, X=16-bit] */
    bank1d_func_cb7a(ctx);
    /* $1D:C31E: A9 38 00    | LDA #$0038         [M=16-bit, X=16-bit] */
    ctx->a = 0x0038u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C321: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C323: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C326: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C328: 20 D1 CB    | JSR $CBD1          [M=16-bit, X=16-bit] */
    bank1d_func_cbd1(ctx);
    /* $1D:C32B: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C32E: C9 F0 00    | CMP #$00F0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00F0u);
    /* $1D:C331: B0 1B       | BCS $C34E          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c34e;
    /* $1D:C333: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C335: 89 7F 00    | BIT #$007F         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x007Fu)) == 0u ? 1u : 0u);
    /* $1D:C338: D0 07       | BNE $C341          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c341;
    /* $1D:C33A: B5 36       | LDA $36,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C33C: 49 02 00    | EOR #$0002         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C33F: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
L_c341: /* [M=16-bit, X=16-bit] */
    /* $1D:C341: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C344: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C346: A9 FC FF    | LDA #$FFFC         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFCu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C349: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C34B: 20 04 CB    | JSR $CB04          [M=16-bit, X=16-bit] */
    bank1d_func_cb04(ctx);
L_c34e: /* [M=16-bit, X=16-bit] */
    /* $1D:C34E: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C350: D0 07       | BNE $C359          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c359;
    /* $1D:C352: CE 3C 16    | DEC $163C          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(v16)); }
    /* $1D:C355: 10 02       | BPL $C359          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c359;
    /* $1D:C357: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_c359: /* [M=16-bit, X=16-bit] */
    /* $1D:C359: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c35a  (SNES $1D:C35A | Ghidra 0x1dc35a)
 * Discovery Provenance: JMP ($0000) table $1D:C04B @ $1D:C048
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c35a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C35A: 20 7A CB    | JSR $CB7A          [M=16-bit, X=16-bit] */
    bank1d_func_cb7a(ctx);
    /* $1D:C35D: A9 78 00    | LDA #$0078         [M=16-bit, X=16-bit] */
    ctx->a = 0x0078u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C360: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C362: A9 80 00    | LDA #$0080         [M=16-bit, X=16-bit] */
    ctx->a = 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C365: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C367: 20 D1 CB    | JSR $CBD1          [M=16-bit, X=16-bit] */
    bank1d_func_cbd1(ctx);
    /* $1D:C36A: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C36D: 10 02       | BPL $C371          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c371;
    /* $1D:C36F: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_c371: /* [M=16-bit, X=16-bit] */
    /* $1D:C371: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c372  (SNES $1D:C372 | Ghidra 0x1dc372)
 * Discovery Provenance: JMP ($0000) table $1D:C04B @ $1D:C048
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c372(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C372: AD 42 16    | LDA $1642          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C375: D0 26       | BNE $C39D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c39d;
    /* $1D:C377: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C378: B5 1C       | LDA $1C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C37A: 69 80 14    | ADC #$1480         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x1480u);
    /* $1D:C37D: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:C37F: B5 1E       | LDA $1E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C381: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:C384: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:C386: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C388: C9 C0 00    | CMP #$00C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00C0u);
    /* $1D:C38B: 90 0F       | BCC $C39C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c39c;
    /* $1D:C38D: 74 1A       | STZ $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(0u));
    /* $1D:C38F: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:C391: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:C393: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C396: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:C399: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
L_c39c: /* [M=16-bit, X=16-bit] */
    /* $1D:C39C: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c39d: /* [M=16-bit, X=16-bit] */
    /* $1D:C39D: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3A0: 8D 60 16    | STA $1660          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1660u), (uint16_t)(ctx->a));
    /* $1D:C3A3: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3A5: 89 7F 00    | BIT #$007F         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x007Fu)) == 0u ? 1u : 0u);
    /* $1D:C3A8: D0 4A       | BNE $C3F4          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c3f4;
    /* $1D:C3AA: A9 91 00    | LDA #$0091         [M=16-bit, X=16-bit] */
    ctx->a = 0x0091u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3AD: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C3AE: 20 73 CA    | JSR $CA73          [M=16-bit, X=16-bit] */
    bank1d_func_ca73(ctx);
    /* $1D:C3B1: B0 40       | BCS $C3F3          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c3f3;
    /* $1D:C3B3: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3B6: 95 3A       | STA $3A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(ctx->a));
    /* $1D:C3B8: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3BB: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:C3BD: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3C0: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:C3C2: A9 B0 00    | LDA #$00B0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00B0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3C5: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C3C7: 86 FC       | STX $FC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(ctx->x));
    /* $1D:C3C9: 64 FE       | STZ $FE            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFEu), (uint16_t)(0u));
    /* $1D:C3CB: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3CD: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:C3CE: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:C3CF: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
L_c3d1: /* [M=16-bit, X=16-bit] */
    /* $1D:C3D1: A9 91 00    | LDA #$0091         [M=16-bit, X=16-bit] */
    ctx->a = 0x0091u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3D4: 20 73 CA    | JSR $CA73          [M=16-bit, X=16-bit] */
    bank1d_func_ca73(ctx);
    /* $1D:C3D7: B0 14       | BCS $C3ED          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c3ed;
    /* $1D:C3D9: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C3DA: A5 FE       | LDA $FE            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFEu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3DC: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:C3DF: 85 FE       | STA $FE            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFEu), (uint16_t)(ctx->a));
    /* $1D:C3E1: A5 FC       | LDA $FC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3E3: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:C3E5: A5 FE       | LDA $FE            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFEu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3E7: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
    /* $1D:C3E9: C6 FA       | DEC $FA            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(v16)); }
    /* $1D:C3EB: 10 E4       | BPL $C3D1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c3d1;
L_c3ed: /* [M=16-bit, X=16-bit] */
    /* $1D:C3ED: A6 FC       | LDX $FC            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C3EF: A5 FE       | LDA $FE            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFEu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3F1: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
L_c3f3: /* [M=16-bit, X=16-bit] */
    /* $1D:C3F3: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_c3f4: /* [M=16-bit, X=16-bit] */
    /* $1D:C3F4: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3F7: 10 02       | BPL $C3FB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c3fb;
    /* $1D:C3F9: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_c3fb: /* [M=16-bit, X=16-bit] */
    /* $1D:C3FB: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c3fc  (SNES $1D:C3FC | Ghidra 0x1dc3fc)
 * Discovery Provenance: JMP ($0000) table $1D:C04B @ $1D:C048
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c3fc(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C3FC: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C3FF: 10 03       | BPL $C404          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c404;
    /* $1D:C401: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:C403: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c404: /* [M=16-bit, X=16-bit] */
    /* $1D:C404: B4 38       | LDY $38,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C406: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C409: 8D 60 16    | STA $1660          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1660u), (uint16_t)(ctx->a));
    /* $1D:C40C: AD 42 16    | LDA $1642          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C40F: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C410: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:C411: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C412: BF 1C C4 1D | LDA $1DC41C,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x1DC41Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C416: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C417: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C419: 6C 00 00    | JMP ($0000)        [M=16-bit, X=16-bit] */
    {
        uint16_t jmp_tgt = snes_bus_read16(ctx, 0x000000u);
        switch (jmp_tgt) {
            case 0xC42Eu: bank1d_func_c42e(ctx); return;
            case 0xC44Cu: bank1d_func_c44c(ctx); return;
            case 0xC481u: bank1d_func_c481(ctx); return;
            case 0xC487u: bank1d_func_c487(ctx); return;
            case 0xC492u: bank1d_func_c492(ctx); return;
            case 0xC498u: bank1d_func_c498(ctx); return;
            case 0xC4F9u: bank1d_func_c4f9(ctx); return;
            default:
                // REQUIRES INTERPRETER FALLBACK
                snes_interp_fallback_at(ctx, 0x1DC419u, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);
                return;
        }
    }
}

/* ==========================================================================
 * Function: bank1d_func_c42e  (SNES $1D:C42E | Ghidra 0x1dc42e)
 * Discovery Provenance: JMP ($0000) table $1D:C41C @ $1D:C419
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c42e(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C42E: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C430: C9 7C 00    | CMP #$007C         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x007Cu);
    /* $1D:C433: B0 02       | BCS $C437          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c437;
    /* $1D:C435: F6 0E       | INC $0E,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(v16)); }
L_c437: /* [M=16-bit, X=16-bit] */
    /* $1D:C437: EE 3C 16    | INC $163C          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(v16)); }
    /* $1D:C43A: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C43D: C9 14 00    | CMP #$0014         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0014u);
    /* $1D:C440: 90 09       | BCC $C44B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c44b;
    /* $1D:C442: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
    /* $1D:C445: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C448: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
L_c44b: /* [M=16-bit, X=16-bit] */
    /* $1D:C44B: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c44c  (SNES $1D:C44C | Ghidra 0x1dc44c)
 * Discovery Provenance: JMP ($0000) table $1D:C41C @ $1D:C419
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c44c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C44C: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C44E: C9 40 00    | CMP #$0040         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0040u);
    /* $1D:C451: B0 05       | BCS $C458          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c458;
    /* $1D:C453: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C456: 80 0F       | BRA $C467          [M=16-bit, X=16-bit] */
    goto L_c467;
L_c458: /* [M=16-bit, X=16-bit] */
    /* $1D:C458: C9 90 00    | CMP #$0090         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0090u);
    /* $1D:C45B: 90 05       | BCC $C462          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c462;
    /* $1D:C45D: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C460: 80 05       | BRA $C467          [M=16-bit, X=16-bit] */
    goto L_c467;
L_c462: /* [M=16-bit, X=16-bit] */
    /* $1D:C462: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C464: 29 02 00    | AND #$0002         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
L_c467: /* [M=16-bit, X=16-bit] */
    /* $1D:C467: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:C469: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C46A: B9 87 FF    | LDA $FF87,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF87u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C46D: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C46E: B9 0A 00    | LDA $000A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C471: 99 36 00    | STA $0036,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0036u), (uint16_t)(ctx->a));
    /* $1D:C474: B9 0E 00    | LDA $000E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C477: 99 38 00    | STA $0038,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0038u), (uint16_t)(ctx->a));
    /* $1D:C47A: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C47D: 8D 42 16    | STA $1642          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(ctx->a));
    /* $1D:C480: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c481  (SNES $1D:C481 | Ghidra 0x1dc481)
 * Discovery Provenance: JMP ($0000) table $1D:C41C @ $1D:C419
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c481(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C481: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C482: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C483: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $1D:C486: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* Fallthrough into function $1D:C487 */
    bank1d_func_c487(ctx);
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c487  (SNES $1D:C487 | Ghidra 0x1dc487)
 * Discovery Provenance: JMP ($0000) table $1D:C41C @ $1D:C419
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c487(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C487: B5 3E       | LDA $3E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C489: C9 D0 00    | CMP #$00D0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00D0u);
    /* $1D:C48C: B0 02       | BCS $C490          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c490;
    /* $1D:C48E: F6 3E       | INC $3E,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Eu))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(v16)); }
L_c490: /* [M=16-bit, X=16-bit] */
    /* $1D:C490: 80 0F       | BRA $C4A1          [M=16-bit, X=16-bit] */
    goto L_c4a1;
L_c4a1: /* [M=16-bit, X=16-bit] */
    /* $1D:C4A1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C4A2: AD 3E 16    | LDA $163E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4A5: 79 93 FF    | ADC $FF93,Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF93u)));
    /* $1D:C4A8: 8D 3E 16    | STA $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->a));
    /* $1D:C4AB: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:C4AE: D9 8B FF    | CMP $FF8B,Y        [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF8Bu)));
    /* $1D:C4B1: D0 27       | BNE $C4DA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c4da;
    /* $1D:C4B3: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
    /* $1D:C4B6: AD 42 16    | LDA $1642          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4B9: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:C4BC: F0 05       | BEQ $C4C3          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c4c3;
    /* $1D:C4BE: C9 07 00    | CMP #$0007         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0007u);
    /* $1D:C4C1: D0 17       | BNE $C4DA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c4da;
L_c4c3: /* [M=16-bit, X=16-bit] */
    /* $1D:C4C3: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4C5: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4C8: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $1D:C4CB: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:C4CD: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4D0: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:C4D3: A9 39 00    | LDA #$0039         [M=16-bit, X=16-bit] */
    ctx->a = 0x0039u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4D6: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
L_c4da: /* [M=16-bit, X=16-bit] */
    /* $1D:C4DA: B4 38       | LDY $38,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C4DC: B9 87 FF    | LDA $FF87,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF87u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4DF: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C4E0: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C4E1: B9 36 00    | LDA $0036,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0036u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4E4: F9 0A 00    | SBC $000A,Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Au)));
    /* $1D:C4E7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C4E8: 75 0A       | ADC $0A,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au)));
    /* $1D:C4EA: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C4EC: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C4ED: B9 38 00    | LDA $0038,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0038u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4F0: F9 0E 00    | SBC $000E,Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Eu)));
    /* $1D:C4F3: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C4F4: 75 0E       | ADC $0E,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu)));
    /* $1D:C4F6: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C4F8: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c492  (SNES $1D:C492 | Ghidra 0x1dc492)
 * Discovery Provenance: JMP ($0000) table $1D:C41C @ $1D:C419
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c492(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C492: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C493: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C494: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $1D:C497: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* Fallthrough into function $1D:C498 */
    bank1d_func_c498(ctx);
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c498  (SNES $1D:C498 | Ghidra 0x1dc498)
 * Discovery Provenance: JMP ($0000) table $1D:C41C @ $1D:C419
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c498(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C498: B5 3E       | LDA $3E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C49A: C9 C0 00    | CMP #$00C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00C0u);
    /* $1D:C49D: 90 02       | BCC $C4A1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c4a1;
    /* $1D:C49F: D6 3E       | DEC $3E,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Eu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(v16)); }
L_c4a1: /* [M=16-bit, X=16-bit] */
    /* $1D:C4A1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C4A2: AD 3E 16    | LDA $163E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4A5: 79 93 FF    | ADC $FF93,Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF93u)));
    /* $1D:C4A8: 8D 3E 16    | STA $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->a));
    /* $1D:C4AB: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:C4AE: D9 8B FF    | CMP $FF8B,Y        [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF8Bu)));
    /* $1D:C4B1: D0 27       | BNE $C4DA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c4da;
    /* $1D:C4B3: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
    /* $1D:C4B6: AD 42 16    | LDA $1642          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4B9: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:C4BC: F0 05       | BEQ $C4C3          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c4c3;
    /* $1D:C4BE: C9 07 00    | CMP #$0007         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0007u);
    /* $1D:C4C1: D0 17       | BNE $C4DA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c4da;
L_c4c3: /* [M=16-bit, X=16-bit] */
    /* $1D:C4C3: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4C5: 29 0F 00    | AND #$000F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4C8: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $1D:C4CB: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:C4CD: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4D0: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:C4D3: A9 39 00    | LDA #$0039         [M=16-bit, X=16-bit] */
    ctx->a = 0x0039u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4D6: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
L_c4da: /* [M=16-bit, X=16-bit] */
    /* $1D:C4DA: B4 38       | LDY $38,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C4DC: B9 87 FF    | LDA $FF87,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF87u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4DF: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C4E0: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C4E1: B9 36 00    | LDA $0036,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0036u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4E4: F9 0A 00    | SBC $000A,Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Au)));
    /* $1D:C4E7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C4E8: 75 0A       | ADC $0A,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au)));
    /* $1D:C4EA: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C4EC: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C4ED: B9 38 00    | LDA $0038,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0038u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4F0: F9 0E 00    | SBC $000E,Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Eu)));
    /* $1D:C4F3: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C4F4: 75 0E       | ADC $0E,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu)));
    /* $1D:C4F6: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C4F8: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c4f9  (SNES $1D:C4F9 | Ghidra 0x1dc4f9)
 * Discovery Provenance: JMP ($0000) table $1D:C41C @ $1D:C419
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c4f9(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C4F9: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:C4FB: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C4FD: 10 1B       | BPL $C51A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c51a;
    /* $1D:C4FF: B5 38       | LDA $38,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C501: 49 02 00    | EOR #$0002         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C504: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:C506: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C507: B9 87 FF    | LDA $FF87,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF87u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C50A: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C50B: B9 0A 00    | LDA $000A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C50E: 99 36 00    | STA $0036,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0036u), (uint16_t)(ctx->a));
    /* $1D:C511: A9 B0 00    | LDA #$00B0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00B0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C514: 99 38 00    | STA $0038,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0038u), (uint16_t)(ctx->a));
    /* $1D:C517: EE 42 16    | INC $1642          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1642u), (uint16_t)(v16)); }
L_c51a: /* [M=16-bit, X=16-bit] */
    /* $1D:C51A: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c51b  (SNES $1D:C51B | Ghidra 0x1dc51b)
 * Discovery Provenance: JMP ($0000) table $1D:C04B @ $1D:C048
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c51b(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C51B: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C51D: 89 3F 00    | BIT #$003F         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x003Fu)) == 0u ? 1u : 0u);
    /* $1D:C520: D0 22       | BNE $C544          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c544;
    /* $1D:C522: A9 93 00    | LDA #$0093         [M=16-bit, X=16-bit] */
    ctx->a = 0x0093u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C525: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C526: 20 73 CA    | JSR $CA73          [M=16-bit, X=16-bit] */
    bank1d_func_ca73(ctx);
    /* $1D:C529: B0 19       | BCS $C544          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c544;
    /* $1D:C52B: A9 03 00    | LDA #$0003         [M=16-bit, X=16-bit] */
    ctx->a = 0x0003u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C52E: A4 E8       | LDY $E8            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C530: 10 03       | BPL $C535          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c535;
    /* $1D:C532: A9 FD FF    | LDA #$FFFD         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFDu;
    snes_set_nz16(ctx, ctx->a);
L_c535: /* [M=16-bit, X=16-bit] */
    /* $1D:C535: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:C537: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C53A: A4 EA       | LDY $EA            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xEAu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C53C: 10 03       | BPL $C541          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c541;
    /* $1D:C53E: A9 FF FF    | LDA #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
L_c541: /* [M=16-bit, X=16-bit] */
    /* $1D:C541: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:C543: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_c544: /* [M=16-bit, X=16-bit] */
    /* $1D:C544: A9 60 00    | LDA #$0060         [M=16-bit, X=16-bit] */
    ctx->a = 0x0060u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C547: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C549: A9 80 00    | LDA #$0080         [M=16-bit, X=16-bit] */
    ctx->a = 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C54C: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C54E: 20 D1 CB    | JSR $CBD1          [M=16-bit, X=16-bit] */
    bank1d_func_cbd1(ctx);
    /* $1D:C551: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C554: 10 02       | BPL $C558          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c558;
    /* $1D:C556: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_c558: /* [M=16-bit, X=16-bit] */
    /* $1D:C558: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c559  (SNES $1D:C559 | Ghidra 0x1dc559)
 * Discovery Provenance: JMP ($0000) table $1D:C02B @ $1D:C028
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c559(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C559: A9 8B 00    | LDA #$008B         [M=16-bit, X=16-bit] */
    ctx->a = 0x008Bu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C55C: A0 00 07    | LDY #$0700         [M=16-bit, X=16-bit] */
    ctx->y = 0x0700u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C55F: 20 52 CA    | JSR $CA52          [M=16-bit, X=16-bit] */
    bank1d_func_ca52(ctx);
    /* $1D:C562: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C565: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:C567: 74 36       | STZ $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(0u));
    /* $1D:C569: 74 32       | STZ $32,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x32u), (uint16_t)(0u));
    /* $1D:C56B: 74 34       | STZ $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(0u));
    /* $1D:C56D: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C570: 8D 3C 16    | STA $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(ctx->a));
    /* $1D:C573: A9 FC FF    | LDA #$FFFC         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFCu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C576: 8D 3E 16    | STA $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->a));
    /* $1D:C579: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c57a  (SNES $1D:C57A | Ghidra 0x1dc57a)
 * Discovery Provenance: JMP ($0000) table $1D:C02B @ $1D:C028
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c57a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C57A: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C57D: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:C57F: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c580  (SNES $1D:C580 | Ghidra 0x1dc580)
 * Discovery Provenance: JMP ($0000) table $1D:C02B @ $1D:C028
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c580(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C580: A9 FF FF    | LDA #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C583: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:C585: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c586  (SNES $1D:C586 | Ghidra 0x1dc586)
 * Discovery Provenance: JMP ($0000) table $1D:C02B @ $1D:C028
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c586(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C586: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C589: 22 5D A1 01 | JSL $01A15D        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a15d(ctx); ctx->pb = saved_pb; }
    /* $1D:C58D: 8C 3E 16    | STY $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->y));
    /* $1D:C590: A9 8F 00    | LDA #$008F         [M=16-bit, X=16-bit] */
    ctx->a = 0x008Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C593: A0 00 05    | LDY #$0500         [M=16-bit, X=16-bit] */
    ctx->y = 0x0500u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C596: 20 52 CA    | JSR $CA52          [M=16-bit, X=16-bit] */
    bank1d_func_ca52(ctx);
    /* $1D:C599: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C59C: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:C59E: 74 36       | STZ $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(0u));
    /* $1D:C5A0: A9 20 01    | LDA #$0120         [M=16-bit, X=16-bit] */
    ctx->a = 0x0120u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5A3: 8D 3C 16    | STA $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(ctx->a));
    /* $1D:C5A6: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c5a7  (SNES $1D:C5A7 | Ghidra 0x1dc5a7)
 * Discovery Provenance: JMP ($0000) table $1D:C02B @ $1D:C028
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c5a7(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C5A7: A9 90 00    | LDA #$0090         [M=16-bit, X=16-bit] */
    ctx->a = 0x0090u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5AA: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C5AB: 20 73 CA    | JSR $CA73          [M=16-bit, X=16-bit] */
    bank1d_func_ca73(ctx);
    /* $1D:C5AE: A9 00 34    | LDA #$3400         [M=16-bit, X=16-bit] */
    ctx->a = 0x3400u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5B1: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C5B3: 74 16       | STZ $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(0u));
    /* $1D:C5B5: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C5B6: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c5b7  (SNES $1D:C5B7 | Ghidra 0x1dc5b7)
 * Discovery Provenance: JMP ($0000) table $1D:C02B @ $1D:C028
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c5b7(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C5B7: 74 24       | STZ $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(0u));
    /* $1D:C5B9: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C5BA: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5BD: 69 00 01    | ADC #$0100         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0100u);
    /* $1D:C5C0: 8D 44 16    | STA $1644          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1644u), (uint16_t)(ctx->a));
    /* $1D:C5C3: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5C6: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:C5C8: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c5c9  (SNES $1D:C5C9 | Ghidra 0x1dc5c9)
 * Discovery Provenance: JMP ($0000) table $1D:C02B @ $1D:C028
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c5c9(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C5C9: A9 92 00    | LDA #$0092         [M=16-bit, X=16-bit] */
    ctx->a = 0x0092u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5CC: A0 80 05    | LDY #$0580         [M=16-bit, X=16-bit] */
    ctx->y = 0x0580u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C5CF: 20 52 CA    | JSR $CA52          [M=16-bit, X=16-bit] */
    bank1d_func_ca52(ctx);
    /* $1D:C5D2: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5D5: 8D 3A 03    | STA $033A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x033Au), (uint16_t)(ctx->a));
    /* $1D:C5D8: 8D 7A 04    | STA $047A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x047Au), (uint16_t)(ctx->a));
    /* $1D:C5DB: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5DE: 8D 3A 04    | STA $043A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x043Au), (uint16_t)(ctx->a));
    /* $1D:C5E1: 8D 7A 05    | STA $057A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x057Au), (uint16_t)(ctx->a));
    /* $1D:C5E4: A9 80 00    | LDA #$0080         [M=16-bit, X=16-bit] */
    ctx->a = 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5E7: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:C5EA: 8D 3E 16    | STA $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->a));
    /* $1D:C5ED: A9 D0 00    | LDA #$00D0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00D0u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5F0: 95 3E       | STA $3E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(ctx->a));
    /* $1D:C5F2: 9C 3C 16    | STZ $163C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Cu), (uint16_t)(0u));
    /* $1D:C5F5: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C5F8: 69 60 00    | ADC #$0060         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0060u);
    /* $1D:C5FB: 8D 44 16    | STA $1644          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1644u), (uint16_t)(ctx->a));
    /* $1D:C5FE: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c5ff  (SNES $1D:C5FF | Ghidra 0x1dc5ff)
 * Discovery Provenance: JMP ($0000) table $1D:C02B @ $1D:C028
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c5ff(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C5FF: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C601: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:C604: D0 06       | BNE $C60C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c60c;
    /* $1D:C606: A9 00 03    | LDA #$0300         [M=16-bit, X=16-bit] */
    ctx->a = 0x0300u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C609: 8D 44 16    | STA $1644          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1644u), (uint16_t)(ctx->a));
L_c60c: /* [M=16-bit, X=16-bit] */
    /* $1D:C60C: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c60d  (SNES $1D:C60D | Ghidra 0x1dc60d)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c60d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C60D: AD 3E 16    | LDA $163E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C610: C9 02 00    | CMP #$0002         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0002u);
    /* $1D:C613: F0 19       | BEQ $C62E          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c62e;
    /* $1D:C615: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C618: C9 30 00    | CMP #$0030         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0030u);
    /* $1D:C61B: 90 0F       | BCC $C62C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c62c;
    /* $1D:C61D: B5 14       | LDA $14,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C61F: F0 08       | BEQ $C629          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c629;
    /* $1D:C621: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C624: 8D 3E 16    | STA $163E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Eu), (uint16_t)(ctx->a));
    /* $1D:C627: 95 06       | STA $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(ctx->a));
L_c629: /* [M=16-bit, X=16-bit] */
    /* $1D:C629: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
L_c62c: /* [M=16-bit, X=16-bit] */
    /* $1D:C62C: 80 59       | BRA $C687          [M=16-bit, X=16-bit] */
    goto L_c687;
L_c62e: /* [M=16-bit, X=16-bit] */
    /* $1D:C62E: B5 16       | LDA $16,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x16u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C630: C9 30 00    | CMP #$0030         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0030u);
    /* $1D:C633: D0 27       | BNE $C65C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c65c;
    /* $1D:C635: D6 30       | DEC $30,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x30u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x30u), (uint16_t)(v16)); }
    /* $1D:C637: 10 06       | BPL $C63F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c63f;
    /* $1D:C639: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C63C: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:C63E: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c63f: /* [M=16-bit, X=16-bit] */
    /* $1D:C63F: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C641: 89 04 00    | BIT #$0004         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0004u)) == 0u ? 1u : 0u);
    /* $1D:C644: D0 05       | BNE $C64B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c64b;
    /* $1D:C646: B5 3A       | LDA $3A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C648: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C64A: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c64b: /* [M=16-bit, X=16-bit] */
    /* $1D:C64B: 74 00       | STZ $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(0u));
    /* $1D:C64D: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c65c: /* [M=16-bit, X=16-bit] */
    /* $1D:C65C: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C65F: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:C662: 20 C9 CA    | JSR $CAC9          [M=16-bit, X=16-bit] */
    bank1d_func_cac9(ctx);
    /* $1D:C665: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C668: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:C66C: A9 CE C4    | LDA #$C4CE         [M=16-bit, X=16-bit] */
    ctx->a = 0xC4CEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C66F: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C670: 5C 66 CF 01 | JML $01CF66        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf66(ctx); ctx->pb = saved_pb; }
    return;
L_c687: /* [M=16-bit, X=16-bit] */
    /* $1D:C687: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $1D:C688: 20 E9 CA    | JSR $CAE9          [M=16-bit, X=16-bit] */
    bank1d_func_cae9(ctx);
    /* $1D:C68B: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C68C: 22 A4 AC 1D | JSL $1DACA4        [M=16-bit, X=16-bit] */
    bank1d_func_aca4(ctx);
    /* $1D:C690: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C691: AD 0A 09    | LDA $090A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C694: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:C697: 65 08       | ADC $08            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:C699: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C69B: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C69C: AD 0E 09    | LDA $090E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C69F: 69 E4 FF    | ADC #$FFE4         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE4u);
    /* $1D:C6A2: 65 0A       | ADC $0A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $1D:C6A4: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C6A6: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c64e  (SNES $1D:C64E | Ghidra 0x1dc64e)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c64e(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C64E: AD 88 16    | LDA $1688          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1688u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C651: D0 09       | BNE $C65C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c65c;
    /* $1D:C653: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C656: 30 04       | BMI $C65C          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c65c;
    /* $1D:C658: B5 06       | LDA $06,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C65A: 10 18       | BPL $C674          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c674;
L_c65c: /* [M=16-bit, X=16-bit] */
    /* $1D:C65C: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C65F: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
    /* $1D:C662: 20 C9 CA    | JSR $CAC9          [M=16-bit, X=16-bit] */
    bank1d_func_cac9(ctx);
    /* $1D:C665: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C668: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:C66C: A9 CE C4    | LDA #$C4CE         [M=16-bit, X=16-bit] */
    ctx->a = 0xC4CEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C66F: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C670: 5C 66 CF 01 | JML $01CF66        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf66(ctx); ctx->pb = saved_pb; }
    return;
L_c674: /* [M=16-bit, X=16-bit] */
    /* $1D:C674: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C677: E0 00 05    | CPX #$0500         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0500u);
    /* $1D:C67A: 90 0B       | BCC $C687          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c687;
    /* $1D:C67C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C67D: 6D 3E 16    | ADC $163E          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu)));
    /* $1D:C680: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C683: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C684: 69 20 00    | ADC #$0020         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0020u);
L_c687: /* [M=16-bit, X=16-bit] */
    /* $1D:C687: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $1D:C688: 20 E9 CA    | JSR $CAE9          [M=16-bit, X=16-bit] */
    bank1d_func_cae9(ctx);
    /* $1D:C68B: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C68C: 22 A4 AC 1D | JSL $1DACA4        [M=16-bit, X=16-bit] */
    bank1d_func_aca4(ctx);
    /* $1D:C690: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C691: AD 0A 09    | LDA $090A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C694: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:C697: 65 08       | ADC $08            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:C699: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C69B: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C69C: AD 0E 09    | LDA $090E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C69F: 69 E4 FF    | ADC #$FFE4         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE4u);
    /* $1D:C6A2: 65 0A       | ADC $0A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $1D:C6A4: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C6A6: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c6a7  (SNES $1D:C6A7 | Ghidra 0x1dc6a7)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c6a7(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C6A7: AD 88 16    | LDA $1688          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1688u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6AA: F0 03       | BEQ $C6AF          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c6af;
    /* $1D:C6AC: 82 1A 01    | BRL $C7C9          [M=16-bit, X=16-bit] */
    goto L_c7c9;
L_c6af: /* [M=16-bit, X=16-bit] */
    /* $1D:C6AF: B5 12       | LDA $12,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6B1: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C6B2: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:C6B3: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C6B4: BF BE C6 1D | LDA $1DC6BE,X      [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_longx(ctx, 0x1DC6BEu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6B8: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C6B9: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C6BB: 6C 00 00    | JMP ($0000)        [M=16-bit, X=16-bit] */
    {
        uint16_t jmp_tgt = snes_bus_read16(ctx, 0x000000u);
        switch (jmp_tgt) {
            case 0xC6C6u: bank1d_func_c6c6(ctx); return;
            case 0xC703u: bank1d_func_c703(ctx); return;
            case 0xC73Fu: bank1d_func_c73f(ctx); return;
            case 0xC7C2u: bank1d_func_c7c2(ctx); return;
            default:
                // REQUIRES INTERPRETER FALLBACK
                snes_interp_fallback_at(ctx, 0x1DC6BBu, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);
                return;
        }
    }
L_c7c9: /* [M=16-bit, X=16-bit] */
    /* $1D:C7C9: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7CC: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:C7CE: 5C 2C A5 01 | JML $01A52C        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a52c(ctx); ctx->pb = saved_pb; }
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c6c6  (SNES $1D:C6C6 | Ghidra 0x1dc6c6)
 * Discovery Provenance: JMP ($0000) table $1D:C6BE @ $1D:C6BB
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c6c6(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C6C6: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:C6C8: A9 90 00    | LDA #$0090         [M=16-bit, X=16-bit] */
    ctx->a = 0x0090u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6CB: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C6CD: A9 3E 00    | LDA #$003E         [M=16-bit, X=16-bit] */
    ctx->a = 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6D0: 95 32       | STA $32,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x32u), (uint16_t)(ctx->a));
    /* $1D:C6D2: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6D5: 95 34       | STA $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(ctx->a));
    /* $1D:C6D7: 74 36       | STZ $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(0u));
    /* $1D:C6D9: A9 69 C5    | LDA #$C569         [M=16-bit, X=16-bit] */
    ctx->a = 0xC569u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6DC: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:C6DE: A9 00 3E    | LDA #$3E00         [M=16-bit, X=16-bit] */
    ctx->a = 0x3E00u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6E1: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C6E3: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6E6: 85 0C       | STA $0C            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Cu), (uint16_t)(ctx->a));
    /* $1D:C6E8: A9 F8 FF    | LDA #$FFF8         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFF8u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6EB: 85 0E       | STA $0E            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C6ED: A9 3C 40    | LDA #$403C         [M=16-bit, X=16-bit] */
    ctx->a = 0x403Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6F0: 85 10       | STA $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->a));
    /* $1D:C6F2: 64 00       | STZ $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(0u));
    /* $1D:C6F4: A9 0A 00    | LDA #$000A         [M=16-bit, X=16-bit] */
    ctx->a = 0x000Au;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6F7: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $1D:C6F9: A9 0C 00    | LDA #$000C         [M=16-bit, X=16-bit] */
    ctx->a = 0x000Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C6FC: A0 40 03    | LDY #$0340         [M=16-bit, X=16-bit] */
    ctx->y = 0x0340u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C6FF: 5C 1C A4 01 | JML $01A41C        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a41c(ctx); ctx->pb = saved_pb; }
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c703  (SNES $1D:C703 | Ghidra 0x1dc703)
 * Discovery Provenance: JMP ($0000) table $1D:C6BE @ $1D:C6BB
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c703(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C703: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C704: AD 0A 09    | LDA $090A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C707: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:C70A: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C70B: F5 0A       | SBC $0A,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au)));
    /* $1D:C70D: 10 03       | BPL $C712          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c712;
    /* $1D:C70F: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
L_c712: /* [M=16-bit, X=16-bit] */
    /* $1D:C712: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:C715: B0 32       | BCS $C749          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c749;
    /* $1D:C717: 74 18       | STZ $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(0u));
    /* $1D:C719: 74 1A       | STZ $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(0u));
    /* $1D:C71B: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C71C: AD 0E 09    | LDA $090E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C71F: 69 E4 FF    | ADC #$FFE4         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE4u);
    /* $1D:C722: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C723: F5 0E       | SBC $0E,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu)));
    /* $1D:C725: 10 03       | BPL $C72A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c72a;
    /* $1D:C727: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
L_c72a: /* [M=16-bit, X=16-bit] */
    /* $1D:C72A: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:C72D: B0 1A       | BCS $C749          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c749;
    /* $1D:C72F: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:C731: A9 70 00    | LDA #$0070         [M=16-bit, X=16-bit] */
    ctx->a = 0x0070u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C734: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:C736: A9 80 01    | LDA #$0180         [M=16-bit, X=16-bit] */
    ctx->a = 0x0180u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C739: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C73B: 5C 13 A5 01 | JML $01A513        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a513(ctx); ctx->pb = saved_pb; }
    return;
L_c749: /* [M=16-bit, X=16-bit] */
    /* $1D:C749: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C74C: 8D 60 16    | STA $1660          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1660u), (uint16_t)(ctx->a));
    /* $1D:C74F: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C751: 30 0E       | BMI $C761          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c761;
    /* $1D:C753: C9 20 00    | CMP #$0020         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0020u);
    /* $1D:C756: 90 09       | BCC $C761          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c761;
    /* $1D:C758: C9 E0 00    | CMP #$00E0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00E0u);
    /* $1D:C75B: 90 08       | BCC $C765          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c765;
    /* $1D:C75D: 74 36       | STZ $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(0u));
    /* $1D:C75F: 80 04       | BRA $C765          [M=16-bit, X=16-bit] */
    goto L_c765;
L_c761: /* [M=16-bit, X=16-bit] */
    /* $1D:C761: F6 36       | INC $36,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(v16)); }
    /* $1D:C763: F6 36       | INC $36,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(v16)); }
L_c765: /* [M=16-bit, X=16-bit] */
    /* $1D:C765: B5 36       | LDA $36,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C767: D0 1A       | BNE $C783          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c783;
    /* $1D:C769: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C76B: 10 05       | BPL $C772          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c772;
    /* $1D:C76D: C9 FC FF    | CMP #$FFFC         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFFCu);
    /* $1D:C770: 90 29       | BCC $C79B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c79b;
L_c772: /* [M=16-bit, X=16-bit] */
    /* $1D:C772: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C773: B5 18       | LDA $18,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C775: E9 00 30    | SBC #$3000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x3000u);
    /* $1D:C778: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:C77A: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C77C: E9 00 00    | SBC #$0000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0000u);
    /* $1D:C77F: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:C781: 80 18       | BRA $C79B          [M=16-bit, X=16-bit] */
    goto L_c79b;
L_c783: /* [M=16-bit, X=16-bit] */
    /* $1D:C783: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C785: 30 05       | BMI $C78C          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c78c;
    /* $1D:C787: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:C78A: B0 0F       | BCS $C79B          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c79b;
L_c78c: /* [M=16-bit, X=16-bit] */
    /* $1D:C78C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C78D: B5 18       | LDA $18,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C78F: 69 00 30    | ADC #$3000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x3000u);
    /* $1D:C792: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:C794: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C796: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:C799: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
L_c79b: /* [M=16-bit, X=16-bit] */
    /* $1D:C79B: B5 34       | LDA $34,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C79D: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C79E: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C79F: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7A2: 95 34       | STA $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(ctx->a));
    /* $1D:C7A4: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C7A5: B9 1A 94    | LDA $941A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x941Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7A8: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:C7A9: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C7AB: B9 CA 93    | LDA $93CA,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x93CAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7AE: 2A          | ROL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_rol16(ctx, ctx->a);
    /* $1D:C7AF: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C7B1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C7B2: B5 0C       | LDA $0C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7B4: 65 00       | ADC $00            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u)));
    /* $1D:C7B6: 95 0C       | STA $0C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Cu), (uint16_t)(ctx->a));
    /* $1D:C7B8: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7BA: 65 02       | ADC $02            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u)));
    /* $1D:C7BC: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C7BE: 5C 13 A5 01 | JML $01A513        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a513(ctx); ctx->pb = saved_pb; }
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c73f  (SNES $1D:C73F | Ghidra 0x1dc73f)
 * Discovery Provenance: JMP ($0000) table $1D:C6BE @ $1D:C6BB
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c73f(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C73F: AD 42 16    | LDA $1642          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1642u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C742: C9 05 00    | CMP #$0005         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0005u);
    /* $1D:C745: D0 02       | BNE $C749          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c749;
    /* $1D:C747: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
L_c749: /* [M=16-bit, X=16-bit] */
    /* $1D:C749: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C74C: 8D 60 16    | STA $1660          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1660u), (uint16_t)(ctx->a));
    /* $1D:C74F: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C751: 30 0E       | BMI $C761          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c761;
    /* $1D:C753: C9 20 00    | CMP #$0020         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0020u);
    /* $1D:C756: 90 09       | BCC $C761          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c761;
    /* $1D:C758: C9 E0 00    | CMP #$00E0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00E0u);
    /* $1D:C75B: 90 08       | BCC $C765          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c765;
    /* $1D:C75D: 74 36       | STZ $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(0u));
    /* $1D:C75F: 80 04       | BRA $C765          [M=16-bit, X=16-bit] */
    goto L_c765;
L_c761: /* [M=16-bit, X=16-bit] */
    /* $1D:C761: F6 36       | INC $36,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(v16)); }
    /* $1D:C763: F6 36       | INC $36,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(v16)); }
L_c765: /* [M=16-bit, X=16-bit] */
    /* $1D:C765: B5 36       | LDA $36,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C767: D0 1A       | BNE $C783          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c783;
    /* $1D:C769: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C76B: 10 05       | BPL $C772          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c772;
    /* $1D:C76D: C9 FC FF    | CMP #$FFFC         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFFCu);
    /* $1D:C770: 90 29       | BCC $C79B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c79b;
L_c772: /* [M=16-bit, X=16-bit] */
    /* $1D:C772: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C773: B5 18       | LDA $18,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C775: E9 00 30    | SBC #$3000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x3000u);
    /* $1D:C778: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:C77A: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C77C: E9 00 00    | SBC #$0000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0000u);
    /* $1D:C77F: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:C781: 80 18       | BRA $C79B          [M=16-bit, X=16-bit] */
    goto L_c79b;
L_c783: /* [M=16-bit, X=16-bit] */
    /* $1D:C783: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C785: 30 05       | BMI $C78C          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c78c;
    /* $1D:C787: C9 04 00    | CMP #$0004         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0004u);
    /* $1D:C78A: B0 0F       | BCS $C79B          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c79b;
L_c78c: /* [M=16-bit, X=16-bit] */
    /* $1D:C78C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C78D: B5 18       | LDA $18,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C78F: 69 00 30    | ADC #$3000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x3000u);
    /* $1D:C792: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:C794: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C796: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:C799: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
L_c79b: /* [M=16-bit, X=16-bit] */
    /* $1D:C79B: B5 34       | LDA $34,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C79D: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C79E: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C79F: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7A2: 95 34       | STA $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(ctx->a));
    /* $1D:C7A4: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C7A5: B9 1A 94    | LDA $941A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x941Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7A8: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:C7A9: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C7AB: B9 CA 93    | LDA $93CA,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x93CAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7AE: 2A          | ROL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_rol16(ctx, ctx->a);
    /* $1D:C7AF: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:C7B1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C7B2: B5 0C       | LDA $0C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7B4: 65 00       | ADC $00            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u)));
    /* $1D:C7B6: 95 0C       | STA $0C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Cu), (uint16_t)(ctx->a));
    /* $1D:C7B8: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7BA: 65 02       | ADC $02            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u)));
    /* $1D:C7BC: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C7BE: 5C 13 A5 01 | JML $01A513        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a513(ctx); ctx->pb = saved_pb; }
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c7c2  (SNES $1D:C7C2 | Ghidra 0x1dc7c2)
 * Discovery Provenance: JMP ($0000) table $1D:C6BE @ $1D:C6BB
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c7c2(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C7C2: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:C7C4: 10 0C       | BPL $C7D2          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c7d2;
    /* $1D:C7C6: EE 12 09    | INC $0912          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0912u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0912u), (uint16_t)(v16)); }
    /* $1D:C7C9: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7CC: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:C7CE: 5C 2C A5 01 | JML $01A52C        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a52c(ctx); ctx->pb = saved_pb; }
    return;
L_c7d2: /* [M=16-bit, X=16-bit] */
    /* $1D:C7D2: 5C 13 A5 01 | JML $01A513        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_a513(ctx); ctx->pb = saved_pb; }
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c7d6  (SNES $1D:C7D6 | Ghidra 0x1dc7d6)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c7d6(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C7D6: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7D8: C9 B4 00    | CMP #$00B4         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B4u);
    /* $1D:C7DB: 90 14       | BCC $C7F1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c7f1;
    /* $1D:C7DD: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7E0: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:C7E4: A9 FC FF    | LDA #$FFFC         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFCu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7E7: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:C7E9: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C7EA: A9 16 C5    | LDA #$C516         [M=16-bit, X=16-bit] */
    ctx->a = 0xC516u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7ED: 5C 66 CF 01 | JML $01CF66        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf66(ctx); ctx->pb = saved_pb; }
    return;
L_c7f1: /* [M=16-bit, X=16-bit] */
    /* $1D:C7F1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C7F2: B5 1C       | LDA $1C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7F4: 69 00 30    | ADC #$3000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x3000u);
    /* $1D:C7F7: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:C7F9: B5 1E       | LDA $1E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C7FB: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:C7FE: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:C800: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c801  (SNES $1D:C801 | Ghidra 0x1dc801)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c801(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C801: AD 88 16    | LDA $1688          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1688u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C804: D0 2F       | BNE $C835          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c835;
    /* $1D:C806: AD 3C 16    | LDA $163C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C809: C9 F0 00    | CMP #$00F0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00F0u);
    /* $1D:C80C: 90 14       | BCC $C822          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c822;
    /* $1D:C80E: D0 07       | BNE $C817          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c817;
    /* $1D:C810: A9 1C 00    | LDA #$001C         [M=16-bit, X=16-bit] */
    ctx->a = 0x001Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C813: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:C815: 80 04       | BRA $C81B          [M=16-bit, X=16-bit] */
    goto L_c81b;
L_c817: /* [M=16-bit, X=16-bit] */
    /* $1D:C817: A5 BC       | LDA $BC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xBCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C819: F0 03       | BEQ $C81E          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c81e;
L_c81b: /* [M=16-bit, X=16-bit] */
    /* $1D:C81B: A9 36 C5    | LDA #$C536         [M=16-bit, X=16-bit] */
    ctx->a = 0xC536u;
    snes_set_nz16(ctx, ctx->a);
L_c81e: /* [M=16-bit, X=16-bit] */
    /* $1D:C81E: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:C820: 80 25       | BRA $C847          [M=16-bit, X=16-bit] */
    goto L_c847;
L_c822: /* [M=16-bit, X=16-bit] */
    /* $1D:C822: C9 02 00    | CMP #$0002         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0002u);
    /* $1D:C825: 90 0E       | BCC $C835          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c835;
    /* $1D:C827: AC 3E 16    | LDY $163E          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C82A: B9 16 00    | LDA $0016,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0016u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C82D: F0 06       | BEQ $C835          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c835;
    /* $1D:C82F: 30 04       | BMI $C835          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c835;
    /* $1D:C831: B5 06       | LDA $06,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C833: 10 12       | BPL $C847          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c847;
L_c835: /* [M=16-bit, X=16-bit] */
    /* $1D:C835: 20 C9 CA    | JSR $CAC9          [M=16-bit, X=16-bit] */
    bank1d_func_cac9(ctx);
    /* $1D:C838: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C83B: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:C83F: A9 CE C4    | LDA #$C4CE         [M=16-bit, X=16-bit] */
    ctx->a = 0xC4CEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C842: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C843: 5C 66 CF 01 | JML $01CF66        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf66(ctx); ctx->pb = saved_pb; }
    return;
L_c847: /* [M=16-bit, X=16-bit] */
    /* $1D:C847: 20 E9 CA    | JSR $CAE9          [M=16-bit, X=16-bit] */
    bank1d_func_cae9(ctx);
    /* $1D:C84A: AC 3C 16    | LDY $163C          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C84D: C0 40 00    | CPY #$0040         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->y, 0x0040u);
    /* $1D:C850: 90 03       | BCC $C855          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c855;
    /* $1D:C852: A0 40 00    | LDY #$0040         [M=16-bit, X=16-bit] */
    ctx->y = 0x0040u;
    snes_set_nz16(ctx, ctx->y);
L_c855: /* [M=16-bit, X=16-bit] */
    /* $1D:C855: 22 A4 AC 1D | JSL $1DACA4        [M=16-bit, X=16-bit] */
    bank1d_func_aca4(ctx);
    /* $1D:C859: AC 3E 16    | LDY $163E          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C85C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C85D: B9 12 13    | LDA $1312,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x1312u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C860: 65 08       | ADC $08            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:C862: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C864: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C865: B9 14 13    | LDA $1314,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x1314u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C868: 65 0A       | ADC $0A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $1D:C86A: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C86C: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c86d  (SNES $1D:C86D | Ghidra 0x1dc86d)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c86d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C86D: B5 06       | LDA $06,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C86F: 30 0A       | BMI $C87B          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c87b;
    /* $1D:C871: AD 88 16    | LDA $1688          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1688u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C874: D0 05       | BNE $C87B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c87b;
    /* $1D:C876: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C879: 10 0F       | BPL $C88A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c88a;
L_c87b: /* [M=16-bit, X=16-bit] */
    /* $1D:C87B: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C87E: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:C882: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C883: A9 16 C5    | LDA #$C516         [M=16-bit, X=16-bit] */
    ctx->a = 0xC516u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C886: 5C 66 CF 01 | JML $01CF66        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf66(ctx); ctx->pb = saved_pb; }
    return;
L_c88a: /* [M=16-bit, X=16-bit] */
    /* $1D:C88A: B5 12       | LDA $12,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C88C: D0 50       | BNE $C8DE          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c8de;
    /* $1D:C88E: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C88F: AD 0A 09    | LDA $090A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C892: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:C895: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C897: 85 FC       | STA $FC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFCu), (uint16_t)(ctx->a));
    /* $1D:C899: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C89A: AD 0E 09    | LDA $090E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C89D: 69 E0 FF    | ADC #$FFE0         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE0u);
    /* $1D:C8A0: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C8A2: 85 FE       | STA $FE            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFEu), (uint16_t)(ctx->a));
    /* $1D:C8A4: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:C8A5: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8A7: 89 1F 00    | BIT #$001F         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x001Fu)) == 0u ? 1u : 0u);
    /* $1D:C8AA: D0 30       | BNE $C8DC          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c8dc;
    /* $1D:C8AC: A9 90 00    | LDA #$0090         [M=16-bit, X=16-bit] */
    ctx->a = 0x0090u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8AF: 20 73 CA    | JSR $CA73          [M=16-bit, X=16-bit] */
    bank1d_func_ca73(ctx);
    /* $1D:C8B2: B0 28       | BCS $C8DC          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c8dc;
    /* $1D:C8B4: F6 12       | INC $12,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x12u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(v16)); }
    /* $1D:C8B6: A5 FC       | LDA $FC            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFCu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8B8: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C8BA: A5 FE       | LDA $FE            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFEu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8BC: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C8BE: 74 30       | STZ $30,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x30u), (uint16_t)(0u));
    /* $1D:C8C0: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8C2: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:C8C3: 95 06       | STA $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $1D:C8C5: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C8C6: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8C8: 29 01 00    | AND #$0001         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8CB: 69 01 00    | ADC #$0001         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0001u);
    /* $1D:C8CE: 89 01 00    | BIT #$0001         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0001u)) == 0u ? 1u : 0u);
    /* $1D:C8D1: D0 04       | BNE $C8D7          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c8d7;
    /* $1D:C8D3: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8D6: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
L_c8d7: /* [M=16-bit, X=16-bit] */
    /* $1D:C8D7: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:C8D9: 20 D4 CC    | JSR $CCD4          [M=16-bit, X=16-bit] */
    bank1d_func_ccd4(ctx);
L_c8dc: /* [M=16-bit, X=16-bit] */
    /* $1D:C8DC: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:C8DD: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c8de: /* [M=16-bit, X=16-bit] */
    /* $1D:C8DE: B5 1E       | LDA $1E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8E0: 30 20       | BMI $C902          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c902;
    /* $1D:C8E2: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8E4: 30 1C       | BMI $C902          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c902;
    /* $1D:C8E6: C9 B0 00    | CMP #$00B0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B0u);
    /* $1D:C8E9: 90 17       | BCC $C902          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c902;
    /* $1D:C8EB: 20 D4 CC    | JSR $CCD4          [M=16-bit, X=16-bit] */
    bank1d_func_ccd4(ctx);
    /* $1D:C8EE: B5 30       | LDA $30,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x30u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8F0: D0 10       | BNE $C902          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c902;
    /* $1D:C8F2: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8F4: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8F7: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8F8: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:C8FA: F6 30       | INC $30,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x30u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x30u), (uint16_t)(v16)); }
    /* $1D:C8FC: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C8FF: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
L_c902: /* [M=16-bit, X=16-bit] */
    /* $1D:C902: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C903: B5 1C       | LDA $1C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C905: 69 00 3C    | ADC #$3C00         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x3C00u);
    /* $1D:C908: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:C90A: B5 1E       | LDA $1E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C90C: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:C90F: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:C911: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c912  (SNES $1D:C912 | Ghidra 0x1dc912)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c912(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C912: AD 88 16    | LDA $1688          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1688u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C915: D0 05       | BNE $C91C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c91c;
    /* $1D:C917: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C91A: 10 08       | BPL $C924          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c924;
L_c91c: /* [M=16-bit, X=16-bit] */
    /* $1D:C91C: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C91D: A9 72 C6    | LDA #$C672         [M=16-bit, X=16-bit] */
    ctx->a = 0xC672u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C920: 5C 66 CF 01 | JML $01CF66        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf66(ctx); ctx->pb = saved_pb; }
    return;
L_c924: /* [M=16-bit, X=16-bit] */
    /* $1D:C924: B5 3A       | LDA $3A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C926: F0 38       | BEQ $C960          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c960;
    /* $1D:C928: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C92A: 10 05       | BPL $C931          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c931;
    /* $1D:C92C: C9 80 FF    | CMP #$FF80         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFF80u);
    /* $1D:C92F: 90 EB       | BCC $C91C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c91c;
L_c931: /* [M=16-bit, X=16-bit] */
    /* $1D:C931: B5 38       | LDA $38,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C933: C9 C0 FF    | CMP #$FFC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFC0u);
    /* $1D:C936: D0 1F       | BNE $C957          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c957;
    /* $1D:C938: D6 24       | DEC $24,X          [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(v16)); }
    /* $1D:C93A: 10 1A       | BPL $C956          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c956;
    /* $1D:C93C: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C93F: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:C941: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C942: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C944: F5 36       | SBC $36,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u)));
    /* $1D:C946: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C947: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C949: AD 84 00    | LDA $0084          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0084u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C94C: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:C94D: 29 02 00    | AND #$0002         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C950: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C951: B9 23 FF    | LDA $FF23,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF23u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C954: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
L_c956: /* [M=16-bit, X=16-bit] */
    /* $1D:C956: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c957: /* [M=16-bit, X=16-bit] */
    /* $1D:C957: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C958: B5 38       | LDA $38,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C95A: E9 04 00    | SBC #$0004         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0004u);
    /* $1D:C95D: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $1D:C95F: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c960: /* [M=16-bit, X=16-bit] */
    /* $1D:C960: B4 38       | LDY $38,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C962: B9 16 00    | LDA $0016,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0016u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C965: F0 B5       | BEQ $C91C          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_c91c;
    /* $1D:C967: B9 38 00    | LDA $0038,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0038u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C96A: B4 36       | LDY $36,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C96C: 22 A4 AC 1D | JSL $1DACA4        [M=16-bit, X=16-bit] */
    bank1d_func_aca4(ctx);
    /* $1D:C970: B4 38       | LDY $38,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C972: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C973: B9 0A 00    | LDA $000A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C976: 65 08       | ADC $08            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:C978: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C97A: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C97B: B9 0E 00    | LDA $000E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C97E: 65 0A       | ADC $0A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $1D:C980: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C982: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_c983  (SNES $1D:C983 | Ghidra 0x1dc983)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_c983(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:C983: AD 88 16    | LDA $1688          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1688u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C986: D0 05       | BNE $C98D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_c98d;
    /* $1D:C988: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C98B: 10 08       | BPL $C995          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_c995;
L_c98d: /* [M=16-bit, X=16-bit] */
    /* $1D:C98D: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C98E: A9 CE C4    | LDA #$C4CE         [M=16-bit, X=16-bit] */
    ctx->a = 0xC4CEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C991: 5C 66 CF 01 | JML $01CF66        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf66(ctx); ctx->pb = saved_pb; }
    return;
L_c995: /* [M=16-bit, X=16-bit] */
    /* $1D:C995: AD 3A 16    | LDA $163A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C998: B4 3A       | LDY $3A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Au));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C99A: 30 2E       | BMI $C9CA          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_c9ca;
    /* $1D:C99C: E0 40 04    | CPX #$0440         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0440u);
    /* $1D:C99F: B0 04       | BCS $C9A5          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_c9a5;
    /* $1D:C9A1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C9A2: 69 E0 FF    | ADC #$FFE0         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE0u);
L_c9a5: /* [M=16-bit, X=16-bit] */
    /* $1D:C9A5: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C9A6: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $1D:C9A9: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:C9AC: AC 3C 16    | LDY $163C          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Cu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C9AF: 22 A4 AC 1D | JSL $1DACA4        [M=16-bit, X=16-bit] */
    bank1d_func_aca4(ctx);
    /* $1D:C9B3: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C9B4: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C9B5: 69 C0 FF    | ADC #$FFC0         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFC0u);
    /* $1D:C9B8: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C9B9: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C9BA: B9 0A 00    | LDA $000A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C9BD: 65 08       | ADC $08            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:C9BF: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C9C1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C9C2: B9 0E 00    | LDA $000E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x000Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C9C5: 65 0A       | ADC $0A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $1D:C9C7: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C9C9: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_c9ca: /* [M=16-bit, X=16-bit] */
    /* $1D:C9CA: AD 3E 16    | LDA $163E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C9CD: E0 40 04    | CPX #$0440         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0440u);
    /* $1D:C9D0: 90 06       | BCC $C9D8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_c9d8;
    /* $1D:C9D2: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C9D3: 6D 3E 09    | ADC $093E          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x093Eu)));
    /* $1D:C9D6: 80 04       | BRA $C9DC          [M=16-bit, X=16-bit] */
    goto L_c9dc;
L_c9d8: /* [M=16-bit, X=16-bit] */
    /* $1D:C9D8: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:C9D9: ED 3E 09    | SBC $093E          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x093Eu)));
L_c9dc: /* [M=16-bit, X=16-bit] */
    /* $1D:C9DC: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C9DF: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:C9E2: A0 14 00    | LDY #$0014         [M=16-bit, X=16-bit] */
    ctx->y = 0x0014u;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:C9E5: 22 A4 AC 1D | JSL $1DACA4        [M=16-bit, X=16-bit] */
    bank1d_func_aca4(ctx);
    /* $1D:C9E9: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C9EA: AD 0A 09    | LDA $090A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C9ED: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:C9F0: 65 08       | ADC $08            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u)));
    /* $1D:C9F2: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:C9F4: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:C9F5: AD 0E 09    | LDA $090E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:C9F8: 69 E8 FF    | ADC #$FFE8         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE8u);
    /* $1D:C9FB: 65 0A       | ADC $0A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $1D:C9FD: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:C9FF: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ca00  (SNES $1D:CA00 | Ghidra 0x1dca00)
 * Discovery Provenance: JML [$0000] table $05:A48A @ $00:EB17
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ca00(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CA00: AD 44 16    | LDA $1644          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1644u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA03: 30 05       | BMI $CA0A          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_ca0a;
    /* $1D:CA05: AD 88 16    | LDA $1688          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1688u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA08: F0 0F       | BEQ $CA19          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ca19;
L_ca0a: /* [M=16-bit, X=16-bit] */
    /* $1D:CA0A: A9 30 00    | LDA #$0030         [M=16-bit, X=16-bit] */
    ctx->a = 0x0030u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA0D: 22 8F 86 00 | JSL $00868F        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_868f(ctx); ctx->pb = saved_pb; }
    /* $1D:CA11: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:CA12: A9 CE C4    | LDA #$C4CE         [M=16-bit, X=16-bit] */
    ctx->a = 0xC4CEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA15: 5C 66 CF 01 | JML $01CF66        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_cf66(ctx); ctx->pb = saved_pb; }
    return;
L_ca19: /* [M=16-bit, X=16-bit] */
    /* $1D:CA19: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA1B: B4 1E       | LDY $1E,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:CA1D: 30 07       | BMI $CA26          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_ca26;
    /* $1D:CA1F: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $1D:CA22: 90 14       | BCC $CA38          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ca38;
    /* $1D:CA24: 80 05       | BRA $CA2B          [M=16-bit, X=16-bit] */
    goto L_ca2b;
L_ca26: /* [M=16-bit, X=16-bit] */
    /* $1D:CA26: C9 20 00    | CMP #$0020         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0020u);
    /* $1D:CA29: B0 0D       | BCS $CA38          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ca38;
L_ca2b: /* [M=16-bit, X=16-bit] */
    /* $1D:CA2B: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA2C: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA2F: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA30: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:CA32: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA35: 8D 62 16    | STA $1662          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x1662u), (uint16_t)(ctx->a));
L_ca38: /* [M=16-bit, X=16-bit] */
    /* $1D:CA38: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA3A: B4 1A       | LDY $1A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:CA3C: 30 07       | BMI $CA45          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_ca45;
    /* $1D:CA3E: C9 F0 00    | CMP #$00F0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00F0u);
    /* $1D:CA41: 90 0E       | BCC $CA51          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ca51;
    /* $1D:CA43: 80 05       | BRA $CA4A          [M=16-bit, X=16-bit] */
    goto L_ca4a;
L_ca45: /* [M=16-bit, X=16-bit] */
    /* $1D:CA45: C9 08 00    | CMP #$0008         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0008u);
    /* $1D:CA48: B0 07       | BCS $CA51          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ca51;
L_ca4a: /* [M=16-bit, X=16-bit] */
    /* $1D:CA4A: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA4B: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA4E: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA4F: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
L_ca51: /* [M=16-bit, X=16-bit] */
    /* $1D:CA51: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ca52  (SNES $1D:CA52 | Ghidra 0x1dca52)
 * Discovery Provenance: JSR $CA52 @ $1D:C55F
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ca52(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CA52: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:CA54: 84 04       | STY $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->y));
    /* $1D:CA56: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:CA57: 64 00       | STZ $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(0u));
    /* $1D:CA59: A2 00 03    | LDX #$0300         [M=16-bit, X=16-bit] */
    ctx->x = 0x0300u;
    snes_set_nz16(ctx, ctx->x);
L_ca5c: /* [M=16-bit, X=16-bit] */
    /* $1D:CA5C: 20 7D CA    | JSR $CA7D          [M=16-bit, X=16-bit] */
    bank1d_func_ca7d(ctx);
    /* $1D:CA5F: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA61: 95 24       | STA $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->a));
    /* $1D:CA63: E6 00       | INC $00            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(v16)); }
    /* $1D:CA65: E6 00       | INC $00            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(v16)); }
    /* $1D:CA67: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CA68: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA69: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $1D:CA6C: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:CA6D: C5 04       | CMP $04            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x04u)));
    /* $1D:CA6F: 90 EB       | BCC $CA5C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ca5c;
    /* $1D:CA71: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:CA72: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ca73  (SNES $1D:CA73 | Ghidra 0x1dca73)
 * Discovery Provenance: JSR $CA73 @ $1D:C8AF
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ca73(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CA73: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $1D:CA74: 22 83 9E 01 | JSL $019E83        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x01u; bank01_func_9e83(ctx); ctx->pb = saved_pb; }
    /* $1D:CA78: 68          | PLA                [M=16-bit, X=16-bit] */
    ctx->a = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA79: B0 4D       | BCS $CAC8          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_cac8;
    /* $1D:CA7B: 80 02       | BRA $CA7F          [M=16-bit, X=16-bit] */
    goto L_ca7f;
L_ca7f: /* [M=16-bit, X=16-bit] */
    /* $1D:CA7F: 95 10       | STA $10,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x10u), (uint16_t)(ctx->a));
    /* $1D:CA81: 74 12       | STZ $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(0u));
    /* $1D:CA83: 74 20       | STZ $20,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x20u), (uint16_t)(0u));
    /* $1D:CA85: 74 22       | STZ $22,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x22u), (uint16_t)(0u));
    /* $1D:CA87: 74 18       | STZ $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(0u));
    /* $1D:CA89: 74 1A       | STZ $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(0u));
    /* $1D:CA8B: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:CA8D: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:CA8F: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CA90: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA92: 69 0A 00    | ADC #$000A         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x000Au);
    /* $1D:CA95: 95 06       | STA $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $1D:CA97: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA9A: 95 28       | STA $28,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x28u), (uint16_t)(ctx->a));
    /* $1D:CA9C: A9 F8 FF    | LDA #$FFF8         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFF8u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA9F: 95 2A       | STA $2A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Au), (uint16_t)(ctx->a));
    /* $1D:CAA1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CAA2: AD 0A 09    | LDA $090A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAA5: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:CAA8: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:CAAA: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CAAB: AD 0E 09    | LDA $090E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAAE: 69 E4 FF    | ADC #$FFE4         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE4u);
    /* $1D:CAB1: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:CAB3: AD 36 16    | LDA $1636          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAB6: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:CAB7: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:CAB8: B9 3E FF    | LDA $FF3E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF3Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CABB: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:CABD: B9 67 FF    | LDA $FF67,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF67u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAC0: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:CAC2: B9 77 FF    | LDA $FF77,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF77u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAC5: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:CAC7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
L_cac8: /* [M=16-bit, X=16-bit] */
    /* $1D:CAC8: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ca7d  (SNES $1D:CA7D | Ghidra 0x1dca7d)
 * Discovery Provenance: JSR $CA7D @ $1D:CA5C
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ca7d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CA7D: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA7F: 95 10       | STA $10,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x10u), (uint16_t)(ctx->a));
    /* $1D:CA81: 74 12       | STZ $12,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x12u), (uint16_t)(0u));
    /* $1D:CA83: 74 20       | STZ $20,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x20u), (uint16_t)(0u));
    /* $1D:CA85: 74 22       | STZ $22,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x22u), (uint16_t)(0u));
    /* $1D:CA87: 74 18       | STZ $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(0u));
    /* $1D:CA89: 74 1A       | STZ $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(0u));
    /* $1D:CA8B: 74 1C       | STZ $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(0u));
    /* $1D:CA8D: 74 1E       | STZ $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(0u));
    /* $1D:CA8F: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CA90: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA92: 69 0A 00    | ADC #$000A         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x000Au);
    /* $1D:CA95: 95 06       | STA $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $1D:CA97: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA9A: 95 28       | STA $28,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x28u), (uint16_t)(ctx->a));
    /* $1D:CA9C: A9 F8 FF    | LDA #$FFF8         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFF8u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CA9F: 95 2A       | STA $2A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Au), (uint16_t)(ctx->a));
    /* $1D:CAA1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CAA2: AD 0A 09    | LDA $090A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAA5: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:CAA8: 95 0A       | STA $0A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $1D:CAAA: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CAAB: AD 0E 09    | LDA $090E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAAE: 69 E4 FF    | ADC #$FFE4         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFE4u);
    /* $1D:CAB1: 95 0E       | STA $0E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x0Eu), (uint16_t)(ctx->a));
    /* $1D:CAB3: AD 36 16    | LDA $1636          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1636u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAB6: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:CAB7: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:CAB8: B9 3E FF    | LDA $FF3E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF3Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CABB: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:CABD: B9 67 FF    | LDA $FF67,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF67u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAC0: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:CAC2: B9 77 FF    | LDA $FF77,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF77u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAC5: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:CAC7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CAC8: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cac9  (SNES $1D:CAC9 | Ghidra 0x1dcac9)
 * Discovery Provenance: JSR $CAC9 @ $1D:C835
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cac9(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CAC9: 20 E9 CA    | JSR $CAE9          [M=16-bit, X=16-bit] */
    bank1d_func_cae9(ctx);
    /* $1D:CACC: 20 E9 CA    | JSR $CAE9          [M=16-bit, X=16-bit] */
    bank1d_func_cae9(ctx);
    /* $1D:CACF: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $1D:CAD0: 29 7E 00    | AND #$007E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x007Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAD3: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:CAD4: B9 5A 8D    | LDA $8D5A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8D5Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAD7: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:CAD9: B9 7A 8D    | LDA $8D7A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8D7Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CADC: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:CADE: B9 FA 8D    | LDA $8DFA,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8DFAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAE1: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:CAE3: B9 1A 8E    | LDA $8E1A,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8E1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAE6: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:CAE8: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cae9  (SNES $1D:CAE9 | Ghidra 0x1dcae9)
 * Discovery Provenance: JSR $CAE9 @ $1D:CAC9
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cae9(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CAE9: B5 24       | LDA $24,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x24u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CAEB: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:CAEC: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:CAED: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:CAEE: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $1D:CAEF: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CAF0: 69 20 00    | ADC #$0020         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0020u);
    /* $1D:CAF3: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CAF4: 6D 3A 16    | ADC $163A          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au)));
    /* $1D:CAF7: E0 00 05    | CPX #$0500         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0500u);
    /* $1D:CAFA: 90 04       | BCC $CB00          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cb00;
    /* $1D:CAFC: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CAFD: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
L_cb00: /* [M=16-bit, X=16-bit] */
    /* $1D:CB00: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB03: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cb04  (SNES $1D:CB04 | Ghidra 0x1dcb04)
 * Discovery Provenance: JSR $CB04 @ $1D:C1EF
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cb04(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CB04: B5 36       | LDA $36,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB06: D0 19       | BNE $CB21          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_cb21;
    /* $1D:CB08: B5 34       | LDA $34,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB0A: 30 04       | BMI $CB10          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_cb10;
    /* $1D:CB0C: C5 00       | CMP $00            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u)));
    /* $1D:CB0E: B0 28       | BCS $CB38          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_cb38;
L_cb10: /* [M=16-bit, X=16-bit] */
    /* $1D:CB10: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CB11: B5 32       | LDA $32,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x32u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB13: 69 00 10    | ADC #$1000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x1000u);
    /* $1D:CB16: 95 32       | STA $32,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x32u), (uint16_t)(ctx->a));
    /* $1D:CB18: B5 34       | LDA $34,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB1A: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:CB1D: 95 34       | STA $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(ctx->a));
    /* $1D:CB1F: 80 17       | BRA $CB38          [M=16-bit, X=16-bit] */
    goto L_cb38;
L_cb21: /* [M=16-bit, X=16-bit] */
    /* $1D:CB21: B5 34       | LDA $34,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB23: 10 04       | BPL $CB29          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_cb29;
    /* $1D:CB25: C5 02       | CMP $02            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u)));
    /* $1D:CB27: 90 0F       | BCC $CB38          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cb38;
L_cb29: /* [M=16-bit, X=16-bit] */
    /* $1D:CB29: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:CB2A: B5 32       | LDA $32,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x32u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB2C: E9 00 10    | SBC #$1000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x1000u);
    /* $1D:CB2F: 95 32       | STA $32,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x32u), (uint16_t)(ctx->a));
    /* $1D:CB31: B5 34       | LDA $34,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB33: E9 00 00    | SBC #$0000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0000u);
    /* $1D:CB36: 95 34       | STA $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(ctx->a));
L_cb38: /* [M=16-bit, X=16-bit] */
    /* $1D:CB38: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CB39: B5 32       | LDA $32,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x32u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB3B: 75 30       | ADC $30,X          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x30u)));
    /* $1D:CB3D: 95 30       | STA $30,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x30u), (uint16_t)(ctx->a));
    /* $1D:CB3F: B5 34       | LDA $34,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB41: 6D 3A 16    | ADC $163A          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x163Au)));
    /* $1D:CB44: 8D 3A 16    | STA $163A          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x163Au), (uint16_t)(ctx->a));
    /* $1D:CB47: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cb48  (SNES $1D:CB48 | Ghidra 0x1dcb48)
 * Discovery Provenance: JSR $CB48 @ $1D:BF76
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cb48(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CB48: B5 36       | LDA $36,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB4A: B4 34       | LDY $34,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x34u));
    snes_set_nz16(ctx, ctx->y);
    /* $1D:CB4C: F0 08       | BEQ $CB56          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_cb56;
    /* $1D:CB4E: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB4F: C9 4B 01    | CMP #$014B         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x014Bu);
    /* $1D:CB52: 90 10       | BCC $CB64          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cb64;
    /* $1D:CB54: 80 06       | BRA $CB5C          [M=16-bit, X=16-bit] */
    goto L_cb5c;
L_cb56: /* [M=16-bit, X=16-bit] */
    /* $1D:CB56: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB57: C9 20 01    | CMP #$0120         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0120u);
    /* $1D:CB5A: B0 08       | BCS $CB64          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_cb64;
L_cb5c: /* [M=16-bit, X=16-bit] */
    /* $1D:CB5C: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $1D:CB5D: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB5E: 49 01 00    | EOR #$0001         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB61: 95 34       | STA $34,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x34u), (uint16_t)(ctx->a));
    /* $1D:CB63: 68          | PLA                [M=16-bit, X=16-bit] */
    ctx->a = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->a);
L_cb64: /* [M=16-bit, X=16-bit] */
    /* $1D:CB64: 95 36       | STA $36,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x36u), (uint16_t)(ctx->a));
    /* Fallthrough into function $1D:CB66 */
    bank1d_func_cb66(ctx);
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cb66  (SNES $1D:CB66 | Ghidra 0x1dcb66)
 * Discovery Provenance: JSR $CB66 @ $1D:C0DD
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cb66(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CB66: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:CB67: AD D8 13    | LDA $13D8          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x13D8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB6A: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:CB6C: 29 FE 01    | AND #$01FE         [M=16-bit, X=16-bit] */
    ctx->a &= 0x01FEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB6F: E9 A0 00    | SBC #$00A0         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x00A0u);
    /* $1D:CB72: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:CB73: B5 36       | LDA $36,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x36u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB75: 22 6C E7 00 | JSL $00E76C        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_e76c(ctx); ctx->pb = saved_pb; }
    /* $1D:CB79: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cb7a  (SNES $1D:CB7A | Ghidra 0x1dcb7a)
 * Discovery Provenance: JSR $CB7A @ $1D:C1B8
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cb7a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CB7A: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB7C: 30 0E       | BMI $CB8C          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_cb8c;
    /* $1D:CB7E: C9 40 00    | CMP #$0040         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0040u);
    /* $1D:CB81: 90 09       | BCC $CB8C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cb8c;
    /* $1D:CB83: C9 88 00    | CMP #$0088         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0088u);
    /* $1D:CB86: 90 09       | BCC $CB91          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cb91;
    /* $1D:CB88: 74 38       | STZ $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(0u));
    /* $1D:CB8A: 80 05       | BRA $CB91          [M=16-bit, X=16-bit] */
    goto L_cb91;
L_cb8c: /* [M=16-bit, X=16-bit] */
    /* $1D:CB8C: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB8F: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
L_cb91: /* [M=16-bit, X=16-bit] */
    /* $1D:CB91: B5 38       | LDA $38,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x38u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB93: D0 19       | BNE $CBAE          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_cbae;
    /* $1D:CB95: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CB97: 10 05       | BPL $CB9E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_cb9e;
    /* $1D:CB99: C9 FF FF    | CMP #$FFFF         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFFFu);
    /* $1D:CB9C: 90 28       | BCC $CBC6          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cbc6;
L_cb9e: /* [M=16-bit, X=16-bit] */
    /* $1D:CB9E: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:CB9F: B5 18       | LDA $18,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBA1: E9 00 10    | SBC #$1000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x1000u);
    /* $1D:CBA4: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:CBA6: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBA8: E9 00 00    | SBC #$0000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0000u);
    /* $1D:CBAB: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $1D:CBAD: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_cbae: /* [M=16-bit, X=16-bit] */
    /* $1D:CBAE: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBB0: 30 05       | BMI $CBB7          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_cbb7;
    /* $1D:CBB2: C9 01 00    | CMP #$0001         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0001u);
    /* $1D:CBB5: B0 0F       | BCS $CBC6          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_cbc6;
L_cbb7: /* [M=16-bit, X=16-bit] */
    /* $1D:CBB7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CBB8: B5 18       | LDA $18,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBBA: 69 00 10    | ADC #$1000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x1000u);
    /* $1D:CBBD: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $1D:CBBF: B5 1A       | LDA $1A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBC1: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:CBC4: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
L_cbc6: /* [M=16-bit, X=16-bit] */
    /* $1D:CBC6: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cbd1  (SNES $1D:CBD1 | Ghidra 0x1dcbd1)
 * Discovery Provenance: JSR $CBD1 @ $1D:C1C5
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cbd1(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CBD1: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBD3: 30 0C       | BMI $CBE1          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_cbe1;
    /* $1D:CBD5: C5 00       | CMP $00            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u)));
    /* $1D:CBD7: 90 08       | BCC $CBE1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cbe1;
    /* $1D:CBD9: C5 02       | CMP $02            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u)));
    /* $1D:CBDB: 90 09       | BCC $CBE6          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cbe6;
    /* $1D:CBDD: 74 3E       | STZ $3E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(0u));
    /* $1D:CBDF: 80 05       | BRA $CBE6          [M=16-bit, X=16-bit] */
    goto L_cbe6;
L_cbe1: /* [M=16-bit, X=16-bit] */
    /* $1D:CBE1: A9 01 00    | LDA #$0001         [M=16-bit, X=16-bit] */
    ctx->a = 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBE4: 95 3E       | STA $3E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(ctx->a));
L_cbe6: /* [M=16-bit, X=16-bit] */
    /* $1D:CBE6: B5 3E       | LDA $3E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x3Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBE8: D0 19       | BNE $CC03          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_cc03;
    /* $1D:CBEA: B5 1E       | LDA $1E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBEC: 10 05       | BPL $CBF3          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_cbf3;
    /* $1D:CBEE: C9 FF FF    | CMP #$FFFF         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFFFu);
    /* $1D:CBF1: 90 28       | BCC $CC1B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cc1b;
L_cbf3: /* [M=16-bit, X=16-bit] */
    /* $1D:CBF3: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $1D:CBF4: B5 1C       | LDA $1C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBF6: E9 00 18    | SBC #$1800         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x1800u);
    /* $1D:CBF9: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:CBFB: B5 1E       | LDA $1E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CBFD: E9 00 00    | SBC #$0000         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0000u);
    /* $1D:CC00: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:CC02: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_cc03: /* [M=16-bit, X=16-bit] */
    /* $1D:CC03: B5 1E       | LDA $1E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC05: 30 05       | BMI $CC0C          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_cc0c;
    /* $1D:CC07: C9 01 00    | CMP #$0001         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0001u);
    /* $1D:CC0A: B0 0F       | BCS $CC1B          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_cc1b;
L_cc0c: /* [M=16-bit, X=16-bit] */
    /* $1D:CC0C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CC0D: B5 1C       | LDA $1C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC0F: 69 00 18    | ADC #$1800         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x1800u);
    /* $1D:CC12: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $1D:CC14: B5 1E       | LDA $1E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x1Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC16: 69 00 00    | ADC #$0000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0000u);
    /* $1D:CC19: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
L_cc1b: /* [M=16-bit, X=16-bit] */
    /* $1D:CC1B: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cc1c  (SNES $1D:CC1C | Ghidra 0x1dcc1c)
 * Discovery Provenance: JSR $CC1C @ $1D:B10F
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cc1c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CC1C: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC1F: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:CC21: A9 10 00    | LDA #$0010         [M=16-bit, X=16-bit] */
    ctx->a = 0x0010u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC24: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $1D:CC26: A9 0C 4C    | LDA #$4C0C         [M=16-bit, X=16-bit] */
    ctx->a = 0x4C0Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC29: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:CC2A: 22 A3 FE 00 | JSL $00FEA3        [M=16-bit, X=16-bit] */
    { uint8_t saved_pb = ctx->pb; ctx->pb = 0x00u; bank00_func_fea3(ctx); ctx->pb = saved_pb; }
    /* $1D:CC2E: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:CC2F: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cc30  (SNES $1D:CC30 | Ghidra 0x1dcc30)
 * Discovery Provenance: JSR $CC30 @ $1D:C305
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cc30(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CC30: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CC31: AD 0A 09    | LDA $090A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Au));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC34: 69 1C 00    | ADC #$001C         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x001Cu);
    /* $1D:CC37: 99 0A 00    | STA $000A,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x000Au), (uint16_t)(ctx->a));
    /* $1D:CC3A: AD 0E 09    | LDA $090E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x090Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC3D: 99 0E 00    | STA $000E,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x000Eu), (uint16_t)(ctx->a));
    /* $1D:CC40: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC42: 29 03 00    | AND #$0003         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0003u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC45: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CC46: 69 FE FF    | ADC #$FFFE         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFFEu);
    /* $1D:CC49: 99 1A 00    | STA $001A,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x001Au), (uint16_t)(ctx->a));
    /* $1D:CC4C: A5 EA       | LDA $EA            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xEAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC4E: 99 18 00    | STA $0018,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0018u), (uint16_t)(ctx->a));
    /* $1D:CC51: A9 FE FF    | LDA #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC54: 99 1E 00    | STA $001E,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x001Eu), (uint16_t)(ctx->a));
    /* $1D:CC57: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_cc58  (SNES $1D:CC58 | Ghidra 0x1dcc58)
 * Discovery Provenance: JSR $CC58 @ $1D:BF89
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_cc58(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CC58: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $1D:CC59: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC5B: D0 08       | BNE $CC65          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_cc65;
    /* $1D:CC5D: A2 00 04    | LDX #$0400         [M=16-bit, X=16-bit] */
    ctx->x = 0x0400u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:CC60: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC63: 80 08       | BRA $CC6D          [M=16-bit, X=16-bit] */
    goto L_cc6d;
L_cc65: /* [M=16-bit, X=16-bit] */
    /* $1D:CC65: A2 00 03    | LDX #$0300         [M=16-bit, X=16-bit] */
    ctx->x = 0x0300u;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:CC68: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC6A: 29 0E 00    | AND #$000E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Eu;
    snes_set_nz16(ctx, ctx->a);
L_cc6d: /* [M=16-bit, X=16-bit] */
    /* $1D:CC6D: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
L_cc6f: /* [M=16-bit, X=16-bit] */
    /* $1D:CC6F: A9 94 00    | LDA #$0094         [M=16-bit, X=16-bit] */
    ctx->a = 0x0094u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC72: 95 10       | STA $10,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x10u), (uint16_t)(ctx->a));
    /* $1D:CC74: A9 31 40    | LDA #$4031         [M=16-bit, X=16-bit] */
    ctx->a = 0x4031u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC77: 95 16       | STA $16,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $1D:CC79: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC7C: 95 28       | STA $28,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x28u), (uint16_t)(ctx->a));
    /* $1D:CC7E: A9 F8 FF    | LDA #$FFF8         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFF8u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC81: 95 2A       | STA $2A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Au), (uint16_t)(ctx->a));
    /* $1D:CC83: A9 58 00    | LDA #$0058         [M=16-bit, X=16-bit] */
    ctx->a = 0x0058u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC86: 95 30       | STA $30,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x30u), (uint16_t)(ctx->a));
    /* $1D:CC88: 74 06       | STZ $06,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x06u), (uint16_t)(0u));
    /* $1D:CC8A: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC8C: 29 0E 00    | AND #$000E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x000Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC8F: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $1D:CC90: B9 67 FF    | LDA $FF67,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF67u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC93: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $1D:CC95: 95 3A       | STA $3A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(ctx->a));
    /* $1D:CC97: B9 77 FF    | LDA $FF77,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xFF77u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CC9A: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $1D:CC9C: 94 24       | STY $24,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x24u), (uint16_t)(ctx->y));
    /* $1D:CC9E: E6 00       | INC $00            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(v16)); }
    /* $1D:CCA0: E6 00       | INC $00            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(v16)); }
    /* $1D:CCA2: A5 84       | LDA $84            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x84u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CCA4: D0 04       | BNE $CCAA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ccaa;
    /* $1D:CCA6: E6 00       | INC $00            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(v16)); }
    /* $1D:CCA8: E6 00       | INC $00            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(v16)); }
L_ccaa: /* [M=16-bit, X=16-bit] */
    /* $1D:CCAA: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CCAC: C9 10 00    | CMP #$0010         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0010u);
    /* $1D:CCAF: 90 02       | BCC $CCB3          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ccb3;
    /* $1D:CCB1: 64 00       | STZ $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(0u));
L_ccb3: /* [M=16-bit, X=16-bit] */
    /* $1D:CCB3: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CCB4: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CCB5: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $1D:CCB8: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $1D:CCB9: C9 00 05    | CMP #$0500         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0500u);
    /* $1D:CCBC: 90 B1       | BCC $CC6F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_cc6f;
    /* $1D:CCBE: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $1D:CCBF: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ccc0  (SNES $1D:CCC0 | Ghidra 0x1dccc0)
 * Discovery Provenance: JSR $CCC0 @ $1D:BF8C
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ccc0(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CCC0: A5 E8       | LDA $E8            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE8u));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CCC2: 29 03 00    | AND #$0003         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0003u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CCC5: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CCC6: 69 05 00    | ADC #$0005         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0005u);
    /* $1D:CCC9: 89 04 00    | BIT #$0004         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0004u)) == 0u ? 1u : 0u);
    /* $1D:CCCC: D0 03       | BNE $CCD1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ccd1;
    /* $1D:CCCE: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
L_ccd1: /* [M=16-bit, X=16-bit] */
    /* $1D:CCD1: 95 3E       | STA $3E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(ctx->a));
    /* $1D:CCD3: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank1d_func_ccd4  (SNES $1D:CCD4 | Ghidra 0x1dccd4)
 * Discovery Provenance: JSR $CCD4 @ $1D:C8EB
 * REA Bank Evidence ID: ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank1d_func_ccd4(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $1D:CCD4: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $1D:CCD5: A5 EA       | LDA $EA            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xEAu));
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CCD7: 29 01 00    | AND #$0001         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $1D:CCDA: 69 F9 FF    | ADC #$FFF9         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0xFFF9u);
    /* $1D:CCDD: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $1D:CCDF: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}
