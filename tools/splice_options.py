#!/usr/bin/env python3
"""Generate the per-class Splice option reference (docs/reference/splice-options.md) from the option tables.

Usage: python3 tools/splice_options.py [--out FILE] [--tsv]
  --out FILE  write the markdown there (default: docs/reference/splice-options.md; '-' for stdout)
  --tsv       instead print one tab-separated row per (class, option id) binding: cid, class, option id, record,
              name, type, access (for scripting and diffing; nothing is written)

Sources (all parsed as text; nothing is compiled):
  src/game/app/class_registry_options.inc  g_aeopid records (D_002AC880), the per-class option arrays, the class
                                           index (D_00279AD0, gives CID -> class name)
  src/game/app/class_registry.cpp          the class descriptor table D_002BAAE8 (parent of each CID)
  include/sly2/*.h                         struct members with '/* +0xNN' comments (names of directly accessed fields)

Per class the output lists only the options the class adds or rebinds relative to its parent class (an option id
whose record differs from the parent's binding for the same id); everything else is inherited. Option names are
derived, in order: the thunk symbol (RefThunk<Class><SCRIPT_NAME> or RefThunk_<Native>); the getter / setter
function names; the field at the record's byte offset in the
class's struct (or a base struct); else the raw offset '+0xNN'. Names are labels for finding the code, not script
spellings: scripts address options by number only.
Re-run after renaming thunks, accessors or fields, or after editing the option tables.
"""
import os, re, sys

REPO = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
INC = os.path.join(REPO, 'src/game/app/class_registry_options.inc')
REG = os.path.join(REPO, 'src/game/app/class_registry.cpp')
HDR = os.path.join(REPO, 'include/sly2')
SIDEBAG_MIN = 0x4D1   # GetBasicOption / SetBasicOption: ids above 0x4D0 go to the object's sidebag

WORD = r'EOPID_(?:NONE|INT\([^)]*\)|OFF\([^)]*\)|PMFT?I?\([^)]*\)|VPMF\([^)]*\)|FN\([^)]*\))'
REC = re.compile(r'^\s*\{(0x[0-9A-Fa-f]+|\d+), (0x[0-9A-Fa-f]+|\d+), (' + WORD + r'), (' + WORD + r'), (' + WORD +
                 r'), (' + WORD + r')\}, /\* \[(\d+)\]')
ARR = re.compile(r'^extern const SPLOPTROW (D_[0-9A-F]+)\[\d+\] SPLOPT_RODATA = \{\s*/\* (\w+) \(cid (\d+)\) \*/')
PAIR = re.compile(r'\{(\d+), &D_002AC880\[(\d+)\]\}')


def _lines(path):
    with open(path) as f:
        return f.read().split('\n')


def word(w):
    """('none'|'int'|'off'|'pmf'|'vpmf'|'fn', value); 'off' is EOPID_OFF(T, m), value = member name m"""
    m = re.match(r'EOPID_(\w+?)\((.*)\)$', w)
    if not m:
        return ('none', None)
    k, v = m.group(1), m.group(2)
    if k == 'INT':
        return ('int', int(v, 0))
    if k == 'OFF':
        return ('off', v.split(',')[1].strip())
    if k in ('PMF', 'PMFI', 'PMFT'):
        return ('pmf', v.split(',')[0].strip())
    if k == 'VPMF':
        return ('vpmf', v)
    return ('fn', v)


def parse_tables():
    recs, arrays, cidname = {}, {}, {}
    cur = None
    for line in _lines(INC):
        m = REC.match(line)
        if m:
            recs[int(m.group(7))] = dict(otyp=int(m.group(1), 0), grf=int(m.group(2), 0), get=word(m.group(3)),
                                          set=word(m.group(4)), file=word(m.group(5)), obj=word(m.group(6)))
            continue
        m = ARR.match(line)
        if m:
            cur = int(m.group(3))
            cidname[cur] = m.group(2)
            arrays[cur] = []
            continue
        if cur is not None:
            if line.startswith('};'):
                cur = None
                continue
            arrays[cur] += [(int(a), int(b)) for a, b in PAIR.findall(line)]
    return recs, arrays, cidname


def parse_parents():
    par = {}
    for line in _lines(REG):
        m = re.match(r'\s*\{(0|&D_002BAAE8\[(\d+)\]), (\d+), (\d+), ', line)
        if m:
            par[int(m.group(3))] = int(m.group(2)) if m.group(2) else None
    return par


