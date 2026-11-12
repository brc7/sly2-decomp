#!/usr/bin/env python3
"""Struct-return convention check: do all declarations of one linker symbol agree on how a struct result travels?

g++ 2.95 on the EE passes the address of a struct (or class) result as a hidden first argument, so
    MATRIX3 BuildX(float a, qword b)          and          void BuildX(MATRIX3 *out, float a, qword b)
call the same code, and the decomp uses both spellings for one symbol in different files. A native build does not:
x86-64 SysV passes a MEMORY-class result's address in rdi (the first argument: the mismatch happens to work there),
returns small trivially copyable structs in registers (it does not), and arm64 (AAPCS64) passes the result address
in x8 with the arguments still in x0.. (it never works). This tool lists every symbol whose native-visible
declarations, definitions and function-pointer table entries mix the two forms.

  sretcheck.py [--root REPO] [--out DIR]

Declarations come from sigcheck.py's parser (src/ and include/). A declaration belongs to a linker symbol through its
asm label, its C name (extern "C"), or because it is the canonical C++ declaration of a mangled config symbol. Lines
that only the EE compiles (`#if defined(SLY2_EE_LAYOUT)` / `SLY2_EE_ASM` / `__R5900__` branches) are skipped, and so
are the forms bound natively elsewhere: SLY2_SRET_LABEL(ee, native) declarations (their native label is checked
instead) and SLY2_BUILTIN_C bodies (natively `sym_c`, with a CRef-returning `sym` entry).

Forms: `value` (a class/struct or qword result: the hidden-pointer convention on the EE, x8 / registers natively),
`out` (one more leading pointer parameter, result void or a pointer), `scalar` (anything else). A symbol is listed
when it has both a `value` declaration with N parameters and an `out` declaration with N + 1.

Function-pointer tables, checked against the pointer type the caller uses:
  - bif.cpp g_mpbifkbif: every entry is called as `CRef (*)(int, CRef *, CFrame *)` (splice/eval.cpp BIF);
  - class_registry_options.inc g_aeopid: otyp & 0x8000 getters are called as `CRef (BASIC::*)()` (pmfGetRef),
    otyp 9 / 10 / 11 getters as `void (BASIC::*)(CLQ / LM / SMP *)` (an explicit out-pointer);
  - the world callback API (PostSwCallback / EnsureSwCallback / ClearSwCallbacks): declarations that pass the
    `void (CObj::*)(int)` PMF as an integer or the argument word as a non-pointer-sized integer (natively a PMF is
    two pointer-sized words; a pointer argument truncates).

Outputs (DIR, default build/signatures/): sret.csv (one row per declaration of a listed symbol or table entry) and
sret.md (the symbol list with the forms and sites). Exit 0 (a report, not a gate).
"""
import argparse
import collections
import csv
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sigcheck as S   # noqa: E402

SCALARS = S.INTS | {'float', 'double', 'longdouble', 'void'}


def form(dl, enums):
    """('value' | 'out' | 'scalar' | 'none', result type) of a declaration"""
    r = S.drop_top_const(dl.ret or '')
    if not dl.params and not dl.variadic:
        if r == 'void': return 'none', r          # `void f()`: an address-only declaration (tables)
    if r in ('longlong', 'ulonglong'):
        return 'value', r
    if r and not S.is_ptr(r) and S.strip_const(r) not in SCALARS and r not in enums and not r.startswith(
            ('fn/', 'mfn/', '...')):
        return 'value', r
    if dl.params and S.is_ptr(dl.params[0]) and (r in ('void', '') or S.is_ptr(r)):
        return 'out', r
    return 'scalar', r


def flat(dl):
    return dl.flat()


def site(dl):
    return f'{dl.file}:{dl.line}'


