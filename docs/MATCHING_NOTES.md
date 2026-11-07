# Sly 2 matching notes (compiler facts and techniques)

> **Historical document.** This was the working guide for the people and Claude agents who matched Sly 2,
> kept because it records the compiler facts and matching techniques we found. It refers to files that were
> removed when the project was finalized (`notes/`, `build/hand_wip/`, `docs/WORKLIST.md`, and helper scripts such
> as `worklist.py`, `permute.sh`, `quickfix.py`). They are all still in the git history at commit `e0aacd4`.


> **Merging per-function files into one file per translation unit** (the TU merge wave): see
> [guides/tu-merging.md](guides/tu-merging.md) for the procedure, the mismatch causes met so far and the merged TUs.

Goal: write C/C++ source that compiles to **byte-identical** code for functions of Sly 2 (SCUS_973.16).
Project root: `/home/claude/decomp`. Always run tools from the root.

## Your loop
1. `python3 tools/worklist.py LO HI [MAXINSTR]` — unmatched functions in your hex address range, smallest first.
   Re-run it now and then: other processes add matches and you must skip those.
2. `python3 tools/show.py NAME` — target disassembly, the best m2c draft (if any), and Sly 1 reference C if known.
   `python3 tools/matchtool.py target NAME` prints just the target asm.
3. Write `NAME.c` (or `.cpp`, see below) in the directory of the function's translation unit
   (`src/game/.../<unit>/`; look the unit up in `tools/layout/units.csv`) and check it:
   `python3 tools/matchtool.py check src/game/.../<unit>/NAME.c NAME`
   It prints `MATCH` or a side-by-side diff (`!!` marks differing lines; left = target, right = yours).
   Relocated operands (addresses of calls/globals) are masked, so `jal func_X` matches any symbol.
4. Iterate until MATCH. If after a real effort (~10–15 attempts) it still differs, move the best attempt to
   `build/hand_wip/NAME.c` (create the dir), note the remaining diff in your notes file, and move on.
   **Only matching files may stay under `src/`.**
5. Never run `tools/progress.py`, `automatch*.py`, `quickfix.py`, or edit anything in `tools/`, `include/`,
   `config/`, or files you did not create. Other agents work at the same time. Write only your own new files under `src/game/`,
   `build/hand_wip/*`, and your notes file `notes/<your-range>.md`.

