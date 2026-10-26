#!/usr/bin/env python3
"""
Contra III: The Alien Wars (USA) - All-Stage Soak Verification

Drives every stage (1-6, 1P and 2P) of the static recompilation headlessly with a
deterministic, biased-random input bot and checks that the recompiled code never
hits an unresolved indirect jump and never hangs (logic frame counter frozen).

For each run the script:
  1. Generates a power-on .c3m movie: title-screen menu inputs, then bot input.
  2. Warps to the stage by writing the stage number ($0086) during the
     mode-select fade (--poke), keeps lives topped up (--infinite-lives).
  3. Runs `contra3.exe --headless --verify --trace` and parses the summary.

This is a robustness soak, not an equivalence proof: bit-exact equivalence needs a
reference emulator trace (tools/emu_trace.lua + tools/compare_trace.py).

Usage:
  python tools/soak_all_stages.py [--frames 36000] [--seeds 1] [--out build/soak]
"""

import argparse
import csv
import os
import random
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EXE = os.path.join(ROOT, "build", "contra3.exe")

B, Y, SELECT, START = 0x8000, 0x4000, 0x2000, 0x1000
UP, DOWN, LEFT, RIGHT = 0x0800, 0x0400, 0x0200, 0x0100
A, X, L, R = 0x0080, 0x0040, 0x0020, 0x0010

OVERHEAD_STAGES = {2, 5}

# Frame numbers of the boot / title sequence (see README headless example)
TITLE_START = (620, 625)
MENU_DOWN = (700, 703)
MENU_START = (760, 765)
WARP_FRAME = 800
POINT_SELECT = 1460          # overhead stages: "CHOOSE YOUR STARTING POINT"
BOT_START = 1000


class Bot:
    """Deterministic input generator with held 'segments' so movement looks intentional."""

    def __init__(self, rng, overhead):
        self.rng = rng
        self.overhead = overhead
        self.move = RIGHT if not overhead else UP
        self.move_left = 0
        self.aim = 0
        self.aim_left = 0
        self.jump_left = 0
        self.next_jump = rng.randint(30, 90)
        self.rot = 0
        self.rot_left = 0
        self.next_bomb = rng.randint(900, 2400)

    def step(self, f):
        r = self.rng
        m = 0
        if self.move_left <= 0:
            if self.overhead:
                self.move = r.choices([UP, UP | LEFT, UP | RIGHT, DOWN, 0], [6, 2, 2, 1, 1])[0]
            else:
                self.move = r.choices([RIGHT, LEFT, 0], [8, 1, 1])[0]
            self.move_left = r.randint(20, 160)
        self.move_left -= 1
        m |= self.move

        if self.aim_left <= 0:
            self.aim = 0 if self.overhead else r.choices([0, UP, DOWN, UP | RIGHT], [5, 2, 1, 2])[0]
            self.aim_left = r.randint(15, 120)
        self.aim_left -= 1
        m |= self.aim

        if f % 6 < 4:                      # tap-fire for single-shot weapons
            m |= Y

        if self.overhead:
            if self.rot_left <= 0 and r.random() < 0.02:
                self.rot = r.choice([L, R])
                self.rot_left = r.randint(10, 50)
            if self.rot_left > 0:
                m |= self.rot
                self.rot_left -= 1
        else:
            self.next_jump -= 1
            if self.next_jump <= 0:
                self.jump_left = r.randint(4, 18)
                self.next_jump = r.randint(25, 110)
            if self.jump_left > 0:
                m |= B
                self.jump_left -= 1

        self.next_bomb -= 1
        if self.next_bomb <= 0:
            m |= A
            self.next_bomb = r.randint(900, 2400)
        if m & UP and m & DOWN:
            m &= ~DOWN
        if m & LEFT and m & RIGHT:
            m &= ~LEFT
        return m


