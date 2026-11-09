#!/usr/bin/env python3
"""Dump the offset and size of every member of every struct/class/union in the shared headers, as the real compiler
lays them out (offsetof/sizeof compiled with the game compiler and read back from the assembly).

Usage:
  dump_offsets.py [--root REPO] [--out DIR] [--subdirs sly2,] [--exclude a.h,b.h] [--prelude sly2/basic.h]
                  [--needs HDR=PRE ...]

Headers: REPO/include/<subdir>/*.h for each comma-separated subdir ('' = include/ itself). Each header is compiled
in a probe file that first includes --prelude (comma-separated) and then any --needs prerequisites for that header.
Members the compiler rejects (bit-fields, private types) are dropped and the probe retried. Writes DIR/offsets.json:
  {type: {"size": n, "header": h, "base": b, "fields": [[off, size, name, typeword, isptr]]}}
Compiler: REPO/tools/matchtool.py's CC/CFLAGS/INCLUDES run through wibo (override with CENSUS_CC='cmd {src} {out}').
Requires an SLY2_OFFSETOF-style macro in the prelude (or pass --offsetof NAME).
"""
import argparse, json, os, re, shlex, subprocess, sys
ap = argparse.ArgumentParser(description='dump header layouts (see docstring)')
ap.add_argument('--root', default=os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
ap.add_argument('--out')
ap.add_argument('--subdirs', default='sly2,')
ap.add_argument('--exclude', default='m2c_prelude.h,splicewrap.h')
ap.add_argument('--prelude', default='sly2/basic.h')
ap.add_argument('--needs', action='append', default=['sly2/screen.h=sly2/classes.h', 'sly2/rip.h=sly2/classes.h'])
ap.add_argument('--offsetof', default='SLY2_OFFSETOF')
if any(a in ('-h', '--help') for a in sys.argv[1:]):
    print(__doc__); sys.exit(0)
A = ap.parse_args()
W = os.path.abspath(A.root)
O = os.path.abspath(A.out or os.path.join(W, 'build', 'census'))
C = O
NEEDS = {}
for n in A.needs:
    h, p = n.split('=', 1); NEEDS.setdefault(h, []).append(p)


def compile_s(src, out):
    if os.environ.get('CENSUS_CC'):
        cmd = os.environ['CENSUS_CC'].format(src=shlex.quote(src), out=shlex.quote(out))
        return subprocess.run(cmd, shell=True, capture_output=True, text=True, timeout=120)
    sys.path.insert(0, os.path.join(W, 'tools'))
    import matchtool, wibo_cc
    cmd = [wibo_cc.default_wibo(), matchtool.CC, '-S'] + matchtool.INCLUDES + matchtool.CFLAGS + [src, '-o', out]
    try:
        return subprocess.run(cmd, capture_output=True, text=True, timeout=120, cwd=W)
    except subprocess.TimeoutExpired:
        return subprocess.CompletedProcess(cmd, 124, '', 'timeout')

def strip(s):
    s = re.sub(r'/\*.*?\*/', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), s, flags=re.S)
    return re.sub(r'//[^\n]*', lambda m: ' ' * len(m.group(0)), s)

def body_of(s, i):
    d = 1; j = i
    while d and j < len(s):
        if s[j] == '{': d += 1
        elif s[j] == '}': d -= 1
        j += 1
    return s[i:j - 1], j

DEF = re.compile(r'(template\s*<[^>]*>\s*)?\b(struct|class|union)\s+(\w+)\s*(:\s*(?:public\s+|private\s+|protected\s+)?(\w+)[^{;]*)?\{')

def members(body):
    """direct members (descending into anonymous struct/union), as (name, typeword, isptr)"""
    body = re.sub(r'\)\s*(const\s*)?(:[^{;]*)?\{', lambda m: m.group(0), body)
    # drop function bodies
    while True:
        m = re.search(r'\)\s*(const\s*)?(:[^{;]*)?\{', body)
        if not m: break
        b, j = body_of(body, m.end()); body = body[:m.start()] + ');' + body[j:]
    out = []; k = 0
    while k < len(body):
        m = re.match(r'\s*(public|private|protected)\s*:', body[k:])
        if m: k += m.end(); continue
        m = re.match(r'\s*(union|struct|class)\s*(\w*)\s*(:[^{;]*)?\{', body[k:])
        if m:
            b, j = body_of(body, k + m.end())
            rest = re.match(r'\s*([^;]*);', body[j:])
            decl = rest.group(1).strip() if rest else ''
            if not m.group(2) and not decl:
                out += members(b)          # anonymous: names belong to the outer type
            elif decl:
                for part in decl.split(','):
                    mm = re.match(r'\s*(\**)\s*(\w+)', part)
                    if mm: out.append((mm.group(2), m.group(2) or '?', bool(mm.group(1))))
            k = j + (rest.end() if rest else 0); continue
        m = re.match(r'\s*enum\b[^{;]*\{', body[k:])
        if m:
            b, j = body_of(body, k + m.end()); e = body.find(';', j); k = e + 1; continue
        e = body.find(';', k)
        if e < 0: break
        st = body[k:e].strip(); k = e + 1
        if not st or '(' in st or re.match(r'(typedef|friend|static|virtual|using|enum|template|operator)\b', st): continue
        st = re.sub(r'\b(const|volatile|mutable)\b', '', st).strip()
        mt = re.match(r'((?:unsigned|signed|struct|class|union)\s+)*(\w+(?:\s*<[^>]*>)?)\s*(.*)$', st, re.S)
        if not mt: continue
        tw, rest = mt.group(2), mt.group(3)
        for part in rest.split(','):
            mm = re.match(r'\s*([*&\s]*)(\w+)\s*((\[[^\]]*\])*)\s*(:\s*\w+)?\s*(=.*)?$', part.strip(), re.S)
            if not mm or mm.group(5): continue    # bitfields: no offsetof
            out.append((mm.group(2), tw, '*' in mm.group(1)))
    return out

