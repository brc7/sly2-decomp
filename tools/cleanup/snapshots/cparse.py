"""Light C parser for the local view structs of one region (namespace or file) of a source file.
Members in order with census offsets (census sig) where the counts agree, else own layout (natural alignment)."""
import re, os, json, collections
import common as C

import snapcfg
_ct = os.path.join(snapcfg.get('census'), 'census_tiers.json')
CENSUS = json.load(open(_ct)) if os.path.exists(_ct) else []   # census_all.py output; optional (own layouts otherwise)
CBY = collections.defaultdict(list)
for e in CENSUS: CBY[e['file']].append(e)
SRC = {}
def src(f):
    if f not in SRC: SRC[f] = open(os.path.join(C.WT, f), errors='ignore').read()
    return SRC[f]

PRIM = {'char': 1, 'uchar': 1, 'u8': 1, 's8': 1, 'bool': 1, 'undefined1': 1, 'byte': 1, 'unsigned char': 1, 'signed char': 1,
        'short': 2, 'ushort': 2, 's16': 2, 'u16': 2, 'unsigned short': 2,
        'int': 4, 'uint': 4, 'unsigned': 4, 'unsigned int': 4, 's32': 4, 'u32': 4, 'float': 4, 'undefined4': 4,
        'uint32_t': 4, 'int32_t': 4, 'OID': 4, 'CID': 4, 'MSGID': 4, 'f32': 4, 'TICK': 8, 'ulong': 8, 'long': 8,
        's64': 8, 'u64': 8, 'qword': 16, 'u128': 16, 's128': 16}
FLOATS = {'float', 'f32'}

def strip_comments(s):
    s = re.sub(r'/\*.*?\*/', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), s, flags=re.S)
    return re.sub(r'//[^\n]*', lambda m: ' ' * len(m.group(0)), s)

def region(file, ns):
    s = strip_comments(src(file))
    if ns:
        i = s.find('namespace %s {' % ns)
        if i < 0: i = s.find('namespace %s\n{' % ns)
        if i >= 0:
            j = s.find('\nnamespace ', i + 10)
            k = s.find('} // namespace %s' % ns, i)
            end = k if k > 0 else (j if j > 0 else len(s))
            return s[i:end], i
    return s, 0

def match_brace(s, i):
    d = 0
    for k in range(i, len(s)):
        if s[k] == '{': d += 1
        elif s[k] == '}':
            d -= 1
            if d == 0: return k
    return len(s)

def split_members(body):
    """top-level ';'-separated declarations; nested struct/union bodies kept whole."""
    out, d, cur = [], 0, ''
    for ch in body:
        if ch == '{': d += 1
        if ch == '}': d -= 1
        if ch == ';' and d == 0: out.append(cur.strip()); cur = ''
        else: cur += ch
    return [x for x in out if x]

def parse_members(body):
    """-> [dict(name, base, ptr, arr, nested)]"""
    mem = []
    for decl in split_members(body):
        decl = re.sub(r'__attribute__\s*\(\(.*?\)\)', '', decl).strip()
        if not decl or decl.startswith(('typedef', 'static', 'friend', 'enum ')) or '(' in decl and ')' in decl and '{' not in decl and '(*' not in decl:
            continue
        if '{' in decl:          # nested struct/union: one member
            m = re.search(r'\}\s*(\w+)?\s*(\[[^\]]*\])?\s*$', decl)
            mem.append(dict(name=(m.group(1) if m else None) or '_anon', base='{nested}', ptr=0, arr=1, nested=True,
                            union=decl.lstrip().startswith('union'))); continue
        decl = re.sub(r'^(const|volatile|struct|union|class|unsigned(?=\s+\w+\s*[\w\*]))\s+', '', decl)
        parts = [x.strip() for x in decl.split(',')]
        m = re.match(r'^(.*?[\w>])([\s\*]+)\(?\s*\**\s*(\w+)\s*\)?\s*((?:\[[^\]]*\])*)\s*(:\s*\w+)?$', parts[0])
        if not m: mem.append(dict(name='?', base='?', ptr=0, arr=1, nested=False)); continue
        base = m.group(1).strip()
        decls = [parts[0][len(m.group(1)):]] + parts[1:]
        for d in decls:
            nm = re.search(r'(\w+)\s*\)?\s*((?:\[[^\]]*\])*)\s*(:\s*\w+)?\s*$', d)
            arr = 1
            for a_ in re.findall(r'\[([^\]]*)\]', d):
                try: arr *= int(eval(a_.replace('u', '').replace('U', ''), {}, {}))
                except Exception: arr *= 1
            mem.append(dict(name=nm.group(1) if nm else '?', base=base, ptr=d.count('*'), arr=arr, nested=False,
                            bits=bool(nm and nm.group(3))))
    return mem

def kind_of(mb):
    if mb['nested']: return 'nested'
    n = mb['name'].lower()
    if mb['ptr']: return 'ptr'
    if n.startswith(('pad', '_pad', 'unk', 'gap', 'filler', 'reserved', 'res', 'skip', 'u0', 'u1', 'x', 'dummy')) and mb['arr'] > 1: return 'pad'
    if mb['base'] in ('char', 'uchar', 'u8', 'undefined1', 'unsigned char') and mb['arr'] >= 8: return 'pad'
    if mb['base'] in FLOATS: return 'float'
    if mb['base'] in PRIM and PRIM[mb['base']] <= 4: return 'int'
    if mb['base'] in PRIM: return 'wide'
    return 'struct:' + mb['base']

def structs_in(file, ns):
    """all struct/union/class definitions in the region: name -> dict(name, line, members[...with off,size,kind], size)"""
    reg, base_off = region(file, ns)
    full = src(file)
    out = {}
    for m in re.finditer(r'\b(struct|union|class)\s+(\w+)\s*(?::\s*(?:public\s+)?(\w+)\s*)?\{', reg):
        k0 = m.end() - 1; k1 = match_brace(reg, k0)
        line = full.count('\n', 0, base_off + m.start()) + 1
        mem = parse_members(reg[k0 + 1:k1])
        name = m.group(2)
        if name in out: continue
        out[name] = dict(name=name, kind=m.group(1), base=m.group(3), line=line, members=mem)
    # offsets from census sig when the member counts agree
    cen = {(e['name'], e['line']): e for e in CBY.get(file, [])}
    for s in out.values():
        e = cen.get((s['name'], s['line'])) or next((x for x in CBY.get(file, []) if x['name'] == s['name'] and abs(x['line'] - s['line']) <= 2), None)
        s['census'] = e
        sig = e.get('sig') if e else None
        mem = s['members']
        if sig and (len(sig[1]) == len(mem) or (s['base'] and len(sig[1]) > len(mem))):
            for mb, (o, sz) in zip(mem, sig[1][len(sig[1]) - len(mem):]): mb['off'] = o; mb['esz'] = sz
            s['size'] = sig[0]; s['offsets'] = 'census'
        else:
            s['size'] = e['size'] if e else None; s['offsets'] = 'none' if not sig else 'mismatch'
        for mb in mem: mb['kind'] = kind_of(mb)
    return out
