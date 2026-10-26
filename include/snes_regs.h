#ifndef CONTRA3_SNES_REGS_H
#define CONTRA3_SNES_REGS_H

/*
 * Symbolic SNES Hardware Register Definitions for Contra III Static Recompilation
 * Covers PPU ($2100-$213F), APU ($2140-$217F), WRAM Port ($2180-$2183),
 * Joypad Serial ($4016-$4017), CPU I/O ($4200-$421F), and DMA/HDMA ($4300-$437F).
 */

/* --- PPU Registers ($2100 - $213F) --- */
#define PPU_INIDISP   0x2100u /* Screen Display Register (force blank + brightness) */
#define PPU_OBSEL     0x2101u /* Object Size and Character Size Register */
#define PPU_OAMADDL   0x2102u /* OAM Address Low Byte */
#define PPU_OAMADDH   0x2103u /* OAM Address High Byte + Priority Rotation */
#define PPU_OAMDATA   0x2104u /* OAM Data Write Register */
#define PPU_BGMODE    0x2105u /* BG Mode and Character Size Register */
#define PPU_MOSAIC    0x2106u /* Mosaic Size and BG Enable Register */
#define PPU_BG1SC     0x2107u /* BG1 Tilemap Address and Size */
#define PPU_BG2SC     0x2108u /* BG2 Tilemap Address and Size */
#define PPU_BG3SC     0x2109u /* BG3 Tilemap Address and Size */
#define PPU_BG4SC     0x210Au /* BG4 Tilemap Address and Size */
#define PPU_BG12NBA   0x210Bu /* BG1/BG2 Chr Tile Data Base Address */
#define PPU_BG34NBA   0x210Cu /* BG3/BG4 Chr Tile Data Base Address */
#define PPU_BG1HOFS   0x210Du /* BG1 Horizontal Scroll / M7HOFS */
#define PPU_BG1VOFS   0x210Eu /* BG1 Vertical Scroll / M7VOFS */
#define PPU_BG2HOFS   0x210Fu /* BG2 Horizontal Scroll */
#define PPU_BG2VOFS   0x2110u /* BG2 Vertical Scroll */
#define PPU_BG3HOFS   0x2111u /* BG3 Horizontal Scroll */
#define PPU_BG3VOFS   0x2112u /* BG3 Vertical Scroll */
#define PPU_BG4HOFS   0x2113u /* BG4 Horizontal Scroll */
#define PPU_BG4VOFS   0x2114u /* BG4 Vertical Scroll */
#define PPU_VMAIN     0x2115u /* Video Port Control Register */
#define PPU_VMADDL    0x2116u /* VRAM Address Low Byte */
#define PPU_VMADDH    0x2117u /* VRAM Address High Byte */
#define PPU_VMDATAL   0x2118u /* VRAM Data Write Low Byte */
#define PPU_VMDATAH   0x2119u /* VRAM Data Write High Byte */
#define PPU_M7SEL     0x211Au /* Mode 7 Settings Register */
#define PPU_M7A       0x211Bu /* Mode 7 Matrix Parameter A (also Multiplicand) */
#define PPU_M7B       0x211Cu /* Mode 7 Matrix Parameter B (also Multiplier) */
#define PPU_M7C       0x211Du /* Mode 7 Matrix Parameter C */
#define PPU_M7D       0x211Eu /* Mode 7 Matrix Parameter D */
#define PPU_M7X       0x211Fu /* Mode 7 Center X */
#define PPU_M7Y       0x2120u /* Mode 7 Center Y */
#define PPU_CGADD     0x2121u /* CGRAM Palette Address */
#define PPU_CGDATA    0x2122u /* CGRAM Palette Data Write */
#define PPU_W12SEL    0x2123u /* Window Mask Settings for BG1/BG2 */
#define PPU_W34SEL    0x2124u /* Window Mask Settings for BG3/BG4 */
#define PPU_WOBJSEL   0x2125u /* Window Mask Settings for OBJ/Color Window */
#define PPU_WH0       0x2126u /* Window 1 Left Position */
#define PPU_WH1       0x2127u /* Window 1 Right Position */
#define PPU_WH2       0x2128u /* Window 2 Left Position */
#define PPU_WH3       0x2129u /* Window 2 Right Position */
#define PPU_WBGLOG    0x212Au /* Window Mask Logic for BGs */
#define PPU_WOBJLOG   0x212Bu /* Window Mask Logic for OBJs/Color Window */
#define PPU_TM        0x212Cu /* Main Screen Designation */
#define PPU_TS        0x212Du /* Sub Screen Designation */
#define PPU_TMW       0x212Eu /* Window Mask Designation for Main Screen */
#define PPU_TSW       0x212Fu /* Window Mask Designation for Sub Screen */
#define PPU_CGWSEL    0x2130u /* Color Addition Select */
#define PPU_CGADSUB   0x2131u /* Color Math Designation */
#define PPU_COLDATA   0x2132u /* Fixed Color Data */
#define PPU_SETINI    0x2133u /* Screen Mode/Video Select */
#define PPU_MPYL      0x2134u /* Signed Multiply Result Low */
#define PPU_MPYM      0x2135u /* Signed Multiply Result Mid */
#define PPU_MPYH      0x2136u /* Signed Multiply Result High */
#define PPU_SLHV      0x2137u /* Software Latch for H/V Counter */
#define PPU_OAMDATAREAD 0x2138u /* OAM Data Read */
#define PPU_VMDATALREAD 0x2139u /* VRAM Data Read Low */
#define PPU_VMDATAHREAD 0x213Au /* VRAM Data Read High */
#define PPU_CGDATAREAD  0x213Bu /* CGRAM Data Read */
#define PPU_OPHCT     0x213Cu /* Horizontal Scanline Location */
#define PPU_OPVCT     0x213Du /* Vertical Scanline Location */
#define PPU_STAT77    0x213Eu /* PPU1 Status Flag and Version */
#define PPU_STAT78    0x213Fu /* PPU2 Status Flag and Version */

