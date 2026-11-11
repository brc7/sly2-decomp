"""Demangler for the old GNU (g++ 2.95, "gnu-v2") C++ ABI, enough for the Sly 2 symbols.

binutils dropped this style (c++filt -s gnu-v2 is gone), so this is a small reimplementation of the parts of
libiberty's cplus-dem.c that g++ 2.95 emits for functions:

  name__F<params>            free function            InitDl__FP2DLi        InitDl(DL*, int)
  name__<class><params>      member function          Free__11CLinearHeapPv CLinearHeap::Free(void*)
  name__C<class><params>     const member function
  __<class><params>          constructor              __1Ai                 A::A(int)
  _$_<class>  / _._<class>   destructor
  __<op>__<class|F><params>  operator                 __as__1ARC1A          A::operator=(A const&)
  __op<type>__<class>        conversion operator

Types: v b c s i l x f d r w (void bool char short int long longlong float double longdouble wchar), U/S/C/V
prefixes, P pointer, R reference, A<n-1>_ array, F<params>_<ret> function, M<class>F.. member pointer, <len>Name class,
Q<n> qualified name, t<len>Name<nargs>{Z<type>|<type><literal>} template, G<class> class by value, I<hex bits>
(I80 = 128-bit), e ellipsis. Repeats: T<i> repeats parameter i, N<count><i> repeats it count times; for a member
function index 0 is the class itself. Multi-digit T/N numbers end in '_'.
Static member functions mangle exactly like non-static ones (verified with the game's compiler), so `static` is
reported as unknown.

Canonical type strings (shared with the declaration parser): builtins as above (uint, uchar, schar, ushort, ulong,
longlong, ulonglong, longdouble), classes by name (NS::B, CRefPtr<CFrame>), then suffixes ' const', '*', '&',
'[N]'; function types as 'fn/<nparams>' (so a function pointer is 'fn/2*').
"""
import re

BUILTIN = {'v': 'void', 'b': 'bool', 'c': 'char', 's': 'short', 'i': 'int', 'l': 'long', 'x': 'longlong',
           'f': 'float', 'd': 'double', 'r': 'longdouble', 'w': 'wchar', 'e': '...'}
UNSIGNED = {'char': 'uchar', 'short': 'ushort', 'int': 'uint', 'long': 'ulong', 'longlong': 'ulonglong'}
OPS = {'nw': 'new', 'dl': 'delete', 'vn': 'new[]', 'vd': 'delete[]', 'as': '=', 'ne': '!=', 'eq': '==', 'ge': '>=',
       'gt': '>', 'le': '<=', 'lt': '<', 'pl': '+', 'apl': '+=', 'mi': '-', 'ami': '-=', 'ml': '*', 'aml': '*=',
       'dv': '/', 'adv': '/=', 'md': '%', 'amd': '%=', 'er': '^', 'aer': '^=', 'ad': '&', 'aad': '&=', 'or': '|',
       'aor': '|=', 'co': '~', 'nt': '!', 'ls': '<<', 'als': '<<=', 'rs': '>>', 'ars': '>>=', 'aa': '&&', 'oo': '||',
       'pp': '++', 'mm': '--', 'vc': '[]', 'cl': '()', 'rf': '->', 'rm': '->*', 'cm': ','}


class Fail(Exception):
    pass


class P:
    def __init__(self, s):
        self.s, self.i = s, 0

    def peek(self):
        return self.s[self.i] if self.i < len(self.s) else ''

    def take(self, n=1):
        if self.i + n > len(self.s): raise Fail('eof')
        r = self.s[self.i:self.i + n]; self.i += n
        return r

    def count(self):
        """consume_count: all digits"""
        m = re.match(r'\d+', self.s[self.i:])
        if not m: raise Fail('count')
        self.i += len(m.group(0))
        return int(m.group(0))

    def small(self):
        """get_count: one digit, or several followed by '_'"""
        m = re.match(r'(\d+)_', self.s[self.i:])
        if m and len(m.group(1)) > 1:
            self.i += len(m.group(0)); return int(m.group(1))
        if not self.peek().isdigit(): raise Fail('small')
        return int(self.take())


def name_part(p):
    """a class/namespace name: <len>Name, Q<n>..., t... ; returns the canonical name"""
    c = p.peek()
    if c == 'Q':
        p.take()
        if p.peek() == '_':
            p.take(); n = p.count()
            if p.peek() == '_': p.take()
        else:
            n = int(p.take())
        return '::'.join(name_part(p) for _ in range(n))
    if c == 't':
        p.take()
        n = p.count(); base = p.take(n)
        k = p.small()
        args = []
        for _ in range(k):
            if p.peek() == 'Z':
                p.take(); args.append(typ(p, []))
            else:
                t = typ(p, [])
                if t.endswith('*') or t.endswith('&'):
                    ln = p.count(); args.append('&' + p.take(ln))
                else:
                    neg = '-' if p.peek() == 'm' else ''
                    if neg: p.take()
                    args.append(neg + str(p.count()))
        return f'{base}<{",".join(args)}>'
    if c.isdigit():
        n = p.count()
        return p.take(n)
    raise Fail('name ' + c)


