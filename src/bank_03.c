/*
 * Contra III: The Alien Wars (USA) - Statically Recompiled Bank $03
 * SNES LoROM Range : $03:8000 - $03:FFFF (32,768 bytes)
 * Ghidra Base Addr : 0x38000
 * REA Evidence ID  : ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Bank SHA-256     : 5a279c39552b0db2787ab711e9908399392a78946d5f60b4fef450874ad862ba
 * Functions        : 62
 */

#include "contra3_recomp.h"

/* ==========================================================================
 * Function: bank03_func_e180  (SNES $03:E180 | Ghidra 0x3e180)
 * Role: Mode7_StageUpdate - Bank 03 Mode 7 overhead stage mechanics & rotation handler
 * Discovery Provenance: JSL $03E180 @ $00:8441
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e180(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E180: C2 30       | REP #$30           [M=16-bit, X=16-bit] */
    /* REP #$30 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x30u);
    /* $03:E182: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E185: F0 08       | BEQ $E18F          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e18f;
    /* $03:E187: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E188: D0 05       | BNE $E18F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e18f;
    /* $03:E18A: A5 3A       | LDA $3A            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x3Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E18C: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:E18D: B0 04       | BCS $E193          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e193;
L_e18f: /* [M=16-bit, X=16-bit] */
    /* $03:E18F: 5C 18 E9 03 | JML $03E918        [M=16-bit, X=16-bit] */
    bank03_func_e918(ctx);
    return;
L_e193: /* [M=16-bit, X=16-bit] */
    /* $03:E193: 5C B2 F3 03 | JML $03F3B2        [M=16-bit, X=16-bit] */
    bank03_func_f3b2(ctx);
    return;
}

/* ==========================================================================
 * Function: bank03_func_e197  (SNES $03:E197 | Ghidra 0x3e197)
 * Discovery Provenance: JSR $E197 @ $03:EB53
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e197(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E197: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:E198: A6 DA       | LDX $DA            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xDAu));
    snes_set_nz16(ctx, ctx->x);
    /* $03:E19A: 7C 9D E1    | JMP ($E19D,X)      [M=16-bit, X=16-bit] */
    {
        uint16_t jmp_ptr = (uint16_t)(0xE19Du + ctx->x);
        uint16_t jmp_tgt = snes_bus_read16(ctx, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_ptr);
        switch (jmp_tgt) {
            case 0xE1A5u: bank03_func_e1a5(ctx); return;
            case 0xE262u: bank03_func_e262(ctx); return;
            case 0xE322u: bank03_func_e322(ctx); return;
            case 0xE3E5u: bank03_func_e3e5(ctx); return;
            default:
                // REQUIRES INTERPRETER FALLBACK
                snes_interp_fallback(ctx, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);
                return;
        }
    }
}

/* ==========================================================================
 * Function: bank03_func_e1a5  (SNES $03:E1A5 | Ghidra 0x3e1a5)
 * Discovery Provenance: JMP ($E19D,X) table @ $03:E19A
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e1a5(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E1A5: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E1A6: E5 1A       | SBC $1A            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Au)));
    /* $03:E1A8: 30 57       | BMI $E201          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e201;
    /* $03:E1AA: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E1AD: B0 29       | BCS $E1D8          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e1d8;
    /* $03:E1AF: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E1B0: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E1B1: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E1B2: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E1B3: E5 18       | SBC $18            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u)));
    /* $03:E1B5: 30 23       | BMI $E1DA          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e1da;
    /* $03:E1B7: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E1BA: B0 1C       | BCS $E1D8          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e1d8;
    /* $03:E1BC: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E1BD: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E1BE: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E1C0: 85 DD       | STA $DD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDDu), (uint16_t)(ctx->a));
    /* $03:E1C2: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E1C4: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E1C5: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E1C6: 77 D4       | ADC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E1C8: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E1C9: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E1CB: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E1CD: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E1CF: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E1D0: E5 DD       | SBC $DD            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xDDu)));
    /* $03:E1D2: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E1D3: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E1D5: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E1D7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
L_e1d8: /* [M=16-bit, X=16-bit] */
    /* $03:E1D8: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E1D9: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e1da: /* [M=16-bit, X=16-bit] */
    /* $03:E1DA: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E1DD: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E1DE: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E1E1: B0 F5       | BCS $E1D8          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e1d8;
    /* $03:E1E3: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E1E4: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E1E5: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E1E7: 85 E0       | STA $E0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xE0u), (uint16_t)(ctx->a));
    /* $03:E1E9: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E1EB: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E1EC: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E1ED: 77 D7       | ADC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E1EF: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E1F0: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E1F2: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E1F4: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E1F6: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E1F7: E5 E0       | SBC $E0            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE0u)));
    /* $03:E1F9: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E1FA: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E1FC: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E1FE: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E1FF: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E200: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e201: /* [M=16-bit, X=16-bit] */
    /* $03:E201: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E204: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E205: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E208: B0 2C       | BCS $E236          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e236;
    /* $03:E20A: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E20B: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E20C: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E20D: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E20E: E5 18       | SBC $18            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u)));
    /* $03:E210: 30 26       | BMI $E238          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e238;
    /* $03:E212: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E215: B0 1F       | BCS $E236          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e236;
    /* $03:E217: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E218: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E219: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E21B: 85 DD       | STA $DD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDDu), (uint16_t)(ctx->a));
    /* $03:E21D: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E21F: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E220: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E221: F7 D4       | SBC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E223: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E224: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E226: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E228: A5 DD       | LDA $DD            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xDDu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E22A: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E22B: 77 D7       | ADC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E22D: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E230: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E231: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E233: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E235: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
L_e236: /* [M=16-bit, X=16-bit] */
    /* $03:E236: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E237: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e238: /* [M=16-bit, X=16-bit] */
    /* $03:E238: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E23B: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E23C: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E23F: B0 F5       | BCS $E236          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e236;
    /* $03:E241: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E242: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E243: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E245: 85 E0       | STA $E0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xE0u), (uint16_t)(ctx->a));
    /* $03:E247: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E249: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E24A: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E24B: F7 D7       | SBC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E24D: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E24E: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E250: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E252: A5 E0       | LDA $E0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E254: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E255: 77 D4       | ADC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E257: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E25A: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E25B: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E25D: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E25F: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E260: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E261: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e262  (SNES $03:E262 | Ghidra 0x3e262)
 * Discovery Provenance: JMP ($E19D,X) table @ $03:E19A
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e262(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E262: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E263: E5 1A       | SBC $1A            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Au)));
    /* $03:E265: 30 5A       | BMI $E2C1          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e2c1;
    /* $03:E267: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E26A: B0 2C       | BCS $E298          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e298;
    /* $03:E26C: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E26D: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E26E: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E26F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E270: E5 18       | SBC $18            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u)));
    /* $03:E272: 30 26       | BMI $E29A          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e29a;
    /* $03:E274: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E277: B0 1F       | BCS $E298          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e298;
    /* $03:E279: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E27A: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E27B: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E27D: 85 DD       | STA $DD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDDu), (uint16_t)(ctx->a));
    /* $03:E27F: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E281: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E282: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E283: 77 D4       | ADC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E285: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E288: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E289: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E28B: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E28D: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E28F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E290: E5 DD       | SBC $DD            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xDDu)));
    /* $03:E292: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E293: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E295: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E297: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
L_e298: /* [M=16-bit, X=16-bit] */
    /* $03:E298: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E299: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e29a: /* [M=16-bit, X=16-bit] */
    /* $03:E29A: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E29D: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E29E: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E2A1: B0 F5       | BCS $E298          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e298;
    /* $03:E2A3: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E2A4: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E2A5: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2A7: 85 E0       | STA $E0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xE0u), (uint16_t)(ctx->a));
    /* $03:E2A9: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2AB: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E2AC: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E2AD: 77 D7       | ADC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E2AF: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E2B0: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E2B2: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E2B4: A5 E0       | LDA $E0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2B6: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E2B7: F7 D4       | SBC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E2B9: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E2BA: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E2BC: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E2BE: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E2BF: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E2C0: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e2c1: /* [M=16-bit, X=16-bit] */
    /* $03:E2C1: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2C4: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2C5: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E2C8: B0 2F       | BCS $E2F9          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e2f9;
    /* $03:E2CA: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E2CB: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E2CC: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2CD: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E2CE: E5 18       | SBC $18            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u)));
    /* $03:E2D0: 30 29       | BMI $E2FB          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e2fb;
    /* $03:E2D2: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E2D5: B0 22       | BCS $E2F9          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e2f9;
    /* $03:E2D7: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E2D8: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E2D9: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2DB: 85 DD       | STA $DD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDDu), (uint16_t)(ctx->a));
    /* $03:E2DD: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2DF: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E2E0: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E2E1: F7 D4       | SBC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E2E3: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2E6: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E2E7: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E2E9: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E2EB: A5 DD       | LDA $DD            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xDDu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2ED: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E2EE: 77 D7       | ADC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E2F0: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2F3: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E2F4: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E2F6: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E2F8: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
L_e2f9: /* [M=16-bit, X=16-bit] */
    /* $03:E2F9: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E2FA: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e2fb: /* [M=16-bit, X=16-bit] */
    /* $03:E2FB: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2FE: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E2FF: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E302: B0 F5       | BCS $E2F9          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e2f9;
    /* $03:E304: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E305: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E306: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E308: 85 E0       | STA $E0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xE0u), (uint16_t)(ctx->a));
    /* $03:E30A: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E30C: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E30D: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E30E: F7 D7       | SBC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E310: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E311: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E313: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E315: A5 E0       | LDA $E0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E317: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E318: 77 D4       | ADC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E31A: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E31B: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E31D: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E31F: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E320: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E321: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e322  (SNES $03:E322 | Ghidra 0x3e322)
 * Discovery Provenance: JMP ($E19D,X) table @ $03:E19A
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e322(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E322: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E323: E5 1A       | SBC $1A            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Au)));
    /* $03:E325: 30 5D       | BMI $E384          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e384;
    /* $03:E327: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E32A: B0 2C       | BCS $E358          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e358;
    /* $03:E32C: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E32D: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E32E: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E32F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E330: E5 18       | SBC $18            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u)));
    /* $03:E332: 30 26       | BMI $E35A          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e35a;
    /* $03:E334: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E337: B0 1F       | BCS $E358          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e358;
    /* $03:E339: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E33A: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E33B: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E33D: 85 DD       | STA $DD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDDu), (uint16_t)(ctx->a));
    /* $03:E33F: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E341: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E342: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E343: 77 D4       | ADC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E345: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E348: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E349: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E34B: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E34D: A5 DD       | LDA $DD            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xDDu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E34F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E350: F7 D7       | SBC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E352: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E353: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E355: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E357: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
L_e358: /* [M=16-bit, X=16-bit] */
    /* $03:E358: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E359: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e35a: /* [M=16-bit, X=16-bit] */
    /* $03:E35A: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E35D: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E35E: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E361: B0 F5       | BCS $E358          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e358;
    /* $03:E363: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E364: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E365: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E367: 85 E0       | STA $E0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xE0u), (uint16_t)(ctx->a));
    /* $03:E369: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E36B: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E36C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E36D: 77 D7       | ADC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E36F: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E372: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E373: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E375: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E377: A5 E0       | LDA $E0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E379: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E37A: F7 D4       | SBC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E37C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E37D: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E37F: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E381: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E382: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E383: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e384: /* [M=16-bit, X=16-bit] */
    /* $03:E384: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E387: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E388: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E38B: B0 2C       | BCS $E3B9          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e3b9;
    /* $03:E38D: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E38E: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E38F: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E390: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E391: E5 18       | SBC $18            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u)));
    /* $03:E393: 30 26       | BMI $E3BB          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e3bb;
    /* $03:E395: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E398: B0 1F       | BCS $E3B9          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e3b9;
    /* $03:E39A: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E39B: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E39C: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E39E: 85 DD       | STA $DD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDDu), (uint16_t)(ctx->a));
    /* $03:E3A0: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3A2: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E3A3: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E3A4: F7 D4       | SBC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E3A6: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3A9: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E3AA: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E3AC: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E3AE: A5 DD       | LDA $DD            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xDDu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3B0: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E3B1: 77 D7       | ADC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E3B3: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E3B4: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E3B6: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E3B8: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
L_e3b9: /* [M=16-bit, X=16-bit] */
    /* $03:E3B9: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E3BA: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e3bb: /* [M=16-bit, X=16-bit] */
    /* $03:E3BB: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3BE: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3BF: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E3C2: B0 F5       | BCS $E3B9          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e3b9;
    /* $03:E3C4: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E3C5: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E3C6: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3C8: 85 E0       | STA $E0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xE0u), (uint16_t)(ctx->a));
    /* $03:E3CA: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3CC: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E3CD: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E3CE: F7 D7       | SBC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E3D0: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3D3: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E3D4: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E3D6: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E3D8: A5 E0       | LDA $E0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3DA: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E3DB: 77 D4       | ADC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E3DD: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E3DE: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E3E0: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E3E2: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E3E3: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E3E4: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e3e5  (SNES $03:E3E5 | Ghidra 0x3e3e5)
 * Discovery Provenance: JMP ($E19D,X) table @ $03:E19A
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e3e5(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E3E5: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E3E6: E5 1A       | SBC $1A            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Au)));
    /* $03:E3E8: 30 5A       | BMI $E444          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e444;
    /* $03:E3EA: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E3ED: B0 29       | BCS $E418          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e418;
    /* $03:E3EF: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E3F0: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E3F1: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E3F2: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E3F3: E5 18       | SBC $18            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u)));
    /* $03:E3F5: 30 23       | BMI $E41A          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e41a;
    /* $03:E3F7: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E3FA: B0 1C       | BCS $E418          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e418;
    /* $03:E3FC: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E3FD: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E3FE: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E400: 85 DD       | STA $DD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDDu), (uint16_t)(ctx->a));
    /* $03:E402: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E404: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E405: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E406: 77 D4       | ADC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E408: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E409: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E40B: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E40D: A5 DD       | LDA $DD            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xDDu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E40F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E410: F7 D7       | SBC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E412: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E413: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E415: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E417: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
L_e418: /* [M=16-bit, X=16-bit] */
    /* $03:E418: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E419: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e41a: /* [M=16-bit, X=16-bit] */
    /* $03:E41A: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E41D: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E41E: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E421: B0 F5       | BCS $E418          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e418;
    /* $03:E423: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E424: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E425: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E427: 85 E0       | STA $E0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xE0u), (uint16_t)(ctx->a));
    /* $03:E429: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E42B: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E42C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E42D: 77 D7       | ADC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E42F: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E432: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E433: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E435: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E437: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E439: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E43A: E5 E0       | SBC $E0            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE0u)));
    /* $03:E43C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E43D: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E43F: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E441: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E442: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E443: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e444: /* [M=16-bit, X=16-bit] */
    /* $03:E444: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E447: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E448: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E44B: B0 29       | BCS $E476          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e476;
    /* $03:E44D: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E44E: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E44F: 98          | TYA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->y;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E450: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E451: E5 18       | SBC $18            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u)));
    /* $03:E453: 30 23       | BMI $E478          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_e478;
    /* $03:E455: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E458: B0 1C       | BCS $E476          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e476;
    /* $03:E45A: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E45B: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E45C: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E45E: 85 DD       | STA $DD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDDu), (uint16_t)(ctx->a));
    /* $03:E460: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E462: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E463: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E464: F7 D4       | SBC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E466: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E467: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E469: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E46B: A5 DD       | LDA $DD            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xDDu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E46D: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E46E: 77 D7       | ADC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E470: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E471: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E473: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E475: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
L_e476: /* [M=16-bit, X=16-bit] */
    /* $03:E476: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E477: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e478: /* [M=16-bit, X=16-bit] */
    /* $03:E478: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E47B: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E47C: C9 B8 00    | CMP #$00B8         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x00B8u);
    /* $03:E47F: B0 F5       | BCS $E476          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e476;
    /* $03:E481: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E482: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E483: B7 D7       | LDA [$D7],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E485: 85 E0       | STA $E0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xE0u), (uint16_t)(ctx->a));
    /* $03:E487: B7 D4       | LDA [$D4],Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E489: 9B          | TXY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->x;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E48A: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E48B: F7 D7       | SBC [$D7],Y        [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD7u)));
    /* $03:E48D: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E490: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E491: 65 1C       | ADC $1C            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Cu)));
    /* $03:E493: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E495: A5 E0       | LDA $E0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xE0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E497: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E498: 77 D4       | ADC [$D4],Y        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dpily(ctx, 0xD4u)));
    /* $03:E49A: 49 FF FF    | EOR #$FFFF         [M=16-bit, X=16-bit] */
    ctx->a ^= 0xFFFFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E49D: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E49E: 65 1E       | ADC $1E            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Eu)));
    /* $03:E4A0: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E4A2: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E4A3: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E4A4: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e4ef  (SNES $03:E4EF | Ghidra 0x3e4ef)
 * Discovery Provenance: JSR $E4EF @ $03:E4BF
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e4ef(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E4EF: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $03:E4F0: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4F2: 29 FF 03    | AND #$03FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x03FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4F5: 8D 04 42    | STA $4204          [M=16-bit, X=16-bit] */
    snes_write_cpu_io16(ctx, CPUIO_WRDIVL, (uint16_t)(ctx->a));
    /* $03:E4F8: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E4FA: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E4FC: 8D 06 42    | STA $4206          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E4FF: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E501: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E502: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E503: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E504: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E505: A9 1A 00    | LDA #$001A         [M=16-bit, X=16-bit] */
    ctx->a = 0x001Au;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E508: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E509: 6D 14 42    | ADC $4214          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_read_cpu_io16(ctx, CPUIO_RDDIVL));
    /* $03:E50C: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E50E: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E510: 29 FF 03    | AND #$03FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x03FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E513: 8D 04 42    | STA $4204          [M=16-bit, X=16-bit] */
    snes_write_cpu_io16(ctx, CPUIO_WRDIVL, (uint16_t)(ctx->a));
    /* $03:E516: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E518: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E51A: 8D 06 42    | STA $4206          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E51D: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E51F: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E520: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E521: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E522: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E523: A9 0A 00    | LDA #$000A         [M=16-bit, X=16-bit] */
    ctx->a = 0x000Au;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E526: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E527: 6D 14 42    | ADC $4214          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_read_cpu_io16(ctx, CPUIO_RDDIVL));
    /* $03:E52A: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E52C: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E52E: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E52F: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:E532: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:E533: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:E534: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E537: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E538: B9 48 E7    | LDA $E748,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xE748u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E53B: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $03:E53D: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E53F: 29 00 0E    | AND #$0E00         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0E00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E542: 09 00 31    | ORA #$3100         [M=16-bit, X=16-bit] */
    ctx->a |= 0x3100u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E545: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:E547: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E548: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e549  (SNES $03:E549 | Ghidra 0x3e549)
 * Discovery Provenance: JSR $E549 @ $03:E4C2
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e549(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E549: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E54B: 29 FF 03    | AND #$03FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x03FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E54E: C9 00 02    | CMP #$0200         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0200u);
    /* $03:E551: B0 0B       | BCS $E55E          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e55e;
    /* $03:E553: A9 2A 00    | LDA #$002A         [M=16-bit, X=16-bit] */
    ctx->a = 0x002Au;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E556: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $03:E558: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E55B: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $03:E55D: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e55e: /* [M=16-bit, X=16-bit] */
    /* $03:E55E: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E561: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $03:E563: A9 E8 FF    | LDA #$FFE8         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFE8u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E566: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $03:E568: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e569  (SNES $03:E569 | Ghidra 0x3e569)
 * Discovery Provenance: JSR $E569 @ $03:E4D6
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e569(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E569: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E56B: 29 FF 03    | AND #$03FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x03FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E56E: C9 00 02    | CMP #$0200         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0200u);
    /* $03:E571: B0 0B       | BCS $E57E          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e57e;
    /* $03:E573: A9 2C 00    | LDA #$002C         [M=16-bit, X=16-bit] */
    ctx->a = 0x002Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E576: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $03:E578: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E57B: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $03:E57D: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e57e: /* [M=16-bit, X=16-bit] */
    /* $03:E57E: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E581: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
    /* $03:E583: A9 E8 FF    | LDA #$FFE8         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFE8u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E586: 85 0A       | STA $0A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x0Au), (uint16_t)(ctx->a));
    /* $03:E588: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e589  (SNES $03:E589 | Ghidra 0x3e589)
 * Discovery Provenance: JSR $E589 @ $03:E4C5
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e589(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E589: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E58B: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E58C: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:E58F: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E592: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E594: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E595: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:E598: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E59A: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E59D: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E59F: A5 04       | LDA $04            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5A1: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5A3: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E5A6: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5A7: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5A8: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5A9: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5AA: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5AC: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E5AD: 65 0A       | ADC $0A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $03:E5AF: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E5B2: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5B4: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E5B5: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:E5B8: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E5BA: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E5BD: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E5BF: A5 08       | LDA $08            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5C1: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5C3: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E5C6: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5C7: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5C8: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5C9: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5CA: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e5cb  (SNES $03:E5CB | Ghidra 0x3e5cb)
 * Discovery Provenance: JSR $E5CB @ $03:E4D9
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e5cb(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E5CB: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5CD: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E5CE: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:E5D1: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E5D4: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5D6: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E5D7: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:E5DA: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E5DC: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E5DF: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E5E1: A5 04       | LDA $04            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5E3: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5E5: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E5E8: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5E9: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5EA: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5EB: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E5EC: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5EE: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E5EF: 65 0A       | ADC $0A            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Au)));
    /* $03:E5F1: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E5F4: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E5F6: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E5F7: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:E5FA: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E5FC: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E5FF: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E601: A5 08       | LDA $08            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E603: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E605: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E608: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E609: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E60A: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E60B: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E60C: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e60d  (SNES $03:E60D | Ghidra 0x3e60d)
 * Discovery Provenance: JSR $E60D @ $03:E4DC
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e60d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E60D: 20 F3 E8    | JSR $E8F3          [M=16-bit, X=16-bit] */
    bank03_func_e8f3(ctx);
    /* $03:E610: A2 C0 0A    | LDX #$0AC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0AC0u;
    snes_set_nz16(ctx, ctx->x);
L_e613: /* [M=16-bit, X=16-bit] */
    /* $03:E613: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E615: F0 3F       | BEQ $E656          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e656;
    /* $03:E617: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E619: 29 FF 03    | AND #$03FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x03FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E61C: 8D 04 42    | STA $4204          [M=16-bit, X=16-bit] */
    snes_write_cpu_io16(ctx, CPUIO_WRDIVL, (uint16_t)(ctx->a));
    /* $03:E61F: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E621: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E623: 8D 06 42    | STA $4206          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E626: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E628: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E629: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E62A: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E62B: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E62C: A9 1A 00    | LDA #$001A         [M=16-bit, X=16-bit] */
    ctx->a = 0x001Au;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E62F: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E630: 6D 14 42    | ADC $4214          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_read_cpu_io16(ctx, CPUIO_RDDIVL));
    /* $03:E633: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E635: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E637: 29 FF 03    | AND #$03FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x03FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E63A: 8D 04 42    | STA $4204          [M=16-bit, X=16-bit] */
    snes_write_cpu_io16(ctx, CPUIO_WRDIVL, (uint16_t)(ctx->a));
    /* $03:E63D: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E63F: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E641: 8D 06 42    | STA $4206          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E644: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E646: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E647: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E648: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E649: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E64A: A9 0A 00    | LDA #$000A         [M=16-bit, X=16-bit] */
    ctx->a = 0x000Au;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E64D: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E64E: 6D 14 42    | ADC $4214          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_read_cpu_io16(ctx, CPUIO_RDDIVL));
    /* $03:E651: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E653: 20 62 E6    | JSR $E662          [M=16-bit, X=16-bit] */
    bank03_func_e662(ctx);
