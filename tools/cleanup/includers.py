#!/usr/bin/env python3
"""List the sources to re-check after an edit: the touched sources plus every source that includes (transitively)
a touched header.

Usage: includers.py [--root REPO] [--sources LIST] TOUCHED...
  TOUCHED   repo-relative paths (.cpp/.c/.h); or a single file of paths with @FILE
  --sources file listing the candidate sources (default: every .cpp/.c under src/ that is in config/matched.csv's
            file column if present, else every .cpp/.c under src/)
Headers are matched by base name, which is how the include lines name them. Typical use:
  git diff --name-only BASE -- src include | xargs tools/cleanup/includers.py > files.txt
"""
import argparse, csv, os, re, sys

INC = re.compile(r'#\s*include\s*[<"]([^>"]+)[>"]')


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0], epilog=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('touched', nargs='*')
    ap.add_argument('--root', default=os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
    ap.add_argument('--sources')
    a = ap.parse_args()
    os.chdir(a.root)
    touched = []
    for t in a.touched:
        touched += [l.strip() for l in open(t[1:])] if t.startswith('@') else [t]
    touched = [t for t in touched if t]
    if a.sources:
        allf = [l.strip() for l in open(a.sources) if l.strip()]
    else:
        allf = sorted(os.path.join(d, f) for d, _, fs in os.walk('src') for f in fs if f.endswith(('.cpp', '.c')))
    hpath = {}
    for top in ('include', 'src'):
        for d, _, fs in os.walk(top):
            for f in fs:
                if f.endswith(('.h', '.inc')): hpath.setdefault(f, os.path.join(d, f))
    cache = {}

    def incs(p):
        if p not in cache:
            try: cache[p] = [os.path.basename(m) for m in INC.findall(open(p, errors='replace').read())]
            except OSError: cache[p] = []
        return cache[p]

    def closure(p):
        seen, st = set(), [p]
        while st:
            for h in incs(st.pop()):
                if h not in seen:
                    seen.add(h); st.append(hpath.get(h, ''))
        return seen

    hdrs = {os.path.basename(t) for t in touched if t.endswith(('.h', '.inc'))}
    out = {t for t in touched if t in allf}
    if hdrs:
        out |= {f for f in allf if hdrs & closure(f)}
    print('\n'.join(sorted(out)))


if __name__ == '__main__':
    main()
