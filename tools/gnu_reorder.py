#!/usr/bin/env python3
"""SDK-library assembly path ('// SLY2_ASM: gnu-reorder').

Sony's SDK objects were produced by a gcc 2.9x code generator run without delay-slot filling, then assembled
by a GNU as in reorder mode, which fills each branch/jump delay slot with the preceding independent instruction.
That assembler did NOT fill the slot of a backward branch (loop branch) - those keep a nop.
This module rewrites a reorder-mode .s so loop branches are emitted in noreorder with an explicit nop, then
assembles with the GNU as from ee-gcc 2.96 (tools/compilers/ee-gcc2.96)."""
import re, os, subprocess, sys

BRANCH = re.compile(r'^\s+(b|beq|bne|beqz|bnez|blez|bgtz|bltz|bgez|beql|bnel|beqzl|bnezl|blezl|bgtzl|bltzl|bgezl|bc1t|bc1f|bc1tl|bc1fl)\s+(.*),?\s*(\$L\d+|\.L\w+|[A-Za-z_]\w*)\s*$')

def pin_loop_branches(text):
    lines = text.split('\n')
    labels = {}
    for i, l in enumerate(lines):
        m = re.match(r'^(\$L\d+|\.L\w+):', l)
        if m: labels[m.group(1)] = i
    out = []
    noreorder = False
    for i, l in enumerate(lines):
        st = l.strip()
        if st == '.set\tnoreorder' or st == '.set noreorder': noreorder = True
        elif st == '.set\treorder' or st == '.set reorder': noreorder = False
        m = re.match(r'^\s+(\w+)\s+(.*)$', l)
        if m and not noreorder and m.group(1).startswith('b') and m.group(1) not in ('break',):
            target = m.group(2).split(',')[-1].strip()
            if target in labels and labels[target] < i:     # backward branch = loop branch
                # a label right before the branch stops GNU as moving the previous insn into its slot
                out += ['$Lgr_pin%d:' % i, l]
                continue
        out.append(l)
    return '\n'.join(out)

def unfill_short_loops(text, loopmax=5):
    """gcc reorg filled every slot; Sony's toolchain left the slot of a short single-block loop empty (nop).
    Move the slot instruction of such loop branches back in front of the branch (agents' gnu-reorg model)."""
    L = text.split('\n'); labels = {}
    for i, l in enumerate(L):
        m = re.match(r'^(\$L\d+):', l)
        if m: labels[m.group(1)] = i
    out = []; i = 0
    def isloop(a, b):
        for q in range(a, b):
            if re.match(r'^\s+(j\w*|b\w*)\s', L[q]) and not re.match(r'^\s+(jal|break)\s', L[q]): return False
        n = sum(1 for q in range(a, b) if re.match(r'^\s+[a-z]', L[q]) and not L[q].strip().startswith('.'))
        return n <= loopmax
    while i < len(L):
        l = L[i]
        if l.strip() == '.set\tnoreorder' and i + 5 < len(L) and L[i+1].strip() == '.set\tnomacro':
            br = L[i+2]; slot = L[i+3]
            m = re.match(r'^\s+(b\w*)\s+(.*)$', br)
            if m and not m.group(1).endswith('l') and L[i+4].strip() == '.set\tmacro' and L[i+5].strip() == '.set\treorder':
                tgt = m.group(2).split(',')[-1].strip()
                sm = re.match(r'^\s+\w+\s+(\$\w+)', slot)
                regs = set(re.findall(r'\$\w+', m.group(2)))
                wr_br = sm and sm.group(1) in regs
                if tgt in labels and labels[tgt] < i and isloop(labels[tgt], i) and not wr_br:
                    if slot.strip() != 'nop':
                        k = len(out) - 1
                        while k >= 0 and (out[k].strip() == '' or out[k].strip().startswith('#')): k -= 1
                        pm = re.match(r'^\s+\w+\s+(\$\w+)', out[k]) if k >= 0 else None
                        if pm and pm.group(1) in regs: out.insert(k, slot)
                        else: out.append(slot)
                    out.append(br); i += 6; continue
        out.append(l); i += 1
    return '\n'.join(out)

MULTI_CONST = re.compile(r'^\s+(li\.d|dli)\s')
CALL = re.compile(r'^\s+(jal|jalr)\s')

