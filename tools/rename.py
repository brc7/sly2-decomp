#!/usr/bin/env python3
"""Rename functions (and data symbols) in the Sly 2 decomp without breaking the byte match.

  rename.py OLD NEW [--mangled M]      one symbol. OLD = current name or 0xADDR; NEW = plain name or gcc 2.95 mangled name
  rename.py --csv FILE                 batch: rows "addr,new_name[,new_mangled]" (header optional, '#' comments allowed)

Options:
  --dry-run            print the planned edits (add --diff for unified diffs); write nothing
  --on-fail revert     (default) a rename whose edits break any touched file's match is undone automatically;
                       the other renames in the batch are kept
  --on-fail stop       stop at the first verification failure and leave the tree as it is (exit 2)
  --no-verify          skip matchtool verification (unit tests / emergencies only)
  --allow-unverifiable apply renames that touch code in files this machine cannot build (marked in the log)
  --allow-existing-uses accept that code already using the new name refers to this same function (otherwise refused;
                       a local alias bound with __asm__("OLD") is recognised and accepted automatically)
  --log FILE           machine-readable JSON log (default build/rename_log.json)
  --commit-msg FILE    also write a commit-message summary
  --jobs N             parallel matchtool runs (default 8)
  --root DIR           repo root (default: parent of tools/)

What one rename changes (see DESIGN.txt for the reasons):
  * every identifier token OLD in code and every symbol-shaped occurrence inside string literals (asm labels, inline
    "jal OLD") under src/ and include/  ->  the new linker symbol (mangled name if given, otherwise the plain name);
  * every OLD in comments -> the plain name (OLD.c / OLD.cpp in comments -> the new file name);
  * src/**/OLD.c / OLD.cpp -> src/**/<symbol>.c / .cpp;
  * config/symbol_addrs.txt (entry added or renamed), config/functions.csv Name, config/matched.csv name/source.
Identifiers that merely contain OLD (func_00117120_x) are never touched; they are listed in the log as near misses.
Besides the current name, the address placeholder (func_XXXXXXXX / D_XXXXXXXX) is also swept, so re-running a rename
for an already-named address finishes references left behind by earlier partial renames.
Every file whose code changes is rebuilt with tools/matchtool.py before and after; per-address results must be equal.
For a function, the file that matches it (config/matched.csv source) is always rebuilt, and the rename is refused
unless that file defines it under one of the swept names (a C++ member spelled AllocSw, not AllocSw__4DMASi, needs a
hand edit). matchtool checks every function of a file, so a merged TU is verified whole.
"""
import argparse, csv, datetime, difflib, io, json, os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

TOOL_VERSION = '1.0'
SRC_DIRS = ('src', 'include')
SRC_EXTS = ('.c', '.cpp', '.h', '.hpp', '.inc')
EXTERNAL_GLOBS = ('docs', 'tools', 'README.md', 'SETUP.md', 'CREDITS.md')
IDC = 'A-Za-z0-9_$'          # gcc accepts '$' in identifiers (used by _$_ destructors, _GLOBAL_$I$...)

# --------------------------------------------------------------------------------------------------------------------
# lexing: split C/C++ text into code / comment / string / char segments
# --------------------------------------------------------------------------------------------------------------------
_ASM_OPEN = re.compile(r'(?<![A-Za-z0-9_$])(?:__asm__|__asm|asm)(?:\s+(?:__volatile__|volatile))?\s*\(\s*\Z')

def segments(text):
    """List of (kind, start, end) covering text; kind in code|comment|string|char."""
    out = []; n = len(text); i = 0; cs = 0
    def flush(upto):
        if upto > cs: out.append(('code', cs, upto))
    while i < n:
        c = text[i]
        if c == '/' and i + 1 < n and text[i + 1] == '/':
            flush(i); j = i + 2
            while True:                       # a line comment continues over backslash-newline
                k = text.find('\n', j)
                if k == -1: k = n; break
                if text[k - 1] == '\\' or (text[k - 1] == '\r' and text[k - 2] == '\\'): j = k + 1; continue
                break
            out.append(('comment', i, k)); i = cs = k
        elif c == '/' and i + 1 < n and text[i + 1] == '*':
            flush(i); k = text.find('*/', i + 2); k = n if k == -1 else k + 2
            out.append(('comment', i, k)); i = cs = k
        elif c == '"' or c == "'":
            # gcc 2.95 accepts raw newlines inside string literals; the code base uses them in multi-line asm
            # templates (`__asm__("mfc1 %0,%2<newline>\tmfc1 ...")`). Only a string that is an asm template
            # (directly after `asm(` / `asm volatile(`) may span lines, so a stray quote elsewhere cannot swallow code.
            flush(i); j = i + 1
            multiline = c == '"' and _ASM_OPEN.search(text, max(0, i - 40), i) is not None
            while j < n and text[j] != c and (multiline or text[j] != '\n'):
                j += 2 if text[j] == '\\' else 1
            j = min(j + 1, n)
            out.append(('string' if c == '"' else 'char', i, j)); i = cs = j
        else:
            i += 1
    flush(n)
    return out

def blank_non_code(text):
    """text with comments, strings and char literals replaced by spaces (newlines kept)."""
    out = list(text)
    for kind, a, b in segments(text):
        if kind != 'code':
            for i in range(a, b):
                if out[i] != '\n': out[i] = ' '
    return ''.join(out)

def _alts(names):
    return '|'.join(re.escape(x) for x in sorted(names, key=len, reverse=True))

def token_kinds(text, name):
    """Set of segment kinds in which `name` occurs as a whole token."""
    if name not in text: return set()
    pc = re.compile(r'(?<![%s])%s(?![%s])' % (IDC, re.escape(name), IDC))
    ps = re.compile(r'(?:(?<![%s])|(?<=\\[abfnrtv]))%s(?![%s])' % (IDC, re.escape(name), IDC))
    kinds = set()
    for kind, a, b in segments(text):
        seg = text[a:b]
        if name not in seg: continue
        if (ps if kind == 'string' else pc).search(seg): kinds.add(kind)
    return kinds

