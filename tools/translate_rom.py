#!/usr/bin/env python3
"""
Contra III: The Alien Wars (USA) - SNES 65816 to Portable C11 Static Recompiler
===============================================================================
Driven by REA Ghidra 12.1.4 analysis artifacts (`65816:LE:24:snes`).

Implements the complete bank-by-bank translation pipeline:
1. Consumes REA Ghidra analyzed bank bytes and procedure inventories.
2. Performs whole-ROM + intra-procedural dynamic M/X width & PHP/PLP stack analysis,
   emitting explicit 8-bit/16-bit register width comments at function entry, basic
   block labels, and every SEP/REP/PLP transition.
3. Translates all 65816 instructions into direct, portable C11 assignments.
4. Replaces all SNES hardware memory addresses ($2100-$213F, $4200-$421F, plus APU
   $2140-$217F, WRAM port $2180-$2183, Joypad $4016-$4017, DMA $4300-$437F) with
   abstracted project hardware interface macros (`snes_write_ppu`, `snes_read_cpu_io`, etc.).
5. Emits fast switch dispatchers for statically proven jump tables while flagging all
   unresolved indirect paths inline with `// REQUIRES INTERPRETER FALLBACK`.
"""

import argparse
import hashlib
import json
import os
import sys
from collections import deque, defaultdict

# All inputs and outputs are resolved relative to the project, so the generator is
# reproducible from a fresh checkout plus the user's own ROM image.
PROJECT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA_DIR = os.path.join(PROJECT_DIR, "tools", "data")
EXPECTED_ROM_SHA256 = "a93ea87fc835c530b5135c5294433d15eef6dbf656144b387e89ac19cf864996"
DEFAULT_ROM_PATH = os.environ.get(
    "CONTRA3_ROM",
    os.path.join(os.path.dirname(PROJECT_DIR), "ROM", "Contra III - The Alien Wars (USA).sfc"))


def _parse_args(argv):
    ap = argparse.ArgumentParser(description="Contra III 65816 -> C11 static recompiler")
    ap.add_argument("--rom", default=DEFAULT_ROM_PATH,
                    help="Contra III (USA) ROM image (default: $CONTRA3_ROM or ../ROM/...sfc)")
    ap.add_argument("--out", default=PROJECT_DIR,
                    help="output root receiving include/contra3_recomp.h and src/*.c (default: project)")
    ap.add_argument("--stats", default=None,
                    help="translation_stats.json path (default: <out>/build/translation_stats.json)")
    return ap.parse_args(argv)


ARGS = _parse_args(sys.argv[1:] if __name__ == "__main__" else [])
WORKSPACE_DIR = os.path.abspath(ARGS.out)
MANIFEST_PATH = os.path.join(DATA_DIR, "rea_evidence_manifest.json")
PROCS_STEP_PATH = os.path.join(DATA_DIR, "ghidra_procedures.json")
STATS_OUT_PATH = ARGS.stats or os.path.join(WORKSPACE_DIR, "build", "translation_stats.json")

with open(ARGS.rom, "rb") as f:
    ROM = f.read()
_rom_sha = hashlib.sha256(ROM).hexdigest()
if _rom_sha != EXPECTED_ROM_SHA256:
    sys.exit(f"error: {ARGS.rom} has SHA-256 {_rom_sha}; expected Contra III (USA) {EXPECTED_ROM_SHA256}")

with open(MANIFEST_PATH, "r", encoding="utf-8") as f:
    EVIDENCE_MANIFEST = json.load(f)

with open(PROCS_STEP_PATH, "r", encoding="utf-8") as f:
    PROCS_DATA = json.load(f)

GHIDRA_LIST_PROCS_EVID = PROCS_DATA["evidence_id"]
GHIDRA_PROCS = {}
for p in PROCS_DATA["normalized_result"]:
    addr = int(p["address"], 16)
    GHIDRA_PROCS[((addr >> 16) & 0xFF, addr & 0xFFFF)] = p["value"]

KNOWN_ANNOTATIONS = {
    (0x00, 0x8000): "RESET_BootEntry - Hardware init, KONAMI SRAM check, SPC700 IPL upload, falls into WarmReset",
    (0x00, 0x80A3): "WarmReset_Entry - Soft-reset re-initialization (WRAM/VRAM clear, PPU init, background LZSS loop)",
    (0x00, 0x816C): "PRNG_Next16 - 32-bit shift/XOR pseudo-random number generator ($E8-$EB)",
    (0x00, 0x819B): "NMI_Handler - Master VBlank interrupt dispatcher ($81E0 indexed table)",
    (0x00, 0x81E6): "NMI_Mode0_TitleMenu - VBlank handler for title, menus, and intermissions",
    (0x00, 0x823C): "NMI_Mode2_SideScroll - VBlank handler for standard side-scrolling stages",
    (0x00, 0x83A3): "NMI_Mode4_Mode7TopDown - VBlank handler for Mode 7 overhead stages (Stages 2 & 5)",
    (0x00, 0x8452): "DMA_TransferOAMAndCGRAM - Uploads 544B OAM ($7E:0FC0) and 512B CGRAM palette via DMA0/DMA1",
    (0x00, 0x848F): "PPU_EnableDisplay - Sets brightness $0F and enables NMI + Auto-Joypad ($81 -> $4200)",
    (0x00, 0x84AE): "PPU_SyncShadowRegisters - Flushes WRAM PPU shadow mirrors ($1E84-$1EAA) to $2101-$2132",
    (0x00, 0x85A9): "PPU_ForceBlankDisableDMA - Disables NMI/DMA/HDMA and forces blanking ($8F -> $2100)",
    (0x00, 0x85C1): "PPU_RestoreInterruptsClearDMA - Restores $4200 from shadow $1E82 and zeroes DMA/HDMA",
    (0x00, 0x85DF): "VBlank_WaitFrame - Sets $003C=$80 and awaits VBlank NMI completion",
    (0x00, 0x85F3): "Sound_ProcessQueue - Sends queued sound/music command bytes to SPC700 $2140",
    (0x00, 0x868F): "Sound_EnqueueCommand - Pushes audio command ID onto $0100 ring buffer",
    (0x00, 0x86AC): "PPU_InitHardwareDefaults - Full cold/stage reset of PPU $2100-$2133 and CPU I/O $4200-$420C",
    (0x00, 0x88A3): "WRAM_ClearLowRAM_0000_1BFF - Clears $00:0000-$00:1BFF via MVN",
    (0x00, 0x89EE): "Math_DivSigned16By8 - 16-bit fixed-point division using hardware divider $4204/$4206",
    (0x00, 0x8A58): "Math_SubShift32 - Unreferenced SDK 32-bit shift/subtract helper",
    (0x00, 0x8A92): "DP_ClearScratchWrapper - Unreferenced SDK wrapper clearing DP $00-$3E",
    (0x00, 0x8A9A): "DP_ClearScratch - Unreferenced SDK routine clearing Direct Page $00-$3E",
    (0x00, 0x8ADB): "Math_Mul16x16 - Unreferenced SDK 16x16 unsigned multiply via $4202/$4203 -> $4216",
    (0x00, 0x8B19): "Input_ReadJoypads - Polls $4212 auto-joypad ready and reads $4218-$421F + soft-reset combo",
    (0x00, 0xB2F3): "Cheat_ContraSpirits_SoundTest - Unreferenced Contra Spirits (JP) title sound-test code checker",
    (0x00, 0xB346): "Cheat_ContraSpirits_30Lives - Unreferenced Contra Spirits (JP) 30-lives ($1E -> $1E3C) code checker",
    (0x00, 0xB39F): "Cheat_ContraSpirits_StageSelect - Unreferenced Contra Spirits (JP) stage-select ($01 -> $1E60) checker",
    (0x00, 0x8BBA): "HDMA_SetupChannels - Configures DMA channels from table and writes $420C HDMAEN",
    (0x00, 0x8BE2): "IRQ_Handler - Master H/V-Timer interrupt dispatcher ($8C0E indexed table)",
    (0x00, 0x8C14): "IRQ_Mode0_NoOp - Null IRQ handler (immediate RTS)",
    (0x00, 0x8C15): "IRQ_Mode2_Mode7Split - Mid-screen Mode 7 matrix update & secondary V-IRQ arm",
    (0x00, 0x8C8C): "IRQ_Mode4_Mode7LowerSplit - Secondary scanline Mode 7 matrix update",
    (0x00, 0x8CFB): "VRAM_ProcessTransferQueue - Executes queued DMA0 transfers from $7E:3000 command ring",
    (0x00, 0x8DD0): "Engine_ResetSubsystems - Resets VRAM/WRAM queues, PPU defaults, and display state",
    (0x00, 0x8E4B): "VRAM_ClearWords - Zeroes Y words in VRAM starting at word address X",
    (0x00, 0x90A8): "Asset_ExecuteScriptEntry - Dispatches Bank $05 graphics/palette/audio load script entry",
    (0x00, 0x937B): "SPC700_WaitBootHandshake - Waits for $BBAA IPL signature on $2140-$2141 and sends $CC",
    (0x00, 0x94A9): "Konami_LZSS_Decompress - Konami LZSS/RLE bitstream decompressor into $7E:2C00 sliding buffer",
    (0x00, 0x990E): "SPC700_ResetToIPL - Commands audio driver ($84/$FF) to return to SPC700 IPL ROM",
    (0x00, 0x995A): "Background_DecompressWorker - Main-loop background LZSS/SPC700 streaming worker",
    (0x00, 0xA377): "MainModule_Dispatch - Top-level 15-state game loop state machine ($00:A38A table)",
    (0x00, 0xBD40): "Gameplay_MainStep - Active stage gameplay frame update ($00:BD57 substate table)",
    (0x00, 0xE355): "StageEvent_Dispatch24 - 24-bit cross-bank stage event dispatcher ($05:A0B4 table)",
    (0x00, 0xEB04): "Actor_Dispatch24 - Master 160-entry cross-bank actor/boss update dispatcher ($05:A48A table)",
    (0x02, 0xDDC6): "Mode7_NMI_PreUpdate - Overhead stage Mode 7 matrix/camera pre-transfer update",
    (0x02, 0xDDDC): "Mode7_NMI_PostUpdate - Overhead stage Mode 7 post-blank frame logic",
    (0x03, 0xE180): "Mode7_StageUpdate - Bank 03 Mode 7 overhead stage mechanics & rotation handler",
    (0x1D, 0xABA0): "Math_Atan2_256 - Computes 8-bit 256-step angle from delta X/Y",
}

