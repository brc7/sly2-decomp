"""Find the struct / class / union definitions a TU declares at namespace scope and parse their members.

Only definitions whose enclosing scopes are all namespaces or `extern "C" {}` blocks are returned (a struct inside a
function body cannot be named from outside, so the compiler probe cannot lay it out); they are counted apart.
Both branches of an `#if` are seen (preprocessor lines are blanked): a second definition of the same qualified name
is marked `dup`.
"""
import re
from common import strip_comments, match_brace

HEAD_STRUCT = re.compile(r'(?:^|[\s;{}])(typedef\s+)?(struct|class|union)\s*(\w*)\s*(:\s*[^{;()]*)?$', re.S)
HEAD_NS = re.compile(r'(?:^|[\s;{}])namespace\s*(\w*)\s*$')
HEAD_EXTERN = re.compile(r'(?:^|[\s;{}])extern\s*"\s*"\s*$')
PAD_NAME = re.compile(r'^(_?pad|unk(?!nown)|_+\d|p[0-9A-Fa-f]+$|filler|gap|data$|raw$|reserved)', re.I)


def _stmt_start(t, i):
    """Start of the statement head that ends at t[i] (the '{')."""
    j = i - 1
    while j >= 0 and t[j] not in ';{}':
        j -= 1
    return j + 1


def scan(text):
    """[(record dict)] for every namespace-scope struct/class/union definition in `text` (original source)."""
    t = strip_comments(text)
    out, stack, local = [], [], 0
    i, n = 0, len(t)
    line_of = _line_index(text)
    while i < n:
        c = t[i]
        if c == '}':
            if stack:
                stack.pop()
            i += 1; continue
        if c != '{':
            i += 1; continue
        hs = _stmt_start(t, i)
        head = t[hs:i]
        m_ns, m_ex = HEAD_NS.search(head), HEAD_EXTERN.search(head)
        m_st = HEAD_STRUCT.search(head)
        if m_ns:
            stack.append(('ns', m_ns.group(1))); i += 1; continue
        if m_ex:
            stack.append(('extern', '')); i += 1; continue
        end = match_brace(t, i)
        if m_st and 'template' not in head and not re.search(r'\benum\b', head):
            if all(k in ('ns', 'extern') for k, _ in stack):
                semi = t.find(';', end)
                tail = t[end:semi] if semi >= 0 else ''
                # `struct S {...} var, *p;` or `typedef struct {...} S;`
                decl_names = re.findall(r'(\**)\s*(\w+)', tail.split('=')[0]) if tail.strip() else []
                name = m_st.group(3)
                tdname = decl_names[0][1] if m_st.group(1) and decl_names else ''
                if not name and tdname:
                    name = tdname
                st = hs + m_st.start(2) if not m_st.group(1) else hs + m_st.start(1)
                rec = dict(kind=m_st.group(2), name=name, typedef=tdname, ns=[s for k, s in stack if k == 'ns'],
                           base=(m_st.group(4) or '').lstrip(':').strip(), start=st, open=i, close=end,
                           end=(semi + 1 if semi >= 0 else end), body=t[i + 1:end - 1], line=line_of(st),
                           endline=line_of(end), vars=[] if m_st.group(1) else [d[1] for d in decl_names])
                rec['members'], rec['flags'] = members(rec['body'])
                out.append(rec)
            else:
                local += 1
        i = end
    seen = set()
    for r in out:
        q = qual(r)
        r['dup'] = q in seen
        seen.add(q)
    return out, local


def _line_index(text):
    starts = [0] + [m.end() for m in re.finditer('\n', text)]
    import bisect
    return lambda off: bisect.bisect_right(starts, off)


def qual(r):
    return '::'.join([s for s in r['ns'] if s] + [r['name']])


def split_top(s, sep):
    out, d, cur = [], 0, []
    for ch in s:
        if ch in '{([':
            d += 1
        elif ch in '})]':
            d -= 1
        if ch == sep and d == 0:
            out.append(''.join(cur)); cur = []
        else:
            cur.append(ch)
    out.append(''.join(cur))
    return out