def native_groups(L):
    """linker symbol -> the native-visible declarations that reach it (asm label, C name, canonical mangled name;
    EE-only branches and file-local statics skipped, SLY2_SRET_LABEL / SLY2_BUILTIN_C by their native name)"""
    syms, decls = L['syms'], L['decls']
    groups = collections.defaultdict(list)
    canon_key = collections.defaultdict(list)
    for s_, d in syms.items():
        canon_key[(d['name'], d['cls'])].append(s_)
    cnames = {(dl.file, dl.cname) for dl in decls if dl.cdecl and dl.cname and not dl.member}
    for dl in decls:
        if dl.pp == 'ee' or (dl.static and not dl.member and not dl.label):
            continue                   # EE-only, or a file-local helper (static)
        if dl.adapted and dl.native_label:
            groups[dl.native_label].append(dl); continue
        if dl.adapted:                 # SLY2_BUILTIN_C body: natively sym_c (its CRef entry is checked as a table)
            groups[(dl.label or dl.cname) + '_c'].append(dl); continue
        if dl.label:
            groups[dl.label].append(dl); continue
        if dl.cname and not dl.member and (dl.cdecl or (dl.file, dl.cname) in cnames):
            # C linkage: extern "C", or a later definition of a name an extern "C" declaration of the file gave
            groups[dl.cname].append(dl); continue
        for s_ in canon_key.get((dl.name, dl.member or dl.qual), ()):
            if syms[s_]['params'] == dl.params and not dl.templ:
                groups[s_].append(dl)

    return groups


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0], epilog=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--root', default=os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))))))
    ap.add_argument('--out')
    a = ap.parse_args()
    root = os.path.abspath(a.root)
    out = os.path.abspath(a.out or os.path.join(root, 'build', 'signatures'))
    os.makedirs(out, exist_ok=True)
    L = S.load(root)
    syms, decls, enums = L['syms'], L['decls'], L['enums']

    groups = native_groups(L)
    rows, listed = [], []
    # SLY2_SRET_LABEL's native label must name a definition (the adapter)
    defined = {dl.cname for dl in decls if dl.is_def and dl.cname}
    for dl in decls:
        if dl.adapted and dl.native_label and dl.native_label not in defined and dl.pp != 'ee':
            listed.append(dict(symbol=dl.native_label, kind='adapter missing', definition='not found', value=0, out=0,
                               value_sites='', out_sites=site(dl), value_decl='', out_decl=dl.sig()))
    for sym, ds in sorted(groups.items()):
        fs = [(form(dl, enums), dl) for dl in ds]
        vals = [(f, dl) for f, dl in fs if f[0] == 'value']
        outs = [(f, dl) for f, dl in fs if f[0] == 'out']
        if not vals or not outs: continue
        def pairs(o, v):               # o has one more leading parameter than v (v variadic: at least that many)
            n, m = len(flat(o)), len(flat(v))
            return n == m + 1 or (v.variadic and n >= m)
        bad_out = [(f, dl) for f, dl in outs if any(pairs(dl, v) for _, v in vals)]
        if not bad_out: continue
        defs = [dl for _, dl in fs if dl.is_def]
        dform = form(defs[0], enums)[0] if defs else '?'
        listed.append(dict(symbol=sym, kind='declarations', definition=dform,
                           value=len(vals), out=len(bad_out),
                           value_sites=', '.join(site(dl) for _, dl in vals),
                           out_sites=', '.join(site(dl) for _, dl in bad_out),
                           value_decl=vals[0][1].sig(), out_decl=bad_out[0][1].sig()))
        for f, dl in vals + bad_out:
            rows.append(dict(symbol=sym, kind='declarations', form=f[0], is_def=int(dl.is_def), file=dl.file,
                             line=dl.line, decl=dl.sig()))

    # ---- tables
    def native_defs(sym):
        return [dl for dl in groups.get(sym, []) if dl.is_def]

    def table_entry(table, sym, want, expect):
        ds = native_defs(sym)
        builtin_c = any(dl.adapted for dl in groups.get(sym + '_c', []) if dl.is_def)
        if builtin_c:
            return
        if not ds:
            # no definition parsed (C-named stubs, SDK, asm): judge the declarations that are not address-only
            ds = [dl for dl in groups.get(sym, []) if form(dl, enums)[0] != 'none']
            if not ds:
                listed.append(dict(symbol=sym, kind=table, definition='not found', value=0, out=0, value_sites='',
                                   out_sites='', value_decl='', out_decl=f'called as {expect}'))
                return
        f = form(ds[0], enums)[0]
        if f == want: return
        listed.append(dict(symbol=sym, kind=table, definition=f, value=int(f == 'value'), out=int(f == 'out'),
                           value_sites=site(ds[0]) if f == 'value' else '', out_sites=site(ds[0]) if f != 'value' else '',
                           value_decl=f'called as {expect}', out_decl=ds[0].sig()))
        rows.append(dict(symbol=sym, kind=table, form=f, is_def=int(ds[0].is_def), file=ds[0].file, line=ds[0].line,
                         decl=ds[0].sig()))

    def labels_in(path, ns):
        """name -> asm label of the declarations inside namespace ns of path"""
        txt, _ = S.strip_comments_pp(open(os.path.join(root, path), errors='replace').read())
        m = re.search(r'namespace\s+' + ns + r'\s*\{', txt)
        if not m: return {}
        out = dict(re.findall(r'\b(\w+)\s*\(\s*\)\s*__asm__\s*\(\s*"([^"]+)"\s*\)', txt[m.end():]))
        out.update(re.findall(r'\b(\w+)\s*\(\s*\)\s*SLY2_SRET_LABEL\s*\(\s*"[^"]+"\s*,\s*"([^"]+)"\s*\)', txt[m.end():]))
        return out                     # SLY2_SRET_LABEL: natively the entry is the adapter

    bif = 'src/game/app/bif.cpp'
    if os.path.exists(os.path.join(root, bif)):
        lab = labels_in(bif, 'M_bif_data')
        txt, _ = S.strip_comments_pp(open(os.path.join(root, bif)).read())
        m = re.search(r'g_mpbifkbif\s*\[\s*\d+\s*\][^=]*=\s*\{(.*?)\};', txt, re.S)
        for name in re.findall(r'\{\s*(\w+)\s*,', m.group(1)) if m else []:
            table_entry('g_mpbifkbif', lab.get(name, name), 'value', 'CRef (*)(int, CRef *, CFrame *)')
    opts = 'src/game/app/class_registry_options.inc'
    if os.path.exists(os.path.join(root, opts)):
        lab = labels_in(opts, 'EOPIDFN')
        txt = open(os.path.join(root, opts)).read()
        for otyp, fn in re.findall(r'\{\s*(0x[0-9A-Fa-f]+|\d+)\s*,\s*(?:0x[0-9A-Fa-f]+|\d+)\s*,\s*EOPID_PMF\((\w+)\)', txt):
            o = int(otyp, 0)
            if o > 23 and o & 0x8000:
                table_entry('g_aeopid getter (otyp & 0x8000)', lab.get(fn, fn), 'value', 'CRef (BASIC::*)()')
            elif o in (9, 10, 11):
                table_entry(f'g_aeopid getter (otyp {o})', lab.get(fn, fn), 'out',
                            'void (BASIC::*)(%s *)' % {9: 'CLQ', 10: 'LM', 11: 'SMP'}[o])
    # the world callback API
    for sym in ('PostSwCallback', 'EnsureSwCallback', 'ClearSwCallbacks'):
        for dl in groups.get(sym, []):
            p = dl.flat()
            if len(p) < 4: continue
            pmf, arg = p[-2], p[-1]
            why = []
            if not pmf.startswith('mfn/') and pmf not in ('PMFCB', 'PMF') and not S.is_ptr(pmf):
                why.append(f'PMF as {pmf}')
            if arg in S.INTS and arg not in ('long', 'ulong'):
                why.append(f'argument as {arg}')
            if why:
                rows.append(dict(symbol=sym, kind='callback API', form='; '.join(why), is_def=int(dl.is_def),
                                 file=dl.file, line=dl.line, decl=dl.sig()))

    with open(os.path.join(out, 'sret.csv'), 'w', newline='') as f:
        w = csv.DictWriter(f, ['symbol', 'kind', 'form', 'is_def', 'file', 'line', 'decl'], lineterminator='\n')
        w.writeheader(); w.writerows(rows)
    md = ['# Struct-return convention check', '',
          'Generated by tools/cleanup/signatures/sretcheck.py: symbols whose native-visible declarations mix a '
          'by-value struct (or qword) result with an explicit leading out-pointer, and function-pointer table entries '
          'whose definition does not have the form the table is called through.', '',
          f'Listed: {len(listed)} ({sum(1 for x in listed if x["kind"] == "declarations")} by declarations, '
          f'{sum(1 for x in listed if x["kind"] != "declarations")} table entries). Callback-API declarations with '
          f'an integer PMF or argument word: {sum(1 for r in rows if r["kind"] == "callback API")}.', '',
          '| symbol | kind | definition | value form | out form | value sites | out sites |', '|---|---|---|---|---|---|---|']
    for x in listed:
        md.append(f"| `{x['symbol']}` | {x['kind']} | {x['definition']} | `{x['value_decl']}` | `{x['out_decl']}` | "
                  f"{x['value_sites']} | {x['out_sites']} |")
    cb = [r for r in rows if r['kind'] == 'callback API']
    if cb:
        md += ['', '## Callback API declarations', '', '| symbol | issue | site | declaration |', '|---|---|---|---|']
        md += [f"| {r['symbol']} | {r['form']} | {r['file']}:{r['line']} | `{r['decl']}` |" for r in cb]
    open(os.path.join(out, 'sret.md'), 'w').write('\n'.join(md) + '\n')
    print(f'{len(listed)} listed ({sum(1 for x in listed if x["kind"] == "declarations")} symbols by declarations, '
          f'{sum(1 for x in listed if x["kind"] != "declarations")} table entries); {len(cb)} callback-API declarations')
    print(f'outputs: {out}/sret.csv, sret.md')
    return 0


if __name__ == '__main__':
    sys.exit(main())
