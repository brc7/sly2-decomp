#!/usr/bin/env python3
"""Native golden check: build the 64-bit port against a decomp tree, run a fixed scenario, diff the state stream.

  golden.py check  [--decomp REF | --tree DIR] [--long] [--golden DIR] [--port REF] [--keep-stream]
  golden.py record  --decomp REF | --tree DIR  [--long] [--port REF] [--golden DIR] [--force]
  golden.py build  [--decomp REF | --tree DIR] [--port REF]
  golden.py status [--long] [--golden DIR]
(also as `round.py golden ...`, and `round.py check --native` / `try --native` for a round's worktree)

The PS2 checks of round.py prove the matching build is byte-identical; they cannot see the native code paths (#else /
!SLY2_EE_LAYOUT branches, C fallbacks for asm, type changes that move host layouts). This tool builds the port
(sly2-port) with its `decomp/` replaced by the candidate tree, runs the --frames harness with a fixed pad script and the
fixed virtual clock (SLY2_DT_REPLAY=fixed, ASLR off), records the per-frame state stream (port tools/state, schema
v2) and compares it with a golden stream recorded from a known-good decomp commit, using the port's diff engine
(tools/state/diff.py) and the schema tolerances.

The port commit is part of the golden: `check` always builds the port commit the golden was recorded with, so the only
variable is the decomp (`check --port REF` overrides it and flags the report). A port commit builds the decomp version
it pins and rounds on top of it, not older versions and not newer ones that change lines it patches ("the port
window"). Re-record after a port bump (record --decomp <its pin> --port REF --force) or after an intentional
behaviour change. Scenarios: default (5300 frames: intro select, the intro relay, run / jump / walk from frame 4400)
and --long (12000 frames: the intro, then boot-15's soak pad loop P3); each has its own golden directory.

Steps of a build (each timed; logs in <cache>/logs/):
  port     `git archive` of the port commit into <cache>/port (file times = commit time, so a re-extract does not
           invalidate the build); kept while the commit is the same
  decomp   <cache>/port/decomp mirrors include/ src/ config/ of the candidate (a commit, or a worktree's tracked and
           untracked files); only files whose content differs are rewritten, so ninja rebuilds what changed
  gen      gen_compat_asserts, gen_layout_maps, gen_overrides (stale optional hunks and MISSING required hunks are
           reported), state gen_offsets --abi host; outputs whose content did not change keep their file times
  build    cmake, ninja (game objects), gen_sym_aliases, ninja sly2
  run      the scenario (needs SLY2_ISO; without it the run and diff are skipped and the result is BUILD-ONLY)
  diff     tools/state/diff.py against <golden>/stream.jsonl

Settings: environment variable, then the [native] section of the round config ($SLY2_ROUND_CONF, default
~/.config/sly2/round.conf), then the default:
  cache   SLY2_NATIVE_CACHE    {scratch}/native-golden/cache
  golden  SLY2_NATIVE_GOLDEN   {scratch}/native-golden/golden
  port    SLY2_PORT_REPO       /home/claude/work/sly2-port      (record --port REF picks the commit; default port_ref)
  port_ref SLY2_PORT_REF       claude/boot-15
  iso     SLY2_ISO             (unset: build only)
  prefix  SLY2_DEPS_PREFIX     /home/claude/work/deps/prefix    (CMAKE_PREFIX_PATH: SDL3 and friends)
  jobs    JOBS                 2
Exit status: 0 PASS / BUILD-ONLY, 1 FAIL (divergence, crash, short stream, build failure), 2 usage / setup error.
Stale and MISSING port hunks are reported, not fatal (the build goes on without MISSING ones).
"""
import argparse
import configparser
import datetime
import fcntl
import hashlib
import json
import math
import os
import re
import shutil
import signal
import subprocess
import sys
import tarfile
import time
from collections import deque

HERE = os.path.dirname(os.path.abspath(__file__))

# The scenario. Versioned: the golden records the scenario it was made with and `check` replays that one.
# Pad script: port/hal/stub/pad.cpp syntax (from-to:hexmask[@lx/ly/rx/ry], scePadRead numbers; sticks make the pad a
# DualShock). Masks: 0x40 CROSS (jump), 0x80 SQUARE, 0x20 CIRCLE, 0x10 TRIANGLE. 128 = stick centre, ly 0 = forward.
#   1395-1396 CROSS        the intro select (boot-11's 1395-1396:40); the intro relay then walks Sly to the pipe,
#                          jumps (~1511), climbs, crosses the roof and hands over to the pad at ~4000
#   4400-...               run forward, jump + double jump while running, slow walk, run right with a jump, run left
SCENARIO = {
    'version': 1,
    'level': '353856:10938886:0x41',          # Paris (tools/state/native_stream.py PARIS)
    'frames': 5300,
    'pad_script': ('1395-1396:40,'
                   '4400-4600:0@128/0,4460-4466:40@128/0,4480-4486:40@128/0,'
                   '4650-4800:0@128/60,'
                   '4850-5000:0@255/128,4900-4906:40@255/128,'
                   '5050-5200:0@0/128'),
    'dt': 'fixed',                             # SLY2_DT_REPLAY=fixed: every frame 1/60 s, wall clock never read
    'env': {'SLY2_UPDATE_SCREENS': '1'},       # GameMain's UpdateScreens (menus, wipes, HUD), as the boot soaks run
    'objects_every': 300,                      # object lists every 300 records (each ~0.5 MB)
    'aslr': False,                             # setarch -R (native_stream.py's default)
}

