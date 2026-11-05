#!/usr/bin/env python3
"""Drop-in replacement for `wibo` used by the matching tools.

When invoked as  wibo_cc.py ee-gcc.exe ... -c SRC -o OBJ  it compiles SRC to assembly, runs
tools/asfix.py over it (Sly 2's short-loop padding) and assembles the result with the same driver
flags.  Every other invocation is passed straight to the real wibo."""
import os, sys, subprocess, tempfile, shutil, signal, re
HERE = os.path.dirname(os.path.abspath(__file__))
WIN = os.name == 'nt'   # native Windows: the compilers are .exe files, run them directly (no wibo)
def default_wibo():
    """Path of the wibo binary: $SLY2_WIBO_BIN, else tools/wibo-macos on macOS, else tools/wibo-i686."""
    if os.environ.get('SLY2_WIBO_BIN'):
        return os.environ['SLY2_WIBO_BIN']
    return os.path.join(HERE, 'wibo-macos' if sys.platform == 'darwin' else 'wibo-i686')
REAL = None if WIN else default_wibo()
sys.path.insert(0, HERE)
import asfix

def run(cmd, tries=3, timeout=60):
    """wibo occasionally hangs; run in its own process group, kill and retry on timeout"""
    if WIN:
        cmd = [c for c in cmd if c is not None]
    for _ in range(tries):
        p = subprocess.Popen(cmd) if WIN else subprocess.Popen(cmd, start_new_session=True)
        try:
            return p.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            if WIN: p.kill()
            else: os.killpg(p.pid, signal.SIGKILL)
            p.wait()
    return 1

def passthrough(a):
    if WIN:
        sys.exit(subprocess.call(a))
    os.execv(REAL, [REAL] + a)

def marker_addrs(obj):
    """{name: value} of the __af* marker symbols in an object"""
    import struct
    try:
        d = open(obj, 'rb').read()
        shoff, = struct.unpack_from('<I', d, 0x20); shentsize, shnum, _ = struct.unpack_from('<HHH', d, 0x2E)
        secs = [struct.unpack_from('<10I', d, shoff + i * shentsize) for i in range(shnum)]
        st = next(x for x in secs if x[1] == 2); strt = secs[st[6]]
        v = {}
        for i in range(st[5] // 16):
            nm, val = struct.unpack_from('<II', d, st[4] + i * 16)
            name = d[strt[4] + nm:d.index(b'\0', strt[4] + nm)].decode()
            if name.startswith('__af'): v[name] = val
        return v
    except Exception:
        return None

def main():
    a = sys.argv[1:]
    if not (a and a[0].endswith('ee-gcc.exe') and '-c' in a and '-o' in a):
        passthrough(a)
    o = a.index('-o'); obj = a[o + 1]
    rest = a[1:o] + a[o + 2:]
    srcs = [x for x in rest if not x.startswith('-') and os.path.splitext(x)[1] in ('.c', '.cpp', '.cc', '.C')]
    if len(srcs) != 1:
        passthrough(a)
    src = srcs[0]
    t = tempfile.mkdtemp(prefix='wcc')
    try:
        s1 = os.path.join(t, 'a.s'); s2 = os.path.join(t, 'b.s')
        cmd1 = [REAL, a[0]] + ['-S' if x == '-c' else x for x in rest] + ['-o', s1]
        rc = run(cmd1)
        if rc or not os.path.exists(s1):
            return rc or 1
        raw = open(s1).read()
        txt, _ = asfix.fix(raw)
        txt0 = txt
        needs = sorted(set(asfix.LAST_NEEDS))
        open(s2, 'w').write(txt)
        flags = []
        skip = False
        for x in rest:
            if skip: skip = False; continue
            if x == src: continue
            if x == '-x': skip = True; continue
            if x.startswith('-x'): continue
            flags.append(x)
        # Sly 2 was assembled with SN's ps2eeas (it reproduces the original's 64-bit constant expansion
        # and FPU hazard nops); set SLY2_AS=gnu to use the GNU as that ships with the compiler instead
        asm = os.environ.get('SLY2_AS', os.path.join(HERE, 'compilers', '2.95.3-114', 'lib', 'gcc-lib', 'ee', '2.95.3', 'ps2eeas.exe'))
        if asm == 'gnu': asm = None
        if asm and needs:
            # short loops containing an 8-byte alignment (asfix.add_markers).  Measurement pass: drop those
            # alignments and pad each loop fully; every loop then has the length it will have in the final output
            # (alignment nops + remaining padding = full padding), so the address after each loop-head label shows
            # whether the alignment adds a nop there (section start assumed 8-aligned).
            # the indices in needs refer to the text after asfix.divfix (fix() runs it first), so mark that text;
            # divfix is idempotent, so the fix() calls below see the same lines
            marked = asfix.add_markers(asfix.divfix(raw), needs)
            if os.environ.get('SLY2_ALIGNLOG'):
                open(os.environ['SLY2_ALIGNLOG'], 'a').write(src + '\n')
            meas = re.sub(r'^(__afA\d+:)\n\s*\.(p2align|align)\s+3\s*$', r'\1', marked, flags=re.M)
            txt, _ = asfix.fix(meas, {k: 0 for k in range(len(needs))})
            open(s2, 'w').write(txt)
            nal = None
            if not run([REAL, asm, '-G0', '-O2', '-mabi=eabi', '-o', obj, s2]):
                b = marker_addrs(obj)
                if b is not None and all('__afB%d' % k in b for k in range(len(needs))):
                    nal = {k: (b['__afB%d' % k] % 8) // 4 for k in range(len(needs))}
            txt, _ = asfix.fix(marked, nal) if nal is not None else (txt0, 0)
            open(s2, 'w').write(txt)
        if asm:
            return run([REAL, asm, '-G0', '-O2', '-mabi=eabi', '-o', obj, s2])
        return run([REAL, a[0]] + flags + ['-x', 'assembler', s2, '-o', obj])
    finally:
        shutil.rmtree(t, ignore_errors=True)

if __name__ == '__main__':
    sys.exit(main())
