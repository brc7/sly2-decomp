#!/usr/bin/env python3
"""Compile every src/**/*.cpp and src/**/*.c, verify each function, write config/matched.csv, print totals.
Results are cached per source file (by mtime) in build/progress_cache.json."""
import os, sys, glob, csv, json, subprocess, tempfile, shutil
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, os.path.dirname(__file__))
import matchtool as M

CACHE = os.path.join(M.ROOT, 'build', 'progress_cache.json')
B = os.path.join(M.ROOT, 'tools/compilers/bundle/ee-gcc2.95.3-136/lib/gcc-lib/ee/2.95.3')

def compile_any(src):
    head = open(src, errors='ignore').read(2000)
    if 'SLY2_CC:' in head or 'SLY2_CFLAGS:' in head or 'SLY2_ASM:' in head:   # per-file compiler override (SDK library code)
        try:
            of = M.compile_src(src)
        except SystemExit:
            return None
        fns = M.obj_functions(of); os.remove(of)
        return fns
    t = tempfile.mkdtemp(); of = os.path.join(t, 'f.o')
    if src.endswith('.c'):
        flags = ['-x', 'c', '-O2', '-G0', '-ffast-math', '-mvu0-use-vf0-vf31', '-B' + B + '/', '-I' + M.M2C_INC]
    else:
        flags = M.CFLAGS
    p = subprocess.run(M.WIBO_CMD + [M.CC, '-c'] + M.INCLUDES + flags + [os.path.relpath(src, M.ROOT), '-o', of], capture_output=True, text=True, cwd=M.ROOT, timeout=120)   # relative path, as matchtool.py compiles it (a few files depend on the source path)
    if not os.path.exists(of):
        shutil.rmtree(t, ignore_errors=True); return None
    fns = M.obj_functions(of); shutil.rmtree(t, ignore_errors=True)
    return fns

def one(src):
    try:
        fns = compile_any(src)
    except Exception:
        fns = None
    if fns is None: return src, None
    res = []
    for name, (w, r) in fns.items():
        score, out = M.compare(name, w, r, verbose=False)
        if score is None: continue
        res.append([name, score])
    return src, res

def real_functions(funcs):
    out = {}
    junk = M.load_junk()
    for n, (a, s) in funcs.items():
        if not (M.TEXT_VA <= a < M.TEXT_VA + M.TEXT_SIZE): continue
        if M.is_junk(a, s, junk): continue       # dead-code scrap (config/junk.csv), not a function
        w = M.target_words(a, s)
        if all(x == 0 for x in w): continue      # alignment padding, not a function
        out[a] = (n, s)
    return out

def main():
    funcs = M.load_functions()
    real = real_functions(funcs)
    total_f = len(real); total_i = sum(s // 4 for n, s in real.values())
    # Cache is keyed by repo-relative path so results from different environments merge (e.g. on Apple Silicon:
    # wibo-compiled files natively on macOS + Linux-only SDK compilers in Docker, sharing one checkout).
    # A file whose build FAILED is retried on every run; a failed build never overwrites a good result for the
    # same file version (mtime).
    cache = json.load(open(CACHE)) if os.path.exists(CACHE) else {}
    cache = {k: v for k, v in cache.items() if not os.path.isabs(k)}   # drop old absolute-path entries
    srcs = sorted(glob.glob(os.path.join(M.ROOT, 'src', '**', '*.cpp'), recursive=True) +
                  glob.glob(os.path.join(M.ROOT, 'src', '**', '*.c'), recursive=True))
    rel = lambda p: os.path.relpath(p, M.ROOT).replace(os.sep, '/')
    def fresh(s):
        e = cache.get(rel(s))
        return e is not None and e['mtime'] == os.path.getmtime(s) and e['res'] is not None
    todo = [s for s in srcs if not fresh(s)]
    import time
    t0 = time.time(); every = max(1, len(todo) // 100)
    print(f'compiling {len(todo)} of {len(srcs)} sources (others cached), JOBS={os.environ.get("JOBS", "2")}', flush=True)
    with ThreadPoolExecutor(int(os.environ.get('JOBS', '2'))) as ex:
        for i, (src, res) in enumerate(ex.map(one, todo), 1):
            cache[rel(src)] = {'mtime': os.path.getmtime(src), 'res': res}
            if i % every == 0 or i == len(todo):   # progress + ETA (one line per ~1%)
                dt = time.time() - t0; rate = i / dt
                print(f'  {i}/{len(todo)}  {rate:.1f} files/s  elapsed {dt/60:.1f} min  ETA {(len(todo)-i)/rate/60:.1f} min', flush=True)
    live = {rel(s) for s in srcs}
    for k in list(cache):
        if k not in live: del cache[k]
    os.makedirs(os.path.dirname(CACHE), exist_ok=True)
    json.dump(cache, open(CACHE, 'w'))
    rows = []; broken = []
    prev = {}   # previous results per source: a source this machine cannot build (e.g. SDK files needing Linux-only compilers) keeps them
    try:
        for r in csv.DictReader(open(os.path.join(M.ROOT, 'config', 'matched.csv'), newline='')):
            prev.setdefault(r['source'], []).append((r['name'], r['addr'], int(r['n_instr']), r['status'], r['source']))
    except OSError:
        pass
    for src in srcs:
        res = cache[rel(src)]['res']
        if res is None:
            broken.append(src)
            rows.extend(prev.get(os.path.relpath(src, M.ROOT).replace(os.sep, '/'), []))
            continue
        for name, score in res:
            a, s = funcs[name]
            rows.append((name, f'0x{a:06X}', s // 4, 'match' if score == 1.0 else f'{score:.3f}', os.path.relpath(src, M.ROOT).replace(os.sep, '/')))
    rows.sort(key=lambda r: r[1])
    with open(os.path.join(M.ROOT, 'config', 'matched.csv'), 'w', newline='') as f:
        w = csv.writer(f); w.writerow(['name', 'addr', 'n_instr', 'status', 'source']); w.writerows(rows)
    seen = {}
    for r in rows:
        if r[3] == 'match' and int(r[1], 16) in real: seen.setdefault(r[1], r)
    mf = len(seen); mi = sum(r[2] for r in seen.values())
    print(f'matched functions: {mf}/{total_f} ({mf/total_f:.2%})   instructions: {mi}/{total_i} ({mi/total_i:.2%})')
    print(f'  hand-written (src/P2 etc): {sum(1 for r in seen.values() if "/auto/" not in r[4])}, auto/fixed m2c (src/auto): {sum(1 for r in seen.values() if "/auto/" in r[4])}')
    for b in broken[:20]: print('  BUILD FAILED (previous results kept, NOT verified):', rel(b))
    if len(broken) > 20: print(f'  ... and {len(broken) - 20} more')
    if broken:
        nf = sum(len(prev.get(rel(b), [])) for b in broken)
        print(f'WARNING: {len(broken)} source files did not build in any environment yet; their {nf} function results '
              f'are carried over from the committed config/matched.csv and were NOT verified. '
              f'Totals above include them. (On Apple Silicon run both: natively AND `tools/dev.sh progress`.)')
    rc = 1 if broken else 0
    try:
        import progress_graph
        progress_graph.render(real, {int(a, 16) for a in seen})
        print('  graph: docs/progress.svg updated')
    except Exception as e:
        print('  graph not updated:', e)
    return rc

if __name__ == '__main__':
    sys.exit(main())