def near_misses(text, name):
    """Identifiers that contain `name` but are not exactly it (never renamed; reported for review)."""
    if name not in text: return []
    return sorted({m.group(0) for m in re.finditer(r'[%s]*%s[%s]*' % (IDC, re.escape(name), IDC), text)} - {name})

def rewrite(text, mapping):
    """mapping: old -> (code_repl, comment_repl, file_stem_repl).  Returns (new_text, counts{old: {kind: n}})."""
    if not mapping: return text, {}
    present = {o: r for o, r in mapping.items() if o in text}
    if not present: return text, {}
    alts = _alts(present)
    pc = re.compile(r'(?<![%s])(%s)(?![%s])' % (IDC, alts, IDC))
    ps = re.compile(r'(?:(?<![%s])|(?<=\\[abfnrtv]))(%s)(?![%s])' % (IDC, alts, IDC))
    pm = re.compile(r'(?<![%s])(%s)(\.(?:cpp|c|h))?(?![%s])' % (IDC, alts, IDC))
    counts = {}
    def bump(o, k): counts.setdefault(o, {}).setdefault(k, 0); counts[o][k] += 1
    out = []
    for kind, a, b in segments(text):
        seg = text[a:b]
        if not any(o in seg for o in present): out.append(seg); continue
        if kind == 'comment':
            def f(m):
                o = m.group(1); bump(o, 'comment')
                return present[o][2] + m.group(2) if m.group(2) else present[o][1]
            seg = pm.sub(f, seg)
        elif kind in ('code', 'string', 'char'):
            p = ps if kind == 'string' else pc
            def f(m, kind=kind):
                o = m.group(1); bump(o, 'string' if kind != 'code' else 'code'); return present[o][0]
            seg = p.sub(f, seg)
        out.append(seg)
    return ''.join(out), counts

# --------------------------------------------------------------------------------------------------------------------
# gcc 2.95 ("gnu" v2) mangled-name validation
# --------------------------------------------------------------------------------------------------------------------
OPERATORS = {'nw', 'dl', 'vn', 'vd', 'as', 'ne', 'eq', 'ge', 'gt', 'le', 'lt', 'pl', 'apl', 'mi', 'ami', 'ml', 'aml',
             'dv', 'adv', 'md', 'amd', 'er', 'aer', 'ad', 'aad', 'or', 'aor', 'co', 'nt', 'ls', 'als', 'rs', 'ars',
             'aa', 'oo', 'pp', 'mm', 'cm', 'rm', 'rf', 'cl', 'vc', 'mx', 'mn', 'cn'}

class _MP:
    def __init__(self, s): self.s = s; self.i = 0
    def peek(self, k=0): return self.s[self.i + k] if self.i + k < len(self.s) else ''
    def eat(self, c=None):
        ch = self.peek()
        if not ch or (c is not None and ch != c): raise ValueError(f'expected {c or "more"!r} at {self.i}')
        self.i += 1; return ch
    def digits(self):
        j = self.i
        while self.peek().isdigit(): self.i += 1
        if j == self.i: raise ValueError(f'number expected at {j}')
        return int(self.s[j:self.i])
    def count(self):                       # cplus-dem get_count: one digit, or several digits closed by '_'
        if not self.peek().isdigit(): raise ValueError('count expected')
        j = self.i; n = int(self.eat())
        if self.peek().isdigit():
            k = self.i
            while k < len(self.s) and self.s[k].isdigit(): k += 1
            if k < len(self.s) and self.s[k] == '_': n = int(self.s[j:k]); self.i = k + 1
        return n
    def name(self):
        n = self.digits()
        nm = self.s[self.i:self.i + n]
        if len(nm) != n or not re.fullmatch(r'[A-Za-z_$][%s]*' % IDC, nm): raise ValueError('bad class name')
        self.i += n; return nm
    def cls(self):
        c = self.peek()
        if c == 'Q':
            self.eat()
            if self.peek() == '_': self.eat(); n = self.digits(); self.eat('_')
            else: n = int(self.eat())
            if n < 1: raise ValueError('bad Q count')
            return '::'.join(self.cls() for _ in range(n))
        if c == 't':                         # template class t<name><nargs>{Z<type> | <type><value>}
            self.eat(); nm = self.name(); n = self.count()
            for _ in range(n):
                if self.peek() == 'Z': self.eat(); self.type_()
                else:
                    self.type_()
                    if self.peek() == 'm': self.eat()
                    self.digits()
            return nm + '<>'
        if c.isdigit(): return self.name()
        raise ValueError(f'class expected at {self.i}')
    def type_(self, nparams=None):
        while self.peek() in ('C', 'V', 'U', 'S') and self.peek(): self.eat()
        c = self.peek()
        if c in ('P', 'R'): self.eat(); return self.type_(nparams)
        if c == 'A': self.eat(); self.digits(); self.eat('_'); return self.type_(nparams)
        if c == 'F':
            self.eat(); self.params(stop='_'); self.eat('_'); return self.type_(nparams)
        if c == 'M':
            self.eat(); self.cls()
            while self.peek() in ('C', 'V') and self.peek(): self.eat()
            return self.type_(nparams)
        if c == 'O': self.eat(); self.cls(); self.eat('_'); return self.type_(nparams)
        if c == 'G': self.eat(); return self.cls()
        if c and (c.isdigit() or c in 'Qt'): return self.cls()
        if c and c in 'vcsilxfdrbwe': self.eat(); return c
        raise ValueError(f'type expected at {self.i} ({c!r})')
    def params(self, stop='', base=0):
        n = base
        while self.peek() and self.peek() != stop:
            c = self.peek()
            if c == 'T':
                self.eat(); k = self.count()
                if k >= n: raise ValueError('T back-reference out of range')
                n += 1
            elif c == 'N':
                self.eat(); r = self.count(); k = self.count()
                if k >= n or r < 1: raise ValueError('N back-reference out of range')
                n += r
            else:
                self.type_(); n += 1
        return n - base

