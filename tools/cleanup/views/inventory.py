#!/usr/bin/env python3
"""View inventory: every TU-private struct in src/game that overlaps a header class by EE offsets, with its target
class, the member mapping, the functions using it and whether it is on the port's load / frame path.

Usage:
  inventory.py [--root REPO] [--out DIR] [--offsets OFFSETS.json] [--load-path CSV] [--frame-files a,b,..]
               [--targets TARGETS.json] [--jobs N] [FILE...]

Inputs:
  OFFSETS.json  header layouts from tools/cleanup/census/dump_offsets.py (default DIR/offsets.json; built if absent).
  TARGETS.json  hand annotations {"file.cpp:NS::VIEW": "CLASS", ...} (default: targets.json next to this script);
                a key without a namespace ("air.cpp:ACTOR") applies to every same-named view in that file.
  CSV           load-path functions (column `name`); FILE names in --frame-files (base names) count as frame path.
Outputs (DIR, default REPO/build/views): views.json (every struct, `candidate` marks the views; input of rewrite.py), views.csv (one row per view),
probe_failures.txt.

Method:
  1. parse.py finds the namespace-scope struct definitions of each TU and their members;
  2. a probe copy of the TU (written next to it as .viewprobe_<name>, removed afterwards) appends
     `extern "C" int __vp_K_J[2] = { SLY2_OFFSETOF(NS::VIEW, m), sizeof(...m) };` per member and is compiled to
     assembly with the matching compiler: the EE offsets and sizes come from ee-gcc itself, never from a size table.
     Members the compiler rejects (bit-fields...) are dropped and the probe retried;
  3. a struct is a *view candidate* if it has a pad member, a header-class base, a vptr word, a header class's name
     (or <CLASS>V / <CLASS>View / <CLASS>Island) or a hand annotation;
  4. target class: hand table > name > base (restricts to the base's descendants) > comment naming a class >
     propagation (a view member that is a pointer to another view, or embeds one, mapped onto a header member that
     is a pointer to / an instance of class C, makes C the other view's target; if the other view's used members do
     not all fit C, its unique least derived descendant that holds them all) > member-offset fit (every named member starting at a
     member of C with the same size; among classes in one inheritance chain the least derived one that holds every
     used member wins). Confidence: high (hand / name / comment / propagation hint, or a base class plus a fit of
     3+ used members in its subtree; all used members map),
     medium (offset fit alone, unique up to one chain, >= 2 used members), low (anything else);
  5. member mapping against the class's flattened layout (bases, anonymous unions, embedded structs, array
     elements): exact (same offset and size; among aliases the same type, then pointers, then the shortest path),
     "in pad" (inside a pad / unknown block of the class), "straddles <member>", "stride mismatch" (array element
     sizes differ), "beyond" (past the class's size), "vptr" (the EE vptr word at +0xC of a BASIC class).
"""
import argparse, collections, csv, json, os, re, sys
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common, parse

QWORDISH = {'qword', 'QWROW', 'U', 'VECTOR4', 'VU_FLOAT', 'VU_VECTOR4', 'VU_QWORD', 'u128', 'Q', 'QW', 'V4'}
NAME_SUFFIX = re.compile(r'^(\w+?)(V|View|_view|Island|V\d|2)$')