L_e656: /* [M=16-bit, X=16-bit] */
    /* $03:E656: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E657: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E658: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:E65B: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E65C: E0 40 0D    | CPX #$0D40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0D40u);
    /* $03:E65F: 90 B2       | BCC $E613          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_e613;
    /* $03:E661: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e662  (SNES $03:E662 | Ghidra 0x3e662)
 * Discovery Provenance: JSR $E662 @ $03:E653
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e662(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E662: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E664: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E665: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:E668: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E66B: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E66D: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E66E: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:E671: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E673: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E676: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E678: A5 06       | LDA $06            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E67A: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E67D: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E67E: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E67F: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E680: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E681: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e6c2  (SNES $03:E6C2 | Ghidra 0x3e6c2)
 * Discovery Provenance: JSR $E6C2 @ $03:E6A9
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e6c2(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E6C2: AD 8A 1F    | LDA $1F8A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1F8Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6C5: D0 03       | BNE $E6CA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e6ca;
    /* $03:E6C7: 82 D0 00    | BRL $E79A          [M=16-bit, X=16-bit] */
    goto L_e79a;
L_e6ca: /* [M=16-bit, X=16-bit] */
    /* $03:E6CA: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $03:E6CB: AD 26 15    | LDA $1526          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1526u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6CE: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E6CF: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6D0: B9 88 E7    | LDA $E788,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xE788u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6D3: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E6D5: AD 28 15    | LDA $1528          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1528u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6D8: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E6D9: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6DA: B9 90 E7    | LDA $E790,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xE790u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6DD: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E6DF: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6E0: A9 00 39    | LDA #$3900         [M=16-bit, X=16-bit] */
    ctx->a = 0x3900u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6E3: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:E6E5: AD 2A 15    | LDA $152A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x152Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6E8: F0 0A       | BEQ $E6F4          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e6f4;
    /* $03:E6EA: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6EC: 89 04 00    | BIT #$0004         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0004u)) == 0u ? 1u : 0u);
    /* $03:E6EF: F0 03       | BEQ $E6F4          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e6f4;
    /* $03:E6F1: 82 A6 00    | BRL $E79A          [M=16-bit, X=16-bit] */
    goto L_e79a;
L_e6f4: /* [M=16-bit, X=16-bit] */
    /* $03:E6F4: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6F6: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E6F7: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:E6FA: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E6FD: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E6FF: A5 00       | LDA $00            [M=8-bit, X=16-bit] */
    { uint8_t v8 = snes_bus_read8(ctx, snes_addr_dp(ctx, 0x00u)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E701: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E704: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E706: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E709: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E70B: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E70E: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E70F: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E710: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E711: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E712: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E714: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E715: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
    /* $03:E718: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E71B: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E71D: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E71E: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
    /* $03:E721: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E723: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E726: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E728: A9 26 00    | LDA #$0026         [M=16-bit, X=16-bit] */
    ctx->a = 0x0026u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E72B: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E72D: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E730: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E731: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E732: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E733: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E734: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E736: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E737: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
    /* $03:E73A: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E73D: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E73F: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E740: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $03:E743: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E745: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E748: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E74A: A9 26 40    | LDA #$4026         [M=16-bit, X=16-bit] */
    ctx->a = 0x4026u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E74D: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E74F: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E752: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E753: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E754: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E755: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E756: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E758: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E759: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $03:E75C: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E75F: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E761: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E762: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
    /* $03:E765: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E767: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E76A: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E76C: A9 26 80    | LDA #$8026         [M=16-bit, X=16-bit] */
    ctx->a = 0x8026u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E76F: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E771: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E774: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E775: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E776: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E777: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E778: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E77A: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E77B: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $03:E77E: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E781: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E783: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E784: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $03:E787: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E789: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E78C: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E78E: A9 26 C0    | LDA #$C026         [M=16-bit, X=16-bit] */
    ctx->a = 0xC026u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E791: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E793: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E796: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E797: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E798: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E799: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
L_e79a: /* [M=16-bit, X=16-bit] */
    /* $03:E79A: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e79b  (SNES $03:E79B | Ghidra 0x3e79b)
 * Discovery Provenance: JSR $E79B @ $03:E6AC
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e79b(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E79B: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E79E: D0 03       | BNE $E7A3          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e7a3;
    /* $03:E7A0: 82 D8 00    | BRL $E87B          [M=16-bit, X=16-bit] */
    goto L_e87b;
L_e7a3: /* [M=16-bit, X=16-bit] */
    /* $03:E7A3: AD CA 1F    | LDA $1FCA          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1FCAu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7A6: D0 03       | BNE $E7AB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e7ab;
    /* $03:E7A8: 82 D0 00    | BRL $E87B          [M=16-bit, X=16-bit] */
    goto L_e87b;
L_e7ab: /* [M=16-bit, X=16-bit] */
    /* $03:E7AB: 5A          | PHY                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->y);
    /* $03:E7AC: AD 66 15    | LDA $1566          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1566u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7AF: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E7B0: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E7B1: B9 88 E7    | LDA $E788,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xE788u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7B4: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E7B6: AD 68 15    | LDA $1568          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1568u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7B9: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:E7BA: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E7BB: B9 90 E7    | LDA $E790,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0xE790u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7BE: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E7C0: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E7C1: A9 00 3B    | LDA #$3B00         [M=16-bit, X=16-bit] */
    ctx->a = 0x3B00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7C4: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:E7C6: AD 6A 15    | LDA $156A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x156Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7C9: F0 0A       | BEQ $E7D5          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e7d5;
    /* $03:E7CB: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7CD: 89 04 00    | BIT #$0004         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0004u)) == 0u ? 1u : 0u);
    /* $03:E7D0: D0 03       | BNE $E7D5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e7d5;
    /* $03:E7D2: 82 A6 00    | BRL $E87B          [M=16-bit, X=16-bit] */
    goto L_e87b;
L_e7d5: /* [M=16-bit, X=16-bit] */
    /* $03:E7D5: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7D7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E7D8: 69 08 00    | ADC #$0008         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0008u);
    /* $03:E7DB: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E7DE: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7E0: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E7E2: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E7E5: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E7E7: A9 2C 00    | LDA #$002C         [M=16-bit, X=16-bit] */
    ctx->a = 0x002Cu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7EA: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7EC: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E7EF: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E7F0: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E7F1: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E7F2: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E7F3: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7F5: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E7F6: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
    /* $03:E7F9: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E7FC: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E7FE: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E7FF: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
    /* $03:E802: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E804: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E807: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E809: A9 26 00    | LDA #$0026         [M=16-bit, X=16-bit] */
    ctx->a = 0x0026u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E80C: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E80E: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E811: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E812: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E813: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E814: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E815: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E817: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E818: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
    /* $03:E81B: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E81E: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E820: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E821: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $03:E824: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E826: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E829: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E82B: A9 26 40    | LDA #$4026         [M=16-bit, X=16-bit] */
    ctx->a = 0x4026u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E82E: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E830: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E833: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E834: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E835: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E836: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E837: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E839: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E83A: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $03:E83D: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E840: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E842: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E843: E9 10 00    | SBC #$0010         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0010u);
    /* $03:E846: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E848: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E84B: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E84D: A9 26 80    | LDA #$8026         [M=16-bit, X=16-bit] */
    ctx->a = 0x8026u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E850: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E852: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E855: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E856: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E857: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E858: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E859: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E85B: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E85C: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $03:E85F: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E862: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E864: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E865: 69 10 00    | ADC #$0010         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0010u);
    /* $03:E868: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E86A: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E86D: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E86F: A9 26 C0    | LDA #$C026         [M=16-bit, X=16-bit] */
    ctx->a = 0xC026u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E872: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E874: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E877: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E878: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E879: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E87A: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
L_e87b: /* [M=16-bit, X=16-bit] */
    /* $03:E87B: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e87c  (SNES $03:E87C | Ghidra 0x3e87c)
 * Discovery Provenance: JSR $E87C @ $03:E6A6
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e87c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E87C: A2 14 00    | LDX #$0014         [M=16-bit, X=16-bit] */
    ctx->x = 0x0014u;
    snes_set_nz16(ctx, ctx->x);
L_e87f: /* [M=16-bit, X=16-bit] */
    /* $03:E87F: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E882: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:E885: BD 96 E7    | LDA $E796,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0xE796u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E888: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E88A: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E88D: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E88F: BD AA E7    | LDA $E7AA,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0xE7AAu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E892: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:E895: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E896: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E897: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E898: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E899: CA          | DEX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x - 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E89A: CA          | DEX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x - 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:E89B: D0 E2       | BNE $E87F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e87f;
    /* $03:E89D: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e89e  (SNES $03:E89E | Ghidra 0x3e89e)
 * Discovery Provenance: JSR $E89E @ $03:E6AF
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e89e(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E89E: 20 F3 E8    | JSR $E8F3          [M=16-bit, X=16-bit] */
    bank03_func_e8f3(ctx);
    /* $03:E8A1: A2 C0 0A    | LDX #$0AC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0AC0u;
    snes_set_nz16(ctx, ctx->x);
L_e8a4: /* [M=16-bit, X=16-bit] */
    /* $03:E8A4: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E8A6: F0 3F       | BEQ $E8E7          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e8e7;
    /* $03:E8A8: B5 0A       | LDA $0A,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E8AA: 29 FF 03    | AND #$03FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x03FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E8AD: 8D 04 42    | STA $4204          [M=16-bit, X=16-bit] */
    snes_write_cpu_io16(ctx, CPUIO_WRDIVL, (uint16_t)(ctx->a));
    /* $03:E8B0: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E8B2: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E8B4: 8D 06 42    | STA $4206          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E8B7: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E8B9: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E8BA: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E8BB: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E8BC: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E8BD: A9 1A 00    | LDA #$001A         [M=16-bit, X=16-bit] */
    ctx->a = 0x001Au;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E8C0: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E8C1: 6D 14 42    | ADC $4214          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_read_cpu_io16(ctx, CPUIO_RDDIVL));
    /* $03:E8C4: 85 00       | STA $00            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:E8C6: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E8C8: 29 FF 03    | AND #$03FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x03FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E8CB: 8D 04 42    | STA $4204          [M=16-bit, X=16-bit] */
    snes_write_cpu_io16(ctx, CPUIO_WRDIVL, (uint16_t)(ctx->a));
    /* $03:E8CE: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E8D0: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E8D2: 8D 06 42    | STA $4206          [M=8-bit, X=16-bit] */
    snes_write_cpu_io(ctx, CPUIO_WRDIVB, (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:E8D5: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E8D7: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E8D8: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E8D9: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E8DA: EA          | NOP                [M=16-bit, X=16-bit] */
    /* NOP */
    /* $03:E8DB: A9 1A 00    | LDA #$001A         [M=16-bit, X=16-bit] */
    ctx->a = 0x001Au;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E8DE: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E8DF: 6D 14 42    | ADC $4214          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_read_cpu_io16(ctx, CPUIO_RDDIVL));
    /* $03:E8E2: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:E8E4: 20 62 E6    | JSR $E662          [M=16-bit, X=16-bit] */
    bank03_func_e662(ctx);
L_e8e7: /* [M=16-bit, X=16-bit] */
    /* $03:E8E7: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E8E8: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E8E9: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:E8EC: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E8ED: E0 40 0D    | CPX #$0D40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0D40u);
    /* $03:E8F0: 90 B2       | BCC $E8A4          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_e8a4;
    /* $03:E8F2: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e8f3  (SNES $03:E8F3 | Ghidra 0x3e8f3)
 * Discovery Provenance: JSR $E8F3 @ $03:E60D
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e8f3(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E8F3: A2 00 00    | LDX #$0000         [M=16-bit, X=16-bit] */
    ctx->x = 0x0000u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E8F6: A5 78       | LDA $78            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x78u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E8F8: 89 03 00    | BIT #$0003         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0003u)) == 0u ? 1u : 0u);
    /* $03:E8FB: D0 03       | BNE $E900          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e900;
    /* $03:E8FD: A2 02 00    | LDX #$0002         [M=16-bit, X=16-bit] */
    ctx->x = 0x0002u;
    snes_set_nz16(ctx, ctx->x);
L_e900: /* [M=16-bit, X=16-bit] */
    /* $03:E900: 86 FA       | STX $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->x));
    /* $03:E902: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E905: A6 86       | LDX $86            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x86u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:E907: E0 02 00    | CPX #$0002         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0002u);
    /* $03:E90A: F0 03       | BEQ $E90F          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e90f;
    /* $03:E90C: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
L_e90f: /* [M=16-bit, X=16-bit] */
    /* $03:E90F: 05 FA       | ORA $FA            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E911: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E912: BD C0 E7    | LDA $E7C0,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0xE7C0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E915: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:E917: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e918  (SNES $03:E918 | Ghidra 0x3e918)
 * Discovery Provenance: JML $03E918 @ $03:E18F
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e918(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    goto L_e918;
L_e4a5: /* [M=16-bit, X=16-bit] */
    /* $03:E4A5: A0 C0 0F    | LDY #$0FC0         [M=16-bit, X=16-bit] */
    ctx->y = 0x0FC0u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4A8: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4AB: 8D C0 11    | STA $11C0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C0u), (uint16_t)(ctx->a));
    /* $03:E4AE: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4B1: 8D C2 11    | STA $11C2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C2u), (uint16_t)(ctx->a));
    /* $03:E4B4: AD 8A 1F    | LDA $1F8A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1F8Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4B7: D0 03       | BNE $E4BC          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e4bc;
    /* $03:E4B9: 82 0C 00    | BRL $E4C8          [M=16-bit, X=16-bit] */
    goto L_e4c8;
L_e4bc: /* [M=16-bit, X=16-bit] */
    /* $03:E4BC: A2 C0 0E    | LDX #$0EC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0EC0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E4BF: 20 EF E4    | JSR $E4EF          [M=16-bit, X=16-bit] */
    bank03_func_e4ef(ctx);
    /* $03:E4C2: 20 49 E5    | JSR $E549          [M=16-bit, X=16-bit] */
    bank03_func_e549(ctx);
    /* $03:E4C5: 20 89 E5    | JSR $E589          [M=16-bit, X=16-bit] */
    bank03_func_e589(ctx);
L_e4c8: /* [M=16-bit, X=16-bit] */
    /* $03:E4C8: AD CA 1F    | LDA $1FCA          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1FCAu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4CB: D0 03       | BNE $E4D0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e4d0;
    /* $03:E4CD: 82 09 00    | BRL $E4D9          [M=16-bit, X=16-bit] */
    goto L_e4d9;
L_e4d0: /* [M=16-bit, X=16-bit] */
    /* $03:E4D0: A2 00 0F    | LDX #$0F00         [M=16-bit, X=16-bit] */
    ctx->x = 0x0F00u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E4D3: 20 EF E4    | JSR $E4EF          [M=16-bit, X=16-bit] */
    bank03_func_e4ef(ctx);
    /* $03:E4D6: 20 69 E5    | JSR $E569          [M=16-bit, X=16-bit] */
    bank03_func_e569(ctx);
L_e4d9: /* [M=16-bit, X=16-bit] */
    /* $03:E4D9: 20 CB E5    | JSR $E5CB          [M=16-bit, X=16-bit] */
    bank03_func_e5cb(ctx);
    /* $03:E4DC: 20 0D E6    | JSR $E60D          [M=16-bit, X=16-bit] */
    bank03_func_e60d(ctx);
    /* $03:E4DF: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_e4e2: /* [M=16-bit, X=16-bit] */
    /* $03:E4E2: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:E4E5: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4E6: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4E7: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4E8: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4E9: C0 C0 11    | CPY #$11C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->y, 0x11C0u);
    /* $03:E4EC: 90 F4       | BCC $E4E2          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_e4e2;
    /* $03:E4EE: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_e682: /* [M=16-bit, X=16-bit] */
    /* $03:E682: A0 C0 0F    | LDY #$0FC0         [M=16-bit, X=16-bit] */
    ctx->y = 0x0FC0u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E685: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E688: 8D C0 11    | STA $11C0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C0u), (uint16_t)(ctx->a));
    /* $03:E68B: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E68E: 8D C2 11    | STA $11C2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C2u), (uint16_t)(ctx->a));
    /* $03:E691: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E694: 8D C4 11    | STA $11C4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C4u), (uint16_t)(ctx->a));
    /* $03:E697: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E69A: 8D C6 11    | STA $11C6          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C6u), (uint16_t)(ctx->a));
    /* $03:E69D: AD 8E 15    | LDA $158E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x158Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6A0: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6A1: F0 03       | BEQ $E6A6          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e6a6;
    /* $03:E6A3: 82 0C 00    | BRL $E6B2          [M=16-bit, X=16-bit] */
    goto L_e6b2;
L_e6a6: /* [M=16-bit, X=16-bit] */
    /* $03:E6A6: 20 7C E8    | JSR $E87C          [M=16-bit, X=16-bit] */
    bank03_func_e87c(ctx);
    /* $03:E6A9: 20 C2 E6    | JSR $E6C2          [M=16-bit, X=16-bit] */
    bank03_func_e6c2(ctx);
    /* $03:E6AC: 20 9B E7    | JSR $E79B          [M=16-bit, X=16-bit] */
    bank03_func_e79b(ctx);
    /* $03:E6AF: 20 9E E8    | JSR $E89E          [M=16-bit, X=16-bit] */
    bank03_func_e89e(ctx);
L_e6b2: /* [M=16-bit, X=16-bit] */
    /* $03:E6B2: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_e6b5: /* [M=16-bit, X=16-bit] */
    /* $03:E6B5: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:E6B8: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6B9: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6BA: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6BB: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6BC: C0 C0 11    | CPY #$11C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->y, 0x11C0u);
    /* $03:E6BF: 90 F4       | BCC $E6B5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_e6b5;
    /* $03:E6C1: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_e918: /* [M=16-bit, X=16-bit] */
    /* $03:E918: C2 30       | REP #$30           [M=16-bit, X=16-bit] */
    /* REP #$30 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x30u);
    /* $03:E91A: AD 94 15    | LDA $1594          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1594u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E91D: C9 02 00    | CMP #$0002         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0002u);
    /* $03:E920: F0 4C       | BEQ $E96E          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e96e;
    /* $03:E922: C9 06 00    | CMP #$0006         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0006u);
    /* $03:E925: D0 03       | BNE $E92A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e92a;
    /* $03:E927: 82 76 00    | BRL $E9A0          [M=16-bit, X=16-bit] */
    goto L_e9a0;
L_e92a: /* [M=16-bit, X=16-bit] */
    /* $03:E92A: AD 8E 15    | LDA $158E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x158Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E92D: F0 03       | BEQ $E932          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e932;
    /* $03:E92F: 82 50 FD    | BRL $E682          [M=16-bit, X=16-bit] */
    goto L_e682;
L_e932: /* [M=16-bit, X=16-bit] */
    /* $03:E932: AD 90 15    | LDA $1590          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1590u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E935: F0 03       | BEQ $E93A          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e93a;
    /* $03:E937: 82 6B FB    | BRL $E4A5          [M=16-bit, X=16-bit] */
    goto L_e4a5;
L_e93a: /* [M=16-bit, X=16-bit] */
    /* $03:E93A: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E93C: A9 07       | LDA #$07           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x07u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E93E: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:E93F: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:E940: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E942: 20 E8 E9    | JSR $E9E8          [M=16-bit, X=16-bit] */
    bank03_func_e9e8(ctx);
    /* $03:E945: 20 78 EA    | JSR $EA78          [M=16-bit, X=16-bit] */
    bank03_func_ea78(ctx);
    /* $03:E948: 20 A9 EA    | JSR $EAA9          [M=16-bit, X=16-bit] */
    bank03_func_eaa9(ctx);
    /* $03:E94B: 20 D7 E9    | JSR $E9D7          [M=16-bit, X=16-bit] */
    bank03_func_e9d7(ctx);
    /* $03:E94E: B0 06       | BCS $E956          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e956;
    /* $03:E950: 20 BF EA    | JSR $EABF          [M=16-bit, X=16-bit] */
    bank03_func_eabf(ctx);
    /* $03:E953: 20 48 EB    | JSR $EB48          [M=16-bit, X=16-bit] */
    bank03_func_eb48(ctx);
L_e956: /* [M=16-bit, X=16-bit] */
    /* $03:E956: 20 72 EB    | JSR $EB72          [M=16-bit, X=16-bit] */
    bank03_func_eb72(ctx);
    /* $03:E959: 20 8F EB    | JSR $EB8F          [M=16-bit, X=16-bit] */
    bank03_func_eb8f(ctx);
    /* $03:E95C: 20 63 EC    | JSR $EC63          [M=16-bit, X=16-bit] */
    bank03_func_ec63(ctx);
    /* $03:E95F: 20 F7 F0    | JSR $F0F7          [M=16-bit, X=16-bit] */
    bank03_func_f0f7(ctx);
    /* $03:E962: 20 61 F1    | JSR $F161          [M=16-bit, X=16-bit] */
    bank03_func_f161(ctx);
    /* $03:E965: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E967: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E969: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:E96A: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:E96B: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E96D: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_e96e: /* [M=16-bit, X=16-bit] */
    /* $03:E96E: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E970: A9 07       | LDA #$07           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x07u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E972: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:E973: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:E974: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E976: 20 E8 E9    | JSR $E9E8          [M=16-bit, X=16-bit] */
    bank03_func_e9e8(ctx);
    /* $03:E979: 20 78 EA    | JSR $EA78          [M=16-bit, X=16-bit] */
    bank03_func_ea78(ctx);
    /* $03:E97C: 20 A9 EA    | JSR $EAA9          [M=16-bit, X=16-bit] */
    bank03_func_eaa9(ctx);
    /* $03:E97F: 20 BF EA    | JSR $EABF          [M=16-bit, X=16-bit] */
    bank03_func_eabf(ctx);
    /* $03:E982: 20 48 EB    | JSR $EB48          [M=16-bit, X=16-bit] */
    bank03_func_eb48(ctx);
    /* $03:E985: 20 72 EB    | JSR $EB72          [M=16-bit, X=16-bit] */
    bank03_func_eb72(ctx);
    /* $03:E988: 20 33 ED    | JSR $ED33          [M=16-bit, X=16-bit] */
    bank03_func_ed33(ctx);
    /* $03:E98B: 20 F6 ED    | JSR $EDF6          [M=16-bit, X=16-bit] */
    bank03_func_edf6(ctx);
    /* $03:E98E: 20 AA EE    | JSR $EEAA          [M=16-bit, X=16-bit] */
    bank03_func_eeaa(ctx);
    /* $03:E991: 20 F7 F0    | JSR $F0F7          [M=16-bit, X=16-bit] */
    bank03_func_f0f7(ctx);
    /* $03:E994: 20 61 F1    | JSR $F161          [M=16-bit, X=16-bit] */
    bank03_func_f161(ctx);
    /* $03:E997: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E999: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E99B: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:E99C: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:E99D: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E99F: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_e9a0: /* [M=16-bit, X=16-bit] */
    /* $03:E9A0: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E9A2: A9 07       | LDA #$07           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x07u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E9A4: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:E9A5: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:E9A6: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E9A8: 20 E8 E9    | JSR $E9E8          [M=16-bit, X=16-bit] */
    bank03_func_e9e8(ctx);
    /* $03:E9AB: 20 78 EA    | JSR $EA78          [M=16-bit, X=16-bit] */
    bank03_func_ea78(ctx);
    /* $03:E9AE: 20 A9 EA    | JSR $EAA9          [M=16-bit, X=16-bit] */
    bank03_func_eaa9(ctx);
    /* $03:E9B1: 20 D7 E9    | JSR $E9D7          [M=16-bit, X=16-bit] */
    bank03_func_e9d7(ctx);
    /* $03:E9B4: B0 06       | BCS $E9BC          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_e9bc;
    /* $03:E9B6: 20 BF EA    | JSR $EABF          [M=16-bit, X=16-bit] */
    bank03_func_eabf(ctx);
    /* $03:E9B9: 20 48 EB    | JSR $EB48          [M=16-bit, X=16-bit] */
    bank03_func_eb48(ctx);