# --long: boot-15's soak pad loop P3 (scratch b14/pads.sh) after the same intro, looped from read 4400 with a period
# of 1800 reads, 12000 frames: reaches the paths boot-15 found v1.29 native bugs on (guardbhv.cpp VuScaleByX #else
# recursion, a stack overflow near frame 8000; grab.cpp AddFlags reading the flag / pointer word as 32 bits).
SCENARIO_LONG = {
    'version': 1,
    'name': 'long',
    'level': SCENARIO['level'],
    'frames': 12000,
    'pad_script': ('1395-1396:40,4400-4500:0@255/128,4500-4504:80@255/128,4520-4524:80@255/128,'
                   '4540-4544:20@255/128,4600-4800:04@0/128,4800-4810:40@0/0,4812-4818:20@0/0,4830-4836:40@0/0,'
                   '4900-5200:0@60/0,4950-4954:80@60/0,5000-5004:20@60/0,5100-5104:10,5200-5300:08@128/0,'
                   '5300-5500:0@128/255,5320-5326:40@128/255,5340-5346:40@128/255,5348-5354:20@128/255,'
                   '5600-5800:0@255/255,5810-5816:80,5900-6100:0@0/255,6150-6156:20'),
    'dt': 'fixed',
    'env': {'SLY2_UPDATE_SCREENS': '1', 'SLY2_PAD_LOOP': '4400:1800'},
    'objects_every': 600,
    'aslr': False,
}
SCENARIO['name'] = 'default'

DEFAULTS = {
    'scratch':  ('SLY2_ROUND_SCRATCH', '/home/claude/work/scratch'),
    'cache':    ('SLY2_NATIVE_CACHE', '{scratch}/native-golden/cache'),
    'golden':   ('SLY2_NATIVE_GOLDEN', '{scratch}/native-golden/golden'),
    'port':     ('SLY2_PORT_REPO', '/home/claude/work/sly2-port'),
    'port_ref': ('SLY2_PORT_REF', 'claude/boot-15'),
    'iso':      ('SLY2_ISO', ''),
    'prefix':   ('SLY2_DEPS_PREFIX', '/home/claude/work/deps/prefix'),
    'jobs':     ('JOBS', '2'),
}
MIRROR_DIRS = ('include', 'src', 'config')
# Generated outputs inside the port copy whose file times are restored when a generator rewrites them unchanged.
GEN_PATHS = ('port/overrides', 'port/compat/generated', 'tools/state/offsets_host.json', 'build/layout_64_map.json')
MIN_FREE_GB = 2.0


def settings():
    cp = configparser.ConfigParser()
    cp.read(os.environ.get('SLY2_ROUND_CONF', os.path.expanduser('~/.config/sly2/round.conf')))
    sec = cp['native'] if cp.has_section('native') else {}
    rsec = cp['round'] if cp.has_section('round') else {}
    out = {}
    for k, (env, dflt) in DEFAULTS.items():
        out[k] = os.environ.get(env) or sec.get(k) or (rsec.get(k) if k == 'scratch' else None) or dflt
    for k in out:
        out[k] = out[k].replace('{scratch}', out['scratch'])
    return out


CFG = None


def sh(cmd, cwd=None, log=None, env=None, check=False, inp=None):
    """run; output to the log file object (or captured). Returns (returncode, captured text or '')."""
    if log is not None:
        log.write(f'\n$ {" ".join(cmd) if isinstance(cmd, list) else cmd}  (cwd {cwd})\n'); log.flush()
        p = subprocess.run(cmd, cwd=cwd, stdout=log, stderr=subprocess.STDOUT, env=env, input=inp)
        out = ''
    else:
        p = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, env=env, input=inp)
        out = p.stdout + p.stderr
    if check and p.returncode:
        raise SetupError(f'{cmd[0] if isinstance(cmd, list) else cmd} exited {p.returncode}' + (f': {out[-500:]}' if out else ''))
    return p.returncode, out


class SetupError(Exception):
    pass


def free_gb(path):
    st = os.statvfs(path)
    return st.f_bavail * st.f_frsize / 2 ** 30


def blob_sha(data):
    return hashlib.sha1(b'blob %d\0' % len(data) + data).hexdigest()


def git_out(repo, *args):
    p = subprocess.run(['git', '-C', repo, *args], capture_output=True, text=True)
    if p.returncode:
        raise SetupError(f'git {" ".join(args)}: {p.stderr.strip()}')
    return p.stdout.strip()


# ------------------------------------------------------------------------------------------------ the cache