def pin_const_before_call(text):
    """Sony's assembler never filled a call's delay slot with the tail of a multi-instruction constant expansion
    (li.d / dli -> ori + dsll32 ...); GNU as does.  A label after such a macro, when the next instruction is a
    call, keeps GNU as from moving it (found on newlib scalbn, func_00249EE0).  The same holds for the one ordinary
    instruction right after the macro when the call follows it (libgcc __floatdisf: li.d; dsra; jal)."""
    L = text.split('\n'); out = []; pins_after = set()
    noreorder = False
    for i, l in enumerate(L):
        st = l.strip()
        if st in ('.set\tnoreorder', '.set noreorder'): noreorder = True
        elif st in ('.set\treorder', '.set reorder'): noreorder = False
        out.append(l)
        if i in pins_after: out.append('$Lgr_cpin2_%d:' % i)
        if not noreorder and MULTI_CONST.match(l):
            j = i + 1
            while j < len(L) and (not L[j].strip() or L[j].strip().startswith('#')): j += 1
            if j < len(L) and CALL.match(L[j]):
                out.append('$Lgr_cpin%d:' % i)
            elif j < len(L) and re.match(r'^\s+[a-z]', L[j]) and not L[j].strip().startswith('.'):
                # one ordinary instruction between the macro and the call: Sony's assembler didn't move that one into
                # the call's slot either (it came right after a macro expansion).  Pin it with a label after it.
                k = j + 1
                while k < len(L) and (not L[k].strip() or L[k].strip().startswith('#')): k += 1
                if k < len(L) and CALL.match(L[k]):
                    pins_after.add(j)
    return '\n'.join(out)

FCMP = re.compile(r'^\s+c\.\w+\.s\s')
BC1 = re.compile(r'^\s+bc1[ft]l?\s')

def fcmp_branch_nop(text):
    """Sony's assembler put a nop between an FPU compare (c.cond.s) and an immediately following bc1t/bc1f
    that gcc emitted in a noreorder block (the MIPS I/II compare->branch hazard); GNU as for the R5900 doesn't.  Found on libm acosf/asinf/atan2f."""
    L = text.split('\n'); out = []
    noreorder = False
    for i, l in enumerate(L):
        st = l.strip()
        if st in ('.set\tnoreorder', '.set noreorder'): noreorder = True
        elif st in ('.set\treorder', '.set reorder'): noreorder = False
        out.append(l)
        if FCMP.match(l):
            j = i + 1; entered_noreorder = False
            while j < len(L) and (not L[j].strip() or L[j].strip().startswith('#')
                                  or L[j].strip().startswith('.set')
                                  or re.match(r'^\$?\w+:$', L[j].strip())):     # a label (another branch jumps onto the bc1)
                if L[j].strip() in ('.set\tnoreorder', '.set noreorder'): entered_noreorder = True
                j += 1
            # only when gcc scheduled the branch itself (noreorder block); when GNU as handles the branch
            # (reorder mode) Sony's assembler added nothing (floorf)
            if j < len(L) and BC1.match(L[j]) and entered_noreorder:
                out.append('\tnop')
    return '\n'.join(out)

MFC1 = re.compile(r'^\s+mfc1\s+(\$\w+)\s*,')

def mfc1_branch_nop(text):
    """Sony's assembler kept the MIPS-I coprocessor-move delay: `mfc1 $r,$fN` immediately followed by a branch that
    gcc emitted in a noreorder block and that reads $r gets a nop in between (gcc writes `#nop` there; GNU as for the
    R5900 drops it).  mtc1 -> c.cond does NOT get one.  Found on libm __kernel_sinf/__kernel_cosf."""
    L = text.split('\n'); out = []
    for i, l in enumerate(L):
        out.append(l)
        m = MFC1.match(l)
        if not m: continue
        r = m.group(1); j = i + 1; entered = False
        while j < len(L) and (not L[j].strip() or L[j].strip().startswith('#') or L[j].strip().startswith('.set')):
            if L[j].strip() in ('.set\tnoreorder', '.set noreorder'): entered = True
            j += 1
        if j < len(L) and entered:
            bm = re.match(r'^\s+(b\w*)\s+(.*)$', L[j])
            if bm and r in re.findall(r'\$\w+', bm.group(2)):
                out.append('\tnop')
    return '\n'.join(out)

DIVS = re.compile(r'^(\s+)(div\.s|sqrt\.s|rsqrt\.s)\s')

def libm_div_nops(text):
    """'// SLY2_DIVNOP' (Sony libm): that compiler emitted every div.s as a noreorder `nop; nop; div.s` unit, even
    mid-block, and never placed a division in a delay slot.  Wrap every div.s that way; a div.s that gcc's reorg put
    in a branch delay slot is moved in front of the branch (an FPU divide never feeds the branch) and the slot
    gets a nop."""
    L = text.split('\n'); out = []; noreorder = False; i = 0
    def isdiv(l): return l.split()[:1] == ['div.s']
    def unit(l):
        ind = re.match(r'^\s*', l).group(0) or '\t'
        return [ind + 'nop', ind + 'nop', l]
    while i < len(L):
        l = L[i]; st = l.strip()
        if st in ('.set\tnoreorder', '.set noreorder'): noreorder = True
        elif st in ('.set\treorder', '.set reorder'): noreorder = False
        if noreorder and re.match(r'^\s+(b\w*|j|jal|jr|jalr|bc1\w*)\s', l) and i + 1 < len(L) and isdiv(L[i + 1]):
            ind = re.match(r'^\s*', l).group(0)
            out += unit(L[i + 1]) + [l, ind + 'nop']
            i += 2; continue
        if isdiv(l):
            if noreorder: out += unit(l)
            else:
                ind = re.match(r'^\s*', l).group(0)
                out += [ind + '.set\tnoreorder'] + unit(l) + [ind + '.set\treorder']
            i += 1; continue
        out.append(l); i += 1
    return '\n'.join(out)

