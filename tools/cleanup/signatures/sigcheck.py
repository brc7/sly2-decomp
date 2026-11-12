#!/usr/bin/env python3
"""Mangled-name signature check: does each declaration that binds to a g++ 2.95 mangled function symbol agree with
the signature the mangled name encodes?

The decomp often declares a function under its mangled linker name with C-level types of its own choosing:
  void func_001703F8(void *pgsb, int a, int b) __asm__("InitGsb__FP3GSBii");   // asm label
  extern "C" int IsEntryInDl__FP2DLPv(void *, void *);                                // C name = mangled name
The compiler cannot check either: the label (or C linkage) bypasses mangling, so the declared parameter types are
whatever the author wrote. That is harmless for matching but a 64-bit port compiles these declarations natively,
where an int standing in for a pointer truncates it. This tool lists every such disagreement.

  sigcheck.py [--root REPO] [--map LINKMAP] [--out DIR] [--top N]

1. Symbols: every name in config/symbol_addrs.txt, config/functions.csv, config/matched.csv (and the link map if
   given; default build/link/sly2.map when present) that demangles as a function (demangle.py).
2. Declarations: every function declaration or definition in src/ and include/ (a small C/C++ declaration parser:
   scopes, typedefs per include closure, struct/enum names). A declaration binds to a symbol through an asm label,
   or because its own name is the mangled symbol. Plain C++ declarations whose name, class and parameter types
   equal the demangled signature are its "canonical" declarations (the compiler mangled them, so they are right by
   construction); they give the return type, which the mangled name does not encode.
3. Comparison on the ABI-level parameter list: for a member function the class pointer (`this`, `C const*` for a
   const member) comes first; a declaration inside a class gets its class pointer the same way. Since g++ 2.95
   mangles static members like non-static ones, a free declaration may match either form.

Classes, most severe first: wrong-count, int-vs-pointer (an integer where the symbol has a pointer or reference, or
the reverse: the 64-bit hazard), wrong-type (pointer/reference/class to another class, or pointer depth),
return (return type differs from the canonical declaration; int-vs-pointer returns are ranked as int-vs-pointer),
other (scalar kind or width, function pointer arity, ellipsis), void-pointer (T* declared as void*), alias
(enum vs int, reference vs pointer: same ABI), const.

Output (DIR, default build/signatures/): sigcheck.csv (one row per binding declaration with a mismatch),
cross_tu.csv (symbols whose declarations disagree with each other), summary.md (totals, the top findings ranked by
severity with file:line, and the cross-declaration conflicts). Exit 0 always (a report, not a gate).
"""
import argparse
import collections
import csv
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from demangle import demangle, pretty   # noqa: E402

# ------------------------------------------------------------------ tokenizer

TOK = re.compile(r'''
    (?P<id>[A-Za-z_$][A-Za-z0-9_$]*)
  | (?P<num>\.?\d[\w.]*(?:[eE][+-]\d+)?)
  | (?P<str>"(?:\\.|[^"\\\n])*")
  | (?P<chr>'(?:\\.|[^'\\\n])*')
  | (?P<op>::|->\*?|\.\.\.|<<=?|>>=?|[-+*/%&|^!=<>]=|&&|\|\||\+\+|--|[^\s])
''', re.X)


def strip_comments_pp(text):
    """blank comments and preprocessor lines (keeping newlines); return (text, [include names])"""
    out, i, n = [], 0, len(text)
    incs = []
    bol = True
    while i < n:
        c = text[i]
        if bol:
            j = i
            while j < n and text[j] in ' \t': j += 1
            if j < n and text[j] == '#':
                k = j
                while True:
                    e = text.find('\n', k)
                    if e < 0: e = n
                    if e > 0 and text[e - 1] == '\\' and e < n: k = e + 1; continue
                    break
                line = text[j:e]
                m = re.match(r'#\s*include\s*[<"]([^>"]+)[>"]', line)
                if m: incs.append(m.group(1))
                out.append('\n' * text.count('\n', i, e)); i = e; continue
        bol = False
        if text.startswith('//', i):
            e = text.find('\n', i); e = n if e < 0 else e
            i = e; continue
        if text.startswith('/*', i):
            e = text.find('*/', i + 2); e = n if e < 0 else e + 2
            out.append('\n' * text.count('\n', i, e) or ' '); i = e; continue
        if c in '"\'':
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            out.append(text[i:j + 1]); i = j + 1; continue
        if c == '\n': bol = True
        out.append(c); i += 1
    return ''.join(out), incs


EE_MACROS = {'SLY2_EE_LAYOUT', 'SLY2_EE_ASM', '__R5900__'}


def pp_regions(text):
    """per line (index = line number): None (compiled on both targets), 'ee' (only on the EE: a native build skips it)
    or 'native' (only natively), from #if / #ifdef / #ifndef / #elif / #else / #endif on the EE macros (EE_MACROS:
    defined on the EE, undefined natively); other conditions count as true on both"""
    def ev(cond, ee):
        c = re.sub(r'defined\s*\(\s*(\w+)\s*\)|defined\s+(\w+)',
                   lambda m: '1' if ee and (m.group(1) or m.group(2)) in EE_MACROS else '0', cond)
        if not any(x in cond for x in EE_MACROS): return None
        c = re.sub(r'\b[A-Za-z_]\w*\b', lambda m: '1' if ee and m.group(0) in EE_MACROS else '0', c)
        c = c.replace('&&', ' and ').replace('||', ' or ').replace('!', ' not ').replace(' not =', '!=')
        try: return bool(eval(c, {}))
        except Exception: return None
    lines = text.split('\n')
    state = [None] * (len(lines) + 2)
    stack = []                   # [ee active, native active, ee taken, native taken]
    cur = (True, True)
    for i, ln in enumerate(lines, 1):
        m = re.match(r'\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)', ln)
        if m:
            k, cond = m.group(1), m.group(2).split('//')[0].split('/*')[0].strip()
            if k in ('if', 'ifdef', 'ifndef'):
                if k == 'ifdef': cond = f'defined({cond})'
                elif k == 'ifndef': cond = f'!defined({cond})'
                e, n = ev(cond, True), ev(cond, False)
                e = True if e is None else e
                n = True if n is None else n
                stack.append([e, n, e, n, cur])
            elif k == 'elif' and stack:
                t = stack[-1]
                e, n = ev(cond, True), ev(cond, False)
                e = True if e is None else e
                n = True if n is None else n
                t[0], t[1] = e and not t[2], n and not t[3]
                t[2], t[3] = t[2] or e, t[3] or n
            elif k == 'else' and stack:
                t = stack[-1]
                t[0], t[1] = not t[2], not t[3]
                t[2] = t[3] = True
            elif k == 'endif' and stack:
                stack.pop()
            ee = all(t[0] for t in stack)
            na = all(t[1] for t in stack)
            cur = (ee, na)
            continue
        ee, na = cur
        state[i] = None if ee == na else ('ee' if ee else 'native')
    return state