class Cache:
    def __init__(self, root):
        self.root = root
        self.port = os.path.join(root, 'port')
        self.decomp = os.path.join(self.port, 'decomp')
        self.build = os.path.join(self.port, 'build', 'cm')
        self.logs = os.path.join(root, 'logs')
        os.makedirs(self.logs, exist_ok=True)
        self.state_path = os.path.join(root, 'cache.json')
        self.state = json.load(open(self.state_path)) if os.path.exists(self.state_path) else {}
        self._lock = open(os.path.join(root, 'lock'), 'w')
        try:
            fcntl.flock(self._lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except OSError:
            raise SetupError(f'another golden.py run holds {root}/lock')

    def save(self):
        json.dump(self.state, open(self.state_path + '.tmp', 'w'), indent=1)
        os.replace(self.state_path + '.tmp', self.state_path)

    def log(self, name):
        return open(os.path.join(self.logs, name), 'w', errors='replace')

    # -- port source
    def ensure_port(self, repo, commit):
        if self.state.get('port_commit') == commit and os.path.isdir(os.path.join(self.port, 'tools')):
            return False
        if os.path.isdir(self.port):
            shutil.rmtree(self.port)
        os.makedirs(self.port)
        p = subprocess.Popen(['git', '-C', repo, 'archive', '--format=tar', commit], stdout=subprocess.PIPE)
        with tarfile.open(fileobj=p.stdout, mode='r|') as tf:
            tf.extractall(self.port, filter='tar') if hasattr(tarfile, 'tar_filter') else tf.extractall(self.port)
        if p.wait():
            raise SetupError(f'git archive {commit} failed')
        self.state = {'port_commit': commit}
        self.save()
        return True

    # -- decomp mirror
    def sync_decomp(self, source):
        """source: ('commit', repo, sha) or ('tree', dir). Returns (n written, n deleted, changed paths)."""
        want = {}       # rel -> blob sha
        data = {}
        if source[0] == 'commit':
            _, repo, sha = source
            for line in git_out(repo, 'ls-tree', '-r', '--full-tree', sha, '--', *MIRROR_DIRS).splitlines():
                meta, rel = line.split('\t', 1)
                mode, typ, bsha = meta.split()
                if typ == 'blob':
                    want[rel] = bsha
        else:
            d = source[1]
            files = subprocess.run(['git', '-C', d, 'ls-files', '-z', '-co', '--exclude-standard', '--', *MIRROR_DIRS],
                                   capture_output=True, check=True).stdout.decode().split('\0')
            for rel in files:
                if not rel:
                    continue
                full = os.path.join(d, rel)
                if os.path.islink(full) or not os.path.isfile(full):
                    continue
                b = open(full, 'rb').read()
                want[rel] = blob_sha(b)
                data[rel] = b
        have = self.state.get('decomp_manifest', {})
        if not os.path.isdir(self.decomp):
            have = {}
        os.makedirs(self.decomp, exist_ok=True)
        todo = [r for r, s in want.items() if have.get(r) != s]
        if source[0] == 'commit' and todo:
            p = subprocess.run(['git', '-C', source[1], 'cat-file', '--batch'], input=''.join(want[r] + '\n' for r in todo).encode(),
                               capture_output=True, check=True)
            buf, pos = p.stdout, 0
            for r in todo:
                nl = buf.index(b'\n', pos)
                size = int(buf[pos:nl].split()[2])
                data[r] = buf[nl + 1:nl + 1 + size]
                pos = nl + 1 + size + 1
        changed = []
        for r in todo:
            full = os.path.join(self.decomp, r)
            os.makedirs(os.path.dirname(full), exist_ok=True)
            if os.path.lexists(full):
                os.unlink(full)
            with open(full, 'wb') as f:
                f.write(data[r])
            changed.append(full)
        gone = [r for r in have if r not in want]
        for r in gone:
            full = os.path.join(self.decomp, r)
            if os.path.lexists(full):
                os.unlink(full)
            changed.append(full)
        self.state['decomp_manifest'] = want
        self.save()
        return len(todo), len(gone), changed


def snapshot_files(root, rels):
    snap = {}
    for rel in rels:
        p = os.path.join(root, rel)
        if os.path.isfile(p):
            items = [p]
        elif os.path.isdir(p):
            items = [os.path.join(d, f) for d, _, fs in os.walk(p) for f in fs]
        else:
            items = []
        for f in items:
            st = os.stat(f)
            snap[f] = (hashlib.sha1(open(f, 'rb').read()).hexdigest(), st.st_mtime_ns)
    return snap


def restore_times(before, root, rels):
    """generated files rewritten with the same content get their old file time back; returns changed paths"""
    after = snapshot_files(root, rels)
    changed = []
    for f, (h, _) in after.items():
        b = before.get(f)
        if b and b[0] == h:
            os.utime(f, ns=(b[1], b[1]))
        else:
            changed.append(f)
    changed += [f for f in before if f not in after]
    return changed


# ------------------------------------------------------------------------------------------------ build

class Result:
    def __init__(self):
        self.timings = []        # (step, seconds)
        self.stale = []          # (file, snippet)
        self.missing = []
        self.build_error = None  # (step, lines)
        self.notes = []
        self.changed = 0
        self.offsets_stale = False
        self.missing_roots = []

    def timed(self, step, t0):
        self.timings.append((step, time.time() - t0))


def parse_gen_overrides(text, res):
    for line in text.splitlines():
        m = re.match(r"gen_overrides: stale optional hunk in (\S+): (.*)", line)
        if m:
            res.stale.append((m.group(1), m.group(2)))
        m = re.match(r"gen_overrides: MISSING required hunk in (\S+): (.*)", line)
        if m:
            res.missing.append((m.group(1), m.group(2)))


def first_errors(path, n=4):
    """the first compiler / tool error lines of a log"""
    out = []
    for line in open(path, errors='replace'):
        if re.search(r'\berror\b|FAILED:|Error|Traceback|MISSING', line) and 'warning' not in line:
            out.append(line.rstrip()[:200])
            if len(out) >= n:
                break
    return out


def build(cache, source, res):
    """prepare and build; returns the sly2 path or None (res.build_error set)"""
    py = sys.executable
    port, decomp = cache.port, cache.decomp
    t = time.time()
    n_w, n_d, changed = cache.sync_decomp(source)
    res.changed = n_w + n_d
    res.timed('decomp', t)
    res.notes.append(f'decomp mirror: {n_w} files written, {n_d} removed')

    # generators (content-preserving file times)
    t = time.time()
    before = snapshot_files(port, GEN_PATHS)
    with cache.log('gen.log') as log:
        for script in ('tools/gen_compat_asserts.py', 'tools/gen_layout_maps.py'):
            rc, _ = sh([py, script], cwd=port, log=log)
            if rc:
                res.build_error = (script, first_errors(log.name))
                res.timed('gen', t)
                return None
    rc, out = sh([py, 'tools/gen_overrides.py'], cwd=port)
    open(os.path.join(cache.logs, 'gen_overrides.log'), 'w').write(out)
    parse_gen_overrides(out, res)
    if rc and not (res.missing and 'files written' in out):
        res.build_error = ('gen_overrides', [l for l in out.splitlines() if 'MISSING' in l or 'Error' in l][:4]
                           or out.splitlines()[-4:])
        res.timed('gen', t)
        return None
    # MISSING required hunks: gen_overrides has written every file (it exits 1 at the end). The build goes on without
    # them: a required hunk that the decomp absorbed (a landed port request) is simply covered; one that is not shows
    # up as a compile error or in the scenario. Either way they are reported.
    # state offsets for this decomp's headers (host ABI only: the EE side is not needed natively)
    with cache.log('gen_offsets.log') as log:
        rc, _ = sh([py, 'tools/state/gen_offsets.py', '--abi', 'host'], cwd=port, log=log)
    if rc:
        res.notes.append('gen_offsets FAILED (schema path renamed in the decomp?): using the port commit\'s '
                         'offsets_host.json; see logs/gen_offsets.log')
        res.offsets_stale = True
        blob = subprocess.run(['git', '-C', CFG['port'], 'show', f'{cache.state["port_commit"]}:tools/state/offsets_host.json'],
                              capture_output=True, check=True).stdout
        open(os.path.join(port, 'tools/state/offsets_host.json'), 'wb').write(blob)
    gen_changed = restore_times(before, port, GEN_PATHS)
    changed += gen_changed
    res.timed('gen', t)

    # cmake + ninja
    t = time.time()
    with cache.log('build.log') as log:
        if not os.path.exists(os.path.join(cache.build, 'CMakeCache.txt')):
            rc, _ = sh(['cmake', '-S', port, '-B', cache.build, '-G', 'Ninja', '-DCMAKE_BUILD_TYPE=Debug',
                        '-DCMAKE_C_COMPILER=clang', '-DCMAKE_CXX_COMPILER=clang++',
                        f'-DCMAKE_PREFIX_PATH={CFG["prefix"]}', '-DSLY2_BUILD_TESTS=OFF', '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'], log=log)
        else:
            rc, _ = sh(['cmake', cache.build], log=log)      # the source list depends on which overrides exist
        if rc:
            res.build_error = ('cmake', first_errors(log.name))
            return None
        jobs = CFG['jobs']
        rc, _ = sh(['ninja', '-C', cache.build, '-j', jobs, '-k', '0',
                    'sly2_game', 'sly2_link_support', 'sly2_hal_stub', 'sly2_iso'], log=log)
        if rc:
            failed = [l.split()[1] for l in open(log.name, errors='replace') if l.startswith('FAILED: ')]
            res.build_error = (f'compile ({len(failed)} targets failed: '
                               + ', '.join(os.path.basename(f).replace('.o', '') for f in failed[:6])
                               + (', ...' if len(failed) > 6 else '') + ')',
                               [l for l in first_errors(log.name, 12) if 'error:' in l][:3])
            res.timed('build', t)
            return None
    # gen_sym_aliases' AST cache keys TUs by source mtime only: drop TUs whose dependencies changed
    drop_stale_ast(cache, changed)
    with cache.log('sym_aliases.log') as log:
        before = snapshot_files(port, ('port/compat/generated',))
        rc, _ = sh([py, 'tools/gen_sym_aliases.py'], cwd=port, log=log)
        restore_times(before, port, ('port/compat/generated',))
        if rc:
            res.build_error = ('gen_sym_aliases', first_errors(log.name))
            return None
    with open(os.path.join(cache.logs, 'build.log'), 'a') as log:
        rc, _ = sh(['ninja', '-C', cache.build, '-j', CFG['jobs'], 'sly2'], log=log)
        if rc:
            res.build_error = ('link', first_errors(log.name, 6))
            return None
    res.timed('build', t)
    return os.path.join(cache.build, 'sly2')


def drop_stale_ast(cache, changed):
    ast = os.path.join(cache.port, 'build', 'ast_syms_elf.json')
    if not os.path.exists(ast) or not changed:
        return
    changed = {os.path.realpath(c) for c in changed}
    p = subprocess.run(['ninja', '-C', cache.build, '-t', 'deps'], capture_output=True, text=True)
    stale_objs, cur, hit = set(), None, False
    for line in p.stdout.splitlines() + ['']:
        if line and not line.startswith(' '):
            if cur and hit:
                stale_objs.add(cur)
            cur, hit = line.split(':')[0], False
        elif line.strip():
            dep = line.strip()
            if not os.path.isabs(dep):
                dep = os.path.join(cache.build, dep)
            if os.path.realpath(dep) in changed:
                hit = True
    if cur and hit:
        stale_objs.add(cur)
    data = json.load(open(ast))
    drop = []
    for rel in list(data):
        tail = rel + '.o'
        if any(o.endswith(tail) for o in stale_objs):
            drop.append(rel)
    for rel in drop:
        del data[rel]
    json.dump(data, open(ast, 'w'))


# ------------------------------------------------------------------------------------------------ run and diff

def run_scenario(cache, exe, scenario, out, res):
    """the harness with the state stream; returns (records, harness exit status, last log lines)"""
    sys.path.insert(0, os.path.join(cache.port, 'tools', 'state'))
    import native_stream
    plan, missing = native_stream.make_plan(exe, os.path.join(cache.port, 'tools/state/offsets_host.json'))
    planf = out + '.plan'
    open(planf, 'w').write(plan)
    env = {k: v for k, v in os.environ.items() if not k.startswith('SLY2_') or k == 'SLY2_ISO'}
    env.update(scenario.get('env', {}))
    env.update(SLY2_ISO=CFG['iso'], SLY2_STATE_STREAM=out, SLY2_STATE_PLAN=planf, SLY2_STATE_EVERY='1',
               SLY2_STATE_OBJECTS_EVERY=str(scenario['objects_every']), SLY2_STATE_FROM='0',
               SLY2_PAD_SCRIPT=scenario['pad_script'], SLY2_DT_REPLAY=scenario['dt'])
    cmd = [exe, '--load-level', scenario['level'], '--frames', str(scenario['frames'])]
    if not scenario.get('aslr') and sys.platform.startswith('linux'):
        cmd = ['setarch', '-R'] + cmd
    tail, nlines = deque(maxlen=400), 0
    caught = []
    p = subprocess.Popen(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=cache.root)
    for raw in p.stdout:
        nlines += 1
        line = raw.decode('latin-1').rstrip()
        if not line.startswith('[ee-asm]'):
            tail.append(line)
            if 'Caught signal' in line or (caught and len(caught) < 8):
                caught.append(line)
    rc = p.wait()
    open(os.path.join(cache.logs, 'run.log'), 'w').write(f'$ {" ".join(cmd)}\n{nlines} lines; last {len(tail)} '
                                                          f'(without [ee-asm]):\n' + '\n'.join(tail) + '\n')
    os.unlink(planf)
    n = sum(1 for _ in open(out)) if os.path.exists(out) else 0
    res.missing_roots = missing
    return n, rc, caught or list(tail)[-6:]


def fmt(v):
    if isinstance(v, float):
        return f'{v:.6g}'
    if isinstance(v, list):
        return '[' + ', '.join(fmt(x) for x in v[:4]) + (', ...' if len(v) > 4 else '') + ']'
    if isinstance(v, str) and len(v) > 24:
        return v[:24] + '...'
    return str(v)


def diff_streams(cache, golden_stream, cand_stream):
    sys.path.insert(0, os.path.join(cache.port, 'tools', 'state'))
    import diff as sdiff
    fmeta, ometa = sdiff.load_meta(os.path.join(cache.port, 'tools/state/offsets_host.json'))
    a, b = sdiff.load_stream(golden_stream), sdiff.load_stream(cand_stream)
    d = sdiff.run(a, b, 'start', None, fmeta, ometa)
    with open(os.path.join(cache.logs, 'diff.txt'), 'w') as f:
        d.report(out=f)
        if d.only_a or d.only_b:
            f.write(f'\nkeys only in golden: {len(d.only_a)}, only in candidate: {len(d.only_b)}\n')
    json.dump(d.to_json(), open(os.path.join(cache.logs, 'diff.json'), 'w'), indent=1)
    return d, len(a), len(b)


def summarize_diff(d, n_gold, n_cand, lines):
    """<= ~8 lines: first divergence plus the fields that diverged, earliest first"""
    if d.first is not None:
        key, what = d.first
        desc = ', '.join(f'{k} {fmt(v)}' for k, v in what.items() if k not in ('detail',) and v not in ('', None))
        lines.append(f'  first divergence: frame {key}: {desc}')
    rows = sorted(((st['first']['key'], name, st) for name, st in d.fields.items() if st['diverged']),
                  key=lambda x: (x[0], x[1]))
    if rows:
        lines.append(f'  {"field":20} {"first@":>6} {"div":>5}/{"of":<5} {"max err":>9}  golden | candidate at first')
        for key, name, st in rows[:6]:
            fd = st['first']
            me = 'inf' if math.isinf(st['max_err']) else f'{st["max_err"]:.3g}'
            lines.append(f'  {name:20} {key:>6} {st["diverged"]:>5}/{st["compared"]:<5} {me:>9}  '
                         f'{fmt(fd["a"])} | {fmt(fd["b"])}')
        if len(rows) > 6:
            lines.append(f'  ... {len(rows) - 6} more diverged fields: ' + ', '.join(r[1] for r in rows[6:12]))
    bad_cls = [(st['first']['key'] if st['first'] else 1 << 30, cid) for cid, st in d.cls.items()
               if st['only_a'] or st['only_b'] or st['field_div']]
    if bad_cls:
        lines.append(f'  objects: {len(bad_cls)} of {len(d.cls)} classes differ (first at frame {min(bad_cls)[0]}, '
                     f'cid {min(bad_cls)[1]})')


# ------------------------------------------------------------------------------------------------ commands

def resolve_source(a, default_tree=None):
    """('commit', repo, sha) / ('tree', dir) and a label"""
    if getattr(a, 'tree', None) or (default_tree and not getattr(a, 'decomp', None)):
        d = os.path.realpath(a.tree or default_tree)
        head = git_out(d, 'rev-parse', 'HEAD')
        dirty = bool(git_out(d, 'status', '--porcelain', '--', *MIRROR_DIRS))
        return ('tree', d), dict(kind='tree', path=d, head=head, dirty=dirty,
                                 label=f'{d} @ {head[:10]}' + (' + uncommitted changes' if dirty else ''))
    repo = getattr(a, 'repo', None) or default_repo()
    ref = a.decomp
    sha = git_out(repo, 'rev-parse', ref + '^{commit}')
    return ('commit', repo, sha), dict(kind='commit', ref=ref, commit=sha, label=f'{ref} ({sha[:10]})')


def default_repo():
    p = subprocess.run(['git', 'rev-parse', '--path-format=absolute', '--git-common-dir'], cwd=HERE,
                       capture_output=True, text=True)
    return os.path.dirname(p.stdout.strip()) if p.returncode == 0 else os.getcwd()


def golden_dir(a):
    g = a.golden or CFG['golden']
    return g + '-long' if getattr(a, 'long', False) and not a.golden else g


def golden_meta(gdir):
    p = os.path.join(gdir, 'meta.json')
    return json.load(open(p)) if os.path.exists(p) else None


def prepare(a, port_commit, source, res, lines):
    os.makedirs(CFG['cache'], exist_ok=True)
    if free_gb(CFG['cache']) < MIN_FREE_GB:
        raise SetupError(f'less than {MIN_FREE_GB} GB free on the cache file system; free space first')
    cache = Cache(CFG['cache'])
    t = time.time()
    if cache.ensure_port(CFG['port'], port_commit):
        res.notes.append(f'port {port_commit[:10]} extracted (full build follows)')
    res.timed('port', t)
    exe = build(cache, source, res)
    return cache, exe


def report_build(res, lines):
    if res.missing:
        files = sorted({os.path.basename(f) for f, _ in res.missing})
        lines.append(f'  missing hunks: {len(res.missing)} required port hunks no longer match (absorbed by the decomp, '
                     f'e.g. a landed port request, or broken by it) in {", ".join(files[:5])}'
                     + (f' (+{len(files) - 5})' if len(files) > 5 else '') + '; built without them; logs/gen_overrides.log')
    if res.stale:
        files = sorted({f for f, _ in res.stale})
        lines.append(f'  stale hunks: {len(res.stale)} optional hunks no longer match (decomp changed under a port '
                     f'workaround) in {", ".join(os.path.basename(f) for f in files[:5])}'
                     + (f' (+{len(files) - 5})' if len(files) > 5 else '') + '; list: logs/gen_overrides.log')
    if res.build_error:
        step, errs = res.build_error
        lines.append(f'  build      FAIL  at {step}')
        lines += ['    ' + e[:150] for e in errs[:4]]


def timing_line(res, t0):
    return '  time: ' + ', '.join(f'{s} {v:.0f}s' for s, v in res.timings) + f'; total {time.time() - t0:.0f}s'


def cmd_check(a, default_tree=None):
    t0 = time.time()
    gdir = golden_dir(a)
    meta = golden_meta(gdir)
    if not meta:
        print(f'native golden: no golden at {gdir} (golden.py record --decomp <known-good ref>)')
        return 2
    source, info = resolve_source(a, default_tree)
    res = Result()
    lines = [f'native golden: {info["label"]} vs golden {meta["decomp"]["label"]} '
             f'(port {meta["port_commit"][:10]}, scenario {meta["scenario"].get("name", "default")} '
             f'v{meta["scenario"]["version"]}, {meta["scenario"]["frames"]} frames)']
    port_commit = meta['port_commit']
    if getattr(a, 'port', None):
        port_commit = git_out(CFG['port'], 'rev-parse', a.port + '^{commit}')
        if port_commit != meta['port_commit']:
            n = git_out(CFG['port'], 'rev-list', '--count', f'{meta["port_commit"]}...{port_commit}')
            lines.append(f'  WARNING: port {a.port} ({port_commit[:10]}) is not the golden\'s port '
                         f'({meta["port_commit"][:10]}; {n} commits apart): differences may come from the port')
    try:
        cache, exe = prepare(a, port_commit, source, res, lines)
    except SetupError as e:
        print(lines[0] + f'\n  SETUP ERROR: {e}')
        return 2
    report_build(res, lines)
    if not exe:
        lines.insert(1, '  RESULT     FAIL  (build)')
        lines.append(timing_line(res, t0))
        lines.append(f'  logs: {cache.logs}')
        print('\n'.join(lines))
        return 1
    if not CFG['iso'] or not os.path.exists(CFG['iso']):
        lines.insert(1, '  RESULT     BUILD-ONLY  (SLY2_ISO not set: scenario run and diff skipped)')
        lines.append(timing_line(res, t0))
        print('\n'.join(lines))
        return 0
    t = time.time()
    out = os.path.join(cache.root, 'candidate.jsonl')
    if os.path.exists(out):
        os.unlink(out)
    n, rc, tail = run_scenario(cache, exe, meta['scenario'], out, res)
    res.timed('run', t)
    t = time.time()
    d, n_gold, n_cand = diff_streams(cache, os.path.join(gdir, 'stream.jsonl'), out)
    res.timed('diff', t)
    crashed = rc != 0
    short = n_cand < n_gold
    ok = not d.diverged and not crashed and not short
    verdict = 'PASS' if ok else 'FAIL'
    how = (f'killed by {signal.Signals(-rc).name}' if rc < 0 else f'exit {rc}') if crashed else ''
    detail = (f'{len(d.keys)} frames identical within tolerance' if ok else
              f'{"harness " + how + " after " + str(n_cand) + " records; " if crashed else ""}'
              f'{"stream short (" + str(n_cand) + "/" + str(n_gold) + "); " if short and not crashed else ""}'
              f'{"diverged" if d.diverged else "no divergence before the end"}')
    lines.insert(1, f'  RESULT     {verdict}  {detail}')
    if crashed:
        caught = [l for l in tail if 'Caught signal' in l or re.match(r'\s*#0[2-5] ', l)]
        lines += ['    ' + l.strip()[:150] for l in caught[:4]] or \
            ['    (no signal handler output: a stack overflow kills the process before the handler runs)']
    if d.diverged:
        summarize_diff(d, n_gold, n_cand, lines)
    for nte in res.notes:
        if 'FAILED' in nte:
            lines.append('  note: ' + nte)
    new_missing = sorted(set(res.missing_roots) - set(meta.get('missing_roots', [])))
    if new_missing:
        lines.append(f'  note: schema roots without a host symbol (not in the golden run): {", ".join(new_missing)}')
    gl = meta.get('offsets_sha')
    if gl and gl != file_sha(os.path.join(cache.port, 'tools/state/offsets_host.json')):
        lines.append('  note: host offsets of the schema fields differ from the golden\'s (a host layout changed; '
                     'fields are compared by name)')
    lines.append(timing_line(res, t0))
    lines.append(f'  logs: {cache.logs} (diff.txt: full per-field table)')
    if a.keep_stream:
        keep = os.path.join(cache.logs, 'candidate.jsonl')
        shutil.move(out, keep)
    elif os.path.exists(out):
        os.unlink(out)
    print('\n'.join(lines))
    return 0 if ok else 1


def file_sha(p):
    return hashlib.sha1(open(p, 'rb').read()).hexdigest() if os.path.exists(p) else None


def cmd_record(a):
    t0 = time.time()
    gdir = golden_dir(a)
    scen = SCENARIO_LONG if a.long else SCENARIO
    if golden_meta(gdir) and not a.force:
        print(f'native golden: {gdir} already holds a golden ({golden_meta(gdir)["decomp"]["label"]}); --force replaces it')
        return 2
    if not CFG['iso'] or not os.path.exists(CFG['iso']):
        print('native golden: record needs SLY2_ISO (the disc image or the sparse extent directory)')
        return 2
    source, info = resolve_source(a)
    if info['kind'] == 'tree' and info['dirty']:
        print('native golden: refusing to record from uncommitted changes (commit them, or use --decomp REF)')
        return 2
    port_commit = git_out(CFG['port'], 'rev-parse', (a.port or CFG['port_ref']) + '^{commit}')
    res = Result()
    lines = [f'native golden record: decomp {info["label"]}, port {a.port or CFG["port_ref"]} ({port_commit[:10]})']
    try:
        cache, exe = prepare(a, port_commit, source, res, lines)
    except SetupError as e:
        print(lines[0] + f'\n  SETUP ERROR: {e}')
        return 2
    report_build(res, lines)
    if not exe:
        lines.append(timing_line(res, t0))
        print('\n'.join(lines))
        return 1
    out = os.path.join(cache.root, 'record.jsonl')
    t = time.time()
    n, rc, tail = run_scenario(cache, exe, scen, out, res)
    res.timed('run', t)
    if rc != 0 or n < scen['frames']:
        lines.append(f'  FAILED: harness exit {rc}, {n}/{scen["frames"]} records; golden not written')
        lines += ['    ' + l for l in tail[-3:]]
        print('\n'.join(lines))
        return 1
    # determinism: a second run must be identical, or the golden is worthless
    out2 = out + '.2'
    n2, rc2, _ = run_scenario(cache, exe, scen, out2, res)
    d, _, _ = diff_streams(cache, out, out2)
    os.unlink(out2)
    if d.diverged or rc2 or n2 != n:
        lines.append(f'  FAILED: two runs of the same build differ (first {d.first}); golden not written')
        print('\n'.join(lines))
        return 1
    os.makedirs(gdir, exist_ok=True)
    shutil.move(out, os.path.join(gdir, 'stream.jsonl'))
    shutil.copyfile(os.path.join(cache.port, 'tools/state/offsets_host.json'), os.path.join(gdir, 'offsets_host.json'))
    open(os.path.join(gdir, 'stale_hunks.txt'), 'w').write(''.join(f'{f}: {s}\n' for f, s in res.stale))
    last = json.loads(open(os.path.join(gdir, 'stream.jsonl')).readlines()[-1])
    f = last.get('fields', {})
    meta = dict(recorded=datetime.datetime.now().isoformat(timespec='seconds'), decomp=info,
                port_commit=port_commit, port_ref=a.port or CFG['port_ref'], scenario=scen, records=n,
                offsets_sha=file_sha(os.path.join(gdir, 'offsets_host.json')), stale_hunks=len(res.stale),
                missing_hunks=len(res.missing), missing_roots=res.missing_roots,
                end_state={k: f.get(k) for k in ('jt.pos', 'jt.state', 'cframe', 'clock.time')})
    json.dump(meta, open(os.path.join(gdir, 'meta.json'), 'w'), indent=1)
    lines.append(f'  recorded {n} records (second run identical) -> {gdir}/stream.jsonl '
                 f'({os.path.getsize(os.path.join(gdir, "stream.jsonl")) / 2**20:.1f} MB)')
    lines.append(f'  end state: jt.pos {fmt(f.get("jt.pos"))}, jt.state {f.get("jt.state")}')
    lines.append(timing_line(res, t0))
    print('\n'.join(lines))
    return 0


def cmd_build(a):
    t0 = time.time()
    source, info = resolve_source(a)
    meta = golden_meta(a.golden or CFG['golden'])
    port_commit = git_out(CFG['port'], 'rev-parse', (a.port or (meta and meta['port_commit']) or CFG['port_ref']) + '^{commit}')
    res = Result()
    lines = [f'native build: decomp {info["label"]}, port {port_commit[:10]}']
    try:
        cache, exe = prepare(a, port_commit, source, res, lines)
    except SetupError as e:
        print(lines[0] + f'\n  SETUP ERROR: {e}')
        return 2
    report_build(res, lines)
    lines.insert(1, f'  RESULT     {"BUILT " + exe if exe else "FAIL"}')
    lines.append(timing_line(res, t0))
    print('\n'.join(lines))
    return 0 if exe else 1


def cmd_status(a):
    gdir = golden_dir(a)
    meta = golden_meta(gdir)
    if not meta:
        print(f'native golden: none at {gdir}')
    else:
        sz = os.path.getsize(os.path.join(gdir, 'stream.jsonl')) / 2 ** 20
        print(f'native golden: {gdir}: decomp {meta["decomp"]["label"]}, port {meta["port_ref"]} '
              f'({meta["port_commit"][:10]}), scenario v{meta["scenario"]["version"]}, {meta["records"]} records, '
              f'{sz:.1f} MB, recorded {meta["recorded"]}; {meta.get("stale_hunks", 0)} stale hunks at record time')
    cpath = os.path.join(CFG['cache'], 'cache.json')
    if os.path.exists(cpath):
        st = json.load(open(cpath))
        du = subprocess.run(['du', '-sh', CFG['cache']], capture_output=True, text=True).stdout.split()[0]
        print(f'  cache: {CFG["cache"]} ({du}), port {st.get("port_commit", "?")[:10]}, '
              f'{len(st.get("decomp_manifest", {}))} decomp files mirrored')
    print(f'  SLY2_ISO: {CFG["iso"] or "(unset: build only)"}')
    return 0


def add_source_args(p, record=False):
    g = p.add_mutually_exclusive_group(required=record)
    g.add_argument('--decomp', metavar='REF', help='decomp commit / branch / tag (in the decomp repository)')
    g.add_argument('--tree', metavar='DIR', help='a decomp worktree as it is on disk (tracked + untracked files)')
    p.add_argument('--repo', help='decomp repository for --decomp (default: the one this script is in)')
    p.add_argument('--golden', help='golden directory (default: setting golden, plus -long with --long)')
    p.add_argument('--long', action='store_true', help='the long scenario (12000 frames, P3 pad loop)')


def main(argv=None, default_tree=None):
    global CFG
    CFG = settings()
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0], epilog=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('check'); add_source_args(p)
    p.add_argument('--keep-stream', action='store_true', help='keep the candidate stream in <cache>/logs/')
    p.add_argument('--port', help='build this port commit instead of the golden\'s (cross-port comparison; '
                   'flagged in the report)')
    p = sub.add_parser('record'); add_source_args(p, record=True)
    p.add_argument('--port', help='port commit / branch (default: setting port_ref)')
    p.add_argument('--force', action='store_true', help='replace an existing golden')
    p = sub.add_parser('build'); add_source_args(p)
    p.add_argument('--port', help='port commit (default: the golden\'s, else setting port_ref)')
    p = sub.add_parser('status'); p.add_argument('--golden'); p.add_argument('--long', action='store_true')
    a = ap.parse_args(argv)
    try:
        if a.cmd == 'check':
            if not a.decomp and not a.tree and not default_tree:
                a.tree = os.getcwd()
            return cmd_check(a, default_tree)
        if a.cmd == 'build' and not a.decomp and not a.tree:
            a.tree = default_tree or os.getcwd()
        return {'record': cmd_record, 'build': cmd_build, 'status': cmd_status}[a.cmd](a)
    except SetupError as e:
        print(f'native golden: SETUP ERROR: {e}')
        return 2


if __name__ == '__main__':
    sys.exit(main())