def typ(p, seen):
    c = p.peek()
    if c == 'C': p.take(); return typ(p, seen) + ' const'
    if c == 'V': p.take(); return typ(p, seen) + ' volatile'
    if c == 'u': p.take(); return typ(p, seen)                       # restrict
    if c == 'U': p.take(); t = typ(p, seen); return UNSIGNED.get(t, 'unsigned ' + t)
    if c == 'S': p.take(); t = typ(p, seen); return 'schar' if t == 'char' else t
    if c == 'J': p.take(); return 'complex ' + typ(p, seen)
    if c == 'P':
        p.take()
        if p.peek() == 'F':
            return fntype(p, seen) + '*'
        return typ(p, seen) + '*'
    if c == 'R': p.take(); return typ(p, seen) + '&'
    if c == 'A':
        p.take(); n = p.count()
        if p.take() != '_': raise Fail('array')
        return typ(p, seen) + f'[{n + 1}]'
    if c == 'F': return fntype(p, seen)
    if c == 'M':
        p.take(); cls = name_part(p)
        t = typ(p, seen)
        return f'{cls}::{t}' if not t.startswith('fn/') else f'mfn/{int(t.split("/")[1]) - 1}'
    if c == 'O':
        p.take(); cls = name_part(p); return f'{cls}::{typ(p, seen)}'
    if c == 'G': p.take(); return typ(p, seen)
    if c == 'X':
        p.take(); idx = int(p.take()); p.take()          # X<index><level>: a template parameter
        targs = getattr(p, 'targs', [])
        if idx >= len(targs): raise Fail('template parm')
        return targs[idx]
    if c == 'I':
        p.take()
        if p.peek() == '_':
            p.take(); bits = int(p.count()); p.take()
        else:
            bits = int(p.take(2), 16)
        return {8: 'char', 16: 'short', 32: 'int', 64: 'long', 128: 'longlong'}.get(bits, f'int{bits}')
    if c in BUILTIN: p.take(); return BUILTIN[c]
    if c in 'Qt' or c.isdigit():
        return name_part(p)
    raise Fail('type ' + c)


def fntype(p, seen):
    p.take()  # F
    args = params(p, [], stop='_')
    if p.peek() != '_': raise Fail('fn ret')
    p.take()
    typ(p, [])
    return f'fn/{len(args)}'


def params(p, seen, stop=''):
    """parameter list until end (or `stop`); seen is the type vector (for T/N), updated in place"""
    out = []
    while p.peek() and p.peek() != stop:
        c = p.peek()
        if c in 'TN':
            p.take()
            r = p.small() if c == 'N' else 1
            t = p.small()
            if t >= len(seen): raise Fail('repeat index')
            for _ in range(r):
                out.append(seen[t]); seen.append(seen[t])
            continue
        t = typ(p, seen)
        out.append(t); seen.append(t)
    if out == ['void']: out = []
    return out


def demangle(sym):
    """dict(name, cls, const, params, kind) or None if `sym` is not a mangled function name"""
    if not sym or '__' not in sym and not sym.startswith(('_$_', '_._')):
        return None
    for prefix in ('_$_', '_._'):
        if sym.startswith(prefix):
            try:
                p = P(sym[3:]); cls = name_part(p)
                if p.peek(): return None
                return dict(name='~' + cls.split('::')[-1].split('<')[0], cls=cls, const=False, params=[], kind='dtor')
            except Fail:
                return None
    cands = []
    if sym.startswith('__'):
        m = re.match(r'__(op(.+?)|[a-z]{2,3})__(.+)$', sym)
        if m and (m.group(1) in OPS or m.group(2)):
            cands.append(('operator' + (OPS.get(m.group(1)) or ' ' + m.group(2)), m.group(3), 'op'))
        cands.append(('', sym[2:], 'ctor'))
    else:
        for m in re.finditer(r'(?=__)', sym):
            i = m.start()
            if i > 0: cands.append((sym[:i], sym[i + 2:], 'func'))
    for name, rest, kind in cands:
        r = _sig(name, rest, kind)
        if r: return r
    return None


def _sig(name, rest, kind):
    p = P(rest)
    try:
        const = False
        while p.peek() in ('S', 'C', 'V', 'u') and p.i + 1 < len(p.s) and (p.s[p.i + 1].isdigit() or p.s[p.i + 1] in 'QtCVS'):
            if p.take() == 'C': const = True
        if p.peek() == 'F':
            if kind == 'ctor': return None
            p.take()
            ps = params(p, [])
            if p.peek(): return None
            return dict(name=name, cls=None, const=False, params=ps, kind=kind)
        if p.peek() == 'H':          # template function: H<n><args>_<params>_<return>
            p.take(); k = p.small(); p.targs = []
            for _ in range(k):
                if p.take() != 'Z': return None
                p.targs.append(typ(p, []))
            if p.take() != '_': return None
            ps = params(p, [], stop='_')
            if p.take() != '_': return None
            ret = typ(p, [])
            if p.peek(): return None
            return dict(name=name, cls=None, const=False, params=ps, kind=kind, ret=ret,
                        targs=p.targs)
        cls = name_part(p)
        seen = [cls]
        ps = params(p, seen)
        if p.peek(): return None
        if kind == 'ctor': name = cls.split('::')[-1].split('<')[0]
        return dict(name=name, cls=cls, const=const, params=ps, kind=kind)
    except (Fail, ValueError, IndexError):
        return None


def pretty(d):
    q = (d['cls'] + '::') if d['cls'] else ''
    if d.get('targs'): q = f"{d.get('ret')} " + q
    return f"{q}{d['name']}({', '.join(d['params'])}){' const' if d['const'] else ''}"


if __name__ == '__main__':
    import sys
    for s in sys.argv[1:] or (l.strip() for l in sys.stdin):
        d = demangle(s)
        print(f'{s}\t{pretty(d) if d else "?"}')
