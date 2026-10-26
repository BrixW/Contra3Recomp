# Contra III: The Alien Wars (USA) — SNES 65816 Static Recompilation & Native Desktop Port

Portable C11 static recompilation and standalone Windows/Linux desktop executables for **Contra III: The Alien Wars (USA)** (`1,048,576` bytes, LoROM, SHA-256 `a93ea87fc835c530b5135c5294433d15eef6dbf656144b387e89ac19cf864996`), generated from Ghidra 12.1.4 (`65816:LE:24:snes`) analysis via the REA MCP server.

---

## Project Highlights

- **Complete Whole-ROM Code Coverage**: All **6 executable code banks** (`Bank $00`, `Bank $01`, `Bank $02`, `Bank $03`, `Bank $04`, and `Bank $1D`) are translated function-by-function into portable C11:
  - **2,288 translated functions** (`2,263` reachable from hardware vectors + `25` unreferenced Konami SDK & disabled *Contra Spirits* subroutines in Bank `$00`).
  - **57,007 unique decoded 65816 instructions** (`0` mid-instruction overlaps across the entire 1 MB image).
  - **All 32 ROM banks (`$00`–`$1F`)** verified against Ghidra memory segments with cryptographic SHA-256 digests and immutable REA `evidence_id` provenance.
- **Standalone Playable Desktop Executable (`build/contra3.exe`) + S-PPU1/S-PPU2 Scanline Software Rasterizer (`src/snes_ppu.c`)**:
  - Full scanline-accurate software PPU supporting Modes `0`, `1` (with `BGMODE` bit 3 BG3 HUD top-priority), `2` (offset-per-tile), `3`, `4`, and **Mode 7** (16-bit signed affine matrix + `EXTBG` + mid-frame scanline `V-IRQ` `$00:8BE2` splits for 1P & 2P split-screen overhead stages).
  - 128 OAM sprites (all `OBSEL` size pairs, 9-bit X wrap, H/V flip, lower-OAM-index pixel ownership, 4 priority tiers).
  - Hardware-exact Ricoh 5A22 per-scanline H-Blank DMA (`HDMA` `$420C`, direct & indirect modes `0..7`, including `NTRL = 0x80` 128-line borrow semantics used by the Mode 7 / Mode 1 split Title Screen).
  - Dual windows (`W1`/`W2` + `OR/AND/XOR/XNOR`), Main/Sub screen color math (`CGWSEL` / `CGADSUB` / `COLDATA` add/subtract/half for translucent explosions, fire seas, and circular Mega Bomb shockwaves), Mosaic, and `INIDISP` brightness fades.
  - Native Win32 GDI + `waveOut` (`48 kHz` 16-bit stereo via 4-tap Catmull-Rom cubic upsampling from native `32 kHz` S-DSP) + `QueryPerformanceCounter` `60.0988 Hz` desktop window (`768x672` 3x default scale, aspect-ratio preserving resize, borderless fullscreen, Keyboard + dynamic XInput 1P/2P controllers, quicksave/quickload with full APU state, mute toggle, frame-stepping, turbo, and `--headless` / `--wav` CLI harness running at **~10,000 FPS** with live SPC700 + S-DSP synthesis).
- **Sony S-SMP (`SPC700` + 64-Byte IPL Boot ROM) & 8-Voice `32 kHz` Stereo `S-DSP` Audio Core (`src/snes_apu.c`)**:
  - Zero-dependency portable C11 implementation of all **256 Sony SPC700 opcodes**, the authentic **64-byte IPL Boot ROM (`$FFC0–$FFFF`)**, 64 KB ARAM, `$F0–$FF` I/O registers, and 3 hardware timers (`8 kHz` / `8 kHz` / `64 kHz`).
  - Hardware-accurate 8-voice **Sony S-DSP** (`32,000 Hz` stereo): 4-filter BRR ADPCM block decoding with 15-bit sign-wrap semantics, 512-entry 4-tap Gaussian pitch interpolation, ADSR + 5 GAIN envelope modes, `PMON` pitch modulation, LFSR white noise (`NON`), stereo panning, and `ESA`/`EDL`/`EFB` echo ring buffer + 8-tap FIR filter.
  - Executes Konami's real LZSS-compressed SPC700 sound driver (`ARAM $0200` / `$0896`) and custom `$1AF3` stage sample uploader (`$84` / `$FF` / `$CC` protocol) across all stages and cutscenes.
