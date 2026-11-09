#!/usr/bin/env python3
"""Compare compiled objects against a baseline recorded before an edit: what matchtool cannot see.

matchtool masks every field a relocation fills in, so a call that now resolves to another overload or virtual slot,
a linkonce copy that appears or vanishes, or a symbol whose size or section changed can all still report MATCH.
This records, per source file, four normalised views of the object and compares them later:

  nm    defined and undefined symbol names (nm)
  size  defined symbols with size, type and section-relative size (nm -S --defined-only)
  sec   section names and sizes (objdump -h; .gnu.linkonce.* sections included)
  rel   relocations by symbol: section, offset, type, target symbol (objdump -r)

Usage:
  objcmp.py record [--root REPO] [--kinds nm,size,sec,rel] [--jobs N] BASE.json FILE...
  objcmp.py cmp    [--root REPO] [--kinds ...]            [--jobs N] BASE.json FILE...

FILE paths are relative to REPO (default: the repository this script lives in). Objects are built with REPO's
tools/matchtool.py (its environment variables apply, e.g. SLY2_PYTHONPATH, SLY2_M2C_INC). Record the baseline once,
in a separate worktree checked out at the base commit, BEFORE editing; then compare the edited tree against it.
The hash suffix g++ appends to _GLOBAL_$I$/$D$ names is normalised away. Exit status 1 if anything differs.
"""
import argparse, json, os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

KINDS = ('nm', 'size', 'sec', 'rel')


def norm(s):
    return re.sub(r'(_GLOBAL_\$[ID]\$\S*?\.c(?:pp)?)\w{6}', r'\1', s)


def views(obj, kinds):
    out = {}
    if 'nm' in kinds:
        txt = subprocess.run(['nm', obj], capture_output=True, text=True).stdout
        out['nm'] = sorted(norm(l.split(None, 1)[-1] if len(l.split()) >= 2 else l) for l in txt.splitlines())
    if 'size' in kinds:
        txt = subprocess.run(['nm', '-S', '--defined-only', obj], capture_output=True, text=True).stdout
        out['size'] = sorted('%s %s %s' % (norm(p[3]), p[2], p[1]) for p in (l.split() for l in txt.splitlines())
                             if len(p) == 4)
    if 'sec' in kinds:
        txt = subprocess.run(['objdump', '-h', obj], capture_output=True, text=True).stdout
        out['sec'] = ['%s %s' % (norm(p[1]), p[2]) for p in (l.split() for l in txt.splitlines())
                      if len(p) >= 7 and p[0].isdigit()]
    if 'rel' in kinds:
        txt = subprocess.run(['objdump', '-r', obj], capture_output=True, text=True).stdout
        rel, sec = [], None
        for l in txt.splitlines():
            m = re.match(r'RELOCATION RECORDS FOR \[(.*)\]:', l)
            if m:
                sec = m.group(1); continue
            p = l.split()
            if len(p) >= 3 and re.fullmatch(r'[0-9a-f]+', p[0]):
                rel.append('%s %s %s %s' % (sec, p[0], p[1], norm(' '.join(p[2:]))))
        out['rel'] = rel
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0], epilog=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('mode', choices=('record', 'cmp'))
    ap.add_argument('base')
    ap.add_argument('files', nargs='*')
    ap.add_argument('--root', default=os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
    ap.add_argument('--kinds', default=','.join(KINDS))
    ap.add_argument('--jobs', type=int, default=int(os.environ.get('JOBS', '2')))
    a = ap.parse_args()
    kinds = [k for k in a.kinds.split(',') if k]
    assert all(k in KINDS for k in kinds), 'kinds: ' + ','.join(KINDS)
    base = os.path.abspath(a.base)
    os.chdir(a.root)
    sys.path.insert(0, os.path.join(a.root, 'tools'))
    import matchtool

    def one(f):
        try:
            o = matchtool.compile_src(f)
        except SystemExit:
            return f, 'COMPILE-FAIL'
        try:
            return f, views(o, kinds)
        finally:
            os.unlink(o)

    with ThreadPoolExecutor(a.jobs) as ex:
        res = dict(ex.map(one, a.files))
    if a.mode == 'record':
        d = json.load(open(base)) if os.path.exists(base) else {}
        d.update(res)
        json.dump(d, open(base, 'w'))
        print('recorded', len(res), 'fails', sum(v == 'COMPILE-FAIL' for v in res.values()))
        return 0
    old = json.load(open(base))
    bad = {k: 0 for k in kinds}
    for f, r in sorted(res.items()):
        if f not in old:
            print('NOBASE', f); continue
        if r == 'COMPILE-FAIL' or old[f] == 'COMPILE-FAIL':
            if r != old[f]:
                print('COMPILE', f, 'base:', 'fail' if old[f] == 'COMPILE-FAIL' else 'ok',
                      'now:', 'fail' if r == 'COMPILE-FAIL' else 'ok')
                for k in kinds: bad[k] += 1
            continue
        for k in kinds:
            if k not in old[f]:
                print('NOBASE', f, k); continue
            if r[k] != old[f][k]:
                bad[k] += 1
                x, y = set(old[f][k]), set(r[k])
                print('%sDIFF %s -%d +%d  - %s  + %s' % (k.upper(), f, len(x - y), len(y - x),
                                                       sorted(x - y)[:4], sorted(y - x)[:4]))
    print('compared %d files: ' % len(res) + ', '.join('%s %d diff' % (k, bad[k]) for k in kinds))
    return 1 if any(bad.values()) else 0


if __name__ == '__main__':
    sys.exit(main())
