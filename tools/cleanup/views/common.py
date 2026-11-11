"""Shared helpers for the view tools: repository paths, the game compiler, object comparison.

Everything here runs the repository's own tools/matchtool.py settings (CC, CFLAGS, INCLUDES; its environment
variables such as SLY2_PYTHONPATH apply), so the layouts and objects are the matching build's.
"""
import json, os, re, shutil, struct, subprocess, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))


def set_root(root):
    global ROOT
    ROOT = os.path.abspath(root)


def _matchtool(root=None):
    root = root or ROOT
    p = os.path.join(root, 'tools')
    if p not in sys.path:
        sys.path.insert(0, p)
    import matchtool, wibo_cc
    return matchtool, wibo_cc


def compile_asm(src_abs, out_s, root=None):
    """Compile one C++ file to assembly with the matching flags. Returns (ok, stderr)."""
    root = root or ROOT
    mt, wc = _matchtool(root)
    cmd = [wc.default_wibo(), mt.CC, '-S'] + mt.INCLUDES + mt.CFLAGS + [src_abs, '-o', out_s]
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=300, cwd=root)
    except subprocess.TimeoutExpired:
        return False, 'timeout'
    return r.returncode == 0 and os.path.exists(out_s), r.stderr


def compile_obj(rel, root=None):
    """Compile repo-relative source `rel` in `root` the way matchtool does; returns the .o path or None.
    Runs in a subprocess so that two worktrees' matchtool modules never mix."""
    root = root or ROOT
    code = 'import sys;sys.path.insert(0,"tools");import matchtool;print(matchtool.compile_src(%r))' % rel
    r = subprocess.run([sys.executable, '-c', code], cwd=root, capture_output=True, text=True)
    p = (r.stdout.strip().split('\n') or [''])[-1]
    if not p.endswith('.o') or not os.path.exists(p):
        return None, r.stderr[-3000:]
    return p, ''


# ---------------------------------------------------------------- object comparison (byte for byte)
def _elf(path):
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2E)
    sh = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    so = sh[shstrndx][4]
    names = [d[so + s[0]:d.index(b'\0', so + s[0])].decode() for s in sh]
    return d, sh, names