- **Explicit Dynamic `M` / `X` Register Width Tracking**:
  - Whole-program and intra-procedural `SEP` / `REP` + `PHP` / `PLP` stack analysis (`65/65` `PLP` instructions statically matched to intra-procedural `PHP` saves).
  - Every function header, basic-block label (`L_xxxx:`), and `SEP`/`REP`/`PLP` instruction documents the exact 8-bit/16-bit accumulator (`M`) and index (`X`) state inline in C comments.
- **Abstracted SNES Hardware Interface**:
  - Zero raw MMIO pointer dereferences in translated code; all **1,162 hardware register calls** (`$2100–$213F` PPU, `$2140–$217F` APU, `$2180–$2183` WRAM port, `$4016–$4017` Joypad, `$4200–$421F` CPU I/O, `$4300–$437F` DMA/HDMA) route through project hardware interface macros (`snes_write_ppu`, `snes_read_ppu`, `snes_write_cpu_io`, `snes_read_cpu_io`, `snes_write_apu`, `snes_read_apu`, `snes_write_dma`, `snes_read_dma`, etc.).
- **Indirect Jump Resolution + Interpreter Fallback**:
  - All **190 unique indirect jump/call sites** (`JMP ($xxxx)`, `JMP ($xxxx,X)`, `JSR ($xxxx,X)`, `JML [$xxxx]`) emit statically resolved C `switch` tables for proven ROM jump tables while guarding runtime/dynamic targets with inline `// REQUIRES INTERPRETER FALLBACK` markers and `snes_interp_fallback(ctx, addr24)`. The generated code contains **192** such `switch` statements because the sites at `$00:D714` and `$01:8FF6` live in tail code shared by two functions and are emitted in each.
  - `0` unresolved jumps across the all-stage soak (`tools/soak_all_stages.py`: every stage 1-6, 1P and 2P, including stage clears into the next stage).
- **Hardware OpenGL 3.3 Presentation (`src/win32_gl.c`)**:
  - Default presenter: one streaming `GL_RGBA8` texture + full-screen triangle, GLSL 3.30 **Sharp-Bilinear**, **Nearest** and **CRT** (Gaussian scanlines, aperture-grille mask, soft curvature) filters, integer scaling, optional `WGL_EXT_swap_control` VSync. Links only `opengl32.dll`; GDI `StretchDIBits` remains as automatic fallback (`--gdi`).
  - `--gl-selftest` renders a live Stage 1 frame through every filter and reads the back buffer back; the Nearest 3x output is checked bit-exact against the CPU frame buffer.
- **Input Movie Playback, Recording & Co-Simulation Harness (`src/snes_movie.c`, `tools/`)**:
  - Loads Snes9x `.smv` (v1/v4/v5), BizHawk `.bk2` (built-in zip + DEFLATE decoder, LogKey-driven button mapping) or a bare `Input Log.txt`, and the native `.c3m` text movie written by `--record` / `F6`.
  - `--trace` writes per-frame WRAM probes; `tools/emu_trace.lua` exports the identical CSV from BizHawk / Snes9x-rr, `--lag-from` skips the emulator's lag frames on playback, and `tools/compare_trace.py` reports the first divergence.

---

## Directory Layout

