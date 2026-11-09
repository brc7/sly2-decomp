#!/usr/bin/env python3
"""Resolve every conflict hunk in the given files to the incoming side ("theirs"), keeping the rest of the file.

Usage: take_theirs.py FILE...
For files where one side is known to be authoritative (for example a family branch that owns a header, merged into
the integration branch). Only `<<<<<<<` / `=======` / `>>>>>>>` hunks are touched; diff3-style `|||||||` bases are
dropped with the ours side. Review with `git diff` and re-verify afterwards: taking one side can drop a rename the
other side made in the same lines.
"""
import sys

if len(sys.argv) < 2 or sys.argv[1] in ('-h', '--help'):
    print(__doc__); sys.exit(0)
for p in sys.argv[1:]:
    lines = open(p).read().split('\n'); out = []; st = 0
    for l in lines:
        if l.startswith('<<<<<<< '): st = 1; continue
        if st and l.startswith('|||||||'): st = 1; continue
        if st and l.strip() == '=======': st = 2; continue
        if st and l.startswith('>>>>>>> '): st = 0; continue
        if st == 1: continue
        out.append(l)
    open(p, 'w').write('\n'.join(out))
