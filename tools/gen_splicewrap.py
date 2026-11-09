#!/usr/bin/env python3
"""Generate C for the 438 splice binding wrappers (functions that call RefSetArgsFromSplice).

Shape (C++):  CRef Wrapper(BASIC *pbasic, int cref, CRef *aref)
  { OTYP aotyp[n] = {...}; T0 a0; T1 a1; ...
    CRef ref = RefSetArgsFromSplice(n, aref, aotyp, &a0, &a1, ...);
    <result setup + native call>;  return ref; }
We emit C with the hidden return pointer and ctor/dtor calls explicit, reusing m2c's draft for the
native-call part. Every output is compile-verified (as a standalone C file); matches go to
src/generated/splicewrap/NAME.inc. The .inc files are not compiled on their own: they are fragments of the
crt0_splicewrap TU, src/game/app/class_registry.cpp, which includes each one at its address inside
SLY2_SPLICEWRAP_BEGIN(NAME) / SLY2_SPLICEWRAP_END (a new wrapper needs that block added by hand). One TU because the
original is one: its functions and their .rodata literals interleave in address order (docs/guides/tu-merging.md,
"class_registry"). Needs the matching-era helpers automatch/quickfix/c_rewrites, which are no longer in the repo.
The committed .inc files were later made valid ISO C++ by hand-run edits that keep the objects identical: (s32)
casts where pbasic is passed for an s32, (char *)pbasic for byte offsets, and extern "C++" labelled prototypes where
two wrappers' C prototypes of one native function differ; a regeneration would have to emit those.
"""
import os, re, sys, subprocess, csv, struct
sys.path.insert(0, os.path.dirname(__file__))
import matchtool as M
import automatch as A
import quickfix as Q
from c_rewrites import voidify

RO_VA, RO_OFF = 0x262380, 0x162400
ELF = open(M.ELF, 'rb').read()

def ro_word(va):
    off = va - RO_VA + RO_OFF
    return struct.unpack_from('<i', ELF, off)[0]

def parse_call_setup(asm):
    """Find the RefSetArgsFromSplice call and the register setup feeding it."""
    lines = [l.split('*/', 1)[1].strip() for l in asm.split('\n') if re.match(r'\s*/\* [0-9A-F]+ [0-9A-F]{8} [0-9A-F]{8} \*/', l)]
    idx = next((i for i, l in enumerate(lines) if 'RefSetArgsFromSplice' in l), None)
    if idx is None: return None
    setup = lines[max(0, idx - 40):idx + 2]
    regs = {}
    for l in setup:
        m = re.match(r'(addiu|daddu|lui|ori)\s+\$(\w+),\s*\$(\w+),\s*(.+)$', l)
        if m and m.group(1) == 'addiu':
            regs[m.group(2)] = (m.group(3), m.group(4).strip())
        elif m and m.group(1) == 'daddu':
            regs[m.group(2)] = ('mov', m.group(3))
    n = None
    if 'a1' in regs and regs['a1'][0] == 'zero':
        n = int(regs['a1'][1], 0)
    elif regs.get('a1') == ('mov', 'zero'):
        n = 0
    def off_of(r, depth=0):
        v = regs.get(r)
        if v and v[0] == 'mov' and v[1] != 'sp' and v[1] in regs and depth < 4: return off_of(v[1], depth + 1)
        if v == ('mov', 'sp'): return 0
        if v and v[0] == 'sp': return int(v[1], 0)
        return None
    outs = []
    for r in ('t0', 't1', 't2', 't3'):
        o = off_of(r)
        if o is not None: outs.append(o)
    stk = []
    for l in setup:
        m = re.match(r'sw\s+\$(\w+),\s*(0x[0-9A-Fa-f]+|\d+)\(\$sp\)', l)
        if m and m.group(1) in regs and regs[m.group(1)][0] == 'sp':
            stk.append((int(m.group(2), 0), int(regs[m.group(1)][1], 0)))
    outs += [o for _, o in sorted(stk)]
    aotyp = regs.get('a3')
    # static OTYP table copied with ldl/ldr from %lo(D_xxx)
    static = None
    for l in lines[:idx]:
        m = re.search(r'%lo\((D_[0-9A-F]+)\)', l)
        if m and ('ldl' in ' '.join(lines[:idx]) or 'lw' in l): static = m.group(1)
    stack_out = []  # >4 outputs passed on the stack: sw $vX, off($sp) of addiu $vX,$sp,...
    return dict(n=n, outs=outs, aotyp=aotyp, aot_off=off_of('a3'), ref_off=off_of('a0'), static=static, lines=lines, idx=idx)