/* --- APU / SPC700 I/O Ports ($2140 - $2143, mirrored through $217F) --- */
#define APU_PORT0     0x2140u
#define APU_PORT1     0x2141u
#define APU_PORT2     0x2142u
#define APU_PORT3     0x2143u

/* --- WRAM Access Port ($2180 - $2183) --- */
#define WRAM_WMDATA   0x2180u
#define WRAM_WMADDL   0x2181u
#define WRAM_WMADDM   0x2182u
#define WRAM_WMADDH   0x2183u

/* --- Legacy Serial Joypad Ports ($4016 - $4017) --- */
#define JOY_JOYSER0   0x4016u
#define JOY_JOYSER1   0x4017u

/* --- CPU On-Chip I/O Registers ($4200 - $421F) --- */
#define CPUIO_NMITIMEN 0x4200u /* Interrupt Enable & Joypad Auto-Read */
#define CPUIO_WRIO     0x4201u /* Programmable I/O Port (Out) */
#define CPUIO_WRMPYA   0x4202u /* Multiplicand A (8-bit unsigned) */
#define CPUIO_WRMPYB   0x4203u /* Multiplier B (8-bit unsigned; triggers multiply) */
#define CPUIO_WRDIVL   0x4204u /* Dividend C Low Byte */
#define CPUIO_WRDIVH   0x4205u /* Dividend C High Byte */
#define CPUIO_WRDIVB   0x4206u /* Divisor B (8-bit unsigned; triggers divide) */
#define CPUIO_HTIMEL   0x4207u /* H-Count Timer Target Low */
#define CPUIO_HTIMEH   0x4208u /* H-Count Timer Target High */
#define CPUIO_VTIMEL   0x4209u /* V-Count Timer Target Low */
#define CPUIO_VTIMEH   0x420Au /* V-Count Timer Target High */
#define CPUIO_MDMAEN   0x420Bu /* General Purpose DMA Channel Enable */
#define CPUIO_HDMAEN   0x420Cu /* H-Blank DMA (HDMA) Channel Enable */
#define CPUIO_MEMSEL   0x420Du /* FastROM Speed Select */
#define CPUIO_RDNMI    0x4210u /* V-Blank NMI Flag and CPU Version */
#define CPUIO_TIMEUP   0x4211u /* H/V-Timer IRQ Flag */
#define CPUIO_HVBJOY   0x4212u /* H/V-Blank Flag and Auto-Joypad Busy Status */
#define CPUIO_RDIO     0x4213u /* Programmable I/O Port (In) */
#define CPUIO_RDDIVL   0x4214u /* Unsigned Division Quotient Low */
#define CPUIO_RDDIVH   0x4215u /* Unsigned Division Quotient High */
#define CPUIO_RDMPYL   0x4216u /* Unsigned Product / Division Remainder Low */
#define CPUIO_RDMPYH   0x4217u /* Unsigned Product / Division Remainder High */
#define CPUIO_JOY1L    0x4218u /* Joypad 1 Data Low */
#define CPUIO_JOY1H    0x4219u /* Joypad 1 Data High */
#define CPUIO_JOY2L    0x421Au /* Joypad 2 Data Low */
#define CPUIO_JOY2H    0x421Bu /* Joypad 2 Data High */
#define CPUIO_JOY3L    0x421Cu /* Joypad 3 Data Low */
#define CPUIO_JOY3H    0x421Du /* Joypad 3 Data High */
#define CPUIO_JOY4L    0x421Eu /* Joypad 4 Data Low */
#define CPUIO_JOY4H    0x421Fu /* Joypad 4 Data High */

/* --- DMA / HDMA Channel Registers ($4300 - $437F) --- */
#define DMA_DMAP0      0x4300u
#define DMA_BBAD0      0x4301u
#define DMA_A1T0L      0x4302u
#define DMA_A1T0H      0x4303u
#define DMA_A1B0       0x4304u
#define DMA_DAS0L      0x4305u
#define DMA_DAS0H      0x4306u
#define DMA_DASB0      0x4307u
#define DMA_A2A0L      0x4308u
#define DMA_A2A0H      0x4309u
#define DMA_NTRL0      0x430Au

#endif /* CONTRA3_SNES_REGS_H */