# ---------------------------------------------------------------- compiler probe
def probe_layouts(rel, recs, root):
    """Fill rec['size'] and member['off'], ['size'] for each record (EE compiler). Returns a list of failures."""
    src = os.path.join(root, rel)
    text = open(src).read()
    lines, keys = [], []
    for k, r in enumerate(recs):
        if not r['name'] or r['dup'] or 'virtual' in r['flags']:
            continue
        q = parse.qual(r)
        lines.append('extern "C" int __vp_%d_s = sizeof(%s);' % (k, q)); keys.append((k, None))
        for j, m in enumerate(_flat_members(r['members'])):
            if m['bits']:
                continue
            lines.append('extern "C" int __vp_%d_%d[2] = { SLY2_OFFSETOF(%s, %s), sizeof(((%s *)0)->%s) };'
                         % (k, j, q, m['name'], q, m['name']))
            keys.append((k, j))
    if not lines:
        return []
    d, b = os.path.split(src)
    psrc = os.path.join(d, '.viewprobe_' + b)
    pasm = psrc + '.s'
    # #line resets the numbering (TUs pin asm with #line): probe line k is reported as line k + 1 of "viewprobe_lines"
    prelude = '\n#ifndef SLY2_OFFSETOF\n#define SLY2_OFFSETOF(T, m) ((int)(char *)&((T *)0)->m)\n#endif\n#line 1 "viewprobe_lines"\n'
    base_line = 1
    alive = list(range(len(lines)))
    fails = []
    try:
        for it in range(12):
            body = text + '\n' + prelude + '\n'.join(lines[i] for i in alive) + '\n'
            open(psrc, 'w').write(body)
            ok, err = common.compile_asm(psrc, pasm, root)
            if ok:
                break
            bad = set()
            for m in re.finditer(r'^[^:\n]*viewprobe_[^:\n]*:(\d+):', err, re.M):
                ln = int(m.group(1)) - base_line
                if 0 <= ln < len(alive):
                    bad.add(alive[ln])
            if not bad:
                fails.append('%s: probe compile failed outside the probe lines: %s' % (rel, err.strip()[-400:]))
                return fails
            for i in bad:
                fails.append('%s: dropped probe %s' % (rel, lines[i][:160]))
            alive = [i for i in alive if i not in bad]
        else:
            fails.append('%s: probe did not converge' % rel)
            return fails
        vals = _read_words(open(pasm).read())
    finally:
        for p in (psrc, pasm):
            if os.path.exists(p):
                os.remove(p)
    for k, r in enumerate(recs):
        r['size'] = vals.get('__vp_%d_s' % k, [None])[0]
        for j, m in enumerate(_flat_members(r['members'])):
            v = vals.get('__vp_%d_%d' % (k, j))
            if v and len(v) == 2:
                m['off'], m['size'] = v
    return fails


def _flat_members(ms):
    return ms


def _read_words(asm):
    vals, cur = {}, None
    for l in asm.splitlines():
        m = re.match(r'^(__vp_\w+):', l)
        if m:
            cur = m.group(1); vals[cur] = []; continue
        m = re.match(r'^\s*\.word\s+(-?\w+)', l)
        if m and cur:
            vals[cur].append(int(m.group(1), 0)); continue
        if cur and re.match(r'^\s*\.(space|byte|half)\b', l):
            vals[cur].append(0)
        elif cur and l.strip() and not l.strip().startswith(('.align', '.size', '.type', '.globl')):
            cur = None if not l.startswith('\t') else cur
    return vals


