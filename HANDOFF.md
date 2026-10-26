# Contra III: The Alien Wars (USA) — Engineering Handoff Guide

## 1. Current Completion Status

| Deliverable | Status | Details |
| :--- | :---: | :--- |
| **REA Ghidra 12.1.4 Analysis** | Complete | All 32 LoROM banks (`$00`–`$1F`, `1,048,576` bytes) verified via `rea` MCP (`list_segments`, `list_procedures`, `procedure_assembly`, `read_bytes`) with SHA-256 + `evidence_id` provenance |
| **Executable Code Bank Translation** | Complete | **100%** of executable banks translated to C11: `src/bank_00.c` (`514` funcs), `src/bank_01.c` (`484` funcs), `src/bank_02.c` (`620` funcs), `src/bank_03.c` (`62` funcs), `src/bank_04.c` (`453` funcs), `src/bank_1d.c` (`155` funcs) — **2,288 functions / 57,007 instructions** |
| **Dynamic `M` / `X` Width Comments** | Complete | Entry widths, basic-block widths, `SEP`/`REP` transitions, and all `65` intra-procedural `PHP`/`PLP` pairs analyzed and documented inline in C comments |
| **Hardware Interface Macros** | Complete | All PPU (`$2100–$213F`), APU (`$2140–$217F`), WRAM (`$2180–$2183`), Joypad (`$4016–$4017`), CPU I/O (`$4200–$421F`), and DMA (`$4300–$437F`) accesses abstracted via `include/snes_hw.h` |
| **Indirect Jump Fallback Guards** | Complete | All `190` unique indirect sites emit resolved `switch` tables + `default: // REQUIRES INTERPRETER FALLBACK` calling `snes_interp_fallback` (`192` emitted switches; `$00:D714` and `$01:8FF6` are duplicated into two functions that share their tail code) |
| **Step 1: Interactive Desktop Window & S-PPU Renderer** | **Complete** | Full scanline software PPU (`src/snes_ppu.c`) + Win32 frontend (`src/win32_main.c`) supporting Modes 0–7, 128 sprites, HDMA (`$420C`), dual windows, color math, and Mode 7 V-IRQ splits |
| **Step 2: S-SMP (SPC700 + IPL) & S-DSP Audio Core** | **Complete** | 8-voice `32 kHz` stereo synthesis (`src/snes_apu.c`) + `48 kHz` Catmull-Rom cubic upsampled `waveOut` live streaming |
| **Step 3: Mode 7 Overhead Polish & Hardware GPU Presentation** | **Complete** | OpenGL 3.3 Core presenter with Sharp-Bilinear / Nearest / CRT shaders, integer scaling and VSync (`src/win32_gl.c`); 2P split-screen behaviour on Stages 2 & 5 verified |
| **Step 4: Input Movie Co-Simulation Verification** | **Harness complete, reference comparison pending** | `.smv` / `.bk2` / `Input Log.txt` / `.c3m` playback and recording, per-frame traces, lag-frame alignment, emulator trace exporter + comparator, all-stage soak. Bit-exact comparison against a reference emulator still needs BizHawk / Snes9x-rr and a power-on movie, neither of which is in this workspace |

---

## 2. Visual, Audio & Functional Verification Summary

Verified end-to-end with zero compiler warnings (`gcc -std=c11 -O2 -Wall -Wextra -Werror`), `9/9` smoke tests, visual `.bmp` frame inspection, and audible + waveform checks:
1. **Cold Boot & Konami Logo (`Frames 1–300`)**: `"KONAMI"` SRAM initialization, 64-byte Sony IPL upload of Konami's SPC700 driver (`ARAM $0200` -> `$0896`), laser-beam HDMA window scan, Konami logo fade-in/out and chime.
2. **Intro Cinematic & Title Screen**: `"A.D. 2636"` color-math text over the parallax skyline, orchestral intro theme, and the Mode 7 / Mode 1 HDMA-split title screen.
3. **All six stages, 1P and 2P**: `tools/soak_all_stages.py --frames 54000 --seeds 3` ran 36 runs (stages 1–6 × 1P/2P × 3 bot seeds, 1,944,000 frames ≈ 9 hours of gameplay, ~8,000 FPS): **36/36 clean, `0` unresolved jumps, `0` hangs**. The bot cleared Stage 1 into Stage 2 in all 6 Stage 1 runs and Stage 4 into Stage 5 in 4 of 6 runs, so stage-clear tallies, game-over/continue states (`$0B`/`$0C`) and next-stage loads are exercised as well. No run reached the Stage 6 final boss or the ending (state `$0E`); that needs a real playthrough movie (§4A).
4. **2P Mode 7 split screen (Stages 2 & 5)**: independent upper/lower affine matrices (`$00:8C15` / `$00:8C8C` at scanline 112) with the Mode 1 HUD band in between. Per-scanline register capture showed that a black half is the game's HDMA `INIDISP = $00` fade for a dead or out-of-lives player; it correlates exactly with `$1F8A` / `$1FCA`.
5. **OpenGL presenter**: `contra3.exe --gl-selftest <prefix>` on an NVIDIA RTX 5070 Ti (OpenGL 3.3.0, driver 617.14): the Nearest 3x readback has 0 mismatches against the CPU frame buffer, and the Sharp-Bilinear and CRT output was inspected visually. The interactive window runs at a locked 60.1 FPS with the CRT filter.
6. **Movie round trip**: a 3,000-frame scripted run recorded with `--record` and replayed with `--movie` produces identical traces on every frame. A synthetic emulator trace with 81 injected lag frames re-aligns exactly through `--lag-from`, and `compare_trace.py` flags the run immediately when lag skipping is left off.