CUR_PP = [None]


def tokenize(text):
    toks = []
    line = 1
    pos = 0
    for m in TOK.finditer(text):
        line += text.count('\n', pos, m.start()); pos = m.start()
        toks.append((m.group(0), line))
    return toks


# ------------------------------------------------------------------ types

BUILTIN_WORDS = {'void', 'bool', 'char', 'short', 'int', 'long', 'float', 'double', 'signed', 'unsigned', 'wchar_t',
                 '__int128'}
CV = {'const', 'volatile'}
TAGS = {'struct', 'class', 'union', 'enum', 'typename'}
STORAGE = {'static', 'extern', 'inline', '__inline__', '__inline', 'virtual', 'explicit', 'friend', 'register',
           'auto', 'mutable', '__extension__'}
NOT_NAMES = BUILTIN_WORDS | CV | TAGS | STORAGE | {
    'return', 'if', 'while', 'for', 'switch', 'sizeof', 'case', 'else', 'do', 'goto', 'new', 'delete', 'throw',
    'typedef', 'using', 'namespace', 'template', 'default', 'break', 'continue', 'asm', '__asm__', '__asm',
    '__attribute__', 'operator', 'public', 'private', 'protected', '__typeof__', 'typeof', 'this'}
INTS = {'char', 'schar', 'uchar', 'short', 'ushort', 'int', 'uint', 'long', 'ulong', 'longlong', 'ulonglong', 'bool',
        'wchar'}


def builtin(words, tim=False):
    w = collections.Counter(words)
    uns = w['unsigned'] > 0
    if w['void']: return 'void'
    if w['bool']: return 'bool'
    if w['float']: return 'float'
    if w['double']: return 'longdouble' if w['long'] else 'double'
    if w['wchar_t']: return 'wchar'
    if tim or w['__int128']: return 'ulonglong' if uns else 'longlong'
    if w['char']: return 'uchar' if uns else ('schar' if w['signed'] else 'char')
    if w['short']: return 'ushort' if uns else 'short'
    if w['long'] >= 2: return 'ulonglong' if uns else 'longlong'
    if w['long']: return 'ulong' if uns else 'long'
    return 'uint' if uns else 'int'


def split_top(toks, sep=','):
    parts, cur, d = [], [], 0
    for t in toks:
        if t in ('(', '[', '{'): d += 1
        elif t in (')', ']', '}'): d -= 1
        elif t == '<': d += 1
        elif t == '>': d -= 1
        elif t == '>>': d -= 2
        if t == sep and d == 0:
            parts.append(cur); cur = []
        else:
            cur.append(t)
    if cur or parts: parts.append(cur)
    return parts


def match_close(toks, i, o='(', c=')'):
    d = 0
    for j in range(i, len(toks)):
        if toks[j] == o: d += 1
        elif toks[j] == c:
            d -= 1
            if d == 0: return j
    return -1


class Types:
    """name resolution for one file: typedefs visible through its include closure, then any typedef"""

    def __init__(self, typedefs, glob, enums):
        self.td, self.glob, self.enums = typedefs, glob, enums

    def lookup(self, name):
        return self.td.get(name) or self.glob.get(name)


