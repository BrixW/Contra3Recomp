#!/usr/bin/env python3
"""
Contra III: The Alien Wars (USA) - Trace Co-Simulation Comparator

Compares a reference emulator trace (tools/emu_trace.lua) with a trace from
    contra3.exe --headless 0 --movie <movie> --lag-from emu_trace.csv --trace ours.csv

Both files share the same columns. Emulator lag frames (lag=1) are dropped
before alignment because the static recompilation services every NMI and the
lag frames' input was already skipped on playback via --lag-from. Remaining
rows are aligned 1:1 (optionally shifted with --offset) and every probe field
is compared. The first divergence is reported with surrounding context.

Exit status: 0 = traces match, 1 = divergence found, 2 = usage / input error.
"""

import argparse
import csv
import sys

DEFAULT_FIELDS = [
    "state", "sub", "nmi_mode", "stage",
    "p1_x", "p1_y", "p2_x", "p2_y",
    "lives1", "lives2", "cam_x", "rng_lo", "rng_hi",
]


def load(path, drop_lag):
    try:
        with open(path, newline="") as fp:
            rows = list(csv.DictReader(fp))
    except OSError as exc:
        sys.exit(f"error: {exc}")
    if drop_lag:
        rows = [r for r in rows if r.get("lag", "0") in ("0", "")]
    return rows


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("reference", help="emulator trace CSV (tools/emu_trace.lua)")
    ap.add_argument("recomp", help="contra3.exe --trace CSV")
    ap.add_argument("--offset", type=int, default=0,
                    help="shift: compare reference row i with recomp row i+OFFSET")
    ap.add_argument("--fields", default=",".join(DEFAULT_FIELDS),
                    help="comma separated fields to compare (default: %(default)s)")
    ap.add_argument("--keep-lag", action="store_true", help="do not drop reference lag frames")
    ap.add_argument("--context", type=int, default=4, help="rows of context around the first divergence")
    ap.add_argument("--max-report", type=int, default=10, help="divergent rows to list")
    args = ap.parse_args()

    ref = load(args.reference, drop_lag=not args.keep_lag)
    ours = load(args.recomp, drop_lag=False)
    fields = [f.strip() for f in args.fields.split(",") if f.strip()]
    for f in fields:
        if ref and f not in ref[0]:
            sys.exit(f"error: field '{f}' missing from {args.reference}")
        if ours and f not in ours[0]:
            sys.exit(f"error: field '{f}' missing from {args.recomp}")

    n = min(len(ref), len(ours) - args.offset) if args.offset >= 0 else min(len(ref) + args.offset, len(ours))
    if n <= 0:
        print("error: no overlapping rows", file=sys.stderr)
        return 2

    mismatch_counts = {f: 0 for f in fields}
    divergent = []
    first = None
    for i in range(n):
        ri = i if args.offset >= 0 else i - args.offset
        oi = i + args.offset if args.offset >= 0 else i
        r, o = ref[ri], ours[oi]
        bad = [f for f in fields if r[f] != o[f]]
        for f in bad:
            mismatch_counts[f] += 1
        if bad:
            divergent.append((ri, oi, bad))
            if first is None:
                first = (ri, oi)

    print(f"reference rows (lag dropped): {len(ref)} | recomp rows: {len(ours)} | compared: {n}")
    if first is None:
        print(f"MATCH: all {n} aligned frames identical across {len(fields)} probe fields")
        return 0

    ok = n - len(divergent)
    print(f"DIVERGED: {len(divergent)} / {n} frames differ ({100.0 * ok / n:.2f}% identical)")
    print("per-field mismatches: " + ", ".join(f"{f}={c}" for f, c in mismatch_counts.items() if c))
    ri0, oi0 = first
    print(f"first divergence: reference frame {ref[ri0]['frame']} (movie {ref[ri0]['movie_frame']}) "
          f"vs recomp frame {ours[oi0]['frame']}")

    cols = ["frame"] + fields
    lo = max(0, ri0 - args.context)
    hi = min(n, ri0 + args.context + 1)
    print("\ncontext (ref / recomp):")
    print("  " + " ".join(f"{c:>8}" for c in cols))
    for k in range(lo, hi):
        rk = k if args.offset >= 0 else k - args.offset
        ok_ = k + args.offset if args.offset >= 0 else k
        if rk >= len(ref) or ok_ >= len(ours):
            break
        mark = "*" if any(ref[rk][f] != ours[ok_][f] for f in fields) else " "
        print(f"{mark}R " + " ".join(f"{ref[rk][c]:>8}" for c in cols))
        print(f"{mark}C " + " ".join(f"{ours[ok_][c]:>8}" for c in cols))

    print(f"\nfirst {min(args.max_report, len(divergent))} divergent frames:")
    for ri, oi, bad in divergent[:args.max_report]:
        diffs = ", ".join(f"{f}: {ref[ri][f]} != {ours[oi][f]}" for f in bad)
        print(f"  ref {ref[ri]['frame']:>7} / recomp {ours[oi]['frame']:>7}: {diffs}")
    return 1


if __name__ == "__main__":
    sys.exit(main())