| Path | Description |
| :--- | :--- |
| `include/snes_regs.h` | Symbolic SNES hardware register constants (`PPU_INIDISP`, `CPUIO_NMITIMEN`, `APU_PORT0`, `DMA_DMAP0`, etc.) |
| `include/snes_hw.h` | Abstracted hardware interface macros (`snes_write_ppu`, `snes_read_cpu_io`, etc.) and PPU/CPUIO/APU/DMA state structs |
| `include/snes_ppu.h` | Public S-PPU1/S-PPU2 scanline rasterizer, joypad bitmasks (`SNES_BTN_*`), `snes_step_frame`, and `.bmp` writer API |
| `include/snes_apu.h` | Public Sony S-SMP (`SPC700` + IPL) & `32 kHz` 8-voice `S-DSP` audio core (`SnesApu`, `snes_apu_*`) and `.wav` writer API |
| `include/cpu_65816.h` | Portable `SnesCpuContext`, status flag pack/unpack, binary + BCD ALU helpers (`snes_adc8/16`, `snes_sbc8/16`), and LoROM addressing |
| `include/snes_movie.h` | Input movie API: `.smv` / `.bk2` / `Input Log.txt` / `.c3m` loading, `.c3m` recording, raw DEFLATE decoder |
| `include/contra3_recomp.h` | Master prototypes for all `2,288` translated C functions, 32-bank manifest declarations, and `contra3_dispatch_call` |
| `src/snes_runtime.c` | Portable SNES LoROM memory bus, hardware 8x8 multiply & 16/8 divide, General DMA (`MDMAEN`), `MVN`/`MVP`, and fallback dispatcher |
| `src/snes_ppu.c` | Full SNES S-PPU1/S-PPU2 scanline software rasterizer (Modes `0–7`, OAM, HDMA, dual windows, color math, V-IRQ splits) |
| `src/snes_apu.c` | Zero-dependency C11 Sony `SPC700` (all `256` opcodes + 64-byte IPL ROM + 3 timers) & 8-voice BRR/Gaussian/Echo `S-DSP` |
| `src/snes_rom.c` | ROM discovery ($CONTRA3_ROM, exe-relative folders) and validation (title, cartridge checksum, copier header) |
| `src/snes_savestate.c` | Versioned save states: magic, format version, struct-layout fingerprint, ROM + payload checksums, staged load, atomic write |
| `src/snes_movie.c` | Portable C11 movie decoders (Snes9x SMV, BizHawk BK2 zip/DEFLATE, native C3M) and C3M recorder |
| `src/win32_main.c` | Win32 interactive desktop runner (OpenGL/GDI + `48 kHz` Catmull-Rom upsampled `waveOut` stereo + dynamic `XInput`), movie playback/recording, and the `--headless` / `--trace` / `--verify` CLI harness |
| `src/linux_main.c` | Linux GTK4 launcher and SDL2 desktop runner (ROM chooser, saved keyboard/controller mappings, window, stereo audio, resize and fullscreen support) |
| `src/win32_gl.c` | OpenGL 3.3 Core presenter (`wglCreateContextAttribsARB`, GLSL Sharp-Bilinear / Nearest / CRT filters, integer scaling, VSync) |
| `src/bank_00.c` | Statically recompiled **Bank `$00`** (`514` functions, `13,007` instructions — Core Engine, Boot, NMI/IRQ, LZSS, SPC700, Dispatchers) |
| `src/bank_01.c` | Statically recompiled **Bank `$01`** (`484` functions, `12,457` instructions — Player Controller, Weapons, Projectiles, Stage Enemies) |
| `src/bank_02.c` | Statically recompiled **Bank `$02`** (`620` functions, `13,757` instructions — Stage Bosses, Mode 7 Overhead Stages, Multi-Part Actors) |
| `src/bank_03.c` | Statically recompiled **Bank `$03`** (`62` functions, `3,625` instructions — Mode 7 Overhead Mechanics; `$8000–$E17F` is Mode 7 data) |
| `src/bank_04.c` | Statically recompiled **Bank `$04`** (`453` functions, `10,542` instructions — Cutscenes, Intro/Ending, Stage Events, Mid/Final Bosses) |
| `src/bank_1d.c` | Statically recompiled **Bank `$1D`** (`155` functions, `3,619` instructions — Shared 16-bit Trig/Physics Helpers & Late-Stage Boss AI) |
| `src/bank_data_manifest.c` | Complete 32-bank (`$00`–`$1F`) metadata table with REA Ghidra `evidence_id`s and SHA-256 digests |
| `src/dispatch.c` | Global 24-bit entry address router (`contra3_dispatch_call`) covering all `2,288` compiled functions |
| `tools/translate_rom.py` | Reproducible 65816 CFG + dynamic `M`/`X` analyzer and C11 code generator |
| `tools/emu_trace.lua` | BizHawk / Snes9x-rr Lua exporter writing the reference trace CSV (same columns as `--trace`, plus emulator lag flags) |
| `tools/compare_trace.py` | Aligns a reference emulator trace with a `--trace` CSV and reports the first divergence |
| `tools/soak_all_stages.py` | All-stage robustness soak: warps to stages 1-6 (1P/2P), drives a deterministic input bot, asserts no hangs / unresolved jumps |
| `tests/smoke_test.c` | Native execution verification suite (`RESET`, `KONAMI` SRAM, `PRNG`, `Math_Mul16x16`, `$1D:ABA0`, `NMI`, `snes_ppu`, `snes_apu`, movie decoders) |
| `ARCHITECTURE.md` | Deep technical breakdown of Contra III's engine, memory map, interrupt architecture, LZSS, PPU rasterizer, and SPC700/S-DSP pipeline |
| `BANK_MAP.md` | Full 32-bank (`$00`–`$1F`) ROM map and REA Ghidra provenance ledger |
| `INDIRECT_JUMPS.md` | Complete catalog of indirect jump/call sites and jump tables |
| `HANDOFF.md` | Engineering handoff guide, visual & audio verification summary, and optional future enhancement roadmap |

