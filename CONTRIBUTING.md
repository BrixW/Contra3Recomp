# Contributing to Contra3Recomp

Thanks for wanting to help. This is a hobby project: fixes, ports, filters, tools and documentation are all welcome.

## Branches

| Branch | Purpose |
| :--- | :--- |
| **`AlienWars`** | **The community branch. Open your pull requests here.** Anyone can contribute to it. |
| `HardCorps` | The maintainer's working branch for ongoing iteration. |
| `main` | The stable line. Work from `AlienWars` and `HardCorps` is merged into it once verified. |

```sh
git clone https://github.com/BrixW/Contra3Recomp.git
cd Contra3Recomp
git checkout AlienWars
git checkout -b my-change
# ... work, commit ...
git push -u origin my-change     # (or push to your fork) and open a PR into AlienWars
```

## The ROM

- **Never commit a ROM, a save state or anything extracted from the ROM** (graphics, music, raw data). `.gitignore` already blocks `*.sfc`, `*.smc`, `*.state` and `contra3.ini`.
- You need your own copy of *Contra III: The Alien Wars (USA)* (SHA-256 `a93ea87fc835c530b5135c5294433d15eef6dbf656144b387e89ac19cf864996`). Point the tools at it with `CONTRA3_ROM`, a path argument, or by placing it next to the executable or in a sibling `ROM` folder.
- The generated `src/bank_*.c` and `src/dispatch.c` are committed on purpose. Change the generator (`tools/translate_rom.py`), not the generated files, and regenerate.

## Building

- **Linux / macOS**: install SDL2, CMake and Ninja, then `cmake -S . -B build -G Ninja && cmake --build build` (see the README for the per-distro packages).
- **Windows**: `powershell -ExecutionPolicy Bypass -File .\build.ps1` (w64devkit GCC), or CMake with MinGW. Add `-DCONTRA3_SDL=ON` to also build the SDL2 frontend.

The code is C11 and builds with `-Wall -Wextra -Werror`, so keep it warning-free on GCC and Clang.

## Before opening a pull request

1. **Unit tests** (no ROM needed): `ctest --test-dir build --output-on-failure`. CI runs these on Linux, macOS and Windows for every push and PR.
2. **If you touched the runtime, PPU, APU, main thread or generator**, run the ROM tests too:
   - `build.ps1`, or `ctest` configured with `-DCONTRA3_ROM=/path/to/rom.sfc`: smoke test + 5 golden replays. A golden hash change means the emulation changed. If that's intended, say why in the PR and update with `contra3_replay_test --update`.
   - `python tools/check_regen.py`: the committed generated code must match the generator's output.
   - Bigger timing changes: `python tools/refemu/compare_all.py --core <snes9x_libretro>` must stay 13/13 bit-exact (see `ARCHITECTURE.md` §7.7).
3. **Frontend changes**: say which platforms you tried. Frontend-only features (menus, filters, input) belong in the portable files (`c3_config.c`, `c3_overlay.c`, `gl_present.c`, ...) so both the Win32 and SDL2 runners get them.

## Style

- Match the surrounding code: 4-space indentation, `snake_case`, `uint8_t`/`uint16_t` for SNES-sized values, unsigned literals (`0u`) where the code does.
- Name WRAM addresses and explain any game fact you rely on in a comment. `ARCHITECTURE.md` §3 is the memory map, so add new findings there.
- Keep commits focused, with a message that says what changed and why.

## Good first contributions

- New GLSL filters in `src/gl_present.c` (and a menu entry for them).
- More gamepad niceties in `src/sdl_main.c` (hot-plug player assignment, per-pad mapping).
- Porting the headless verification CLI (`--headless`, `--trace`, `--verify`) from `win32_main.c` to a portable runner.
- Distro packaging (Flatpak, AUR) that asks the user for their ROM.
- Documenting more of the game's WRAM.

By contributing, you agree that your contributions are released under the project's [MIT License](LICENSE).