def types_in(path):
    s = strip(open(path).read())
    res = []
    for m in DEF.finditer(s):
        if m.group(1): continue
        name = m.group(3)
        body, j = body_of(s, m.end())
        res.append((name, m.group(5), members(body)))
    return res

def run(hdr, rel, types):
    gen = []
    for name, base, mem in types:
        gen.append(('S', name, None, 'extern "C" int s__%s = sizeof(%s);' % (name, name)))
        seen = set()
        for f, tw, ptr in mem:
            if f in seen: continue
            seen.add(f)
            gen.append(('F', name, (f, tw, ptr), 'extern "C" int o__%s__%s = %s(%s, %s); extern "C" int z__%s__%s = sizeof(((%s *)0)->%s);'
                        % (name, f, A.offsetof, name, f, name, f, name, f)))
    pre = ''.join('#include "%s"\n' % h for h in [x for x in A.prelude.split(',') if x] + NEEDS.get(rel, []) + [rel])
    npre = pre.count('\n')
    for it in range(12):
        src = pre + '\n'.join(g[3] for g in gen) + '\n'
        tf = '%s/tmp/o_%s.cpp' % (C, rel.replace('/', '_'))
        open(tf, 'w').write(src)
        p = compile_s(tf, tf + '.s')
        bad = set()
        for ln in re.findall(r'\.ii:(\d+):', p.stdout + p.stderr):
            pass
        for line in (p.stdout + p.stderr).split('\n'):
            mm = re.match(r'(.*?):(\d+): (?!warning)', line)
            if mm and ('.ii' in mm.group(1) or 'o_' in mm.group(1) or 'tmp' in mm.group(1)) and 'include' not in mm.group(1):
                bad.add(int(mm.group(2)) - npre - 1)
        if p.returncode == 0: break
        if not bad and p.returncode == 124 and it < 6: continue   # wibo hang: retry
        if not bad:
            print('FAIL', rel, (p.stdout + p.stderr)[:2000]); return {}
        gen = [g for i, g in enumerate(gen) if i not in bad]
    asm = open(tf + '.s').read()
    vals = {}
    for m in re.finditer(r'^(\w+):\s*\n\s*\.word\s+(-?\w+)', asm, re.M):
        vals[m.group(1)] = int(m.group(2), 0)
    out = {}
    for kind, name, info, _ in gen:
        if kind == 'S' and 's__' + name in vals:
            out.setdefault(name, {'size': vals['s__' + name], 'header': rel, 'fields': []})
    for name, base, mem in types:
        if name in out: out[name]['base'] = base
    for kind, name, info, _ in gen:
        if kind == 'F' and name in out:
            f, tw, ptr = info
            o, z = vals.get('o__%s__%s' % (name, f)), vals.get('z__%s__%s' % (name, f))
            if o is not None: out[name]['fields'].append([o, z, f, tw, ptr])
    for v in out.values(): v['fields'].sort()
    return out

allt = {}
os.makedirs(C + '/tmp', exist_ok=True)
hdrs = []
for sd in A.subdirs.split(','):
    d = os.path.join(W, 'include', sd)
    hdrs += sorted(os.path.join(sd, f) if sd else f for f in os.listdir(d)
                   if f.endswith('.h') and f not in A.exclude.split(','))
for rel in hdrs:
    t = types_in(W + '/include/' + rel)
    if not t: continue
    r = run(W + '/include/' + rel, rel, t)
    for k, v in r.items():
        if k not in allt: allt[k] = v
    print(rel, len(t), 'types,', len(r), 'measured', file=sys.stderr)
json.dump(allt, open(O + '/offsets.json', 'w'), indent=0)
print(len(allt), 'types')