---

## Building and Running

### Option 1: PowerShell Build Script (`w64devkit` GCC)
```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
```
Produces:
- `build\libcontra3_recomp.a` — Portable static C11 game + runtime + PPU + APU library
- `build\contra3.exe` — Interactive Win32 desktop executable (OpenGL 3.3 / GDI, `60.0988 Hz` video + `48 kHz` cubic-upsampled stereo audio) & headless CLI
- `build\contra3_smoke_test.exe` — Automated verification test suite (`11/11` checks)

### Option 2: CMake + Ninja
```powershell
cmake -B build-cmake -G Ninja -DCMAKE_C_COMPILER="Z:/REA/tools/w64devkit/bin/gcc.exe"
cmake --build build-cmake
ctest --test-dir build-cmake --output-on-failure
```

### Option 3: Linux (CMake + SDL2)
Install a C11 compiler, CMake, pkg-config, SDL2 and GTK4 development files (for example, `libsdl2-dev` and `libgtk-4-dev` on Debian/Ubuntu), then build:
```sh
cmake -S . -B build-linux -DCMAKE_BUILD_TYPE=Release
cmake --build build-linux -j
```
This produces `build-linux/contra3`. Run it with no arguments to open the launcher and choose a ROM. For each keyboard or controller binding, click **Set** and press the desired input, or click **Clear** to remove it. Settings are saved in your user config directory under `contra3/launcher.ini`. You can also launch directly with `./build-linux/contra3 "/path/to/Contra III - The Alien Wars (USA).sfc"`, or set `CONTRA3_ROM`; pass `--launcher` to open the launcher while preselecting a ROM path. SDL2 provides the resizable desktop window, audio output, and accelerated presentation with a software fallback. `Esc` quits and `F11` toggles fullscreen. Up to two SDL GameController compatible pads are detected; the D-pad and left stick handle movement.

### Launching the Game (`build\contra3.exe`)
```powershell
.\build\contra3.exe "Z:\REA\ROM\Contra III - The Alien Wars (USA).sfc"
```
**Finding the ROM.** `contra3.exe`, the tests and `tools/translate_rom.py` look in this order:
1. A path argument (or a ROM dragged onto `contra3.exe`)
2. `$CONTRA3_ROM`
3. `Contra III - The Alien Wars (USA).sfc` next to the executable, in its parent folder, or in a sibling `ROM` folder (`..\ROM`, `..\..\ROM`)

If none is found when launching the desktop app, it opens a file picker. Images are validated by internal title and cartridge checksum, and a 512-byte copier header is stripped automatically. `build.ps1` uses `$env:CONTRA3_CC` (path to `gcc.exe`) if set, otherwise `..\tools\w64devkit`, otherwise `gcc` on `PATH`.

### Interactive Controls (Keyboard + XInput Gamepads 1 & 2)

