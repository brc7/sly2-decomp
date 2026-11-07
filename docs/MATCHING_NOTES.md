# Sly 2 matching notes (compiler facts and techniques)

What we learned writing C/C++ that compiles to **byte-identical** code for SCUS_973.16: the compiler and assembler
facts, the build annotations, and the source techniques that moved stuck functions, organised by topic. Merging files
into one source per TU, data layout and linkage are in [guides/tu-merging.md](guides/tu-merging.md); building is in
[guides/setup.md](guides/setup.md). The dated diary this was condensed from (per-round entries, agent lanes, pointers to
`notes/*.md` working files) is in git history (`git show 3493d772:docs/MATCHING_NOTES.md`; the `notes/` files at
`e0aacd4`). Example function names are the current ones.

**The loop**: `python3 tools/show.py NAME` prints the target disassembly and any draft; edit the function in its TU's file
(`tools/layout/units.csv` maps functions to units); `python3 tools/matchtool.py check <file> [NAME]` prints `MATCH` or a
side-by-side diff (`!!` marks differing lines; left = target). Relocated operands (call and global addresses) are masked.
The `aligned N` count lines up the two instruction streams first, so a draft one instruction off shows as 1, not 100+;
use it to judge closeness, but only an exact MATCH counts. For a big draft far off (aligned > 20), rewrite fresh from the
asm section by section rather than patching an old m2c draft.

## 1. Compiler and toolchain facts (game code)