def parse_mangled(s):
    """Validate a gcc 2.95 mangled function name.  Returns dict(base, cls, kind) or raises ValueError."""
    if not re.fullmatch(r'[A-Za-z_$][%s]*' % IDC, s): raise ValueError('not an identifier')
    m = re.fullmatch(r'_[$.]_(.+)', s)
    if m:                                     # destructor _$_<class>
        p = _MP(m.group(1)); c = p.cls()
        if p.i != len(p.s): raise ValueError('trailing junk after destructor class')
        return dict(base='~' + c.split('::')[-1], cls=c, kind='dtor')
    errs = []
    start = 2 if s.startswith('__') else 0
    for pos in [k.start() for k in re.finditer(r'(?=__)', s) if k.start() >= start] + ([0] if s.startswith('__') else []):
        base, sig = s[:pos], s[pos + 2:]
        kind = 'func'
        if base == '': kind = 'ctor'
        elif base.startswith('__'):
            op = base[2:]
            if op in OPERATORS: kind = 'operator'
            elif op.startswith('op'):
                try:
                    q = _MP(op[2:]); q.type_()
                    if q.i != len(q.s): raise ValueError
                    kind = 'operator'
                except ValueError: continue
            elif not re.fullmatch(r'__[A-Za-z_$][%s]*' % IDC, base) or base.endswith('_'): continue
        elif not re.fullmatch(r'[A-Za-z_$][%s]*' % IDC, base) or base.endswith('_'): continue
        p = _MP(sig)
        try:
            if p.peek() == 'F':
                if kind == 'ctor': raise ValueError('constructor cannot be a free function')
                p.eat()
                if p.params() == 0: raise ValueError('free function needs at least one parameter code (v)')
                cls = None
            else:
                if p.peek() in ('C', 'S') and (p.peek(1).isdigit() or p.peek(1) in 'Qt'): p.eat()
                cls = p.cls(); p.params(base=1)
            if p.i != len(sig): raise ValueError('trailing characters')
            return dict(base=base if kind != 'ctor' else cls.split('::')[-1], cls=cls, kind=kind)
        except ValueError as e:
            errs.append(f'split@{pos}: {e}')
    raise ValueError('; '.join(errs) or 'no "__" signature separator')

def looks_mangled(s):
    try: parse_mangled(s); return True
    except ValueError: return False

C_KEYWORDS = set('''auto break case char const continue default do double else enum extern float for goto if inline int long
register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while asm bool catch
class const_cast delete dynamic_cast explicit export false friend mutable namespace new operator private protected public
reinterpret_cast static_cast template this throw true try typeid typename using virtual wchar_t __asm__ __attribute__
__inline__ __extension__ __typeof__ typeof'''.split())
PLACEHOLDER = re.compile(r'(func|D|FUN|junk|sub|dat|DAT|LAB|jtbl)_(0x)?[0-9A-Fa-f]{6,8}$')

def check_plain(name):
    if not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', name): return 'not a valid C identifier'
    if name in C_KEYWORDS: return 'is a C/C++ keyword'
    if '__' in name: return 'contains "__" (reserved, and indistinguishable from a mangled name); give new_mangled instead'
    if PLACEHOLDER.match(name): return 'is an address placeholder name'
    return None

# --------------------------------------------------------------------------------------------------------------------
# repo model
# --------------------------------------------------------------------------------------------------------------------
SA_RE = re.compile(r'(\s*)(\S+)(\s*=\s*0x)([0-9A-Fa-f]+)(;.*)?$')

class Repo:
    def __init__(self, root):
        self.root = os.path.abspath(root)
        self.cfg = {k: self._read(f'config/{k}') for k in ('symbol_addrs.txt', 'functions.csv', 'matched.csv')}
        self.files = {}
        for d in SRC_DIRS:
            for dp, dn, fn in os.walk(os.path.join(self.root, d)):
                dn.sort()
                for f in sorted(fn):
                    if f.endswith(SRC_EXTS):
                        p = os.path.relpath(os.path.join(dp, f), self.root).replace(os.sep, '/')
                        self.files[p] = self._read(p)
        self.reindex()

    def _read(self, rel):
        with open(os.path.join(self.root, rel), newline='') as f: return f.read()

    def reindex(self):
        self.sa = {}; self.sa_by_name = {}
        for line in self.cfg['symbol_addrs.txt'].splitlines():
            m = SA_RE.match(line)
            if m:
                a = int(m.group(4), 16); self.sa.setdefault(a, m.group(2)); self.sa_by_name[m.group(2)] = a
        self.fc = {}; self.fc_by_name = {}
        for line in self.cfg['functions.csv'].splitlines()[1:]:
            p = line.split(',')
            if len(p) >= 4:
                a = int(p[1], 16); self.fc[a] = (p[0], int(p[2], 16) - a); self.fc_by_name[p[0]] = a

    def name_at(self, addr):
        if addr in self.sa: return self.sa[addr]
        if addr in self.fc: return self.fc[addr][0]
        return 'D_%08X' % addr

    def addr_of(self, name):
        if re.fullmatch(r'0x[0-9A-Fa-f]+', name): return int(name, 16)
        if name in self.sa_by_name: return self.sa_by_name[name]
        if name in self.fc_by_name: return self.fc_by_name[name]
        m = re.fullmatch(r'(?:func|D)_([0-9A-Fa-f]{8})', name)
        if m: return int(m.group(1), 16)
        return None

    def kind_of(self, addr):
        return 'func' if addr in self.fc else 'data'

    def table_owner(self, name):
        """Address that the name tables give `name`, or None."""
        return self.sa_by_name.get(name, self.fc_by_name.get(name))

    def unbuildable(self, rel):
        """True if matchtool cannot build this file on this machine (native 32-bit Linux SDK compilers)."""
        if not rel.endswith(('.c', '.cpp')): return False
        m = re.search(r'SLY2_CC:\s*(\S+)', self.files.get(rel, '')[:2000])
        if m and os.path.exists(os.path.join(self.root, m.group(1), 'bin', 'ee-gcc')):
            return not os.path.exists('/lib/ld-linux.so.2')
        return False