| Action | SNES Button | Keyboard (Primary / Alt) | Xbox / XInput Controller |
| :--- | :---: | :--- | :--- |
| Move / Aim | `D-Pad` | `Arrow Keys` | `D-Pad` or `Left Stick` |
| Jump | `B` | `Z` or `J` | `A` |
| Shoot | `Y` | `X` or `K` | `X` |
| Switch Weapon | `X` | `A` or `U` | `Y` |
| Mega Bomb | `A` | `S` or `I` | `B` |
| Lock Position / Aim | `L` | `Q` or `L` | `LB` or `LT` |
| Dual-Wield Fire | `R` | `E` or `O` | `RB` or `RT` |
| Start / Pause | `Start` | `Enter` | `Start` |
| Select | `Select` | `Right Shift` or `Backspace` | `Back` |

All settings live in `contra3.ini` next to `contra3.exe`. It's created with the defaults on first run and saved on exit when anything changed; command-line options override it. Sections:
- `[p1]` / `[p2]`: up to four keys per button. **Player 2** defaults to the numpad (`8/5/4/6` D-pad, `1` jump, `2` shoot, `3` switch, `0` bomb, `7`/`9` L/R, `+` Start, `-` Select). Pressing opposite directions together cancels them, as on a real D-pad.
- **In-window rebinding**: `F2` (Player 1) or `Shift+F2` (Player 2) pauses the game and asks for each button in turn in the title bar. Esc keeps the current key and `F2` cancels; the result is saved straight away. A key taken from another button is removed from it.
- `[gamepad]`: remaps XInput `a b x y lb rb lt rt back start` to any SNES button (or `none`) for both pads, plus the stick `deadzone`. The D-pad and left stick always steer.
- `[options]`: the in-game OPTION MODE values (PLAYER lives 3/5/7, LEVEL easy/normal/hard, SOUND stereo/mono). They're saved when you quit and restored at power-on, so they survive restarts like the console's warm-reset memory does within a session. Movie playback and recording always start from the game's defaults, so movies stay reproducible.
- `[video]`, `[game]`, `[extras]`: filter, integer scaling, VSync, lag model, and the toggles below.

### Application Hotkeys
- `Tab` (hold) / `F1` (toggle): Uncapped Turbo Fast-Forward
- `F3`: Toggle authentic slowdown (the CPU-time lag model, see below)
- `F4`: Toggle the restored **STAGE SELECT** screen (press START on the title screen; UP/DOWN choose 1–6). Also `--stage-select`, and `--30-lives` for 30 starting lives. These are the effects of *Contra Spirits* (Japan) codes whose detectors were disconnected in the US release.
- `Shift+F4`: Toggle infinite lives (`--infinite-lives-cheat`): during play, every player in the game keeps at least 2 lives.
- `--start-stage <1-6>` (or `start_stage` in `[extras]`): the first game started from each title visit begins at that stage. Combined with stage select, it sets where the cursor starts.
- `F6`: Start / stop recording an input movie (`contra3_rec_XXXX.c3m`; recording power-cycles the console so the movie replays from power-on)
- `F9`: Cycle OpenGL filter (Sharp-Bilinear -> Nearest -> CRT)
- `F10`: Toggle integer scaling (OpenGL)
- `P` / `Pause`: Pause emulation (`Space` advances 1 single frame while paused)
- `M`: Mute / Unmute live stereo audio
- `F5` / `F7`: Quick Save / Quick Load state (`contra3_quicksave.state`, full CPU + PPU + SPC700/S-DSP state). States are versioned (`src/snes_savestate.c`): a file from an incompatible build, another ROM, or a truncated/corrupt write is refused with a reason in the title bar and never touches the running game.
- `F8`: Activate 30-Lives cheat (`0x001E` -> `$1E3C` & `$1F8A`)
- `F11` / `Alt+Enter`: Toggle borderless fullscreen
- `F12`: Save `.bmp` screenshot (`contra3_shot_XXXX.bmp`)
- `Ctrl+R`: Soft Reset (`WarmReset_Entry` `$00:80A3`)
- `Esc`: Exit

### Presentation Options
```powershell
.\build\contra3.exe --filter crt --integer --vsync   # OpenGL 3.3 (default presenter)
.\build\contra3.exe --gdi                            # force the GDI StretchDIBits presenter
.\build\contra3.exe --gl-selftest build\gltest       # render + read back every GL filter, verify Nearest 3x bit-exact
```

