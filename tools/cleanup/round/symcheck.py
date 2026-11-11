"""Native-link symbol check over a set of objects (ported from the rounds' scratch symcheck.py).

For every merged game source (config/matched.csv sources under src/game, minus the per-function directories air/,
splice_thunks/ and crt0/), every undefined global must resolve to
  - a name in config/symbol_addrs.txt or config/functions.csv, or
  - a global defined by another of these objects, or
  - a D_XXXXXXXX / func_XXXXXXXX name (address convention; reported separately).
Also reported: strong globals defined by more than one object, and namespace-mangled global definitions.
report() returns a JSON-able dict; regressions() lists what a report has that a baseline report has not.
"""
import collections
import csv
import os
import re

from objdiff import Obj

SKIP = ('/air/', '/splice_thunks/', '/crt0/')


def sources(root):
    return sorted({r['source'] for r in csv.DictReader(open(os.path.join(root, 'config/matched.csv'), newline=''))
                   if r['source'].startswith('src/game/') and not any(s in r['source'] for s in SKIP)})


def known_names(root):
    known = set()
    for line in open(os.path.join(root, 'config/symbol_addrs.txt')):
        m = re.match(r'\s*(\S+)\s*=', line)
        if m: known.add(m.group(1))
    for r in csv.DictReader(open(os.path.join(root, 'config/functions.csv'), newline='')):
        known.add(r['Name'])
    return known


def syms(path):
    und, dfn, strong = set(), set(), set()
    o = Obj(path)
    for s in o.syms:
        if not s['name'] or s['bind'] not in (1, 2): continue
        if s['shndx'] == 0: und.add(s['name'])
        else:
            dfn.add(s['name'])
            if s['bind'] == 1 and s['shndx'] != 0xFFF2: strong.add(s['name'])
    return und, dfn, strong


def report(root, objs):
    """objs: {source: object path or None (compile failure)} for sources(root)."""
    known = known_names(root)
    tab = {s: syms(p) for s, p in objs.items() if p}
    defined_by, strong_by = collections.defaultdict(list), collections.defaultdict(list)
    for s, (u, d, st) in tab.items():
        for x in d: defined_by[x].append(s)
        for x in st: strong_by[x].append(s)

    def cls(sym):
        if sym in known: return 'config'
        if sym in defined_by: return 'object'
        if re.fullmatch(r'D_[0-9A-F]{8}|func_[0-9A-F]{8}', sym): return 'convention'
        if re.search(r'M_|V_[A-Za-z]|REG', sym) and ('__' in sym or '$' in sym): return 'namespace'
        return 'unknown'

    rep = {'objects': len(tab), 'failed': sorted(s for s, p in objs.items() if not p),
           'unresolved': {}, 'convention': {}, 'dupdefs': {}, 'nsdefs': {}}
    for s, (u, d, st) in sorted(tab.items()):
        l = sorted(x for x in d if re.search(r'\d(M_|V_)\w|Q2\d+(M_|V_|REG)|__\d+(M_|V_)', x))
        if l: rep['nsdefs'][s] = l
        for x in sorted(u):
            c = cls(x)
            if c in ('namespace', 'unknown'): rep['unresolved'].setdefault(s, []).append([c, x])
            elif c == 'convention': rep['convention'].setdefault(s, []).append(x)
    for x, ss in strong_by.items():
        if len(ss) > 1 and not x.startswith(('_GLOBAL_', '__static_initialization')):
            rep['dupdefs'][x] = sorted(ss)
    return rep


def totals(rep):
    return (f"objects {rep['objects']}, failed {len(rep['failed'])}, "
            f"unresolved {sum(len(v) for v in rep['unresolved'].values())}, "
            f"convention-only {sum(len(v) for v in rep['convention'].values())}, "
            f"duplicate strong {len(rep['dupdefs'])}, namespace defs {sum(len(v) for v in rep['nsdefs'].values())}")


def _flat(rep):
    out = set()
    for s, l in rep['unresolved'].items(): out |= {('unresolved', s, x[1]) for x in l}
    for x, ss in rep['dupdefs'].items(): out.add(('dupdef', x, ','.join(ss)))
    for s, l in rep['nsdefs'].items(): out |= {('nsdef', s, x) for x in l}
    out |= {('failed', s, '') for s in rep['failed']}
    return out


def regressions(rep, base):
    return sorted(_flat(rep) - _flat(base))