PPU_SYM = {
    0x2100: "PPU_INIDISP", 0x2101: "PPU_OBSEL", 0x2102: "PPU_OAMADDL", 0x2103: "PPU_OAMADDH",
    0x2104: "PPU_OAMDATA", 0x2105: "PPU_BGMODE", 0x2106: "PPU_MOSAIC", 0x2107: "PPU_BG1SC",
    0x2108: "PPU_BG2SC", 0x2109: "PPU_BG3SC", 0x210A: "PPU_BG4SC", 0x210B: "PPU_BG12NBA",
    0x210C: "PPU_BG34NBA", 0x210D: "PPU_BG1HOFS", 0x210E: "PPU_BG1VOFS", 0x210F: "PPU_BG2HOFS",
    0x2110: "PPU_BG2VOFS", 0x2111: "PPU_BG3HOFS", 0x2112: "PPU_BG3VOFS", 0x2113: "PPU_BG4HOFS",
    0x2114: "PPU_BG4VOFS", 0x2115: "PPU_VMAIN", 0x2116: "PPU_VMADDL", 0x2117: "PPU_VMADDH",
    0x2118: "PPU_VMDATAL", 0x2119: "PPU_VMDATAH", 0x211A: "PPU_M7SEL", 0x211B: "PPU_M7A",
    0x211C: "PPU_M7B", 0x211D: "PPU_M7C", 0x211E: "PPU_M7D", 0x211F: "PPU_M7X",
    0x2120: "PPU_M7Y", 0x2121: "PPU_CGADD", 0x2122: "PPU_CGDATA", 0x2123: "PPU_W12SEL",
    0x2124: "PPU_W34SEL", 0x2125: "PPU_WOBJSEL", 0x2126: "PPU_WH0", 0x2127: "PPU_WH1",
    0x2128: "PPU_WH2", 0x2129: "PPU_WH3", 0x212A: "PPU_WBGLOG", 0x212B: "PPU_WOBJLOG",
    0x212C: "PPU_TM", 0x212D: "PPU_TS", 0x212E: "PPU_TMW", 0x212F: "PPU_TSW",
    0x2130: "PPU_CGWSEL", 0x2131: "PPU_CGADSUB", 0x2132: "PPU_COLDATA", 0x2133: "PPU_SETINI",
    0x2134: "PPU_MPYL", 0x2135: "PPU_MPYM", 0x2136: "PPU_MPYH", 0x2137: "PPU_SLHV",
    0x2138: "PPU_OAMDATAREAD", 0x2139: "PPU_VMDATALREAD", 0x213A: "PPU_VMDATAHREAD",
    0x213B: "PPU_CGDATAREAD", 0x213C: "PPU_OPHCT", 0x213D: "PPU_OPVCT",
    0x213E: "PPU_STAT77", 0x213F: "PPU_STAT78",
}

CPUIO_SYM = {
    0x4200: "CPUIO_NMITIMEN", 0x4201: "CPUIO_WRIO", 0x4202: "CPUIO_WRMPYA", 0x4203: "CPUIO_WRMPYB",
    0x4204: "CPUIO_WRDIVL", 0x4205: "CPUIO_WRDIVH", 0x4206: "CPUIO_WRDIVB", 0x4207: "CPUIO_HTIMEL",
    0x4208: "CPUIO_HTIMEH", 0x4209: "CPUIO_VTIMEL", 0x420A: "CPUIO_VTIMEH", 0x420B: "CPUIO_MDMAEN",
    0x420C: "CPUIO_HDMAEN", 0x420D: "CPUIO_MEMSEL", 0x4210: "CPUIO_RDNMI", 0x4211: "CPUIO_TIMEUP",
    0x4212: "CPUIO_HVBJOY", 0x4213: "CPUIO_RDIO", 0x4214: "CPUIO_RDDIVL", 0x4215: "CPUIO_RDDIVH",
    0x4216: "CPUIO_RDMPYL", 0x4217: "CPUIO_RDMPYH", 0x4218: "CPUIO_JOY1L", 0x4219: "CPUIO_JOY1H",
    0x421A: "CPUIO_JOY2L", 0x421B: "CPUIO_JOY2H", 0x421C: "CPUIO_JOY3L", 0x421D: "CPUIO_JOY3H",
    0x421E: "CPUIO_JOY4L", 0x421F: "CPUIO_JOY4H",
}

APU_SYM = {0x2140: "APU_PORT0", 0x2141: "APU_PORT1", 0x2142: "APU_PORT2", 0x2143: "APU_PORT3"}
WRAM_SYM = {0x2180: "WRAM_WMDATA", 0x2181: "WRAM_WMADDL", 0x2182: "WRAM_WMADDM", 0x2183: "WRAM_WMADDH"}
JOY_SYM = {0x4016: "JOY_JOYSER0", 0x4017: "JOY_JOYSER1"}

def lorom_off(bank, addr16):
    if addr16 < 0x8000 or addr16 > 0xFFFF:
        return None
    return ((bank & 0x1F) * 0x8000) + (addr16 - 0x8000)

def u8(bank, addr16):
    o = lorom_off(bank, addr16)
    return ROM[o] if o is not None and 0 <= o < len(ROM) else None

def u16(bank, addr16):
    o = lorom_off(bank, addr16)
    if o is None or o + 1 >= len(ROM):
        return None
    return ROM[o] | (ROM[o + 1] << 8)

OPS_RAW = """
00 BRK imm8|01 ORA dpxi|02 COP imm8|03 ORA sr|04 TSB dp|05 ORA dp|06 ASL dp|07 ORA dpil
08 PHP imp|09 ORA immm|0A ASL acc|0B PHD imp|0C TSB abs|0D ORA abs|0E ASL abs|0F ORA long
10 BPL rel|11 ORA dpiy|12 ORA dpi|13 ORA sriy|14 TRB dp|15 ORA dpx|16 ASL dpx|17 ORA dpily
18 CLC imp|19 ORA absy|1A INC acc|1B TCS imp|1C TRB abs|1D ORA absx|1E ASL absx|1F ORA longx
20 JSR abs|21 AND dpxi|22 JSL long|23 AND sr|24 BIT dp|25 AND dp|26 ROL dp|27 AND dpil
28 PLP imp|29 AND immm|2A ROL acc|2B PLD imp|2C BIT abs|2D AND abs|2E ROL abs|2F AND long
30 BMI rel|31 AND dpiy|32 AND dpi|33 AND sriy|34 BIT dpx|35 AND dpx|36 ROL dpx|37 AND dpily
38 SEC imp|39 AND absy|3A DEC acc|3B TSC imp|3C BIT absx|3D AND absx|3E ROL absx|3F AND longx
40 RTI imp|41 EOR dpxi|42 WDM imm8|43 EOR sr|44 MVP mv|45 EOR dp|46 LSR dp|47 EOR dpil
48 PHA imp|49 EOR immm|4A LSR acc|4B PHK imp|4C JMP abs|4D EOR abs|4E LSR abs|4F EOR long
50 BVC rel|51 EOR dpiy|52 EOR dpi|53 EOR sriy|54 MVN mv|55 EOR dpx|56 LSR dpx|57 EOR dpily
58 CLI imp|59 EOR absy|5A PHY imp|5B TCD imp|5C JML long|5D EOR absx|5E LSR absx|5F EOR longx
60 RTS imp|61 ADC dpxi|62 PER rell|63 ADC sr|64 STZ dp|65 ADC dp|66 ROR dp|67 ADC dpil
68 PLA imp|69 ADC immm|6A ROR acc|6B RTL imp|6C JMP absi|6D ADC abs|6E ROR abs|6F ADC long
70 BVS rel|71 ADC dpiy|72 ADC dpi|73 ADC sriy|74 STZ dpx|75 ADC dpx|76 ROR dpx|77 ADC dpily
78 SEI imp|79 ADC absy|7A PLY imp|7B TDC imp|7C JMP absxi|7D ADC absx|7E ROR absx|7F ADC longx
80 BRA rel|81 STA dpxi|82 BRL rell|83 STA sr|84 STY dp|85 STA dp|86 STX dp|87 STA dpil
88 DEY imp|89 BIT immm|8A TXA imp|8B PHB imp|8C STY abs|8D STA abs|8E STX abs|8F STA long
90 BCC rel|91 STA dpiy|92 STA dpi|93 STA sriy|94 STY dpx|95 STA dpx|96 STX dpy|97 STA dpily
98 TYA imp|99 STA absy|9A TXS imp|9B TXY imp|9C STZ abs|9D STA absx|9E STZ absx|9F STA longx
A0 LDY immx|A1 LDA dpxi|A2 LDX immx|A3 LDA sr|A4 LDY dp|A5 LDA dp|A6 LDX dp|A7 LDA dpil
A8 TAY imp|A9 LDA immm|AA TAX imp|AB PLB imp|AC LDY abs|AD LDA abs|AE LDX abs|AF LDA long
B0 BCS rel|B1 LDA dpiy|B2 LDA dpi|B3 LDA sriy|B4 LDY dpx|B5 LDA dpx|B6 LDX dpy|B7 LDA dpily
B8 CLV imp|B9 LDA absy|BA TSX imp|BB TYX imp|BC LDY absx|BD LDA absx|BE LDX absy|BF LDA longx
C0 CPY immx|C1 CMP dpxi|C2 REP imm8|C3 CMP sr|C4 CPY dp|C5 CMP dp|C6 DEC dp|C7 CMP dpil
C8 INY imp|C9 CMP immm|CA DEX imp|CB WAI imp|CC CPY abs|CD CMP abs|CE DEC abs|CF CMP long
D0 BNE rel|D1 CMP dpiy|D2 CMP dpi|D3 CMP sriy|D4 PEI dpi|D5 CMP dpx|D6 DEC dpx|D7 CMP dpily
D8 CLD imp|D9 CMP absy|DA PHX imp|DB STP imp|DC JML absil|DD CMP absx|DE DEC absx|DF CMP longx
E0 CPX immx|E1 SBC dpxi|E2 SEP imm8|E3 SBC sr|E4 CPX dp|E5 SBC dp|E6 INC dp|E7 SBC dpil
E8 INX imp|E9 SBC immm|EA NOP imp|EB XBA imp|EC CPX abs|ED SBC abs|EE INC abs|EF SBC long
F0 BEQ rel|F1 SBC dpiy|F2 SBC dpi|F3 SBC sriy|F4 PEA abs|F5 SBC dpx|F6 INC dpx|F7 SBC dpily
F8 SED imp|F9 SBC absy|FA PLX imp|FB XCE imp|FC JSR absxi|FD SBC absx|FE INC absx|FF SBC longx
""".strip()

