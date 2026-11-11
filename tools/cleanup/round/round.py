#!/usr/bin/env python3
"""Round harness: one command to set up a cleanup round, check it, and try candidate edits fast.

  round.py start NAME [--base REF] [--branch B] [--worktree DIR] [--reuse] [--no-progress]
  round.py check [--full] [--round NAME]
  round.py try PATCHFILE | --stdin | -- COMMAND...   [--keep] [--all] [--vs base|pre] [-p N] [--round NAME]
  round.py status [--round NAME]

start   creates the worktree and branch at REF (default HEAD), symlinks the untracked inputs (disc, compilers, wibo),
        compiles every source into the shared object cache, and records the baseline in the round's scratch
        directory: baseline objects, base.json (objcmp views), symcheck report, progress totals and cache.
check   incremental: recompiles only the sources whose content key changed since the baseline (the key hashes the
        source and every header it includes, so includers of a changed header are found automatically), compares
        each object byte for byte with its baseline object (objdiff.py), and runs matchtool's comparison on every
        function and data symbol of those objects. --full adds symcheck vs baseline, linkcheck, docnames, progress
        from a deleted cache (matched.csv must stay unchanged) and the real link (0 differing bytes, every symbol at
        its config address).
try     snapshots the worktree (a git tree object, written through a temporary index: the real index, stash and
        branches are never touched), applies the edit (a patch with git apply, or runs COMMAND), compiles the
        affected sources and compares them with their pre-edit objects (--vs base: with the baseline), prints
        IDENTICAL / RENAMED / a compact first difference, and restores the snapshot unless --keep. It stops at the
        first differing object unless --all. If interrupted, the next round.py command restores the snapshot.

The round is found from --round, $SLY2_ROUND, or the current directory (inside a round's worktree). Settings come
from environment variables, then a config file ($SLY2_ROUND_CONF, default ~/.config/sly2/round.conf, section
[round]), then the defaults below; see README.md.
"""
import argparse
import configparser
import contextlib
import csv
import datetime
import hashlib
import importlib.util
import io
import json
import os
import re
import shutil
import signal
import subprocess
import sys
import tempfile
import threading
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import objdiff      # noqa: E402
import symcheck     # noqa: E402

# setting: (environment variable, default). {scratch} and {repo_parent} are expanded.
SETTINGS = {
    'scratch':    ('SLY2_ROUND_SCRATCH', '/home/claude/work/scratch'),
    'objcache':   ('SLY2_ROUND_OBJCACHE', '{scratch}/.objcache'),
    'worktrees':  ('SLY2_ROUND_WORKTREES', '{repo_parent}'),
    'disc':       ('SLY2_ROUND_DISC', '/home/claude/vclone/disc/SCUS_973.16'),
    'compilers':  ('SLY2_ROUND_COMPILERS', '/home/claude/vclone/tools/compilers'),
    'wibo':       ('SLY2_ROUND_WIBO', '/home/claude/vclone/tools/wibo-i686'),
    'pythonpath': ('SLY2_PYTHONPATH', '/home/claude/work/pystub'),
    'm2c_inc':    ('SLY2_M2C_INC', '/home/claude/m2c'),
    'jobs':       ('JOBS', '2'),
}
INPUTS = (('disc', 'disc/SCUS_973.16'), ('compilers', 'tools/compilers'), ('wibo', 'tools/wibo-i686'))
TOOL_FILES = ('tools/matchtool.py', 'tools/wibo_cc.py', 'tools/asfix.py', 'tools/gnu_reorder.py')
COMPILE_ENV = ('SLY2_CC', 'SLY2_CFLAGS', 'SLY2_INC', 'SLY2_M2C_INC', 'SLY2_WIBO')


def git(*args, cwd=None, env=None, check=True, inp=None):
    p = subprocess.run(['git', *args], cwd=cwd, env=env, capture_output=True, text=True, input=inp)
    if check and p.returncode:
        sys.exit(f'git {" ".join(args)}: {p.stderr.strip()}')
    return p.stdout.strip()


def main_repo():
    d = git('rev-parse', '--path-format=absolute', '--git-common-dir', cwd=HERE)
    return os.path.dirname(d)


def settings():
    cp = configparser.ConfigParser()
    cp.read(os.environ.get('SLY2_ROUND_CONF', os.path.expanduser('~/.config/sly2/round.conf')))
    sec = cp['round'] if cp.has_section('round') else {}
    out = {}
    for k, (env, dflt) in SETTINGS.items():
        out[k] = os.environ.get(env) or sec.get(k) or dflt
    rp = os.path.dirname(main_repo())
    for k in out:
        out[k] = out[k].replace('{scratch}', out['scratch']).replace('{repo_parent}', rp)
    return out


CFG = None


def log_open(sd, name):
    os.makedirs(os.path.join(sd, 'logs'), exist_ok=True)
    return open(os.path.join(sd, 'logs', name), 'w')


# ---------------------------------------------------------------- round state

def scratch_dir(name):
    return os.path.join(CFG['scratch'], name)