def parse_type(toks, types, want_name=False):
    """canonical type of a parameter/typedef token list; returns (type or None, name)"""
    toks = [t for t in toks if t not in ('register', '__restrict', '__restrict__', 'restrict')]
    tim = False
    if '__attribute__' in toks:
        i = toks.index('__attribute__')
        j = match_close(toks, i + 1)
        attr = toks[i:j + 1]
        tim = 'mode' in attr and any(a in ('TI', '__TI__') for a in attr)
        toks = toks[:i] + toks[j + 1:]
    if not toks: return None, None
    name = None
    # function pointer / pointer to array:  R ( [C::] * [name] ) ( params )   or  R ( * name ) [ N ]
    if '(' in toks:
        i = toks.index('(')
        j = match_close(toks, i)
        grp = toks[i + 1:j]
        if j < 0: return None, None
        rest = toks[j + 1:]
        mptr = '::' in grp
        ids = [t for t in grp if re.match(r'[A-Za-z_]\w*$', t) and t not in CV]
        if mptr and len(ids) >= 2: name = ids[-1]
        elif not mptr and ids: name = ids[-1]
        if '*' not in grp and '&' not in grp:
            return None, None
        if rest[:1] == ['(']:
            k = match_close(rest, 0)
            ps = split_top(rest[1:k])
            n = 0 if not ps or ps == [['void']] or ps == [[]] else len(ps)
            return (f'mfn/{n}*' if mptr else f'fn/{n}*'), name
        if rest[:1] == ['[']:
            base, _ = parse_type(toks[:i], types)
            dims = ''.join(f'[{x}]' for x in re.findall(r'\[\s*(\w+)\s*\]', ' '.join(rest)))
            return (base + dims + '*') if base else None, name
        return None, None
    # arrays after the name: first dimension decays
    dims = []
    while toks and toks[-1] == ']':
        k = len(toks) - 1 - toks[::-1].index('[')
        dims.insert(0, ''.join(toks[k + 1:-1])); toks = toks[:k]
    # parameter name
    if len(toks) > 1 and re.match(r'[A-Za-z_$][\w$]*$', toks[-1]) and toks[-1] not in NOT_NAMES and \
            toks[-2] not in TAGS and toks[-2] != '::':
        rem = toks[:-1]
        if any(t in BUILTIN_WORDS or (re.match(r'[A-Za-z_]\w*$', t) and t not in CV | TAGS) for t in rem):
            name = toks[-1]; toks = rem
    # base specifiers, then declarator operators
    base_words, base_name, cv_base, ops = [], None, False, []
    i = 0
    while i < len(toks):
        t = toks[i]
        if t in CV:
            if base_words or base_name: ops.append(' const' if t == 'const' else '')
            else: cv_base = cv_base or t == 'const'
        elif t in TAGS:
            pass
        elif t in BUILTIN_WORDS and not base_name:
            base_words.append(t)
        elif t in ('*', '&'):
            ops.append(t)
        elif re.match(r'[A-Za-z_$][\w$]*$', t) and not base_name and not base_words:
            # qualified / template name
            nm = [t]
            while i + 2 < len(toks) and toks[i + 1] == '::':
                nm.append(toks[i + 2]); i += 2
            if i + 1 < len(toks) and toks[i + 1] == '<':
                k = i + 1; d = 0
                while k < len(toks):
                    if toks[k] == '<': d += 1
                    elif toks[k] == '>': d -= 1
                    elif toks[k] == '>>': d -= 2
                    if d <= 0: break
                    k += 1
                args = split_top(toks[i + 2:k])
                targs = []
                for a in args:
                    if len(a) == 1 and re.match(r'-?\d', a[0]): targs.append(a[0].rstrip('uUlL'))
                    else: targs.append(parse_type(a, types)[0] or '?')
                nm[-1] += '<' + ','.join(targs) + '>'
                i = k
            base_name = '::'.join(nm)
        elif t == '...':
            return '...', None
        else:
            return None, name
        i += 1
    if base_words:
        base = builtin(base_words, tim)
    elif base_name:
        r = types.lookup(base_name) if types else None
        base = r if r else base_name
        if tim and base in ('int', 'uint', 'longlong', 'ulonglong', 'long', 'ulong'):
            base = 'ulonglong' if base.startswith('u') else 'longlong'
    else:
        return None, name
    s = base + (' const' if cv_base and not base.endswith(' const') else '')
    for o in ops:
        s += o
    if dims:
        s += ''.join(f'[{d}]' for d in dims[1:]) + '*'
    return s, name


def drop_top_const(t):
    return t[:-6] if t and t.endswith(' const') else t


# ------------------------------------------------------------------ declarations

class Decl:
    __slots__ = ('file', 'line', 'name', 'qual', 'scope', 'member', 'static', 'ret', 'params', 'const', 'label',
                 'is_def', 'unparsed', 'variadic', 'templ', 'cname', 'adapted', 'cdecl', 'pp', 'native_label')

    def flat(self, cls_this=True):
        p = list(self.params)
        if self.member and not self.static and cls_this:
            p.insert(0, self.member + (' const*' if self.const else '*'))
            if self.name.startswith('~'): p.append('int')     # g++ 2.95 destructors take __in_chrg
        return p

    def sig(self):
        q = f'{self.member}::' if self.member else (f'{self.qual}::' if self.qual else '')
        return f"{self.ret or ''} {q}{self.name}({', '.join(self.params)}){' const' if self.const else ''}".strip()


def scan_file(path, rel, types_for, record):
    text = open(path, errors='replace').read()
    CUR_PP[0] = pp_regions(text)
    text, _ = strip_comments_pp(text)
    toks = tokenize(text)
    T = types_for(rel)
    stack = [('file', None)]
    seg = []
    depth = 0
    init = 0
    last_closed = None
    for tok, line in toks:
        if init:
            seg.append((tok, line))
            if tok == '{': init += 1
            elif tok == '}': init -= 1
            continue
        if tok in ('(', '['): depth += 1
        elif tok in (')', ']'): depth -= 1
        if depth == 0 and tok in (';', '{', '}'):
            words = [t for t, _ in seg]
            if tok == '{':
                if '=' in words and 'operator' not in words:
                    init = 1; seg.append((tok, line)); continue
                kind = scope_of(words)
                if kind[0] == 'func':
                    handle_decl(seg, stack, T, rel, record, is_def=True)
                stack.append(kind)
                last_closed = None
            elif tok == ';':
                if last_closed and last_closed[0] == 'class' and last_closed[3]:   # typedef struct X {...} Y, *PY;
                    record('typedef_after', rel, last_closed, words)
                elif words[:1] == ['typedef']:
                    record('typedef', rel, None, words)
                else:
                    handle_decl(seg, stack, T, rel, record, is_def=False)
                last_closed = None
            else:
                if len(stack) > 1:
                    last_closed = stack.pop()
                if seg:
                    handle_decl(seg, stack, T, rel, record, is_def=False)
            seg = []
            continue
        seg.append((tok, line))