---

## 3. What Was Delivered for Steps 3 and 4

### Step 3: Mode 7 Overhead Polish & Hardware GPU Presentation
- **`src/win32_gl.c` / `src/win32_gl.h`**: OpenGL 3.3 Core via `wglCreateContextAttribsARB` (falls back to a compatibility context of 3.3+, otherwise to GDI). One streaming `GL_RGBA8` texture uploaded as `GL_BGRA` / `UNSIGNED_INT_8_8_8_8_REV` and an attribute-less full-screen triangle. GLSL filters: Sharp-Bilinear (default), Nearest, CRT. Integer scaling (`--integer`, `F10`), filter cycling (`--filter`, `F9`), VSync (`--vsync`; swap-paced only on a 59–61 Hz display, otherwise the QPC 60.0988 Hz loop keeps pacing).
- **`--gdi`** forces the original `StretchDIBits` path, and **`--gl-selftest`** is a repeatable GPU check.
- **2P overhead**: verified on Stages 2 and 5, with the findings in `ARCHITECTURE.md` §5.5. No renderer change was needed. The black half seen during 2P play is correct game behaviour.
- **Fixed along the way**: `snes_write_bmp` (`src/snes_ppu.c`) used a fixed 256-pixel stack row buffer despite taking any width, so wider images overflowed the stack. It now allocates the row from the real width.

### Step 4: Input Movie Co-Simulation
- **`src/snes_movie.c` / `include/snes_movie.h`**: Snes9x `.smv` v1/v4/v5, BizHawk `.bk2` (zip + built-in DEFLATE, LogKey mapping, savestate-start rejection), bare `Input Log.txt`, native `.c3m` reader and writer.
- **`contra3.exe`**: `--movie`, `--movie-offset`, `--lag-from`, `--record`, `--trace`, `--verify`, `--hang-frames`, `--press2`, `--poke`, `--infinite-lives`. Interactive movie playback hands control back to the player when the movie ends, and `F6` records a power-on `.c3m`.
- **`tools/emu_trace.lua`**: BizHawk / Snes9x-rr reference trace exporter (same CSV columns plus `lag`).
- **`tools/compare_trace.py`**: drops reference lag rows, aligns 1:1 (`--offset`), and reports per-field mismatch counts plus context around the first divergence.
- **`tools/soak_all_stages.py`**: all-stage, 1P/2P soak with a deterministic input bot.

### Corrected WRAM facts (found while building the probes)
- **`$0086` is the stage number** (1–6). It's driven by the stage-select menu at `$00:A7C8`, and writing it during the mode-select fade warps to that stage.
- **`$1E02` is not the stage**: it holds the title-menu selection (1P / 2P-A / 2P-B / Option).

---

## 4. Remaining Work

### A. Bit-exact equivalence against a reference emulator (completes Step 4)
1. Install BizHawk 2.x (bsnes core) or Snes9x-rr and get a **power-on** Contra III (USA) movie, e.g. a TASVideos `.bk2` / `.smv` publication.
2. Play it with `tools/emu_trace.lua` loaded to produce `emu_trace.csv`.
3. `contra3.exe --headless 0 --movie run.bk2 --lag-from emu_trace.csv --verify --trace ours.csv`
4. `python tools/compare_trace.py emu_trace.csv ours.csv`. The first divergence points straight at the frame (and probe) where recompiled logic or timing departs from hardware. Expect boot-time differences around the SPC700 upload handshake first; `--movie-offset` and `--offset` are there to absorb a constant shift.

### B. Optional polish
- **Persistent cartridge config**: save/load `ctx.wram[0x1E00..0x1FFF]` to `contra3_nvram.bin`.
- **Restore *Contra Spirits* cheats**: expose Sound Test (`bank00_func_b2f3`), 30 Lives (`bank00_func_b346`) and Stage Select (`bank00_func_b39f`).
- **Vulkan presenter**: not needed for parity; the OpenGL path covers GPU scaling, VSync and shaders.

---

## 5. Regenerating, Rebuilding & Verifying

```powershell
# Optional: regenerate src/bank_*.c and src/dispatch.c from the ROM
python .\tools\translate_rom.py

# Compile library, desktop executable (build\contra3.exe), run the 9 smoke tests
powershell -ExecutionPolicy Bypass -File .\build.ps1

# GPU presenter self-test and all-stage soak
.\build\contra3.exe --gl-selftest build\gltest
python .\tools\soak_all_stages.py --frames 36000
```