OPS = {}
for line in OPS_RAW.splitlines():
    for entry in line.split("|"):
        parts = entry.strip().split()
        OPS[int(parts[0], 16)] = (parts[1], parts[2])

MODE_LEN = {
    "imp": 0, "acc": 0,
    "imm8": 1, "dp": 1, "dpx": 1, "dpy": 1, "dpi": 1, "dpxi": 1, "dpiy": 1,
    "dpil": 1, "dpily": 1, "sr": 1, "sriy": 1, "rel": 1,
    "abs": 2, "absx": 2, "absy": 2, "absi": 2, "absxi": 2, "absil": 2, "rell": 2, "mv": 2,
    "long": 3, "longx": 3,
}
BRANCH_OPS = {"BPL", "BMI", "BVC", "BVS", "BCC", "BCS", "BNE", "BEQ", "BRA", "BRL"}


def func_cname(bank, addr16):
    return f"bank{bank & 0x1F:02x}_func_{addr16:04x}"


def mx_str(m, x):
    return f"M={'8-bit' if m else '16-bit'}, X={'8-bit' if x else '16-bit'}"


def format_operand_asm(bank, pc, mn, mode, n, val):
    if mode == "imp":
        return ""
    if mode == "acc":
        return "A"
    if mode in ("imm8", "immm", "immx"):
        return f"#${val:0{n*2}X}"
    if mode == "dp":
        return f"${val:02X}"
    if mode == "dpx":
        return f"${val:02X},X"
    if mode == "dpy":
        return f"${val:02X},Y"
    if mode == "dpi":
        return f"(${val:02X})"
    if mode == "dpxi":
        return f"(${val:02X},X)"
    if mode == "dpiy":
        return f"(${val:02X}),Y"
    if mode == "dpil":
        return f"[${val:02X}]"
    if mode == "dpily":
        return f"[${val:02X}],Y"
    if mode == "sr":
        return f"${val:02X},S"
    if mode == "sriy":
        return f"(${val:02X},S),Y"
    if mode == "abs":
        return f"${val:04X}"
    if mode == "absx":
        return f"${val:04X},X"
    if mode == "absy":
        return f"${val:04X},Y"
    if mode == "absi":
        return f"(${val:04X})"
    if mode == "absxi":
        return f"(${val:04X},X)"
    if mode == "absil":
        return f"[${val:04X}]"
    if mode == "long":
        return f"${val:06X}"
    if mode == "longx":
        return f"${val:06X},X"
    if mode == "rel":
        d = val - 256 if val > 127 else val
        return f"${(pc + 2 + d) & 0xFFFF:04X}"
    if mode == "rell":
        d = val - 65536 if val > 32767 else val
        return f"${(pc + 3 + d) & 0xFFFF:04X}"
    if mode == "mv":
        dst = val & 0xFF
        src = (val >> 8) & 0xFF
        return f"${dst:02X},${src:02X}"
    return f"${val:X}"


def build_whole_rom_cfg():
    func_entries = {}
    visited_states = set()
    ins_at = {}
    indirect_sites = {}
    resolved_tables = {}
    q = deque()

    def add_func(bank, addr16, m, x, reason):
        b = bank & 0x1F
        if not (0x8000 <= addr16 <= 0xFFFF):
            return
        if (b, addr16) not in func_entries:
            func_entries[(b, addr16)] = (m, x, reason)
        if (b, addr16, m, x, ()) not in visited_states:
            visited_states.add((b, addr16, m, x, ()))
            q.append((b, addr16, m, x, ()))

    def read_16bit_jtable(tbl_bank, tbl_addr, target_bank):
        key = ("16", tbl_bank & 0x1F, tbl_addr, target_bank & 0x1F)
        if tbl_addr < 0x8000:
            return []
        if key in resolved_tables:
            return resolved_tables[key]
        entries = []
        min_target = 0x10000
        for i in range(128):
            ea = tbl_addr + 2 * i
            if ea + 1 >= min_target or ea >= 0xFFF0:
                break
            if (tbl_bank & 0x1F, ea) in ins_at or (tbl_bank & 0x1F, ea + 1) in ins_at:
                break
            tgt = u16(tbl_bank, ea)
            if i == 0 and tgt == 0:
                entries.append(0)
                continue
            if tgt is None or tgt < 0x8000 or tgt >= 0xFFF0:
                break
            first_op = u8(target_bank, tgt)
            if first_op in (0x00, 0x02, 0x42, 0xDB, 0xFF):
                break
            entries.append(tgt)
            if (tbl_bank & 0x1F) == (target_bank & 0x1F) and tgt > tbl_addr and tgt < min_target:
                min_target = tgt
            if len(entries) >= 64 and min_target == 0x10000:
                break
        resolved_tables[key] = entries
        return entries

    def read_24bit_jtable(tbl_bank, tbl_addr, max_entries=512):
        key = ("24", tbl_bank & 0x1F, tbl_addr)
        if tbl_addr < 0x8000:
            return []
        if key in resolved_tables:
            return resolved_tables[key]
        entries = []
        for i in range(max_entries):
            ea = tbl_addr + 3 * i
            if ea + 2 > 0xFFFF:
                break
            lo, hi, bk = u8(tbl_bank, ea), u8(tbl_bank, ea + 1), u8(tbl_bank, ea + 2)
            ta = lo | (hi << 8)
            if i == 0 and ta == 0 and bk == 0:
                entries.append((0, 0))
                continue
            if not (((0x00 <= bk <= 0x1F) or (0x80 <= bk <= 0x9F)) and 0x8000 <= ta < 0xFFF0):
                break
            first_op = u8(bk & 0x1F, ta)
            if first_op in (0x00, 0x02, 0x42, 0xDB, 0xFF):
                break
            entries.append((bk & 0x1F, ta))
        resolved_tables[key] = entries
        return entries

    def detect_konami_inline_jtable(bank, jmp_pc):
        for off_back in (7, 6, 8, 9, 10):
            p = jmp_pc - off_back
            if p >= 0x8000 and u8(bank, p) == 0xBF:
                lo, hi, tb = u8(bank, p + 1), u8(bank, p + 2), u8(bank, p + 3)
                ta = lo | (hi << 8)
                if ((0x00 <= tb <= 0x1F) or (0x80 <= tb <= 0x9F)) and ta >= 0x8000:
                    return (tb & 0x1F, ta)
        for off_back in (6, 5, 7, 8):
            p = jmp_pc - off_back
            if p >= 0x8000 and u8(bank, p) in (0xBD, 0xB9):
                lo, hi = u8(bank, p + 1), u8(bank, p + 2)
                ta = lo | (hi << 8)
                if ta >= 0x8000:
                    if ta == jmp_pc + 3:
                        return (bank & 0x1F, ta)
                    return (5 if (bank & 0x1F) == 0 else (bank & 0x1F), ta)
        return None

    def detect_konami_24bit_jtable(bank, jml_pc):
        for back in range(8, 16):
            p = jml_pc - back
            if p < 0x8000:
                continue
            op = u8(bank, p)
            if op in (0xB9, 0xBD):
                lo, hi = u8(bank, p + 1), u8(bank, p + 2)
                ta = lo | (hi << 8)
                if ta >= 0x8000 and u8(bank, p + 5) == op:
                    ta2 = u8(bank, p + 6) | (u8(bank, p + 7) << 8)
                    if ta2 == ta + 2:
                        return (5, ta)
            elif op == 0xBF:
                lo, hi, tb = u8(bank, p + 1), u8(bank, p + 2), u8(bank, p + 3)
                ta = lo | (hi << 8)
                if ((0x00 <= tb <= 0x1F) or (0x80 <= tb <= 0x9F)) and ta >= 0x8000:
                    return (tb & 0x1F, ta)
        return None

    add_func(0, 0x8000, 1, 1, "Hardware RESET vector ($00:FFFC)")
    add_func(0, 0x80A3, 0, 0, "Soft-reset JMP $80A3 @ $00:8B5C")
    add_func(0, 0x819B, 0, 0, "Hardware Native NMI vector ($00:FFEA)")
    add_func(0, 0x8BE2, 0, 0, "Hardware Native IRQ/BRK/COP vector ($00:FFEE)")

    # Unreferenced Konami SDK library utilities & disabled Contra Spirits (JP) cheat routines in Bank $00
    unreferenced_bank00 = [
        (0x863D, 0, 0, "Unreferenced Bank $00 SDK audio reset routine"),
        (0x8819, 0, 0, "Unreferenced Bank $00 SDK WRAM clear helper ($0000-$1BFF)"),
        (0x8830, 0, 0, "Unreferenced Bank $00 SDK WRAM clear helper ($0084-$00FF)"),
        (0x8847, 0, 0, "Unreferenced Bank $00 SDK WRAM clear helper ($0200-$1BFF)"),
        (0x8875, 0, 0, "Unreferenced Bank $00 SDK WRAM clear helper ($0070-$00FF)"),
        (0x888C, 0, 0, "Unreferenced Bank $00 SDK WRAM clear helper ($1200-$1AFF)"),
        (0x892B, 0, 0, "Unreferenced Bank $00 SDK WRAM clear helper ($1B00-$1BFF)"),
        (0x8A58, 0, 0, "Unreferenced Bank $00 SDK 32-bit shift/subtract helper"),
        (0x8A92, 0, 0, "Unreferenced Bank $00 SDK DP scratch clear wrapper"),
        (0x8A9A, 0, 0, "Unreferenced Bank $00 SDK DP scratch clear ($00-$3E)"),
        (0x8ADB, 0, 0, "Unreferenced Bank $00 SDK 16x16 hardware multiply ($4202/$4203)"),
        (0x8B64, 0, 0, "Unreferenced Bank $00 SDK pause/stage-restart helper"),
        (0x8D6B, 0, 0, "Unreferenced Bank $00 SDK VRAM queue zero-fill helper"),
        (0x8E3E, 0, 0, "Unreferenced Bank $00 SDK VRAM $5800 clear helper"),
        (0x8E67, 0, 0, "Unreferenced Bank $00 SDK VRAM tile upload routine"),
        (0xB17B, 0, 0, "Unreferenced Bank $00 HDMA table helper"),
        (0xB2F3, 0, 0, "Unreferenced Contra Spirits (JP) Sound Test cheat checker"),
        (0xB346, 0, 0, "Unreferenced Contra Spirits (JP) 30-Lives cheat checker ($1E -> $1E3C)"),
        (0xB39F, 0, 0, "Unreferenced Contra Spirits (JP) Stage Select cheat checker ($01 -> $1E60)"),
        (0xCC2A, 0, 0, "Unreferenced Bank $00 BG scroll variable clear stub"),
    ]
    for u_pc, u_m, u_x, u_reason in unreferenced_bank00:
        add_func(0, u_pc, u_m, u_x, u_reason)

    while q:
        b, pc, m, x, pstk = q.popleft()
        op = u8(b, pc)
        if op is None or op in (0x00, 0x02, 0xDB):
            continue
        mn, mode = OPS[op]
        n = (1 if m else 2) if mode == "immm" else ((1 if x else 2) if mode == "immx" else MODE_LEN[mode])
        raw = [u8(b, pc + i) for i in range(1 + n)]
        if any(v is None for v in raw):
            continue
        val = int.from_bytes(bytes(raw[1:]), "little") if n > 0 else 0
        if (b, pc) not in ins_at:
            ins_at[(b, pc)] = (op, mn, mode, n, val, m, x)

        nm, nx, npstk = m, x, pstk
        if mn == "REP":
            if val & 0x20:
                nm = 0
            if val & 0x10:
                nx = 0
        elif mn == "SEP":
            if val & 0x20:
                nm = 1
            if val & 0x10:
                nx = 1
        elif mn == "PHP":
            if len(pstk) < 4:
                npstk = pstk + ((m, x),)
        elif mn == "PLP":
            if pstk:
                nm, nx = pstk[-1]
                npstk = pstk[:-1]

        next_pc = pc + 1 + n

        def push_state(nb, npc, sm, sx, sp):
            if 0x8000 <= npc <= 0xFFFF and (nb, npc, sm, sx, sp) not in visited_states:
                visited_states.add((nb, npc, sm, sx, sp))
                q.append((nb, npc, sm, sx, sp))

        if mn in ("RTS", "RTL", "RTI"):
            continue
        elif mn == "JMP":
            if mode == "abs":
                push_state(b, val, nm, nx, npstk)
            elif mode == "absxi":
                tgts = read_16bit_jtable(b, val, b)
                indirect_sites[(b, pc)] = (mn, mode, val, (b, val), tgts)
                for tgt in tgts:
                    if tgt:
                        add_func(b, tgt, nm, nx, f"JMP (${val:04X},X) table @ ${b:02X}:{pc:04X}")
            elif mode == "absi":
                tbl = detect_konami_inline_jtable(b, pc)
                tgts = read_16bit_jtable(tbl[0], tbl[1], b) if tbl else []
                indirect_sites[(b, pc)] = (mn, mode, val, tbl, tgts)
                for tgt in tgts:
                    if tgt:
                        add_func(b, tgt, nm, nx, f"JMP (${val:04X}) table ${tbl[0]:02X}:{tbl[1]:04X} @ ${b:02X}:{pc:04X}")
            continue
        elif mn == "JML":
            if mode == "long":
                tb = (val >> 16) & 0xFF
                ta = val & 0xFFFF
                if ((0x00 <= tb <= 0x1F) or (0x80 <= tb <= 0x9F)) and 0x8000 <= ta <= 0xFFFF:
                    add_func(tb & 0x1F, ta, nm, nx, f"JML ${val:06X} @ ${b:02X}:{pc:04X}")
            else:
                tbl = detect_konami_24bit_jtable(b, pc)
                tgts24 = read_24bit_jtable(tbl[0], tbl[1]) if tbl else []
                indirect_sites[(b, pc)] = (mn, mode, val, tbl, tgts24)
                for tb2, ta2 in tgts24:
                    if ta2:
                        add_func(tb2, ta2, nm, nx, f"JML [${val:04X}] table ${tbl[0]:02X}:{tbl[1]:04X} @ ${b:02X}:{pc:04X}")
            continue
        elif mn == "JSR":
            if mode == "abs":
                add_func(b, val, nm, nx, f"JSR ${val:04X} @ ${b:02X}:{pc:04X}")
            elif mode == "absxi":
                tgts = read_16bit_jtable(b, val, b)
                indirect_sites[(b, pc)] = (mn, mode, val, (b, val), tgts)
                for tgt in tgts:
                    if tgt:
                        add_func(b, tgt, nm, nx, f"JSR (${val:04X},X) table @ ${b:02X}:{pc:04X}")
            push_state(b, next_pc, nm, nx, npstk)
        elif mn == "JSL":
            tb = (val >> 16) & 0xFF
            ta = val & 0xFFFF
            if ((0x00 <= tb <= 0x1F) or (0x80 <= tb <= 0x9F)) and 0x8000 <= ta <= 0xFFFF:
                add_func(tb & 0x1F, ta, nm, nx, f"JSL ${val:06X} @ ${b:02X}:{pc:04X}")
            push_state(b, next_pc, nm, nx, npstk)
        elif mn in BRANCH_OPS:
            if mode == "rel":
                d = val - 256 if val > 127 else val
                tgt = (pc + 2 + d) & 0xFFFF
            else:
                d = val - 65536 if val > 32767 else val
                tgt = (pc + 3 + d) & 0xFFFF
            push_state(b, tgt, nm, nx, npstk)
            if mn not in ("BRA", "BRL"):
                push_state(b, next_pc, nm, nx, npstk)
        else:
            push_state(b, next_pc, nm, nx, npstk)

    return func_entries, ins_at, indirect_sites