L_e9bc: /* [M=16-bit, X=16-bit] */
    /* $03:E9BC: 20 72 EB    | JSR $EB72          [M=16-bit, X=16-bit] */
    bank03_func_eb72(ctx);
    /* $03:E9BF: 20 B0 F1    | JSR $F1B0          [M=16-bit, X=16-bit] */
    bank03_func_f1b0(ctx);
    /* $03:E9C2: 20 02 F2    | JSR $F202          [M=16-bit, X=16-bit] */
    bank03_func_f202(ctx);
    /* $03:E9C5: 20 E3 F2    | JSR $F2E3          [M=16-bit, X=16-bit] */
    bank03_func_f2e3(ctx);
    /* $03:E9C8: 20 63 EC    | JSR $EC63          [M=16-bit, X=16-bit] */
    bank03_func_ec63(ctx);
    /* $03:E9CB: 20 61 F1    | JSR $F161          [M=16-bit, X=16-bit] */
    bank03_func_f161(ctx);
    /* $03:E9CE: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:E9D0: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:E9D2: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:E9D3: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:E9D4: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:E9D6: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e9d7  (SNES $03:E9D7 | Ghidra 0x3e9d7)
 * Discovery Provenance: JSR $E9D7 @ $03:E9B1
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e9d7(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E9D7: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E9DA: F0 03       | BEQ $E9DF          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e9df;
    /* $03:E9DC: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E9DD: D0 07       | BNE $E9E6          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e9e6;
L_e9df: /* [M=16-bit, X=16-bit] */
    /* $03:E9DF: AD EC 0E    | LDA $0EEC          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0EECu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E9E2: F0 02       | BEQ $E9E6          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e9e6;
    /* $03:E9E4: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:E9E5: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_e9e6: /* [M=16-bit, X=16-bit] */
    /* $03:E9E6: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:E9E7: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_e9e8  (SNES $03:E9E8 | Ghidra 0x3e9e8)
 * Discovery Provenance: JSR $E9E8 @ $03:E976
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_e9e8(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:E9E8: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E9EB: F0 03       | BEQ $E9F0          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e9f0;
    /* $03:E9ED: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E9EE: F0 27       | BEQ $EA17          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ea17;
L_e9f0: /* [M=16-bit, X=16-bit] */
    /* $03:E9F0: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E9F3: 8D 0C 00    | STA $000C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x000Cu), (uint16_t)(ctx->a));
    /* $03:E9F6: A9 E0 00    | LDA #$00E0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00E0u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E9F9: 8D 0E 00    | STA $000E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x000Eu), (uint16_t)(ctx->a));
    /* $03:E9FC: AD 0C 15    | LDA $150C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x150Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E9FF: 85 1C       | STA $1C            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $03:EA01: AD 0E 15    | LDA $150E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x150Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA04: 85 1E       | STA $1E            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $03:EA06: AD 10 15    | LDA $1510          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1510u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA09: 85 18       | STA $18            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $03:EA0B: AD 12 15    | LDA $1512          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1512u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA0E: 85 1A       | STA $1A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $03:EA10: AD 14 15    | LDA $1514          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1514u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA13: 85 16       | STA $16            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $03:EA15: 80 25       | BRA $EA3C          [M=16-bit, X=16-bit] */
    goto L_ea3c;
L_ea17: /* [M=16-bit, X=16-bit] */
    /* $03:EA17: A9 F0 FF    | LDA #$FFF0         [M=16-bit, X=16-bit] */
    ctx->a = 0xFFF0u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA1A: 8D 0C 00    | STA $000C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x000Cu), (uint16_t)(ctx->a));
    /* $03:EA1D: A9 68 00    | LDA #$0068         [M=16-bit, X=16-bit] */
    ctx->a = 0x0068u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA20: 8D 0E 00    | STA $000E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x000Eu), (uint16_t)(ctx->a));
    /* $03:EA23: AD 0C 15    | LDA $150C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x150Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA26: 85 1C       | STA $1C            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $03:EA28: AD 0E 15    | LDA $150E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x150Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA2B: 85 1E       | STA $1E            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $03:EA2D: AD 10 15    | LDA $1510          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1510u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA30: 85 18       | STA $18            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $03:EA32: AD 12 15    | LDA $1512          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1512u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA35: 85 1A       | STA $1A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $03:EA37: AD 14 15    | LDA $1514          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1514u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA3A: 85 16       | STA $16            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x16u), (uint16_t)(ctx->a));
L_ea3c: /* [M=16-bit, X=16-bit] */
    /* $03:EA3C: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:EA3D: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:EA3E: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:EA3F: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:EA40: 29 06 00    | AND #$0006         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0006u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA43: 85 DA       | STA $DA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDAu), (uint16_t)(ctx->a));
    /* $03:EA45: A5 16       | LDA $16            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA47: 29 3F 00    | AND #$003F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA4A: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:EA4B: 85 D4       | STA $D4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD4u), (uint16_t)(ctx->a));
    /* $03:EA4D: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EA4E: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EA4F: 65 D4       | ADC $D4            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD4u)));
    /* $03:EA51: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EA52: 69 00 80    | ADC #$8000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x8000u);
    /* $03:EA55: 85 D4       | STA $D4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD4u), (uint16_t)(ctx->a));
    /* $03:EA57: A5 16       | LDA $16            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA59: 29 3F 00    | AND #$003F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA5C: 49 3F 00    | EOR #$003F         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA5F: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA60: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:EA61: 85 D7       | STA $D7            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD7u), (uint16_t)(ctx->a));
    /* $03:EA63: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EA64: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EA65: 65 D7       | ADC $D7            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD7u)));
    /* $03:EA67: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EA68: 69 00 80    | ADC #$8000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x8000u);
    /* $03:EA6B: 85 D7       | STA $D7            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD7u), (uint16_t)(ctx->a));
    /* $03:EA6D: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:EA6F: A9 03       | LDA #$03           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x03u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:EA71: 85 D6       | STA $D6            [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0xD6u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:EA73: 85 D9       | STA $D9            [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0xD9u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:EA75: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:EA77: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_ea78  (SNES $03:EA78 | Ghidra 0x3ea78)
 * Discovery Provenance: JSR $EA78 @ $03:E979
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_ea78(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EA78: AD 0A 02    | LDA $020A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x020Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA7B: 8D C0 0F    | STA $0FC0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FC0u), (uint16_t)(ctx->a));
    /* $03:EA7E: AD 0E 02    | LDA $020E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x020Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA81: 8D C1 0F    | STA $0FC1          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FC1u), (uint16_t)(ctx->a));
    /* $03:EA84: AD 00 02    | LDA $0200          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0200u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA87: 8D C2 0F    | STA $0FC2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FC2u), (uint16_t)(ctx->a));
    /* $03:EA8A: AD 4A 02    | LDA $024A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x024Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA8D: 8D C4 0F    | STA $0FC4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FC4u), (uint16_t)(ctx->a));
    /* $03:EA90: AD 4E 02    | LDA $024E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x024Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA93: 8D C5 0F    | STA $0FC5          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FC5u), (uint16_t)(ctx->a));
    /* $03:EA96: AD 40 02    | LDA $0240          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0240u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA99: 8D C6 0F    | STA $0FC6          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FC6u), (uint16_t)(ctx->a));
    /* $03:EA9C: AD C0 11    | LDA $11C0          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x11C0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EA9F: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAA2: 09 AA 00    | ORA #$00AA         [M=16-bit, X=16-bit] */
    ctx->a |= 0x00AAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAA5: 8D C0 11    | STA $11C0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C0u), (uint16_t)(ctx->a));
    /* $03:EAA8: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_eaa9  (SNES $03:EAA9 | Ghidra 0x3eaa9)
 * Discovery Provenance: JSR $EAA9 @ $03:E97C
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_eaa9(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EAA9: A2 D0 0F    | LDX #$0FD0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0FD0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EAAC: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:EAAE: A2 C0 11    | LDX #$11C0         [M=16-bit, X=16-bit] */
    ctx->x = 0x11C0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EAB1: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:EAB3: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAB5: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:EAB6: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAB9: 09 80 00    | ORA #$0080         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EABC: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:EABE: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_eabf  (SNES $03:EABF | Ghidra 0x3eabf)
 * Discovery Provenance: JSR $EABF @ $03:E97F
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_eabf(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EABF: A2 00 03    | LDX #$0300         [M=16-bit, X=16-bit] */
    ctx->x = 0x0300u;
    snes_set_nz16(ctx, ctx->x);
L_eac2: /* [M=16-bit, X=16-bit] */
    /* $03:EAC2: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAC4: F0 73       | BEQ $EB39          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_eb39;
    /* $03:EAC6: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAC8: 30 10       | BMI $EADA          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_eada;
    /* $03:EACA: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EACB: ED 14 15    | SBC $1514          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1514u)));
    /* $03:EACE: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAD1: 15 00       | ORA $00,X          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAD3: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:EAD4: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EAD5: B9 C8 80    | LDA $80C8,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x80C8u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAD8: 80 04       | BRA $EADE          [M=16-bit, X=16-bit] */
    goto L_eade;
L_eada: /* [M=16-bit, X=16-bit] */
    /* $03:EADA: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EADC: F0 5B       | BEQ $EB39          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_eb39;
L_eade: /* [M=16-bit, X=16-bit] */
    /* $03:EADE: 15 02       | ORA $02,X          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAE0: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $03:EAE2: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EAE4: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAE6: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:EAE9: B0 4E       | BCS $EB39          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_eb39;
    /* $03:EAEB: A4 10       | LDY $10            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EAED: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EAEE: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EAF0: F5 16       | SBC $16,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x16u)));
    /* $03:EAF2: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:EAF4: 30 43       | BMI $EB39          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_eb39;
    /* $03:EAF6: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:EAF8: 10 3F       | BPL $EB39          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_eb39;
    /* $03:EAFA: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:EAFD: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EAFE: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB00: F5 16       | SBC $16,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x16u)));
    /* $03:EB02: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:EB05: 10 32       | BPL $EB39          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_eb39;
    /* $03:EB07: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:EB0A: 30 2D       | BMI $EB39          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_eb39;
    /* $03:EB0C: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:EB0E: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:EB10: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:EB13: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:EB15: A5 04       | LDA $04            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB17: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:EB1A: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EB1B: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EB1C: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EB1D: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EB1E: 84 10       | STY $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->y));
    /* $03:EB20: B5 16       | LDA $16,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x16u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB22: C9 08 00    | CMP #$0008         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0008u);
    /* $03:EB25: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:EB27: 90 10       | BCC $EB39          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_eb39;
    /* $03:EB29: A4 12       | LDY $12            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EB2B: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB2D: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:EB30: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EB31: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EB32: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB35: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:EB37: 84 12       | STY $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->y));
L_eb39: /* [M=16-bit, X=16-bit] */
    /* $03:EB39: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB3A: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EB3B: 69 20 00    | ADC #$0020         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0020u);
    /* $03:EB3E: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EB3F: E0 80 09    | CPX #$0980         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0980u);
    /* $03:EB42: B0 03       | BCS $EB47          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_eb47;
    /* $03:EB44: 82 7B FF    | BRL $EAC2          [M=16-bit, X=16-bit] */
    goto L_eac2;
L_eb47: /* [M=16-bit, X=16-bit] */
    /* $03:EB47: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_eb48  (SNES $03:EB48 | Ghidra 0x3eb48)
 * Discovery Provenance: JSR $EB48 @ $03:E982
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_eb48(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EB48: A2 80 09    | LDX #$0980         [M=16-bit, X=16-bit] */
    ctx->x = 0x0980u;
    snes_set_nz16(ctx, ctx->x);
L_eb4b: /* [M=16-bit, X=16-bit] */
    /* $03:EB4B: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB4D: F0 17       | BEQ $EB66          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_eb66;
    /* $03:EB4F: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EB51: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB53: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:EB56: B0 0E       | BCS $EB66          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_eb66;
    /* $03:EB58: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB5A: 09 00 01    | ORA #$0100         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB5D: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:EB5F: B4 00       | LDY $00,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EB61: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:EB62: 20 77 F0    | JSR $F077          [M=16-bit, X=16-bit] */
    bank03_func_f077(ctx);
    /* $03:EB65: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_eb66: /* [M=16-bit, X=16-bit] */
    /* $03:EB66: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB67: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EB68: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:EB6B: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EB6C: E0 C0 0A    | CPX #$0AC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0AC0u);
    /* $03:EB6F: 90 DA       | BCC $EB4B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_eb4b;
    /* $03:EB71: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_eb72  (SNES $03:EB72 | Ghidra 0x3eb72)
 * Discovery Provenance: JSR $EB72 @ $03:E985
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_eb72(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EB72: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_eb74: /* [M=16-bit, X=16-bit] */
    /* $03:EB74: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EB75: 90 FD       | BCC $EB74          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_eb74;
    /* $03:EB77: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:EB79: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:EB7B: E0 C0 10    | CPX #$10C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10C0u);
    /* $03:EB7E: B0 0E       | BCS $EB8E          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_eb8e;
    /* $03:EB80: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_eb83: /* [M=16-bit, X=16-bit] */
    /* $03:EB83: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:EB85: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EB86: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EB87: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EB88: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EB89: E0 C0 10    | CPX #$10C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10C0u);
    /* $03:EB8C: 90 F5       | BCC $EB83          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_eb83;
L_eb8e: /* [M=16-bit, X=16-bit] */
    /* $03:EB8E: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_eb8f  (SNES $03:EB8F | Ghidra 0x3eb8f)
 * Discovery Provenance: JSR $EB8F @ $03:E959
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_eb8f(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EB8F: A2 C0 10    | LDX #$10C0         [M=16-bit, X=16-bit] */
    ctx->x = 0x10C0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EB92: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:EB94: A2 D0 11    | LDX #$11D0         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EB97: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:EB99: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EB9C: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:EB9E: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBA1: F0 03       | BEQ $EBA6          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_eba6;
    /* $03:EBA3: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBA4: D0 08       | BNE $EBAE          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ebae;
L_eba6: /* [M=16-bit, X=16-bit] */
    /* $03:EBA6: AD EC 0E    | LDA $0EEC          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0EECu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBA9: F0 03       | BEQ $EBAE          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ebae;
    /* $03:EBAB: 82 98 00    | BRL $EC46          [M=16-bit, X=16-bit] */
    goto L_ec46;
L_ebae: /* [M=16-bit, X=16-bit] */
    /* $03:EBAE: 64 08       | STZ $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(0u));
    /* $03:EBB0: A2 40 0D    | LDX #$0D40         [M=16-bit, X=16-bit] */
    ctx->x = 0x0D40u;
    snes_set_nz16(ctx, ctx->x);
L_ebb3: /* [M=16-bit, X=16-bit] */
    /* $03:EBB3: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBB5: F0 7D       | BEQ $EC34          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ec34;
    /* $03:EBB7: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:EBB8: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EBB9: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBBC: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:EBBE: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBC1: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:EBC3: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EBC5: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBC7: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:EBCA: 90 0A       | BCC $EBD6          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ebd6;
    /* $03:EBCC: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBCE: 29 FB FF    | AND #$FFFB         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFBu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBD1: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:EBD3: 82 5E 00    | BRL $EC34          [M=16-bit, X=16-bit] */
    goto L_ec34;
L_ebd6: /* [M=16-bit, X=16-bit] */
    /* $03:EBD6: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBD8: 09 04 00    | ORA #$0004         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBDB: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:EBDD: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBDF: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:EBE0: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EBE1: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EBE2: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:EBE4: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBE6: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EBE7: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:EBE9: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EBEA: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:EBED: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EBEE: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EBEF: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBF2: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EBF3: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:EBF5: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EBF6: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBF8: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:EBF9: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:EBFB: 1D AC 80    | ORA $80AC,X        [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_absx(ctx, 0x80ACu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EBFE: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:EC00: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC02: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:EC03: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC05: BC 8C 80    | LDY $808C,X        [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x808Cu));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EC08: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC09: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC0C: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:EC0F: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC12: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:EC15: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC18: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:EC1B: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EC1C: 20 F9 EF    | JSR $EFF9          [M=16-bit, X=16-bit] */
    bank03_func_eff9(ctx);
    /* $03:EC1F: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC20: A5 D0       | LDA $D0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC22: D0 09       | BNE $EC2D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ec2d;
    /* $03:EC24: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC26: 29 FE FF    | AND #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC29: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:EC2B: 80 07       | BRA $EC34          [M=16-bit, X=16-bit] */
    goto L_ec34;
L_ec2d: /* [M=16-bit, X=16-bit] */
    /* $03:EC2D: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC2F: 09 01 00    | ORA #$0001         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC32: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_ec34: /* [M=16-bit, X=16-bit] */
    /* $03:EC34: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:EC36: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:EC38: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC39: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EC3A: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:EC3D: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC3E: E0 C0 0E    | CPX #$0EC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0EC0u);
    /* $03:EC41: B0 03       | BCS $EC46          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ec46;
    /* $03:EC43: 82 6D FF    | BRL $EBB3          [M=16-bit, X=16-bit] */
    goto L_ebb3;
L_ec46: /* [M=16-bit, X=16-bit] */
    /* $03:EC46: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_ec48: /* [M=16-bit, X=16-bit] */
    /* $03:EC48: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EC49: 90 FD       | BCC $EC48          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ec48;
    /* $03:EC4B: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:EC4D: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC4F: E0 20 11    | CPX #$1120         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1120u);
    /* $03:EC52: B0 0E       | BCS $EC62          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ec62;
    /* $03:EC54: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_ec57: /* [M=16-bit, X=16-bit] */
    /* $03:EC57: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:EC59: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC5A: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC5B: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC5C: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC5D: E0 20 11    | CPX #$1120         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1120u);
    /* $03:EC60: 90 F5       | BCC $EC57          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ec57;
L_ec62: /* [M=16-bit, X=16-bit] */
    /* $03:EC62: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_ec63  (SNES $03:EC63 | Ghidra 0x3ec63)
 * Discovery Provenance: JSR $EC63 @ $03:E9C8
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_ec63(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EC63: A2 80 11    | LDX #$1180         [M=16-bit, X=16-bit] */
    ctx->x = 0x1180u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC66: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:EC68: A2 DC 11    | LDX #$11DC         [M=16-bit, X=16-bit] */
    ctx->x = 0x11DCu;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EC6B: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:EC6D: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC70: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:EC72: 64 08       | STZ $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(0u));
    /* $03:EC74: A2 C0 0E    | LDX #$0EC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0EC0u;
    snes_set_nz16(ctx, ctx->x);
L_ec77: /* [M=16-bit, X=16-bit] */
    /* $03:EC77: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC7A: F0 0A       | BEQ $EC86          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ec86;
    /* $03:EC7C: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC7D: F0 07       | BEQ $EC86          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ec86;
    /* $03:EC7F: B5 2C       | LDA $2C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC81: F0 10       | BEQ $EC93          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ec93;
    /* $03:EC83: 82 7E 00    | BRL $ED04          [M=16-bit, X=16-bit] */
    goto L_ed04;
L_ec86: /* [M=16-bit, X=16-bit] */
    /* $03:EC86: E0 C0 0E    | CPX #$0EC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0EC0u);
    /* $03:EC89: F0 08       | BEQ $EC93          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ec93;
    /* $03:EC8B: AD EC 0E    | LDA $0EEC          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0EECu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC8E: 0D 2C 0F    | ORA $0F2C          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0F2Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC91: D0 71       | BNE $ED04          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ed04;
L_ec93: /* [M=16-bit, X=16-bit] */
    /* $03:EC93: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC95: F0 6D       | BEQ $ED04          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ed04;
    /* $03:EC97: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:EC98: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EC99: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EC9C: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:EC9E: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECA1: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:ECA3: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:ECA5: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECA7: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:ECAA: 90 0B       | BCC $ECB7          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ecb7;
    /* $03:ECAC: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECAF: 9D B0 00    | STA $00B0,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B0u), (uint16_t)(ctx->a));
    /* $03:ECB2: 9D B4 00    | STA $00B4,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B4u), (uint16_t)(ctx->a));
    /* $03:ECB5: 80 4D       | BRA $ED04          [M=16-bit, X=16-bit] */
    goto L_ed04;
L_ecb7: /* [M=16-bit, X=16-bit] */
    /* $03:ECB7: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECB9: 9D B0 00    | STA $00B0,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B0u), (uint16_t)(ctx->a));
    /* $03:ECBC: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECBE: 9D B4 00    | STA $00B4,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B4u), (uint16_t)(ctx->a));
    /* $03:ECC1: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECC3: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:ECC4: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:ECC5: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:ECC6: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:ECC8: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECCA: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:ECCB: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:ECCD: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:ECCE: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:ECD1: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:ECD2: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:ECD3: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECD6: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:ECD7: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:ECD9: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:ECDA: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECDC: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:ECDD: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:ECDF: 1D A8 80    | ORA $80A8,X        [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_absx(ctx, 0x80A8u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECE2: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:ECE4: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECE6: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:ECE7: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECE9: BC 88 80    | LDY $8088,X        [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8088u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:ECEC: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:ECED: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECF0: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:ECF3: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECF6: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:ECF9: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ECFC: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:ECFF: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:ED00: 20 F9 EF    | JSR $EFF9          [M=16-bit, X=16-bit] */
    bank03_func_eff9(ctx);
    /* $03:ED03: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_ed04: /* [M=16-bit, X=16-bit] */
    /* $03:ED04: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:ED06: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:ED08: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED09: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:ED0A: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:ED0D: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:ED0E: E0 40 0F    | CPX #$0F40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0F40u);
    /* $03:ED11: B0 03       | BCS $ED16          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ed16;
    /* $03:ED13: 82 61 FF    | BRL $EC77          [M=16-bit, X=16-bit] */
    goto L_ec77;
L_ed16: /* [M=16-bit, X=16-bit] */
    /* $03:ED16: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_ed18: /* [M=16-bit, X=16-bit] */
    /* $03:ED18: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:ED19: 90 FD       | BCC $ED18          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ed18;
    /* $03:ED1B: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:ED1D: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:ED1F: E0 A0 11    | CPX #$11A0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x11A0u);
    /* $03:ED22: B0 0E       | BCS $ED32          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ed32;
    /* $03:ED24: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_ed27: /* [M=16-bit, X=16-bit] */
    /* $03:ED27: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:ED29: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:ED2A: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:ED2B: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:ED2C: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:ED2D: E0 A0 11    | CPX #$11A0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x11A0u);
    /* $03:ED30: 90 F5       | BCC $ED27          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ed27;
L_ed32: /* [M=16-bit, X=16-bit] */
    /* $03:ED32: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_ed33  (SNES $03:ED33 | Ghidra 0x3ed33)
 * Discovery Provenance: JSR $ED33 @ $03:E988
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_ed33(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:ED33: A2 C0 10    | LDX #$10C0         [M=16-bit, X=16-bit] */
    ctx->x = 0x10C0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:ED36: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:ED38: A2 D0 11    | LDX #$11D0         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:ED3B: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:ED3D: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED40: D0 07       | BNE $ED49          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ed49;
    /* $03:ED42: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED45: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:ED47: 80 0B       | BRA $ED54          [M=16-bit, X=16-bit] */
    goto L_ed54;