def no_loop_fix(text):
    """'// SLY2_NOLOOPFIX': GNU as for the R5900 pads every short loop it assembles in reorder mode (and rejects one
    in noreorder); Sony's assembler didn't pad some (libmpeg _motionComp0's IPU wait loop).  For a backward branch
    whose loop body is plain single instructions, emit the branch as a raw .word in a noreorder block with a nop slot
    (the slot pin_loop_branches would give it anyway), so GNU as neither pads nor complains."""
    import asfix
    L = text.split('\n')
    labels = {}
    for i, l in enumerate(L):
        m = re.match(r'^(\$L\d+|\.L\w+):', l)
        if m: labels[m.group(1)] = i
    out = []; noreorder = False
    for i, l in enumerate(L):
        st = l.strip()
        if st in ('.set\tnoreorder', '.set noreorder'): noreorder = True
        elif st in ('.set\treorder', '.set reorder'): noreorder = False
        m = re.match(r'^(\s+)(b\w*)\s+(.*)$', l)
        if m and not noreorder and m.group(2) in asfix.COND:
            ops = [o.strip() for o in m.group(3).split(',')]
            target = ops[-1]
            if target in labels and labels[target] < i:
                n = 0; ok = True
                for q in range(labels[target] + 1, i):
                    c = asfix.classify(L[q])
                    if c[0] in ('blank', 'dir', 'label'):     # labels: pin_loop_branches' $Lgr_pin labels
                        if c[0] == 'dir' and not c[1].startswith('.set'): ok = False; break
                        continue
                    if c[0] != 'insn' or asfix.JUMPY.match(c[1]) or asfix.multi(c[1], c[2]) or c[1] in ('li', 'la', 'dli', 'li.s', 'li.d'):
                        ok = False; break
                    n += 1
                if ok and n:
                    ind = m.group(1)
                    w = asfix.encode(m.group(2), ops[:-1], -(n + 1))
                    out += [ind + '.set\tnoreorder', ind + '.word 0x%08X  # %s %s' % (w, m.group(2), m.group(3)),
                            ind + 'nop', ind + '.set\treorder']
                    continue
        out.append(l)
    return '\n'.join(out)

def run_linux_tool(cmd, root):
    """Run a 32-bit x86 Linux tool (GNU as from ee-gcc 2.96).

    On Linux it runs directly.  On macOS it can't, so it runs inside the toolchain Docker image
    (tools/docker/Dockerfile, built by `tools/dev.sh image`) with the repo and every directory named by an
    absolute path argument bind-mounted at the same path.  Docker Desktop shares /Users and the temp dirs
    (/var/folders, /private, /tmp) by default.  Override the image with $SLY2_IMAGE."""
    if sys.platform != 'darwin':
        return subprocess.run(cmd, capture_output=True, text=True)
    dirs = {os.path.realpath(root)}
    for a in cmd:
        if os.path.isabs(a):
            dirs.add(os.path.realpath(os.path.dirname(a)))
    mounts = []
    for d in sorted(dirs):
        mounts += ['-v', f'{d}:{d}']
    real = [os.path.realpath(a) if os.path.isabs(a) else a for a in cmd]
    image = os.environ.get('SLY2_IMAGE', 'sly2-decomp-tools:latest')
    return subprocess.run(['docker', 'run', '--rm', '--platform', 'linux/amd64'] + mounts +
                          ['-w', os.path.realpath(root), image] + real, capture_output=True, text=True)

def assemble(s_path, obj, root, mode='gnu-reorder', divnop=False, noloopfix=False):
    g = os.path.join(root, 'tools', 'compilers', 'ee-gcc2.96')
    txt = open(s_path).read()
    if mode == 'gnu-reorg':
        txt = unfill_short_loops(txt)
    txt = pin_loop_branches(txt)
    txt = pin_const_before_call(txt)
    txt = fcmp_branch_nop(txt)
    txt = mfc1_branch_nop(txt)
    if divnop:
        txt = libm_div_nops(txt)
    if noloopfix:
        txt = no_loop_fix(txt)
    s2 = s_path + '.gr.s'
    open(s2, 'w').write(txt)
    p = run_linux_tool([os.path.join(g, 'ee', 'bin', 'as'), '-G0', '-EL', '-mabi=eabi', '-o', obj, s2], root)
    if p.returncode:
        sys.stderr.write(p.stderr)
    return p.returncode