### Headless Scripted Verification, Frame Dumping & `.wav` Capture
```powershell
.\build\contra3.exe --headless 2200 `
  --press "620-625:START" `
  --dump "750:title.bmp" `
  --press "760-765:START" `
  --press "1200-1800:RIGHT" `
  --press "1250-1800:SHOOT" `
  --dump "1350:stage1.bmp" `
  --wav "contra3_stage1.wav"
```
Run `contra3.exe --help` for the full list. Additional harness options: `--press2` (Player 2 script), `--poke <frame>:<addr>=<hex16>` (WRAM write, e.g. stage warp `--poke 800:0086=5`), `--infinite-lives`, `--lag-model [cycles]` (authentic slowdown), `--strict` (abort with a register dump on the first unresolved indirect jump), `--record <file.c3m>`, `--trace <file.csv>`, `--verify` (state/stage progress log + hang detector, `--hang-frames`), and `--movie` / `--movie-logic` / `--movie-offset` / `--lag-from` (see below), and `--cpu-budget <cycles>` (timing-only CPU cycles per frame, default 45000). Exit status: `0` clean, `2` unresolved indirect jump, `3` `--strict` abort, `4` hang. Every unresolved jump is also logged immediately to stderr and `contra3_fallback.log` with its frame number and dispatch site.

### Bit-Exact Equivalence Against a Reference Emulator (`tools/refemu/`)
`tools/refemu/refemu_trace.c` drives any libretro SNES core headlessly (built here from upstream snes9x). It feeds the same input movie, applies the same pokes and infinite-lives rule, and writes the same trace CSV as `contra3.exe`. `tools/refemu/compare_all.py` runs the full suite: stages 1–6 in 1P and 2P plus the attract demo.
```powershell
# One-time: build the reference core (outside the project) and the harness
git clone --depth 1 https://github.com/libretro/snes9x ..\refemu\snes9x
mingw32-make -C ..\refemu\snes9x\libretro platform=win CC=gcc CXX=g++
gcc -std=c11 -O2 -Iinclude -I..\refemu\snes9x\libretro tools\refemu\refemu_trace.c build\libcontra3_recomp.a -o build\refemu_trace.exe

python tools\refemu\compare_all.py --core ..\refemu\snes9x\libretro\snes9x_libretro.dll
```
**Result: 13/13 scenarios are bit-exact.** That's 156,000 logic frames compared over 13 probes every frame: game state and sub-state, stage, both players' positions and lives, the camera, and the RNG. Both sides consume the movie per *logic* frame (`--movie-logic`), so emulator lag frames can't shift the input.

### Input Movies & Emulator Co-Simulation
```powershell
# Replay any power-on movie (Snes9x .smv, BizHawk .bk2 / Input Log.txt, native .c3m), interactively or headless
.\build\contra3.exe --movie run.bk2
.\build\contra3.exe --headless 0 --movie run.bk2 --verify --trace ours.csv

# Bit-exact co-simulation against a reference emulator:
#  1. In BizHawk (or Snes9x-rr) play run.bk2 from power-on with tools\emu_trace.lua loaded -> emu_trace.csv
#  2. Replay in the recompilation, skipping the emulator's lag frames, and compare
.\build\contra3.exe --headless 0 --movie run.bk2 --lag-from emu_trace.csv --trace ours.csv
python .\tools\compare_trace.py emu_trace.csv ours.csv