def gen(name):
    asm = A.split_asm()[name]
    info = parse_call_setup(asm)
    if not info or info['n'] is None: return None, 'no setup'
    draft_p = os.path.join(M.ROOT, 'build', 'automatch', name + '.c')
    if not os.path.exists(draft_p): return None, 'no draft'
    draft = open(draft_p).read().replace('#include "m2c_prelude.h"\n', '')
    m = re.search(r'\n[^\n]*\b%s\(([^)]*)\) \{\n(.*?)\n\}' % re.escape(name), '\n' + draft, re.S)
    if not m: return None, 'no body'
    decls = '\n' + draft[:draft.find(m.group(0).lstrip('\n'))]
    body = m.group(2)
    n = info['n']
    # locals m2c declared for stack slots and their types
    sptypes = {}
    for d in re.finditer(r'^\s*([\w\s\*]+?)\s+(sp[0-9A-F]+);\s*$', body, re.M):
        sptypes[int(d.group(2)[2:], 16)] = d.group(1).strip()
    body = re.sub(r'^\s*[\w\s\*]+?\s+sp[0-9A-F]+;\s*\n', '', body, flags=re.M)
    aot_off = info['aot_off'] if n else None
    ref_off = info['ref_off']
    if ref_off is None: return None, 'ref slot unknown'
    otyp_vals = []
    if n:
        if info['static']:
            base = int(info['static'][2:], 16)
            otyp_vals = [str(ro_word(base + 4 * k)) for k in range(n)]
        else:
            for i in range(n):
                mm = re.search(r'\bsp%X = (-?(?:0x[0-9A-Fa-f]+|\d+))[^;]*;' % (aot_off + 4 * i), body)
                if not mm: return None, 'otyp unknown'
                otyp_vals.append(mm.group(1))
                body = re.sub(r'^\s*sp%X = [^;]*;\s*\n' % (aot_off + 4 * i), '', body, flags=re.M)
    body = re.sub(r'^\s*M2C_ERROR\(/\* unknown instruction: (ldl|ldr|sdl|sdr) [^*]*\*/\);\s*\n', '', body, flags=re.M)
    # m2c's own local declarations (registers) must stay at the top
    DECL = r'^\s*(?!return\b|goto\b|break\b|continue\b)[A-Za-z_][\w\s\*]*?\s\**\w+;\s*'
    m2c_decls = [d.rstrip() for d in re.findall(DECL + '$', body, re.M)]
    body = re.sub(DECL + '\n', '', body, flags=re.M)
    names = {ref_off: ('ref', 'CRef', '')}
    if n: names[aot_off] = ('aotyp', 'OTYP', f'[{n}]')
    for k, off in enumerate(info['outs']):
        t = sptypes.get(off, 's32')
        if t in ('M2C_UNK', 'M2C_UNK32', '?'): t = 's32'
        names[off] = (f'arg{k}_v', t, '')
    decl_lines = []
    for off in sorted(names):
        nm, t, arr = names[off]
        if nm == 'aotyp' and info['static']:
            decl_lines.append(f'    OTYP aotyp[{n}] = {{ {", ".join(otyp_vals)} }};')
        else:
            decl_lines.append(f'    {t} {nm}{arr};')
    pre = [f'    aotyp[{i}] = {v};' for i, v in enumerate(otyp_vals)] if n and not info['static'] else []
    call = f'    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, {n}, aref, {"aotyp" if n else "NULL"}' + ''.join(f', &{names[o][0]}' for o in info['outs']) + ');'
    body2, cnt = re.subn(r'^\s*RefSetArgsFromSplice__FiP4CRefP4OTYPe\([^;]*\);\s*$', call.replace('\\', '\\\\'), body, count=1, flags=re.M)
    if cnt == 0: return None, 'no call line'
    def addr(off):
        nm = names.get(off)
        if nm is None: return None
        return nm[0] if nm[0] == 'aotyp' else '&' + nm[0]
    def val(off):
        nm = names.get(off)
        if nm is None: return None
        return 'aotyp[0]' if nm[0] == 'aotyp' else nm[0]
    bad = []
    def sub_addr(mm):
        a = addr(int(mm.group(1), 16)); 
        if a is None: bad.append(mm.group(0)); return mm.group(0)
        return a
    def sub_val(mm):
        a = val(int(mm.group(1), 16))
        if a is None: bad.append(mm.group(0)); return mm.group(0)
        return a
    body2 = re.sub(r'&sp([0-9A-F]+)\b', sub_addr, body2)
    body2 = re.sub(r'\bsp([0-9A-F]+)\b', sub_val, body2)
    body2 = re.sub(r'(?<![\w&])sp\b', lambda mm: addr(0) or 'sp', body2)
    if bad or re.search(r'(?<![\w&])sp\b', body2): return None, 'unmapped stack slot'
    body2 = re.sub(r'\barg0\b', 'ret', body2); body2 = re.sub(r'\barg1\b', 'pbasic', body2)
    body2 = re.sub(r'\barg2\b', 'cref', body2); body2 = re.sub(r'\barg3\b', 'aref', body2)
    decls = '\n'.join(l for l in decls.split('\n') if not re.search(r'RefSetArgsFromSplice|func_00129EC8|func_0012A0B0|func_001291A8|^\s*$', l))
    body2 = body2.replace('func_00129EC8', 'Decref__4CRef').replace('func_0012A0B0', 'InitTag__4CRef').replace('func_001291A8', '__4CRefRC4CRef')
    src = '#include "splicewrap.h"\n' + decls + f'\n\nCRef *{name}(CRef *ret, void *pbasic, s32 cref, CRef *aref)\n{{\n' + '\n'.join(decl_lines) + '\n' + ''.join(d + '\n' for d in m2c_decls) + '\n'.join(pre) + ('\n' if pre else '') + body2 + '\n}\n'
    return voidify(src), 'ok'