def make_movie(path, stage, players, frames, seed):
    overhead = stage in OVERHEAD_STAGES
    rng = random.Random(seed * 1000 + stage * 10 + players)
    bots = [Bot(random.Random(rng.random()), overhead) for _ in range(players)]
    with open(path, "w") as fp:
        fp.write("C3M 1\n")
        fp.write(f"# soak bot: stage {stage}, {players}P, seed {seed}, {frames} frames\n")
        for f in range(1, frames + 1):
            pads = [0, 0]
            if TITLE_START[0] <= f <= TITLE_START[1] or MENU_START[0] <= f <= MENU_START[1]:
                pads[0] |= START
            if players == 2 and MENU_DOWN[0] <= f <= MENU_DOWN[1]:
                pads[0] |= DOWN
            if overhead and POINT_SELECT <= f <= POINT_SELECT + 5:
                pads[0] |= START
            if overhead and players == 2 and POINT_SELECT + 10 <= f <= POINT_SELECT + 15:
                pads[1] |= START
            if f >= BOT_START and not (POINT_SELECT - 20 <= f <= POINT_SELECT + 40 and overhead):
                for p in range(players):
                    pads[p] |= bots[p].step(f)
            fp.write(f"{pads[0]:04X} {pads[1]:04X}\n")


def run(stage, players, frames, seed, out_dir):
    tag = f"stage{stage}_{players}p_seed{seed}"
    movie = os.path.join(out_dir, tag + ".c3m")
    trace = os.path.join(out_dir, tag + ".csv")
    shot = os.path.join(out_dir, tag + "_last.bmp")
    make_movie(movie, stage, players, frames, seed)
    cmd = [EXE, "--headless", "0", "--movie", movie, "--movie-logic", "--poke", f"{WARP_FRAME}:0086={stage:x}",
           "--infinite-lives", "--strict", "--verify", "--trace", trace, "--dump", f"{frames}:{shot}"]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    out = proc.stdout

    m = re.search(r"unresolved=(\d+)(.*)$", out.strip().splitlines()[-1] if out.strip() else "")
    unresolved = int(m.group(1)) if m else -1
    hang = "HANG" in out
    fps = re.search(r"\(([\d.]+) FPS\)", out)

    stages_seen, max_cam, clears, states = set(), 0, 0, set()
    prev_state = None
    with open(trace, newline="") as fp:
        for row in csv.DictReader(fp):
            st, stg = int(row["state"]), int(row["stage"])
            states.add(st)
            if st == 9:
                stages_seen.add(stg)
                max_cam = max(max_cam, int(row["cam_x"]))
            if st == 0x0A and prev_state != 0x0A:     # stage-clear tally entered
                clears += 1
            prev_state = st
    return {
        "tag": tag, "rc": proc.returncode, "unresolved": unresolved, "hang": hang,
        "stages": sorted(stages_seen), "clears": clears, "max_cam": max_cam,
        "states": "".join(f"{s:X}" for s in sorted(states)),
        "fps": float(fps.group(1)) if fps else 0.0,
        "detail": [l for l in (out + proc.stderr).splitlines()
                   if l.startswith(("[HANG]", "[HEADLESS] last", "[STRICT]", "[FALLBACK]"))],
    }


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--frames", type=int, default=36000, help="frames per run (default 36000 = 10 minutes)")
    ap.add_argument("--seeds", type=int, default=1, help="bot seeds per stage/player-count")
    ap.add_argument("--stages", default="1,2,3,4,5,6")
    ap.add_argument("--players", default="1,2")
    ap.add_argument("--out", default=os.path.join(ROOT, "build", "soak"))
    args = ap.parse_args()

    if not os.path.exists(EXE):
        sys.exit(f"error: {EXE} not found - run build.ps1 first")
    os.makedirs(args.out, exist_ok=True)

    results = []
    for stage in [int(s) for s in args.stages.split(",")]:
        for players in [int(p) for p in args.players.split(",")]:
            for seed in range(1, args.seeds + 1):
                r = run(stage, players, args.frames, seed, args.out)
                results.append(r)
                status = "OK" if r["rc"] == 0 and r["unresolved"] == 0 and not r["hang"] else "FAIL"
                print(f"{status:4} {r['tag']:<22} unresolved={r['unresolved']} hang={int(r['hang'])} "
                      f"stages_played={r['stages']} clears={r['clears']} max_cam_x={r['max_cam']} "
                      f"states={r['states']} {r['fps']:.0f} FPS", flush=True)
                for d in r["detail"]:
                    print("     " + d)

    failed = [r for r in results if r["rc"] != 0 or r["unresolved"] != 0 or r["hang"]]
    total = sum(args.frames for _ in results)
    print(f"\n{len(results) - len(failed)}/{len(results)} runs clean, {total} frames "
          f"({total / 60.0988 / 60:.1f} min of gameplay), artifacts in {args.out}")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