# All-stage soak (stages 1-6, 1P + 2P, deterministic input bot, ~2 hours of gameplay in about a minute)
python .\tools\soak_all_stages.py --frames 36000
```
### Authentic Slowdown (`--lag-model`, `F3`)
Every generated instruction charges its WDC 65C816 cycle cost (`SNES_CYCLES`): M/X width, the direct-page penalty, taken branches, MVN/MVP and DMA are all included. With `--lag-model`, an NMI whose game logic exceeds the frame budget (45,642 CPU cycles by default, about one NTSC frame at SlowROM speed) makes the following NMIs hit Konami's `$003C` busy guard. Those are lag frames: no game logic runs and no input is read, just as on hardware. `--verify` prints the per-frame CPU load (average 25–33k cycles in normal Stage 1/2/4 play). The `nmi_cycles`, `apu_polls` and `lag` trace columns show it frame by frame.

The SPC700 runs on the same timeline as the CPU: it catches up to the 65816's current time before every `$2140–$2143` access and at every frame boundary. So audio-upload handshakes and synchronous decompression take real time. With `--lag-model`, power-on holds the screen for about 2 s, the Stage 1 intro card for about 4 s, and the stage-clear tally for about 3.7 s before play resumes. Without it, video skips over that time but the audio still lives through it, so a capture is about 11 s longer over the first 100 s. The 45,642-cycle budget hasn't been calibrated against hardware, so the model is off by default. Calibrate it with an emulator trace: `contra3.exe --headless 0 --movie run.bk2 --lag-model --trace ours.csv`, then `python tools\compare_trace.py emu_trace.csv ours.csv --lag-report`.

The recompilation without `--lag-model` services every NMI, so it never lags. Real hardware drops frames when Konami's NMI logic overruns (the `$003C` re-entrancy guard), and the joypad input on those frames is ignored. Skipping the emulator's lag frames (`--lag-from`) therefore keeps the input aligned with the game's logic frames.

---

## Roadmap & Status of Milestones (Steps 1–4)

1. **Step 1: Interactive Window & Software PPU Renderer (`Complete`)**:
   - Software S-PPU1/S-PPU2 scanline rasterizer (`src/snes_ppu.c`) supporting Modes 0–7, 128 sprites, HDMA, dual windows, color math, and Mode 7 V-IRQ splits.
   - Interactive Win32 GDI presentation window (`src/win32_main.c`) at `60.0988 Hz` with keyboard and XInput gamepad controls.
2. **Step 2: S-SMP (SPC700) & S-DSP Audio Core (`Complete & Audibly Verified`)**:
   - Portable C11 Sony SPC700 interpreter (all 256 opcodes + 64-byte IPL ROM + 3 timers) and 8-voice `32 kHz` stereo S-DSP (`src/snes_apu.c`).
   - Win32 `waveOut` audio output upsampled to `48 kHz` via 4-tap Catmull-Rom cubic interpolation with zero underruns.
3. **Step 3: Mode 7 Overhead Polish & Hardware GPU Presentation (`Complete`)**:
   - Software Mode 7 affine transform, 13-bit clip, `EXTBG`, and mid-frame `V-IRQ` `$00:8BE2` 1P/2P splits (`src/snes_ppu.c`).
   - OpenGL 3.3 Core presenter with Sharp-Bilinear / Nearest / CRT shaders, integer scaling and VSync (`src/win32_gl.c`); verified on an NVIDIA RTX 5070 Ti (driver 617.14), with the Nearest 3x readback matching the CPU frame buffer exactly.
   - 2-player split screen checked on Stages 2 and 5: each half uses its own Mode 7 matrix, with the HUD band between them. A half that goes black is the game's own HDMA `INIDISP = $00` fade for a player who is dead or out of lives. It lines up exactly with that player's lives counter, so it is not a renderer fault.
4. **Step 4: Input Movie Co-Simulation Verification (`Complete`)**:
   - **Done**: `.smv` / `.bk2` / `Input Log.txt` / `.c3m` playback (`--movie`), recording (`--record`, `F6`), per-frame WRAM traces (`--trace`), progress/hang verification (`--verify`), the emulator lag-frame skip (`--lag-from`), the BizHawk/Snes9x trace exporter and the trace comparator. Record -> replay is deterministic: the traces match on every frame.
   - **Done**: All-stage soak across Stages 1-6, 1P and 2P, including stage clears (`tools/soak_all_stages.py`): `0` unresolved jumps, `0` hangs.
   - **Done**: Bit-exact against snes9x in all 13 suite scenarios (`tools/refemu/compare_all.py`). Getting there needed the preemptible main thread (background SPC700 uploads now span frames while NMIs keep running, as on hardware) and calibrating the CPU budget to 45,000 cycles per frame. Before those two changes, Stage 6 2P diverged when a mid-stage upload released the camera lock about 125 frames too early.