def parse_structs():
    """struct name -> (base or None, {offset: member name}) for members commented '/* +0xNN'."""
    out = {}
    for fn in sorted(os.listdir(HDR)):
        if not fn.endswith('.h'):
            continue
        with open(os.path.join(HDR, fn), errors='replace') as f:
            lines = f.read().split('\n')
        i = 0
        while i < len(lines):
            m = re.match(r'\s*struct (\w+)\s*(?::\s*public\s+(\w+))?\s*(\{|/\*.*)?$', lines[i])
            if not m or (m.group(3) is None and (i + 1 >= len(lines) or not lines[i + 1].strip().startswith('{'))):
                i += 1
                continue
            name, base, depth, fields = m.group(1), m.group(2), 0, {}
            j = i
            while j < len(lines):
                code = lines[j].split('/*')[0].split('//')[0]
                if depth == 1:
                    f = re.match(r'\s*[\w\s\*:<>,]+?\**\s*\b(\w+)\s*(?:\[[^\]]*\])*\s*;\s*/\*\s*\+0x([0-9A-Fa-f]+)', lines[j])
                    if f:
                        fields.setdefault(int(f.group(2), 16), f.group(1))
                depth += code.count('{') - code.count('}')
                if depth <= 0 and '{' in ''.join(lines[i:j + 1]) and j > i - 1 and '}' in code:
                    break
                j += 1
            if name not in out or len(fields) > len(out[name][1]):
                out[name] = (base, fields)
            i = j + 1
    return out


def field_name(structs, cls, off):
    seen = set()
    while cls and cls in structs and cls not in seen:
        seen.add(cls)
        base, fields = structs[cls]
        if off in fields:
            return fields[off]
        cls = base
    return None


INTS = {5, 6, 7, 8, 13, 14, 15, 16, 17, 19, 20, 23}
BASE = {0: 'bool', 1: 'float', 2: 'vector', 4: 'matrix', 9: 'clq', 10: 'lm', 11: 'smp'}


def type_name(o, cidname, method):
    if method:
        return '-'
    if o & 0x8000:
        return 'any'
    if o & 0x2000:
        c = o & 0xFFF
        return 'object' + ('<%s>' % cidname[c] if c in cidname and c else '')
    if o & 0x1000:
        return 'int'
    if o in BASE:
        return BASE[o]
    if o in INTS:
        return 'int'
    return 'otyp 0x%X' % o


def thunk_label(fn):
    """Thunks are listed by their full symbol (greppable; RefThunk<Class><SCRIPT_NAME> carries the script-side name,
    a trailing Q standing for '?'; RefThunk_<Native> names the native function it calls)."""
    return fn


def describe(r, cls, structs, cidname):
    """(name, type, access) of one record. grfeopid bits (sly2/eopid.h, spliceobj.cpp, brx.cpp): 1 script method
    (thunk in the set word, argument count in the file word), 2 / 4 get by offset / getter, 0x10 / 0x20 set by
    offset / setter, 0x80 / 0x100 / 0x400 file-time store by offset / setter / thunk (the thunk in the get word),
    0x2000 the field lives in a sub-object reached through the record's accessor."""
    grf = r['grf']
    method = bool(grf & 1)
    nargs = r['file'][1] if r['file'][0] == 'int' else 0
    names, acc = [], []

    def add(n):
        if n and n not in names:
            names.append(n)

    def offname(off):
        if isinstance(off, str):      # EOPID_OFF(T, m): the member is named in the table itself
            return ('sub-object ' + off) if grf & 0x2000 else off
        if grf & 0x2000:
            return 'sub-object +0x%X' % off
        return field_name(structs, cls, off) or '+0x%X' % off

    def fnname(w):
        return thunk_label(w[1]) if w[0] == 'fn' else (w[1] if w[0] == 'pmf' else 'virtual slot ' + w[1])

    if method:
        if r['set'][0] == 'fn':
            add(thunk_label(r['set'][1]))
        acc.append('method(%d)' % nargs)
    else:
        for key, direct, pmf in (('get', 2, 4), ('set', 0x10, 0x20)):
            w = r[key]
            if grf & direct and w[0] in ('int', 'off'):
                acc.append(key)
                add(offname(w[1]))
            elif grf & pmf and w[0] in ('pmf', 'vpmf', 'fn'):
                acc.append(key)
                add(fnname(w))
    if grf & 0x400 and r['get'][0] == 'fn':
        acc.append('file-method(%d)' % nargs if not method else 'file')
        add(thunk_label(r['get'][1]))
    elif grf & 0x100 and r['file'][0] in ('pmf', 'vpmf'):
        acc.append('file')
        add(fnname(r['file']))
    elif grf & 0x80 and r['file'][0] in ('int', 'off'):
        acc.append('file')
        add(offname(r['file'][1]))
    is_method = method or r['otyp'] == 0x12
    return ' / '.join(names) or '?', type_name(r['otyp'], cidname, is_method), ' '.join(acc) or 'none'