# --------------------------------------------------------------------------------------------------------------------
# requests / renames
# --------------------------------------------------------------------------------------------------------------------
class Rename:
    def __init__(self, line, key, new_name, new_mangled):
        self.line, self.key, self.new_name, self.new_mangled = line, key, (new_name or '').strip(), (new_mangled or '').strip()
        self.addr = None; self.old = None; self.aliases = []; self.symbol = None; self.plain = None
        self.kind = None; self.policy = None; self.status = 'pending'; self.reason = ''
        self.files = {}          # rel -> {kind: count}
        self.defining = {}       # rel -> new rel
        self.verify = []         # rel paths (original) to verify
        self.unverifiable = []
        self.near = {}; self.external = []; self.config = []; self.dups = []; self.existing_uses = []
        self.final_paths = {}    # rel -> path at the end of the run (another rename may have renamed the file)
        self.verification = {}

    def mapping(self):
        stem = self.symbol
        return {a: (self.symbol, self.plain, stem) for a in self.aliases}

    def to_json(self):
        return dict(input_line=self.line, addr=('0x%06X' % self.addr) if self.addr is not None else None, kind=self.kind,
                    old=self.old, aliases_swept=self.aliases, new_symbol=self.symbol, new_plain=self.plain,
                    new_mangled=self.new_mangled or None, policy=self.policy, status=self.status, reason=self.reason,
                    files_touched=[dict(path=p, new_path=self.final_paths.get(p, self.defining.get(p, p)), **c)
                                   for p, c in sorted(self.files.items())],
                    config_edits=self.config, verified_files=self.verify, unverifiable_files=self.unverifiable,
                    near_miss_identifiers=self.near, external_mentions_not_edited=self.external,
                    duplicate_definitions=self.dups, existing_uses_of_new_name=self.existing_uses,
                    verification=self.verification)

def read_requests(path):
    reqs = []
    with open(path, newline='') as f:
        for i, row in enumerate(csv.reader(f), 1):
            if not row or not row[0].strip() or row[0].strip().startswith('#'): continue
            if i == 1 and row[0].strip().lower() in ('addr', 'address'): continue
            row = [c.strip() for c in row] + ['', '']
            reqs.append(Rename(i, row[0], row[1], row[2]))
    return reqs