def variants(src):
    out = [src]
    # 1) unused decoded args: m2c dropped them from the native call -> append to the call that uses the others
    used = lambda v, t: len(re.findall(r'\b%s\b' % v, t)) > 2   # decl + &v in RefSetArgs + a use
    outs = re.findall(r'\b(arg\d+_v)\b;', src)
    unused = [v for v in outs if not used(v, src)]
    if unused:
        lines = src.split('\n')
        for i, l in enumerate(lines):
            if 'RefSetArgsFromSplice' in l or '__4CRefRC4CRef' in l or 'Decref__4CRef' in l or 'InitTag' in l: continue
            m = re.search(r'(\w+)\(pbasic(?:, [^()]*)?\)', l)
            if m and any(re.search(r'\b%s\b' % v, l) for v in outs if v not in unused):
                call = m.group(0)
                new = call[:-1] + ''.join(', ' + v for v in unused) + ')'
                l2 = lines[:]; l2[i] = l.replace(call, new); out.append('\n'.join(l2))
                # also widen its prototype
                fn = m.group(1)
                out[-1] = re.sub(r'(\b%s\([^;]*)\);' % fn, lambda mm: mm.group(1) + ''.join(', s32' for _ in unused) + ');', out[-1], count=1)
                break
    # 2) hoist a nested call out of a setter:  f(&ref, g(...)) -> tmp = g(...); f(&ref, tmp)
    for base in list(out):
        m = re.search(r'^(\s*)(\w+)\(&ref, (\w+\((?:[^()]|\([^()]*\))*\))\);', base, re.M)
        if m:
            ind = m.group(1)
            rep = f'{ind}{{\n{ind}    s32 tmp = {m.group(3)};\n{ind}    {m.group(2)}(&ref, tmp);\n{ind}}}'
            out.append(base[:m.start()] + rep + base[m.end():])
    return out

def main():
    src_all = open(os.path.join(M.ROOT, 'asm', 'text.s')).read()
    names = [m.group(1) for m in re.finditer(r'glabel (\S+)\n(.*?)endlabel', src_all, re.S) if 'RefSetArgsFromSplice' in m.group(2)]
    if len(sys.argv) > 1: names = sys.argv[1:]
    os.makedirs(os.path.join(M.ROOT, 'src', 'generated', 'splicewrap'), exist_ok=True)
    stats = {}
    for nm in names:
        if os.path.exists(os.path.join(M.ROOT, 'src', 'generated', 'splicewrap', nm + '.inc')): stats['already'] = stats.get('already', 0) + 1; continue
        src, why = gen(nm)
        if src is None:
            stats[why] = stats.get(why, 0) + 1; continue
        best = None
        for cand in variants(src):
            c = Q.score_src(nm, cand)
            if c is not None and (best is None or c < best[0]): best = (c, cand)
            if c == 0: break
        sc, src = (best if best else (None, src))
        open(os.path.join(M.ROOT, 'build', 'automatch', nm + '.sw.c'), 'w').write(src)
        key = 'match' if sc == 0 else ('cc_fail' if sc is None else 'diff')
        stats[key] = stats.get(key, 0) + 1
        if sc == 0:
            open(os.path.join(M.ROOT, 'src', 'generated', 'splicewrap', nm + '.inc'), 'w').write(src)
        if len(sys.argv) > 1: print(nm, key, sc)
    print(stats)

if __name__ == '__main__':
    main()