# --- MMIO Classification & Macro Expression Generators ---

def classify_static_mmio(mode, val):
    """
    Returns (kind, reg_expr) if (mode, val) is a static SNES MMIO register access,
    else None.
    Note: Confirmed across all 56,502 instructions that every access in
    $2100-$2183, $4016-$4017, $4200-$421F, $4300-$437F occurs with DB in a
    LoROM system bank ($00, $05-$08), never $7E/$7F.
    """
    if mode == "abs":
        reg = val & 0xFFFF
        idx_expr = None
    elif mode in ("absx", "absy") and (0x2100 <= val <= 0x2183 or 0x4200 <= val <= 0x437F):
        reg = val & 0xFFFF
        idx_expr = "ctx->x" if mode == "absx" else "ctx->y"
    elif mode in ("long", "longx"):
        bk = (val >> 16) & 0x7F
        reg = val & 0xFFFF
        if bk >= 0x40:
            return None
        idx_expr = "ctx->x" if mode == "longx" else None
    else:
        return None

    def fmt_reg(sym_map):
        base = sym_map.get(reg, f"0x{reg:04X}u")
        return f"(uint16_t)({base} + {idx_expr})" if idx_expr else base

    if 0x2100 <= reg <= 0x213F:
        return ("ppu", fmt_reg(PPU_SYM))
    if 0x2140 <= reg <= 0x217F:
        return ("apu", fmt_reg(APU_SYM))
    if 0x2180 <= reg <= 0x2183:
        return ("wram_port", fmt_reg(WRAM_SYM))
    if reg in (0x4016, 0x4017):
        return ("joy_io", fmt_reg(JOY_SYM))
    if 0x4200 <= reg <= 0x421F:
        return ("cpu_io", fmt_reg(CPUIO_SYM))
    if 0x4300 <= reg <= 0x437F:
        return ("dma", fmt_reg({}))
    return None


def emit_ea_expr(mode, val):
    if mode == "dp":
        return f"snes_addr_dp(ctx, 0x{val:02X}u)"
    if mode == "dpx":
        return f"snes_addr_dpx(ctx, 0x{val:02X}u)"
    if mode == "dpy":
        return f"snes_addr_dpy(ctx, 0x{val:02X}u)"
    if mode == "dpi":
        return f"snes_addr_dpi(ctx, 0x{val:02X}u)"
    if mode == "dpxi":
        return f"snes_addr_dpxi(ctx, 0x{val:02X}u)"
    if mode == "dpiy":
        return f"snes_addr_dpiy(ctx, 0x{val:02X}u)"
    if mode == "dpil":
        return f"snes_addr_dpil(ctx, 0x{val:02X}u)"
    if mode == "dpily":
        return f"snes_addr_dpily(ctx, 0x{val:02X}u)"
    if mode == "abs":
        return f"snes_addr_abs(ctx, 0x{val:04X}u)"
    if mode == "absx":
        return f"snes_addr_absx(ctx, 0x{val:04X}u)"
    if mode == "absy":
        return f"snes_addr_absy(ctx, 0x{val:04X}u)"
    if mode == "long":
        return f"0x{val:06X}u"
    if mode == "longx":
        return f"snes_addr_longx(ctx, 0x{val:06X}u)"
    raise ValueError(f"Unsupported EA mode: {mode}")


def emit_read_expr(mode, val, bits):
    if mode in ("imm8", "immm", "immx"):
        mask = 0xFF if bits == 8 else 0xFFFF
        return f"0x{val & mask:0{bits // 4}X}u"
    mmio = classify_static_mmio(mode, val)
    if mmio is not None:
        kind, reg_expr = mmio
        if bits == 8:
            return f"snes_read_{kind}(ctx, {reg_expr})"
        if kind in ("wram_port", "joy_io"):
            # Sequential two-byte read
            return f"((uint16_t)snes_read_{kind}(ctx, {reg_expr}) | ((uint16_t)snes_read_{kind}(ctx, (uint16_t)({reg_expr} + 1u)) << 8))"
        return f"snes_read_{kind}16(ctx, {reg_expr})"
    ea = emit_ea_expr(mode, val)
    return f"snes_bus_read{bits}(ctx, {ea})"


def emit_write_stmt(mode, val, bits, val_expr):
    mmio = classify_static_mmio(mode, val)
    if mmio is not None:
        kind, reg_expr = mmio
        if bits == 8:
            return f"snes_write_{kind}(ctx, {reg_expr}, (uint8_t)({val_expr}));"
        return f"snes_write_{kind}16(ctx, {reg_expr}, (uint16_t)({val_expr}));"
    ea = emit_ea_expr(mode, val)
    return f"snes_bus_write{bits}(ctx, {ea}, (uint{bits}_t)({val_expr}));"