class Engine:
    def __init__(self, root, verifier=None, jobs=8, allow_unverifiable=False, allow_existing_uses=False, out=sys.stdout):
        self.repo = Repo(root)
        self.verifier = verifier if verifier is not None else MatchtoolVerifier(self.repo.root, jobs)
        self.allow_unverifiable = allow_unverifiable
        self.allow_existing_uses = allow_existing_uses
        self.out = out
        self.orig_files = dict(self.repo.files)
        self.orig_cfg = dict(self.repo.cfg)
        self.state = {p: p for p in self.orig_files}      # original path -> current path
        self.events = []

    def log(self, msg): print(msg, file=self.out)

    # ---- resolve + validate ------------------------------------------------------------------------------------
    def resolve(self, r):
        R = self.repo
        addr = R.addr_of(r.key)
        if addr is None: return self._refuse(r, f'unknown symbol or address {r.key!r}')
        r.addr = addr; r.kind = R.kind_of(addr); r.old = R.name_at(addr)
        if re.fullmatch(r'0x[0-9A-Fa-f]+', r.key) is None and r.key != r.old and r.key not in (
                'func_%08X' % addr, 'D_%08X' % addr):
            return self._refuse(r, f'{r.key} is not the current name at 0x{addr:06X} ({r.old})')
        nn, nm = r.new_name, r.new_mangled
        if not nm and nn and ('__' in nn or nn.startswith('_$_')) and looks_mangled(nn):
            nm = nn; nn = ''                              # a mangled name passed as new_name
        if nm:
            if r.kind == 'data': return self._refuse(r, 'mangled names are only supported for functions')
            try: info = parse_mangled(nm)
            except ValueError as e: return self._refuse(r, f'new_mangled {nm!r} is not a valid gcc 2.95 mangled name ({e})')
            if nn:
                if info['kind'] == 'func' and info['base'] != nn:
                    return self._refuse(r, f'new_name {nn!r} does not match the mangled base name {info["base"]!r}')
            else:
                nn = info['base'] if re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', info['base']) else nm
            r.symbol, r.plain, r.policy = nm, nn, 'mangled'
        else:
            if not nn: return self._refuse(r, 'no new name')
            err = check_plain(nn)
            if err: return self._refuse(r, f'new_name {nn!r} {err}')
            r.symbol = r.plain = nn; r.policy = 'plain-c-linkage' if r.kind == 'func' else 'data'
        if r.kind == 'func' and not (0x100000 <= addr < 0x100000 + 0x159df0):
            return self._refuse(r, 'function address outside .text')
        ph = ('func_%08X' if r.kind == 'func' else 'D_%08X') % addr
        r.aliases = [a for a in dict.fromkeys([r.old, ph]) if a != r.symbol]
        return r

    def _refuse(self, r, why):
        r.status = 'refused'; r.reason = why; return r

    # ---- plan --------------------------------------------------------------------------------------------------
    def plan(self, renames):
        R = self.repo
        live = [r for r in renames if r.status == 'pending']
        # batch-level conflicts
        seen_addr, seen_sym, alias_owner = {}, {}, {}
        for r in live:
            for a in r.aliases: alias_owner[a] = r
        for r in live:
            if r.addr in seen_addr: self._refuse(r, f'address also renamed on line {seen_addr[r.addr].line}'); continue
            if r.symbol in seen_sym: self._refuse(r, f'new name also used on line {seen_sym[r.symbol].line}'); continue
            if r.symbol in alias_owner and alias_owner[r.symbol] is not r:
                self._refuse(r, f'new name is the current name of line {alias_owner[r.symbol].line} (chains/swaps not supported)'); continue
            seen_addr[r.addr] = r; seen_sym[r.symbol] = r
        live = [r for r in live if r.status == 'pending']
        lower_paths = {p.lower(): p for p in R.files}
        claimed = {}
        for r in live:
            owner = R.table_owner(r.symbol)
            if owner is not None and owner != r.addr:
                self._refuse(r, f'{r.symbol} already names 0x{owner:06X} in config'); continue
            stems = {}
            for p, text in R.files.items():
                stem = os.path.splitext(os.path.basename(p))[0]
                hit = {}
                for a in r.aliases:
                    k = token_kinds(text, a)
                    if k: hit[a] = k
                    nm = near_misses(text, a)
                    if nm: r.near.setdefault(p, []).extend(nm)
                if stem in r.aliases: stems[p] = stem
                if hit: r.files[p] = {}
                # collision: the new symbol already used as a code/string token
                if r.symbol in text:
                    k = token_kinds(text, r.symbol) - {'comment'}
                    if k and r.symbol != r.old:
                        if self._asm_alias_of(text, r.symbol, r.aliases):
                            r.existing_uses.append(dict(path=p, kind='asm-label alias of this function (accepted)'))
                        elif self.allow_existing_uses:
                            r.existing_uses.append(dict(path=p, kind='existing use accepted by --allow-existing-uses'))
                        else:
                            r.reason = r.reason or (f'{r.symbol} already used in code: {p} '
                                                    '(if that is this same function, re-run with --allow-existing-uses)')
            if r.reason and r.status == 'pending':
                self._refuse(r, r.reason); continue
            for p, stem in stems.items():
                d, base = os.path.split(p); ext = os.path.splitext(base)[1]
                np_ = (d + '/' if d else '') + r.symbol + ext
                if np_.lower() in lower_paths or np_.lower() in claimed:
                    self._refuse(r, f'file {np_} already exists (case-insensitively)'); break
                claimed[np_.lower()] = r; r.defining[p] = np_; r.files.setdefault(p, {})
            if r.status != 'pending': continue
            if r.kind == 'data' and not r.files and r.addr not in R.sa:
                self._refuse(r, f'no source references {"/".join(r.aliases)} and 0x{r.addr:X} is not a known function; '
                                'check the address'); continue
            # count planned edits per file
            for p in list(r.files):
                _, counts = rewrite(R.files[p], r.mapping())
                c = {}
                for o, kc in counts.items():
                    for k, v in kc.items(): c[k] = c.get(k, 0) + v
                r.files[p] = c
            # the file that matches this address (matched.csv source) must hold a definition spelled with one of the
            # swept names, or the rename would leave it emitting the old symbol (round 2: DMAS::AllocSw in dmas.cpp)
            defsrc = self.def_sources(r.addr) if r.kind == 'func' else []
            if r.kind == 'func':
                if not defsrc:
                    self._refuse(r, f'no config/matched.csv source for 0x{r.addr:06X}; cannot locate the definition'); continue
                bad = [p for p in defsrc if p not in R.files or not self._defines_any(R.files[p], r.aliases)]
                if bad:
                    self._refuse(r, f'the definition in {bad[0]} is not spelled {" or ".join(r.aliases)} (e.g. a C++ '
                                    'member or s1_ label); rename it by hand'); continue
            # which files must be rebuilt: any code/string edit, any file rename; headers -> their includers;
            # always the defining file (a whole merged TU: every function of it is compared by address)
            ver = set(defsrc)
            for p, c in r.files.items():
                if c.get('code') or c.get('string') or p in r.defining: ver.add(p)
            for h in [p for p in ver if not p.endswith(('.c', '.cpp'))]:
                ver.discard(h); ver |= self.includers(h)
            # comment-only edits cannot reach the compiler, but rebuilding them is cheap where possible
            ver |= {p for p, c in r.files.items() if p.endswith(('.c', '.cpp')) and not R.unbuildable(p)}
            r.verify = sorted(ver)
            r.unverifiable = sorted(p for p in ver if R.unbuildable(p))
            if r.unverifiable and not self.allow_unverifiable:
                self._refuse(r, f'touches code in {len(r.unverifiable)} file(s) this machine cannot build: '
                                + ', '.join(r.unverifiable[:3])); continue
            r.verify = [p for p in r.verify if p not in r.unverifiable]
            r.dups = sorted(p for p in r.files if re.search(r'\.(c|cpp)$', p) and self._defines(R.files[p], r.aliases))
            if len(r.dups) < 2: r.dups = []
            r.external = self.external_mentions(r.aliases)
            r.config = self.config_edit_list(r)
            for p in list(r.near): r.near[p] = sorted(set(r.near[p]))
            if not r.files and not r.config:
                r.status = 'already-applied'; r.reason = 'nothing left to change'
        return renames

    def _asm_alias_of(self, text, name, aliases):
        """True if every code use of `name` in text is a local alias bound by __asm__("<alias>") to this function."""
        A = r'(?:__asm__|__asm|asm)'
        decl = re.compile(r'(?<![%s])%s\s*\([^;{]*?\)\s*%s\s*\(\s*"(%s)"\s*\)' % (IDC, re.escape(name), A, _alts(aliases)), re.S)
        return bool(decl.search(text)) and not re.search(r'%s\s*\(\s*"(?!(%s)")' % (A, _alts(aliases)),
                                                         ''.join(m.group(0) for m in re.finditer(
                                                             r'(?<![%s])%s\s*\([^;{]*?\)\s*%s\s*\([^)]*\)' % (IDC, re.escape(name), A), text, re.S)))

    def def_sources(self, addr):
        """Source files that config/matched.csv (as of the start of the run) gives for this address."""
        out = []
        for line in self.orig_cfg['matched.csv'].splitlines()[1:]:
            p = line.rstrip('\r').split(',')
            if len(p) >= 5 and p[4] and re.fullmatch(r'0x[0-9A-Fa-f]+', p[1]) and int(p[1], 16) == addr:
                out.append(p[4])
        return sorted(set(out))

    def _defines_any(self, text, aliases):
        """_defines, or a definition through an asm name: `__asm__("OLD")` on a declaration, or an `OLD:` label in
        an asm string."""
        if self._defines(text, aliases): return True
        alts = _alts(aliases)
        if re.search(r'(?:__asm__|__asm|asm)\s*\(\s*"(%s)"\s*\)' % alts, text): return True
        lab = re.compile(r'(?<![%s])(%s)\s*:(?!:)' % (IDC, alts))
        return any(kind == 'string' and lab.search(text[a:b]) for kind, a, b in segments(text))

    def _defines(self, text, aliases):
        """True if text has a file-scope function definition of one of the aliases (brace depth 0, balanced
        parameter list, then optional attributes / asm label, then '{')."""
        if not any(a in text for a in aliases): return False
        code = blank_non_code(text)
        depth = []; d = 0
        for ch in code:
            depth.append(d)
            if ch == '{': d += 1
            elif ch == '}': d = max(0, d - 1)
        # extern "C" { ... } and namespace N { ... } blocks do not count as nesting. A definition inside a namespace
        # only counts when an extern "C" block also encloses it (the merged-file pattern
        # `namespace M_Func { extern "C" { ... } }`): otherwise its symbol carries the namespace and is not OLD.
        ext = {m.end() - 1 for m in re.finditer(r'extern\s*"C"\s*\{', text)}
        nsp = {m.end() - 1 for m in re.finditer(r'(?<![%s])namespace(?:\s+[A-Za-z_]\w*)?\s*\{' % IDC, code)}
        adj = [0] * len(code); in_ns = [False] * len(code); k = 0; stack = []
        for i, ch in enumerate(code):
            if ch == '{':
                kind = 'C' if i in ext else 'ns' if i in nsp else ''
                stack.append(kind); k += bool(kind)
            adj[i] = k
            in_ns[i] = 'ns' in stack and 'C' not in stack
            if ch == '}' and stack: k -= bool(stack.pop())
        for m in re.finditer(r'(?<![%s])(%s)\s*\(' % (IDC, _alts(aliases)), code):
            if depth[m.start()] - adj[m.start()] != 0 or in_ns[m.start()]: continue
            i = m.end(); n = 1
            while i < len(code) and n:
                n += {'(': 1, ')': -1}.get(code[i], 0); i += 1
            rest = code[i:i + 300]
            if re.match(r'\s*((__attribute__|__asm__|__asm|asm)\s*\((?:[^()]|\([^()]*\))*\)\s*)*\{', rest): return True
        return False

    def includers(self, header):
        base = os.path.basename(header); seen = set(); todo = [base]
        while todo:
            h = todo.pop()
            pat = re.compile(r'#\s*include\s*[<"]([^>"]*/)?%s[>"]' % re.escape(h))
            for p, t in self.repo.files.items():
                if p not in seen and pat.search(t):
                    seen.add(p)
                    if not p.endswith(('.c', '.cpp')): todo.append(os.path.basename(p))
        return {p for p in seen if p.endswith(('.c', '.cpp'))}

    def external_mentions(self, aliases):
        hits = []
        for rel, t in self._external_texts():
            if any(a in t and re.search(r'(?<![%s])%s(?![%s])' % (IDC, re.escape(a), IDC), t) for a in aliases):
                hits.append(rel)
        return sorted(hits)

    def _external_texts(self):
        if hasattr(self, '_ext'): return self._ext
        self._ext = []
        for g in EXTERNAL_GLOBS:
            full = os.path.join(self.repo.root, g)
            paths = [full] if os.path.isfile(full) else [os.path.join(dp, f) for dp, _, fs in os.walk(full) for f in fs]
            for fp in paths:
                if '__pycache__' in fp or 'compilers' in fp or fp.endswith(('.exe', '.svg', '.pyc')): continue
                if os.path.basename(fp) == 'rename.py' or '/tests/' in fp: continue
                try: t = open(fp, errors='ignore').read()
                except OSError: continue
                self._ext.append((os.path.relpath(fp, self.repo.root), t))
        return self._ext

    # ---- config edits (pure functions of the original config + active set) -------------------------------------
    def config_edit_list(self, r):
        R = self.repo; edits = []
        if R.sa.get(r.addr) != r.symbol:
            edits.append(f'symbol_addrs.txt: {"rename " + R.sa[r.addr] if r.addr in R.sa else "add"} -> {r.symbol} = 0x{r.addr:X}')
        if r.addr in R.fc and R.fc[r.addr][0] in r.aliases:
            edits.append(f'functions.csv: {R.fc[r.addr][0]} -> {r.symbol}')
        for line in R.cfg['matched.csv'].splitlines()[1:]:
            p = line.split(',')
            if len(p) >= 5 and (p[0] in r.aliases or p[4] in r.defining):
                edits.append(f'matched.csv: {p[0]},{p[4]}')
        return edits

    def render_config(self, active):
        sa = self.orig_cfg['symbol_addrs.txt']; fc = self.orig_cfg['functions.csv']; mc = self.orig_cfg['matched.csv']
        by_addr = {r.addr: r for r in active}
        moved = {p: np_ for r in active for p, np_ in r.defining.items()}
        names = {a: r.symbol for r in active for a in r.aliases}
        # symbol_addrs.txt
        lines = sa.split('\n'); done = set()
        for i, line in enumerate(lines):
            m = SA_RE.match(line)
            if m and int(m.group(4), 16) in by_addr:
                r = by_addr[int(m.group(4), 16)]
                if m.group(2) in r.aliases or m.group(2) == r.symbol:
                    lines[i] = m.group(1) + r.symbol + m.group(3) + m.group(4) + (m.group(5) or ''); done.add(r.addr)
        for r in sorted((r for r in active if r.addr not in done), key=lambda r: r.addr):
            new = f'{r.symbol} = 0x{r.addr:X}; // type:{"func" if r.kind == "func" else "data"}'
            at = None
            for i, line in enumerate(lines):      # before the first entry with a higher address
                m = SA_RE.match(line)
                if m and int(m.group(4), 16) > r.addr: at = i; break
            if at is None:
                at = len(lines)
                while at > 0 and lines[at - 1] == '': at -= 1
            lines.insert(at, new)
        sa = '\n'.join(lines)
        # functions.csv
        out = []
        for i, line in enumerate(fc.split('\n')):
            p = line.split(',')
            if i and len(p) >= 4 and p[0] in names and int(p[1], 16) in by_addr and p[0] in by_addr[int(p[1], 16)].aliases:
                p[0] = by_addr[int(p[1], 16)].symbol; line = ','.join(p)
            out.append(line)
        fc = '\n'.join(out)
        # matched.csv
        out = []
        for i, line in enumerate(mc.split('\n')):
            p = line.split(',')
            if i and len(p) >= 5:
                a = int(p[1], 16)
                if a in by_addr and p[0] in by_addr[a].aliases: p[0] = by_addr[a].symbol
                if p[4].rstrip('\r') in moved: p[4] = moved[p[4].rstrip('\r')] + ('\r' if p[4].endswith('\r') else '')
                line = ','.join(p)
            out.append(line)
        mc = '\n'.join(out)
        return {'symbol_addrs.txt': sa, 'functions.csv': fc, 'matched.csv': mc}

    # ---- materialize -------------------------------------------------------------------------------------------
    def desired(self, active):
        by_file = {}
        for r in active:
            for p in r.files: by_file.setdefault(p, []).append(r)
        want = {}
        for p, rs in by_file.items():
            mp = {}
            for r in rs: mp.update(r.mapping())
            text, _ = rewrite(self.orig_files[p], mp)
            np_ = p
            for r in rs:
                if p in r.defining: np_ = r.defining[p]
            want[p] = (np_, text)
        return want

    def materialize(self, active, all_renames):
        """Make the tree equal to original + `active` renames (only files any rename touches are considered)."""
        touched = {p for r in all_renames for p in r.files}
        want = self.desired(active)
        root = self.repo.root
        # config first: if we are interrupted, config already names the new symbol and a re-run (whose aliases include
        # the address placeholder) finishes the source edits instead of refusing them as a collision
        cfg = self.render_config(active)
        for k, v in cfg.items():
            if self.repo.cfg[k] != v:
                _atomic_write(os.path.join(root, 'config', k), v); self.repo.cfg[k] = v
        removals = []
        for p in sorted(touched):
            np_, text = want.get(p, (p, self.orig_files[p]))
            cur = self.state[p]
            cur_text = self.repo.files.get(cur)
            if np_ != cur or text != cur_text:
                _atomic_write(os.path.join(root, np_), text)
                if np_ != cur: removals.append(cur); self.repo.files.pop(cur, None)
                self.repo.files[np_] = text; self.state[p] = np_
        for cur in removals:
            if cur not in self.repo.files:
                try: os.remove(os.path.join(root, cur))
                except FileNotFoundError: pass
        self.repo.reindex()

    def cur(self, paths):
        return [self.state.get(p, p) for p in paths]

    # ---- run ---------------------------------------------------------------------------------------------------
    def run(self, renames, dry_run=False, verify=True, on_fail='revert', show_diff=False):
        for r in renames:
            if r.status == 'pending': self.resolve(r)
        self.plan(renames)
        pending = [r for r in renames if r.status == 'pending']
        if dry_run:
            for r in pending: r.status = 'planned'
            for r in renames: self.report_line(r)
            if show_diff: self.print_diff(pending)
            return 0
        if not pending:
            for r in renames: self.report_line(r)
            return 0
        baseline = {}
        if verify:
            allf = sorted({p for r in pending for p in r.verify})
            self.log(f'baseline: building {len(allf)} files ...')
            baseline = self.verifier(allf, self.repo, {})
            for r in pending:
                bad = [p for p in r.verify if baseline[p]['build'] != 'ok']
                if bad:
                    self._refuse(r, 'cannot verify, baseline build failed: ' + ', '.join(bad[:3]))
            pending = [r for r in pending if r.status == 'pending']
        accepted = list(pending)
        self.materialize(accepted, renames)
        if verify:
            allf = sorted({p for r in accepted for p in r.verify})
            self.log(f'verify: building {len(allf)} files after {len(accepted)} renames ...')
            after = self.verifier(self.cur(allf), self.repo, self.name_map(accepted))
            after = {p: after[self.state.get(p, p)] for p in allf}
            failed = {p for p in allf if not same_result(baseline[p], after[p])}
            for r in accepted: r.verification = {p: dict(before=baseline[p], after=after[p]) for p in r.verify}
            if failed:
                suspects = [r for r in accepted if failed & set(r.verify)]
                self.log(f'verify: {len(failed)} file(s) changed; {len(suspects)} suspect rename(s)')
                if on_fail == 'stop':
                    for r in suspects: r.status = 'failed'; r.reason = 'verification failed (stopped, tree left modified): ' + ', '.join(sorted(failed & set(r.verify))[:3])
                    for r in accepted:
                        if r.status == 'pending': r.status = 'applied'
                    for r in renames: self.report_line(r)
                    return 2
                keep = [r for r in accepted if r not in suspects]
                self.materialize(keep, renames)
                for r in suspects:                      # retry one at a time on top of what is accepted
                    trial = keep + [r]
                    self.materialize(trial, renames)
                    res = self.verifier(self.cur(r.verify), self.repo, self.name_map(trial))
                    res = {p: res[self.state.get(p, p)] for p in r.verify}
                    bad = [p for p in r.verify if not same_result(baseline[p], res[p])]
                    r.verification = {p: dict(before=baseline[p], after=res[p]) for p in r.verify}
                    if bad:
                        r.status = 'reverted'; r.reason = 'match changed in ' + ', '.join(bad[:4])
                        self.materialize(keep, renames)
                    else:
                        keep.append(r)
                accepted = keep
        for r in accepted: r.status = 'applied'
        for r in renames: r.final_paths = {p: self.state.get(p, p) for p in r.files}
        for r in renames: self.report_line(r)
        return 0 if all(r.status in ('applied', 'already-applied') for r in renames) else 1

    def name_map(self, active):
        return {a: r.symbol for r in active for a in r.aliases}

    def report_line(self, r):
        nf = len(r.files); nd = len(r.defining)
        tgt = r.symbol or r.new_mangled or r.new_name
        self.log(f'{r.status:15s} {("0x%06X" % r.addr) if r.addr is not None else r.key:>9} {r.old or "?"} -> {tgt}'
                 + (f'  [{r.policy}] {nf} files' + (f', file renamed' if nd else '') if r.status in ('pending', 'planned', 'applied') else '')
                 + (f'  ({r.reason})' if r.reason else ''))

    def print_diff(self, active):
        want = self.desired(active)
        for p in sorted(want):
            np_, text = want[p]
            sys.stdout.writelines(difflib.unified_diff(self.orig_files[p].splitlines(True), text.splitlines(True),
                                                       'a/' + p, 'b/' + np_))
        cfg = self.render_config(active)
        for k, v in cfg.items():
            sys.stdout.writelines(difflib.unified_diff(self.orig_cfg[k].splitlines(True), v.splitlines(True),
                                                       'a/config/' + k, 'b/config/' + k))

