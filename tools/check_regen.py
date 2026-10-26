#!/usr/bin/env python3
"""
Regeneration drift check: re-runs tools/translate_rom.py into a temporary directory
and verifies the checked-in generated sources are identical (line endings ignored).

Any hand edit to a generated file, or a generator change that was not regenerated,
makes this fail. Exit status: 0 = identical, 1 = drift, 2 = generator error.

Usage: python tools/check_regen.py [--rom <path>]
"""

import argparse
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GENERATED = [
    "include/contra3_recomp.h",
    "src/bank_00.c", "src/bank_01.c", "src/bank_02.c", "src/bank_03.c", "src/bank_04.c", "src/bank_1d.c",
    "src/dispatch.c", "src/bank_data_manifest.c",
]


def read_lines(path):
    with open(path, "r", encoding="utf-8", newline="") as fp:
        return fp.read().replace("\r\n", "\n").split("\n")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--rom", default=None, help="ROM image (default: translate_rom.py default)")
    args = ap.parse_args()

    with tempfile.TemporaryDirectory(prefix="contra3_regen_") as tmp:
        cmd = [sys.executable, os.path.join(ROOT, "tools", "translate_rom.py"), "--out", tmp,
               "--stats", os.path.join(tmp, "translation_stats.json")]
        if args.rom:
            cmd += ["--rom", args.rom]
        proc = subprocess.run(cmd, capture_output=True, text=True)
        if proc.returncode != 0:
            print(proc.stdout + proc.stderr)
            return 2

        drift = 0
        for rel in GENERATED:
            ours, fresh = read_lines(os.path.join(ROOT, rel)), read_lines(os.path.join(tmp, rel))
            if ours == fresh:
                print(f"[SAME]  {rel}")
                continue
            drift += 1
            first = next((i for i, (a, b) in enumerate(zip(ours, fresh)) if a != b), min(len(ours), len(fresh)))
            print(f"[DRIFT] {rel}: first difference at line {first + 1}")
            print(f"        checked-in: {ours[first] if first < len(ours) else '<EOF>'}")
            print(f"        generated : {fresh[first] if first < len(fresh) else '<EOF>'}")
    print("generated sources match the generator" if drift == 0 else f"{drift} generated file(s) drifted")
    return 1 if drift else 0


if __name__ == "__main__":
    sys.exit(main())