def scope_of(words):
    w = [x for x in words if x not in ('public', 'private', 'protected', ':')] if words[:2] in (
        ['public', ':'], ['private', ':'], ['protected', ':']) else words
    if w[:1] == ['namespace']:
        return ('ns', w[1] if len(w) > 1 else '', False, False)
    if w[:2] == ['extern', '"C"'] and len(w) == 2:
        return ('externC', None, False, False)
    td = 'typedef' in w
    top = []
    d = 0
    for x in w:
        if x == '(': d += 1
        elif x == ')': d -= 1
        elif d == 0: top.append(x)
    if '(' not in w or (any(k in top for k in ('struct', 'class', 'union', 'enum')) and
                        not re.search(r'\)\s*(const\s*)?$', ' '.join(w))):
        for k in ('struct', 'class', 'union', 'enum'):
            if k in top:
                i = top.index(k)
                nm = top[i + 1] if i + 1 < len(top) and re.match(r'[A-Za-z_]\w*$', top[i + 1]) else ''
                if nm == '__attribute__': nm = ''
                return ('class', nm, td, k == 'enum', 'template' in w)
    if '(' in w:
        return ('func', None, False, False)
    return ('block', None, False, False)


def enclosing(stack):
    """(member class, namespace qualifier, in template class, in function body)"""
    cls, ns, templ, body = [], [], False, False
    for s in stack:
        if s[0] == 'class' and not s[3]:
            cls.append(s[1] or '?'); templ = templ or (len(s) > 4 and s[4])
        elif s[0] == 'ns':
            ns.append(s[1])
        elif s[0] in ('func', 'block'):
            body = True; cls = []
    return ('::'.join(ns + cls) if cls else None), ('::'.join(ns) if ns else None), templ, body


def handle_decl(seg, stack, T, rel, record, is_def):
    toks = [t for t, _ in seg]
    lines = [l for _, l in seg]
    i = 0
    cdecl = any(s[0] == 'externC' for s in stack)
    while True:
        if toks[i:i + 2] in (['public', ':'], ['private', ':'], ['protected', ':']): i += 2; continue
        if toks[i:i + 1] == ['template'] and toks[i + 1:i + 2] == ['<']:
            d = 0
            for j in range(i + 1, len(toks)):
                if toks[j] == '<': d += 1
                elif toks[j] == '>': d -= 1
                elif toks[j] == '>>': d -= 2
                if d <= 0: break
            i = j + 1; continue
        if toks[i:i + 2] == ['extern', '"C"']: i += 2; cdecl = True; continue
        if i < len(toks) and re.match(r'SLY2_[A-Z0-9_]+$', toks[i]) and toks[i + 1:i + 2] == ['(']:
            j = match_close(toks, i + 1)           # a statement-like macro without a semicolon (SLY2_BUILTIN_C,
            if 0 < j < len(toks) - 1: i = j + 1; continue   # SLY2_INSTANTIATE_SLOT_POOLS) before a declaration
        break
    toks, lines = toks[i:], lines[i:]
    if not toks or 'typedef' in toks[:1] or '(' not in toks: return
    label = None
    native_label = None
    adapted = False
    out, ol = [], []
    j = 0
    while j < len(toks):
        t = toks[j]
        # SLY2_SRET_LABEL("ee", "native") (sly2/port.h): the EE asm label; natively an adapter with this spelling
        if t == 'SLY2_SRET_LABEL' and toks[j + 1:j + 2] == ['('] and j + 2 < len(toks) and toks[j + 2].startswith('"'):
            label = toks[j + 2][1:-1]; adapted = True
            if toks[j + 3:j + 4] == [','] and toks[j + 4:j + 5] and toks[j + 4].startswith('"'):
                native_label = toks[j + 4][1:-1]
            j = match_close(toks, j + 1) + 1; continue
        if t in ('__asm__', 'asm', '__asm') and toks[j + 1:j + 2] == ['('] and j + 3 < len(toks) and \
                toks[j + 2].startswith('"') and toks[j + 3] == ')':
            label = toks[j + 2][1:-1]; j += 4; continue
        if t == '__attribute__' and toks[j + 1:j + 2] == ['(']:
            j = match_close(toks, j + 1) + 1; continue
        if t == 'throw' and toks[j + 1:j + 2] == ['(']:
            j = match_close(toks, j + 1) + 1; continue
        out.append(t); ol.append(lines[j]); j += 1
    toks, lines = out, ol
    # the declarator: first top-level '(' after a name with only type tokens before it
    d = 0
    for k, t in enumerate(toks):
        if t == '(' and d == 0: break
        if t == '<': d += 1
        elif t == '>': d -= 1
        elif t == '>>': d -= 2
    else:
        return
    if k == 0: return
    nm_end = k
    name = toks[k - 1]
    pre = toks[:k - 1]
    if name == '~' or (len(pre) and pre[-1] == '~'):
        if name == '~': return
        name = '~' + name; pre = pre[:-1]
    elif 'operator' in toks[:k]:
        o = toks.index('operator')
        name = 'operator' + ''.join(toks[o + 1:k]) if toks[o + 1:k] and toks[o + 1] not in BUILTIN_WORDS else \
            'operator ' + ' '.join(toks[o + 1:k])
        pre = toks[:o]
    elif not re.match(r'[A-Za-z_$][\w$]*$', name) or name in NOT_NAMES:
        return
    qual = []
    while len(pre) >= 2 and pre[-1] == '::':
        qual.insert(0, pre[-2]); pre = pre[:-2]
    for t in pre:
        if not (t in ('*', '&', '<', '>', '>>', ',', '::') or re.match(r'[A-Za-z_$][\w$]*$', t) or re.match(r'\d', t)):
            return
        if t in ('return', 'if', 'while', 'for', 'switch', 'case', 'else', 'do', 'goto', 'new', 'delete', 'throw',
                 'sizeof', 'using', 'namespace'):
            return
    close = match_close(toks, k)
    if close < 0: return
    rest = toks[close + 1:]
    const = False
    while rest and rest[0] in CV:
        const = const or rest[0] == 'const'; rest = rest[1:]
    if rest[:2] == ['=', '0']: rest = rest[2:]
    if rest and not (is_def and rest[0] == ':'):
        return
    static = 'static' in pre
    friend = 'friend' in pre
    ret_toks = [t for t in pre if t not in STORAGE]
    member, ns, templ, body = enclosing(stack)
    if friend: member = None
    if qual and not member:
        member = '::'.join(qual)       # out-of-class definition A::f
    is_ctor = name.startswith('~') or (member and name == member.split('::')[-1].split('<')[0])
    if not ret_toks and not is_ctor and not name.startswith('operator '):
        return
    ret = None
    if ret_toks:
        ret, _ = parse_type(ret_toks, T)
        if ret is None: return
    plist = split_top(toks[k + 1:close])
    params, unparsed, variadic = [], False, False
    if plist and plist != [['void']] and plist != [[]]:
        for p in plist:
            if '=' in p: p = p[:p.index('=')]
            t, _ = parse_type(p, T)
            if t is None:
                if p and all(re.match(r'[\d.\'"-]', x) or x in ('+', '-') for x in p):
                    return             # a constructor call with literal arguments, not a declaration
                unparsed = True; t = '?' + ' '.join(p)
            if t == '...': variadic = True
            params.append(drop_top_const(t))
    dl = Decl()
    dl.file, dl.line, dl.name, dl.qual = rel, lines[k - 1], name, ns
    dl.scope = 'body' if body else 'file'
    dl.member, dl.static, dl.ret = member, static, ret
    dl.params, dl.const, dl.label, dl.is_def = params, const, label, is_def
    dl.unparsed, dl.variadic, dl.templ = unparsed, variadic, templ
    dl.cname = name if re.match(r'[A-Za-z_$][\w$]*$', name) else None
    dl.cdecl, dl.native_label = cdecl, native_label
    dl.adapted = adapted or (cdecl and dl.cname is not None and dl.cname in BUILTIN_C.get(rel, ()))
    pp = CUR_PP[0]
    dl.pp = pp[dl.line] if pp and dl.line < len(pp) else None
    record('decl', rel, dl, None)


