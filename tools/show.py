#!/usr/bin/env python3
"""show.py NAME... : print target asm, m2c draft and Sly 1 reference C for functions."""
import os, sys, re, csv, glob
sys.path.insert(0, os.path.dirname(__file__))
import matchtool as M
ROOT = M.ROOT
SLY1 = os.environ.get('SLY2_SLY1', os.path.expanduser('~/sly1'))   # checkout of github.com/TheOnlyZac/sly1

def sly1_ref(name):
    p = os.path.join(ROOT, 'config', 'sly1_matches.csv')
    if not os.path.exists(p): return None
    funcs = M.load_functions()
    if name not in funcs: return None
    addr = funcs[name][0]
    for r in csv.DictReader(open(p)):
        if int(r['sly2_addr'], 16) == addr:
            s1 = r['name']; base = s1.split('__')[0]
            for f in glob.glob(SLY1 + '/src/**/*.c', recursive=True):
                t = open(f, errors='ignore').read()
                m = re.search(r'\n([^\n;{}]*\b%s\s*\([^;{]*\)\s*\{.*?\n\})' % re.escape(base), t, re.S)
                if m and 'INCLUDE_ASM' not in m.group(1):
                    return f'// Sly 1 {s1} ({r["method"]} {r["score"]}) from {os.path.relpath(f, SLY1)}\n' + m.group(1)
            return f'// Sly 1 counterpart {s1} ({r["method"]} {r["score"]}) has no C yet'
    return None

for name in sys.argv[1:]:
    funcs = M.load_functions()
    if name not in funcs: print('unknown', name); continue
    a, s = funcs[name]
    print(f'===== {name} @0x{a:06X} ({s//4} instr)')
    for i, w in enumerate(M.target_words(a, s)):
        print(f'  {a+4*i:06X}  {M.disasm(w, a+4*i)}')
    for d in ('automatch3', 'automatch2', 'automatch'):
        c = os.path.join(ROOT, 'build', d, name + '.best.c')
        if not os.path.exists(c): c = os.path.join(ROOT, 'build', d, name + '.c')
        if os.path.exists(c):
            print(f'----- m2c draft ({os.path.relpath(c, ROOT)})'); print(open(c).read().replace('#include "m2c_prelude.h"\n', '')); break
    r = sly1_ref(name)
    if r: print('----- sly1'); print(r)