L_ed49: /* [M=16-bit, X=16-bit] */
    /* $03:ED49: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED4B: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:ED4C: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED4F: 09 80 00    | ORA #$0080         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED52: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
L_ed54: /* [M=16-bit, X=16-bit] */
    /* $03:ED54: A2 80 0B    | LDX #$0B80         [M=16-bit, X=16-bit] */
    ctx->x = 0x0B80u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:ED57: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED59: F0 7B       | BEQ $EDD6          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_edd6;
    /* $03:ED5B: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:ED5D: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED5F: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:ED62: 90 09       | BCC $ED6D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ed6d;
    /* $03:ED64: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED66: 29 FB FF    | AND #$FFFB         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFBu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED69: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:ED6B: 80 69       | BRA $EDD6          [M=16-bit, X=16-bit] */
    goto L_edd6;
L_ed6d: /* [M=16-bit, X=16-bit] */
    /* $03:ED6D: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED6F: 09 04 00    | ORA #$0004         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED72: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:ED74: A0 04 00    | LDY #$0004         [M=16-bit, X=16-bit] */
    ctx->y = 0x0004u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:ED77: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED7A: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:ED7C: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED7F: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:ED81: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED83: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:ED84: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:ED85: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:ED86: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:ED88: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED8A: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:ED8B: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:ED8D: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:ED8E: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:ED91: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:ED92: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:ED93: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED96: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:ED97: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:ED99: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:ED9A: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:ED9C: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:ED9D: 0D AC 80    | ORA $80AC          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_abs(ctx, 0x80ACu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDA0: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:EDA2: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDA4: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:EDA5: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDA7: AC 8C 80    | LDY $808C          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x808Cu));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EDAA: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EDAB: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDAE: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:EDB1: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDB4: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:EDB7: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDBA: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:EDBD: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EDBE: 20 F9 EF    | JSR $EFF9          [M=16-bit, X=16-bit] */
    bank03_func_eff9(ctx);
    /* $03:EDC1: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EDC2: A5 D0       | LDA $D0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDC4: D0 09       | BNE $EDCF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_edcf;
    /* $03:EDC6: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDC8: 29 FE FF    | AND #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDCB: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:EDCD: 80 07       | BRA $EDD6          [M=16-bit, X=16-bit] */
    goto L_edd6;
L_edcf: /* [M=16-bit, X=16-bit] */
    /* $03:EDCF: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDD1: 09 01 00    | ORA #$0001         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDD4: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_edd6: /* [M=16-bit, X=16-bit] */
    /* $03:EDD6: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_edd8: /* [M=16-bit, X=16-bit] */
    /* $03:EDD8: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EDD9: 90 FD       | BCC $EDD8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_edd8;
    /* $03:EDDB: 09 AA AA    | ORA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a |= 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EDDE: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:EDE0: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:EDE2: E0 D0 10    | CPX #$10D0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10D0u);
    /* $03:EDE5: B0 0E       | BCS $EDF5          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_edf5;
    /* $03:EDE7: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_edea: /* [M=16-bit, X=16-bit] */
    /* $03:EDEA: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:EDEC: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EDED: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EDEE: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EDEF: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EDF0: E0 D0 10    | CPX #$10D0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10D0u);
    /* $03:EDF3: 90 F5       | BCC $EDEA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_edea;
L_edf5: /* [M=16-bit, X=16-bit] */
    /* $03:EDF5: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_edf6  (SNES $03:EDF6 | Ghidra 0x3edf6)
 * Discovery Provenance: JSR $EDF6 @ $03:E98B
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_edf6(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EDF6: A2 E0 10    | LDX #$10E0         [M=16-bit, X=16-bit] */
    ctx->x = 0x10E0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EDF9: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:EDFB: A2 D2 11    | LDX #$11D2         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D2u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EDFE: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:EE00: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE03: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:EE05: 64 08       | STZ $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(0u));
    /* $03:EE07: A2 C0 0E    | LDX #$0EC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0EC0u;
    snes_set_nz16(ctx, ctx->x);
L_ee0a: /* [M=16-bit, X=16-bit] */
    /* $03:EE0A: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE0C: F0 6D       | BEQ $EE7B          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ee7b;
    /* $03:EE0E: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:EE0F: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EE10: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE13: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:EE15: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE18: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:EE1A: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EE1C: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE1E: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:EE21: 90 0B       | BCC $EE2E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ee2e;
    /* $03:EE23: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE26: 9D B0 00    | STA $00B0,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B0u), (uint16_t)(ctx->a));
    /* $03:EE29: 9D B4 00    | STA $00B4,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B4u), (uint16_t)(ctx->a));
    /* $03:EE2C: 80 4D       | BRA $EE7B          [M=16-bit, X=16-bit] */
    goto L_ee7b;
L_ee2e: /* [M=16-bit, X=16-bit] */
    /* $03:EE2E: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE30: 9D B0 00    | STA $00B0,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B0u), (uint16_t)(ctx->a));
    /* $03:EE33: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE35: 9D B4 00    | STA $00B4,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B4u), (uint16_t)(ctx->a));
    /* $03:EE38: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE3A: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:EE3B: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EE3C: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EE3D: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:EE3F: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE41: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EE42: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:EE44: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EE45: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:EE48: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EE49: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EE4A: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE4D: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EE4E: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:EE50: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EE51: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE53: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:EE54: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:EE56: 1D A8 80    | ORA $80A8,X        [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_absx(ctx, 0x80A8u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE59: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:EE5B: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE5D: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:EE5E: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE60: BC 88 80    | LDY $8088,X        [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8088u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EE63: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EE64: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE67: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:EE6A: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE6D: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:EE70: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE73: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:EE76: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EE77: 20 F9 EF    | JSR $EFF9          [M=16-bit, X=16-bit] */
    bank03_func_eff9(ctx);
    /* $03:EE7A: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_ee7b: /* [M=16-bit, X=16-bit] */
    /* $03:EE7B: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:EE7D: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:EE7F: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EE80: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EE81: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:EE84: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EE85: E0 40 0F    | CPX #$0F40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0F40u);
    /* $03:EE88: B0 03       | BCS $EE8D          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ee8d;
    /* $03:EE8A: 82 7D FF    | BRL $EE0A          [M=16-bit, X=16-bit] */
    goto L_ee0a;
L_ee8d: /* [M=16-bit, X=16-bit] */
    /* $03:EE8D: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_ee8f: /* [M=16-bit, X=16-bit] */
    /* $03:EE8F: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EE90: 90 FD       | BCC $EE8F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ee8f;
    /* $03:EE92: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:EE94: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:EE96: E0 00 11    | CPX #$1100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1100u);
    /* $03:EE99: B0 0E       | BCS $EEA9          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_eea9;
    /* $03:EE9B: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_ee9e: /* [M=16-bit, X=16-bit] */
    /* $03:EE9E: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:EEA0: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EEA1: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EEA2: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EEA3: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EEA4: E0 00 11    | CPX #$1100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1100u);
    /* $03:EEA7: 90 F5       | BCC $EE9E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_ee9e;
L_eea9: /* [M=16-bit, X=16-bit] */
    /* $03:EEA9: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_eeaa  (SNES $03:EEAA | Ghidra 0x3eeaa)
 * Discovery Provenance: JSR $EEAA @ $03:E98E
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_eeaa(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EEAA: A2 20 11    | LDX #$1120         [M=16-bit, X=16-bit] */
    ctx->x = 0x1120u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EEAD: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:EEAF: A2 D6 11    | LDX #$11D6         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D6u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EEB2: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:EEB4: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEB7: D0 07       | BNE $EEC0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_eec0;
    /* $03:EEB9: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEBC: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:EEBE: 80 0B       | BRA $EECB          [M=16-bit, X=16-bit] */
    goto L_eecb;
L_eec0: /* [M=16-bit, X=16-bit] */
    /* $03:EEC0: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEC2: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:EEC3: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEC6: 09 80 00    | ORA #$0080         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEC9: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
L_eecb: /* [M=16-bit, X=16-bit] */
    /* $03:EECB: A9 28 10    | LDA #$1028         [M=16-bit, X=16-bit] */
    ctx->a = 0x1028u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EECE: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:EED0: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EED3: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EED4: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:EED6: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EED7: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:EEDA: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EEDB: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EEDC: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEDF: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EEE0: B9 CE 9B    | LDA $9BCE,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x9BCEu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEE3: 85 CA       | STA $CA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCAu), (uint16_t)(ctx->a));
    /* $03:EEE5: B9 0E 9C    | LDA $9C0E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x9C0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEE8: A0 20 00    | LDY #$0020         [M=16-bit, X=16-bit] */
    ctx->y = 0x0020u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EEEB: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EEEC: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEEF: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:EEF2: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEF5: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:EEF8: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EEFB: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:EEFE: A5 18       | LDA $18            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF00: 29 80 FF    | AND #$FF80         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF80u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF03: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:EF05: A5 1A       | LDA $1A            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF07: 29 80 FF    | AND #$FF80         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF80u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF0A: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:EF0C: A9 12 00    | LDA #$0012         [M=16-bit, X=16-bit] */
    ctx->a = 0x0012u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF0F: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
L_ef11: /* [M=16-bit, X=16-bit] */
    /* $03:EF11: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:EF13: A5 C4       | LDA $C4            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF15: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EF16: 7D FE 7F    | ADC $7FFE,X        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absx(ctx, 0x7FFEu)));
    /* $03:EF19: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EF1A: A5 C6       | LDA $C6            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF1C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EF1D: 7D 10 80    | ADC $8010,X        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8010u)));
    /* $03:EF20: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:EF23: B0 05       | BCS $EF2A          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_ef2a;
    /* $03:EF25: A4 CA       | LDY $CA            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCAu));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EF27: 20 F9 EF    | JSR $EFF9          [M=16-bit, X=16-bit] */
    bank03_func_eff9(ctx);
L_ef2a: /* [M=16-bit, X=16-bit] */
    /* $03:EF2A: C6 08       | DEC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:EF2C: C6 08       | DEC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:EF2E: D0 E1       | BNE $EF11          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ef11;
    /* $03:EF30: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF33: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EF34: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:EF36: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EF37: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EF38: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF3B: A0 24 10    | LDY #$1024         [M=16-bit, X=16-bit] */
    ctx->y = 0x1024u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EF3E: 89 20 00    | BIT #$0020         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0020u)) == 0u ? 1u : 0u);
    /* $03:EF41: F0 03       | BEQ $EF46          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_ef46;
    /* $03:EF43: A0 24 D0    | LDY #$D024         [M=16-bit, X=16-bit] */
    ctx->y = 0xD024u;
    snes_set_nz16(ctx, ctx->y);
L_ef46: /* [M=16-bit, X=16-bit] */
    /* $03:EF46: 84 06       | STY $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->y));
    /* $03:EF48: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EF49: B9 84 9C    | LDA $9C84,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x9C84u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF4C: A0 18 00    | LDY #$0018         [M=16-bit, X=16-bit] */
    ctx->y = 0x0018u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EF4F: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:EF50: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF53: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:EF56: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF59: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:EF5C: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF5F: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:EF62: A5 18       | LDA $18            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF64: 29 80 FF    | AND #$FF80         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF80u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF67: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:EF69: A5 1A       | LDA $1A            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF6B: 29 80 FF    | AND #$FF80         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF80u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF6E: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:EF70: A2 32 00    | LDX #$0032         [M=16-bit, X=16-bit] */
    ctx->x = 0x0032u;
    snes_set_nz16(ctx, ctx->x);
L_ef73: /* [M=16-bit, X=16-bit] */
    /* $03:EF73: A5 C4       | LDA $C4            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF75: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EF76: 7D 22 80    | ADC $8022,X        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8022u)));
    /* $03:EF79: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:EF7A: A5 C6       | LDA $C6            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF7C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EF7D: 7D 54 80    | ADC $8054,X        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8054u)));
    /* $03:EF80: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:EF83: B0 50       | BCS $EFD5          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_efd5;
    /* $03:EF85: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EF86: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF88: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EF89: 6D A2 15    | ADC $15A2          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x15A2u)));
    /* $03:EF8C: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:EF8F: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:EF91: 30 42       | BMI $EFD5          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_efd5;
    /* $03:EF93: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:EF95: 10 3E       | BPL $EFD5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_efd5;
    /* $03:EF97: A4 10       | LDY $10            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EF99: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:EF9C: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EF9D: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EF9F: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:EFA2: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:EFA5: 10 2E       | BPL $EFD5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_efd5;
    /* $03:EFA7: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:EFAA: 30 29       | BMI $EFD5          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_efd5;
    /* $03:EFAC: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:EFAE: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:EFB0: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:EFB3: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:EFB5: A5 06       | LDA $06            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EFB7: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:EFBA: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EFBB: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EFBC: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EFBD: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EFBE: 84 10       | STY $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->y));
    /* $03:EFC0: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:EFC1: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:EFC3: 90 10       | BCC $EFD5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_efd5;
    /* $03:EFC5: A4 12       | LDY $12            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:EFC7: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EFC9: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:EFCC: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EFCD: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:EFCE: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EFD1: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:EFD3: 84 12       | STY $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->y));
L_efd5: /* [M=16-bit, X=16-bit] */
    /* $03:EFD5: CA          | DEX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x - 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EFD6: CA          | DEX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x - 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EFD7: D0 9A       | BNE $EF73          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_ef73;
    /* $03:EFD9: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_efdb: /* [M=16-bit, X=16-bit] */
    /* $03:EFDB: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:EFDC: 90 FD       | BCC $EFDB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_efdb;
    /* $03:EFDE: 09 AA AA    | ORA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a |= 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:EFE1: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:EFE3: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:EFE5: E0 70 11    | CPX #$1170         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1170u);
    /* $03:EFE8: B0 0E       | BCS $EFF8          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_eff8;
    /* $03:EFEA: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_efed: /* [M=16-bit, X=16-bit] */
    /* $03:EFED: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:EFEF: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EFF0: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EFF1: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EFF2: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:EFF3: E0 70 11    | CPX #$1170         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1170u);
    /* $03:EFF6: 90 F5       | BCC $EFED          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_efed;
L_eff8: /* [M=16-bit, X=16-bit] */
    /* $03:EFF8: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_eff9  (SNES $03:EFF9 | Ghidra 0x3eff9)
 * Discovery Provenance: JSR $EFF9 @ $03:EF27
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_eff9(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:EFF9: 64 D0       | STZ $D0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD0u), (uint16_t)(0u));
    /* $03:EFFB: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:EFFD: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:EFFE: 6D A2 15    | ADC $15A2          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x15A2u)));
    /* $03:F001: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:F003: B9 00 00    | LDA $0000,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F006: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F009: 85 C8       | STA $C8            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC8u), (uint16_t)(ctx->a));
    /* $03:F00B: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
L_f00d: /* [M=16-bit, X=16-bit] */
    /* $03:F00D: 64 CD       | STZ $CD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(0u));
    /* $03:F00F: B9 01 00    | LDA $0001,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0001u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F012: 10 02       | BPL $F016          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f016;
    /* $03:F014: C6 CD       | DEC $CD            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(v16)); }
L_f016: /* [M=16-bit, X=16-bit] */
    /* $03:F016: 85 CC       | STA $CC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCCu), (uint16_t)(ctx->a));
    /* $03:F018: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F019: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F01B: 65 CD       | ADC $CD            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu)));
    /* $03:F01D: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:F01F: 30 4B       | BMI $F06C          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f06c;
    /* $03:F021: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:F023: 10 47       | BPL $F06C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f06c;
    /* $03:F025: 95 01       | STA $01,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x01u), (uint16_t)(ctx->a));
    /* $03:F027: 64 CD       | STZ $CD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(0u));
    /* $03:F029: B9 00 00    | LDA $0000,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F02C: 10 02       | BPL $F030          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f030;
    /* $03:F02E: C6 CD       | DEC $CD            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(v16)); }
L_f030: /* [M=16-bit, X=16-bit] */
    /* $03:F030: 85 CC       | STA $CC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCCu), (uint16_t)(ctx->a));
    /* $03:F032: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F033: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F035: 65 CD       | ADC $CD            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu)));
    /* $03:F037: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:F03A: 10 30       | BPL $F06C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f06c;
    /* $03:F03C: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:F03F: 30 2B       | BMI $F06C          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f06c;
    /* $03:F041: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:F043: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F045: 95 00       | STA $00,X          [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dpx(ctx, 0x00u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:F047: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F049: E6 D0       | INC $D0            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD0u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD0u), (uint16_t)(v16)); }
    /* $03:F04B: B9 03 00    | LDA $0003,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0003u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F04E: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F050: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:F052: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F053: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F054: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F055: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F056: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F057: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:F059: 90 11       | BCC $F06C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f06c;
    /* $03:F05B: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:F05C: A6 12       | LDX $12            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F05E: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F060: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:F062: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F063: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F064: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F067: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F069: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F06B: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_f06c: /* [M=16-bit, X=16-bit] */
    /* $03:F06C: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F06D: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F06E: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F06F: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F070: C6 C8       | DEC $C8            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC8u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC8u), (uint16_t)(v16)); }
    /* $03:F072: D0 99       | BNE $F00D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f00d;
    /* $03:F074: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F076: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f077  (SNES $03:F077 | Ghidra 0x3f077)
 * Discovery Provenance: JSR $F077 @ $03:EB62
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f077(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F077: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F079: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F07A: 6D A2 15    | ADC $15A2          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x15A2u)));
    /* $03:F07D: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:F07F: B9 00 00    | LDA $0000,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F082: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F085: 85 C8       | STA $C8            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC8u), (uint16_t)(ctx->a));
    /* $03:F087: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
L_f089: /* [M=16-bit, X=16-bit] */
    /* $03:F089: 64 CD       | STZ $CD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(0u));
    /* $03:F08B: B9 01 00    | LDA $0001,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0001u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F08E: 10 02       | BPL $F092          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f092;
    /* $03:F090: C6 CD       | DEC $CD            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(v16)); }
L_f092: /* [M=16-bit, X=16-bit] */
    /* $03:F092: 85 CC       | STA $CC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCCu), (uint16_t)(ctx->a));
    /* $03:F094: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F095: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F097: 65 CD       | ADC $CD            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu)));
    /* $03:F099: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:F09B: 30 4F       | BMI $F0EC          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f0ec;
    /* $03:F09D: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:F09F: 10 4B       | BPL $F0EC          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f0ec;
    /* $03:F0A1: 95 01       | STA $01,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x01u), (uint16_t)(ctx->a));
    /* $03:F0A3: 64 CD       | STZ $CD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(0u));
    /* $03:F0A5: B9 00 00    | LDA $0000,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F0A8: 10 02       | BPL $F0AC          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f0ac;
    /* $03:F0AA: C6 CD       | DEC $CD            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(v16)); }
L_f0ac: /* [M=16-bit, X=16-bit] */
    /* $03:F0AC: 85 CC       | STA $CC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCCu), (uint16_t)(ctx->a));
    /* $03:F0AE: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F0AF: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F0B1: 65 CD       | ADC $CD            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu)));
    /* $03:F0B3: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:F0B6: 10 34       | BPL $F0EC          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f0ec;
    /* $03:F0B8: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:F0BB: 30 2F       | BMI $F0EC          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f0ec;
    /* $03:F0BD: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:F0BF: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F0C1: 95 00       | STA $00,X          [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dpx(ctx, 0x00u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:F0C3: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F0C5: B9 03 00    | LDA $0003,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0003u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F0C8: 29 FF C0    | AND #$C0FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0xC0FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F0CB: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F0CD: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:F0CF: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F0D0: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F0D1: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F0D2: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F0D3: B9 04 00    | LDA $0004,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F0D6: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F0D7: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:F0D9: 90 11       | BCC $F0EC          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f0ec;
    /* $03:F0DB: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:F0DC: A6 12       | LDX $12            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F0DE: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F0E0: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:F0E2: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F0E3: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F0E4: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F0E7: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F0E9: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F0EB: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_f0ec: /* [M=16-bit, X=16-bit] */
    /* $03:F0EC: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F0ED: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F0EE: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F0EF: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F0F0: C6 C8       | DEC $C8            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC8u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC8u), (uint16_t)(v16)); }
    /* $03:F0F2: D0 95       | BNE $F089          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f089;
    /* $03:F0F4: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F0F6: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f0f7  (SNES $03:F0F7 | Ghidra 0x3f0f7)
 * Discovery Provenance: JSR $F0F7 @ $03:E991
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f0f7(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F0F7: A2 C0 0A    | LDX #$0AC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0AC0u;
    snes_set_nz16(ctx, ctx->x);
L_f0fa: /* [M=16-bit, X=16-bit] */
    /* $03:F0FA: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F0FC: F0 57       | BEQ $F155          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f155;
    /* $03:F0FE: E0 40 0D    | CPX #$0D40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0D40u);
    /* $03:F101: B0 07       | BCS $F10A          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f10a;
    /* $03:F103: C9 03 00    | CMP #$0003         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0003u);
    /* $03:F106: F0 4D       | BEQ $F155          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f155;
    /* $03:F108: 80 04       | BRA $F10E          [M=16-bit, X=16-bit] */
    goto L_f10e;
L_f10a: /* [M=16-bit, X=16-bit] */
    /* $03:F10A: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F10C: D0 47       | BNE $F155          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f155;
L_f10e: /* [M=16-bit, X=16-bit] */
    /* $03:F10E: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F110: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F112: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F115: 90 09       | BCC $F120          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f120;
    /* $03:F117: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F119: 29 FB FF    | AND #$FFFB         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFBu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F11C: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F11E: 80 2E       | BRA $F14E          [M=16-bit, X=16-bit] */
    goto L_f14e;
L_f120: /* [M=16-bit, X=16-bit] */
    /* $03:F120: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F122: 09 04 00    | ORA #$0004         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F125: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F127: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F129: 95 18       | STA $18,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $03:F12B: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F12D: 95 1C       | STA $1C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $03:F12F: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F131: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:F133: 30 19       | BMI $F14E          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f14e;
    /* $03:F135: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:F137: 10 15       | BPL $F14E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f14e;
    /* $03:F139: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F13B: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:F13E: 10 0E       | BPL $F14E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f14e;
    /* $03:F140: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:F143: 30 09       | BMI $F14E          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f14e;
    /* $03:F145: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F147: 09 01 00    | ORA #$0001         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F14A: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F14C: 80 07       | BRA $F155          [M=16-bit, X=16-bit] */
    goto L_f155;
L_f14e: /* [M=16-bit, X=16-bit] */
    /* $03:F14E: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F150: 29 FE FF    | AND #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F153: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_f155: /* [M=16-bit, X=16-bit] */
    /* $03:F155: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F156: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F157: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:F15A: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F15B: E0 C0 0E    | CPX #$0EC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0EC0u);
    /* $03:F15E: 90 9A       | BCC $F0FA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f0fa;
    /* $03:F160: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f161  (SNES $03:F161 | Ghidra 0x3f161)
 * Discovery Provenance: JSR $F161 @ $03:E994
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f161(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F161: AD EC 0E    | LDA $0EEC          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0EECu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F164: D0 22       | BNE $F188          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f188;
    /* $03:F166: A2 40 0F    | LDX #$0F40         [M=16-bit, X=16-bit] */
    ctx->x = 0x0F40u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F169: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F16B: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:F16C: D0 1F       | BNE $F18D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f18d;
    /* $03:F16E: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F170: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F172: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F175: 90 09       | BCC $F180          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f180;
    /* $03:F177: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F17A: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $03:F17C: 95 3C       | STA $3C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Cu), (uint16_t)(ctx->a));
    /* $03:F17E: 80 0D       | BRA $F18D          [M=16-bit, X=16-bit] */
    goto L_f18d;