# ------------------------------------------------------------------ comparison

RANK = ['wrong-count', 'int-vs-pointer', 'struct-return', 'wrong-type', 'return', 'other', 'void-pointer',
        'address-only', 'sret-adapted', 'derived', 'alias', 'const']
BENIGN = {'alias', 'const', 'derived', 'void-pointer'}
BUILTIN_C = {}   # file -> symbols named by SLY2_BUILTIN_C(sym) there (bif.cpp's C-style builtins)
BASES = {}   # class -> base classes (struct X : Y)


def derives(a, b, depth=0):
    if depth > 12: return False
    return any(x == b or derives(x, b, depth + 1) for x in BASES.get(a, ()))


def is_ptr(t):
    return t.endswith('*') or t.endswith('&')


def strip_const(t):
    return t.replace(' const', '')


def pointee(t):
    return t[:-1]


def classify(a, b, enums):
    """a: declared type, b: type from the mangled name. None if equal."""
    if a == b: return None
    if a.startswith('?'): return ('other', f'unparsed parameter {a[1:]!r} vs {b}')
    if strip_const(a) == strip_const(b): return ('const', f'{a} vs {b}')
    sa, sb = strip_const(a), strip_const(b)
    ia = sa in INTS or sa in enums
    ib = sb in INTS or sb in enums
    if (is_ptr(sa) and ib) or (is_ptr(sb) and ia):
        return ('int-vs-pointer', f'{a} vs {b}')
    if is_ptr(sa) and is_ptr(sb):
        if sa[:-1] == sb[:-1]: return ('alias', f'{a} vs {b} (reference vs pointer)')
        pa, pb = sa[:-1], sb[:-1]
        if pa == 'void' or pb == 'void':
            return ('void-pointer', f'{a} vs {b}')
        if pa.count('*') != pb.count('*'): return ('wrong-type', f'{a} vs {b} (pointer depth)')
        if derives(pa, pb) or derives(pb, pa):
            return ('derived', f'{a} vs {b} ({pa if derives(pa, pb) else pb} derives from {pb if derives(pa, pb) else pa})')
        if pa.startswith('fn/') or pb.startswith('fn/'): return ('other', f'{a} vs {b}')
        if pa in INTS and pb in INTS or pa in ('float', 'double') or pb in ('float', 'double'):
            return ('other', f'{a} vs {b}')
        return ('wrong-type', f'{a} vs {b}')
    if ia and ib:
        if sa in enums or sb in enums: return ('alias', f'{a} vs {b} (enum vs integer)')
        return ('other', f'{a} vs {b} (integer kind)')
    if sa in INTS | {'float', 'double', 'void'} or sb in INTS | {'float', 'double', 'void'}:
        return ('other', f'{a} vs {b}')
    return ('wrong-type', f'{a} vs {b}')