- ee-gcc 2.95.3 (build 136), `-O2 -G0 -ffast-math -mvu0-use-vf0-vf31`. `.c` compiles as C, `.cpp` as C++ with
  `-fno-exceptions -fno-rtti` (the game's vtables have 0 in the typeinfo slot).
- The original is C++. g++ 2.95 mangling: `Name__ClassArgs`, e.g. `Foo__4CBarii` = `CBar::Foo(int,int)`, `__4CBar` ctor,
  `_$_4CBar` dtor, free function `Foo__FP4CBari` = `Foo(CBar*, int)`. Arg codes: i int, Ui uint, f float, c char,
  Uc uchar, s short, l long (64-bit on the EE: use `long`; `long long` is 128-bit), Ul ulong, v void, P pointer,
  R reference, C const, `Q2..` nested.
- `$ra` is saved with `sd`, callee-saved `$s` registers with `sq`. Struct returns use a hidden pointer in `$a0`.
  Constructors return `this`.
- vtables: 8-byte entries `{short delta; short pad; fn}`; a virtual call loads `lh 8N(vt)` and `lw 8N+4(vt)` for slot N
  (slot 0 unused). The vptr follows the data members of the class that first declares virtuals (+0xC for BASIC-derived
  objects).
- 128-bit by-value arguments/returns (a VECTOR in one GPR, `qword` params): `typedef int qword __attribute__((mode(TI)));`.
  `ulong` ops appear as `dsll32`/`dsra32`/`daddu`; `daddu $x, $y, $zero` is a move. Float constants come from
  `lui`+`mtc1`; `lwc1 f, %gp_rel` / absolute addresses are globals.
- In the EE EABI float arguments go in `$f12..` and integer/pointer arguments in `$a0..`, counted separately, so
  `f(int *p, float g)` and `f(float g, int *p)` compile to the same callee. The caller shows the order (gcc evaluates
  arguments in prototype order): when a caller matches only with a different float/int order, that order is the real one
  (`GSmooth(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext)`, Sly 1's prototype, fixed its callers).
- Jump tables live in .rodata: read them from disc/SCUS_973.16 at file offset 0x80 + (addr - 0x100000).
- **gcse orders some expressions by a hash of the symbol name**, so a global named `D_002DEA4C` can schedule differently
  from the same global under its real name. Extern names never reach the binary, so giving a global its real
  (descriptive, evidence-based) name is legitimate; never pick a name only to hit a hash.

**What the build reproduces (don't fight it).** Game code is assembled by SN's `ps2eeas` (through `tools/wibo_cc.py`), and
`tools/asfix.py` emulates the rest of the original assembler:
- FPU hazard nops (mtc1 -> use, c.cond -> bc1x; a short loop ending `c.cond.s; bc1x` is one instruction longer because of
  that nop), 64-bit constant expansion (`addiu -1; dsll32`).
- Short backward loops padded to 6 instructions (the R5900 erratum fix), including `L: b L`, loops starting with a `jal`
  or any branch, and loops with an inner forward conditional branch whose delay slot is certainly a nop.
- Two nops before `div.s`/`sqrt.s`/`rsqrt.s`/`vdiv`/`vsqrt`/`vrsqrt` so they are at least the 3rd instruction of their basic
  block (2 nops at block position 0, 1 at position 1; calls don't start blocks). Write plain C division / sqrt, never
  `__asm__("nop")`.
- `_start` (crt0) is hand-written asm in the original, kept as a top-level `__asm__` block; kernel syscall stubs
  (`addiu v1,$zero,N; syscall; jr ra; nop`) are asm blocks too.

## 2. Sony SDK code: compilers and build annotations

Sony built its libraries (0x234F48-0x2550A0) with other compilers. Per-file header comments select the build:

| Annotation | Effect |
|---|---|
| `// SLY2_CC: tools/compilers/...` | compiler. **Start with ee-gcc 2.9-991111** (`tools/compilers/extra/`): it emits `mult1`, frame-teardown `j` tail calls and `sd` register saves natively and is at least one of Sony's compilers. libm float functions use the **-01** variant (ee-gcc2.9-991111-01). Some files keep `bundle/ee-gcc2.95.3-114` + `SLY2_SAVES: sd` because their source was tuned for it. ee-gcc 2.96 is not Sony's (keeps a frame around tail calls, mis-rounds float constants); 3.2 also exists but lays code out differently |
| `// SLY2_CFLAGS: -O2 -G0` | flags; try adding `-fstrict-aliasing` (Sony had it on for some files) |
| `// SLY2_ASM: gnu-reorder` | gcc without delay-slot filling + GNU as in reorder mode filling every slot except backward-branch slots (`tools/gnu_reorder.py`). libkernel |
| `// SLY2_ASM: gnu-reorder-db` | gcc reorg fills slots (incl. from branch targets, beql), GNU as fills the rest. libmpeg/libipu, libm, libgraph/libdma, libgcc fp-bit |
| `// SLY2_ASM: gnu-reorg` | as gnu-reorder-db, but slots of short (<= 5 insn) single-block loops are emptied. libgcc dp-bit |
| `// SLY2_SAVES: sd` | with `SLY2_ASM` only: rewrite `$sp`-based `sq`/`lq` to `sd`/`ld` (Sony's 2.95.3-114-era compiler saved 64 bits in 16-byte slots) |
| `// SLY2_DIVNOP` | `nop; nop; div.s` units everywhere and no division in a delay slot (Sony libm), only when not using the -01 compiler |
| `// SLY2_NOLOOPFIX` | emit backward branches of plain single-instruction loop bodies as raw `.word`s in noreorder, so GNU as does not pad loops Sony's assembler left unpadded (libmpeg `_motionComp0`) |

gnu_reorder.py also reproduces Sony's assembler hazards: a nop between `c.cond.s` and a bc1t/bc1f even across labels or in
noreorder blocks, a nop after `mfc1 $r` when a scheduled branch reads `$r`, and no delay-slot filling from the tail of a
multi-instruction constant (`li.d`, `dli`) or the instruction right after it into a following `jal`. Never add your own
delay-slot or loop-padding nops in these modes. Unsolved: `mult`/`mult1` constant strides in scePad.

Sony-source habits: a small constant kept in a callee-saved register for the whole function was a local in the source
(`int one = 1;`); libcdvd uses a `static inline nopdelay()` helper and `while (1) { ... continue; ... break; }` retry loops;
an unexplained alignment nop may be the `.p2align` of a loop that never runs (fp-bit's denormal loop): write it out.

## 3. Dead-code scraps (`config/junk.csv`)

The original link left small scraps between functions: runs of 8-byte units, each one orphaned instruction (the kind that
fills a final `jr $ra` delay slot: `addiu $sp,$sp,N`, `move $2,$4`, `nop`...) plus its alignment nop, the remains of
functions that are not in the ELF. They are listed in `config/junk.csv` with a reason and are not functions (progress.py
skips them). Inside a TU's range the source emits them in place so the object's `.text` links as one run:
`#include "sly2/scrap.h"` and `SLY2_SCRAP("addiu $sp, $sp, 0x30");` per unit, after the previous function and before any
data definition. A unit can be a plain `nop`: 8 zero bytes before a function are such a unit, not 16-byte alignment.
Those junk.csv rows end with "emitted by its TU source (SLY2_SCRAP)". If you find another scrap, add its range to junk.csv
rather than "matching" it. Evidence: [full-link.md, "Junk scraps and padding"](guides/full-link.md#junk-scraps-and-padding).

### Junk list correction

Two `nop`s directly before a `div.s` at a function's start are the divide-hazard padding (section 1), not scrap: 0x117120
was once listed wrongly and now matches as a function.

## 4. Techniques

**Prototypes and calls**
- Declare every callee with a prototype and its real return type, even when the result is ignored: constructors, copy
  constructors and `CRef::operator=` return `this`, memset/memmove `void *`. Declaring them `void` changes scheduling and
  argument order across the whole caller. The reverse also happens: a callee whose result the target never reads may
  match only when declared `void` (its `$v0` adds a scheduler dependence). Try both.
- Declare memcpy/memset under their address names (`func_0023320C`, `func_002333B4`): a prototype named memcpy/memset
  becomes a builtin.
- Parameter order with floats changes integer register assignment; try moving float parameters.
- Memmove-style setup (`a0=s0; a1=s0; a0+=16`): call through a `static inline` forwarder taking two `void *`.
- A function passing a returned 3x3 matrix straight into another call: a `static inline` helper taking the M3 by value,
  called as `Helper(GetMat(...), ...)`, removes the extra stack copy. A g++ named-return-value helper
  (`M3 MulM3(...) return r { ... }`) plus a dead per-row copy reproduced another matrix function's frame.
- Copy matched siblings first: Lilly's ports (`MATRIX3 operator*`), the pass-through base call with the full virtual
  signature, `VU_VECTOR(const qword &)` helpers, pointer-to-member constants.

**Store and schedule order**
- gcc keeps source order of stores but sometimes rotates the last store to the front: for target order A,B,C try B,C,A.
  Chained assignment (`a = b = c = 0`) gives a different order and keeps 0.0f in an FPU register.
- Building a value in a temporary (`tag = ...; tag |= ...; p->x = tag;`) vs an inline expression changes scheduling.
- sched1/sched2 break ties toward the instruction with more dependents later on (`h = t * 0.5f` instead of overwriting `t`
  flipped one). A register set exactly once in the whole function gets the "birthing insn" boost.
- A `volatile` asm is a full scheduler barrier (drop `volatile` when only data dependences matter), and even an empty one
  costs a scheduler slot: if the order around it differs, replace a reload barrier with a plain local of the right type.
  `"memory"` clobbers fix the order of loads/stores around asm that writes scratch memory.
- A `do { } while (0)` emits NOTE_INSN_LOOP_BEG, which the scheduler treats as a barrier and which can leave a stray nop:
  define statement macros as plain `{ }` blocks unless the barrier is what you need.
- reorg never puts an inline-asm instruction in a delay slot, and its backward slot search stops at any asm: write
  `qmfc2`/`qmtc2`/`mfc1` moves and FPU one-liners (`min.s`, `abs.s` -> `b < a ? b : a`, `__builtin_fabsf`) as plain C
  when the target schedules them into slots.
- Aliasing: a scalar local's store never blocks struct-field loads through a pointer; if the target keeps the store
  first, the local is an aggregate. Reading a global as a field of its containing struct (`&D_003DD490.trie` instead of
  the field's own symbol) changes the hi/lo split and makes loads wait for struct stores.
- `k++, i++` in one for-increment can schedule differently from two statements.

**Register allocation**
- Change declaration order of locals, merge/split temporaries, use a block-scoped temp (declared inside the `if`/loop that
  uses it), reuse a variable vs a new one, `x = a; x |= b;` vs `x = a | b;`, swap commutative operands or comparisons,
  `if (!c) {B} else {A}` vs `if (c) {A} else {B}`, separate loop counters (`i`, then `j`).
- Register swaps are global-alloc priority ties: `-da` writes `.greg` ("N regs to allocate" is the priority order) and
  `.lreg`. Priority = floor_log2(refs) * refs / live_length, refs weighted by loop depth. cse1 adds REG_EQUAL to every
  `x = const` and local-alloc then doubles that variable's live length; one extra use wins the tie back. Splitting a
  variable assigned in two blocks into two variables changes its priority.
- Taking a global's address into a local (`G *pg = &g;`) when the target keeps it in an `$s` register; share pointers across
  switch cases (declared once at function scope) when the target reuses a "stale" pointer from an earlier case.
- When the target recomputes `&local` at each use but ours keeps it in a register, pass the address through a small inline
  helper (`static inline T *Addr(T *p) { return p; }`) to stop gcse sharing it.
- Reload hands out spill registers round-robin. A value living across calls with few refs is best a plain local so reload
  makes the target's spill stores.
- Old register pins (`register int x asm("$16")`) are often the problem: remove them **one at a time** and keep each removal
  only if the aligned count drops (removing all at once made 15 of 16 drafts worse).

**Loops**
- `for` vs `while` vs `do-while`; `i++` vs `++i`; pointer walking vs indexing; `x != 0` vs `x`; non-rotated loops
  (`for (i = 0; ; i++) { if (i >= n) break; ... }`); `while (f(p), cond)` for a call inside a loop test; loop exit handling
  inside the test (`j = n; for (;;) { if (--j < 0) { ...; break; } ... }`) gives the target's `b test` entry.
- A target `nop` just before an 8-byte-aligned loop head is gcc's loop alignment (`.p2align 3`), emitted only for real
  `for`/`while`/`do` loops: rewrite goto-built loops as real loops.
- loop.c hoists a constant only when life * 95 * savings >= the loop's insn count (`-da -dL` prints "Loop from..: N real
  insns"; two loop passes), and behaves differently once a body passes ~95 RTL insns. Const-ref helpers
  (`const VU_VECTOR &`) and by-value class helpers add RTL insns without changing the final code, which can push a loop over
  that line. Reading a global into a local before the loop stops `%hi(D_...)` being hoisted. A loop with more than one
  entry point (test the first element, then `goto` into a `for (;;)` at its increment) is skipped by loop.c entirely.
- An `unsigned` counter stops gcc reversing a loop. gcc expands a nested inline-call argument first and loop.c emits giv
  set-ups in reverse discovery order, so moving a nested call into a helper reorders loop-pointer set-ups.
- `beql`/`bnel` come from optimised loops/ifs; you don't write them directly.

**Control flow**
- `(a == b) & (c == d)` gives the movn/sltiu forms; `switch` for set-membership tests; an empty `case 0:` can reproduce a
  switch tree.
- When the target branches to one shared `return 0` but ours computes a setcc or repeats the return, write the early exits
  as `goto fail;` to a shared `fail: return 0;`. A target ending `li v0,K; jr ra; nop` with an unfilled slot: a
  conditional `goto done;` to a shared `result = K; done: return result;`. Separate `return` blocks let cross-jumping
  rebuild tails.
- Normalize-with-fallback in return style (`if (X(l) < 1e-4f) return fallback; return Div(v, l);`) rather than an if/else
  assigning one variable.

**Stack frames and locals**
- An unused local struct or `int unused[4]` can explain a frame size (ra at sp+0x10). A stack area modelled as a local
  struct may really be spilled parameters whose addresses are taken: use the parameters. A slot after the real locals that
  is reloaded around calls is a spilled pseudo: write a plain local.
- `qword` locals get a stack slot when their address is first taken; wrap them in a union/struct to place them at
  declaration. Write asm output to a plain `qword` local, then copy it into a union (direct union outputs store twice).
- A dead temp store in a given slot: a union local in an inner block (the slot is reused by the next same-size temporary).
- A target that constructs a stack object and ends with `dtor(this, 2)`: declare the object at function scope and write the
  ctor and dtor calls explicitly.
- Some `D_` "globals" are a function's local aggregate initializer (`struct LM lm = {0.1f, 1.0f};`, placed in .rodata).
- `extern const float` lets a global float load be hoisted from a loop.
- EE varargs: `__builtin_next_arg` / `__builtin_args_info(2)` for va_start, `*(T*)((ap += 8) - 8)` for va_arg.

**VU0 and inline asm**
- One inline-asm statement per VU op (vsub / dot / sqrt / qmfc2) lets gcc allocate VF registers like the original. An inline
  `SqrtF` helper (`sqrt.s`) is accepted (`__builtin_sqrtf` does not give the game's code); other FPU one-liners C can
  express should be plain C.
- A float out of a VU qword with a single `mtc1`: one asm taking the qword as input and emitting `mtc1`, not `qmfc2` into an
  int plus a cast.
- Constraints: `"=&j"` (earlyclobber) forces a register different from every input, so drop the `&` if the target reuses an
  input's register; `"=j"` instead of `"+j"` fixes vf numbering; `"rV"` gives `lui r; lw r,%lo(r)`; a never-chosen `"V"`
  alternative on a qmtc2 input keeps the word in memory until reload; `"=r,m" : "j,!r"` on a nearby helper's output
  advances the GPR reload round-robin; a commented clobber of a dead VF register skips it in reload's round-robin; a
  clobber of a dead argument register (`"$4"`) delays a later setup of that register.

**Reading the compiler**
- When stuck, compile with gcc's dump flags (`-da` for every RTL pass, or `-fsched-verbose-5`; in a scratch directory, never
  through matchtool, so dump files stay out of the repo) and find the **first** pass where ours differs: `.loop` (hoisting), `.gcse` (pseudo creation order: its table
  size (n_insns / 2) | 1 after cse1 decides PRE numbering, and `hash(plus (reg r) (const_int c)) = (13258 + r + c) mod S`),
  `.greg`/`.lreg` (allocation), sched1/sched2 (ties and dependences). Fix the C that feeds that pass.
- decomp-permuter (the wrapper `tools/permute.sh` is in git history at `e0aacd4`) works on plain-C drafts without inline
  asm; always verify a candidate with matchtool and clean it into readable C.

## 5. What is accepted in matched source

- **Empty asm barriers** (`__asm__ __volatile__("")`) when they stand in for a known difference between our compiler builds
  and the original ones (e.g. Sony's scheduler keeping a return value's use right after the jal), with a one-line comment
  saying what the barrier emulates. Prefer natural C when it exists.
- **Allocation-only asm operands**: in a function that already uses inline VU asm, an extra unused operand on an existing asm
  statement (listing `"j"(dv)` twice), or a constraint alternative that is never chosen (`"r,j"`), only steers allocation.
  Comment it; don't add new asm statements just for this.
- **Not accepted**: locals or empty objects that exist only to move a stack slot or a pseudo number, and probe-style code
  (50 nested identity calls). Write the source naturally.
- Unsolved patterns (skip unless you have a new idea): factory functions where the target keeps the vtable address in `$v1`;
  returning a float VECTOR in `$v0` with pextuw/prot3w.