# ---------------------------------------------------------------- header classes
class Headers:
    def __init__(self, offsets):
        self.T = offsets
        self.tsz = {'char': 1, 'uchar': 1, 'short': 2, 'ushort': 2, 'int': 4, 'uint': 4, 'float': 4, 'long': 8,
                    'ulong': 8, 'qword': 16, 'QWROW': 16, 'OID': 4, 'CID': 4, 'MSGID': 4, 'undefined4': 4,
                    'undefined1': 1, 'undefined2': 2, 'GRFEOPID': 4, 'TICK': 8, 'double': 8, 'bool': 1}
        for k, v in offsets.items():
            self.tsz.setdefault(k, v['size'])
        self._flat = {}
        self.children = collections.defaultdict(set)
        for k, v in offsets.items():
            if v.get('base'):
                self.children[v['base']].add(k)

    def chain(self, t):
        out = []
        while t in self.T and t not in out:
            out.append(t); t = self.T[t].get('base')
        return out

    def descendants(self, t):
        out, todo = set(), [t]
        while todo:
            x = todo.pop()
            if x in out:
                continue
            out.add(x); todo += list(self.children.get(x, ()))
        return out

    def flat(self, t):
        """[(off, size, path, typeword, isptr, is_pad, elem)] of t, bases included (base data at offset 0)."""
        if t in self._flat:
            return self._flat[t]
        out = []
        self._flat[t] = out
        b = self.T[t].get('base')
        if b in self.T:
            out += self.flat(b)
        for o, z, n, tw, ptr in self.T[t]['fields']:
            self._add(out, o, z, n, tw, ptr, 0)
        out.sort(key=lambda e: (e[0], -e[1]))
        return out

    def _add(self, out, o, z, n, tw, ptr, depth):
        leaf = n.split('.')[-1].split('[')[0]
        pad = bool(re.match(r'^(_?pad|filler|gap|reserved|_)', leaf, re.I)) or \
            (bool(re.match(r'^(unk|unknown)', leaf, re.I)) and tw in ('char', 'uchar', 'undefined1', 'byte', 'u8'))
        es = 4 if ptr else self.tsz.get(tw)
        elem = es if es and z > es and z % es == 0 else None
        out.append((o, z, n, tw, ptr, pad, elem))
        if depth > 3:
            return
        if elem and z // elem <= 64:
            for i in range(z // elem):
                self._add(out, o + i * elem, elem, '%s[%d]' % (n, i), tw, ptr, depth + 1)
        elif not ptr and tw in self.T and self.T[tw]['size'] <= z and not elem:
            for so, sz, sn, stw, sp, spad, sel in self.flat(tw):
                if '[' in sn and sn.count('[') > 1:
                    continue
                out.append((o + so, sz, n + '.' + sn, stw, sp, spad, sel))

    def at(self, t, off):
        return [e for e in self.flat(t) if e[0] == off]

    def containing(self, t, off):
        return [e for e in self.flat(t) if e[0] < off < e[0] + e[1]]


def _tnorm(m):
    t = m['type']
    if m.get('ptr'):
        return '*'
    return 'Q' if t in QWORDISH else t


def type_score(vm, e, shapes=None, H=None):
    """How well header entry e fits view member vm (higher is better). shapes: local record name -> member layout
    [(off, size)], compared with the header type's fields (a local `M3 { QWROW row[3]; }` is MATRIX3Q's shape)."""
    o, z, path, tw, ptr, pad, elem = e
    vt = vm['type']; s = 0
    if shapes and H and vt in shapes and tw in H.T and not ptr and not vm['ptr']:
        hs = sorted(set((f[0], f[1], _tnorm(dict(type=f[3], ptr=f[4]))) for f in H.T[tw]['fields']))
        if shapes[vt] and shapes[vt] == hs:
            s += 5
    if vm['ptr'] and ptr:
        s += 3
    elif bool(vm['ptr']) != bool(ptr):
        s -= 2
    if vt == tw:
        s += 4
    elif vt in ('U', 'QW', 'Q') and tw == 'QWROW':
        s += 3.5    # a local {qword q; float f[4]; ...} union: QWROW is the same shape
    elif vt in QWORDISH and tw in QWORDISH:
        s += 2
    elif vt in ('int', 'uint', 'unsigned', 'unsigned int') and tw in ('int', 'uint', 'OID', 'CID', 'MSGID', 'undefined4'):
        s += 1
    if pad or re.search(r'(^|\.)unknown|unk_|pad', path):
        s -= 1
    s -= path.count('.') * 0.5 + path.count('[') * 0.25
    return s


def map_member(H, t, vm, shapes=None):
    """(status, header path or detail, header entry or None)."""
    o, z = vm.get('off'), vm.get('size')
    if o is None:
        return 'no layout', '', None
    if t not in H.T:
        return 'no class', '', None
    if o == 0xC and 'BASIC' in H.chain(t) and vm['ptr'] and re.match(r'(p?vt|vptr|vtbl|_vptr)', vm['name']):
        return 'vptr', 'vptr', None
    if o >= H.T[t]['size']:
        return 'beyond', '+0x%X >= sizeof %s 0x%X' % (o, t, H.T[t]['size']), None
    exact = [e for e in H.at(t, o) if e[1] == z and not e[5]]
    if vm['arr'] and exact:
        # array onto array: element sizes must agree
        ve = z // max(1, _count(vm['arr']))
        ok = [e for e in exact if (e[6] or e[1]) == ve or (e[6] is None and e[1] == z and z == ve)]
        if not ok:
            return 'stride mismatch', '%s (elem %s vs view elem %d)' % (exact[0][2], exact[0][6], ve), exact[0]
        exact = ok
    if exact:
        best = max(exact, key=lambda e: type_score(vm, e, shapes, H))
        if bool(vm['ptr']) != bool(best[4]) and vm['type'] not in ('int', 'uint', 'unsigned', 'unsigned int', 'sptr', 'uptr'):
            return 'type mismatch', '%s (%s%s vs view %s%s)' % (best[2], best[3], ' *' * bool(best[4]), vm['type'],
                                                                ' *' * vm['ptr']), best
        return 'exact', best[2], best
    pads = [e for e in H.at(t, o) + H.containing(t, o) if e[5]]
    if pads:
        return 'in pad', pads[0][2], None
    cont = H.containing(t, o)
    starts = H.at(t, o)
    if starts:
        return 'size mismatch', '%s (0x%X vs view 0x%X)' % (starts[0][2], starts[0][1], z), starts[0]
    if cont:
        top = min(cont, key=lambda e: e[2].count('.') + e[2].count('['))
        return 'straddles', top[2], None
    return 'in pad', '(no member)', None


def _count(arr):
    n = 1
    for a in re.findall(r'\[([^\]]*)\]', arr):
        try:
            n *= int(eval(a, {}))
        except Exception:
            return 1
    return n


# ---------------------------------------------------------------- per file
def region_of(recs, r, text_len):
    """Text range in which r is visible: its innermost namespace block (or the file)."""
    return r.get('region', (r['end'], text_len))


def namespace_regions(stripped):
    """[(name, open, close)] for every namespace block."""
    out = []
    for m in re.finditer(r'\bnamespace\s*(\w*)\s*\{', stripped):
        o = m.end() - 1
        out.append((m.group(1), o, common.match_brace(stripped, o)))
    return out


def analyse_file(rel, root, H, targets, loadfns, framefiles):
    text = open(os.path.join(root, rel)).read()
    stripped = common.strip_comments(text)
    recs, local = parse.scan(text)
    fails = probe_layouts(rel, recs, root) if recs else []
    nsr = namespace_regions(stripped)
    # opaque forward declarations (`struct SPACE;` never defined in the file): pointer-only views, typed by propagation
    defined = {r['name'] for r in recs}
    line_of = parse._line_index(text)
    for m in re.finditer(r'(?<![\w,(])\s*\b(struct|class)\s+(\w+)\s*;', stripped):
        nm = m.group(2)
        if nm in defined or nm in H.T:
            continue
        st = m.start(1)
        inner = [x for x in nsr if x[1] < st < x[2]]
        # skip declarations inside function bodies / records: the enclosing brace must be a namespace (or none)
        depth_ok = _at_namespace_scope(stripped, st, nsr)
        if not depth_ok:
            continue
        recs.append(dict(kind='opaque', name=nm, typedef='', ns=[x[0] for x in sorted(inner, key=lambda x: x[1])],
                         base='', start=st, open=st, close=m.end(), end=m.end(), line=line_of(st),
                         endline=line_of(m.end()), vars=[], members=[], flags=set(), dup=False, size=None))
        defined.add(nm)
    names_here = collections.Counter(r['name'] for r in recs)
    out = []
    for r in recs:
        # visibility region: innermost namespace containing the definition
        inner = [x for x in nsr if x[1] < r['start'] < x[2]]
        a, b = (min(inner, key=lambda x: x[2] - x[1])[1:] if inner else (0, len(text)))
        r['region'] = (r['end'], b)
        r['ns_range'] = (a, b)
        seg = stripped[r['end']:b]
        for m in r['members']:
            m['uses'] = len(re.findall(r'(?:->|\.)\s*%s\b' % re.escape(m['name']), seg))
            m['pad'] = parse.is_pad(m)
        r['functions'] = sorted(set(parse.functions_in(stripped, a, b)) - {r['name']})
        nsname = r['ns'][-1] if r['ns'] else ''
        if nsname.startswith('M_') and nsname[2:] not in r['functions']:
            r['functions'].insert(0, nsname[2:])
        # comment hint: comments on the line before the definition, its first line, or right after its end
        ls = text.rfind('\n', 0, max(0, text.rfind('\n', 0, r['start'])))
        le = text.find('\n', r['open']); le = le if le > 0 else len(text)
        lc = text.find('\n', r['end']); lc = lc if lc > 0 else len(text)
        r['comment_classes'] = (_comment_classes(text[ls + 1:le], H) or _comment_classes(text[r['end']:lc], H))
        r['file'] = rel
        r['path'] = ('frame' if os.path.basename(rel).rsplit('.', 1)[0] in framefiles else '') or \
                    ('load' if any(f in loadfns for f in r['functions']) else '')
        r['local_structs_in_functions'] = local
        out.append(r)
    classify(out, H, targets, rel)
    return out, fails


def _at_namespace_scope(t, i, nsr):
    """True if position i is directly inside a namespace block (or at file scope), not in a function or record."""
    opens = [x[1] for x in nsr if x[1] < i < x[2]]
    start = max(opens) + 1 if opens else 0
    d = 0
    for ch in t[start:i]:
        if ch == '{':
            d += 1
        elif ch == '}':
            d -= 1
    return d == 0


def _comment_classes(seg, H):
    com = ' '.join(a + b for a, b in re.findall(r'/\*(.*?)\*/|//([^\n]*)', seg, re.S))
    return sorted(set(w for w in re.findall(r'\b[A-Z][A-Z0-9_]+\b', com) if w in H.T))


def is_candidate(r, H, targets, rel):
    if not r['name'] or r['dup']:
        return False
    if _hand(targets, rel, r):
        return True
    named = [m for m in r['members'] if not m['pad']]
    pads = [m for m in r['members'] if m['pad']]
    base = re.sub(r'^(public|private|protected)\s+', '', r['base'].split(',')[0].strip()) if r['base'] else ''
    if base in H.T:
        return True
    if r['name'] in H.T or _name_hint(r['name'], H):
        return True
    if pads and named:
        return True
    if any(m.get('off') == 0xC and m['ptr'] and re.match(r'(p?vt|vptr|vtbl)', m['name']) for m in r['members']):
        return True
    return False


def _name_hint(n, H):
    m = NAME_SUFFIX.match(n)
    return m.group(1) if m and m.group(1) in H.T else None


def _hand(targets, rel, r):
    b = os.path.basename(rel)
    return targets.get('%s:%s' % (b, parse.qual(r))) or targets.get('%s:%s' % (b, r['name']))


def fit(H, t, r, used_only=True):
    ms = [m for m in r['members'] if not m['pad'] and m.get('off') is not None and (m['uses'] or not used_only)]
    if not ms:
        return 0, 0
    hits = 0
    for m in ms:
        st, _, _ = map_member(H, t, m)
        hits += st in ('exact', 'vptr')
    return hits, len(ms)


def classify(recs, H, targets, rel):
    shapes = {r['name']: sorted(set((m['off'], m['size'], _tnorm(m)) for m in r['members'] if m.get('off') is not None))
              for r in recs if r.get('name') and r['kind'] != 'opaque'}
    for r in recs:
        r['candidate'] = is_candidate(r, H, targets, rel)
        r['target'], r['confidence'], r['evidence'] = '', '', ''
    bynm = {}
    for r in recs:
        if r['name']:
            bynm[(tuple(r['ns_range']), r['name'])] = r

    def lookup(r, name):
        # the view named `name` visible from r: same namespace block first, then file scope
        return bynm.get((tuple(r['ns_range']), name)) or next(
            (x for (rng, n), x in bynm.items() if n == name and rng[0] <= r['start'] < rng[1] and x['start'] < r['start']
             and (rng[1] - rng[0]) > (r['ns_range'][1] - r['ns_range'][0])), None)
    # pass 1: direct hints
    for r in recs:
        if not r['candidate']:
            continue
        hand = _hand(targets, rel, r)
        base = re.sub(r'^(public|private|protected)\s+', '', r['base'].split(',')[0].strip()) if r['base'] else ''
        if hand:
            _set(r, hand, 'high', 'hand table')
        elif r['name'] in H.T:
            _set(r, r['name'], 'high', 'name (re-declares the header class)')
        elif _name_hint(r['name'], H):
            _set(r, _name_hint(r['name'], H), 'high', 'name %s' % r['name'])
        elif r['comment_classes'] and len(r['comment_classes']) == 1:
            _set(r, r['comment_classes'][0], 'high', 'comment')
        elif base in H.T:
            r['base_class'] = base
    # pass 2: propagation through pointer / embedded members, repeated (again after the offset fit)
    def propagate():
        for _ in range(4):
            changed = False
            for r in recs:
                if not r['target'] or r['target'] not in H.T:
                    continue
                for m in r['members']:
                    o = lookup(r, m['type']) if m.get('off') is not None else None
                    if o is None or o is r:
                        continue
                    if o['target']:
                        continue
                    st, path, e = map_member(H, r['target'], m, shapes)
                    if st == 'exact' and e and bool(e[4]) == bool(m['ptr']) and e[3] in H.T:
                        ev = '%s %s::%s -> %s::%s' % ('pointer' if m['ptr'] else 'member', r['name'], m['name'],
                                                      r['target'], path)
                        t = e[3]
                        h, n = fit(H, t, o)
                        if n and h < n:
                            # the member is declared as a base class: narrow to the descendants every used member fits
                            ok = [d for d in H.descendants(t) if fit(H, d, o)[0] == n]
                            roots = [d for d in ok if not any(x != d and x in H.chain(d) for x in ok)]
                            if len(roots) == 1:
                                t, ev = roots[0], ev + ' (a %s; members fit %s)' % (e[3], roots[0])
                        # a plain local record reached through a view's member becomes a view too
                        # (GROUP { CONTACT *first; GROUP *next; }); it inherits the source's confidence
                        o['candidate'] = True
                        _set(o, t, r['confidence'] if r['confidence'] in ('high', 'medium') else 'low', ev)
                        changed = True
            if not changed:
                break

    propagate()
    # pass 3: offset fit
    for r in recs:
        if not r['candidate'] or r['target']:
            continue
        pool = H.descendants(r['base_class']) if r.get('base_class') else set(H.T)
        best = []
        for t in pool:
            if r.get('size') and H.T[t]['size'] < (r.get('size') or 0) - 16:
                continue
            h, n = fit(H, t, r)
            if n and h == n:
                best.append(t)
        if not best:
            if r.get('base_class'):
                _set(r, r['base_class'], 'low', 'base %s (members do not all map)' % r['base_class'])
            continue
        # least derived class holding every used member, if all candidates share one chain
        roots = [t for t in best if not any(x != t and x in H.chain(t) for x in best)]
        n_used = len([m for m in r['members'] if not m['pad'] and m['uses']])
        if len(roots) == 1:
            # a base class narrows the pool to one subtree: with 3+ used members the fit is as good as a name
            conf = 'high' if r.get('base_class') and n_used >= 3 else 'medium' if n_used >= 2 or r.get('base_class') else 'low'
            ev = 'offset fit (%d used members)' % n_used + ('; base %s' % r['base_class'] if r.get('base_class') else '')
            _set(r, roots[0], conf, ev)
        else:
            _set(r, '', 'low', 'ambiguous offset fit: ' + ','.join(sorted(roots)[:6]) + ('...' if len(roots) > 6 else ''))
    propagate()
    # member mapping
    for r in recs:
        if not r['candidate']:
            continue
        for m in r['members']:
            if m['pad']:
                m['map'] = ('pad', '')
                continue
            st, path, e = map_member(H, r['target'], m, shapes) if r['target'] else ('no target', '', None)
            m['map'] = (st, path)
            m['htype'] = (e[3] + ' *' * bool(e[4])) if e else ''
        used = [m for m in r['members'] if not m['pad'] and m['uses']]
        r['n_used'] = len(used)
        r['n_mapped'] = sum(1 for m in used if m['map'][0] in ('exact', 'vptr'))
        r['mappable'] = bool(r['target']) and r['n_used'] == r['n_mapped']
        r['target_header'] = H.T[r['target']]['header'] if r['target'] in H.T else ''
        if r['target'] and r['confidence'] == 'high' and not r['mappable'] and 'hand' not in r['evidence'] \
                and 'pointer' not in r['evidence']:
            r['confidence'] = 'medium'


def _set(r, t, conf, ev):
    r['target'], r['confidence'], r['evidence'] = t, conf, ev


# ---------------------------------------------------------------- driver
def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', default=common.ROOT)
    ap.add_argument('--out')
    ap.add_argument('--offsets')
    ap.add_argument('--load-path', default='')
    ap.add_argument('--frame-files', default='air,step,mark,stepplayer,jt,cm,path')
    ap.add_argument('--targets', default=os.path.join(common.HERE, 'targets.json'))
    ap.add_argument('--jobs', type=int, default=2)
    ap.add_argument('files', nargs='*')
    A = ap.parse_args()
    root = os.path.abspath(A.root); common.set_root(root)
    out = os.path.abspath(A.out or os.path.join(root, 'build', 'views'))
    os.makedirs(out, exist_ok=True)
    offp = A.offsets or os.path.join(out, 'offsets.json')
    if not os.path.exists(offp):
        import subprocess
        subprocess.run([sys.executable, os.path.join(root, 'tools/cleanup/census/dump_offsets.py'), '--root', root,
                        '--out', os.path.dirname(offp)], check=True)
    H = Headers(json.load(open(offp)))
    targets = common.load_json(A.targets, {}) or {}
    targets = {k: v for k, v in targets.items() if not k.startswith('_')}
    loadfns = set()
    if A.load_path and os.path.exists(A.load_path):
        for row in csv.DictReader(open(A.load_path)):
            loadfns.add(row['name'].split('__')[0])
    framefiles = set(x for x in A.frame_files.split(',') if x)
    files = A.files or sorted(
        os.path.relpath(os.path.join(dp, f), root) for dp, _, fs in os.walk(os.path.join(root, 'src/game'))
        for f in fs if f.endswith('.cpp') and not f.startswith('.'))
    allrecs, allfails = [], []
    with ThreadPoolExecutor(A.jobs) as ex:
        for recs, fails in ex.map(lambda f: analyse_file(f, root, H, targets, loadfns, framefiles), files):
            allrecs += recs; allfails += fails
    views = [r for r in allrecs if r['candidate']]
    for r in allrecs:
        r.pop('body', None)
        r['flags'] = sorted(r['flags'])
    json.dump(allrecs, open(os.path.join(out, 'views.json'), 'w'), indent=1, default=str)
    with open(os.path.join(out, 'views.csv'), 'w', newline='') as f:
        w = csv.writer(f)
        w.writerow(['file', 'line', 'view', 'size', 'target', 'confidence', 'evidence', 'used', 'mapped', 'mappable',
                    'members', 'functions', 'path'])
        for r in views:
            mem = '; '.join('%s@%s->%s%s x%d' % (m['name'], '0x%X' % m['off'] if m.get('off') is not None else '?',
                                                 m['map'][1] if m['map'][0] == 'exact' else m['map'][0] +
                                                 ((' ' + m['map'][1]) if m['map'][1] else ''),
                                                 '', m['uses'])
                            for m in r['members'] if not m['pad'])
            w.writerow([r['file'], r['line'], parse.qual(r), '0x%X' % r['size'] if r.get('size') else '',
                        r['target'], r['confidence'], r['evidence'], r['n_used'], r['n_mapped'],
                        int(r['mappable']), mem, ' '.join(r['functions'][:6]), r['path']])
    open(os.path.join(out, 'probe_failures.txt'), 'w').write('\n'.join(allfails) + '\n')
    c = collections.Counter((r['confidence'] or 'none') for r in views)
    print('files %d, structs %d, view candidates %d (frame %d, load %d); targets: %s; mappable %d; probe failures %d'
          % (len(files), len(allrecs), len(views), sum(r['path'] == 'frame' for r in views),
             sum(r['path'] == 'load' for r in views), dict(c), sum(r['mappable'] for r in views), len(allfails)))


if __name__ == '__main__':
    main()
