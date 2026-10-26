#!/usr/bin/env python3
"""
Contra III: The Alien Wars (USA) - Reference-emulator equivalence suite

Runs every scenario (stages 1-6 in 1P and 2P with the soak bot, plus the attract demo with
no input) on both a libretro reference core (tools/refemu/refemu_trace.c) and the static
recompilation, then compares the per-logic-frame traces with tools/compare_trace.py.

Both sides consume the input movie per *logic* frame (contra3.exe --movie-logic), so
emulator lag frames cannot shift the input; pokes (stage warp) and the --infinite-lives
rule are keyed to logic frames on both sides as well.

Usage:
  python tools/refemu/compare_all.py --core <snes9x_libretro.dll> [--harness <refemu_trace.exe>]
         [--frames 12000] [--budgets 45000,45642] [--out build/refemu]
"""

import argparse
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import soak_all_stages as soak  # noqa: E402  (movie generator)

EXE = os.path.join(ROOT, "build", "contra3.exe")
COMPARE = os.path.join(ROOT, "tools", "compare_trace.py")


def default_rom():
    return os.environ.get("CONTRA3_ROM", os.path.join(os.path.dirname(ROOT), "ROM", "Contra III - The Alien Wars (USA).sfc"))


def run(cmd):
    return subprocess.run(cmd, capture_output=True, text=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--core", required=True, help="libretro SNES core DLL (e.g. snes9x_libretro.dll)")
    ap.add_argument("--harness", default=os.path.join(ROOT, "build", "refemu_trace.exe"))
    ap.add_argument("--rom", default=default_rom())
    ap.add_argument("--frames", type=int, default=12000, help="logic frames per scenario")
    ap.add_argument("--budgets", default="", help="comma separated contra3.exe --cpu-budget values to sweep")
    ap.add_argument("--out", default=os.path.join(ROOT, "build", "refemu"))
    args = ap.parse_args()
    os.makedirs(args.out, exist_ok=True)

    scenarios = []
    idle = os.path.join(args.out, "attract_idle.c3m")
    with open(idle, "w") as fp:
        fp.write("C3M 1\n# no input: attract demos\n")
        fp.writelines("0000 0000\n" for _ in range(args.frames))
    scenarios.append(("attract", idle, []))
    for stage in range(1, 7):
        for players in (1, 2):
            mv = os.path.join(args.out, f"stage{stage}_{players}p.c3m")
            soak.make_movie(mv, stage, players, args.frames, 1)
            scenarios.append((f"stage{stage}_{players}p", mv, ["--poke", f"800:0086={stage:x}", "--infinite-lives"]))

    # Reference traces (independent of the budget)
    for name, mv, extra in scenarios:
        ref = os.path.join(args.out, f"ref_{name}.csv")
        p = run([args.harness, args.core, args.rom, mv, ref, "--frames", str(args.frames)] + extra)
        if p.returncode != 0:
            sys.exit(f"harness failed on {name}:\n{p.stdout}{p.stderr}")

    budgets = [b for b in args.budgets.split(",") if b] or [""]
    worst = 0
    for budget in budgets:
        label = f"--cpu-budget {budget}" if budget else "default budget"
        clean = 0
        print(f"== {label}")
        for name, mv, extra in scenarios:
            ours = os.path.join(args.out, f"ours_{name}{'_' + budget if budget else ''}.csv")
            cmd = [EXE, "--rom", args.rom, "--headless", str(args.frames * 2), "--movie", mv, "--movie-logic",
                   "--strict", "--trace", ours] + extra + (["--cpu-budget", budget] if budget else [])
            run(cmd)
            p = run([sys.executable, COMPARE, os.path.join(args.out, f"ref_{name}.csv"), ours, "--max-report", "1"])
            first = re.search(r"MATCH: all (\d+)|DIVERGED: (\d+) / (\d+)", p.stdout)
            detail = re.search(r"first divergence: reference frame (\d+) \(movie (\d+)\)", p.stdout)
            if p.returncode == 0:
                clean += 1
                print(f"   {name:<12} MATCH ({first.group(1)} logic frames)")
            else:
                where = f", first at logic frame {int(detail.group(2)) + 1}" if detail else ""
                print(f"   {name:<12} DIVERGED {first.group(2)}/{first.group(3)}{where}" if first else f"   {name:<12} ERROR")
        print(f"   {clean}/{len(scenarios)} scenarios bit-exact over 13 probe fields")
        worst = max(worst, len(scenarios) - clean)
    return 1 if worst else 0


if __name__ == "__main__":
    sys.exit(main())