def emit_single_width_insn(bank, pc, mn, mode, val, m, x, pstk_top, func_entries, indirect_sites, indent="    "):
    """Emit C statements for one non-branch, non-JMP instruction under fixed (m, x)."""
    lines = []
    abits = 8 if m else 16
    xbits = 8 if x else 16

    if mn == "NOP":
        lines.append(f"{indent}/* NOP */")
    elif mn == "CLC":
        lines.append(f"{indent}ctx->flag_c = 0u;")
    elif mn == "SEC":
        lines.append(f"{indent}ctx->flag_c = 1u;")
    elif mn == "CLI":
        lines.append(f"{indent}ctx->flag_i = 0u;")
    elif mn == "SEI":
        lines.append(f"{indent}ctx->flag_i = 1u;")
    elif mn == "CLV":
        lines.append(f"{indent}ctx->flag_v = 0u;")
    elif mn == "CLD":
        lines.append(f"{indent}ctx->flag_d = 0u;")
    elif mn == "SED":
        lines.append(f"{indent}ctx->flag_d = 1u;")
    elif mn == "XCE":
        lines.append(f"{indent}{{ uint8_t tmp_ce = ctx->flag_c; ctx->flag_c = ctx->flag_e; ctx->flag_e = tmp_ce; }}")
        lines.append(f"{indent}if (ctx->flag_e) {{ ctx->flag_m = 1u; ctx->flag_x = 1u; ctx->x &= 0x00FFu; ctx->y &= 0x00FFu; ctx->sp = (uint16_t)(0x0100u | (ctx->sp & 0x00FFu)); }}")
    elif mn == "SEP":
        nm = 1 if (val & 0x20) else m
        nx = 1 if (val & 0x10) else x
        lines.append(f"{indent}/* SEP #${val:02X} -> Register Width Transition: [{mx_str(nm, nx)}] */")
        lines.append(f"{indent}snes_sep(ctx, 0x{val:02X}u);")
    elif mn == "REP":
        nm = 0 if (val & 0x20) else m
        nx = 0 if (val & 0x10) else x
        lines.append(f"{indent}/* REP #${val:02X} -> Register Width Transition: [{mx_str(nm, nx)}] */")
        lines.append(f"{indent}snes_rep(ctx, 0x{val:02X}u);")
    elif mn == "LDA":
        rexp = emit_read_expr(mode, val, abits)
        if abits == 8:
            lines.append(f"{indent}{{ uint8_t v8 = {rexp}; ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }}")
        else:
            lines.append(f"{indent}ctx->a = {rexp};")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
    elif mn == "LDX":
        rexp = emit_read_expr(mode, val, xbits)
        if xbits == 8:
            lines.append(f"{indent}ctx->x = (uint16_t)({rexp});")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->x);")
        else:
            lines.append(f"{indent}ctx->x = {rexp};")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->x);")
    elif mn == "LDY":
        rexp = emit_read_expr(mode, val, xbits)
        if xbits == 8:
            lines.append(f"{indent}ctx->y = (uint16_t)({rexp});")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->y);")
        else:
            lines.append(f"{indent}ctx->y = {rexp};")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->y);")
    elif mn == "STA":
        v = "(uint8_t)(ctx->a & 0xFFu)" if abits == 8 else "ctx->a"
        lines.append(f"{indent}{emit_write_stmt(mode, val, abits, v)}")
    elif mn == "STX":
        v = "(uint8_t)(ctx->x & 0xFFu)" if xbits == 8 else "ctx->x"
        lines.append(f"{indent}{emit_write_stmt(mode, val, xbits, v)}")
    elif mn == "STY":
        v = "(uint8_t)(ctx->y & 0xFFu)" if xbits == 8 else "ctx->y"
        lines.append(f"{indent}{emit_write_stmt(mode, val, xbits, v)}")
    elif mn == "STZ":
        lines.append(f"{indent}{emit_write_stmt(mode, val, abits, '0u')}")
    elif mn == "ADC":
        rexp = emit_read_expr(mode, val, abits)
        lines.append(f"{indent}snes_adc{abits}(ctx, {rexp});")
    elif mn == "SBC":
        rexp = emit_read_expr(mode, val, abits)
        lines.append(f"{indent}snes_sbc{abits}(ctx, {rexp});")
    elif mn == "AND":
        rexp = emit_read_expr(mode, val, abits)
        if abits == 8:
            lines.append(f"{indent}{{ uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) & ({rexp})); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }}")
        else:
            lines.append(f"{indent}ctx->a &= {rexp};")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
    elif mn == "ORA":
        rexp = emit_read_expr(mode, val, abits)
        if abits == 8:
            lines.append(f"{indent}{{ uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) | ({rexp})); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }}")
        else:
            lines.append(f"{indent}ctx->a |= {rexp};")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
    elif mn == "EOR":
        rexp = emit_read_expr(mode, val, abits)
        if abits == 8:
            lines.append(f"{indent}{{ uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) ^ ({rexp})); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }}")
        else:
            lines.append(f"{indent}ctx->a ^= {rexp};")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
    elif mn == "CMP":
        rexp = emit_read_expr(mode, val, abits)
        lhs = "(uint8_t)(ctx->a & 0xFFu)" if abits == 8 else "ctx->a"
        lines.append(f"{indent}snes_cmp{abits}(ctx, {lhs}, {rexp});")
    elif mn == "CPX":
        rexp = emit_read_expr(mode, val, xbits)
        lhs = "(uint8_t)(ctx->x & 0xFFu)" if xbits == 8 else "ctx->x"
        lines.append(f"{indent}snes_cmp{xbits}(ctx, {lhs}, {rexp});")
    elif mn == "CPY":
        rexp = emit_read_expr(mode, val, xbits)
        lhs = "(uint8_t)(ctx->y & 0xFFu)" if xbits == 8 else "ctx->y"
        lines.append(f"{indent}snes_cmp{xbits}(ctx, {lhs}, {rexp});")
    elif mn == "BIT":
        rexp = emit_read_expr(mode, val, abits)
        if mode == "immm":
            # Immediate BIT modifies ONLY Z flag on 65816!
            if abits == 8:
                lines.append(f"{indent}ctx->flag_z = (uint8_t)(((ctx->a & 0xFFu) & ({rexp})) == 0u ? 1u : 0u);")
            else:
                lines.append(f"{indent}ctx->flag_z = (uint8_t)((ctx->a & ({rexp})) == 0u ? 1u : 0u);")
        else:
            lines.append(f"{indent}snes_bit{abits}(ctx, {rexp});")
    elif mn == "TSB":
        if abits == 8:
            rexp = emit_read_expr(mode, val, 8)
            wstmt = emit_write_stmt(mode, val, 8, "m8 | (uint8_t)(ctx->a & 0xFFu)")
            lines.append(f"{indent}{{ uint8_t m8 = {rexp}; ctx->flag_z = (uint8_t)(((ctx->a & 0xFFu) & m8) == 0u ? 1u : 0u); {wstmt} }}")
        else:
            rexp = emit_read_expr(mode, val, 16)
            wstmt = emit_write_stmt(mode, val, 16, "m16 | ctx->a")
            lines.append(f"{indent}{{ uint16_t m16 = {rexp}; ctx->flag_z = (uint8_t)((ctx->a & m16) == 0u ? 1u : 0u); {wstmt} }}")
    elif mn == "TRB":
        if abits == 8:
            rexp = emit_read_expr(mode, val, 8)
            wstmt = emit_write_stmt(mode, val, 8, "m8 & (uint8_t)(~(ctx->a & 0xFFu))")
            lines.append(f"{indent}{{ uint8_t m8 = {rexp}; ctx->flag_z = (uint8_t)(((ctx->a & 0xFFu) & m8) == 0u ? 1u : 0u); {wstmt} }}")
        else:
            rexp = emit_read_expr(mode, val, 16)
            wstmt = emit_write_stmt(mode, val, 16, "m16 & (uint16_t)(~ctx->a)")
            lines.append(f"{indent}{{ uint16_t m16 = {rexp}; ctx->flag_z = (uint8_t)((ctx->a & m16) == 0u ? 1u : 0u); {wstmt} }}")
    elif mn in ("INC", "DEC"):
        delta = "+ 1u" if mn == "INC" else "- 1u"
        if mode == "acc":
            if abits == 8:
                lines.append(f"{indent}{{ uint8_t v8 = (uint8_t)((ctx->a & 0xFFu) {delta}); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }}")
            else:
                lines.append(f"{indent}ctx->a = (uint16_t)(ctx->a {delta});")
                lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
        else:
            rexp = emit_read_expr(mode, val, abits)
            wstmt = emit_write_stmt(mode, val, abits, f"v{abits}")
            lines.append(f"{indent}{{ uint{abits}_t v{abits} = (uint{abits}_t)(({rexp}) {delta}); snes_set_nz{abits}(ctx, v{abits}); {wstmt} }}")
    elif mn in ("INX", "DEX"):
        delta = "+ 1u" if mn == "INX" else "- 1u"
        if xbits == 8:
            lines.append(f"{indent}ctx->x = (uint16_t)((ctx->x {delta}) & 0x00FFu);")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->x);")
        else:
            lines.append(f"{indent}ctx->x = (uint16_t)(ctx->x {delta});")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->x);")
    elif mn in ("INY", "DEY"):
        delta = "+ 1u" if mn == "INY" else "- 1u"
        if xbits == 8:
            lines.append(f"{indent}ctx->y = (uint16_t)((ctx->y {delta}) & 0x00FFu);")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->y);")
        else:
            lines.append(f"{indent}ctx->y = (uint16_t)(ctx->y {delta});")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->y);")
    elif mn in ("ASL", "LSR", "ROL", "ROR"):
        fn = f"snes_{mn.lower()}{abits}"
        if mode == "acc":
            if abits == 8:
                lines.append(f"{indent}{{ uint8_t v8 = {fn}(ctx, (uint8_t)(ctx->a & 0xFFu)); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); }}")
            else:
                lines.append(f"{indent}ctx->a = {fn}(ctx, ctx->a);")
        else:
            rexp = emit_read_expr(mode, val, abits)
            wstmt = emit_write_stmt(mode, val, abits, f"v{abits}")
            lines.append(f"{indent}{{ uint{abits}_t v{abits} = {fn}(ctx, {rexp}); {wstmt} }}")
    elif mn == "TAX":
        if xbits == 8:
            lines.append(f"{indent}ctx->x = (uint16_t)(ctx->a & 0x00FFu);")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->x);")
        else:
            lines.append(f"{indent}ctx->x = ctx->a;")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->x);")
    elif mn == "TAY":
        if xbits == 8:
            lines.append(f"{indent}ctx->y = (uint16_t)(ctx->a & 0x00FFu);")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->y);")
        else:
            lines.append(f"{indent}ctx->y = ctx->a;")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->y);")
    elif mn == "TXA":
        if abits == 8:
            lines.append(f"{indent}ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (ctx->x & 0x00FFu));")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));")
        else:
            lines.append(f"{indent}ctx->a = ctx->x;")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
    elif mn == "TYA":
        if abits == 8:
            lines.append(f"{indent}ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (ctx->y & 0x00FFu));")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));")
        else:
            lines.append(f"{indent}ctx->a = ctx->y;")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
    elif mn == "TXY":
        if xbits == 8:
            lines.append(f"{indent}ctx->y = (uint16_t)(ctx->x & 0x00FFu);")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->y);")
        else:
            lines.append(f"{indent}ctx->y = ctx->x;")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->y);")
    elif mn == "TYX":
        if xbits == 8:
            lines.append(f"{indent}ctx->x = (uint16_t)(ctx->y & 0x00FFu);")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->x);")
        else:
            lines.append(f"{indent}ctx->x = ctx->y;")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->x);")
    elif mn == "TXS":
        lines.append(f"{indent}ctx->sp = ctx->x;")
    elif mn == "TSX":
        if xbits == 8:
            lines.append(f"{indent}ctx->x = (uint16_t)(ctx->sp & 0x00FFu);")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->x);")
        else:
            lines.append(f"{indent}ctx->x = ctx->sp;")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->x);")
    elif mn == "TCD":
        lines.append(f"{indent}ctx->dp = ctx->a;")
        lines.append(f"{indent}snes_set_nz16(ctx, ctx->dp);")
    elif mn == "TDC":
        lines.append(f"{indent}ctx->a = ctx->dp;")
        lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
    elif mn == "TCS":
        lines.append(f"{indent}ctx->sp = ctx->a;")
    elif mn == "TSC":
        lines.append(f"{indent}ctx->a = ctx->sp;")
        lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
    elif mn == "XBA":
        lines.append(f"{indent}ctx->a = (uint16_t)((ctx->a << 8) | (ctx->a >> 8));")
        lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)(ctx->a & 0xFFu));")
    elif mn == "PHA":
        if abits == 8:
            lines.append(f"{indent}snes_push8(ctx, (uint8_t)(ctx->a & 0xFFu));")
        else:
            lines.append(f"{indent}snes_push16(ctx, ctx->a);")
    elif mn == "PLA":
        if abits == 8:
            lines.append(f"{indent}{{ uint8_t v8 = snes_pull8(ctx); ctx->a = (uint16_t)((ctx->a & 0xFF00u) | (uint16_t)v8); snes_set_nz8(ctx, v8); }}")
        else:
            lines.append(f"{indent}ctx->a = snes_pull16(ctx);")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->a);")
    elif mn == "PHX":
        if xbits == 8:
            lines.append(f"{indent}snes_push8(ctx, (uint8_t)(ctx->x & 0xFFu));")
        else:
            lines.append(f"{indent}snes_push16(ctx, ctx->x);")
    elif mn == "PLX":
        if xbits == 8:
            lines.append(f"{indent}ctx->x = (uint16_t)snes_pull8(ctx);")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->x);")
        else:
            lines.append(f"{indent}ctx->x = snes_pull16(ctx);")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->x);")
    elif mn == "PHY":
        if xbits == 8:
            lines.append(f"{indent}snes_push8(ctx, (uint8_t)(ctx->y & 0xFFu));")
        else:
            lines.append(f"{indent}snes_push16(ctx, ctx->y);")
    elif mn == "PLY":
        if xbits == 8:
            lines.append(f"{indent}ctx->y = (uint16_t)snes_pull8(ctx);")
            lines.append(f"{indent}snes_set_nz8(ctx, (uint8_t)ctx->y);")
        else:
            lines.append(f"{indent}ctx->y = snes_pull16(ctx);")
            lines.append(f"{indent}snes_set_nz16(ctx, ctx->y);")
    elif mn == "PHP":
        lines.append(f"{indent}snes_push8(ctx, snes_pack_p(ctx));")
    elif mn == "PLP":
        if pstk_top is not None:
            rm, rx = pstk_top
            lines.append(f"{indent}/* PLP -> Restores saved status register [{mx_str(rm, rx)}] */")
        else:
            lines.append(f"{indent}/* PLP -> Restores dynamic status register */")
        lines.append(f"{indent}snes_unpack_p(ctx, snes_pull8(ctx));")
    elif mn == "PHB":
        lines.append(f"{indent}snes_push8(ctx, ctx->db);")
    elif mn == "PLB":
        lines.append(f"{indent}ctx->db = snes_pull8(ctx);")
        lines.append(f"{indent}snes_set_nz8(ctx, ctx->db);")
    elif mn == "PHD":
        lines.append(f"{indent}snes_push16(ctx, ctx->dp);")
    elif mn == "PLD":
        lines.append(f"{indent}ctx->dp = snes_pull16(ctx);")
        lines.append(f"{indent}snes_set_nz16(ctx, ctx->dp);")
    elif mn == "PHK":
        lines.append(f"{indent}snes_push8(ctx, ctx->pb);")
    elif mn == "MVN":
        dst_b = val & 0xFF
        src_b = (val >> 8) & 0xFF
        lines.append(f"{indent}snes_block_mvn(ctx, 0x{dst_b:02X}u, 0x{src_b:02X}u);")
    elif mn == "MVP":
        dst_b = val & 0xFF
        src_b = (val >> 8) & 0xFF
        lines.append(f"{indent}snes_block_mvp(ctx, 0x{dst_b:02X}u, 0x{src_b:02X}u);")
    elif mn == "JSR":
        if mode == "abs":
            lines.append(f"{indent}{func_cname(bank, val)}(ctx);")
        elif mode == "absxi":
            _, _, _, _, tgts = indirect_sites[(bank, pc)]
            lines.append(f"{indent}{{")
            lines.append(f"{indent}    uint16_t jsr_ptr = (uint16_t)(0x{val:04X}u + ctx->x);")
            lines.append(f"{indent}    uint16_t jsr_tgt = snes_bus_read16(ctx, ((uint32_t)ctx->pb << 16) | (uint32_t)jsr_ptr);")
            lines.append(f"{indent}    switch (jsr_tgt) {{")
            for tgt in sorted(set(t for t in tgts if t)):
                lines.append(f"{indent}        case 0x{tgt:04X}u: {func_cname(bank, tgt)}(ctx); break;")
            lines.append(f"{indent}        default:")
            lines.append(f"{indent}            // REQUIRES INTERPRETER FALLBACK")
            lines.append(f"{indent}            snes_interp_fallback_at(ctx, 0x{bank:02X}{pc:04X}u, ((uint32_t)ctx->pb << 16) | (uint32_t)jsr_tgt);")
            lines.append(f"{indent}            break;")
            lines.append(f"{indent}    }}")
            lines.append(f"{indent}}}")
    elif mn == "JSL":
        tb = ((val >> 16) & 0xFF) & 0x1F
        ta = val & 0xFFFF
        if tb != (bank & 0x1F):
            lines.append(f"{indent}{{ uint8_t saved_pb = ctx->pb; ctx->pb = 0x{tb:02X}u; {func_cname(tb, ta)}(ctx); ctx->pb = saved_pb; }}")
        else:
            lines.append(f"{indent}{func_cname(tb, ta)}(ctx);")
    elif mn in ("RTS", "RTL"):
        lines.append(f"{indent}return;")
    elif mn == "RTI":
        lines.append(f"{indent}snes_unpack_p(ctx, isr_saved_p);")
        lines.append(f"{indent}return;")
    else:
        raise ValueError(f"Unhandled mnemonic {mn} ({mode}) at ${bank:02X}:{pc:04X}")

    return lines