L_f180: /* [M=16-bit, X=16-bit] */
    /* $03:F180: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F182: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $03:F184: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F186: 95 3C       | STA $3C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Cu), (uint16_t)(ctx->a));
L_f188: /* [M=16-bit, X=16-bit] */
    /* $03:F188: AD 2C 0F    | LDA $0F2C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0F2Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F18B: D0 22       | BNE $F1AF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f1af;
L_f18d: /* [M=16-bit, X=16-bit] */
    /* $03:F18D: A2 80 0F    | LDX #$0F80         [M=16-bit, X=16-bit] */
    ctx->x = 0x0F80u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F190: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F192: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:F193: D0 1A       | BNE $F1AF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f1af;
    /* $03:F195: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F197: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F199: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F19C: 90 09       | BCC $F1A7          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f1a7;
    /* $03:F19E: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1A1: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $03:F1A3: 95 3C       | STA $3C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Cu), (uint16_t)(ctx->a));
    /* $03:F1A5: 80 08       | BRA $F1AF          [M=16-bit, X=16-bit] */
    goto L_f1af;
L_f1a7: /* [M=16-bit, X=16-bit] */
    /* $03:F1A7: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1A9: 95 38       | STA $38,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x38u), (uint16_t)(ctx->a));
    /* $03:F1AB: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1AD: 95 3C       | STA $3C,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Cu), (uint16_t)(ctx->a));
L_f1af: /* [M=16-bit, X=16-bit] */
    /* $03:F1AF: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f1b0  (SNES $03:F1B0 | Ghidra 0x3f1b0)
 * Discovery Provenance: JSR $F1B0 @ $03:E9BF
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f1b0(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F1B0: A2 C0 0A    | LDX #$0AC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0AC0u;
    snes_set_nz16(ctx, ctx->x);
L_f1b3: /* [M=16-bit, X=16-bit] */
    /* $03:F1B3: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1B5: F0 3F       | BEQ $F1F6          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f1f6;
    /* $03:F1B7: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F1B9: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1BB: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F1BE: 90 09       | BCC $F1C9          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f1c9;
    /* $03:F1C0: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1C2: 29 FB FF    | AND #$FFFB         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFBu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1C5: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F1C7: 80 26       | BRA $F1EF          [M=16-bit, X=16-bit] */
    goto L_f1ef;
L_f1c9: /* [M=16-bit, X=16-bit] */
    /* $03:F1C9: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1CB: 09 04 00    | ORA #$0004         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1CE: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F1D0: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1D2: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:F1D4: 30 19       | BMI $F1EF          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f1ef;
    /* $03:F1D6: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:F1D8: 10 15       | BPL $F1EF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f1ef;
    /* $03:F1DA: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1DC: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:F1DF: 10 0E       | BPL $F1EF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f1ef;
    /* $03:F1E1: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:F1E4: 30 09       | BMI $F1EF          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f1ef;
    /* $03:F1E6: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1E8: 09 01 00    | ORA #$0001         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1EB: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F1ED: 80 07       | BRA $F1F6          [M=16-bit, X=16-bit] */
    goto L_f1f6;
L_f1ef: /* [M=16-bit, X=16-bit] */
    /* $03:F1EF: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1F1: 29 FE FF    | AND #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1F4: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_f1f6: /* [M=16-bit, X=16-bit] */
    /* $03:F1F6: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F1F7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F1F8: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:F1FB: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F1FC: E0 40 0B    | CPX #$0B40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0B40u);
    /* $03:F1FF: 90 B2       | BCC $F1B3          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f1b3;
    /* $03:F201: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f202  (SNES $03:F202 | Ghidra 0x3f202)
 * Discovery Provenance: JSR $F202 @ $03:E9C2
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f202(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F202: A2 C0 10    | LDX #$10C0         [M=16-bit, X=16-bit] */
    ctx->x = 0x10C0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F205: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F207: A2 D0 11    | LDX #$11D0         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F20A: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F20C: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F20F: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F211: 20 CF F2    | JSR $F2CF          [M=16-bit, X=16-bit] */
    bank03_func_f2cf(ctx);
    /* $03:F214: B0 03       | BCS $F219          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f219;
    /* $03:F216: 20 36 F2    | JSR $F236          [M=16-bit, X=16-bit] */
    bank03_func_f236(ctx);
L_f219: /* [M=16-bit, X=16-bit] */
    /* $03:F219: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_f21b: /* [M=16-bit, X=16-bit] */
    /* $03:F21B: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F21C: 90 FD       | BCC $F21B          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f21b;
    /* $03:F21E: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:F220: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F222: E0 E0 10    | CPX #$10E0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10E0u);
    /* $03:F225: B0 0E       | BCS $F235          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f235;
    /* $03:F227: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_f22a: /* [M=16-bit, X=16-bit] */
    /* $03:F22A: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:F22C: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F22D: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F22E: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F22F: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F230: E0 E0 10    | CPX #$10E0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10E0u);
    /* $03:F233: 90 F5       | BCC $F22A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f22a;
L_f235: /* [M=16-bit, X=16-bit] */
    /* $03:F235: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f236  (SNES $03:F236 | Ghidra 0x3f236)
 * Discovery Provenance: JSR $F236 @ $03:F216
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f236(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F236: 64 08       | STZ $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(0u));
    /* $03:F238: A2 40 0B    | LDX #$0B40         [M=16-bit, X=16-bit] */
    ctx->x = 0x0B40u;
    snes_set_nz16(ctx, ctx->x);
L_f23b: /* [M=16-bit, X=16-bit] */
    /* $03:F23B: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F23D: F0 7D       | BEQ $F2BC          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f2bc;
    /* $03:F23F: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:F240: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F241: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F244: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:F246: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F249: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:F24B: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F24D: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F24F: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F252: 90 0A       | BCC $F25E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f25e;
    /* $03:F254: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F256: 29 FB FF    | AND #$FFFB         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFBu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F259: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F25B: 82 5E 00    | BRL $F2BC          [M=16-bit, X=16-bit] */
    goto L_f2bc;
L_f25e: /* [M=16-bit, X=16-bit] */
    /* $03:F25E: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F260: 09 04 00    | ORA #$0004         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F263: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F265: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F267: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F268: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F269: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F26A: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:F26C: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F26E: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F26F: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:F271: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F272: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:F275: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F276: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F277: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F27A: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F27B: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:F27D: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F27E: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F280: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:F281: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F283: 1D AC 80    | ORA $80AC,X        [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_absx(ctx, 0x80ACu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F286: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:F288: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F28A: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:F28B: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F28D: BC 8C 80    | LDY $808C,X        [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x808Cu));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F290: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F291: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F294: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:F297: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F29A: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:F29D: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2A0: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:F2A3: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F2A4: 20 F9 EF    | JSR $EFF9          [M=16-bit, X=16-bit] */
    bank03_func_eff9(ctx);
    /* $03:F2A7: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F2A8: A5 D0       | LDA $D0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2AA: D0 09       | BNE $F2B5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f2b5;
    /* $03:F2AC: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2AE: 29 FE FF    | AND #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2B1: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F2B3: 80 07       | BRA $F2BC          [M=16-bit, X=16-bit] */
    goto L_f2bc;
L_f2b5: /* [M=16-bit, X=16-bit] */
    /* $03:F2B5: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2B7: 09 01 00    | ORA #$0001         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2BA: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_f2bc: /* [M=16-bit, X=16-bit] */
    /* $03:F2BC: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F2BE: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F2C0: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2C1: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F2C2: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:F2C5: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F2C6: E0 C0 0B    | CPX #$0BC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0BC0u);
    /* $03:F2C9: B0 03       | BCS $F2CE          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f2ce;
    /* $03:F2CB: 82 6D FF    | BRL $F23B          [M=16-bit, X=16-bit] */
    goto L_f23b;
L_f2ce: /* [M=16-bit, X=16-bit] */
    /* $03:F2CE: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f2cf  (SNES $03:F2CF | Ghidra 0x3f2cf)
 * Discovery Provenance: JSR $F2CF @ $03:F211
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f2cf(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F2CF: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2D2: F0 03       | BEQ $F2D7          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f2d7;
    /* $03:F2D4: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2D5: D0 08       | BNE $F2DF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f2df;
L_f2d7: /* [M=16-bit, X=16-bit] */
    /* $03:F2D7: AD EC 0E    | LDA $0EEC          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0EECu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2DA: F0 03       | BEQ $F2DF          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f2df;
    /* $03:F2DC: 82 02 00    | BRL $F2E1          [M=16-bit, X=16-bit] */
    goto L_f2e1;
L_f2df: /* [M=16-bit, X=16-bit] */
    /* $03:F2DF: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F2E0: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_f2e1: /* [M=16-bit, X=16-bit] */
    /* $03:F2E1: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F2E2: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f2e3  (SNES $03:F2E3 | Ghidra 0x3f2e3)
 * Discovery Provenance: JSR $F2E3 @ $03:E9C5
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f2e3(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F2E3: A2 00 11    | LDX #$1100         [M=16-bit, X=16-bit] */
    ctx->x = 0x1100u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F2E6: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F2E8: A2 D4 11    | LDX #$11D4         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D4u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F2EB: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F2ED: A9 00 00    | LDA #$0000         [M=16-bit, X=16-bit] */
    ctx->a = 0x0000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2F0: D0 07       | BNE $F2F9          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f2f9;
    /* $03:F2F2: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2F5: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F2F7: 80 0B       | BRA $F304          [M=16-bit, X=16-bit] */
    goto L_f304;
L_f2f9: /* [M=16-bit, X=16-bit] */
    /* $03:F2F9: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2FB: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F2FC: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F2FF: 09 80 00    | ORA #$0080         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F302: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
L_f304: /* [M=16-bit, X=16-bit] */
    /* $03:F304: A2 C0 0B    | LDX #$0BC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0BC0u;
    snes_set_nz16(ctx, ctx->x);
L_f307: /* [M=16-bit, X=16-bit] */
    /* $03:F307: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F309: F0 79       | BEQ $F384          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f384;
    /* $03:F30B: 15 02       | ORA $02,X          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F30D: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $03:F30F: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F311: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F313: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F316: 90 09       | BCC $F321          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f321;
    /* $03:F318: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F31A: 29 FB FF    | AND #$FFFB         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFBu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F31D: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F31F: 80 5C       | BRA $F37D          [M=16-bit, X=16-bit] */
    goto L_f37d;
L_f321: /* [M=16-bit, X=16-bit] */
    /* $03:F321: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F323: 09 04 00    | ORA #$0004         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F326: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F328: A4 10       | LDY $10            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F32A: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F32B: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F32D: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:F330: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:F332: 30 49       | BMI $F37D          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f37d;
    /* $03:F334: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:F336: 10 45       | BPL $F37D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f37d;
    /* $03:F338: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:F33B: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F33C: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F33E: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:F341: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:F344: 10 37       | BPL $F37D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f37d;
    /* $03:F346: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:F349: 30 32       | BMI $F37D          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f37d;
    /* $03:F34B: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:F34D: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F34F: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:F352: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F354: A5 04       | LDA $04            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F356: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:F359: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F35A: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F35B: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F35C: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F35D: 84 10       | STY $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->y));
    /* $03:F35F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F360: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:F362: 90 10       | BCC $F374          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f374;
    /* $03:F364: A4 12       | LDY $12            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F366: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F368: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:F36B: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F36C: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F36D: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F370: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F372: 84 12       | STY $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->y));
L_f374: /* [M=16-bit, X=16-bit] */
    /* $03:F374: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F376: 09 01 00    | ORA #$0001         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0001u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F379: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F37B: 80 07       | BRA $F384          [M=16-bit, X=16-bit] */
    goto L_f384;
L_f37d: /* [M=16-bit, X=16-bit] */
    /* $03:F37D: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F37F: 29 FE FF    | AND #$FFFE         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFEu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F382: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_f384: /* [M=16-bit, X=16-bit] */
    /* $03:F384: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F385: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F386: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:F389: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F38A: E0 C0 0E    | CPX #$0EC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0EC0u);
    /* $03:F38D: B0 03       | BCS $F392          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f392;
    /* $03:F38F: 82 75 FF    | BRL $F307          [M=16-bit, X=16-bit] */
    goto L_f307;
L_f392: /* [M=16-bit, X=16-bit] */
    /* $03:F392: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_f394: /* [M=16-bit, X=16-bit] */
    /* $03:F394: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F395: 90 FD       | BCC $F394          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f394;
    /* $03:F397: 09 AA AA    | ORA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a |= 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F39A: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:F39C: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F39E: E0 30 11    | CPX #$1130         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1130u);
    /* $03:F3A1: B0 0E       | BCS $F3B1          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f3b1;
    /* $03:F3A3: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_f3a6: /* [M=16-bit, X=16-bit] */
    /* $03:F3A6: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:F3A8: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F3A9: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F3AA: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F3AB: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F3AC: E0 30 11    | CPX #$1130         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1130u);
    /* $03:F3AF: 90 F5       | BCC $F3A6          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f3a6;
L_f3b1: /* [M=16-bit, X=16-bit] */
    /* $03:F3B1: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f3b2  (SNES $03:F3B2 | Ghidra 0x3f3b2)
 * Discovery Provenance: JML $03F3B2 @ $03:E193
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f3b2(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    goto L_f3b2;
L_e4a5: /* [M=16-bit, X=16-bit] */
    /* $03:E4A5: A0 C0 0F    | LDY #$0FC0         [M=16-bit, X=16-bit] */
    ctx->y = 0x0FC0u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4A8: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4AB: 8D C0 11    | STA $11C0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C0u), (uint16_t)(ctx->a));
    /* $03:E4AE: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4B1: 8D C2 11    | STA $11C2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C2u), (uint16_t)(ctx->a));
    /* $03:E4B4: AD 8A 1F    | LDA $1F8A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1F8Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4B7: D0 03       | BNE $E4BC          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e4bc;
    /* $03:E4B9: 82 0C 00    | BRL $E4C8          [M=16-bit, X=16-bit] */
    goto L_e4c8;
L_e4bc: /* [M=16-bit, X=16-bit] */
    /* $03:E4BC: A2 C0 0E    | LDX #$0EC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0EC0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E4BF: 20 EF E4    | JSR $E4EF          [M=16-bit, X=16-bit] */
    bank03_func_e4ef(ctx);
    /* $03:E4C2: 20 49 E5    | JSR $E549          [M=16-bit, X=16-bit] */
    bank03_func_e549(ctx);
    /* $03:E4C5: 20 89 E5    | JSR $E589          [M=16-bit, X=16-bit] */
    bank03_func_e589(ctx);
L_e4c8: /* [M=16-bit, X=16-bit] */
    /* $03:E4C8: AD CA 1F    | LDA $1FCA          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1FCAu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E4CB: D0 03       | BNE $E4D0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_e4d0;
    /* $03:E4CD: 82 09 00    | BRL $E4D9          [M=16-bit, X=16-bit] */
    goto L_e4d9;
L_e4d0: /* [M=16-bit, X=16-bit] */
    /* $03:E4D0: A2 00 0F    | LDX #$0F00         [M=16-bit, X=16-bit] */
    ctx->x = 0x0F00u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:E4D3: 20 EF E4    | JSR $E4EF          [M=16-bit, X=16-bit] */
    bank03_func_e4ef(ctx);
    /* $03:E4D6: 20 69 E5    | JSR $E569          [M=16-bit, X=16-bit] */
    bank03_func_e569(ctx);
L_e4d9: /* [M=16-bit, X=16-bit] */
    /* $03:E4D9: 20 CB E5    | JSR $E5CB          [M=16-bit, X=16-bit] */
    bank03_func_e5cb(ctx);
    /* $03:E4DC: 20 0D E6    | JSR $E60D          [M=16-bit, X=16-bit] */
    bank03_func_e60d(ctx);
    /* $03:E4DF: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_e4e2: /* [M=16-bit, X=16-bit] */
    /* $03:E4E2: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:E4E5: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4E6: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4E7: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4E8: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E4E9: C0 C0 11    | CPY #$11C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->y, 0x11C0u);
    /* $03:E4EC: 90 F4       | BCC $E4E2          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_e4e2;
    /* $03:E4EE: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_e682: /* [M=16-bit, X=16-bit] */
    /* $03:E682: A0 C0 0F    | LDY #$0FC0         [M=16-bit, X=16-bit] */
    ctx->y = 0x0FC0u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:E685: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E688: 8D C0 11    | STA $11C0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C0u), (uint16_t)(ctx->a));
    /* $03:E68B: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E68E: 8D C2 11    | STA $11C2          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C2u), (uint16_t)(ctx->a));
    /* $03:E691: A9 AA AA    | LDA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a = 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E694: 8D C4 11    | STA $11C4          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C4u), (uint16_t)(ctx->a));
    /* $03:E697: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:E69A: 8D C6 11    | STA $11C6          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C6u), (uint16_t)(ctx->a));
    /* $03:E69D: AD 8E 15    | LDA $158E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x158Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6A0: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:E6A1: F0 03       | BEQ $E6A6          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_e6a6;
    /* $03:E6A3: 82 0C 00    | BRL $E6B2          [M=16-bit, X=16-bit] */
    goto L_e6b2;
L_e6a6: /* [M=16-bit, X=16-bit] */
    /* $03:E6A6: 20 7C E8    | JSR $E87C          [M=16-bit, X=16-bit] */
    bank03_func_e87c(ctx);
    /* $03:E6A9: 20 C2 E6    | JSR $E6C2          [M=16-bit, X=16-bit] */
    bank03_func_e6c2(ctx);
    /* $03:E6AC: 20 9B E7    | JSR $E79B          [M=16-bit, X=16-bit] */
    bank03_func_e79b(ctx);
    /* $03:E6AF: 20 9E E8    | JSR $E89E          [M=16-bit, X=16-bit] */
    bank03_func_e89e(ctx);
L_e6b2: /* [M=16-bit, X=16-bit] */
    /* $03:E6B2: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_e6b5: /* [M=16-bit, X=16-bit] */
    /* $03:E6B5: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:E6B8: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6B9: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6BA: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6BB: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:E6BC: C0 C0 11    | CPY #$11C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->y, 0x11C0u);
    /* $03:E6BF: 90 F4       | BCC $E6B5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_e6b5;
    /* $03:E6C1: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_f3b2: /* [M=16-bit, X=16-bit] */
    /* $03:F3B2: C2 30       | REP #$30           [M=16-bit, X=16-bit] */
    /* REP #$30 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x30u);
    /* $03:F3B4: AD 94 15    | LDA $1594          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1594u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F3B7: C9 02 00    | CMP #$0002         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0002u);
    /* $03:F3BA: F0 4C       | BEQ $F408          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f408;
    /* $03:F3BC: C9 06 00    | CMP #$0006         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0006u);
    /* $03:F3BF: D0 03       | BNE $F3C4          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f3c4;
    /* $03:F3C1: 82 76 00    | BRL $F43A          [M=16-bit, X=16-bit] */
    goto L_f43a;
L_f3c4: /* [M=16-bit, X=16-bit] */
    /* $03:F3C4: AD 8E 15    | LDA $158E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x158Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F3C7: F0 03       | BEQ $F3CC          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f3cc;
    /* $03:F3C9: 82 B6 F2    | BRL $E682          [M=16-bit, X=16-bit] */
    goto L_e682;
L_f3cc: /* [M=16-bit, X=16-bit] */
    /* $03:F3CC: AD 90 15    | LDA $1590          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1590u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F3CF: F0 03       | BEQ $F3D4          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f3d4;
    /* $03:F3D1: 82 D1 F0    | BRL $E4A5          [M=16-bit, X=16-bit] */
    goto L_e4a5;
L_f3d4: /* [M=16-bit, X=16-bit] */
    /* $03:F3D4: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F3D6: A9 07       | LDA #$07           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x07u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:F3D8: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F3D9: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:F3DA: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F3DC: 20 82 F4    | JSR $F482          [M=16-bit, X=16-bit] */
    bank03_func_f482(ctx);
    /* $03:F3DF: 20 12 F5    | JSR $F512          [M=16-bit, X=16-bit] */
    bank03_func_f512(ctx);
    /* $03:F3E2: 20 43 F5    | JSR $F543          [M=16-bit, X=16-bit] */
    bank03_func_f543(ctx);
    /* $03:F3E5: 20 71 F4    | JSR $F471          [M=16-bit, X=16-bit] */
    bank03_func_f471(ctx);
    /* $03:F3E8: B0 06       | BCS $F3F0          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f3f0;
    /* $03:F3EA: 20 59 F5    | JSR $F559          [M=16-bit, X=16-bit] */
    bank03_func_f559(ctx);
    /* $03:F3ED: 20 E2 F5    | JSR $F5E2          [M=16-bit, X=16-bit] */
    bank03_func_f5e2(ctx);
L_f3f0: /* [M=16-bit, X=16-bit] */
    /* $03:F3F0: 20 0C F6    | JSR $F60C          [M=16-bit, X=16-bit] */
    bank03_func_f60c(ctx);
    /* $03:F3F3: 20 29 F6    | JSR $F629          [M=16-bit, X=16-bit] */
    bank03_func_f629(ctx);
    /* $03:F3F6: 20 FD F6    | JSR $F6FD          [M=16-bit, X=16-bit] */
    bank03_func_f6fd(ctx);
    /* $03:F3F9: 20 91 FB    | JSR $FB91          [M=16-bit, X=16-bit] */
    bank03_func_fb91(ctx);
    /* $03:F3FC: 20 FB FB    | JSR $FBFB          [M=16-bit, X=16-bit] */
    bank03_func_fbfb(ctx);
    /* $03:F3FF: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F401: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:F403: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F404: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:F405: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F407: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_f408: /* [M=16-bit, X=16-bit] */
    /* $03:F408: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F40A: A9 07       | LDA #$07           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x07u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:F40C: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F40D: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:F40E: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F410: 20 82 F4    | JSR $F482          [M=16-bit, X=16-bit] */
    bank03_func_f482(ctx);
    /* $03:F413: 20 12 F5    | JSR $F512          [M=16-bit, X=16-bit] */
    bank03_func_f512(ctx);
    /* $03:F416: 20 43 F5    | JSR $F543          [M=16-bit, X=16-bit] */
    bank03_func_f543(ctx);
    /* $03:F419: 20 59 F5    | JSR $F559          [M=16-bit, X=16-bit] */
    bank03_func_f559(ctx);
    /* $03:F41C: 20 E2 F5    | JSR $F5E2          [M=16-bit, X=16-bit] */
    bank03_func_f5e2(ctx);
    /* $03:F41F: 20 0C F6    | JSR $F60C          [M=16-bit, X=16-bit] */
    bank03_func_f60c(ctx);
    /* $03:F422: 20 CD F7    | JSR $F7CD          [M=16-bit, X=16-bit] */
    bank03_func_f7cd(ctx);
    /* $03:F425: 20 90 F8    | JSR $F890          [M=16-bit, X=16-bit] */
    bank03_func_f890(ctx);
    /* $03:F428: 20 44 F9    | JSR $F944          [M=16-bit, X=16-bit] */
    bank03_func_f944(ctx);
    /* $03:F42B: 20 91 FB    | JSR $FB91          [M=16-bit, X=16-bit] */
    bank03_func_fb91(ctx);
    /* $03:F42E: 20 FB FB    | JSR $FBFB          [M=16-bit, X=16-bit] */
    bank03_func_fbfb(ctx);
    /* $03:F431: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F433: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:F435: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F436: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:F437: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F439: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
L_f43a: /* [M=16-bit, X=16-bit] */
    /* $03:F43A: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F43C: A9 07       | LDA #$07           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x07u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:F43E: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F43F: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:F440: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F442: 20 82 F4    | JSR $F482          [M=16-bit, X=16-bit] */
    bank03_func_f482(ctx);
    /* $03:F445: 20 12 F5    | JSR $F512          [M=16-bit, X=16-bit] */
    bank03_func_f512(ctx);
    /* $03:F448: 20 43 F5    | JSR $F543          [M=16-bit, X=16-bit] */
    bank03_func_f543(ctx);
    /* $03:F44B: 20 71 F4    | JSR $F471          [M=16-bit, X=16-bit] */
    bank03_func_f471(ctx);
    /* $03:F44E: B0 06       | BCS $F456          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f456;
    /* $03:F450: 20 59 F5    | JSR $F559          [M=16-bit, X=16-bit] */
    bank03_func_f559(ctx);
    /* $03:F453: 20 E2 F5    | JSR $F5E2          [M=16-bit, X=16-bit] */
    bank03_func_f5e2(ctx);
