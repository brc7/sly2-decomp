#!/usr/bin/env python3
"""namecheck.py [extract|check|pairs|all] [shared options] -- check header field names/types against RAM snapshots.

Needs steps 1-2 of the snapshot pipeline (objects/, classes.json in WORK). Passes (outputs in WORK/namecheck/):
  extract  namecheck_extract.py: per snapshot and header class, raw words + word kinds + pointee labels of up to 24
           live instances, typed globals, the player's physics block -> cache/<snap>.npz (game-derived data)
  check    namecheck_check.py: (1) name/type-implied kind vs observed values per declared field, aggregated over every
           live class containing it (pName -> pointer, fFoo -> float, cFoo -> small count, ...); mismatches that share
           a layout cluster are reported apart (usually a sampling artifact, not a wrong offset); (3) naming suggestions
           for unknown words -> namecheck.json
  pairs    namecheck_pairs.py [--spec F]: paired-state checks (a field named for a situation changes the right way
           between two snapshots of it) -> pairs.json, merged into namecheck.json
  all      extract, check, pairs (default)
Mismatches are leads, not verdicts: confirm against the code that reads the field before moving or renaming it.
Shared options (--root, --elf, --work, --census ...) are passed through; see snapcfg.py.
"""
import os, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
PASSES = {'extract': 'namecheck_extract.py', 'check': 'namecheck_check.py', 'pairs': 'namecheck_pairs.py'}

def strip_spec(argv):
    out, skip = [], False
    for a in argv:
        if skip: skip = False; continue
        if a == '--spec': skip = True; continue
        if not a.startswith('--spec='): out.append(a)
    return out

if __name__ == '__main__':
    argv = sys.argv[1:]
    if argv[:1] in (['-h'], ['--help']):
        print(__doc__); sys.exit(0)
    cmd = argv.pop(0) if argv and argv[0] in list(PASSES) + ['all'] else 'all'
    for p in (list(PASSES) if cmd == 'all' else [cmd]):
        rest = argv if p == 'pairs' else strip_spec(argv)
        r = subprocess.run([sys.executable, '-I', os.path.join(HERE, PASSES[p])] + rest)
        if r.returncode:
            sys.exit(r.returncode)