def main():
    args = sys.argv[1:]
    if any(x in ('-h', '--help') for x in args):
        print(__doc__)
        return
    recs, arrays, cidname = parse_tables()
    par = parse_parents()
    structs = parse_structs()
    root = {}   # record -> the first CID (in CID order) whose array lists it, for the struct lookup
    for cid in sorted(arrays):
        for _, ri in arrays[cid]:
            root.setdefault(ri, cid)
    rows = []
    for cid in sorted(arrays):
        pmap = dict(arrays.get(par.get(cid), [])) if par.get(cid) is not None else {}
        for opid, ri in arrays[cid]:
            own = pmap.get(opid) != ri
            name, typ, acc = describe(recs[ri], cidname[root[ri]], structs, cidname)
            rows.append((cid, opid, ri, name, typ, acc, own))
    if '--tsv' in args:
        for cid, opid, ri, name, typ, acc, own in rows:
            print('\t'.join(map(str, (cid, cidname[cid], opid, ri, name, typ, acc))))
        return
    out = args[args.index('--out') + 1] if '--out' in args else os.path.join(REPO, 'docs/reference/splice-options.md')
    L = ['# Splice object options by class', '',
         '<!-- Generated by tools/splice_options.py from src/game/app/class_registry_options.inc; do not edit by hand. -->',
         '',
         'Generated reference for [splice-language.md](splice-language.md) section 4. For each class (CID order), the '
         'option ids it adds or rebinds relative to its parent class; all other ids are inherited unchanged from the '
         'parent named in the heading. Columns:', '',
         '- **id**: the option id scripts pass to `geto` / `seto!` (ids above 0x4D0 never reach this table: they are '
         'sidebag variables).',
         '- **name**: a label derived from the code (the thunk symbol, whose `RefThunk<Class><NAME>` form carries the '
         'script-side name with Q for ?, or the accessor or field), not a script spelling. `+0xNN` is an unnamed byte offset; "sub-object" means the field lives in an object reached '
         'through the record\'s accessor.',
         '- **type**: the value type `geto` returns and `seto!` expects (`object<CLS>` is an object reference whose '
         'record names that class; `any` is passed through unconverted; `-` for methods).',
         '- **access**: `get` (`geto` reads it), `set` (`seto!` writes it), `method(n)` (`geto` returns a method '
         'taking n arguments), `file` (the level loader can set it from an object\'s option list), `file-method(n)` (a '
         'method only the level loader calls; scripts cannot reach it), `none` (no access word: the id is reserved).',
         '- **rec**: the index of the record in `g_aeopid` (`D_002AC880` in `class_registry_options.inc`).', '',
         'Using an access a record does not have (for example `seto!` on a get-only option) calls a null function '
         'in the original game.', '']
    total = sum(1 for r in rows if r[6])
    L.append('%d classes, %d records, %d class bindings (%d new or rebound).' % (len(arrays), len(recs), len(rows), total))
    L.append('')
    for cid in sorted(arrays):
        own = [r for r in rows if r[0] == cid and r[6]]
        p = par.get(cid)
        head = '## %s (cid %d)' % (cidname[cid], cid)
        sub = ('parent %s; ' % cidname[p] if p is not None else 'root class; ') + '%d options in all' % len(arrays[cid])
        L.append(head)
        L.append('')
        if not own:
            L.append(sub + '; nothing new.')
            L.append('')
            continue
        L.append(sub + '.')
        L.append('')
        L.append('| id | name | type | access | rec |')
        L.append('|---|---|---|---|---|')
        for _, opid, ri, name, typ, acc, _ in own:
            L.append('| %d | `%s` | %s | %s | %d |' % (opid, name.replace('|', '\\|').replace(' / ', '` / `'), typ.replace('<', '&lt;').replace('>', '&gt;'), acc, ri))
        L.append('')
    text = '\n'.join(L)
    if out == '-':
        sys.stdout.write(text)
    else:
        os.makedirs(os.path.dirname(out), exist_ok=True)
        open(out, 'w').write(text)
        print('wrote %s (%d classes, %d own bindings)' % (os.path.relpath(out, REPO), len(arrays), total))


if __name__ == '__main__':
    main()