L_f456: /* [M=16-bit, X=16-bit] */
    /* $03:F456: 20 0C F6    | JSR $F60C          [M=16-bit, X=16-bit] */
    bank03_func_f60c(ctx);
    /* $03:F459: 20 4A FC    | JSR $FC4A          [M=16-bit, X=16-bit] */
    bank03_func_fc4a(ctx);
    /* $03:F45C: 20 9C FC    | JSR $FC9C          [M=16-bit, X=16-bit] */
    bank03_func_fc9c(ctx);
    /* $03:F45F: 20 7D FD    | JSR $FD7D          [M=16-bit, X=16-bit] */
    bank03_func_fd7d(ctx);
    /* $03:F462: 20 FD F6    | JSR $F6FD          [M=16-bit, X=16-bit] */
    bank03_func_f6fd(ctx);
    /* $03:F465: 20 FB FB    | JSR $FBFB          [M=16-bit, X=16-bit] */
    bank03_func_fbfb(ctx);
    /* $03:F468: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F46A: A9 05       | LDA #$05           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x05u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:F46C: 48          | PHA                [M=8-bit, X=16-bit] */
    snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F46D: AB          | PLB                [M=8-bit, X=16-bit] */
    ctx->db = snes_pull8(ctx);
    snes_set_nz8(ctx, ctx->db);
    /* $03:F46E: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F470: 6B          | RTL                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f471  (SNES $03:F471 | Ghidra 0x3f471)
 * Discovery Provenance: JSR $F471 @ $03:F44B
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f471(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F471: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F474: F0 03       | BEQ $F479          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f479;
    /* $03:F476: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:F477: D0 07       | BNE $F480          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f480;
L_f479: /* [M=16-bit, X=16-bit] */
    /* $03:F479: AD 2C 0F    | LDA $0F2C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0F2Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F47C: F0 02       | BEQ $F480          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f480;
    /* $03:F47E: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F47F: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_f480: /* [M=16-bit, X=16-bit] */
    /* $03:F480: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F481: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f482  (SNES $03:F482 | Ghidra 0x3f482)
 * Discovery Provenance: JSR $F482 @ $03:F410
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f482(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F482: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F485: F0 03       | BEQ $F48A          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f48a;
    /* $03:F487: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:F488: F0 27       | BEQ $F4B1          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f4b1;
L_f48a: /* [M=16-bit, X=16-bit] */
    /* $03:F48A: A9 08 00    | LDA #$0008         [M=16-bit, X=16-bit] */
    ctx->a = 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F48D: 8D 0C 00    | STA $000C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x000Cu), (uint16_t)(ctx->a));
    /* $03:F490: A9 E0 00    | LDA #$00E0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00E0u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F493: 8D 0E 00    | STA $000E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x000Eu), (uint16_t)(ctx->a));
    /* $03:F496: AD 4C 15    | LDA $154C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x154Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F499: 85 1C       | STA $1C            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $03:F49B: AD 4E 15    | LDA $154E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x154Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F49E: 85 1E       | STA $1E            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $03:F4A0: AD 50 15    | LDA $1550          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1550u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4A3: 85 18       | STA $18            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $03:F4A5: AD 52 15    | LDA $1552          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1552u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4A8: 85 1A       | STA $1A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $03:F4AA: AD 54 15    | LDA $1554          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1554u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4AD: 85 16       | STA $16            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x16u), (uint16_t)(ctx->a));
    /* $03:F4AF: 80 25       | BRA $F4D6          [M=16-bit, X=16-bit] */
    goto L_f4d6;
L_f4b1: /* [M=16-bit, X=16-bit] */
    /* $03:F4B1: A9 68 00    | LDA #$0068         [M=16-bit, X=16-bit] */
    ctx->a = 0x0068u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4B4: 8D 0C 00    | STA $000C          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x000Cu), (uint16_t)(ctx->a));
    /* $03:F4B7: A9 E0 00    | LDA #$00E0         [M=16-bit, X=16-bit] */
    ctx->a = 0x00E0u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4BA: 8D 0E 00    | STA $000E          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x000Eu), (uint16_t)(ctx->a));
    /* $03:F4BD: AD 4C 15    | LDA $154C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x154Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4C0: 85 1C       | STA $1C            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Cu), (uint16_t)(ctx->a));
    /* $03:F4C2: AD 4E 15    | LDA $154E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x154Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4C5: 85 1E       | STA $1E            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $03:F4C7: AD 50 15    | LDA $1550          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1550u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4CA: 85 18       | STA $18            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x18u), (uint16_t)(ctx->a));
    /* $03:F4CC: AD 52 15    | LDA $1552          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1552u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4CF: 85 1A       | STA $1A            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $03:F4D1: AD 54 15    | LDA $1554          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1554u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4D4: 85 16       | STA $16            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x16u), (uint16_t)(ctx->a));
L_f4d6: /* [M=16-bit, X=16-bit] */
    /* $03:F4D6: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:F4D7: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:F4D8: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:F4D9: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F4DA: 29 06 00    | AND #$0006         [M=16-bit, X=16-bit] */
    ctx->a &= 0x0006u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4DD: 85 DA       | STA $DA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xDAu), (uint16_t)(ctx->a));
    /* $03:F4DF: A5 16       | LDA $16            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4E1: 29 3F 00    | AND #$003F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4E4: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F4E5: 85 D4       | STA $D4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD4u), (uint16_t)(ctx->a));
    /* $03:F4E7: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F4E8: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F4E9: 65 D4       | ADC $D4            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD4u)));
    /* $03:F4EB: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F4EC: 69 00 80    | ADC #$8000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x8000u);
    /* $03:F4EF: 85 D4       | STA $D4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD4u), (uint16_t)(ctx->a));
    /* $03:F4F1: A5 16       | LDA $16            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4F3: 29 3F 00    | AND #$003F         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4F6: 49 3F 00    | EOR #$003F         [M=16-bit, X=16-bit] */
    ctx->a ^= 0x003Fu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4F9: 1A          | INC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a + 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:F4FA: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F4FB: 85 D7       | STA $D7            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD7u), (uint16_t)(ctx->a));
    /* $03:F4FD: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F4FE: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F4FF: 65 D7       | ADC $D7            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD7u)));
    /* $03:F501: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F502: 69 00 80    | ADC #$8000         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x8000u);
    /* $03:F505: 85 D7       | STA $D7            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD7u), (uint16_t)(ctx->a));
    /* $03:F507: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F509: A9 03       | LDA #$03           [M=8-bit, X=16-bit] */
    { uint8_t v8 = 0x03u; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }
    /* $03:F50B: 85 D6       | STA $D6            [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0xD6u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:F50D: 85 D9       | STA $D9            [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dp(ctx, 0xD9u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:F50F: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F511: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f512  (SNES $03:F512 | Ghidra 0x3f512)
 * Discovery Provenance: JSR $F512 @ $03:F413
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f512(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F512: AD 8A 02    | LDA $028A          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F515: 8D C8 0F    | STA $0FC8          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FC8u), (uint16_t)(ctx->a));
    /* $03:F518: AD 8E 02    | LDA $028E          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x028Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F51B: 8D C9 0F    | STA $0FC9          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FC9u), (uint16_t)(ctx->a));
    /* $03:F51E: AD 80 02    | LDA $0280          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0280u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F521: 8D CA 0F    | STA $0FCA          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FCAu), (uint16_t)(ctx->a));
    /* $03:F524: AD CA 02    | LDA $02CA          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x02CAu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F527: 8D CC 0F    | STA $0FCC          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FCCu), (uint16_t)(ctx->a));
    /* $03:F52A: AD CE 02    | LDA $02CE          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x02CEu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F52D: 8D CD 0F    | STA $0FCD          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FCDu), (uint16_t)(ctx->a));
    /* $03:F530: AD C0 02    | LDA $02C0          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x02C0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F533: 8D CE 0F    | STA $0FCE          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x0FCEu), (uint16_t)(ctx->a));
    /* $03:F536: AD C0 11    | LDA $11C0          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x11C0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F539: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F53C: 09 AA 00    | ORA #$00AA         [M=16-bit, X=16-bit] */
    ctx->a |= 0x00AAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F53F: 8D C0 11    | STA $11C0          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_abs(ctx, 0x11C0u), (uint16_t)(ctx->a));
    /* $03:F542: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f543  (SNES $03:F543 | Ghidra 0x3f543)
 * Discovery Provenance: JSR $F543 @ $03:F416
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f543(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F543: A2 D0 0F    | LDX #$0FD0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0FD0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F546: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F548: A2 C0 11    | LDX #$11C0         [M=16-bit, X=16-bit] */
    ctx->x = 0x11C0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F54B: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F54D: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F54F: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F550: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F553: 09 80 00    | ORA #$0080         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F556: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F558: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f559  (SNES $03:F559 | Ghidra 0x3f559)
 * Discovery Provenance: JSR $F559 @ $03:F419
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f559(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F559: A2 00 03    | LDX #$0300         [M=16-bit, X=16-bit] */
    ctx->x = 0x0300u;
    snes_set_nz16(ctx, ctx->x);
L_f55c: /* [M=16-bit, X=16-bit] */
    /* $03:F55C: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F55E: F0 73       | BEQ $F5D3          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f5d3;
    /* $03:F560: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F562: 30 10       | BMI $F574          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f574;
    /* $03:F564: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F565: ED 54 15    | SBC $1554          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1554u)));
    /* $03:F568: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F56B: 15 00       | ORA $00,X          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F56D: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:F56E: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F56F: B9 C8 80    | LDA $80C8,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x80C8u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F572: 80 04       | BRA $F578          [M=16-bit, X=16-bit] */
    goto L_f578;
L_f574: /* [M=16-bit, X=16-bit] */
    /* $03:F574: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F576: F0 5B       | BEQ $F5D3          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f5d3;
L_f578: /* [M=16-bit, X=16-bit] */
    /* $03:F578: 15 02       | ORA $02,X          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F57A: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $03:F57C: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F57E: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F580: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F583: B0 4E       | BCS $F5D3          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f5d3;
    /* $03:F585: A4 10       | LDY $10            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F587: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F588: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F58A: F5 16       | SBC $16,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x16u)));
    /* $03:F58C: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:F58E: 30 43       | BMI $F5D3          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f5d3;
    /* $03:F590: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:F592: 10 3F       | BPL $F5D3          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f5d3;
    /* $03:F594: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:F597: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F598: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F59A: F5 16       | SBC $16,X          [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x16u)));
    /* $03:F59C: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:F59F: 10 32       | BPL $F5D3          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_f5d3;
    /* $03:F5A1: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:F5A4: 30 2D       | BMI $F5D3          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_f5d3;
    /* $03:F5A6: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:F5A8: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:F5AA: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:F5AD: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:F5AF: A5 04       | LDA $04            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F5B1: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:F5B4: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F5B5: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F5B6: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F5B7: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F5B8: 84 10       | STY $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->y));
    /* $03:F5BA: B5 16       | LDA $16,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x16u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F5BC: C9 08 00    | CMP #$0008         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0008u);
    /* $03:F5BF: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:F5C1: 90 10       | BCC $F5D3          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f5d3;
    /* $03:F5C3: A4 12       | LDY $12            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F5C5: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F5C7: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:F5CA: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F5CB: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F5CC: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F5CF: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F5D1: 84 12       | STY $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->y));
L_f5d3: /* [M=16-bit, X=16-bit] */
    /* $03:F5D3: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F5D4: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F5D5: 69 20 00    | ADC #$0020         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0020u);
    /* $03:F5D8: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F5D9: E0 80 09    | CPX #$0980         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0980u);
    /* $03:F5DC: B0 03       | BCS $F5E1          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f5e1;
    /* $03:F5DE: 82 7B FF    | BRL $F55C          [M=16-bit, X=16-bit] */
    goto L_f55c;
L_f5e1: /* [M=16-bit, X=16-bit] */
    /* $03:F5E1: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f5e2  (SNES $03:F5E2 | Ghidra 0x3f5e2)
 * Discovery Provenance: JSR $F5E2 @ $03:F41C
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f5e2(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F5E2: A2 80 09    | LDX #$0980         [M=16-bit, X=16-bit] */
    ctx->x = 0x0980u;
    snes_set_nz16(ctx, ctx->x);
L_f5e5: /* [M=16-bit, X=16-bit] */
    /* $03:F5E5: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F5E7: F0 17       | BEQ $F600          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f600;
    /* $03:F5E9: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F5EB: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F5ED: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F5F0: B0 0E       | BCS $F600          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f600;
    /* $03:F5F2: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F5F4: 09 00 01    | ORA #$0100         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0100u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F5F7: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:F5F9: B4 00       | LDY $00,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F5FB: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:F5FC: 20 11 FB    | JSR $FB11          [M=16-bit, X=16-bit] */
    bank03_func_fb11(ctx);
    /* $03:F5FF: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_f600: /* [M=16-bit, X=16-bit] */
    /* $03:F600: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F601: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F602: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:F605: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F606: E0 C0 0A    | CPX #$0AC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0AC0u);
    /* $03:F609: 90 DA       | BCC $F5E5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f5e5;
    /* $03:F60B: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f60c  (SNES $03:F60C | Ghidra 0x3f60c)
 * Discovery Provenance: JSR $F60C @ $03:F41F
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f60c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F60C: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_f60e: /* [M=16-bit, X=16-bit] */
    /* $03:F60E: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F60F: 90 FD       | BCC $F60E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f60e;
    /* $03:F611: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:F613: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F615: E0 C0 10    | CPX #$10C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10C0u);
    /* $03:F618: B0 0E       | BCS $F628          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f628;
    /* $03:F61A: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_f61d: /* [M=16-bit, X=16-bit] */
    /* $03:F61D: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:F61F: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F620: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F621: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F622: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F623: E0 C0 10    | CPX #$10C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10C0u);
    /* $03:F626: 90 F5       | BCC $F61D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f61d;
L_f628: /* [M=16-bit, X=16-bit] */
    /* $03:F628: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f629  (SNES $03:F629 | Ghidra 0x3f629)
 * Discovery Provenance: JSR $F629 @ $03:F3F3
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f629(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F629: A2 20 11    | LDX #$1120         [M=16-bit, X=16-bit] */
    ctx->x = 0x1120u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F62C: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F62E: A2 D6 11    | LDX #$11D6         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D6u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F631: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F633: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F636: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F638: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F63B: F0 03       | BEQ $F640          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f640;
    /* $03:F63D: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:F63E: D0 08       | BNE $F648          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f648;
L_f640: /* [M=16-bit, X=16-bit] */
    /* $03:F640: AD 2C 0F    | LDA $0F2C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0F2Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F643: F0 03       | BEQ $F648          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f648;
    /* $03:F645: 82 98 00    | BRL $F6E0          [M=16-bit, X=16-bit] */
    goto L_f6e0;
L_f648: /* [M=16-bit, X=16-bit] */
    /* $03:F648: 64 08       | STZ $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(0u));
    /* $03:F64A: A2 40 0D    | LDX #$0D40         [M=16-bit, X=16-bit] */
    ctx->x = 0x0D40u;
    snes_set_nz16(ctx, ctx->x);
L_f64d: /* [M=16-bit, X=16-bit] */
    /* $03:F64D: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F64F: F0 7D       | BEQ $F6CE          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f6ce;
    /* $03:F651: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:F652: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F653: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F656: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:F658: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F65B: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:F65D: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F65F: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F661: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F664: 90 0A       | BCC $F670          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f670;
    /* $03:F666: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F668: 29 F7 FF    | AND #$FFF7         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFF7u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F66B: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F66D: 82 5E 00    | BRL $F6CE          [M=16-bit, X=16-bit] */
    goto L_f6ce;
L_f670: /* [M=16-bit, X=16-bit] */
    /* $03:F670: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F672: 09 08 00    | ORA #$0008         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F675: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F677: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F679: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F67A: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F67B: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F67C: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:F67E: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F680: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F681: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:F683: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F684: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:F687: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F688: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F689: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F68C: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F68D: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:F68F: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F690: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F692: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:F693: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F695: 1D BC 80    | ORA $80BC,X        [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_absx(ctx, 0x80BCu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F698: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:F69A: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F69C: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:F69D: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F69F: BC 9C 80    | LDY $809C,X        [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x809Cu));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F6A2: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F6A3: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F6A6: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:F6A9: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F6AC: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:F6AF: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F6B2: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:F6B5: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F6B6: 20 93 FA    | JSR $FA93          [M=16-bit, X=16-bit] */
    bank03_func_fa93(ctx);
    /* $03:F6B9: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F6BA: A5 D0       | LDA $D0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F6BC: D0 09       | BNE $F6C7          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f6c7;
    /* $03:F6BE: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F6C0: 29 FD FF    | AND #$FFFD         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFDu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F6C3: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F6C5: 80 07       | BRA $F6CE          [M=16-bit, X=16-bit] */
    goto L_f6ce;
L_f6c7: /* [M=16-bit, X=16-bit] */
    /* $03:F6C7: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F6C9: 09 02 00    | ORA #$0002         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F6CC: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_f6ce: /* [M=16-bit, X=16-bit] */
    /* $03:F6CE: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F6D0: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F6D2: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F6D3: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F6D4: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:F6D7: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F6D8: E0 C0 0E    | CPX #$0EC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0EC0u);
    /* $03:F6DB: B0 03       | BCS $F6E0          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f6e0;
    /* $03:F6DD: 82 6D FF    | BRL $F64D          [M=16-bit, X=16-bit] */
    goto L_f64d;
L_f6e0: /* [M=16-bit, X=16-bit] */
    /* $03:F6E0: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_f6e2: /* [M=16-bit, X=16-bit] */
    /* $03:F6E2: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F6E3: 90 FD       | BCC $F6E2          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f6e2;
    /* $03:F6E5: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:F6E7: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F6E9: E0 80 11    | CPX #$1180         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1180u);
    /* $03:F6EC: B0 0E       | BCS $F6FC          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f6fc;
    /* $03:F6EE: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_f6f1: /* [M=16-bit, X=16-bit] */
    /* $03:F6F1: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:F6F3: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F6F4: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F6F5: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F6F6: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F6F7: E0 80 11    | CPX #$1180         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1180u);
    /* $03:F6FA: 90 F5       | BCC $F6F1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f6f1;
L_f6fc: /* [M=16-bit, X=16-bit] */
    /* $03:F6FC: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f6fd  (SNES $03:F6FD | Ghidra 0x3f6fd)
 * Discovery Provenance: JSR $F6FD @ $03:F462
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f6fd(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F6FD: A2 A0 11    | LDX #$11A0         [M=16-bit, X=16-bit] */
    ctx->x = 0x11A0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F700: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F702: A2 DE 11    | LDX #$11DE         [M=16-bit, X=16-bit] */
    ctx->x = 0x11DEu;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F705: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F707: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F70A: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F70C: 64 08       | STZ $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(0u));
    /* $03:F70E: A2 C0 0E    | LDX #$0EC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0EC0u;
    snes_set_nz16(ctx, ctx->x);
L_f711: /* [M=16-bit, X=16-bit] */
    /* $03:F711: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F714: F0 0A       | BEQ $F720          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f720;
    /* $03:F716: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:F717: F0 07       | BEQ $F720          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f720;
    /* $03:F719: B5 2C       | LDA $2C,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F71B: F0 10       | BEQ $F72D          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f72d;
    /* $03:F71D: 82 7E 00    | BRL $F79E          [M=16-bit, X=16-bit] */
    goto L_f79e;
L_f720: /* [M=16-bit, X=16-bit] */
    /* $03:F720: E0 00 0F    | CPX #$0F00         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0F00u);
    /* $03:F723: F0 08       | BEQ $F72D          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f72d;
    /* $03:F725: AD 2C 0F    | LDA $0F2C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0F2Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F728: 0D EC 0E    | ORA $0EEC          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0EECu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F72B: D0 71       | BNE $F79E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f79e;
L_f72d: /* [M=16-bit, X=16-bit] */
    /* $03:F72D: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F72F: F0 6D       | BEQ $F79E          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f79e;
    /* $03:F731: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:F732: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F733: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F736: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:F738: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F73B: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:F73D: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F73F: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F741: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F744: 90 0B       | BCC $F751          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f751;
    /* $03:F746: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F749: 9D B2 00    | STA $00B2,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B2u), (uint16_t)(ctx->a));
    /* $03:F74C: 9D B6 00    | STA $00B6,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B6u), (uint16_t)(ctx->a));
    /* $03:F74F: 80 4D       | BRA $F79E          [M=16-bit, X=16-bit] */
    goto L_f79e;
L_f751: /* [M=16-bit, X=16-bit] */
    /* $03:F751: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F753: 9D B2 00    | STA $00B2,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B2u), (uint16_t)(ctx->a));
    /* $03:F756: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F758: 9D B6 00    | STA $00B6,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B6u), (uint16_t)(ctx->a));
    /* $03:F75B: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F75D: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F75E: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F75F: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F760: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:F762: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F764: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F765: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:F767: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F768: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:F76B: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F76C: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F76D: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F770: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F771: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:F773: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F774: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F776: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:F777: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F779: 1D B8 80    | ORA $80B8,X        [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_absx(ctx, 0x80B8u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F77C: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:F77E: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F780: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:F781: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F783: BC 98 80    | LDY $8098,X        [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8098u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F786: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F787: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F78A: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:F78D: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F790: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:F793: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F796: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:F799: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F79A: 20 93 FA    | JSR $FA93          [M=16-bit, X=16-bit] */
    bank03_func_fa93(ctx);
    /* $03:F79D: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_f79e: /* [M=16-bit, X=16-bit] */
    /* $03:F79E: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F7A0: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F7A2: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F7A3: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F7A4: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:F7A7: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F7A8: E0 40 0F    | CPX #$0F40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0F40u);
    /* $03:F7AB: B0 03       | BCS $F7B0          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f7b0;
    /* $03:F7AD: 82 61 FF    | BRL $F711          [M=16-bit, X=16-bit] */
    goto L_f711;
L_f7b0: /* [M=16-bit, X=16-bit] */
    /* $03:F7B0: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_f7b2: /* [M=16-bit, X=16-bit] */
    /* $03:F7B2: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F7B3: 90 FD       | BCC $F7B2          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f7b2;
    /* $03:F7B5: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:F7B7: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F7B9: E0 C0 11    | CPX #$11C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x11C0u);
    /* $03:F7BC: B0 0E       | BCS $F7CC          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f7cc;
    /* $03:F7BE: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_f7c1: /* [M=16-bit, X=16-bit] */
    /* $03:F7C1: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:F7C3: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F7C4: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F7C5: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F7C6: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F7C7: E0 C0 11    | CPX #$11C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x11C0u);
    /* $03:F7CA: 90 F5       | BCC $F7C1          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f7c1;
L_f7cc: /* [M=16-bit, X=16-bit] */
    /* $03:F7CC: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f7cd  (SNES $03:F7CD | Ghidra 0x3f7cd)
 * Discovery Provenance: JSR $F7CD @ $03:F422
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f7cd(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F7CD: A2 D0 10    | LDX #$10D0         [M=16-bit, X=16-bit] */
    ctx->x = 0x10D0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F7D0: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F7D2: A2 D0 11    | LDX #$11D0         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F7D5: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F7D7: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F7DA: D0 07       | BNE $F7E3          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f7e3;
    /* $03:F7DC: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F7DF: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F7E1: 80 0B       | BRA $F7EE          [M=16-bit, X=16-bit] */
    goto L_f7ee;
