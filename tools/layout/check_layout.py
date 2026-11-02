#!/usr/bin/env python3
"""Check the source tree against the layering rules (docs/architecture.md).

usage: python3 tools/layout/check_layout.py [--quiet]

Derives each translation unit's subsystem from where its files live now
(config/matched.csv `source` column: src/game/<subsystem...>/<tu>/<file>,
src/sdk/<lib>/<file>, src/generated/<name>/<file>), then runs
check_layering.py with the fixed dependency graph (deps.csv, extracted from
the binary, so it never changes), the layer table (layers.csv) and the
allowlist of known, documented couplings (layering_allow.csv).

Fails (exit 1) if:
  * a translation unit's files are spread over more than one subsystem, or
  * a move creates an upward dependency or a cycle that is not allowlisted.
Moving a TU to another subsystem is fine as long as this passes; if a new
coupling is intended, add it to layering_allow.csv with a reason.
"""
import csv, os, subprocess, sys, tempfile, collections
HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))

def subsystem_of(path):
    parts = path.split('/')
    if parts[0] != 'src' or len(parts) < 3:
        return None
    if parts[1] in ('sdk', 'generated'):
        return '/'.join(parts[1:3])
    if parts[1] == 'game' and len(parts) >= 5:
        return '/'.join(parts[1:-2])      # drop the <tu>/ leaf and the file name
    return None

units = {}
for r in csv.DictReader(open(os.path.join(HERE, 'units.csv'))):
    units[(r['function'], r['addr'].lower())] = r['home_tu']

tu_subs = collections.defaultdict(set)
unknown = []
for r in csv.DictReader(open(os.path.join(ROOT, 'config', 'matched.csv'), newline='')):
    tu = units.get((r['name'], r['addr'].lower()))
    sub = subsystem_of(r['source'])
    if tu is None or sub is None:
        unknown.append((r['name'], r['source'])); continue
    tu_subs[tu].add(sub)

bad = {t: s for t, s in tu_subs.items() if len(s) > 1}
if unknown:
    print(f'note: {len(unknown)} matched.csv rows not covered (new functions or unexpected paths), e.g. {unknown[:3]}')
if bad:
    for t, s in sorted(bad.items()):
        print(f'SPLIT TU: {t} has files in {sorted(s)}')
    sys.exit(1)

with tempfile.NamedTemporaryFile('w', suffix='.csv', delete=False, newline='') as f:
    w = csv.writer(f); w.writerow(['home_tu', 'subsystem'])
    for t, s in sorted(tu_subs.items()):
        w.writerow([t, next(iter(s))])
    mp = f.name
args = [sys.executable, os.path.join(HERE, 'check_layering.py'), mp,
        os.path.join(HERE, 'deps.csv'), os.path.join(HERE, 'layers.csv'),
        '--allow', os.path.join(HERE, 'layering_allow.csv')] + sys.argv[1:]
rc = subprocess.call(args)
os.unlink(mp)
sys.exit(rc)