def obj_view(path):
    """Normalised content of an object: every allocated/progbits section's bytes, relocations by target symbol name,
    and symbols by name (value, size, info, section name). .mdebug and .comment are ignored."""
    d, sh, names = _elf(path)
    symtab = [s for s in sh if s[1] == 2]
    syms, symlist = {}, []
    if symtab:
        st = symtab[0]; strs = sh[st[6]][4]
        for k in range(st[5] // 16):
            nm_, val, size, info, oth, shndx = struct.unpack_from('<IIIBBH', d, st[4] + k * 16)
            sn = d[strs + nm_:d.index(b'\0', strs + nm_)].decode()
            secn = names[shndx] if shndx < len(names) else str(shndx)
            symlist.append(sn or secn)
            if sn:
                key = re.sub(r'^(_GLOBAL_\$[ID]\$.*?\.c(?:pp)?)\w{6}$', r'\1', sn)
                syms[key] = (val, size, info, secn)
    secs = {}
    for i, s in enumerate(sh):
        n, t = names[i], s[1]
        if n.startswith('.mdebug') or n == '.comment' or t not in (1, 8, 9):
            continue
        if t == 8:
            secs[n] = ('nobits', s[5])
        elif t == 9:
            raw = d[s[4]:s[4] + s[5]]
            secs[n] = [(struct.unpack_from('<I', raw, j)[0], struct.unpack_from('<I', raw, j + 4)[0] & 0xff,
                        symlist[struct.unpack_from('<I', raw, j + 4)[0] >> 8] if symlist else '')
                       for j in range(0, len(raw), 8)]
        else:
            secs[n] = d[s[4]:s[4] + s[5]]
    return secs, syms


def obj_diff(a, b):
    """'' if objects a and b are identical in obj_view terms, else a one-line summary of what differs."""
    (sa, ya), (sb, yb) = obj_view(a), obj_view(b)
    ds = [n for n in sorted(set(sa) | set(sb)) if sa.get(n) != sb.get(n)]
    dy = sorted(k for k in set(ya) | set(yb) if ya.get(k) != yb.get(k))
    if not ds and not dy:
        return ''
    out = []
    if ds:
        det = []
        for n in ds[:6]:
            x, y = sa.get(n), sb.get(n)
            if isinstance(x, bytes) and isinstance(y, bytes):
                if len(x) != len(y):
                    det.append('%s size %d->%d' % (n, len(x), len(y)))
                else:
                    nd = sum(1 for i in range(0, len(x), 4) if x[i:i + 4] != y[i:i + 4])
                    det.append('%s %d words' % (n, nd))
            else:
                det.append(n)
        out.append('sections: ' + ', '.join(det))
    if dy:
        out.append('symbols: ' + ', '.join(dy[:6]))
    return '; '.join(out)


class BaseObjects:
    """Objects of the baseline worktree, compiled once per source and cached (copied out of the temp dir)."""

    def __init__(self, base_root, cache_dir):
        self.base_root = os.path.abspath(base_root)
        self.cache = cache_dir
        os.makedirs(cache_dir, exist_ok=True)

    def get(self, rel):
        p = os.path.join(self.cache, rel.replace('/', '__') + '.o')
        if not os.path.exists(p):
            o, err = compile_obj(rel, self.base_root)
            if not o:
                raise RuntimeError('baseline compile failed: %s\n%s' % (rel, err))
            shutil.move(o, p)
        return p


def check_against_base(rel, base, root=None):
    """Compile `rel` in root and compare with the baseline object: '' if identical, else the reason."""
    o, err = compile_obj(rel, root)
    if not o:
        m = [l for l in err.splitlines() if 'error' in l.lower() or re.search(r':\d+:', l)]
        if not m:
            m = syntax_errors(rel, root)
        return 'compile failed: ' + (' | '.join(m[:3]) or 'no message (cc1plus crash)')
    try:
        return obj_diff(base.get(rel), o)
    finally:
        os.remove(o)


def syntax_errors(rel, root=None):
    """cc1plus often dies without a message on errors in this code base: preprocess with ee-gcc and let the host g++
    (-m32, gnu++98, permissive) name the first errors."""
    root = root or ROOT
    mt, wc = _matchtool(root)
    ii = tempfile.mktemp(suffix='.ii')
    flags = [f for f in mt.CFLAGS if f.startswith('-B')]
    r = subprocess.run([wc.default_wibo(), mt.CC, '-E'] + mt.INCLUDES + ['-x', 'c++'] + flags + [rel, '-o', ii],
                       cwd=root, capture_output=True, text=True)
    if r.returncode or not os.path.exists(ii):
        return ['preprocess failed: ' + r.stderr[-300:]]
    r = subprocess.run(['g++', '-fsyntax-only', '-fpermissive', '-w', '-x', 'c++', '-std=gnu++98', '-m32', ii],
                       capture_output=True, text=True)
    os.remove(ii)
    errs = [l for l in r.stderr.splitlines() if 'error' in l and not re.search(
        r"assert_|is negative|emulate|TI.|named return|was not declared in this scope", l)]
    return [re.sub(r'^\S*?([\w.]+:\d+):\d+:', r'\1:', l) for l in errs[:3]]


def includers(paths, root=None):
    """Sources that must be re-checked for the touched paths (tools/cleanup/includers.py)."""
    root = root or ROOT
    r = subprocess.run([sys.executable, os.path.join(root, 'tools/cleanup/includers.py')] + list(paths),
                       cwd=root, capture_output=True, text=True)
    return r.stdout.split()


# ---------------------------------------------------------------- source text helpers
def strip_comments(s):
    """Blank comments and string/char literals, keeping every offset and newline."""
    out = []; i = 0; n = len(s)
    while i < n:
        c = s[i]
        if c == '/' and i + 1 < n and s[i + 1] == '/':
            j = s.find('\n', i)
            j = n if j < 0 else j
            out.append(' ' * (j - i)); i = j
        elif c == '/' and i + 1 < n and s[i + 1] == '*':
            j = s.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r'[^\n]', ' ', s[i:j])); i = j
        elif c in '"\'':
            j = i + 1
            while j < n and s[j] != c:
                j += 2 if s[j] == '\\' else 1
            j = min(j + 1, n)
            out.append(c + re.sub(r'[^\n]', ' ', s[i + 1:j - 1]) + (c if j - i > 1 else '')); i = j
        elif c == '#' and (i == 0 or s[i - 1] == '\n'):
            # preprocessor line (with continuations): blank it, keep newlines
            j = i
            while True:
                k = s.find('\n', j)
                if k < 0:
                    k = n; break
                if s[k - 1] == '\\':
                    j = k + 1; continue
                break
            out.append(re.sub(r'[^\n]', ' ', s[i:k])); i = k
        else:
            out.append(c); i += 1
    return ''.join(out)


def match_brace(s, i):
    """s[i] == '{': index just past the matching '}'."""
    d = 0
    for j in range(i, len(s)):
        if s[j] == '{':
            d += 1
        elif s[j] == '}':
            d -= 1
            if d == 0:
                return j + 1
    return len(s)


def load_json(p, default=None):
    try:
        return json.load(open(p))
    except (OSError, ValueError):
        return default