L_f7e3: /* [M=16-bit, X=16-bit] */
    /* $03:F7E3: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F7E5: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F7E6: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F7E9: 09 80 00    | ORA #$0080         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F7EC: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
L_f7ee: /* [M=16-bit, X=16-bit] */
    /* $03:F7EE: A2 80 0B    | LDX #$0B80         [M=16-bit, X=16-bit] */
    ctx->x = 0x0B80u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F7F1: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F7F3: F0 7B       | BEQ $F870          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f870;
    /* $03:F7F5: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F7F7: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F7F9: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F7FC: 90 09       | BCC $F807          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f807;
    /* $03:F7FE: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F800: 29 F7 FF    | AND #$FFF7         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFF7u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F803: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F805: 80 69       | BRA $F870          [M=16-bit, X=16-bit] */
    goto L_f870;
L_f807: /* [M=16-bit, X=16-bit] */
    /* $03:F807: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F809: 09 08 00    | ORA #$0008         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F80C: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F80E: A0 04 00    | LDY #$0004         [M=16-bit, X=16-bit] */
    ctx->y = 0x0004u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F811: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F814: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:F816: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F819: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:F81B: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F81D: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F81E: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F81F: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F820: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:F822: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F824: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F825: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:F827: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F828: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:F82B: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F82C: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F82D: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F830: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F831: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:F833: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F834: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F836: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:F837: 0D BC 80    | ORA $80BC          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_abs(ctx, 0x80BCu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F83A: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:F83C: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F83E: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:F83F: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F841: AC 9C 80    | LDY $809C          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x809Cu));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F844: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F845: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F848: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:F84B: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F84E: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:F851: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F854: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:F857: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F858: 20 93 FA    | JSR $FA93          [M=16-bit, X=16-bit] */
    bank03_func_fa93(ctx);
    /* $03:F85B: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F85C: A5 D0       | LDA $D0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F85E: D0 09       | BNE $F869          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f869;
    /* $03:F860: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F862: 29 FD FF    | AND #$FFFD         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFDu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F865: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:F867: 80 07       | BRA $F870          [M=16-bit, X=16-bit] */
    goto L_f870;
L_f869: /* [M=16-bit, X=16-bit] */
    /* $03:F869: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F86B: 09 02 00    | ORA #$0002         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F86E: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_f870: /* [M=16-bit, X=16-bit] */
    /* $03:F870: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_f872: /* [M=16-bit, X=16-bit] */
    /* $03:F872: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F873: 90 FD       | BCC $F872          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f872;
    /* $03:F875: 09 AA AA    | ORA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a |= 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F878: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:F87A: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F87C: E0 E0 10    | CPX #$10E0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10E0u);
    /* $03:F87F: B0 0E       | BCS $F88F          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f88f;
    /* $03:F881: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_f884: /* [M=16-bit, X=16-bit] */
    /* $03:F884: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:F886: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F887: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F888: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F889: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F88A: E0 E0 10    | CPX #$10E0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x10E0u);
    /* $03:F88D: 90 F5       | BCC $F884          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f884;
L_f88f: /* [M=16-bit, X=16-bit] */
    /* $03:F88F: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f890  (SNES $03:F890 | Ghidra 0x3f890)
 * Discovery Provenance: JSR $F890 @ $03:F425
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f890(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F890: A2 00 11    | LDX #$1100         [M=16-bit, X=16-bit] */
    ctx->x = 0x1100u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F893: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F895: A2 D4 11    | LDX #$11D4         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D4u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F898: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F89A: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F89D: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F89F: 64 08       | STZ $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(0u));
    /* $03:F8A1: A2 C0 0E    | LDX #$0EC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0EC0u;
    snes_set_nz16(ctx, ctx->x);
L_f8a4: /* [M=16-bit, X=16-bit] */
    /* $03:F8A4: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8A6: F0 6D       | BEQ $F915          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f915;
    /* $03:F8A8: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:F8A9: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F8AA: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8AD: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:F8AF: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8B2: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:F8B4: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F8B6: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8B8: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F8BB: 90 0B       | BCC $F8C8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f8c8;
    /* $03:F8BD: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8C0: 9D B2 00    | STA $00B2,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B2u), (uint16_t)(ctx->a));
    /* $03:F8C3: 9D B6 00    | STA $00B6,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B6u), (uint16_t)(ctx->a));
    /* $03:F8C6: 80 4D       | BRA $F915          [M=16-bit, X=16-bit] */
    goto L_f915;
L_f8c8: /* [M=16-bit, X=16-bit] */
    /* $03:F8C8: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8CA: 9D B2 00    | STA $00B2,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B2u), (uint16_t)(ctx->a));
    /* $03:F8CD: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8CF: 9D B6 00    | STA $00B6,X        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absx(ctx, 0x00B6u), (uint16_t)(ctx->a));
    /* $03:F8D2: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8D4: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F8D5: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F8D6: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F8D7: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:F8D9: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8DB: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F8DC: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:F8DE: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F8DF: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:F8E2: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F8E3: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F8E4: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8E7: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F8E8: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:F8EA: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F8EB: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8ED: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:F8EE: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F8F0: 1D B8 80    | ORA $80B8,X        [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_absx(ctx, 0x80B8u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8F3: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:F8F5: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8F7: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:F8F8: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F8FA: BC 98 80    | LDY $8098,X        [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8098u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F8FD: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F8FE: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F901: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:F904: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F907: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:F90A: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F90D: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:F910: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:F911: 20 93 FA    | JSR $FA93          [M=16-bit, X=16-bit] */
    bank03_func_fa93(ctx);
    /* $03:F914: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_f915: /* [M=16-bit, X=16-bit] */
    /* $03:F915: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F917: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F919: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F91A: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F91B: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:F91E: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F91F: E0 40 0F    | CPX #$0F40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0F40u);
    /* $03:F922: B0 03       | BCS $F927          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f927;
    /* $03:F924: 82 7D FF    | BRL $F8A4          [M=16-bit, X=16-bit] */
    goto L_f8a4;
L_f927: /* [M=16-bit, X=16-bit] */
    /* $03:F927: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_f929: /* [M=16-bit, X=16-bit] */
    /* $03:F929: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F92A: 90 FD       | BCC $F929          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f929;
    /* $03:F92C: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:F92E: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F930: E0 20 11    | CPX #$1120         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1120u);
    /* $03:F933: B0 0E       | BCS $F943          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f943;
    /* $03:F935: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_f938: /* [M=16-bit, X=16-bit] */
    /* $03:F938: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:F93A: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F93B: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F93C: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F93D: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:F93E: E0 20 11    | CPX #$1120         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1120u);
    /* $03:F941: 90 F5       | BCC $F938          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_f938;
L_f943: /* [M=16-bit, X=16-bit] */
    /* $03:F943: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_f944  (SNES $03:F944 | Ghidra 0x3f944)
 * Discovery Provenance: JSR $F944 @ $03:F428
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_f944(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:F944: A2 70 11    | LDX #$1170         [M=16-bit, X=16-bit] */
    ctx->x = 0x1170u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F947: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:F949: A2 DA 11    | LDX #$11DA         [M=16-bit, X=16-bit] */
    ctx->x = 0x11DAu;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F94C: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:F94E: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F951: D0 07       | BNE $F95A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f95a;
    /* $03:F953: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F956: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:F958: 80 0B       | BRA $F965          [M=16-bit, X=16-bit] */
    goto L_f965;
L_f95a: /* [M=16-bit, X=16-bit] */
    /* $03:F95A: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F95C: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:F95D: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F960: 09 80 00    | ORA #$0080         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F963: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
L_f965: /* [M=16-bit, X=16-bit] */
    /* $03:F965: A9 A8 10    | LDA #$10A8         [M=16-bit, X=16-bit] */
    ctx->a = 0x10A8u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F968: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:F96A: A9 40 00    | LDA #$0040         [M=16-bit, X=16-bit] */
    ctx->a = 0x0040u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F96D: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F96E: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:F970: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F971: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:F974: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F975: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F976: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F979: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F97A: B9 CE 9B    | LDA $9BCE,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x9BCEu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F97D: 85 CA       | STA $CA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCAu), (uint16_t)(ctx->a));
    /* $03:F97F: B9 0E 9C    | LDA $9C0E,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x9C0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F982: A0 60 00    | LDY #$0060         [M=16-bit, X=16-bit] */
    ctx->y = 0x0060u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F985: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F986: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F989: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:F98C: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F98F: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:F992: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F995: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:F998: A5 18       | LDA $18            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F99A: 29 80 FF    | AND #$FF80         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF80u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F99D: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:F99F: A5 1A       | LDA $1A            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9A1: 29 80 FF    | AND #$FF80         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF80u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9A4: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:F9A6: A9 12 00    | LDA #$0012         [M=16-bit, X=16-bit] */
    ctx->a = 0x0012u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9A9: 85 08       | STA $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(ctx->a));
L_f9ab: /* [M=16-bit, X=16-bit] */
    /* $03:F9AB: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:F9AD: A5 C4       | LDA $C4            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9AF: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F9B0: 7D FE 7F    | ADC $7FFE,X        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absx(ctx, 0x7FFEu)));
    /* $03:F9B3: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F9B4: A5 C6       | LDA $C6            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9B6: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:F9B7: 7D 10 80    | ADC $8010,X        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8010u)));
    /* $03:F9BA: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:F9BD: B0 05       | BCS $F9C4          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_f9c4;
    /* $03:F9BF: A4 CA       | LDY $CA            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCAu));
    snes_set_nz16(ctx, ctx->y);
    /* $03:F9C1: 20 93 FA    | JSR $FA93          [M=16-bit, X=16-bit] */
    bank03_func_fa93(ctx);
L_f9c4: /* [M=16-bit, X=16-bit] */
    /* $03:F9C4: C6 08       | DEC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F9C6: C6 08       | DEC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:F9C8: D0 E1       | BNE $F9AB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_f9ab;
    /* $03:F9CA: A9 04 00    | LDA #$0004         [M=16-bit, X=16-bit] */
    ctx->a = 0x0004u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9CD: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:F9CE: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:F9D0: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F9D1: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:F9D2: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9D5: A0 A4 10    | LDY #$10A4         [M=16-bit, X=16-bit] */
    ctx->y = 0x10A4u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F9D8: 89 20 00    | BIT #$0020         [M=16-bit, X=16-bit] */
    ctx->flag_z = (uint8_t)((ctx->a & (0x0020u)) == 0u ? 1u : 0u);
    /* $03:F9DB: F0 03       | BEQ $F9E0          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_f9e0;
    /* $03:F9DD: A0 A4 D0    | LDY #$D0A4         [M=16-bit, X=16-bit] */
    ctx->y = 0xD0A4u;
    snes_set_nz16(ctx, ctx->y);
L_f9e0: /* [M=16-bit, X=16-bit] */
    /* $03:F9E0: 84 06       | STY $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->y));
    /* $03:F9E2: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F9E3: B9 84 9C    | LDA $9C84,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x9C84u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9E6: A0 58 00    | LDY #$0058         [M=16-bit, X=16-bit] */
    ctx->y = 0x0058u;
    snes_set_nz16(ctx, ctx->y);
    /* $03:F9E9: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:F9EA: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9ED: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:F9F0: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9F3: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:F9F6: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9F9: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:F9FC: A5 18       | LDA $18            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x18u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:F9FE: 29 80 FF    | AND #$FF80         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF80u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA01: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:FA03: A5 1A       | LDA $1A            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x1Au));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA05: 29 80 FF    | AND #$FF80         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF80u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA08: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:FA0A: A2 32 00    | LDX #$0032         [M=16-bit, X=16-bit] */
    ctx->x = 0x0032u;
    snes_set_nz16(ctx, ctx->x);
L_fa0d: /* [M=16-bit, X=16-bit] */
    /* $03:FA0D: A5 C4       | LDA $C4            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA0F: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FA10: 7D 22 80    | ADC $8022,X        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8022u)));
    /* $03:FA13: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:FA14: A5 C6       | LDA $C6            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA16: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FA17: 7D 54 80    | ADC $8054,X        [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_absx(ctx, 0x8054u)));
    /* $03:FA1A: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:FA1D: B0 50       | BCS $FA6F          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_fa6f;
    /* $03:FA1F: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:FA20: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA22: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FA23: 6D A2 15    | ADC $15A2          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x15A2u)));
    /* $03:FA26: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:FA29: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:FA2B: 30 42       | BMI $FA6F          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fa6f;
    /* $03:FA2D: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:FA2F: 10 3E       | BPL $FA6F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fa6f;
    /* $03:FA31: A4 10       | LDY $10            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FA33: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:FA36: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:FA37: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA39: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:FA3C: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:FA3F: 10 2E       | BPL $FA6F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fa6f;
    /* $03:FA41: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:FA44: 30 29       | BMI $FA6F          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fa6f;
    /* $03:FA46: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:FA48: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:FA4A: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:FA4D: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:FA4F: A5 06       | LDA $06            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA51: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:FA54: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FA55: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FA56: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FA57: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FA58: 84 10       | STY $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->y));
    /* $03:FA5A: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:FA5B: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:FA5D: 90 10       | BCC $FA6F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fa6f;
    /* $03:FA5F: A4 12       | LDY $12            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FA61: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA63: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:FA66: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FA67: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FA68: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA6B: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:FA6D: 84 12       | STY $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->y));
L_fa6f: /* [M=16-bit, X=16-bit] */
    /* $03:FA6F: CA          | DEX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x - 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FA70: CA          | DEX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x - 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FA71: D0 9A       | BNE $FA0D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fa0d;
    /* $03:FA73: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_fa75: /* [M=16-bit, X=16-bit] */
    /* $03:FA75: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:FA76: 90 FD       | BCC $FA75          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fa75;
    /* $03:FA78: 09 AA AA    | ORA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a |= 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA7B: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:FA7D: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:FA7F: E0 C0 11    | CPX #$11C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x11C0u);
    /* $03:FA82: B0 0E       | BCS $FA92          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_fa92;
    /* $03:FA84: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_fa87: /* [M=16-bit, X=16-bit] */
    /* $03:FA87: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:FA89: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FA8A: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FA8B: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FA8C: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FA8D: E0 C0 11    | CPX #$11C0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x11C0u);
    /* $03:FA90: 90 F5       | BCC $FA87          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fa87;
L_fa92: /* [M=16-bit, X=16-bit] */
    /* $03:FA92: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_fa93  (SNES $03:FA93 | Ghidra 0x3fa93)
 * Discovery Provenance: JSR $FA93 @ $03:F9C1
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_fa93(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:FA93: 64 D0       | STZ $D0            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD0u), (uint16_t)(0u));
    /* $03:FA95: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FA97: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FA98: 6D A2 15    | ADC $15A2          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x15A2u)));
    /* $03:FA9B: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:FA9D: B9 00 00    | LDA $0000,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FAA0: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FAA3: 85 C8       | STA $C8            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC8u), (uint16_t)(ctx->a));
    /* $03:FAA5: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
L_faa7: /* [M=16-bit, X=16-bit] */
    /* $03:FAA7: 64 CD       | STZ $CD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(0u));
    /* $03:FAA9: B9 01 00    | LDA $0001,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0001u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FAAC: 10 02       | BPL $FAB0          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fab0;
    /* $03:FAAE: C6 CD       | DEC $CD            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(v16)); }
L_fab0: /* [M=16-bit, X=16-bit] */
    /* $03:FAB0: 85 CC       | STA $CC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCCu), (uint16_t)(ctx->a));
    /* $03:FAB2: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FAB3: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FAB5: 65 CD       | ADC $CD            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu)));
    /* $03:FAB7: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:FAB9: 30 4B       | BMI $FB06          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fb06;
    /* $03:FABB: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:FABD: 10 47       | BPL $FB06          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fb06;
    /* $03:FABF: 95 01       | STA $01,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x01u), (uint16_t)(ctx->a));
    /* $03:FAC1: 64 CD       | STZ $CD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(0u));
    /* $03:FAC3: B9 00 00    | LDA $0000,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FAC6: 10 02       | BPL $FACA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_faca;
    /* $03:FAC8: C6 CD       | DEC $CD            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(v16)); }
L_faca: /* [M=16-bit, X=16-bit] */
    /* $03:FACA: 85 CC       | STA $CC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCCu), (uint16_t)(ctx->a));
    /* $03:FACC: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FACD: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FACF: 65 CD       | ADC $CD            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu)));
    /* $03:FAD1: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:FAD4: 10 30       | BPL $FB06          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fb06;
    /* $03:FAD6: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:FAD9: 30 2B       | BMI $FB06          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fb06;
    /* $03:FADB: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:FADD: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:FADF: 95 00       | STA $00,X          [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dpx(ctx, 0x00u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:FAE1: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:FAE3: E6 D0       | INC $D0            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD0u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xD0u), (uint16_t)(v16)); }
    /* $03:FAE5: B9 03 00    | LDA $0003,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0003u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FAE8: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FAEA: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:FAEC: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FAED: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FAEE: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FAEF: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FAF0: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:FAF1: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:FAF3: 90 11       | BCC $FB06          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fb06;
    /* $03:FAF5: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:FAF6: A6 12       | LDX $12            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:FAF8: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FAFA: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:FAFC: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FAFD: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FAFE: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB01: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:FB03: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:FB05: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_fb06: /* [M=16-bit, X=16-bit] */
    /* $03:FB06: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FB07: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FB08: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FB09: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FB0A: C6 C8       | DEC $C8            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC8u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC8u), (uint16_t)(v16)); }
    /* $03:FB0C: D0 99       | BNE $FAA7          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_faa7;
    /* $03:FB0E: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:FB10: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_fb11  (SNES $03:FB11 | Ghidra 0x3fb11)
 * Discovery Provenance: JSR $FB11 @ $03:F5FC
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_fb11(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:FB11: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB13: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FB14: 6D A2 15    | ADC $15A2          [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_abs(ctx, 0x15A2u)));
    /* $03:FB17: 85 02       | STA $02            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:FB19: B9 00 00    | LDA $0000,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB1C: 29 FF 00    | AND #$00FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0x00FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB1F: 85 C8       | STA $C8            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC8u), (uint16_t)(ctx->a));
    /* $03:FB21: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
L_fb23: /* [M=16-bit, X=16-bit] */
    /* $03:FB23: 64 CD       | STZ $CD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(0u));
    /* $03:FB25: B9 01 00    | LDA $0001,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0001u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB28: 10 02       | BPL $FB2C          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fb2c;
    /* $03:FB2A: C6 CD       | DEC $CD            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(v16)); }
L_fb2c: /* [M=16-bit, X=16-bit] */
    /* $03:FB2C: 85 CC       | STA $CC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCCu), (uint16_t)(ctx->a));
    /* $03:FB2E: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FB2F: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB31: 65 CD       | ADC $CD            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu)));
    /* $03:FB33: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:FB35: 30 4F       | BMI $FB86          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fb86;
    /* $03:FB37: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:FB39: 10 4B       | BPL $FB86          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fb86;
    /* $03:FB3B: 95 01       | STA $01,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x01u), (uint16_t)(ctx->a));
    /* $03:FB3D: 64 CD       | STZ $CD            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(0u));
    /* $03:FB3F: B9 00 00    | LDA $0000,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB42: 10 02       | BPL $FB46          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fb46;
    /* $03:FB44: C6 CD       | DEC $CD            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCDu), (uint16_t)(v16)); }
L_fb46: /* [M=16-bit, X=16-bit] */
    /* $03:FB46: 85 CC       | STA $CC            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xCCu), (uint16_t)(ctx->a));
    /* $03:FB48: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FB49: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB4B: 65 CD       | ADC $CD            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xCDu)));
    /* $03:FB4D: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:FB50: 10 34       | BPL $FB86          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fb86;
    /* $03:FB52: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:FB55: 30 2F       | BMI $FB86          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fb86;
    /* $03:FB57: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:FB59: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:FB5B: 95 00       | STA $00,X          [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_dpx(ctx, 0x00u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:FB5D: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:FB5F: B9 03 00    | LDA $0003,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0003u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB62: 29 FF C0    | AND #$C0FF         [M=16-bit, X=16-bit] */
    ctx->a &= 0xC0FFu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB65: 05 06       | ORA $06            [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dp(ctx, 0x06u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB67: 95 02       | STA $02,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x02u), (uint16_t)(ctx->a));
    /* $03:FB69: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FB6A: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FB6B: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FB6C: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FB6D: B9 04 00    | LDA $0004,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB70: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:FB71: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:FB73: 90 11       | BCC $FB86          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fb86;
    /* $03:FB75: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:FB76: A6 12       | LDX $12            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:FB78: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB7A: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:FB7C: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FB7D: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FB7E: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB81: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:FB83: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:FB85: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
L_fb86: /* [M=16-bit, X=16-bit] */
    /* $03:FB86: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FB87: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FB88: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FB89: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FB8A: C6 C8       | DEC $C8            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0xC8u))) - 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC8u), (uint16_t)(v16)); }
    /* $03:FB8C: D0 95       | BNE $FB23          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fb23;
    /* $03:FB8E: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:FB90: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_fb91  (SNES $03:FB91 | Ghidra 0x3fb91)
 * Discovery Provenance: JSR $FB91 @ $03:F42B
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_fb91(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:FB91: A2 C0 0A    | LDX #$0AC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0AC0u;
    snes_set_nz16(ctx, ctx->x);
L_fb94: /* [M=16-bit, X=16-bit] */
    /* $03:FB94: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FB96: F0 57       | BEQ $FBEF          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_fbef;
    /* $03:FB98: E0 40 0D    | CPX #$0D40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0D40u);
    /* $03:FB9B: B0 07       | BCS $FBA4          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_fba4;
    /* $03:FB9D: C9 03 00    | CMP #$0003         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0003u);
    /* $03:FBA0: F0 4D       | BEQ $FBEF          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_fbef;
    /* $03:FBA2: 80 04       | BRA $FBA8          [M=16-bit, X=16-bit] */
    goto L_fba8;
L_fba4: /* [M=16-bit, X=16-bit] */
    /* $03:FBA4: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBA6: D0 47       | BNE $FBEF          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fbef;
L_fba8: /* [M=16-bit, X=16-bit] */
    /* $03:FBA8: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FBAA: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBAC: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:FBAF: 90 09       | BCC $FBBA          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fbba;
    /* $03:FBB1: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBB3: 29 F7 FF    | AND #$FFF7         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFF7u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBB6: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FBB8: 80 2E       | BRA $FBE8          [M=16-bit, X=16-bit] */
    goto L_fbe8;
L_fbba: /* [M=16-bit, X=16-bit] */
    /* $03:FBBA: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBBC: 09 08 00    | ORA #$0008         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBBF: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FBC1: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBC3: 95 1A       | STA $1A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Au), (uint16_t)(ctx->a));
    /* $03:FBC5: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBC7: 95 1E       | STA $1E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x1Eu), (uint16_t)(ctx->a));
    /* $03:FBC9: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBCB: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:FBCD: 30 19       | BMI $FBE8          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fbe8;
    /* $03:FBCF: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:FBD1: 10 15       | BPL $FBE8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fbe8;
    /* $03:FBD3: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBD5: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:FBD8: 10 0E       | BPL $FBE8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fbe8;
    /* $03:FBDA: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:FBDD: 30 09       | BMI $FBE8          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fbe8;
    /* $03:FBDF: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBE1: 09 02 00    | ORA #$0002         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBE4: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FBE6: 80 07       | BRA $FBEF          [M=16-bit, X=16-bit] */
    goto L_fbef;