TYPE_RE = re.compile(r'^((?:(?:const|volatile|unsigned|signed|struct|union|class|enum|mutable)\s+)*'
                     r'[A-Za-z_][\w:]*(?:\s*<[^>]*>)?(?:\s+(?:int|long|char|short|double))*)\s*(.*)$', re.S)
DECL_RE = re.compile(r'^([\*\s&]*(?:const\s*)?)(\w+)\s*((?:\[[^\]]*\]\s*)*)(:\s*[^=]+)?\s*(=.*)?$', re.S)


def members(body, prefix=()):
    """Parse a struct body. Returns ([member], flags). member: dict(name, type, ptr, arr, bits, anon, nested)."""
    out, flags = [], set()
    for stmt in split_top(body, ';'):
        s = stmt.strip()
        s = re.sub(r'^(public|private|protected)\s*:\s*', '', s)
        if not s:
            continue
        if '{' in s:
            m = re.match(r'^(struct|union|class)\s*(\w*)\s*(:[^{]*)?\{(.*)\}\s*(.*)$', s, re.S)
            if not m:
                flags.add('method')     # an inline member function body
                continue
            sub, f2 = members(m.group(4), prefix)
            flags |= f2
            decl = m.group(5).strip()
            if not decl:
                for x in sub:
                    x['anon'] = True
                out += sub
            else:
                for d in split_top(decl, ','):
                    dm = DECL_RE.match(d.strip())
                    if dm:
                        out.append(dict(name=dm.group(2), type='<%s>' % m.group(1), ptr=dm.group(1).count('*'),
                                        arr=dm.group(3).strip(), bits='', anon=False, nested=sub,
                                        decl=d.strip()))
            continue
        if re.match(r'^(static|typedef|enum|friend|using|template)\b', s):
            if s.startswith('typedef'):
                flags.add('typedef')
            continue
        if re.search(r'\bvirtual\b', s):
            flags.add('virtual'); continue
        fp = re.search(r'\(\s*\*\s*(\w+)\s*\)\s*\(', s)
        if fp:
            out.append(dict(name=fp.group(1), type='<fnptr>', ptr=1, arr='', bits='', anon=False, nested=None, decl=s))
            continue
        if '(' in s:
            flags.add('method'); continue
        parts = split_top(s, ',')
        tm = TYPE_RE.match(parts[0].strip())
        if not tm:
            continue
        ty, rest = tm.group(1), tm.group(2)
        if not rest.strip() or rest.strip().startswith('['):
            # `unsigned x` style: the last word was the declarator
            ws = ty.split()
            if len(ws) < 2:
                continue
            ty, rest = ' '.join(ws[:-1]), ws[-1] + rest
        for k, d in enumerate([rest] + parts[1:]):
            dm = DECL_RE.match(d.strip())
            if not dm:
                continue
            out.append(dict(name=dm.group(2), type=re.sub(r'^(struct|union|class|enum)\s+', '', ty.strip()),
                            ptr=dm.group(1).count('*'), arr=dm.group(3).strip(), bits=(dm.group(4) or '').strip(),
                            anon=False, nested=None, decl=(ty + ' ' + d.strip())))
    return out, flags


def is_pad(m):
    return bool(PAD_NAME.match(m['name'])) and (m['type'] in ('char', 'unsigned char', 'uchar', 'u8', 'byte', 'int',
                                                            'uint', 'undefined1', 'undefined4', 'u32', 'u_char')
                                                 or m['name'].lower().startswith('pad'))


def functions_in(text_stripped, a, b):
    """Names of functions defined in text[a:b] (identifier before a parameter list followed by a body)."""
    seg = text_stripped[a:b]
    names = []
    for m in re.finditer(r'\b(\w+)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*(?:const\s*)?(?:__asm__\s*\(\s*"\s*"\s*\)\s*)?\{', seg):
        nm = m.group(1)
        if nm in ('if', 'while', 'for', 'switch', 'return', 'sizeof', 'catch', '__asm__', 'asm'):
            continue
        names.append(nm)
    return names