def branch_cond_c(mn):
    return {
        "BCC": "!ctx->flag_c",
        "BCS": "ctx->flag_c",
        "BEQ": "ctx->flag_z",
        "BNE": "!ctx->flag_z",
        "BMI": "ctx->flag_n",
        "BPL": "!ctx->flag_n",
        "BVC": "!ctx->flag_v",
        "BVS": "ctx->flag_v",
    }[mn]


def translate_function(fb, fpc, fm, fx, reason, func_entries, indirect_sites):
    """Build complete intra-bank CFG for function (fb, fpc) and emit clean C11 code."""
    pc_mx_states = defaultdict(set)
    pc_pstk_top = {}
    pc_decoded = {}
    sq = deque([(fpc, fm, fx, ())])
    seen = {(fpc, fm, fx, ())}

    def is_local_cont(target_pc, npstk):
        if target_pc == fpc:
            return True
        if (fb, target_pc) in func_entries and not npstk:
            return False
        return True

    while sq:
        pc, m, x, pstk = sq.popleft()
        op = u8(fb, pc)
        if op is None or op in (0x00, 0x02, 0xDB):
            continue
        mn, mode = OPS[op]
        n = (1 if m else 2) if mode == "immm" else ((1 if x else 2) if mode == "immx" else MODE_LEN[mode])
        raw = [u8(fb, pc + i) for i in range(1 + n)]
        val = int.from_bytes(bytes(raw[1:]), "little") if n > 0 else 0

        pc_mx_states[pc].add((m, x))
        pc_pstk_top[pc] = pstk[-1] if pstk else None
        pc_decoded[pc] = (op, mn, mode, n, val, raw)

        nm, nx, npstk = m, x, pstk
        if mn == "REP":
            if val & 0x20:
                nm = 0
            if val & 0x10:
                nx = 0
        elif mn == "SEP":
            if val & 0x20:
                nm = 1
            if val & 0x10:
                nx = 1
        elif mn == "PHP":
            if len(pstk) < 4:
                npstk = pstk + ((m, x),)
        elif mn == "PLP":
            if pstk:
                nm, nx = pstk[-1]
                npstk = pstk[:-1]

        npc = pc + 1 + n
        if mn in ("RTS", "RTL", "RTI", "JML"):
            continue
        elif mn == "JMP":
            if mode == "abs" and is_local_cont(val, npstk):
                st = (val, nm, nx, npstk)
                if st not in seen:
                    seen.add(st)
                    sq.append(st)
            continue
        elif mn in BRANCH_OPS:
            d = (val - 256 if val > 127 else val) if mode == "rel" else (val - 65536 if val > 32767 else val)
            tgt = (pc + (2 if mode == "rel" else 3) + d) & 0xFFFF
            if is_local_cont(tgt, npstk):
                st1 = (tgt, nm, nx, npstk)
                if st1 not in seen:
                    seen.add(st1)
                    sq.append(st1)
            if mn not in ("BRA", "BRL") and is_local_cont(npc, npstk):
                st2 = (npc, nm, nx, npstk)
                if st2 not in seen:
                    seen.add(st2)
                    sq.append(st2)
        else:
            if is_local_cont(npc, npstk):
                st = (npc, nm, nx, npstk)
                if st not in seen:
                    seen.add(st)
                    sq.append(st)

    ordered_pcs = sorted(pc_decoded.keys())
    pc_index = {pc: idx for idx, pc in enumerate(ordered_pcs)}

    # First pass: collect which local PCs actually need a C label `L_xxxx:`
    needed_labels = set()
    if fpc != ordered_pcs[0]:
        needed_labels.add(fpc)

    for idx, pc in enumerate(ordered_pcs):
        op, mn, mode, n, val, raw = pc_decoded[pc]
        npc = pc + 1 + n
        next_emitted_pc = ordered_pcs[idx + 1] if idx + 1 < len(ordered_pcs) else None

        if mn == "JMP" and mode == "abs":
            if val in pc_index:
                needed_labels.add(val)
        elif mn in BRANCH_OPS:
            d = (val - 256 if val > 127 else val) if mode == "rel" else (val - 65536 if val > 32767 else val)
            tgt = (pc + (2 if mode == "rel" else 3) + d) & 0xFFFF
            if tgt in pc_index:
                needed_labels.add(tgt)
            if mn not in ("BRA", "BRL") and npc in pc_index and npc != next_emitted_pc:
                needed_labels.add(npc)
        elif mn not in ("RTS", "RTL", "RTI", "JMP", "JML"):
            if npc in pc_index and npc != next_emitted_pc:
                needed_labels.add(npc)

    has_rti = any(dec[1] == "RTI" for dec in pc_decoded.values())

    # Emit function header & body
    out = []
    cname = func_cname(fb, fpc)
    gname = GHIDRA_PROCS.get((fb, fpc))
    annot = KNOWN_ANNOTATIONS.get((fb, fpc))
    bank_ev = EVIDENCE_MANIFEST["banks"][f"{fb:02X}"]["evidence_id"]

    out.append("/* ==========================================================================")
    out.append(f" * Function: {cname}  (SNES ${fb:02X}:{fpc:04X} | Ghidra 0x{(fb << 16) | fpc:x})")
    if gname:
        out.append(f" * Ghidra Procedure: {gname} (inventory {GHIDRA_LIST_PROCS_EVID})")
    if annot:
        out.append(f" * Role: {annot}")
    out.append(f" * Discovery Provenance: {reason}")
    out.append(f" * REA Bank Evidence ID: {bank_ev}")
    out.append(f" * Entry Register Widths: [{mx_str(fm, fx)}]")
    out.append(" * ========================================================================== */")
    out.append(f"void {cname}(SnesCpuContext *ctx) {{")
    out.append("    (void)ctx;")
    if has_rti:
        out.append("    /* Hardware ISR entry saves processor status P (restored by RTI) */")
        out.append("    uint8_t isr_saved_p = snes_pack_p(ctx);")
    out.append(f"    /* [Entry State: {mx_str(fm, fx)}] */")
    if fpc != ordered_pcs[0]:
        out.append(f"    goto L_{fpc:04x};")

    for idx, pc in enumerate(ordered_pcs):
        op, mn, mode, n, val, raw = pc_decoded[pc]
        npc = pc + 1 + n
        next_emitted_pc = ordered_pcs[idx + 1] if idx + 1 < len(ordered_pcs) else None
        states = sorted(pc_mx_states[pc])

        if pc in needed_labels:
            st_desc = " | ".join(mx_str(sm, sx) for sm, sx in states)
            out.append(f"L_{pc:04x}: /* [{st_desc}] */")

        # Main-loop exit hook at $00:8166 inside WarmReset ($00:80A3) for clean bounded test execution
        if fb == 0 and pc == 0x8166:
            out.append("    if (ctx->max_main_loop_iters != 0u && ctx->main_loop_iters++ >= ctx->max_main_loop_iters) {")
            out.append("        ctx->stop_main_loop = true;")
            out.append("        return;")
            out.append("    }")
        # Cooperative VBlank NMI delivery when spinning at $00:85EC inside VBlank_WaitFrame ($00:85DF)
        if fb == 0 and pc == 0x85EC:
            out.append("    if ((ctx->wram[0x003Cu] & 0x80u) != 0u) {")
            out.append("        bank00_func_819b(ctx);")
            out.append("    }")

        hex_bytes = " ".join(f"{b:02X}" for b in raw)
        asm_op = format_operand_asm(fb, pc, mn, mode, n, val)
        asm_text = f"{mn} {asm_op}".strip()
        m0, x0 = states[0]
        out.append(f"    /* ${fb:02X}:{pc:04X}: {hex_bytes:<11s} | {asm_text:<18s} [{mx_str(m0, x0)}] */")

        # Control-flow instructions
        if mn == "JMP":
            if mode == "abs":
                if val in pc_index:
                    out.append(f"    goto L_{val:04x};")
                else:
                    out.append(f"    {func_cname(fb, val)}(ctx);")
                    out.append("    return;")
            elif mode == "absxi":
                _, _, _, _, tgts = indirect_sites[(fb, pc)]
                out.append("    {")
                out.append(f"        uint16_t jmp_ptr = (uint16_t)(0x{val:04X}u + ctx->x);")
                out.append("        uint16_t jmp_tgt = snes_bus_read16(ctx, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_ptr);")
                out.append("        switch (jmp_tgt) {")
                for tgt in sorted(set(t for t in tgts if t)):
                    out.append(f"            case 0x{tgt:04X}u: {func_cname(fb, tgt)}(ctx); return;")
                out.append("            default:")
                out.append("                // REQUIRES INTERPRETER FALLBACK")
                out.append(f"                snes_interp_fallback_at(ctx, 0x{fb:02X}{pc:04X}u, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);")
                out.append("                return;")
                out.append("        }")
                out.append("    }")
            elif mode == "absi":
                _, _, _, _, tgts = indirect_sites[(fb, pc)]
                out.append("    {")
                out.append(f"        uint16_t jmp_tgt = snes_bus_read16(ctx, 0x00{val:04X}u);")
                out.append("        switch (jmp_tgt) {")
                for tgt in sorted(set(t for t in tgts if t)):
                    out.append(f"            case 0x{tgt:04X}u: {func_cname(fb, tgt)}(ctx); return;")
                if 0 in tgts:
                    out.append("            case 0x0000u: return; /* Null slot 0 in jump table */")
                out.append("            default:")
                out.append("                // REQUIRES INTERPRETER FALLBACK")
                out.append(f"                snes_interp_fallback_at(ctx, 0x{fb:02X}{pc:04X}u, ((uint32_t)ctx->pb << 16) | (uint32_t)jmp_tgt);")
                out.append("                return;")
                out.append("        }")
                out.append("    }")
            continue

        if mn == "JML":
            if mode == "long":
                tb = ((val >> 16) & 0xFF) & 0x1F
                ta = val & 0xFFFF
                if tb != (fb & 0x1F):
                    out.append(f"    {{ uint8_t saved_pb = ctx->pb; ctx->pb = 0x{tb:02X}u; {func_cname(tb, ta)}(ctx); ctx->pb = saved_pb; }}")
                else:
                    out.append(f"    {func_cname(tb, ta)}(ctx);")
                out.append("    return;")
            else:
                _, _, _, _, tgts24 = indirect_sites[(fb, pc)]
                out.append("    {")
                out.append(f"        uint32_t jml_tgt = (uint32_t)snes_bus_read16(ctx, 0x00{val:04X}u) | ((uint32_t)snes_bus_read8(ctx, 0x00{(val + 2) & 0xFFFF:04X}u) << 16);")
                out.append("        uint32_t jml_norm = ((jml_tgt & 0x1F0000u) | (jml_tgt & 0x00FFFFu));")
                out.append("        switch (jml_norm) {")
                for tb2, ta2 in sorted(set((tb2, ta2) for tb2, ta2 in tgts24 if ta2)):
                    norm = (tb2 << 16) | ta2
                    out.append(f"            case 0x{norm:06X}u: {{ uint8_t saved_pb = ctx->pb; ctx->pb = 0x{tb2:02X}u; {func_cname(tb2, ta2)}(ctx); ctx->pb = saved_pb; return; }}")
                if (0, 0) in tgts24:
                    out.append("            case 0x000000u: return; /* Null slot 0 in 24-bit dispatch table */")
                out.append("            default:")
                out.append("                // REQUIRES INTERPRETER FALLBACK")
                out.append(f"                snes_interp_fallback_at(ctx, 0x{fb:02X}{pc:04X}u, jml_tgt);")
                out.append("                return;")
                out.append("        }")
                out.append("    }")
            continue

        if mn in BRANCH_OPS:
            d = (val - 256 if val > 127 else val) if mode == "rel" else (val - 65536 if val > 32767 else val)
            tgt = (pc + (2 if mode == "rel" else 3) + d) & 0xFFFF
            if mn in ("BRA", "BRL"):
                if tgt in pc_index:
                    out.append(f"    goto L_{tgt:04x};")
                else:
                    out.append(f"    {func_cname(fb, tgt)}(ctx);")
                    out.append("    return;")
            else:
                cond = branch_cond_c(mn)
                if tgt in pc_index:
                    out.append(f"    if ({cond}) goto L_{tgt:04x};")
                else:
                    out.append(f"    if ({cond}) {{ {func_cname(fb, tgt)}(ctx); return; }}")
                if npc in pc_index:
                    if npc != next_emitted_pc:
                        out.append(f"    goto L_{npc:04x};")
                else:
                    out.append(f"    {func_cname(fb, npc)}(ctx);")
                    out.append("    return;")
            continue

        # Ordinary or JSR/JSL/RTS/RTL/RTI instruction:
        # Check if PC has dynamic M/X merge (e.g., $00:93ED in SPC700_UploadBlock)
        if len(states) > 1 and mn not in ("REP", "SEP", "RTS", "RTL", "RTI"):
            out.append("    /* Dynamic M-width merge point: dispatch on runtime ctx->flag_m */")
            out.append("    if (ctx->flag_m) {")
            out.extend(emit_single_width_insn(fb, pc, mn, mode, val, 1, x0, pc_pstk_top[pc], func_entries, indirect_sites, indent="        "))
            out.append("    } else {")
            out.extend(emit_single_width_insn(fb, pc, mn, mode, val, 0, x0, pc_pstk_top[pc], func_entries, indirect_sites, indent="        "))
            out.append("    }")
        else:
            out.extend(emit_single_width_insn(fb, pc, mn, mode, val, m0, x0, pc_pstk_top[pc], func_entries, indirect_sites, indent="    "))

        if mn not in ("RTS", "RTL", "RTI"):
            if npc in pc_index:
                if npc != next_emitted_pc:
                    out.append(f"    goto L_{npc:04x};")
            else:
                out.append(f"    /* Fallthrough into function ${fb:02X}:{npc:04X} */")
                out.append(f"    {func_cname(fb, npc)}(ctx);")
                out.append("    return;")

    out.append("}")
    out.append("")
    return "\n".join(out), len(ordered_pcs)