def compare_decl(dl, sym, enums, ref_ret):
    """findings [(class, detail)] of declaration dl against demangled sym"""
    if dl.unparsed and all(p.startswith('?') for p in dl.params):
        pass
    decl_flat = dl.flat()
    if sym['kind'] == 'dtor':          # g++ 2.95 destructors take (this, int __in_chrg)
        variants = [[sym['cls'] + '*', 'int']]
    elif sym['cls']:
        this = [sym['cls'] + (' const*' if sym['const'] else '*')]
        variants = [this + sym['params'], list(sym['params'])]   # member, or static member / namespace function
    else:
        variants = [list(sym['params'])]
    best = None
    sret = False
    for v in variants:
        f = []
        if not dl.params and not dl.variadic and v and not dl.member:
            f.append(('address-only', f'declared with no parameters vs {len(v)}: only its address can be used'))
        elif len(decl_flat) != len(v) and not ('...' in v or '...' in decl_flat):
            cs = [classify(x, y, enums) for x, y in zip(decl_flat[1:], v)]
            adapted = getattr(dl, 'adapted', False)
            if adapted and len(v) == len(decl_flat) + 1 and v and is_ptr(v[0]) and dl.ret and dl.ret != 'void':
                # the reverse form: the symbol's explicit out-pointer taken as a struct result (SLY2_SRET_LABEL)
                cs = [classify(x, y, enums) for x, y in zip(decl_flat, v[1:])]
                f.append(('struct-return', f'struct result {dl.ret} for the explicit out-pointer {v[0]}'))
                f += [(c[0], f'param {i + 1}: {c[1]}') for i, c in enumerate(cs) if c]
                sret_v = True
            elif len(decl_flat) == len(v) + 1 and decl_flat and is_ptr(decl_flat[0]) and \
                    (adapted or all(c is None or c[0] in BENIGN for c in cs)):
                f.append(('struct-return', f'explicit leading return-slot pointer {decl_flat[0]} (the hidden '
                                           f'struct-return argument; arm64 passes it in x8, not x0)'))
                f += [(c[0], f'param {i + 1}: {c[1]}') for i, c in enumerate(cs) if c]
                sret_v = True
            else:
                sret_v = False
                f.append(('wrong-count', f'{len(decl_flat)} parameters vs {len(v)}'))
                for a, b in zip(decl_flat, v):
                    c = classify(a, b, enums)
                    if c: f.append(c)
        else:
            sret_v = False
            for idx, (a, b) in enumerate(zip(decl_flat, v)):
                c = classify(a, b, enums)
                if c: f.append((c[0], f'param {idx}: {c[1]}'))
        score = (len([x for x in f if x[0] == 'wrong-count']), sum(len(RANK) - RANK.index(x[0]) for x in f))
        if best is None or score < best[0]:
            best = (score, f); sret = any(x[0] == 'struct-return' for x in f)
    f = best[1]
    if getattr(dl, 'adapted', False):
        # bound through SLY2_SRET_LABEL / SLY2_BUILTIN_C: the hand-passed result pointer is the EE spelling only;
        # natively the declaration reaches an adapter with its own ABI (sly2/port.h)
        f = [('sret-adapted', x) if c == 'struct-return' else (c, x) for c, x in f]
    if ref_ret and dl.ret and dl.ret != ref_ret and not (sret and dl.ret in ('void', ref_ret + '*')):
        c = classify(dl.ret, ref_ret, enums)
        if c:
            f.append(('int-vs-pointer' if c[0] == 'int-vs-pointer' else ('return' if c[0] not in ('const', 'alias', 'void-pointer') else c[0]),
                      f'return: {c[1]}'))
    return f


def worst(fs):
    return min((RANK.index(c) for c, _ in fs), default=len(RANK))


# ------------------------------------------------------------------ main