L_fbe8: /* [M=16-bit, X=16-bit] */
    /* $03:FBE8: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBEA: 29 FD FF    | AND #$FFFD         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFDu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBED: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_fbef: /* [M=16-bit, X=16-bit] */
    /* $03:FBEF: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBF0: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FBF1: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:FBF4: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FBF5: E0 C0 0E    | CPX #$0EC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0EC0u);
    /* $03:FBF8: 90 9A       | BCC $FB94          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fb94;
    /* $03:FBFA: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_fbfb  (SNES $03:FBFB | Ghidra 0x3fbfb)
 * Discovery Provenance: JSR $FBFB @ $03:F42E
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_fbfb(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:FBFB: AD 2C 0F    | LDA $0F2C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0F2Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FBFE: D0 22       | BNE $FC22          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fc22;
    /* $03:FC00: A2 40 0F    | LDX #$0F40         [M=16-bit, X=16-bit] */
    ctx->x = 0x0F40u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FC03: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC05: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC06: D0 1F       | BNE $FC27          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fc27;
    /* $03:FC08: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FC0A: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC0C: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:FC0F: 90 09       | BCC $FC1A          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fc1a;
    /* $03:FC11: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC14: 95 3A       | STA $3A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(ctx->a));
    /* $03:FC16: 95 3E       | STA $3E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(ctx->a));
    /* $03:FC18: 80 0D       | BRA $FC27          [M=16-bit, X=16-bit] */
    goto L_fc27;
L_fc1a: /* [M=16-bit, X=16-bit] */
    /* $03:FC1A: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC1C: 95 3A       | STA $3A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(ctx->a));
    /* $03:FC1E: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC20: 95 3E       | STA $3E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(ctx->a));
L_fc22: /* [M=16-bit, X=16-bit] */
    /* $03:FC22: AD EC 0E    | LDA $0EEC          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0EECu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC25: D0 22       | BNE $FC49          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fc49;
L_fc27: /* [M=16-bit, X=16-bit] */
    /* $03:FC27: A2 80 0F    | LDX #$0F80         [M=16-bit, X=16-bit] */
    ctx->x = 0x0F80u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FC2A: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC2C: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC2D: D0 1A       | BNE $FC49          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fc49;
    /* $03:FC2F: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FC31: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC33: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:FC36: 90 09       | BCC $FC41          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fc41;
    /* $03:FC38: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC3B: 95 3A       | STA $3A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(ctx->a));
    /* $03:FC3D: 95 3E       | STA $3E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(ctx->a));
    /* $03:FC3F: 80 08       | BRA $FC49          [M=16-bit, X=16-bit] */
    goto L_fc49;
L_fc41: /* [M=16-bit, X=16-bit] */
    /* $03:FC41: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC43: 95 3A       | STA $3A,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Au), (uint16_t)(ctx->a));
    /* $03:FC45: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC47: 95 3E       | STA $3E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x3Eu), (uint16_t)(ctx->a));
L_fc49: /* [M=16-bit, X=16-bit] */
    /* $03:FC49: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_fc4a  (SNES $03:FC4A | Ghidra 0x3fc4a)
 * Discovery Provenance: JSR $FC4A @ $03:F459
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_fc4a(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:FC4A: A2 C0 0A    | LDX #$0AC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0AC0u;
    snes_set_nz16(ctx, ctx->x);
L_fc4d: /* [M=16-bit, X=16-bit] */
    /* $03:FC4D: B5 10       | LDA $10,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC4F: F0 3F       | BEQ $FC90          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_fc90;
    /* $03:FC51: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FC53: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC55: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:FC58: 90 09       | BCC $FC63          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fc63;
    /* $03:FC5A: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC5C: 29 F7 FF    | AND #$FFF7         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFF7u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC5F: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FC61: 80 26       | BRA $FC89          [M=16-bit, X=16-bit] */
    goto L_fc89;
L_fc63: /* [M=16-bit, X=16-bit] */
    /* $03:FC63: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC65: 09 08 00    | ORA #$0008         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC68: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FC6A: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC6C: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:FC6E: 30 19       | BMI $FC89          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fc89;
    /* $03:FC70: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:FC72: 10 15       | BPL $FC89          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fc89;
    /* $03:FC74: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC76: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:FC79: 10 0E       | BPL $FC89          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fc89;
    /* $03:FC7B: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:FC7E: 30 09       | BMI $FC89          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fc89;
    /* $03:FC80: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC82: 09 02 00    | ORA #$0002         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC85: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FC87: 80 07       | BRA $FC90          [M=16-bit, X=16-bit] */
    goto L_fc90;
L_fc89: /* [M=16-bit, X=16-bit] */
    /* $03:FC89: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC8B: 29 FD FF    | AND #$FFFD         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFDu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC8E: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_fc90: /* [M=16-bit, X=16-bit] */
    /* $03:FC90: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FC91: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FC92: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:FC95: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FC96: E0 40 0B    | CPX #$0B40         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0B40u);
    /* $03:FC99: 90 B2       | BCC $FC4D          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fc4d;
    /* $03:FC9B: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_fc9c  (SNES $03:FC9C | Ghidra 0x3fc9c)
 * Discovery Provenance: JSR $FC9C @ $03:F45C
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_fc9c(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:FC9C: A2 E0 10    | LDX #$10E0         [M=16-bit, X=16-bit] */
    ctx->x = 0x10E0u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FC9F: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:FCA1: A2 D2 11    | LDX #$11D2         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D2u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FCA4: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:FCA6: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FCA9: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:FCAB: 20 69 FD    | JSR $FD69          [M=16-bit, X=16-bit] */
    bank03_func_fd69(ctx);
    /* $03:FCAE: B0 03       | BCS $FCB3          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_fcb3;
    /* $03:FCB0: 20 D0 FC    | JSR $FCD0          [M=16-bit, X=16-bit] */
    bank03_func_fcd0(ctx);
L_fcb3: /* [M=16-bit, X=16-bit] */
    /* $03:FCB3: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_fcb5: /* [M=16-bit, X=16-bit] */
    /* $03:FCB5: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:FCB6: 90 FD       | BCC $FCB5          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fcb5;
    /* $03:FCB8: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:FCBA: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:FCBC: E0 00 11    | CPX #$1100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1100u);
    /* $03:FCBF: B0 0E       | BCS $FCCF          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_fccf;
    /* $03:FCC1: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_fcc4: /* [M=16-bit, X=16-bit] */
    /* $03:FCC4: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:FCC6: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FCC7: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FCC8: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FCC9: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FCCA: E0 00 11    | CPX #$1100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1100u);
    /* $03:FCCD: 90 F5       | BCC $FCC4          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fcc4;
L_fccf: /* [M=16-bit, X=16-bit] */
    /* $03:FCCF: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_fcd0  (SNES $03:FCD0 | Ghidra 0x3fcd0)
 * Discovery Provenance: JSR $FCD0 @ $03:FCB0
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_fcd0(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:FCD0: 64 08       | STZ $08            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(0u));
    /* $03:FCD2: A2 40 0B    | LDX #$0B40         [M=16-bit, X=16-bit] */
    ctx->x = 0x0B40u;
    snes_set_nz16(ctx, ctx->x);
L_fcd5: /* [M=16-bit, X=16-bit] */
    /* $03:FCD5: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FCD7: F0 7D       | BEQ $FD56          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_fd56;
    /* $03:FCD9: 0A          | ASL A              [M=16-bit, X=16-bit] */
    ctx->a = snes_asl16(ctx, ctx->a);
    /* $03:FCDA: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:FCDB: B9 06 88    | LDA $8806,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8806u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FCDE: 85 C4       | STA $C4            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC4u), (uint16_t)(ctx->a));
    /* $03:FCE0: B9 28 88    | LDA $8828,Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absy(ctx, 0x8828u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FCE3: 85 C6       | STA $C6            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xC6u), (uint16_t)(ctx->a));
    /* $03:FCE5: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FCE7: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FCE9: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:FCEC: 90 0A       | BCC $FCF8          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fcf8;
    /* $03:FCEE: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FCF0: 29 F7 FF    | AND #$FFF7         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFF7u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FCF3: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FCF5: 82 5E 00    | BRL $FD56          [M=16-bit, X=16-bit] */
    goto L_fd56;
L_fcf8: /* [M=16-bit, X=16-bit] */
    /* $03:FCF8: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FCFA: 09 08 00    | ORA #$0008         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FCFD: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FCFF: B5 20       | LDA $20,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x20u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD01: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:FD02: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:FD03: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:FD04: 85 FA       | STA $FA            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0xFAu), (uint16_t)(ctx->a));
    /* $03:FD06: B5 04       | LDA $04,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD08: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:FD09: E5 16       | SBC $16            [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x16u)));
    /* $03:FD0B: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FD0C: 69 04 00    | ADC #$0004         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0004u);
    /* $03:FD0F: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:FD10: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:FD11: 29 3E 00    | AND #$003E         [M=16-bit, X=16-bit] */
    ctx->a &= 0x003Eu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD14: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FD15: 65 FA       | ADC $FA            [M=16-bit, X=16-bit] */
    snes_adc16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0xFAu)));
    /* $03:FD17: A8          | TAY                [M=16-bit, X=16-bit] */
    ctx->y = ctx->a;
    snes_set_nz16(ctx, ctx->y);
    /* $03:FD18: B5 02       | LDA $02,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD1A: DA          | PHX                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->x);
    /* $03:FD1B: A6 08       | LDX $08            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:FD1D: 1D BC 80    | ORA $80BC,X        [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_absx(ctx, 0x80BCu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD20: 85 06       | STA $06            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x06u), (uint16_t)(ctx->a));
    /* $03:FD22: B1 C4       | LDA ($C4),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC4u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD24: 48          | PHA                [M=16-bit, X=16-bit] */
    snes_push16(ctx, ctx->a);
    /* $03:FD25: B1 C6       | LDA ($C6),Y        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpiy(ctx, 0xC6u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD27: BC 9C 80    | LDY $809C,X        [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x809Cu));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FD2A: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FD2B: BD 00 00    | LDA $0000,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0000u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD2E: 99 80 17    | STA $1780,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1780u), (uint16_t)(ctx->a));
    /* $03:FD31: BD 02 00    | LDA $0002,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0002u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD34: 99 82 17    | STA $1782,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1782u), (uint16_t)(ctx->a));
    /* $03:FD37: BD 04 00    | LDA $0004,X        [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_absx(ctx, 0x0004u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD3A: 99 84 17    | STA $1784,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x1784u), (uint16_t)(ctx->a));
    /* $03:FD3D: 7A          | PLY                [M=16-bit, X=16-bit] */
    ctx->y = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FD3E: 20 93 FA    | JSR $FA93          [M=16-bit, X=16-bit] */
    bank03_func_fa93(ctx);
    /* $03:FD41: FA          | PLX                [M=16-bit, X=16-bit] */
    ctx->x = snes_pull16(ctx);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FD42: A5 D0       | LDA $D0            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0xD0u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD44: D0 09       | BNE $FD4F          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fd4f;
    /* $03:FD46: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD48: 29 FD FF    | AND #$FFFD         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFDu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD4B: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FD4D: 80 07       | BRA $FD56          [M=16-bit, X=16-bit] */
    goto L_fd56;
L_fd4f: /* [M=16-bit, X=16-bit] */
    /* $03:FD4F: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD51: 09 02 00    | ORA #$0002         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD54: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_fd56: /* [M=16-bit, X=16-bit] */
    /* $03:FD56: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:FD58: E6 08       | INC $08            [M=16-bit, X=16-bit] */
    { uint16_t v16 = (uint16_t)((snes_bus_read16(ctx, snes_addr_dp(ctx, 0x08u))) + 1u); snes_set_nz16(ctx, v16); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x08u), (uint16_t)(v16)); }
    /* $03:FD5A: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD5B: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FD5C: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:FD5F: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FD60: E0 C0 0B    | CPX #$0BC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0BC0u);
    /* $03:FD63: B0 03       | BCS $FD68          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_fd68;
    /* $03:FD65: 82 6D FF    | BRL $FCD5          [M=16-bit, X=16-bit] */
    goto L_fcd5;
L_fd68: /* [M=16-bit, X=16-bit] */
    /* $03:FD68: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_fd69  (SNES $03:FD69 | Ghidra 0x3fd69)
 * Discovery Provenance: JSR $FD69 @ $03:FCAB
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_fd69(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:FD69: AD 02 1E    | LDA $1E02          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x1E02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD6C: F0 03       | BEQ $FD71          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_fd71;
    /* $03:FD6E: 3A          | DEC A              [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)(ctx->a - 1u);
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD6F: D0 08       | BNE $FD79          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fd79;
L_fd71: /* [M=16-bit, X=16-bit] */
    /* $03:FD71: AD 2C 0F    | LDA $0F2C          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_abs(ctx, 0x0F2Cu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD74: F0 03       | BEQ $FD79          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_fd79;
    /* $03:FD76: 82 02 00    | BRL $FD7B          [M=16-bit, X=16-bit] */
    goto L_fd7b;
L_fd79: /* [M=16-bit, X=16-bit] */
    /* $03:FD79: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FD7A: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
L_fd7b: /* [M=16-bit, X=16-bit] */
    /* $03:FD7B: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:FD7C: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}

/* ==========================================================================
 * Function: bank03_func_fd7d  (SNES $03:FD7D | Ghidra 0x3fd7d)
 * Discovery Provenance: JSR $FD7D @ $03:F45F
 * REA Bank Evidence ID: ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6
 * Entry Register Widths: [M=16-bit, X=16-bit]
 * ========================================================================== */
void bank03_func_fd7d(SnesCpuContext *ctx) {
    (void)ctx;
    /* [Entry State: M=16-bit, X=16-bit] */
    /* $03:FD7D: A2 30 11    | LDX #$1130         [M=16-bit, X=16-bit] */
    ctx->x = 0x1130u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FD80: 86 10       | STX $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->x));
    /* $03:FD82: A2 D6 11    | LDX #$11D6         [M=16-bit, X=16-bit] */
    ctx->x = 0x11D6u;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FD85: 86 12       | STX $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->x));
    /* $03:FD87: A9 02 00    | LDA #$0002         [M=16-bit, X=16-bit] */
    ctx->a = 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD8A: D0 07       | BNE $FD93          [M=16-bit, X=16-bit] */
    if (!ctx->flag_z) goto L_fd93;
    /* $03:FD8C: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD8F: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:FD91: 80 0B       | BRA $FD9E          [M=16-bit, X=16-bit] */
    goto L_fd9e;
L_fd93: /* [M=16-bit, X=16-bit] */
    /* $03:FD93: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD95: EB          | XBA                [M=16-bit, X=16-bit] */
    ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));
    snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));
    /* $03:FD96: 29 00 FF    | AND #$FF00         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFF00u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD99: 09 80 00    | ORA #$0080         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0080u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FD9C: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
L_fd9e: /* [M=16-bit, X=16-bit] */
    /* $03:FD9E: A2 C0 0B    | LDX #$0BC0         [M=16-bit, X=16-bit] */
    ctx->x = 0x0BC0u;
    snes_set_nz16(ctx, ctx->x);
L_fda1: /* [M=16-bit, X=16-bit] */
    /* $03:FDA1: B5 00       | LDA $00,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDA3: F0 79       | BEQ $FE1E          [M=16-bit, X=16-bit] */
    if (ctx->flag_z) goto L_fe1e;
    /* $03:FDA5: 15 02       | ORA $02,X          [M=16-bit, X=16-bit] */
    ctx->a |= snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDA7: 85 04       | STA $04            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x04u), (uint16_t)(ctx->a));
    /* $03:FDA9: B4 0A       | LDY $0A,X          [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Au));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FDAB: B5 0E       | LDA $0E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x0Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDAD: 20 97 E1    | JSR $E197          [M=16-bit, X=16-bit] */
    bank03_func_e197(ctx);
    /* $03:FDB0: 90 09       | BCC $FDBB          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fdbb;
    /* $03:FDB2: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDB4: 29 F7 FF    | AND #$FFF7         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFF7u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDB7: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FDB9: 80 5C       | BRA $FE17          [M=16-bit, X=16-bit] */
    goto L_fe17;
L_fdbb: /* [M=16-bit, X=16-bit] */
    /* $03:FDBB: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDBD: 09 08 00    | ORA #$0008         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0008u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDC0: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FDC2: A4 10       | LDY $10            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FDC4: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:FDC5: A5 02       | LDA $02            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x02u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDC7: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:FDCA: C5 0C       | CMP $0C            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Cu)));
    /* $03:FDCC: 30 49       | BMI $FE17          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fe17;
    /* $03:FDCE: C5 0E       | CMP $0E            [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x0Eu)));
    /* $03:FDD0: 10 45       | BPL $FE17          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fe17;
    /* $03:FDD2: 99 01 00    | STA $0001,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0001u), (uint16_t)(ctx->a));
    /* $03:FDD5: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:FDD6: A5 00       | LDA $00            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x00u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDD8: E9 08 00    | SBC #$0008         [M=16-bit, X=16-bit] */
    snes_sbc16(ctx, 0x0008u);
    /* $03:FDDB: C9 00 01    | CMP #$0100         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0x0100u);
    /* $03:FDDE: 10 37       | BPL $FE17          [M=16-bit, X=16-bit] */
    if (!ctx->flag_n) goto L_fe17;
    /* $03:FDE0: C9 F0 FF    | CMP #$FFF0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->a, 0xFFF0u);
    /* $03:FDE3: 30 32       | BMI $FE17          [M=16-bit, X=16-bit] */
    if (ctx->flag_n) goto L_fe17;
    /* $03:FDE5: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:FDE7: E2 20       | SEP #$20           [M=16-bit, X=16-bit] */
    /* SEP #$20 -> Register Width Transition: [M=8-bit, X=16-bit] */
    snes_sep(ctx, 0x20u);
    /* $03:FDE9: 99 00 00    | STA $0000,Y        [M=8-bit, X=16-bit] */
    snes_bus_write8(ctx, snes_addr_absy(ctx, 0x0000u), (uint8_t)((uint8_t)(ctx->a & 0xFFu)));
    /* $03:FDEC: C2 20       | REP #$20           [M=8-bit, X=16-bit] */
    /* REP #$20 -> Register Width Transition: [M=16-bit, X=16-bit] */
    snes_rep(ctx, 0x20u);
    /* $03:FDEE: A5 04       | LDA $04            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x04u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FDF0: 99 02 00    | STA $0002,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0002u), (uint16_t)(ctx->a));
    /* $03:FDF3: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FDF4: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FDF5: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FDF6: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FDF7: 84 10       | STY $10            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x10u), (uint16_t)(ctx->y));
    /* $03:FDF9: 38          | SEC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 1u;
    /* $03:FDFA: 66 14       | ROR $14            [M=16-bit, X=16-bit] */
    { uint16_t v16 = snes_ror16(ctx, snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u))); snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(v16)); }
    /* $03:FDFC: 90 10       | BCC $FE0E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fe0e;
    /* $03:FDFE: A4 12       | LDY $12            [M=16-bit, X=16-bit] */
    ctx->y = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x12u));
    snes_set_nz16(ctx, ctx->y);
    /* $03:FE00: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FE02: 99 00 00    | STA $0000,Y        [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_absy(ctx, 0x0000u), (uint16_t)(ctx->a));
    /* $03:FE05: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FE06: C8          | INY                [M=16-bit, X=16-bit] */
    ctx->y = (uint16_t)(ctx->y + 1u);
    snes_set_nz16(ctx, ctx->y);
    /* $03:FE07: A9 00 80    | LDA #$8000         [M=16-bit, X=16-bit] */
    ctx->a = 0x8000u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FE0A: 85 14       | STA $14            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x14u), (uint16_t)(ctx->a));
    /* $03:FE0C: 84 12       | STY $12            [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dp(ctx, 0x12u), (uint16_t)(ctx->y));
L_fe0e: /* [M=16-bit, X=16-bit] */
    /* $03:FE0E: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FE10: 09 02 00    | ORA #$0002         [M=16-bit, X=16-bit] */
    ctx->a |= 0x0002u;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FE13: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
    /* $03:FE15: 80 07       | BRA $FE1E          [M=16-bit, X=16-bit] */
    goto L_fe1e;
L_fe17: /* [M=16-bit, X=16-bit] */
    /* $03:FE17: B5 2E       | LDA $2E,X          [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dpx(ctx, 0x2Eu));
    snes_set_nz16(ctx, ctx->a);
    /* $03:FE19: 29 FD FF    | AND #$FFFD         [M=16-bit, X=16-bit] */
    ctx->a &= 0xFFFDu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FE1C: 95 2E       | STA $2E,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x2Eu), (uint16_t)(ctx->a));
L_fe1e: /* [M=16-bit, X=16-bit] */
    /* $03:FE1E: 8A          | TXA                [M=16-bit, X=16-bit] */
    ctx->a = ctx->x;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FE1F: 18          | CLC                [M=16-bit, X=16-bit] */
    ctx->flag_c = 0u;
    /* $03:FE20: 69 40 00    | ADC #$0040         [M=16-bit, X=16-bit] */
    snes_adc16(ctx, 0x0040u);
    /* $03:FE23: AA          | TAX                [M=16-bit, X=16-bit] */
    ctx->x = ctx->a;
    snes_set_nz16(ctx, ctx->x);
    /* $03:FE24: E0 C0 0E    | CPX #$0EC0         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x0EC0u);
    /* $03:FE27: B0 03       | BCS $FE2C          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_fe2c;
    /* $03:FE29: 82 75 FF    | BRL $FDA1          [M=16-bit, X=16-bit] */
    goto L_fda1;
L_fe2c: /* [M=16-bit, X=16-bit] */
    /* $03:FE2C: A5 14       | LDA $14            [M=16-bit, X=16-bit] */
    ctx->a = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x14u));
    snes_set_nz16(ctx, ctx->a);
L_fe2e: /* [M=16-bit, X=16-bit] */
    /* $03:FE2E: 4A          | LSR A              [M=16-bit, X=16-bit] */
    ctx->a = snes_lsr16(ctx, ctx->a);
    /* $03:FE2F: 90 FD       | BCC $FE2E          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fe2e;
    /* $03:FE31: 09 AA AA    | ORA #$AAAA         [M=16-bit, X=16-bit] */
    ctx->a |= 0xAAAAu;
    snes_set_nz16(ctx, ctx->a);
    /* $03:FE34: 92 12       | STA ($12)          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpi(ctx, 0x12u), (uint16_t)(ctx->a));
    /* $03:FE36: A6 10       | LDX $10            [M=16-bit, X=16-bit] */
    ctx->x = snes_bus_read16(ctx, snes_addr_dp(ctx, 0x10u));
    snes_set_nz16(ctx, ctx->x);
    /* $03:FE38: E0 60 11    | CPX #$1160         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1160u);
    /* $03:FE3B: B0 0E       | BCS $FE4B          [M=16-bit, X=16-bit] */
    if (ctx->flag_c) goto L_fe4b;
    /* $03:FE3D: A9 00 F0    | LDA #$F000         [M=16-bit, X=16-bit] */
    ctx->a = 0xF000u;
    snes_set_nz16(ctx, ctx->a);
L_fe40: /* [M=16-bit, X=16-bit] */
    /* $03:FE40: 95 00       | STA $00,X          [M=16-bit, X=16-bit] */
    snes_bus_write16(ctx, snes_addr_dpx(ctx, 0x00u), (uint16_t)(ctx->a));
    /* $03:FE42: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FE43: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FE44: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FE45: E8          | INX                [M=16-bit, X=16-bit] */
    ctx->x = (uint16_t)(ctx->x + 1u);
    snes_set_nz16(ctx, ctx->x);
    /* $03:FE46: E0 60 11    | CPX #$1160         [M=16-bit, X=16-bit] */
    snes_cmp16(ctx, ctx->x, 0x1160u);
    /* $03:FE49: 90 F5       | BCC $FE40          [M=16-bit, X=16-bit] */
    if (!ctx->flag_c) goto L_fe40;
L_fe4b: /* [M=16-bit, X=16-bit] */
    /* $03:FE4B: 60          | RTS                [M=16-bit, X=16-bit] */
    return;
}