def load_round(name=None):
    name = name or os.environ.get('SLY2_ROUND')
    if not name:
        cwd = os.path.realpath(os.getcwd())
        for d in sorted(os.listdir(CFG['scratch'])) if os.path.isdir(CFG['scratch']) else []:
            p = os.path.join(CFG['scratch'], d, 'round.json')
            if os.path.exists(p):
                wt = os.path.realpath(json.load(open(p))['worktree'])
                if cwd == wt or cwd.startswith(wt + os.sep):
                    name = d; break
    if not name:
        sys.exit('round.py: no round: pass --round NAME, set SLY2_ROUND, or run inside a round worktree')
    p = os.path.join(scratch_dir(name), 'round.json')
    if not os.path.exists(p):
        sys.exit(f'round.py: no round {name} ({p} missing; run round.py start {name})')
    return json.load(open(p))


def save_round(st):
    p = os.path.join(scratch_dir(st['name']), 'round.json')
    json.dump(st, open(p + '.tmp', 'w'), indent=1)
    os.replace(p + '.tmp', p)


# ---------------------------------------------------------------- worktree, keys, compiles

class Tree:
    """A worktree with its own matchtool imported (matchtool locates the compilers, ELF and config from its own
    path), the content keys of its sources and the shared object cache."""

    def __init__(self, wt):
        self.wt = os.path.realpath(wt)
        os.chdir(self.wt)    # compile_src reads the source relative to the process directory
        os.environ.setdefault('SLY2_PYTHONPATH', CFG['pythonpath'])
        os.environ.setdefault('SLY2_M2C_INC', CFG['m2c_inc'])
        sys.path.insert(0, os.path.join(self.wt, 'tools'))
        spec = importlib.util.spec_from_file_location('matchtool', os.path.join(self.wt, 'tools', 'matchtool.py'))
        self.mt = importlib.util.module_from_spec(spec)
        sys.modules['matchtool'] = self.mt
        spec.loader.exec_module(self.mt)
        self._tl = threading.local()
        for modname in ('gnu_reorder', 'wibo_cc'):
            try:
                m = importlib.import_module(modname)
                m.print = self._print
            except Exception:
                pass
        self.mt.print = self._print
        self._speed_up_matchtool()
        self.cache = CFG['objcache']
        self.jobs = int(CFG['jobs'])
        fp = hashlib.sha1()
        for f in TOOL_FILES:
            p = os.path.join(self.wt, f)
            fp.update(f.encode() + (open(p, 'rb').read() if os.path.exists(p) else b'-'))
        for e in COMPILE_ENV:
            fp.update(f'{e}={os.environ.get(e, "")}\n'.encode())
        fp.update(os.path.realpath(os.path.join(self.wt, 'tools', 'compilers')).encode())
        self.fp = fp.hexdigest()
        self._content = {}
        self._incs = {}
        self._hmap = None

    def _print(self, *a, **k):
        buf = getattr(self._tl, 'buf', None)
        if buf is not None:
            print(*a, file=buf)

    def _speed_up_matchtool(self):
        """matchtool re-reads the config and the ELF on every compare(); cache them for this process."""
        import functools
        mt = self.mt
        for fn in ('load_functions', 'load_data_addrs', 'elf_sections', 'load_junk'):
            if hasattr(mt, fn) and not hasattr(getattr(mt, fn), 'cache_clear'):
                setattr(mt, fn, functools.lru_cache(None)(getattr(mt, fn)))
        elf = {}

        def target_words(addr, size, _orig=mt.target_words):
            if 'd' not in elf:
                elf['d'] = open(mt.ELF, 'rb').read()
            import struct
            off = addr - mt.TEXT_VA + mt.TEXT_OFF
            return list(struct.unpack('<%dI' % (size // 4), elf['d'][off:off + size]))
        mt.target_words = target_words

    def config_changed(self):
        for fn in ('load_functions', 'load_data_addrs', 'elf_sections', 'load_junk'):
            getattr(self.mt, fn).cache_clear()

    # -- sources and keys
    def sources(self):
        out = []
        for d, ds, fs in os.walk(os.path.join(self.wt, 'src')):
            ds.sort()
            for f in sorted(fs):
                if f.endswith(('.c', '.cpp')):
                    out.append(os.path.relpath(os.path.join(d, f), self.wt))
        return out

    def read(self, rel):
        if rel not in self._content:
            try:
                self._content[rel] = open(os.path.join(self.wt, rel), 'rb').read()
            except OSError:
                self._content[rel] = None
        return self._content[rel]

    def forget(self):
        self._content.clear(); self._incs.clear(); self._hmap = None

    def resolve(self, inc, frm):
        for cand in (os.path.join(os.path.dirname(frm), inc), os.path.join('include', inc), os.path.join('src', inc)):
            cand = os.path.normpath(cand)
            if os.path.isfile(os.path.join(self.wt, cand)):
                return cand
        if self._hmap is None:
            self._hmap = {}
            for top in ('include', 'src'):
                for d, _, fs in os.walk(os.path.join(self.wt, top)):
                    for f in fs:
                        if not f.endswith(('.c', '.cpp')):
                            self._hmap.setdefault(f, os.path.relpath(os.path.join(d, f), self.wt))
        return self._hmap.get(os.path.basename(inc))

    def includes(self, rel):
        if rel not in self._incs:
            txt = self.read(rel) or b''
            self._incs[rel] = [r for r in (self.resolve(m.decode('latin-1'), rel) for m in
                               re.findall(rb'^\s*#\s*include\s*[<"]([^>"]+)[>"]', txt, re.M)) if r]
        return self._incs[rel]

    def closure(self, rel):
        seen, st = set(), [rel]
        while st:
            for h in self.includes(st.pop()):
                if h not in seen:
                    seen.add(h); st.append(h)
        return sorted(seen)

    def key(self, rel):
        h = hashlib.sha1(self.fp.encode() + b'\0' + rel.encode() + b'\0')
        c = self.read(rel)
        if c is None:
            return None
        h.update(c)
        for f in self.closure(rel):
            h.update(b'\0' + f.encode() + b'\0' + (self.read(f) or b''))
        return h.hexdigest()

    def keys(self, srcs=None):
        self.forget()
        return {s: self.key(s) for s in (srcs if srcs is not None else self.sources())}

    # -- object cache
    def cpath(self, key, ext='.o'):
        return os.path.join(self.cache, key[:2], key + ext)

    def cached(self, key):
        """object path, or None if the cached compile failed, or False if not cached"""
        if os.path.exists(self.cpath(key)): return self.cpath(key)
        if os.path.exists(self.cpath(key, '.fail')): return None
        return False

    def build(self, rel, key):
        """(object path or None, error text). Compiles into the cache unless it is there."""
        c = self.cached(key)
        if c is not False:
            return c, (open(self.cpath(key, '.fail')).read() if c is None else '')
        self._tl.buf = io.StringIO()
        try:
            try:
                o = self.mt.compile_src(rel)
            except SystemExit:
                o = None
            except Exception as e:  # noqa: BLE001
                print(f'{type(e).__name__}: {e}', file=self._tl.buf); o = None
            err = self._tl.buf.getvalue()
        finally:
            self._tl.buf = None
        if self.key_now(rel) != key:     # the file changed while compiling: do not cache
            if o: os.unlink(o)
            return None, 'source changed during compile'
        os.makedirs(os.path.dirname(self.cpath(key)), exist_ok=True)
        if o:
            tmp = self.cpath(key, '.tmp%d' % threading.get_ident())
            shutil.move(o, tmp); os.replace(tmp, self.cpath(key))
            return self.cpath(key), ''
        if 'timed out' not in err and 'changed during' not in err:
            open(self.cpath(key, '.fail'), 'w').write(err)
        return None, err

    def key_now(self, rel):
        h = hashlib.sha1(self.fp.encode() + b'\0' + rel.encode() + b'\0')
        c = open(os.path.join(self.wt, rel), 'rb').read() if os.path.exists(os.path.join(self.wt, rel)) else None
        if c is None: return None
        h.update(c)
        for f in self.closure(rel):
            p = os.path.join(self.wt, f)
            h.update(b'\0' + f.encode() + b'\0' + (open(p, 'rb').read() if os.path.exists(p) else b''))
        return h.hexdigest()

    def build_many(self, items, stop=None, progress=None):
        """items: [(rel, key)]. Yields (rel, key, obj, err) in completion order; stop(result) -> True cancels the
        rest (already running compiles finish)."""
        todo = [(r, k) for r, k in items if k]
        with ThreadPoolExecutor(self.jobs) as ex:
            futs = {ex.submit(self.build, r, k): (r, k) for r, k in todo}
            done = 0
            for f in as_completed(futs):
                r, k = futs[f]
                o, err = f.result()
                done += 1
                if progress: progress(done, len(todo))
                yield r, k, o, err
                if stop and stop(r, o):
                    for g in futs: g.cancel()
                    break

    # -- matchtool on one object
    def match_object(self, rel, obj):
        """[(name, ok, headline, lines)] for every function and data symbol with a known address."""
        mt = self.mt
        out = []
        fns = mt.apply_aliases(rel, mt.obj_functions(obj))
        for n, (w, r) in fns.items():
            score, res = mt.compare(n, w, r, verbose=True)
            if score is None: continue
            out.append((n, res[0].endswith('MATCH') or res[0].endswith('MATCH*'), res[0], res[1]))
        data, known = mt.obj_data(obj), mt.load_data_addrs()
        for n, v in data.items():
            if not mt.data_addr(n, known): continue
            score, res = mt.compare_data(n, *v, known=known)
            if score is None: continue
            out.append((n, ' MATCH' in res[0] or res[0].endswith('MATCH'), res[0], res[1]))
        return out

    def disasm(self, word, va):
        return self.mt.disasm(word, va)


def config_names(root_or_text):
    """{name: address} from symbol_addrs.txt text and functions.csv text"""
    sa, fc = root_or_text
    out = {}
    for line in sa.splitlines():
        m = re.match(r'\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+);', line)
        if m: out[m.group(1)] = int(m.group(2), 16)
    for r in csv.DictReader(io.StringIO(fc)):
        out.setdefault(r['Name'], int(r['Start'], 16))
    return out


def config_now(wt):
    return config_names((open(os.path.join(wt, 'config/symbol_addrs.txt')).read(),
                         open(os.path.join(wt, 'config/functions.csv')).read()))


def mismatch_excerpt(lines, n=6):
    """the first differing instruction of a matchtool listing with a little context"""
    i = next((k for k, l in enumerate(lines) if l.startswith('!!')), 0)
    return ['  ' + l[:110] for l in lines[max(0, i - 1):max(0, i - 1) + n]]


# ---------------------------------------------------------------- snapshots (try)

def snapshot(wt):
    """tree object of the whole worktree (tracked + untracked, not ignored), via a temporary index"""
    # The temporary index starts from HEAD without stat data, so every file is hashed: a copy of the real index
    # would let git skip a file edited within the same second at the same size.
    fd, tmp = tempfile.mkstemp(prefix='round-idx-'); os.close(fd); os.unlink(tmp)
    try:
        env = dict(os.environ, GIT_INDEX_FILE=tmp)
        git('read-tree', 'HEAD', cwd=wt, env=env)
        git('add', '-A', '.', cwd=wt, env=env)
        return git('write-tree', cwd=wt, env=env)
    finally:
        if os.path.exists(tmp): os.unlink(tmp)


def changed_paths(wt, t1, t2):
    out = git('diff', '--no-renames', '--name-status', '-z', t1, t2, cwd=wt)
    parts = [p for p in out.split('\0') if p]
    return list(zip(parts[0::2], parts[1::2]))


def restore(wt, target, cur=None):
    """make the worktree's files equal to tree `target` (the index is not touched). Files are written from the
    blobs directly: `git restore`/`checkout` trust the index's stat data and can skip a file edited within the same
    second at the same size (seen with a two-line swap)."""
    cur = cur or snapshot(wt)
    if cur == target: return
    for s, p in changed_paths(wt, target, cur):
        full = os.path.join(wt, p)
        if s == 'A':
            os.unlink(full); continue
        mode, _, sha = git('ls-tree', target, '--', p, cwd=wt).split('\t')[0].split()
        data = subprocess.run(['git', 'cat-file', 'blob', sha], cwd=wt, capture_output=True, check=True).stdout
        if os.path.lexists(full) and (mode == '120000' or os.path.islink(full)): os.unlink(full)
        os.makedirs(os.path.dirname(full), exist_ok=True)
        if mode == '120000':
            os.symlink(data.decode(), full); continue
        with open(full, 'wb') as f: f.write(data)
        os.chmod(full, 0o755 if mode == '100755' else 0o644)
    now = snapshot(wt)
    if now != target:
        sys.exit(f'round.py: RESTORE FAILED: worktree is {now}, expected {target}; '
                 f'run: git -C {wt} restore --source={target} --worktree -- . (and remove files it lacks)')


def recover(st):
    p = os.path.join(scratch_dir(st['name']), 'try.pending.json')
    if os.path.exists(p):
        t = json.load(open(p))['tree']
        print(f'round.py: restoring the worktree from an interrupted try (tree {t[:10]})')
        restore(st['worktree'], t)
        os.unlink(p)


# ---------------------------------------------------------------- start

def cmd_start(a):
    name = a.name
    sd = scratch_dir(name)
    if os.path.exists(os.path.join(sd, 'round.json')) and not a.reuse:
        sys.exit(f'round.py: round {name} exists ({sd}); use --reuse to re-record its baseline')
    os.makedirs(sd, exist_ok=True)
    lock(name)
    repo = main_repo()
    wt = os.path.abspath(a.worktree or os.path.join(CFG['worktrees'], 'wt-' + name))
    branch = a.branch or 'round/' + name
    t0 = time.time()
    if os.path.exists(wt):
        if not a.reuse:
            sys.exit(f'round.py: {wt} exists; use --reuse to adopt it')
    else:
        git('worktree', 'add', wt, '-b', branch, a.base, cwd=repo)
    for k, rel in INPUTS:
        src, dst = CFG[k], os.path.join(wt, rel)
        if not os.path.exists(src):
            sys.exit(f'round.py: input {k} not found at {src} (set {SETTINGS[k][0]} or [round] {k} in the config)')
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        if os.path.islink(dst): os.unlink(dst)
        if not os.path.exists(dst): os.symlink(src, dst)
    dirty = git('status', '--porcelain', '--untracked-files=no', cwd=wt)
    if dirty:
        sys.exit(f'round.py: {wt} has uncommitted changes; the baseline must be recorded from a clean tree')
    tree = Tree(wt)
    head = git('rev-parse', 'HEAD', cwd=wt)
    keys = tree.keys()
    print(f'round {name}: worktree {wt} on {git("rev-parse", "--abbrev-ref", "HEAD", cwd=wt)} at {head[:10]}; '
          f'building {len(keys)} sources ({sum(tree.cached(k) is False for k in keys.values())} not cached)', flush=True)
    log = log_open(sd, 'start.log')
    bdir = os.path.join(sd, 'baseobj')
    if os.path.isdir(bdir): shutil.rmtree(bdir)
    os.makedirs(bdir)
    fails = []
    tick = lambda d, n: (d % 100 == 0) and print(f'  compiled {d}/{n}', flush=True)
    for rel, k, o, err in tree.build_many(sorted(keys.items()), progress=tick):
        if o:
            os.link(o, os.path.join(bdir, k + '.o'))
        else:
            fails.append(rel); log.write(f'COMPILE FAIL {rel}\n{err}\n')
    print(f'  built: {len(keys) - len(fails)} objects, {len(fails)} sources do not build here', flush=True)
    # objcmp views (base.json) for the old check.sh workflow
    sys.path.insert(0, os.path.join(wt, 'tools', 'cleanup'))
    try:
        import objcmp
        bj = {rel: objcmp.views(os.path.join(bdir, k + '.o'), objcmp.KINDS) if rel not in fails else 'COMPILE-FAIL'
              for rel, k in keys.items() if rel.startswith('src/game/')}
        json.dump(bj, open(os.path.join(sd, 'base.json'), 'w'))
    except Exception as e:  # noqa: BLE001
        print(f'  base.json not written: {e}')
    # names for telling renames from retargets
    defined = set()
    for rel, k in keys.items():
        if rel not in fails:
            defined |= objdiff.Obj(os.path.join(bdir, k + '.o')).defined_globals()
    srcs = symcheck.sources(wt)
    rep = symcheck.report(wt, {s: (os.path.join(bdir, keys[s] + '.o') if s in keys and s not in fails else None)
                               for s in srcs})
    json.dump(rep, open(os.path.join(sd, 'symcheck.base.json'), 'w'), indent=1)
    st = dict(name=name, worktree=wt, branch=branch, base_ref=a.base, base_commit=head,
              created=datetime.datetime.now().isoformat(timespec='seconds'),
              base_keys=keys, base_fail=fails, defined=sorted(defined),
              base_config=config_now(wt), progress_base=None)
    save_round(st)
    print(f'  symcheck: {symcheck.totals(rep)}', flush=True)
    if not a.no_progress:
        ok, line, broken = run_progress(wt, sd, 'progress.base.log', delete=True)
        st['progress_base'], st['progress_broken'] = line, broken
        pc = os.path.join(wt, 'build', 'progress_cache.json')
        if os.path.exists(pc): shutil.copyfile(pc, os.path.join(sd, 'progress_cache.base.json'))
        save_round(st)
        print(f'  progress: {"PASS" if ok else "FAIL"} {line}')
    print(f'round {name} ready in {time.time() - t0:.0f}s: cd {wt}; scratch {sd}')


# ---------------------------------------------------------------- check

def compare_objs(st, tree, items, against, known_old, known_new, stop_first=False):
    """items: [(rel, key)] to compile; against: {rel: object path or None (base failed) or missing}. Returns a list
    of dicts per source, in item order (when stop_first, only those compiled before the first difference)."""
    defined = set(st['defined'])
    res = {}

    def stop(rel, o):
        return stop_first and res[rel]['status'] not in ('IDENTICAL', 'RENAMED', 'BOTH-FAIL')

    order = {r: i for i, (r, _) in enumerate(items)}
    gen = tree.build_many(items, stop=stop)
    for rel, k, o, err in gen:
        b = against.get(rel, 'missing')
        r = dict(rel=rel, key=k, obj=o, err=err, base=b)
        if o is None and b is None: r['status'] = 'BOTH-FAIL'
        elif o is None: r['status'] = 'COMPILE-FAIL'
        elif b == 'missing': r['status'] = 'NEW'
        elif b is None: r['status'] = 'NOW-BUILDS'
        else:
            ob, on = objdiff.Obj(b), objdiff.Obj(o)
            cr = objdiff.compare(ob, on, known_old, known_new, defined)
            r['status'], r['cmp'] = cr.verdict, cr
            if cr.verdict == 'DIFF':
                r['first'] = objdiff.first_diff(ob, on, cr, tree.disasm)
        res[rel] = r
    return sorted(res.values(), key=lambda r: order[r['rel']])


def cmd_check(a):
    st = load_round(a.round)
    lock(st)
    recover(st)
    sd, wt = scratch_dir(st['name']), st['worktree']
    t0 = time.time()
    tree = Tree(wt)
    keys = tree.keys()
    bk = st['base_keys']
    affected = [(s, k) for s, k in keys.items() if k != bk.get(s)]
    removed = sorted(set(bk) - set(keys))
    edited = set(git('diff', '--name-only', st['base_commit'], '--', 'src', cwd=wt).split()) | \
        set(git('ls-files', '--others', '--exclude-standard', 'src', cwd=wt).split())
    affected.sort(key=lambda x: (x[0] not in edited, x[0]))
    cached = sum(tree.cached(k) is not False for _, k in affected)
    bdir = os.path.join(sd, 'baseobj')
    against = {s: (os.path.join(bdir, bk[s] + '.o') if s not in st['base_fail'] else None) for s in bk}
    cfg_changed = bool(git('diff', '--name-only', st['base_commit'], '--', 'config', cwd=wt))
    res = compare_objs(st, tree, affected, against, {k: v for k, v in st['base_config'].items()}, config_now(wt))
    log = log_open(sd, 'check.log')
    counts = {}
    for r in res:
        counts[r['status']] = counts.get(r['status'], 0) + 1
        log.write(f"{r['status']:12s} {r['rel']}\n")
        if r.get('cmp'):
            for x in r['cmp'].renames: log.write(f'    rename {x[0]} -> {x[1]}\n')
            for d in r['cmp'].diffs: log.write(f'    {d[0]} {d[1]}: {d[2]}\n')
        for l in r.get('first', []): log.write('    ' + l + '\n')
        if r['err']: log.write('    ' + r['err'].replace('\n', '\n    ')[:3000] + '\n')
    # matchtool on every function / data symbol of the affected objects
    nf = nok = 0
    first_mt = None
    for r in res:
        if not r['obj']: continue
        for n, ok, head, lines in tree.match_object(r['rel'], r['obj']):
            nf += 1; nok += ok
            if not ok:
                log.write(f"MATCHTOOL {r['rel']}: {head}\n" + ''.join('    ' + l + '\n' for l in lines))
                if not first_mt: first_mt = (r['rel'], head, mismatch_excerpt(lines))
    bad = [r for r in res if r['status'] in ('DIFF', 'COMPILE-FAIL')]
    lines = [f"round {st['name']}: HEAD {git('rev-parse', '--short', 'HEAD', cwd=wt)} (base {st['base_commit'][:9]}), "
             f"{len(affected)} sources affected ({sum(s in edited for s, _ in affected)} edited), "
             f"{len(affected) - cached} compiled, {time.time() - t0:.1f}s"
             + (f'; removed: {len(removed)}' if removed else '')]
    lines.append(f"  objects    {'FAIL' if bad else 'PASS'}  " + ', '.join(f'{v} {k.lower()}' for k, v in sorted(counts.items()))
                 if counts else '  objects    PASS  nothing changed since the baseline')
    lines.append(f"  matchtool  {'PASS' if nok == nf else 'FAIL'}  {nok}/{nf} symbols MATCH"
                 + ('; config changed since base: run --full' if cfg_changed and not a.full else ''))
    if bad:
        r = bad[0]
        lines.append(f"  first: {r['rel']} {r['status']}")
        if r['status'] == 'COMPILE-FAIL':
            lines += ['    ' + l for l in [l for l in r['err'].splitlines() if 'error' in l.lower() or re.search(r':\d+:', l)][:3]]
        else:
            lines += ['    ' + l for l in r.get('first', [])[:5]]
    elif first_mt:
        lines.append(f'  first: {first_mt[0]}: {first_mt[1]}')
        lines += first_mt[2][:4]
    failed = bool(bad) or nok != nf
    log.close()
    if a.full:
        failed |= full_checks(st, tree, keys, lines)
    lines.append(f'  logs: {os.path.join(sd, "logs")}')
    print('\n'.join(lines))
    return 1 if failed else 0


def run_progress(wt, sd, logname, delete=True, base_broken=None):
    """progress from a deleted cache; if more sources failed to build than in the baseline, one more run (which
    retries only the failed builds: a wibo timeout under load is transient) before calling it a failure."""
    r = _progress(wt, sd, logname, delete, base_broken)
    if not r[0] and base_broken is not None and r[2] > base_broken:
        r2 = _progress(wt, sd, logname.replace('.log', '.retry.log'), False, base_broken)
        return r2[0], r2[1] + f' (after retrying {r[2] - base_broken} failed builds)', r2[2]
    return r


def _progress(wt, sd, logname, delete=True, base_broken=None):
    """tools/progress.py (exit status 1 whenever a source does not build here, e.g. the SDK files that need Docker,
    so the pass rule is: N/N, matched.csv unchanged, no more unbuilt sources than the baseline).
    Returns (ok, summary line, number of sources that did not build)."""
    pc = os.path.join(wt, 'build', 'progress_cache.json')
    if delete and os.path.exists(pc): os.unlink(pc)
    with log_open(sd, logname) as f:
        p = subprocess.run([sys.executable, 'tools/progress.py'], cwd=wt, stdout=f, stderr=subprocess.STDOUT,
                           env=dict(os.environ, JOBS=CFG['jobs']))
    txt = open(os.path.join(sd, 'logs', logname)).read()
    m = re.findall(r'matched functions: (\d+)/(\d+)', txt)
    line = re.findall(r'matched functions: [^\n]*', txt)
    nb = re.findall(r'WARNING: (\d+) source files did not build', txt)
    broken = int(nb[-1]) if nb else 0
    unchanged = not git('diff', '--name-only', '--', 'config/matched.csv', cwd=wt)
    ok = bool(m) and m[-1][0] == m[-1][1] and unchanged and (base_broken is None or broken <= base_broken) and \
        (p.returncode == 0 or bool(nb))
    summary = (' '.join(line[-1].split()[:3]) if line else f'no total (exit {p.returncode})') + \
        f', {broken} sources not built here' + (f' (base {base_broken})' if base_broken not in (None, broken) else '') + \
        ('' if unchanged else '; config/matched.csv CHANGED')
    return ok, summary, broken


def full_checks(st, tree, keys, lines):
    sd, wt = scratch_dir(st['name']), st['worktree']
    failed = False

    def add(name, ok, detail, t):
        nonlocal failed
        failed |= not ok
        lines.append(f"  {name:10s} {'PASS' if ok else 'FAIL'}  {detail} [{t:.0f}s]")

    # symcheck vs baseline, from the cached objects
    t = time.time()
    srcs = symcheck.sources(wt)
    objs = {}
    for s in srcs:
        k = keys.get(s)
        c = tree.cached(k) if k else None
        if c is False: c, _ = tree.build(s, k)
        objs[s] = c or None
    rep = symcheck.report(wt, objs)
    json.dump(rep, open(os.path.join(sd, 'logs', 'symcheck.json'), 'w'), indent=1)
    reg = symcheck.regressions(rep, json.load(open(os.path.join(sd, 'symcheck.base.json'))))
    with log_open(sd, 'symcheck.log') as f:
        f.write(symcheck.totals(rep) + '\n' + ''.join(f'NEW {x}\n' for x in reg))
    add('symcheck', not reg, f'{len(reg)} new vs baseline' + (f' (first: {" ".join(reg[0])})' if reg else ''),
        time.time() - t)
    for name, cmd, ok_re in (('linkcheck', ['tools/linkcheck.py'], None),
                             ('docnames', ['tools/cleanup/docnames.py', '.'], None)):
        t = time.time()
        with log_open(sd, name + '.log') as f:
            p = subprocess.run([sys.executable] + cmd, cwd=wt, stdout=f, stderr=subprocess.STDOUT)
        tail = [l for l in open(os.path.join(sd, 'logs', name + '.log')).read().splitlines() if l.strip()]
        add(name, p.returncode == 0, tail[-1][:90] if tail else '', time.time() - t)
    t = time.time()
    ok, line, _ = run_progress(wt, sd, 'progress.log', delete=True, base_broken=st.get('progress_broken'))
    add('progress', ok, line, time.time() - t)
    t = time.time()
    with log_open(sd, 'link.log') as f:
        p = subprocess.run([sys.executable, 'tools/link.py'], cwd=wt, stdout=f, stderr=subprocess.STDOUT)
    txt = open(os.path.join(sd, 'logs', 'link.log')).read()
    lderr = re.findall(r'^ld\.lld: exit \d+, (\d+) errors', txt, re.M)
    m = re.search(r'^differing bytes by class:\n(.*?)(?:\n\n|\Z)', txt, re.M | re.S)
    ndiff = sum(int(x.split()[0]) for x in m.group(1).splitlines() if x.strip() and x.split()[0].isdigit()) if m else None
    sa = re.findall(r'symbols at their config address: (\d+) of (\d+)', txt)
    fb = re.findall(r'(\d+) in per-function fallback', txt)
    ok = p.returncode == 0 and lderr[-1:] == ['0'] and ndiff == 0 and bool(sa) and sa[-1][0] == sa[-1][1]
    add('link', ok, f"ld errors {lderr[-1] if lderr else '?'}, {ndiff if ndiff is not None else '?'} bytes differ, "
        f"symbols at config address {'/'.join(sa[-1]) if sa else '?'}, fallback {fb[-1] if fb else '?'}", time.time() - t)
    return failed


# ---------------------------------------------------------------- try

class Interrupted(Exception):
    pass


def cmd_try(a):
    st = load_round(a.round)
    lock(st)
    recover(st)
    sd, wt = scratch_dir(st['name']), st['worktree']
    t0 = time.time()
    tree = Tree(wt)
    pending = os.path.join(sd, 'try.pending.json')
    patch = None
    if a.stdin:
        patch = sys.stdin.read()
    elif a.patch:
        patch = open(a.patch).read()
    elif not a.command:
        sys.exit('round.py try: give PATCHFILE, --stdin, or -- COMMAND')
    pre_keys = tree.keys()
    t1 = snapshot(wt)
    json.dump({'tree': t1, 'started': time.time()}, open(pending, 'w'))

    def on_term(*_):
        raise Interrupted()
    old_term = signal.signal(signal.SIGTERM, on_term)
    restored = False
    try:
        if patch is not None:
            p = subprocess.run(['git', 'apply', '--whitespace=nowarn', f'-p{a.p}', '-'], cwd=wt, input=patch,
                               capture_output=True, text=True)
            if p.returncode:
                print('NOT APPLIED: ' + p.stderr.strip().splitlines()[0] if p.stderr.strip() else 'NOT APPLIED')
                return 2
        else:
            p = subprocess.run(a.command, cwd=wt)
            if p.returncode:
                print(f'EDIT FAILED: command exited {p.returncode}')
                return 2
        t2 = snapshot(wt)
        ch = changed_paths(wt, t1, t2)
        if not ch:
            print('NO CHANGE: the edit changed no file')
            return 0
        post_keys = tree.keys()
        aff = [(s, k) for s, k in post_keys.items() if k != pre_keys.get(s)]
        touched = {p for _, p in ch}
        aff.sort(key=lambda x: (x[0] not in touched, x[0]))
        cfg_old = st['base_config'] if a.vs == 'base' else None
        if a.vs == 'base':
            bk = st['base_keys']
            against = {s: (os.path.join(sd, 'baseobj', bk[s] + '.o') if s not in st['base_fail'] else None) for s in bk}
            aff = [(s, k) for s, k in post_keys.items() if k != bk.get(s)]
            aff.sort(key=lambda x: (x[0] not in touched, x[0]))
        else:
            missing = [(s, pre_keys[s]) for s, _ in aff if s in pre_keys and tree.cached(pre_keys[s]) is False]
            if missing:      # pre-edit objects not cached: build them from the snapshot, then come back
                restore(wt, t1, t2)
                tree.forget()
                list(tree.build_many(missing))
                restore(wt, t2, t1)
                tree.forget()
            against = {}
            for s, k in pre_keys.items():
                c = tree.cached(k)
                against[s] = c if c else None
            pre_cfg = {}
            for f in ('config/symbol_addrs.txt', 'config/functions.csv'):
                pre_cfg[f] = git('show', f'{t1}:{f}', cwd=wt)
            cfg_old = config_names((pre_cfg['config/symbol_addrs.txt'], pre_cfg['config/functions.csv']))
        cfg_new = config_now(wt)
        tree.config_changed()
        res = compare_objs(st, tree, aff, against, cfg_old, cfg_new, stop_first=not a.all)
        elapsed = time.time() - t0
        bad = [r for r in res if r['status'] not in ('IDENTICAL', 'RENAMED', 'BOTH-FAIL')]
        names = ', '.join(p for _, p in ch[:3]) + (f' (+{len(ch) - 3})' if len(ch) > 3 else '')
        with log_open(sd, 'try.log') as log:
            for r in res:
                log.write(f"{r['status']:12s} {r['rel']}\n")
                if r.get('cmp'):
                    for d in r['cmp'].diffs: log.write(f'    {d[0]} {d[1]}: {d[2]}\n')
                    for x in r['cmp'].renames: log.write(f'    rename {x[0]} -> {x[1]}\n')
                for l in r.get('first', []): log.write('    ' + l + '\n')
                if r['err']: log.write('    ' + r['err'][:3000] + '\n')
        out = []
        if not aff:
            out.append(f'IDENTICAL: edit of {names} changes no source or header a source includes [{elapsed:.1f}s]')
        elif not bad:
            ren = [x for r in res if r.get('cmp') for x in r['cmp'].renames]
            if ren:
                out.append(f'RENAMED: {len(res)} objects identical except symbol names ({len(ren)} renames, e.g. '
                           f'{ren[0][0]} -> {ren[0][1]}) [{elapsed:.1f}s]')
            else:
                out.append(f'IDENTICAL: {len(res)}/{len(aff)} objects ({names}) [{elapsed:.1f}s]')
            bf = sum(r['status'] == 'BOTH-FAIL' for r in res)
            if bf: out.append(f'  ({bf} affected sources do not build here before or after)')
        else:
            r = bad[0]
            out.append(f"{r['status']}: {r['rel']} (checked {len(res)}/{len(aff)} affected) [{elapsed:.1f}s]")
            if r['status'] == 'COMPILE-FAIL':
                el = [l for l in r['err'].splitlines() if 'error' in l.lower() or re.search(r':\d+:', l)] or r['err'].splitlines()
                out += ['  ' + l[:150] for l in el[:5]]
            elif r.get('first'):
                out += ['  ' + l for l in r['first'][:7]]
                fn = re.match(r'\S+ \S+ at (\S+)\+0x', r['first'][0])
                if fn and r['obj']:
                    for n, ok, head, lines in tree.match_object(r['rel'], r['obj']):
                        if n == fn.group(1):
                            out.append(f'  vs ELF: {head}'); break
        print('\n'.join(out))
        if a.keep:
            os.unlink(pending); restored = True
            print('  kept (edit left applied)')
        return 0 if not bad else 1
    except (KeyboardInterrupt, Interrupted):
        print('interrupted: restoring')
        return 130
    finally:
        signal.signal(signal.SIGTERM, old_term)
        if not restored:
            restore(wt, t1)
            if os.path.exists(pending): os.unlink(pending)


_LOCK = None


def lock(st_or_name):
    """one round.py command per round at a time (a try during a --full check would corrupt it)"""
    global _LOCK
    import fcntl
    name = st_or_name if isinstance(st_or_name, str) else st_or_name['name']
    os.makedirs(scratch_dir(name), exist_ok=True)
    _LOCK = open(os.path.join(scratch_dir(name), 'lock'), 'w')
    try:
        fcntl.flock(_LOCK, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except OSError:
        sys.exit(f'round.py: another round.py command is running on round {name}')


def cmd_status(a):
    st = load_round(a.round)
    sd = scratch_dir(st['name'])
    print(f"round {st['name']}: worktree {st['worktree']} branch {st['branch']}; base {st['base_ref']} = "
          f"{st['base_commit'][:10]} (recorded {st['created']}); scratch {sd}")
    print(f"  baseline: {len(st['base_keys'])} sources, {len(st['base_fail'])} do not build here; "
          f"progress {st.get('progress_base') or 'not run'}")
    if os.path.exists(os.path.join(sd, 'try.pending.json')):
        print('  an interrupted try is pending: the next command restores it')


def main():
    global CFG
    CFG = settings()
    os.environ.setdefault('SLY2_PYTHONPATH', CFG['pythonpath'])    # for matchtool here and in progress/link runs
    os.environ.setdefault('SLY2_M2C_INC', CFG['m2c_inc'])
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0], epilog=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('start'); p.add_argument('name')
    p.add_argument('--base', default='HEAD'); p.add_argument('--branch'); p.add_argument('--worktree')
    p.add_argument('--reuse', action='store_true'); p.add_argument('--no-progress', action='store_true')
    p = sub.add_parser('check'); p.add_argument('--full', action='store_true'); p.add_argument('--round')
    p = sub.add_parser('try'); p.add_argument('patch', nargs='?'); p.add_argument('--stdin', action='store_true')
    p.add_argument('--keep', action='store_true'); p.add_argument('--all', action='store_true')
    p.add_argument('--vs', choices=('pre', 'base'), default='pre'); p.add_argument('-p', type=int, default=1)
    p.add_argument('--round')
    p = sub.add_parser('status'); p.add_argument('--round')
    argv = sys.argv[1:]
    cmd = None
    if '--' in argv:     # try -- COMMAND...
        i = argv.index('--'); cmd = argv[i + 1:]; argv = argv[:i]
    a = ap.parse_args(argv)
    if a.cmd == 'try':
        a.command = cmd or None
        if a.patch and cmd: sys.exit('round.py try: give either PATCHFILE or -- COMMAND')
    return {'start': cmd_start, 'check': cmd_check, 'try': cmd_try, 'status': cmd_status}[a.cmd](a) or 0


if __name__ == '__main__':
    sys.exit(main())