def load(root, mp=None):
    """parse the config symbols and every declaration of src/ and include/: a dict with names, syms (demangled),
    decls (Decl), enums, classes, labels_seen, mangled_like (shared by sigcheck.py and sretcheck.py)"""
    # symbols
    names = set()
    for line in open(os.path.join(root, 'config/symbol_addrs.txt')):
        m = re.match(r'\s*(\S+)\s*=', line)
        if m: names.add(m.group(1))
    for r in csv.DictReader(open(os.path.join(root, 'config/functions.csv'), newline='')): names.add(r['Name'])
    for r in csv.DictReader(open(os.path.join(root, 'config/matched.csv'), newline='')): names.add(r['name'])
    mp = mp or os.path.join(root, 'build/link/sly2.map')
    if os.path.exists(mp):
        for line in open(mp, errors='replace'):
            for t in re.findall(r'[A-Za-z_$][\w$.]*__[\w$]+|_\$_\w+', line): names.add(t)
    syms = {}
    for n in names:
        d = demangle(n)
        if d: syms[n] = d
    mangled_like = {n for n in names if '__' in n and not n.startswith('__') or n.startswith(('_$_', '_._'))}

    # files, includes, typedefs
    files = []
    for top in ('include', 'src'):
        for dp, ds, fs in os.walk(os.path.join(root, top)):
            ds.sort()
            for f in sorted(fs):
                if f.endswith(('.c', '.cpp', '.h', '.hpp', '.inc')):
                    files.append(os.path.relpath(os.path.join(dp, f), root))
    byname = {}
    for f in files: byname.setdefault(os.path.basename(f), f)
    incs = {}
    for f in files:
        _, il = strip_comments_pp(open(os.path.join(root, f), errors='replace').read())
        res = []
        for i in il:
            for c in (os.path.normpath(os.path.join(os.path.dirname(f), i)), os.path.join('include', i)):
                if c in byname.values() or os.path.isfile(os.path.join(root, c)):
                    res.append(c); break
            else:
                if os.path.basename(i) in byname: res.append(byname[os.path.basename(i)])
        incs[f] = res

    def closure(f):
        seen, st = [], [f]
        while st:
            for h in incs.get(st.pop(), []):
                if h not in seen: seen.append(h); st.append(h)
        return seen

    # pass 1: typedefs, enums, class names (resolved with a global table)
    raw_td = collections.defaultdict(dict)     # file -> name -> tokens
    enums, classes = set(), set()
    decls = []

    def rec1(kind, rel, obj, words):
        if kind == 'typedef':
            parts = split_top(words[1:])
            if not parts: return
            first = parts[0]
            spec = []
            for t in first:
                if t in ('*', '&') or (spec and re.match(r'[A-Za-z_]\w*$', t) and t not in BUILTIN_WORDS | CV | TAGS
                                       and (spec[-1] not in TAGS) and any(x in BUILTIN_WORDS or (re.match(r'[A-Za-z_]\w*$', x) and x not in CV | TAGS) for x in spec)):
                    break
                spec.append(t)
            for i, p in enumerate(parts):
                toks = p if i == 0 else spec + p
                ids = [t for t in toks if re.match(r'[A-Za-z_]\w*$', t) and t not in NOT_NAMES]
                if '(' in toks:
                    _, nm = parse_type(toks, None)
                else:
                    nm = ids[-1] if ids else None
                if nm: raw_td[rel][nm] = toks
        elif kind == 'typedef_after':
            cls = obj[1]
            for p in split_top(words):
                ids = [t for t in p if re.match(r'[A-Za-z_]\w*$', t)]
                if not ids: continue
                nm = ids[-1]
                if not cls: cls = nm
                raw_td[rel][nm] = (['enum', cls] if obj[3] else ['struct', cls]) + [t for t in p if t in ('*', '&')] + [nm]
                if obj[3]: enums.add(nm)
                else: classes.add(nm)
        elif kind == 'decl':
            decls.append(obj)

    labels_seen = []

    # collect class/enum names quickly by regex (the scanner records typedef'd ones)
    for f in files:
        txt, _ = strip_comments_pp(open(os.path.join(root, f), errors='replace').read())
        for k, n, b in re.findall(r'\b(struct|class|union|enum)\s+([A-Za-z_]\w*)\s*(:[^;{]*)?\{', txt):
            (enums if k == 'enum' else classes).add(n)
            if b and k != 'enum':
                BASES.setdefault(n, set()).update(x for x in re.findall(r'[A-Za-z_]\w*', b)
                                                  if x not in ('public', 'private', 'protected', 'virtual'))
        for m in re.finditer(r'(?:__asm__|\basm)\s*\(\s*"([^"]+)"\s*\)', txt):
            if demangle(m.group(1)): labels_seen.append((f, txt.count('\n', 0, m.start()) + 1, m.group(1)))
        for m in re.finditer(r'^\s*SLY2_BUILTIN_C\(\s*(\w+)\s*\)', txt, re.M):
            BUILTIN_C.setdefault(f, set()).add(m.group(1))

    glob_raw = {}
    for f in files:
        scan_file(os.path.join(root, f), f, lambda rel: None, rec1)
    for f in files:
        for n, toks in raw_td[f].items():
            if n not in glob_raw or f.startswith('include/'): glob_raw.setdefault(n, toks)
    decls.clear()

    resolved = {}

    def resolve_in(td_raw, name, depth=0):
        toks = td_raw.get(name) or glob_raw.get(name)
        if toks is None or depth > 10: return None
        key = (id(toks), name)
        if key in resolved: return resolved[key]

        class _T:
            def lookup(self, n):
                if n == name: return None
                return resolve_in(td_raw, n, depth + 1)
        t, _ = parse_type(toks, _T())
        resolved[key] = t
        return t

    tcache = {}

    def types_for(rel):
        if rel not in tcache:
            vis = {}
            for h in reversed(closure(rel)):
                vis.update(raw_td.get(h, {}))
            vis.update(raw_td.get(rel, {}))

            class _T:
                def lookup(self, n):
                    return resolve_in(vis, n)
            tcache[rel] = _T()
        return tcache[rel]

    for f in files:
        scan_file(os.path.join(root, f), f, types_for, rec1)

    return dict(names=names, syms=syms, decls=decls, enums=enums, classes=classes, labels_seen=labels_seen,
                mangled_like=mangled_like)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0], epilog=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--root', default=os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))))))
    ap.add_argument('--map')
    ap.add_argument('--out')
    ap.add_argument('--top', type=int, default=40)
    a = ap.parse_args()
    root = os.path.abspath(a.root)
    out = os.path.abspath(a.out or os.path.join(root, 'build', 'signatures'))
    os.makedirs(out, exist_ok=True)

    L = load(root, a.map)
    names, syms, decls, enums, labels_seen, mangled_like = (
        L['names'], L['syms'], L['decls'], L['enums'], L['labels_seen'], L['mangled_like'])

    # bind declarations to symbols
    bound = collections.defaultdict(list)
    plain = collections.defaultdict(list)
    for dl in decls:
        if dl.label and dl.label in syms:
            bound[dl.label].append(('asm-label', dl))
        elif dl.label:
            continue
        elif dl.cname and dl.cname in syms and not dl.member:
            bound[dl.cname].append(('c-name', dl))
        else:
            plain[(dl.name, dl.member or dl.qual)].append(dl)
    # canonical plain declarations and reference return types
    canon = {}
    for s, d in syms.items():
        key = (d['name'], d['cls'])
        c = [dl for dl in plain.get(key, []) if dl.params == d['params'] and not dl.templ]
        if c: canon[s] = c
    rows, cross = [], []
    nbound = 0
    for s in sorted(set(bound) | set(canon)):
        d = syms[s]
        refs = [dl.ret for dl in canon.get(s, []) if dl.ret]
        ref_ret = d.get('ret') or (collections.Counter(refs).most_common(1)[0][0] if refs else None)
        for how, dl in bound.get(s, []):
            nbound += 1
            fs = compare_decl(dl, d, enums, ref_ret)
            if fs:
                rows.append(dict(symbol=s, demangled=pretty(d), file=dl.file, line=dl.line, binding=how,
                                 scope=('member of ' + dl.member) if dl.member else 'free', decl=dl.sig(),
                                 worst=RANK[worst(fs)], classes=';'.join(sorted({c for c, _ in fs}, key=RANK.index)),
                                 details=' | '.join(x for _, x in fs), _w=worst(fs)))
        # cross-declaration conflicts: distinct ABI signatures among all declarations of the symbol
        alld = [('canonical', dl) for dl in canon.get(s, [])] + bound.get(s, [])
        if len(alld) > 1:
            variants = collections.OrderedDict()
            for how, dl in alld:
                k = (tuple(dl.flat()), drop_top_const(dl.ret or ''))
                variants.setdefault(k, []).append((how, dl))
            if len(variants) > 1:
                ks = list(variants)
                base = ks[0]
                fsw = []
                for k in ks[1:]:
                    if len(k[0]) != len(base[0]): fsw.append(('wrong-count', ''))
                    for a2, b2 in zip(k[0], base[0]):
                        c = classify(a2, b2, enums)
                        if c: fsw.append(c)
                    if k[1] and base[1] and k[1] != base[1]:
                        c = classify(k[1], base[1], enums)
                        if c: fsw.append(('int-vs-pointer' if c[0] == 'int-vs-pointer' else 'return', c[1]))
                tus = {dl.file for _, dl in alld}
                cross.append(dict(symbol=s, demangled=pretty(d), variants=len(variants), files=len(tus),
                                  worst=RANK[worst(fsw)] if fsw else 'const', _w=worst(fsw),
                                  sites=' || '.join(f"{dl.ret or ''} ({', '.join(k[0])}) @ " +
                                                    ', '.join(f'{x.file}:{x.line}' for _, x in v[:4]) +
                                                    (f' +{len(v) - 4}' if len(v) > 4 else '')
                                                    for k, v in variants.items() for dl in [v[0][1]])))
    rows.sort(key=lambda r: (r['_w'], r['symbol'], r['file'], r['line']))
    cross.sort(key=lambda r: (r['_w'], -r['variants'], r['symbol']))
    cols = ['worst', 'classes', 'symbol', 'demangled', 'file', 'line', 'binding', 'scope', 'decl', 'details']
    with open(os.path.join(out, 'sigcheck.csv'), 'w', newline='') as f:
        w = csv.DictWriter(f, cols, extrasaction='ignore', lineterminator='\n'); w.writeheader(); w.writerows(rows)
    with open(os.path.join(out, 'cross_tu.csv'), 'w', newline='') as f:
        w = csv.DictWriter(f, ['worst', 'symbol', 'demangled', 'variants', 'files', 'sites'], extrasaction='ignore',
                           lineterminator='\n')
        w.writeheader(); w.writerows(cross)
    by_class = collections.Counter(r['worst'] for r in rows)
    any_class = collections.Counter(c for r in rows for c in r['classes'].split(';'))
    sym_class = collections.Counter()
    for s in {r['symbol'] for r in rows}:
        sym_class[RANK[min(r['_w'] for r in rows if r['symbol'] == s)]] += 1
    unparsed = sum(1 for _, l in ((h, dl) for v in bound.values() for h, dl in v) if l.unparsed)
    md = [f'# Signature check: declarations vs mangled names', '',
          f'Generated by tools/cleanup/signatures/sigcheck.py. Symbols: {len(names)} names, {len(syms)} demangle as '
          f'functions ({len(mangled_like) - len([n for n in mangled_like if n in syms])} mangled-looking names do '
          f'not). Declarations parsed: {len(decls)}; binding declarations (asm label or mangled C name): {nbound} '
          f'for {len(bound)} symbols ({unparsed} with a parameter the parser could not read); symbols with a '
          f'canonical C++ declaration: {len(canon)}.', '',
          f'Mismatching binding declarations: {len(rows)} ({len({r["symbol"] for r in rows})} symbols). '
          f'Cross-declaration conflicts: {len(cross)} symbols.', '',
          '| class | declarations (worst class) | declarations (any) | symbols (worst) |', '|---|---|---|---|']
    for c in RANK:
        md.append(f'| {c} | {by_class.get(c, 0)} | {any_class.get(c, 0)} | {sym_class.get(c, 0)} |')
    bysym = collections.OrderedDict()
    for r in rows: bysym.setdefault(r['symbol'], []).append(r)
    ranked = sorted(bysym.items(), key=lambda kv: (kv[1][0]['_w'], -len(kv[1]), kv[0]))
    md += ['', f'## Top {a.top} symbols', '',
           'One line per symbol: its worst class, how many declarations disagree, the first such declaration and '
           'where the others are.', '',
           '| class | symbol (demangled) | sites | first declaration | details | where |', '|---|---|---|---|---|---|']
    for s_, rs in ranked[:a.top]:
        r = rs[0]
        md.append(f"| {r['worst']} | `{r['demangled']}` | {len(rs)} | `{r['decl']}` | {r['details'].replace('|', '/')} | "
                  + ', '.join(f"{x['file']}:{x['line']}" for x in rs[:3]) + (f' +{len(rs) - 3}' if len(rs) > 3 else '') + ' |')
    seen_lab = {(dl.file, dl.label) for v in bound.values() for h, dl in v if h == 'asm-label'}
    missed = [x + ('no such symbol in the config' if x[2] not in syms else 'not parsed',)
              for x in labels_seen if (x[0], x[2]) not in seen_lab]
    with open(os.path.join(out, 'unparsed.csv'), 'w', newline='') as f:
        w = csv.writer(f, lineterminator='\n'); w.writerow(['file', 'line', 'label', 'why']); w.writerows(missed)
    nos = sum(1 for x in missed if x[3].startswith('no such'))
    md += ['', f'Function asm labels in the tree: {len(labels_seen)}; naming a function no config file has: {nos}; '
           f'not read as a declaration by the parser: {len(missed) - nos} (both listed in unparsed.csv).']
    md += ['', f'## Cross-declaration conflicts (top {a.top})', '',
           'Declarations of the same symbol (asm-labelled, C-named or canonical C++) with different ABI signatures.', '',
           '| class | symbol | variants | sites |', '|---|---|---|---|']
    for r in cross[:a.top]:
        md.append(f"| {r['worst']} | `{r['symbol']}` | {r['variants']} | {r['sites'].replace('|', '/')} |")
    open(os.path.join(out, 'summary.md'), 'w').write('\n'.join(md) + '\n')
    print(f'{nbound} binding declarations for {len(bound)} symbols; {len(rows)} mismatching '
          f'({len({r["symbol"] for r in rows})} symbols); {len(cross)} cross-declaration conflicts')
    print('by worst class: ' + ', '.join(f'{c} {by_class.get(c, 0)}' for c in RANK))
    print(f'outputs: {out}/sigcheck.csv, cross_tu.csv, summary.md')
    return 0


if __name__ == '__main__':
    sys.exit(main())