## Compiler facts
- ee-gcc 2.95.3 (build 136), `-O2 -G0 -ffast-math -mvu0-use-vf0-vf31`. `.c` files compile as C, `.cpp` as C++
  (with `-fno-exceptions -fno-rtti`: the game's vtables have 0 in the typeinfo slot).
- The original is C++. Use `.cpp` when the symbol is a mangled name (e.g. `Free__9CDynArray`): declare the class
  locally in your file with the right member offsets and define the method. g++ 2.95 mangling: `Name__ClassArgs`,
  e.g. `Foo__4CBarii` = `CBar::Foo(int,int)`, `__4CBar` = ctor, `_$_4CBar` = dtor, free function
  `Foo__FP4CBari` = `Foo(CBar*, int)`. Arg codes: i int, Ui uint, f float, c char, Uc uchar, s short, l long
  (64-bit on EE: use `long`), Ul ulong, v void, P pointer, R reference, C const, `Q2..` nested.
- For `func_XXXXXXXX` names (unknown), write plain C with that exact function name. Types are free to choose.
- Shared headers you may `#include` (read-only): `common.h` (byte/uchar/ushort/uint/ulong(64-bit)),
  `vec.h` (VECTOR/VECTOR4), `mat.h` (MATRIX3/4), `qw.h` (QW), `bis.h`, `dmas.h`, `vifs.h`, `gifs.h`, `heap.h`.
  For anything else declare structs/externs locally in your file.
- Declare called functions with prototypes; an undeclared function returning int changes codegen.
  A call whose result is unused should be declared `void` (or cast the call to void).
- `$ra` saved with `sd`; callee-saved `$s` regs with `sq`. Struct returns use a hidden pointer in `$a0`.
- vtables: entries are 8 bytes `{short delta; short pad; fn}`. A virtual call loads `lh 8N(vt)` and `lw 8N+4(vt)`
  for slot N (slot 0 unused). vptr is placed after the data members of the class that first declares virtuals
  (offset 0 if that class has no data). Constructors return `this`.
- `ulong` (64-bit) ops appear as `dsll32`, `dsra32`, `daddu` etc. `daddu $x, $y, $zero` is just a move.
- Float constants come from `lui`+`mtc1`; `lwc1 f, %gp_rel` / absolute addresses are globals.

## Toolchain status (things the build now reproduces — don't fight them)
- Assembly goes through SN's `ps2eeas` (via tools/wibo_cc.py): FPU hazard nops (mtc1→use, c.cond→bc1x),
  64-bit constant expansion (`addiu -1; dsll32`), and short backward branch padding (tools/asfix.py) all
  match the original now. If you believe a remaining diff is a toolchain issue, write it in your notes with
  the function name instead of working around it.
- C++ is compiled with exceptions disabled (-fno-exceptions), so C++ objects with destructors and
  temporaries can be used naturally.

## Knowledge from earlier agents (read notes/*.md for details)
- 128-bit by-value arguments/returns (VECTOR passed in one GPR, `qword` params): use
  `typedef int qword __attribute__((mode(TI)));`.
- Allocation: func_001AE4F8 = operator new / alloc(cb). memcpy = func_0023320C, memset = func_002333B4:
  declare them under their func_ names (a prototype named memcpy/memset becomes a builtin).
- CRef = {int m_tagk; int m_tag}; tagk 0x15 int, 0x16 float, 0x17 bool, 0x24 void. func_00129EC8 = ~CRef,
  func_001291A8 = CRef copy ctor. Splice wrappers: see src/generated/splicewrap/ and notes/120000.md.
- DL list {pvFirst, pvLast, ibDle}, DLE {pvNext, pvPrev}; DLI iterator {pdl, ppv, ibDle, pdliNext},
  g_pdliFirst = D_002DD2FC (notes/130000.md has inline InitDli/PvNextDli that match).
- g_clock = D_002DDED0 {float t; float dt; ...} (notes/140000.md). Big game-state global D_003DD490.
- LO-style objects have the vptr at +0xC.
- Unsolved patterns (skip unless you have a new idea): "factory" functions `p = alloc(cb); p->vtbl = X; ...`
  where the target keeps the vtable address in $v1 (notes/110000.md); returning a float VECTOR in $v0 with
  pextuw/prot3w (notes/140000.md).

## Matching techniques that worked here
- **Store order**: gcc keeps source order of stores, except it sometimes rotates the last store to the front.
  If the target stores A,B,C try source order B,C,A.
- **Register allocation** differences: change declaration order of locals, merge/split temporaries,
  introduce a block-scoped temp, reuse a variable vs. new one, write `x = a; x |= b;` vs `x = a | b;`,
  swap operands of commutative ops / comparisons, `if (!c) {B} else {A}` vs `if (c) {A} else {B}`.
- Building a value in a temporary (`ulong tag = ...; tag |= ...; p->x = tag;`) vs inline expression changes
  instruction scheduling.
- `for` vs `while` vs `do-while`; `i++` vs pre-increment; pointer walking vs indexing; `x != 0` vs `x`.
- `beql`/`bnel` (branch-likely) come from loops/ifs the compiler optimised; you don't write them directly.
- Short backward loops are padded with `nop`s by the build (tools/asfix.py) — don't try to reproduce those nops.
- `pv->x = pv->y = pv->z = 0` (chained) gives a different store order / keeps 0.0f in an FPU reg.
- Non-rotated loops: `for (i = 0; ; i++) { if (i >= n) break; ... }`; `do { ... } while (1)` with breaks.
- `(a == b) & (c == d)` gives the movn/sltiu forms; `switch` for set-membership tests.
- Taking a global's address into a local first (`G *pg = &g;`) when the target keeps it in an $s register.
- An unused local struct can reproduce an otherwise unexplained stack frame size.
- Inline functions in headers (e.g. a small accessor) can explain duplicated code sequences.
- Switch statements produce jump tables (`lw` from a table in .rodata + `jr`).
- Functions using VU0 macro-mode (`vadd.xyz`, `qmtc2`, `lqc2`…) are hard; skip them (note them) unless small.

## Notes file
Keep `notes/<range>.md` with: struct layouts / class names you inferred (with offsets and evidence),
real names you are confident about (from Sly 1 counterparts, strings, vtables), and near-misses left in
`build/hand_wip/`. These get merged into shared headers later.

## Round-3 discoveries (from notes/1E0000.md and notes/200000.md — read those for details)
- Parameter order with floats changes integer register assignment: try moving float params (e.g. `(p, int, float, int, float, int, int)`).
- A separate counter per loop (`i`, then `j`) can fix saved-register priority.
- Declare callees with their real return type (memset/memmove return `void *`, constructors return `this`) — fixes $v0/$v1 choice.
- `qword` locals get a stack slot when their address is first taken; wrap in a union/struct to place at declaration.
- VU0: one inline-asm statement per VU op (vsub / dot / sqrt / qmfc2) lets gcc allocate VF regs like the original.
- EE varargs: `__builtin_next_arg` / `__builtin_args_info(2)` for va_start, `*(T*)((ap += 8) - 8)` for va_arg.
- `extern const float` lets a global float load be hoisted from a loop; `int unused[4]` explains a frame with ra at sp+0x10;
  `while (f(p), cond)` reproduces calls inside a loop test; an empty `case 0:` can reproduce a switch tree.
- Jump tables: read from disc/SCUS_973.16 at file offset 0x80 + (addr - 0x100000).

## Sony SDK library code (0x234F48–0x2550A0)
- Built by Sony with a modified 2.95.x compiler we do NOT have (see notes/234F48.md, notes/245000.md). ee-gcc 2.96 is NOT it
  (always keeps a frame around tail calls, mis-rounds float constants).
- Best available: `// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136` + `// SLY2_CFLAGS: -O2 -G0` in src/lib/NAME.c.
  Matches leaf functions and functions whose frame only holds $ra + locals (no saved $s regs, no tail calls,
  no mult1, no padded short loops, no float compare-branch, no assembler-built 64-bit constants).
- `long long` is 128-bit on the EE; use `long` for 64-bit values.

## Toolchain fix (round 5)
- tools/asfix.py now pads `L: b L` (while(1);) and short loops that start with a `jal` (call + empty delay slot)
  to 6 instructions like the original. Re-verified: no regressions. If you see one extra nop before a short
  backward branch, report it in your notes with the function name.
- `_start` (crt0) is hand-written asm in the original; it is kept as a top-level `__asm__` block in src/game/platform/boot/crt0/_start.c.

## Toolchain fixes (round 6) — IMPORTANT, supersedes earlier "nop before div" advice
- The build now inserts the two nops Sly 2's assembler put before div.s / sqrt.s / rsqrt.s / vdiv / vsqrt / vrsqrt
  when that instruction starts a basic block (function entry, branch target, or right after a branch + delay slot;
  calls don't count). Do NOT write `__asm__("nop\n\tnop")` anymore — write the plain C division / sqrt.
- Unconditional backward `b` loops in noreorder blocks, and loops whose first instruction is any branch/jump,
  are now padded like the original (UpdateJoy, func_001974C8 match without hacks).
- Kernel syscall stubs (`addiu v1,$zero,N; syscall; jr ra; nop`) are done as asm blocks in src/lib/syscall/.

## SDK assembler mode: `// SLY2_ASM: gnu-reorder`  (added after round 8 — use it for Sony SDK code)
- Sony's SDK objects = gcc code generator WITHOUT delay-slot filling + GNU as in reorder mode, which fills every
  branch/jump delay slot with the preceding independent instruction, EXCEPT loop (backward) branches, which keep a nop.
- Header for SDK files:   // SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
                          // SLY2_CFLAGS: -O2 -G0
                          // SLY2_ASM: gnu-reorder
  (tools/gnu_reorder.py adds -fno-delayed-branch, pins loop branches, assembles with ee-gcc2.96's GNU as.)
  Verified: kCopy, kCopy_00239898, kputchar, memclr match this way. Also try SLY2_CC: tools/compilers/ee-gcc2.96 with it.
- Do NOT add your own `nop`s for delay slots or short-loop padding in this mode; the assembler handles them.

## SDK build modes (round 10) — pick per library, all with SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
| `// SLY2_ASM:` | Model | Libraries seen so far (verified matches) |
|---|---|---|
| `gnu-reorder`    | gcc delay-slot filling OFF, GNU as fills slots, loop slots stay nop | libkernel: kCopy*, memclr, kputchar, DIntr, _sceIDC |
| `gnu-reorder-db` | gcc reorg fills slots (incl. from branch targets, beql), GNU as fills what's left | libmpeg/libipu (sceMpegInit, sceIpuSync), libm (floorf), libgraph/libdma (sceDmaPutEnv), libgcc fp-bit (__pack_d) |
| `gnu-reorg`      | as gnu-reorder-db, but slots of short (<=5 insn) single-block loops are emptied | libgcc dp-bit (litodp) |
- Try `-fstrict-aliasing` in SLY2_CFLAGS: Sony's lib build had it on for some files (sceDmaPutEnv, iAllocTimerCounter).
- Still unsolved toolchain issues (skip these functions): `$s` regs saved with `sd` into 16-byte slots; frameless `j` tail calls;
  `mult1`/`mult` constant strides (scePad); the mfc1->branch hazard nop in libm.

## Housekeeping rules
- Never `mv`/`cp` anything to /dev/null or other system paths. To set a file aside, move it to build/hand_wip/.
- Never run git. The coordinator commits and pushes.
- Ranges are split three ways (see ASSIGNMENTS.md): Dimitri + Claude agents 0x100000–0x190000; NaGa 0x190000–0x1F0000; Lilly 0x1F0000–0x230000 and 0x234F48–0x2550A0. Claude agents must stay OUT of NaGa's and Lilly's ranges.

## Divide/sqrt hazard padding (2026-10-16)
`tools/asfix.py` now pads div.s/sqrt.s/rsqrt.s/vdiv/vsqrt/vrsqrt to be at least the 3rd instruction of its basic
block: 2 nops at block position 0, 1 nop at position 1. Positions count real machine instructions (`li.s` = 1–3,
`li`/`la` = 1–2) and the padding nops themselves. Verified on a full uncached rebuild (no regressions; +func_0011D018).

## `// SLY2_SAVES: sd` (Sony SDK register saves, 2026-10-16)
Sony's SDK compiler saved/restored callee-saved registers with 64-bit `sd`/`ld` (still in 16-byte stack slots);
the ee-gcc builds we have emit 128-bit `sq`/`lq`. Add `// SLY2_SAVES: sd` (only together with `SLY2_ASM:`) and
matchtool rewrites sq/lq to sd/ld at assembly time. This emulates the compiler difference, like asfix.py does for
the game's assembler; do not put the macro in the C source. Typical SDK header set:
`SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114` / `SLY2_CFLAGS: -O2 -G0` / `SLY2_ASM: gnu-reorder-db` / `SLY2_SAVES: sd`.

## Short loops with an inner forward branch (2026-10-16)
asfix.py now also pads short loops that contain a forward conditional branch in reorder mode, when the
instruction before that branch writes a register the branch tests (so its delay slot is certainly a nop): the branch
counts as 2 instructions. A branch that is the loop's first instruction keeps the older rule. Fixed strchr
(func_0021EDF8) and func_001A6260; full uncached rebuild shows no regressions.

## Dead-code scraps: config/junk.csv (2026-10-16)
The original linker left small scraps of dead code between functions: orphan `addiu $sp,$sp,+N` / `nop` epilogue
pieces and lone delay-slot instructions with no return. 141 such ranges are listed in `config/junk.csv`; they are
not functions, so progress.py, the graph and worklist.py skip them (their bytes are still part of .text for a full
rebuild). Three entries that were a scrap followed by a real tiny function were split, so the function now starts
at the real address (func_00119BC8, func_0014C1F0, func_00259D60, and 9 more in a second pass that also treats
a leading "orphan instruction + nop" pair as scrap). Totals dropped from 6,813 to 6,661 functions.
If you find another scrap, add its range to junk.csv with a reason rather than "matching" it.

Update (link/junk-padding, 2026-11-06): each scrap is a run of 8-byte units, one orphaned return-delay-slot
instruction plus its alignment nop: the remains of functions that are not in the ELF (most likely cut an instruction
short when unused functions were removed). Inside a TU's range the source now emits them in place so that the object's
`.text` links as one run: `#include "sly2/scrap.h"` and `SLY2_SCRAP("addiu $sp, $sp, 0x30");` per unit, written
after the previous function and before any data definition (a top-level asm lands in whatever section gcc is in).
A unit can be a plain `nop` (8 zero bytes before a function are such a unit, not 16-byte alignment). The junk.csv
rows emitted this way end with "emitted by its TU source (SLY2_SCRAP)"; they stay out of the function counts. Details
and evidence: [full-link.md, "Junk scraps and padding"](guides/full-link.md#junk-scraps-and-padding-linkjunk-padding-2026-11-06).

## Callee return types matter (2026-10-16)
Declare callees with their real return type even when the result is ignored: constructors, copy constructors and
CRef::operator= (func_00129658 returns CRef*, func_001291A8 returns void*) return `this`. Declaring them `void`
changes scheduling/argument order across the *whole* caller. This finished func_00200DA0, func_00200F58,
func_001D54B8 and func_001DE008. (The reverse also happens: func_0011B500's result is unused in one caller, and there
`void` matched — try both.)

## Empty asm barriers (policy, 2026-10-16)
`__asm__ __volatile__("")` emits no instructions; it only stops gcc moving code across it. It is accepted in matched
files when it stands in for a known difference between our compiler builds and the ones the game was built with
(e.g. Sony's SDK scheduler keeps the use of a call's return value right after the jal). Prefer natural C when it
exists, and leave a one-line comment saying what the barrier emulates.

## SDK modes: FPU compare -> branch nop and mfc1 -> branch nop (2026-10-16)
Also: labels between c.cond.s and the bc1 are skipped (the nop still goes after the compare), and `mfc1 $r,$fN`
followed by a gcc-scheduled branch reading $r gets a nop (Sony kept the coprocessor-move delay). Matched
__kernel_rem_pio2f, __kernel_sinf, __kernel_cosf; all 272 SDK-mode files re-verified.
gnu_reorder.py also inserts a nop between `c.cond.s` and a following bc1t/bc1f when gcc emitted the branch in a
noreorder block (Sony assembler hazard rule; matched acosf/asinf/atan2f, floorf unaffected). SLY2_SAVES now only
rewrites $sp-based sq/lq (real 128-bit data stores stay sq). All 113 SDK-mode files re-verified.

## SDK modes: constant expansion before a call (2026-10-16)
tools/gnu_reorder.py now also stops GNU as from moving the tail of a multi-instruction constant (`li.d`, `dli`) into
the delay slot of an immediately following `jal`/`jalr`; Sony's assembler left a nop there. All 101 SDK-mode files
re-verified; this finished newlib scalbn (func_00249EE0).

## GSmooth prototype (2026-10-16)
func_0011D018 is Sly 1's GSmooth: `float GSmooth(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext)`.
Declaring it with that argument order fixes argument-setup order in its callers (it alone matched func_00150160).
Check other drafts that call 0x11D018 with a different prototype.

## `// SLY2_DIVNOP` (Sony libm divisions, 2026-10-16)
Sony's libm compiler emitted every `div.s` as a `nop; nop; div.s` unit, even mid-block, and never put a division in
a delay slot. Opt-in header `// SLY2_DIVNOP` (with SLY2_ASM) makes gnu_reorder.py reproduce that (and moves a div.s
that gcc placed in a delay slot in front of its branch). Matched __ieee754_atan2f; atanf also needs the source tweak
in notes/245000.md. Use only for libm files that show the pattern.

## Junk list correction
Two `nop`s directly before a `div.s` at a function's start are the divide-hazard padding, not scrap: 0x117120 was
wrongly listed and is fixed (func_00117120 now matches).

## New compilers: ee-gcc 2.9 and 3.2 (2026-10-16)
`tools/compilers/extra/` now has ee-gcc2.9-990721, 2.9-991111, 2.9-991111a, 2.9-991111-01, 2.9-991111-01-dtls13010,
3.2-030926, 3.2-040921 (native Linux; use with `// SLY2_CC: tools/compilers/extra/<name>`). ee-gcc 2.9 emits `mult1`
and frame-teardown `j` tail calls and saves registers with `sd` natively (no SLY2_SAVES needed) — it is at least one
of Sony's SDK compilers: setD3_CHCR matched with `SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111`,
`SLY2_CFLAGS: -O2 -G0`, `SLY2_ASM: gnu-reorder-db`. The "mult1" and "j tail call" blockers in notes/23C000.md are
therefore probably solvable: retry them with ee-gcc 2.9 first. ee-gcc 3.2 also emits both but laid out setD3_CHCR differently.

## Real compiler instead of emulation (2026-10-16)
71 Sony SDK files that matched with ee-gcc 2.95.3-114 + `SLY2_SAVES: sd` emulation also match with the real Sony
compiler ee-gcc 2.9-991111 (or 991111-01) with no emulation, so they were switched to it. 25 still need the 114 +
SLY2_SAVES form (their source was tuned for it). New SDK work should start with ee-gcc 2.9.

## decomp-permuter for plain-C near-misses (2026-10-16)
`bash tools/permute.sh build/hand_wip/NAME.c [seconds] [jobs]` runs decomp-permuter (random source rewrites scored
against the original bytes; tools/perm_objdump.py stands in for MIPS objdump). Works only on plain C drafts without
inline asm (pycparser). Candidates land in /tmp/perm/NAME/output-*/source.c; ALWAYS verify a candidate with
matchtool and then clean it up into readable C before saving it to src/. Use at most 1 job per agent (2 cores total).

## FPU compare → short-loop branch (asfix, 2026-10-16)
When a short loop ends `c.cond.s; bc1x`, SN's assembler inserts its compare→branch hazard nop, which makes the loop one
instruction longer (often long enough that it is no longer padded). asfix now counts that nop. If a loop that ends in an
FPU compare/branch used to be off by one nop, recheck it: func_001E7538 matched with no source change.

## Allocation-only asm operands (policy, 2026-10-16)
In a function that already uses inline VU asm, an extra *unused* operand on an existing asm statement (e.g. listing
`"j"(dv)` twice) is accepted the same way register pins are. It emits no instructions and only changes the
register-allocation heuristics. Say so in a comment on that line. Don't add new asm statements just for this.
First example: src/game/engine/anim/act.cpp.

## Float vs. int parameter order is invisible in the callee (2026-10-16)
In the EE EABI, float arguments go in $f12.. and integer/pointer arguments go in $a0.., each counted separately. So
`f(int *p, float g)` and `f(float g, int *p)` compile to identical callee code, and a matched callee's prototype
order is only a guess. The caller is where the order shows: gcc evaluates arguments in prototype order, which changes
scheduling, e.g. whether `&hit` is formed before or after a float argument that contains a call. When a caller
matches only with a different float/int order, use that order (it's the real one). If you can, update the shared
prototype and re-check the callee and its other callers. Examples: func_00117120 (floats first, func_0012E060) and
func_001A93C8 (float before HIT*, func_0013B5E8).

## Sony code: constants held in locals (2026-10-16)
When the original keeps a small constant (0/1/2/0x20…) in a callee-saved register ($s3/$s5) for a whole function, the
Sony source declared it as a local variable, e.g. `int one = 1;`, `int two = 2;`, or a reused flag like `sel`.
Write that local and use it in every place the original reads the register. This matched sceIoctl and _getAllRefs.

## Matrix-returning callees: by-value inline helper (2026-10-16)
If a function passes an M3 (3x3 matrix) returned by one call straight into another, write a `static inline` helper that
takes the M3 by value, and call it directly with the call that returns it: `Helper(GetMat(...), ...)`. This removes
the extra stack copy and matched func_00231300 and func_0014E1A8.

## Aligned difference count (2026-10-17)
`matchtool.py check` now also prints `aligned N`: the number of differences after lining up the two instruction
streams. With a positional count, one extra or missing instruction shifts every later instruction, so a draft that
is really 5 instructions off can show as 100+. RANKING.csv is sorted by the aligned count (column `aligned`). Use it
to judge how close a draft really is. Only an exact MATCH counts.

## Block-scoped locals change register assignment (2026-10-17)
Declaring a temporary inside the `if`/loop block that uses it, instead of at function scope, changes gcc's register
allocation and can fix saved-register swaps. This matched _sceCd_cd_read_intr. Related Sony libcdvd patterns: a
`static inline nopdelay()` helper and a `while(1){...continue;...break;}` retry loop (matched _sceCd_*_prechk).

## li.d / dli then one instruction then a call (gnu_reorder, 2026-10-17)
Sony's assembler also left the instruction right after a multi-instruction constant (li.d/dli) out of a following
call's delay slot. gnu_reorder now pins it, which matched libgcc __floatdisf. Check other libgcc converters
(__floatdidf etc.) with the same `li.d; insn; jal` shape.

## A target nop just before a loop head = a real loop in the source (2026-10-17)
If the target has a `nop` right before a backward-branch target that sits on an 8-byte boundary, and our build doesn't,
that nop is gcc's loop-head alignment (`.p2align 3`). gcc emits it only for real `for`/`while`/`do` loops, not for
loops built from gotos (and not after a loop rotation that gcc doesn't recognise as a loop). It is NOT an assembler
rule. Rewrite that loop as a real loop. If gcc then hoists constants out of it (loop.c), work on that next. Seen in
func_001F2500 and func_0012D508.

## SLY2_NOLOOPFIX: Sony code whose short loops are NOT padded (2026-10-17)
On the SDK assembly path, GNU as pads every short loop it assembles in reorder mode (the R5900 erratum fix). It has no
option to turn that off, and it rejects such a loop in noreorder. Sony's assembler didn't pad some loops (libmpeg
_motionComp0's IPU wait loop). Add `// SLY2_NOLOOPFIX` to the file header: gnu_reorder then emits each backward branch
whose loop body is plain single instructions as a raw `.word` in a noreorder block with a nop slot, so nothing is
padded. It's opt-in per file, so use it only when the target shows an unpadded short loop where ours has 1–2 extra
nops before the branch.

## One shared `return 0` vs several (gcc setcc) (2026-10-17)
When the target branches to a single shared `return 0` block but ours computes the result with slt/sltu/xori (a
setcc) or repeats the return, write the early exits as `goto fail;` (or `break` out of the loop) to one shared
`fail: return 0;`. That gave big drops on func_00136870, func_00154770 and func_00126A18.

## Let gcc emit qmfc2/qmtc2/mfc1 moves itself (2026-10-17)
gcc's reorg never puts an inline-asm instruction in a branch delay slot. If the target has a `qmfc2`/`qmtc2`/`mfc1`
in a delay slot (or scheduled across a branch) and ours has it as an asm statement, write the move as plain C (a
union/cast between the vector, qword and float views, or a plain assignment) so gcc emits it and can schedule it.
This matched func_0011EE08 and func_00151700.

## Empty asm barriers cost a scheduler slot (2026-10-17)
An empty `__asm__ volatile("")` used to force a reload isn't free: gcc's scheduler counts it as an instruction, so the
code around it gets ordered differently. If a draft has one and the target's order around that spot differs, replace
the barrier with a plain local of the right type (e.g. a `qword` local holding the reloaded value). That matched
func_00223A70 and took func_0020A818 from 14 to 2.

## Scoped object with an explicit ctor/dtor (2026-10-17)
If the target constructs a stack object, calls the constructor, and at the end calls `dtor(this, 2)`, declare that object
(e.g. FRAME) at function scope and write the ctor and dtor calls out explicitly. Matched func_0013B860.

## Share variables across switch cases (2026-10-17)
If the target reuses a "stale" pointer that an earlier case or block assigned, declare those pointers once at function
scope and reuse them in every block rather than giving each block its own locals. Matched func_001281B0 (108 → 0).

## Float from a qword: mtc1 instead of qmfc2 (2026-10-17)
If the target moves a float out of a VU qword with a single `mtc1`, write one asm that takes the qword as input and emits
`mtc1` directly, rather than `qmfc2 "=r"` into an int followed by a cast. Matched func_00145DB0 and func_001462A8.

## A stack "local struct" may be spilled parameters (2026-10-17)
When an old draft models a stack area as a local struct, check whether it's really the function's own parameters stored
to the stack (for example, arguments whose addresses are taken, or varargs-style spills). Use the parameters directly or take
their addresses. Matched func_0014E4C8 (20 → 0).

## For big, far-off drafts, rewrite fresh instead of patching (2026-10-17)
_PES_packet (~1900 instructions) matched in one session when it was written fresh from the asm plus the known library
structure: 339 → 38 → 29 → 17 → 12 → 0. Old m2c-based drafts with aligned > 20 often carry wrong structure that's slow to
patch out, so prefer a clean rewrite, built section by section, and recheck the aligned count after each section.

## Sony libm uses ee-gcc2.9-991111-01 (2026-10-17)
The libm float functions (acosf, asinf, powf, ...) were built with the **-01** compiler variant, ee-gcc2.9-991111-01. Its
`div.s` comes out as `nop; nop; div.s` and is never put in a delay slot. With that compiler, leave SLY2_DIVNOP out
(not even in a comment). If another Sony function's only remaining diff is a div/delay-slot gap, try the -01 variant.

## An unexplained alignment nop may be a dead loop (2026-10-17)
__unpack_d had a stray alignment nop that turned out to be the `.p2align` of a loop that never runs, here fp-bit's
denormal-normalising `while`. Write that loop out as in the original library source, even when it looks dead.

## Old register pins can be the problem (2026-10-17)
Many old drafts pin variables to registers (`register int x asm("$16")`) from earlier tries. Taking the pins out cut
func_001A64E8 from 26 to 8 and func_001FEA60 from 38 to 6. Removing ALL pins at once made 15 of 16 drafts worse, so
take them out ONE AT A TIME and keep each removal only if the aligned count drops (func_001A4AD8 17→12).

## "memory" clobbers fix the order of scratch stores (2026-10-17)
If an asm writes to scratch memory (stack or scratchpad) and the target orders loads and stores around it differently,
add a `"memory"` clobber to those asm statements so gcc doesn't move memory accesses across them. This matched
UpdateWrMatrixes__FP2WR (14 → 0).

## M3 matrix helpers: named return plus a dead copy (2026-10-23)
func_00122F00 (18 → 0) matched with a g++ **named-return-value** MulM3 helper (`M3 MulM3(...) return r { ... }`), plus a
per-row M3 copy that is never used afterwards but still shapes the stack frame and schedule. If a matrix function has
an unexplained stack copy, try a named return and a dead local copy.

## Take a global's address through its containing struct (2026-10-23)
If the target builds an address as `lui`/`addiu` of a struct base plus a field offset, not the field's own symbol,
write it as `&D_struct.field` rather than `&D_fieldsymbol`, even when both are the same address. This changes how gcc
splits the hi/lo parts and schedules them. Matched func_00143410 (`&D_003DD490.trie` instead of `&D_003E0EA8`).

## Unsigned loop counters stop gcc reversing a loop (2026-10-23)
If the target counts up while our build counts down (or the reverse), or the loop's exit test differs, try an
`unsigned` counter. In func_001477D8 it stopped gcc's loop reversal (98 → 90).

## Write asm output to a plain qword, then copy it into the union (2026-10-23)
An asm whose `"=r"`/`"=j"` output goes straight into a union member makes gcc store to the stack twice. Write the
output to a plain `qword` local and assign that to the union afterwards. This was the last fix for SmoothMatrix.

## Statement macros: a plain `{ }` block, not `do { } while (0)` (2026-10-23)
With this gcc, a `do { ... } while (0)` macro can leave an extra nop (it's a loop to the scheduler). If a macro body
shows up with a stray nop, define it as a plain `{ ... }` block. That helped func_0013C560 go from 46 to 13.

## Pass &local through a small inline helper to stop gcse sharing the address (2026-10-23)
When the target recomputes `&local` (an `addiu` from sp) at each use but our build computes it once and keeps it in a
register, pass the address through a small inline helper (e.g. `static inline T *Addr(T *p) { return p; }`) or have an
inline wrapper take the address itself. This stops gcse sharing the address. It matched func_00120FB0 (2 → 0) and
func_001300B8 (46 → 0).

## Use the scheduler dump to break ties; check callee return types (2026-10-23)
For a schedule-order diff, compile with `-fsched-verbose-5` and read the sched2 dump. When two instructions are tied, the
tie goes to the one with more dependent instructions later on. A call that "returns" a value in $v0 adds a dependence,
so **declaring a callee `void` when the target never uses its result** can flip the tie. That matched func_00230710
(1 → 0) with no pins. Also check every callee prototype: an `int` return the target never reads changes scheduling.

## Read the compiler's pass dumps to find the first divergence (2026-10-23)
When a draft is stuck, compile with gcc's dump flags (`-da` for every RTL pass, or `-fsched-verbose-5` for the
scheduler) and find the FIRST pass where our output differs from the target. Fix the C that feeds that pass.
- **.loop:** shows loop-invariant hoisting. func_0015B9F0 matched once a global was read into a local before the
  loop, which stopped `%hi(D_...)` being hoisted out.
- **sched1/sched2:** show priority ties and dependences (see the scheduler-dump section above).
- **.greg/.lreg:** show register allocation; a tie there can justify the one allowed pin.
- **.gcse:** shows the order gcse creates pseudos in, which can decide register and slot order.

## Comma increments and earlyclobber on asm outputs (2026-10-23)
- In a loop with two counters, `k++, i++` in one for-increment can produce a different schedule from two separate
  statements. That took func_0015B428 from 56 to 4.
- An earlyclobber output (`"=&j"`) forces a different register from every input. If the target reuses an input's
  register for the output, drop the `&` (`"=j"`). That was the last fix for func_0015B428 (4 → 0) and helped func_00137248.

## gcse hash order depends on symbol NAMES (2026-10-23)
gcse orders some expressions by a hash of the symbol name. So a global called `D_002DEA4C` can schedule differently
from the same global under its real name. Lanes D and E each found a function stuck for this reason (func_00157FA0, where
renaming the global took it 8 → 2, and func_0015CAD8). Globals are extern declarations, and their names never reach the
binary, so giving one a real name is legitimate. Look for the real name in Sly 1's symbols (config/harvest_sly1.csv,
sly1_matches.csv) or a matched sibling's usage. Use a plausible descriptive name, never one picked only to hit a hash.
Declare it in the shared header if siblings use it, and note the rename in the commit/notes.

## Clobber a dead argument register to delay a later setup (2026-10-23)
Adding a clobber of an argument register that's dead at that point (e.g. `"$4"` for a0) to an asm makes any later
setup of that register wait until after the asm. func_0011B500 matched with that plus an allocation-only `"+r"` operand.

## Some D_ "globals" are really local initializers (2026-10-23)
If a D_ symbol holds a small constant block that only one function uses (e.g. `{0.1f, 1.0f}`), it may be that
function's local aggregate initializer, `struct LM lm = {0.1f, 1.0f};`, which gcc puts in .rodata. Writing it
as a local changes register allocation. In func_00146B18 that let one pin be removed.

## Policy note: multi-alternative constraints (2026-10-23)
An asm constraint with an alternative that's never chosen (e.g. `"r,j"`) only steers allocation, so it counts as an
allocation-only operand: allowed, with a comment saying why (func_0011BA08).

## Loops that loop.c leaves alone: multiple entry points (2026-10-24)
If the -dL dump shows the target's loop was NOT optimized (no hoisting, no strength reduction), give the loop more
than one entry point, which makes loop.c skip it. Test the first element before the loop, then `goto` into a
`for (;;)` at its increment. That matched func_00149440. A loop built entirely from gotos does the same job
(func_00138970).

## 2026-10-24 (Claude agent): allocation-only operands that changed allocation
- **EnforceDzgDz** (8→0): one commented empty asm with allocation-only `"f"(1e-4f)` / `"f"(-1e-4f)` inputs. local-alloc doubles a constant's live length when it has an extra use, which moved the FPU register choice to the target's.
- **func_00155868** (2→0): extra unused `"j"(b), "r"(b), "r"(b)` operands make b GPR-class, so its qmtc2 is emitted as a reload in the target's position (one vf1 pin kept).
- **func_00179898**: a plain field load `q = p->p10->v3E0;` plus one commented empty `asm volatile("":::"memory")` before the call fixed the load order.

## 2026-10-24 (Claude agent): sqrt.s helper and Lilly's helpers
- An inline `SqrtF` helper (`sqrt.s` asm) is accepted: `__builtin_sqrtf` does not give the game's code (checked on func_001F70C0), and many matched files already use the helper. Other FPU one-liners that C can express (e.g. `abs.s` → `__builtin_fabsf`) should be plain C.
- Lilly's matched files (src/hand/, comment "ported from Lilly's personal Sly 2 decomp") are a good helper library: copying her MATRIX3 `operator*` from func_0021CB18.cpp as-is matched func_00142AF8.

- **Pass dumps:** use `sh tools/dump.sh <workdir> <draft> -da` (or `-fsched-verbose-5`). It compiles inside <workdir>, so the dump files never land in the repo root. Never pass -da through matchtool.py.

- **Const-ref helper params** (`const VU_VECTOR &a`) instead of by-value qwords changed spill slots and loop sizes enough to match func_00173F18 and to take func_00180668 17→3 and func_0018FC60 134→75 (2026-10-24).

- **Loop-size threshold:** loop.c hoists invariants differently once a loop body passes about 95 RTL insns. Const-ref helpers and extra copies add RTL insns without changing the final code, so they can push a loop over that line. That fixed the hoist order in func_0018F2B0 and func_00180668 (2026-10-24).

- **Reload round-robin:** reload hands out spill registers in turn. A commented clobber of a DEAD VF register on a nearby VU helper asm skips that register, so a later reload lands where the target has it. That matched func_00180668 (2026-10-24).

- **GPR reload round-robin:** to move a GPR reload, give a nearby VU helper asm output a never-chosen memory alternative, `"=r,m" : "j,!r"` (the `!` keeps reload from choosing m). Its output reload advances the round-robin. func_00169198 went 5→3 and func_00168638 33→31 (2026-10-24).

- **Growing loop RTL:** cse never removes 16-byte register copies from inline struct pass-throughs, and nothing runs cse between the two loop passes. So VU_VECTOR/VU_FLOAT class helpers that take and return by value add about 2 insns each to both loop passes and nothing to the final code. Use this to reach the loop-size thresholds when the target keeps invariants inside a loop (func_001477D8, lane L2, 2026-10-24). Write the source naturally; 50 nested identity calls is a probe, not source.

- **Giv set-up order:** gcc expands a nested inline-call argument before the other arguments, and loop.c emits giv set-ups in reverse order of discovery. Moving a nested call inside a helper body reorders the loop-pointer set-ups (matched func_00185E40, 2026-10-24).

- **"r,V" memory-input on qmtc2:** a never-chosen "V" alternative on a qmtc2 input keeps the word in memory until reload, which changes register and slot choices. It matched func_0014F110 (2026-10-24); worth trying on any qmtc2 from a global or struct word.

## 2026-10-24 (NaGa's lanes): what moved the last hard functions
Wave 1 of ten agents on 0x190000-0x1F0000 matched 15 functions that had resisted every earlier try (full per-function notes in
`notes/190000-1F0000_lanes.md`). The techniques that did it, newest first:
- **Un-hack drafts one pin / asm at a time.** A draft at aligned 1-5 that needs several pins or whole-instruction asm is only a map.
  Delete each pin or asm separately and keep the deletion if the count holds (func_00197500: 9 pins + a volatile asm block -> natural C).
- **Register swaps are global-alloc priority ties.** `-da` writes `.greg` ("N regs to allocate" is the priority order) and `.lreg`
  (refs, live length). Priority = floor_log2(refs) * refs / live_length, refs weighted by loop depth; it predicted every s-register
  choice. A `do { } while (0)` around one statement raises a global's depth; splitting a variable that is assigned in two blocks into two
  variables changes its priority.
- **Scheduler barriers.** A `volatile` asm is a full barrier (drop `volatile` when only data dependences matter). A `do { } while (0)` in a
  copy macro emits NOTE_INSN_LOOP_BEG, which sched also treats as a barrier (func_001A4AD8). sched1 ties go to the insn with more
  dependents (func_0019AC88: `h = t * 0.5f` instead of overwriting `t`). The "birthing insn" boost applies to registers set exactly once
  in the whole function.
- **loop.c / gcse.** A constant is hoisted out of a loop only when life * 95 * savings >= the loop's insn count (`-da -dL` prints
  "Loop from..: N real insns"; two loop passes). gcse's table size, (n_insns / 2) | 1 after cse1, decides PRE register numbering and spill
  order; a dead counter that survives cse1 can flip it (func_001EA000). gcse hoists a lone `sp+K` computed after a loop.
- **Spilled pseudo vs struct member.** A stack slot after the real locals that is reloaded around calls is a spilled pseudo: write a plain
  local (func_001E9858).
- **Call setup / returns.** Memmove-style `a0=s0; a1=s0; a0+=16` -> call through a `static inline` forwarder taking two `void *`. A target
  ending `li v0,K; jr ra; nop` with an unfilled slot -> a conditional `goto done;` to a shared `result = K; done: return result;`;
  separate `return` blocks instead of a shared variable let cross-jumping rebuild tails.
- **VU asm.** Rewrite from scratch with one asm per VU op; `"=&j"` for multi-insn outputs; a duplicated unused `"j"` input (commented) raises
  a value's allocation priority; `"=j"` instead of `"+j"` fixes vf numbering; `"rV"` gives `lui r; lw r,%lo(r)`.
- **Dead-store slot.** To put a dead temp store in a given slot, declare a union local in an inner block; the slot is reused by the next
  same-size temporary (func_001C3500).

## 2026-10-25 (NaGa's lanes, wave 3): what finished another 19 functions
Per-function notes are in `notes/190000-1F0000_lanes.md` (agents `w3lane1`..`w3lane10`).
- **Copy matched siblings first** (Lilly's ports and earlier matches): the pass-through base call with the full virtual signature,
  two-input `Dot`, `VU_VECTOR(const qword &)` helpers, pointer-to-member constants, `GAME *game = &D_003DD490` (func_001DF0A0,
  func_001C2EF0, func_00194F70).
- **reorg's backward delay-slot search stops at any asm.** Whether a compare is an asm helper or plain C decides what fills the
  branch slot (func_001999D0). Replace FPU one-liner asms (`min.s`, `abs.s`) with plain C (`b < a ? b : a`, `__builtin_fabsf`)
  before any scheduling work (func_001D2E18).
- **Normalize-with-fallback in return style** (`if (X(l) < 1e-4f) return fallback; return Div(v, l);`): an if/else assigning one
  variable lets jump.c hoist the fallback copy (the old vf1-pin cases).
- **cse1 adds REG_EQUAL to every `x = const`, and local-alloc doubles that variable's live length** (halving its priority). Compare the
  "across N insns" counts in `.flow` and `.lreg`; one extra use (e.g. an `i < count` guard) wins the tie back (func_001CE2E8).
- **Loop exit handling inside the test** (`j = n; for (;;) { if (--j < 0) { ...; break; } ... }`) keeps jump.c from copying the test in
  front of the loop and gives the target's `b test` entry; it replaced goto/`do{}while(0)` hacks (func_00194F70).
- **Reload drives many VU/GPR choices.** A `"j"` repeat of a spilled pseudo is an extra reload that adds a spill register and shifts
  reload's round-robin; `"jm"` removes it (func_001D4568). A value that lives across calls with few refs is best left a plain local so
  reload makes the target's spill stores (func_001DB0C8, func_001E5FC8).
- **Aliasing:** a scalar local's store never blocks struct-field loads through a pointer, so sched1 hoists them; if the target keeps the
  store first, the local is an aggregate. Reading a global as a field of its containing struct makes a load wait for a struct store.
- **Measured gcse PRE hash:** `hash(plus (reg r) (const_int c)) = (13258 + r + c) mod S`, S = (insns/2)|1 (func_001A8B50 notes).
- **Not accepted:** locals or empty objects that exist only to move a stack slot or a pseudo number (func_001AA1D8's `NOOP` objects,
  the earlier padded func_001A87F0).

## 2026-11 readability pass 2: what the compiler does not let go
Patterns from the second readability pass over each subsystem (every rewrite checked by matchtool, section bytes and the
object's nm set; in the player pass ~120 rewrites were kept and ~20 reverted).

**Free** (same object):
- Enum constants for magic numbers (Splice tags, parser states, buffer sizes, slot and bit constants, result codes, states,
  pad buttons); renaming locals/fields of TU-private views; switching a type name of a view used only under C linkage or by
  a global variable.
- Whitespace/layout (clang-format on a function range; there are no debug sections, but mind `#line` pins).
- `static inline` helpers that wrap one repeated statement (LO tagged-flag get/set/clear in relay.cpp, ZeroVector in
  UpdateHackship); a helper that does a loop's fallback stores through `p` itself (NewRelayJsg's SetBlockEnd).
- Field access through the real layout instead of `(char *)p + 0x2C0` or `*(T *)((char *)p + off)` with the same type.
- An early `return` for a trailing `if` without else (a void function whose whole body is `if (c) {...}` -> `if (!c) return;`);
  merging `if (A) { if (B) {...} }` into `&&`; `} else { if` -> `} else if`; dropping a bare `{ }` block that holds no
  declarations; `(f ^ 1) & 1` -> `!(f & 1)`; `x = x + 1` -> `x++`; `sizeof(T)` for a literal size.
- `goto next` -> `continue` when the label ends the loop body (also with a `k = kNext` increment moved into the `for` header).
- m2c's `i = 0; if (n > 0) { p = base; do { f(p); i++; p++; } while (i < n); }` -> `for (i = 0; i < n; i++) f(&base[i]);`
  (loop.c re-derives the pointer); cursor/countdown do-whiles -> `for (int i...)` with a block-local index
  (ChooseBhvSearchPoints); a backward `goto loop` -> `for (;;)` with breaks when the outer `if` and the bottom test are kept
  (UpdateBhvScript).
- `goto L` over a block -> `if/else` (MeasureJtJumpToTargetAtTime, leap init); a `goto` into the else branch of the same `if`
  (StartJtMode), or of an earlier `if` with the same test -> two plain ifs (GenerateStepguardLoot); a goto *back* to a loop head -> `for (;;)` (FindBspLeafContainingPointFast); a goto *into* a case -> plain
  cases (RefOpMult's int/float multiply).
- A goto into a shared tail **between if-branches** -> the tail written twice (cross-jumping merges it again:
  UpdateBentleySubState; AI `goto fill`/`goto take`/`goto spawn`).
- `(unsigned)x - 8u < 5u` -> `x >= 8 && x <= 12` when x is a plain field or local (via a local when it came from a macro read).
- A `goto skip` around one call becomes an `if` when the skipped condition is assigned in the condition
  (`if (!parent || (bit = (parent->bits >> 60) & 1) != 0)`, AddLo); the plain `(... & 1)` form changes allocation.
- Search loops written as `loop: p = t[i]; if (!p || p->id == id) return p; i = (i+1) % N; goto loop;` -> `for (;;)`
  (FindBitmapInHash), and `goto set` chains that all reach one call -> a single `&&` condition (SetTnTns gate).
- Declaring a helper as `(..., float, int)` instead of `(..., int, float)`: float and int arguments go to separate register
  files, so fixing a view's argument order is free.
- A `goto` into a shared "set flag" block (`if (p) { if (N(p) >= 100) goto setf; } if (cheat) { setf: f = 1; }`) ->
  `if ((p && N(p) >= 100) || cheat) f = 1;` (UpdateEpisodeMenu).
- Inlined SetLoFlags / ClearLoFlags bodies (the tagged LO flag word) moved into a `static inline` helper; minified
  one-line bodies re-laid out; extra brace scopes around a CRef temp; anonymous unions naming one offset two ways (a JOB's
  +0x7C start count vs a TASK's job pointer); enum constants in `case` labels.
- The `_GLOBAL_$I$<file><hash>` constructor stub's suffix follows the file text, so every edit changes it (aliased by
  wildcard; ignore it in nm comparisons).
- One-call `static inline` vtable helpers (SendSpliceEvent, SetStateVirtual for a `VCALL` macro); the tagged LO flag set
  as `SetTaggedFlags(unsigned *word, bits)`; `*(int *)&f = 0` as `f = 0.0f`; `(T *)((char *)p + i * 0x1C + 0x2AC)` as
  `&p->arr[i]`; `((float *)&q)[0]` as a union's `.x`; per-function command views replaced by one shared union (RELAYCMD in
  the relay builders; a two-argument `AddRelayCommand` view was just fAsync already sitting in a2).
- `do { ...; goto next; ... next: i = n; } while (i < 3)` -> `for (...; i = n) { ...; continue; }` (DrawFrameCaptureScreen).
- A nested `if (d == 0) { ... }` comparator chain as early returns (CompareSortRecs); `{int j; for (j = 0, p = a; j < n;
  j++, p++) f(p->k);}` as `for (int j = 0; j < n; j++) f(a[j].k);` (CprplPartitionArpl).
- `goto finish` from several early cases into a shared tail -> an `if / else if` chain ending in the tail
  (HandleBhvguardMessage; the nested `if (a) { if (b) {...} }` case folds into `else if (a && b)`); anonymous unions for
  per-class child fields (`union { int lookAt; int gotoSgf; };`); typed `&p->segments[i]` / `p->mover` / `p->heldNoise` for
  `(char *)p + 0x60 + i * 32` style arithmetic; a `template <class T> static inline` helper for the vtable-slot-8 start
  call (StartPushedBhv).
- Macros for flag bits, volume channels and virtual slots; `sizeof(field)` for memset sizes; splitting a `char sNN[]` view
  field into named fields; an array for three consecutive same-typed fields (sound.cpp).

**Not free** (reverted):
- Single-exit gotos and shared return labels (`goto done;`, `ret0:`/`ret1:`, `goto fail`, a single-exit result variable) ->
  early/separate `return`s: the return duplicates STACKSCOPE's cleanup (FInvertMatrix), a second copy of the return
  expression changes allocation/layout (func_0011F198, OSTRM::CbWrite, RefOpFindBindingOrNoop), the result lives in `$v0`
  instead of a saved register (`state = X; goto done;` in CalculateDetectorTargetState), or blocks reorder
  (IsStepguardTargetValid, DoesTargetRecordAllowGuard, ChooseStepguardTarget).
- A goto that leaves a nested loop to continue the outer one -> `break` + re-test (loop rotation changes): keep a labelled
  `goto nextX`.
- Gotos that share tails across switch cases or jump into a common cleanup label (MsgFromMcError, McJobCreateSave,
  load_file_from_disc, SetBentleyState, ChooseJtGroundState): `break` + a re-test of the result, per-case `return`s or a
  duplicated tail reorder blocks (40-90% differ). Case-tail sharing is a layout decision.
- A `goto out;`/`goto found;` to a label right after a loop -> `break` (FindPathzoneSegmentBlocker, BuildConvexHull's inner
  `while`, UpdateRipt1: 15% differ), `goto done;` to a shared tail -> wrapping the body in `if`, or an
  early `return` instead of `goto leaf` (FindPathzoneAtPos, moves the leaf block): keep such gotos, give the label a meaning,
  and say why. (A `goto test;` into a rotated `for (;;)` did become a plain `for`: RemoveObstacleEvents.)
- Dropping a redundant test: `if (r == 3) return; if (r != 4) return;` keeps the first test (PostCid14Load).
- Merging switch cases with identical bodies (9/12/13 in ComputeJtJumpGoal, four cases in UpdateHackship: one jump-table
  target instead of separate blocks); keep one call per case (a helper keeps it short). Duplicated selector branches
  (`switch (n) { case A: q = x; if (q) q->vt[8](); ... default: ... vt[9] }`) do not collapse into a computed slot either
  (SetDprizeDprizes).
- A loop's fallback store keeps its reload: `for (...) { ... goto done; } p->last = p->first; done:` as a helper that
  returns a pointer reuses the register instead of reloading `p->first` (NewRelayJsg).
- `if (a) goto L; if (b) goto L;` -> `if (a || b) goto L` (CanJtUseGadgetInAir).
- `again: ... goto again` search loops (FindProxyRefPath, BindLoOidTable's part search): `while (--j >= 0)` or a reset of the
  iterator moves the loop test.
- `goto test; loop: ... test: if (i < 8) goto loop;` with an early `goto done` (IgsAllocGsb) -> `for` + `return` (register
  choice changes); RenderBlipgSelf's `top:/next:` list walk as a `for`/`do-while` (frame size changes). Such a rotated loop
  is not a `while` either: it has no loop notes, so loop.c does not hoist (UpdateBhvEscort); hand-rotated loops with pinned
  registers stay as they are.
- A top-tested `for (;;) { if (!p) return; ...; p = p->next; }` list walk -> `while (p)` / `for (; p; )` (loop.c rotates those
  to a bottom test; DetachXfActById); a loop entered at its test via `goto test` -> a `for (;;)` with the test first (block
  order changes; ChooseSmaTransition).
- A `for` loop that carries m2c's hand-held pointer (ResolveStepguardAnimBindings); m2c's `if (n > 0) do {...} while (j < n);`
  as a `for` (GenerateStepguardLoot's coin split).
- A named `int` temporary for a flag or float compare (`int isClose = a < b; if (isClose)`, `int isInWorld =
  (int)(flags >> 60) & 1;` in its own block) inlined into the `if` (the branch structure changes).
- Branch order: ReleaseAnimBinding matches only as `if (kind != 2) { if (kind == 3) ... } else ...`, not as a switch or an
  `== 2 / == 3` chain.
- `*(i + array)` <-> `array[i]` (operand order of the address add): keep the index-first form where the match needs it, with a
  comment.
- A `do { ... goto end; } while (0)` tail -> `if/else` (SetMurrayLiftSubstate).
- `*(*pp)++ = x` -> `T **slot = (*pp)++; *slot = x;` (CollectBspSurfacesTouchingPolygon): the temp form fixes the evaluation
  order the target has.
- Inverting a float compare (`!(x > 0.1f)` or a float `if (a > b) goto L;` turned into an if-block: with -ffast-math it folds
  to `a <= b`, c.le/bc1f instead of c.lt/bc1t; UpdateBhvCircle) or rewriting a 64-bit flag bit test `(int)(f >> 60) & 1`;
  dropping the `(uchar)` cast on a 1-bit bitfield test (UpdateJt).
- Anything near register-pinned asm or register-pinned constants (Startup's colours, cane, SetMurrayCarrySubstate,
  SmoothJtCameraCurveTarget).
- `if (j < n) do {} while (++j < n)` as a `for` (UpdateVolbtn).
- Nested range tests `if (s < 3) if (s >= 0)` written `s < 3 && s >= 0` (becomes one unsigned compare).
- A goto out of a nested contact walk to the code after it -> a helper that returns the result (frame and registers change:
  UpdateButton, SendBarrierImpactMessages).
- `&verts[i]` for `(P4 *)((i << 4) + (int)verts)` in CalculateWayptCurtainDz (different schedule).
- Factoring a repeated block into a `static inline` helper, even once (UpdateEmitter's particle-count clamp: register
  allocation changes); `int f = 0 < d; if (f) c = d;` written `if (d > 0) c = d;` (loses slt/movn); folding
  `if (!a(p)) continue; if (b(p)) goto found;` into `if (a(p) && b(p)) ...` through an inline predicate.
- `if (a > 0) if (a < 3)` -> `if (a > 0 && a < 3)` (PlanBhvgoto); `(struct SEG *)(i * 32 + (int)p)` -> `&p->segments[i]` in
  SetBhvpathlinkState (address computed differently).
- A `static inline` helper can be emitted out of line (a non-template one in tu_171F58, the template with a cast in
  tu_1553F0: a new local symbol in `nm`): check the nm set, not only matchtool, after introducing a helper.
- A nested `if (x != 0) { if (x == 1) B; } else A;` as `if (x == 0) A; else if (x == 1) B;` (block order follows the
  source; BuildReconSelectionText); byte arithmetic `(H *)(i * sizeof(H) + (int)base)` as `&base[i]` (scheduling;
  RestartNestedSprite).
- A reference alias to a member (`S &stack = p->musicStack;`) changes PushMusic / FindMusic (spell the member each time);
  UpdateMusicMix keeps its `goto smooth` / `goto normalize` (if/else in either order moves the smoothing call before the
  jump: 36-76% differ), and its top-of-stack read stays `((REF **)&count)[count]` (`musicStack[count - 1]` differs).
- Not codegen but a compile error: a global enum constant passed as a call argument makes g++'s argument-dependent lookup
  see the file-scope declaration next to a per-function `extern "C"` redeclaration (ambiguous overload); use a macro for
  constants passed to such calls (sound.cpp SUKR_*).