def same_result(before, after):
    if before['build'] != after['build']: return False
    return before['funcs'] == after['funcs']

def _atomic_write(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    tmp = path + '.rename-tmp'
    with open(tmp, 'w', newline='') as f: f.write(text)
    os.replace(tmp, path)

# --------------------------------------------------------------------------------------------------------------------
# verification through tools/matchtool.py
# --------------------------------------------------------------------------------------------------------------------
LINE_RE = re.compile(r'^(\S+) @0x([0-9A-Fa-f]+): (.*)$')

class MatchtoolVerifier:
    """Runs `matchtool.py check FILE` and returns {path: {build, funcs{addr|noaddr:name: status}, log}}.
    Keys are addresses so the result is comparable across a rename; statuses are MATCH / MATCH* / 'k/n differ'."""
    def __init__(self, root, jobs=8):
        self.root = root; self.jobs = jobs

    def one(self, rel, name_map):
        p = subprocess.run([sys.executable, os.path.join(self.root, 'tools', 'matchtool.py'), 'check', rel],
                           cwd=self.root, capture_output=True, text=True, timeout=600)
        funcs = {}; other = []
        for line in p.stdout.splitlines():
            m = LINE_RE.match(line)
            if m:
                st = m.group(3)
                st = st if st.startswith('MATCH') else st.split(' (')[0]
                funcs['0x%06X' % int(m.group(2), 16)] = st
            elif line.endswith('(add it to config/symbol_addrs.txt)'):
                n = line.split(':')[0]
                inv = {v: k for k, v in name_map.items()}
                funcs['noaddr:' + inv.get(n, n)] = 'NOADDR'
            elif not line.startswith(('!!', '  ')): other.append(line)
        build = 'ok' if p.returncode == 0 and (funcs or not other) else 'failed'
        return dict(build=build, funcs=funcs, log=(p.stdout + p.stderr)[-2000:] if build != 'ok' else '')

    def __call__(self, paths, repo, name_map):
        with ThreadPoolExecutor(self.jobs) as ex:
            res = list(ex.map(lambda q: self.one(q, name_map), paths))
        return dict(zip(paths, res))

# --------------------------------------------------------------------------------------------------------------------
def commit_message(renames):
    ap = [r for r in renames if r.status == 'applied']
    lines = [f'Rename {len(ap)} function{"s" if len(ap) != 1 else ""} (tools/rename.py; every touched file re-verified MATCH)', '']
    for r in ap:
        lines.append(f'  0x{r.addr:06X} {r.old} -> {r.symbol}' + ('' if r.policy == 'mangled' else '  (plain C-linkage name; signature unconfirmed)' if r.kind == 'func' else '  (data)'))
    other = [r for r in renames if r.status not in ('applied', 'already-applied')]
    if other:
        lines += ['', 'Not applied:'] + [f'  {r.key} -> {r.symbol or r.new_name}: {r.status}: {r.reason}' for r in other]
    return '\n'.join(lines) + '\n'

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0], formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    ap.add_argument('old', nargs='?'); ap.add_argument('new', nargs='?')
    ap.add_argument('--mangled', default=''); ap.add_argument('--csv')
    ap.add_argument('--dry-run', action='store_true'); ap.add_argument('--diff', action='store_true')
    ap.add_argument('--on-fail', choices=('revert', 'stop'), default='revert')
    ap.add_argument('--no-verify', action='store_true'); ap.add_argument('--allow-unverifiable', action='store_true')
    ap.add_argument('--allow-existing-uses', action='store_true')
    ap.add_argument('--log'); ap.add_argument('--commit-msg'); ap.add_argument('--jobs', type=int, default=8)
    ap.add_argument('--root', default=os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    a = ap.parse_args(argv)
    if a.csv: reqs = read_requests(a.csv)
    elif a.old and a.new: reqs = [Rename(0, a.old, a.new, a.mangled)]
    else: ap.error('give OLD NEW or --csv FILE')
    eng = Engine(a.root, jobs=a.jobs, allow_unverifiable=a.allow_unverifiable, allow_existing_uses=a.allow_existing_uses,
                 verifier=(lambda paths, repo, nm: {}) if a.no_verify else None)
    started = datetime.datetime.now(datetime.timezone.utc).isoformat()
    head = subprocess.run(['git', 'rev-parse', 'HEAD'], cwd=a.root, capture_output=True, text=True).stdout.strip()
    rc = eng.run(reqs, dry_run=a.dry_run, verify=not a.no_verify, on_fail=a.on_fail, show_diff=a.diff)
    counts = {}
    for r in reqs: counts[r.status] = counts.get(r.status, 0) + 1
    log = dict(tool='tools/rename.py', version=TOOL_VERSION, started=started,
               finished=datetime.datetime.now(datetime.timezone.utc).isoformat(), git_head=head, argv=sys.argv[1:] if argv is None else argv,
               dry_run=a.dry_run, verified=not a.no_verify, on_fail=a.on_fail, counts=counts,
               renames=[r.to_json() for r in reqs])
    lp = a.log or os.path.join(a.root, 'build', 'rename_log.json')
    os.makedirs(os.path.dirname(os.path.abspath(lp)), exist_ok=True)
    json.dump(log, open(lp, 'w'), indent=1)
    if a.commit_msg: open(a.commit_msg, 'w').write(commit_message(reqs))
    print('summary:', ', '.join(f'{k} {v}' for k, v in sorted(counts.items())), f'(log: {lp})')
    return rc

if __name__ == '__main__':
    sys.exit(main())