def main():
    os.makedirs(os.path.join(WORKSPACE_DIR, "include"), exist_ok=True)
    os.makedirs(os.path.join(WORKSPACE_DIR, "src"), exist_ok=True)

    func_entries, ins_at, indirect_sites = build_whole_rom_cfg()
    code_banks = [0x00, 0x01, 0x02, 0x03, 0x04, 0x1D]

    funcs_by_bank = defaultdict(list)
    for (b, fpc), (fm, fx, reason) in sorted(func_entries.items()):
        funcs_by_bank[b].append((fpc, fm, fx, reason))

    # 1. Write include/contra3_recomp.h
    hdr = [
        "#ifndef CONTRA3_RECOMP_H",
        "#define CONTRA3_RECOMP_H",
        "",
        '#include "cpu_65816.h"',
        "",
        "#ifdef __cplusplus",
        'extern "C" {',
        "#endif",
        "",
        "typedef struct Contra3BankManifestEntry {",
        "    uint8_t     bank;",
        "    uint32_t    ghidra_base_addr;",
        "    uint32_t    byte_length;",
        "    bool        has_executable_code;",
        "    uint32_t    function_count;",
        "    uint32_t    decoded_insn_count;",
        "    const char *classification;",
        "    const char *rea_evidence_id;",
        "    const char *sha256;",
        "} Contra3BankManifestEntry;",
        "",
        "extern const Contra3BankManifestEntry g_contra3_bank_manifest[32];",
        "bool contra3_dispatch_call(SnesCpuContext *ctx, uint32_t addr24);",
        "",
    ]

    for b in code_banks:
        hdr.append(f"/* --- Bank ${b:02X} ({len(funcs_by_bank[b])} functions) --- */")
        for fpc, fm, fx, _ in funcs_by_bank[b]:
            hdr.append(f"void {func_cname(b, fpc)}(SnesCpuContext *ctx); /* ${b:02X}:{fpc:04X} [{mx_str(fm, fx)}] */")
        hdr.append("")

    hdr.extend([
        "#ifdef __cplusplus",
        "}",
        "#endif",
        "",
        "#endif /* CONTRA3_RECOMP_H */",
        "",
    ])

    with open(os.path.join(WORKSPACE_DIR, "include", "contra3_recomp.h"), "w", encoding="utf-8") as f:
        f.write("\n".join(hdr))

    # 2. Translate each code bank in order: 00 -> 01 -> 02 -> 03 -> 04 -> 1D
    bank_stats = {}
    for b in code_banks:
        b_ev = EVIDENCE_MANIFEST["banks"][f"{b:02X}"]["evidence_id"]
        b_sha = EVIDENCE_MANIFEST["banks"][f"{b:02X}"]["sha256"]
        src_lines = [
            "/*",
            f" * Contra III: The Alien Wars (USA) - Statically Recompiled Bank ${b:02X}",
            f" * SNES LoROM Range : ${b:02X}:8000 - ${b:02X}:FFFF (32,768 bytes)",
            f" * Ghidra Base Addr : 0x{(b << 16) | 0x8000:x}",
            f" * REA Evidence ID  : {b_ev}",
            f" * Bank SHA-256     : {b_sha}",
            f" * Functions        : {len(funcs_by_bank[b])}",
            " */",
            "",
            '#include "contra3_recomp.h"',
            "",
        ]
        emitted_ins_sum = 0
        for fpc, fm, fx, reason in funcs_by_bank[b]:
            c_body, fn_ins_cnt = translate_function(b, fpc, fm, fx, reason, func_entries, indirect_sites)
            src_lines.append(c_body)
            emitted_ins_sum += fn_ins_cnt

        out_c_path = os.path.join(WORKSPACE_DIR, "src", f"bank_{b:02x}.c")
        with open(out_c_path, "w", encoding="utf-8") as f:
            f.write("\n".join(src_lines))

        uniq_ins = sum(1 for (ib, _) in ins_at if ib == b)
        ind_b = sum(1 for (ib, _) in indirect_sites if ib == b)
        bank_stats[f"{b:02X}"] = {
            "functions": len(funcs_by_bank[b]),
            "unique_instructions": uniq_ins,
            "emitted_instructions": emitted_ins_sum,
            "indirect_sites": ind_b,
            "c_file": f"src/bank_{b:02x}.c",
        }
        print(f"[Translated Bank {b:02X}] -> {out_c_path} ({len(funcs_by_bank[b])} funcs, {uniq_ins} unique insns, {ind_b} indirects)")

    # 3. Write src/dispatch.c (24-bit address -> compiled C function)
    disp = [
        "/*",
        " * Global 24-bit Function Dispatcher for Contra III Static Recompilation",
        f" * Maps all {len(func_entries)} discovered 65816 entry points across Banks 00, 01, 02, 03, 04, 1D.",
        " */",
        "",
        '#include "contra3_recomp.h"',
        "",
        "bool contra3_dispatch_call(SnesCpuContext *ctx, uint32_t addr24) {",
        "    uint8_t bank = (uint8_t)((addr24 >> 16) & 0x1Fu);",
        "    uint16_t pc  = (uint16_t)(addr24 & 0xFFFFu);",
        "    uint32_t key = ((uint32_t)bank << 16) | (uint32_t)pc;",
        "    uint8_t saved_pb = ctx->pb;",
        "    ctx->pb = bank;",
        "    switch (key) {",
    ]
    for (b, fpc) in sorted(func_entries.keys()):
        key24 = (b << 16) | fpc
        disp.append(f"        case 0x{key24:06X}u: {func_cname(b, fpc)}(ctx); ctx->pb = saved_pb; return true;")
    disp.extend([
        "        default:",
        "            ctx->pb = saved_pb;",
        "            return false;",
        "    }",
        "}",
        "",
    ])
    with open(os.path.join(WORKSPACE_DIR, "src", "dispatch.c"), "w", encoding="utf-8") as f:
        f.write("\n".join(disp))

    # 4. Write src/bank_data_manifest.c covering all 32 ROM banks ($00-$1F)
    bank_roles = {
        0x00: "Executable Code: Core Engine, Boot/NMI/IRQ, DMA/VRAM, Konami LZSS, SPC700 Loader, Actor/Stage Dispatchers",
        0x01: "Executable Code: Player Controller, Weapons, Projectiles, Side-Scrolling Stage Enemies",
        0x02: "Executable Code: Stage Bosses, Mode 7 Overhead Stages (Stages 2 & 5), Multi-Part Entities",
        0x03: "Hybrid Data + Code: Mode 7 Lookup/Tile Data ($8000-$E17F) + Mode 7 Stage Mechanics ($E180-$FE4B)",
        0x04: "Executable Code: Cutscenes, Intro/Ending Sequences, Stage Events, Mid-Bosses & Final Bosses",
        0x05: "Read-Only Master Data Bank (Default DB=$05): 24-bit Dispatch Tables ($A0B4, $A48A) & Asset Load Scripts ($F537, $F55B)",
        0x06: "Read-Only Data Bank (DB=$06): Stage Layout, Metatile & Collision Tables + LZSS Graphics",
        0x07: "Read-Only Data Bank (DB=$07): Mode 7 Overhead Stage Maps & Lookup Tables + LZSS Graphics",
        0x08: "Read-Only Data Bank (DB=$08): Master CGRAM Palettes, Palette Fade Tables & LZSS Graphics",
        0x1D: "Hybrid Data + Code: Sprite Assembly Tables ($8000-$AB9F) + Shared Physics/Trig & Late-Boss AI ($ABA0-$CCDF)",
    }
    manifest_c = [
        "/*",
        " * Complete 32-Bank ROM Inventory & REA Ghidra Provenance Table ($00 - $1F)",
        f" * list_segments REA Evidence ID: {EVIDENCE_MANIFEST['list_segments_evidence_id']}",
        f" * Full 1MB ROM SHA-256         : {EVIDENCE_MANIFEST['full_rom_sha256']}",
        " */",
        "",
        '#include "contra3_recomp.h"',
        "",
        "const Contra3BankManifestEntry g_contra3_bank_manifest[32] = {",
    ]
    for b in range(32):
        binfo = EVIDENCE_MANIFEST["banks"][f"{b:02X}"]
        has_code = "true" if b in code_banks else "false"
        fcnt = len(funcs_by_bank[b]) if b in code_banks else 0
        icnt = sum(1 for (ib, _) in ins_at if ib == b)
        role = bank_roles.get(b, "Read-Only Asset Bank: Konami LZSS Compressed Graphics, Tilemaps, or SPC700 Audio/BRR Data")
        manifest_c.append(
            f'    {{ 0x{b:02X}u, 0x{(b << 16) | 0x8000:06X}u, 32768u, {has_code}, {fcnt}u, {icnt}u, "{role}", "{binfo["evidence_id"]}", "{binfo["sha256"]}" }},'
        )
    manifest_c.extend(["};", ""])
    with open(os.path.join(WORKSPACE_DIR, "src", "bank_data_manifest.c"), "w", encoding="utf-8") as f:
        f.write("\n".join(manifest_c))

    # 5. Export translation_stats.json for documentation & verification
    indirect_export = []
    for (b, pc), (mn, mode, val, tbl, tgts) in sorted(indirect_sites.items()):
        if mn == "JML":
            fmt_tgts = [f"${tb2:02X}:{ta2:04X}" if ta2 else "$00:0000(null)" for tb2, ta2 in tgts]
        else:
            fmt_tgts = [f"${b:02X}:{t:04X}" if t else "$0000(null)" for t in tgts]
        indirect_export.append({
            "site": f"${b:02X}:{pc:04X}",
            "bank": b,
            "pc": pc,
            "mnemonic": mn,
            "mode": mode,
            "operand": f"${val:04X}",
            "table": f"${tbl[0]:02X}:{tbl[1]:04X}" if tbl else None,
            "entry_count": len(tgts),
            "targets": fmt_tgts,
        })

    stats = {
        "total_functions": len(func_entries),
        "total_unique_instructions": len(ins_at),
        "total_indirect_sites": len(indirect_sites),
        "code_banks": bank_stats,
        "indirect_sites": indirect_export,
    }
    os.makedirs(os.path.dirname(STATS_OUT_PATH), exist_ok=True)
    with open(STATS_OUT_PATH, "w", encoding="utf-8") as f:
        json.dump(stats, f, indent=2)

    print(f"\nTranslation complete: {len(func_entries)} functions, {len(ins_at)} instructions across {len(code_banks)} code banks.")


if __name__ == "__main__":
    main()
