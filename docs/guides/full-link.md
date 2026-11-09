# Full PS2 link

What it takes to link the matched sources back into SCUS_973.16, and how far the repo is. `tools/link.py` does a real
link (one section per object, placed at the ELF's addresses) and compares the result with the ELF byte for byte;
"Status" has the current numbers, "End state" what is left before the link reproduces the ELF the way the original
build did. The symbol side is in
[tu-merging.md, "Linkage of namespace-scoped declarations"](tu-merging.md#linkage-of-namespace-scoped-declarations).
The per-branch measurements that led here (prototype link, code order, linkonce families, data order) are in this
file's git history (last full version at 3493d772).

## Status (data/commons-file, 2026-11-06)

The one current table (earlier branches' tables are in this file's history). data/commons-file (from f765bdd8) defines the game's commons in one data-only
source, src/game/app/commons.cpp, in the original's allocation order (user decision; "Absolute symbols" below and
[tu-merging.md, "Phase 9: the commons file"](tu-merging.md#phase-9-the-commons-file)):
absolute symbols of class common 69 -> 15 (the SDK sources' buffers), 274 -> 220 in all. Every value from freshly
built objects (build/link and build/progress_cache.json deleted first); data/address-names-2's in brackets where
different.

| measure | value | tool |
|---|---|---|
| functions matching | 6,662 / 6,662 (100%; 857 sources (commons.cpp has no functions), 329 SDK sources carried over, verified on the Mac); config/matched.csv content unchanged by the run (the first run after deleting the cache lost 4 sources to compile timeouts on the shared machine; the rerun built them) | `JOBS=3 tools/progress.py`, cache deleted, run alone |
| all game sources (211 with commons.cpp) | functions 6,017 / 6,017, data symbols 4,725 / 4,725 MATCH (now including the COMMON copies of the 22 pool/heap/sentry names config gives addresses: 2,538 in 211 objects) | `tools/matchtool.py check` (here: its comparison on the link objects) |
| vtables from objects | 284 `.gnu.linkonce.d._vt$*` sections, each MATCH against the ELF and emitted in ascending ELF order per object; 2 left in the linkonce data area (EMITTER's/EXPLO's template-instance tables 0x2CE680, 0x2CED18: tu-merging.md) | `tools/matchtool.py check`, llvm-readelf |
| absolute symbols | 220 [274]: SDK functions the SDK objects define (Mac) 109, SDK data and SDK-only references 53, SDK name mismatches 8, commons 15 [69] (the SDK sources' 989snd/libkernel buffers), VU-memory/scratchpad offsets 26, game objects not defined 9; plus 17 labels on the `.vutext`/`.irx` blobs | `tools/link.py` (build/link/absolute.txt) |
| undefined references not in config (209 merged objects and commons.cpp), duplicate strong definitions | 4: the four `_gp` (plus commons.cpp's `gcc2_compiled.` and `__gnu_compiled_cplusplus`: local markers g++ leaves without a section in a file with no `.text`, not references); 0 | scratch symcheck.py |
| relocations checked against the ELF | 55,457 [52,803] OK, 0 mismatched (more checked: config names the pools, heaps and sentries now) | scratch relverify.py |
| link objects | 528 [527]: 528 whole, 0 in per-function fallback; the new one is commons.cpp (link.py `DATA_SOURCES`) | `tools/link.py` |
| ld.lld | 0 errors; 1,287 [1,286] object sections placed (commons.cpp's `.sbss` and `.bss`; dl has no `.bss` now), 2 unplaced (140 bytes: an SDK object's `.data` 100, bq's linkonce copy 40), 2 `.bss` overlapping another bound to their address (tn, cplcy) | `tools/link.py` |
| `.bss`/`.sbss` gaps no object defines | 60,264 [60,772] bytes in 4 [6] address assignments (`.sbss` is all commons.cpp's; the SDK tail ends at 0x452098) | `tools/link.py` |
| differing bytes | 0; .text, .rodata, .data, .vutext, .irx equal; from objects .text 1,295,844, .rodata 128,544, .data 180,852 | `tools/link.py` |
| blob bytes | 822,888: IOP modules 474,176; data no object defines 193,760; text of SDK sources that do not build here 118,224; VU microcode 34,112; junk.csv 1,576; padding / not in config 1,036; `func_00100000` 4 | `tools/link.py` |
| config names at their config address | 8,120 / 8,120 [8,066]: config names the 6 slot pools, 5 splice heaps and their 11 sentries (symbol_addrs.txt); 23 [45] undefined names no config file knows | `tools/link.py` |
| docs links | 0 broken | `tools/linkcheck.py` |

## Absolute symbols (data/address-names-2, data/commons-file)

tools/link.py lists every name it binds to an absolute address in build/link/absolute.txt (address, ELF section,
class, the referencing objects) and prints `absolute symbols: N` by class. A name no object defines is bound to its
address (address names by the address in the name, config names by config, the rest by the ELF's relocated value),
commons to the ELF's address; a name that points into the VU microcode or the IOP modules is defined by blobs.o as a
symbol on its blob section instead (those two stay binary for good, "What stays a blob" below), so it moves with
the blob. What changed, by class (start 492, integrate/round10):

| class | before | after | how |
|---|---:|---:|---|
| reference to a name whose address an object defines under another name | 70 + 30 | 0 | asm labels to the defined symbol (58 vtable names, `__builtin_new`, `_$_10CStreamBis`, `g_pfrmOpen`, `CopyAb__FPvT0Ui`, `__pure_virtual`, ...), the defining object renamed where config names it so (`__4CRefRC4CRef`, `AllocFromStack`, printf/sprintf, the GC pool words' template statics) |
| interior of a defined object | 11 | 0 | gs's double buffer as four objects; elsewhere an asm label with an addend (`__asm__("D_002DDF40+0x60")`): a separate symbol to g++, the owner to the assembler (tu-merging.md, Phase 8) |
| vtables no object emitted (address-named references) | 45 | 0 | emitted by their key TUs (57 with the unreferenced ones) |
| other game objects not defined | 33 | 9 | bsp's sentinel planes/leaves, util's limits, mat's/vec's matrix and vector constants, hud's M23, grabspot's flag, emitter's empty reference blocks |
| commons | 84 | 69, then 15 (data/commons-file) | the address-named duplicates of template statics merged into them; then the 54 game commons defined by one file, src/game/app/commons.cpp (below); the 15 SDK ones stay |
| `.vutext`/`.irx` labels | 17 | 0 (17 blob labels) | link.py defines them on the blobs |
| VU-memory / scratchpad offsets (0x0..0x3AB0) | 26 | 26 | not image addresses: absolute is right |
| SDK | 179 | 170 | `_ctype_+1`, `_impure_ptr`, `get_eh_context` by name; five syscall/libgraph sources and sceSifLoadModuleBuffer define the names their callers use |

Left, and who decides:

- **Mac (SDK objects)**: 109 SDK function names resolve when `--sdk-only` objects link; 53 SDK data names and
  SDK-only references come with the SDK objects (989snd's words and strings, libkernel/libgraph tables, newlib's
  reent data, `_ctype_`). 8 SDK names do not match the defining source's name and stay absolute even then:
  strtol (func_00234580.c), RemoveIntcHandler, sceSifGetReg, Deci2Call, GetMemorySize, `_InitTLB`, DisableIntc (the
  syscall stubs func_002347xx..002351E0) and sceSifSetReg (0x23A920, which config calls sceSifExecNotifyHandler):
  renames of SDK sources the naming pass deferred to the Mac. Also to verify
  on the Mac: func_00237B80.c and _sceMpegCscStoreRefImage.c (gnu-reorder) now call SignalSema / FlushCache by name.
- **User decisions**: palette16's built-in font (D_0027A778, D_0029A790, font.cpp's D_0029A7A8/D_002A28E8) and
  palette256's bitmap block (D_0029A8C0, D_002A28D0): game artwork, how image assets are kept in source.
- **The commons (data/commons-file)**: the user chose one source file over a link.py ordering rule.
  [src/game/app/commons.cpp](../../src/game/app/commons.cpp) defines the 54 game commons (the Splice GC kinds'
  list words and block tables, CSlotPool/CSpliceHeap's template statics and their sentries, prog's and hud's words)
  and dl's two `.sbss` words as ordinary objects, in address order: one object section for all of `.sbss`
  (0x2DD280..0x2DD31C) and one for the end of `.bss` (0x452098..0x452200). g++ 2.95 emits class and template
  statics as COMMON and GNU ld allocated commons after every object's `.bss` (8 bytes or less in `.sbss`) in its
  hash-table order; the strong definitions take the commons' place, so link.py places that file like any object and
  binds a common to an address only when no object defines it. How the definitions are spelled, and why the pools
  are storage under the mangled name rather than C++ member definitions: [tu-merging.md, "Phase 9: the commons
  file"](tu-merging.md#phase-9-the-commons-file). Left as commons: the 15 of the
  SDK sources (989snd's buffers 0x443800..0x448500, libkernel's D_0044EF80), which are not commons of the original
  link but SDK library `.bss` between other SDK objects' data (`_sif_*` 0x44EDD4.., libgcc's `objects` 0x450278):
  they belong to the SDK sources (Mac), as `.bss` of one object per library block, not to a game file.
- **Owner not known (game)**: D_002BBB50 (one of three vectors 0x2BBB30..0x2BBB60 between bsp's leaves and util's
  limits), class_registry's D_002DE720/D_002DE750 (`.bss` after emitter; likely guarded local statics of emitter.h's
  inline accessors).

## The target: the ELF's layout

Entry `_start` at 0x100008, `_gp` = 0x2E5270 (from `.reginfo`), two PT_LOAD segments:

| section | address | size | content | from where in a link |
|---|---|---:|---|---|
| .text | 0x100000 | 0x159DF0 | EE code | objects; 98.8% of the bytes are matched functions (functions.csv ranges of the matched.csv rows); the rest are 153 dead-code ranges in config/junk.csv (1,924 bytes), `func_00100000` (4 bytes before `_start`) and padding |
| .vutext | 0x259DF0 | 0x8540 | VU microcode, with 21 `.DVP.overlay..*` sections and `.DVP.ovlytab` describing the VU-memory overlays | not source: a blob from the ELF (or the original DVP assembler sources, which we do not have) |
| .rodata | 0x262380 | 0x4A4E4 | constants, strings, jump tables, vtables | objects (named data and anonymous literals) + unbuilt data |
| .vudata | 0x2AC864 | 0 | | |
| .data | 0x2AC880 | 0x309B0 | initialized data; starts with the crt0-block option records g_aeopid (0x2AC880-0x2BAAE8, src/game/app/class_registry_options.inc) | objects + unbuilt data |
| .sbss | 0x2DD280 | 0x9C | small (gp-relative) bss | objects |
| .bss | 0x2DD380 | 0x174E80 | | objects + unbuilt data |
| .vubss | 0x452200 | 0 | | |
| .stack | 0x452200 | 0x20000 | PROGBITS, not in a segment | linker script only |
| .irx | 0x472200 | 0x73C40 | the IOP modules, second segment | blob from the ELF |

Segment 0 covers .text through .bss (file size 0x1DD230, memory size 0x352200); segment 1 is .irx.

## Inputs

| input | sources | functions | builds here | note |
|---|---:|---:|---|---|
| game TUs (src/game) | 210 + commons.cpp | 6,017 | yes | one object per original TU; class_registry.cpp is the whole crt0_splicewrap TU |
| generated Splice wrappers (src/generated/splicewrap) | 373 | 373 | yes, inside class_registry.cpp | `.inc` fragments of the crt0_splicewrap TU, included at their addresses |
| SDK (src/sdk: libc, libgcc, libkernel, libcdvd, libgraph, libmc, libmpeg, 989snd, ...) | 646 | 646 | partly | 329 sources (SDK files that need GNU as for `SLY2_ASM: gnu-reorder` or the native ee-gcc 2.96, and asm files) do not build in the Linux workspace and are verified on the Mac |
| crt0 | 1 | 1 (`_start`) | yes | src/game/platform/boot/crt0/; `func_00100000` before it is unmatched |
| dead code | - | - | - | config/junk.csv ranges: emitted by their TU sources (`SLY2_SCRAP`, "Junk scraps and padding") or kept as blobs |
| VU microcode, IRX modules | - | - | - | blobs |
| data no object defines | - | - | - | bound to absolute addresses ("Absolute symbols") |

## How the link works

`python3 tools/link.py [--no-compile] [--per-function]` or `--sdk-only`; tools/verify.sh runs it without options. Needs
`ld.lld` and `llvm-objcopy` (LLVM 18; no MIPS GNU binutils needed) and your disc/SCUS_973.16. The script finds the tools
via `$LLVM_BIN`, then PATH, then Homebrew's keg-only `llvm`/`lld` (macOS: `brew install llvm`; if Homebrew's llvm aborts
with `Library not loaded: .../libz3...dylib`, its build is stale against a newer z3: `brew upgrade llvm`). Everything it
writes goes to build/link/ (ignored): objects, blobs.o (byte ranges cut from your ELF), the linker script sly2.ld,
sly2.elf, ld.log and the diagnostics sections.txt, tutext.txt, wrongaddr.txt, mismatch.txt, absolute.txt. A full run is about 3 to 5 min here (528 sources, 2 cores, plus the fallback objects); `--no-compile` about 5 s.
A source that fails to compile loses its object (a stale one is deleted, never linked), the link still runs (its
bytes come from the ELF as blobs) and prints its summary, and link.py exits 1 listing the failing sources, so
verify.sh's link step fails (link/linkonce-rest; before, the previous object was kept and the link passed).

1. **Objects as the original build made them.** Every buildable source of config/matched.csv through matchtool's
   `compile_src`, without `-ffunction-sections` (build/link/obj-tu/): one `.text`, `.data`, `.rodata`, `.bss` per
   object (plus the named sections a few sources ask for: `.rodata.early`, `.data.buildinfo`) and a
   `.gnu.linkonce.*` section per vague-linkage instance.
2. **Which objects link whole.** For each object, every function of its `.text` with a config address must sit at
   that address relative to one base, which is what a contiguous TU in the ELF means. The others are classified
   by reason (per object in build/link/tutext.txt; none today):
   linkonce copies (a function whose address is in the linkonce area 0x254F00..0x259DF0), split by other objects'
   code, junk.csv scraps inside, gap/padding, functions in another order, code the ELF does not have. Those objects
   fall back to their `-ffunction-sections` build (build/link/obj/, compiled only for them), each function placed
   on its own; their data sections are the same in both builds. `--per-function` builds every object that way (the
   old mode, kept for comparison).
3. **Placement and script.** As before, each section's address comes from the ELF (symbol votes, then
   relocations). The script lists the sections of each output section in address order, which for the whole
   objects is TU order (tools/layout/units.csv's order of home TUs: the address order is the same list); the first
   placed copy of each linkonce section is linked and every other object's copy bound to it. A gap becomes a blob
   only if the linker would not fill it with the same bytes: alignment padding that is zero in the ELF is left to
   the linker (output sections are filled with `=0`; ld.lld's MIPS default fill is not zero), and a section's own
   padding is cut only where the ELF starts the next section. Address assignments remain only for gaps in
   `.bss`/`.sbss` (20), so the count measures how much of the layout is still forced.
4. **Compare** as before; build/link/sections.txt lists every section not simply linked at its address (disagreeing
   evidence, unplaced, bound, overlapping).

The 329 SDK sources that need i386 Linux binaries (279 compiled by a native Linux ee-gcc: 2.9-991111 and its -01
variant, 2.96 for src/sdk/libmpeg/func_0024B8F8.c; 49 compiled through wibo and assembled by GNU as, tools/gnu_reorder.py)
are linked as objects when build/link/obj-tu/ has one, which `--sdk-only` builds (only those sources; a failed
compile keeps the previous object); otherwise blobs cover them. Here every one fails (no `/lib/ld-linux.so.2`); see
"End state", SDK objects.

Object fixes before linking (in build/link/fix/): llvm-objcopy rewrites each object (gcc 2.95's as puts local symbols
after globals, which ld.lld rejects); sections holding one symbol are cut to the symbol's size (gas pads each section to
its alignment). `--gc-sections` with every placed section `KEEP`: the key-TU extras (unreferenced vtables, typeinfo)
drop out. The compare step reports, per differing byte, the owner (object section or blob) and the reason (from the
relocation at the differing word), and lists config names at the wrong address (this is what checks `.sbss`/`.bss`).

### Linkonce copies

127 functions the original emitted with vague linkage (`.gnu.linkonce.t.*`, in the linkonce area 0x254F00..0x259DF0)
were once plain functions homed in a TU that uses them, which split those objects' `.text`. All are template or inline
instances again, each in its own `.gnu.linkonce.t.<symbol>` section: the Splice GC pools (`CGcObj<T>` in sly2/gc.h,
`CRefPtr<T>` in sly2/basic.h), the EMITTER/EXPLO set (function templates in sly2/emitter.h), accessors whose address the
option table takes (emitted by class_registry.cpp), the containers (sly2/arrayv.h, instantiated by dynarraystr.cpp), and a
few plain definitions with `weak` + `section(".gnu.linkonce.t.<symbol>")` (class_registry's three constructors, bif's
`operator*`, dimitri's VecCombo). Why each form: tu-merging.md, "Inline functions and linkonce copies" and "Linkonce
copies by family". The first placed copy of each linkonce section is linked and every other object's copy bound to it.

### Junk scraps and padding

Objects whose range holds config/junk.csv dead-code scraps or odd padding could not link as one run until their
sources emitted those bytes (link/junk-padding). `tools/link.py`'s check, before -> after that branch:

| verdict | before | after |
|---|---:|---:|
| `.text` links as one run at its ELF address | 855 | 893 |
| split by other objects' code (linkonce copies, crt0 block: link/linkonce-*, link/crt0-block) | 38 | 38 |
| dead code (config/junk.csv scraps) inside the object's range | 33 | 0 |
| gap/padding differs | 4 | 0 |
| unplaced section (grab.cpp) | 1 | 0 |

Each of the 37 objects, compiled without `-ffunction-sections`, has a `.text` equal to the ELF's at its address
(relocated words aside; scratch check). Differing bytes stay 115; `.text` from objects 1,282,044 -> 1,282,864 bytes,
blob bytes for junk.csv 2,136 -> 1,708 and for padding 14,276 -> 13,884 (the five new junk.csv rows moved 40 bytes
from the second label to the first; four of those nop units sit at the end of a function's section, which link.py cuts
to the function's symbol when the rest is zeros, so their 32 bytes still come from blobs in this measure); unplaced
sections 6 -> 4 (544 -> 416 bytes).

**What the scraps are.** Every junk.csv range is a run of 8-byte units, each one instruction followed by a nop to
the next 8-byte boundary (164 of 168 ranges start 8-aligned; the other four start at the alignment nop before the
first unit). The instruction is the kind that fills the delay slot of a function's final `jr $ra`: `addiu $sp,$sp,N`
(267 of the 338 units), `move $2,$4`, `sw $5,N($4)`, `slt $2,$0,$2`, `nop`... Their frame
sizes do not match the neighbouring functions (dynarray's 0x116D20 says 0x30 between a 0x10 and a 0x20 frame), and
runs of several units occur (six at dynarray's 0x116EA8). So each unit is what is left of one function that is not in
the ELF: its last instruction and its alignment padding. Most likely the original toolchain's removal of unused
functions cut each one an instruction short; that is an inference from the bytes (nothing else in the ELF records the
removed functions). No C produces an instruction without its return, so the source emits each unit as one
instruction of assembly: `SLY2_SCRAP("addiu $sp, $sp, 0x30");` (include/sly2/scrap.h: `.align 3` + the instruction,
in the current text section, no symbol), at the place of the removed function. The asm goes after the previous
function's code and before any data definition (gcc 2.95 emits top-level asm where it stands, and after a data
definition the current section is `.rodata`: lo.cpp's first try put its scraps there). With `-ffunction-sections`
the scrap trails the preceding function's section, which is what link.py's check lays out; matchtool compares
functions by symbol size, so no function changes.

**The four "gap/padding" objects** (language 0x1A2D98, mat 0x11C4F8, so 0x1F6A60, joy 0x188328) are not 16-byte
alignment: so's next function (0x1F6A68) is only 8-aligned, and over the whole ELF the 8 gaps of one zero unit before a
function land 5 times 16-aligned and 3 times not. They are scrap units whose orphaned instruction is a `nop` (a
removed function ending `jr $ra; nop`): `SLY2_SCRAP("nop")`. slotheap's 0x11CDD0 and util's 0x11E028/0x11E030 are
more of them inside junk ranges. No aligned attribute or `-falign-functions` is involved.

**grab.cpp:** func_00192A28 kept its frame record as a local class, `struct { char c; STACKSCOPE frame; int i, j; }`;
g++ 2.95 emits a local class's implicit constructor and destructor out of line (two local functions, the destructor
with its `__builtin_delete` call) even though the function inlines both. Named at namespace scope (`SCOPEDIJ`, a
placeholder) they are inline functions with nothing that needs them out of line, so none is emitted.

**config/junk.csv:** the 64 rows inside these objects now end with "; emitted by its TU source (SLY2_SCRAP)"
(they are still not functions, so progress.py keeps skipping them); five rows for the nop units are new (173 rows).
On integrate/round8 ref's 0x129D50 and splicemap's 0x1FFA68 joined them, and after link/linkonce-misc the scraps
of cm, emitter, memcard and bsp plus two new nop-unit rows (bsp 0x118FA8, guardbhv 0x178CF8): 80 rows emitted by sources.
The rows at TU boundaries or inside the linkonce/crt0 objects are unchanged. No entry decodes as a function, so none
moved to functions.csv.

**Checks:** every touched file's functions and data MATCH (the same counts as before); nm symbol sets equal except
grab's two removed local functions and its `__builtin_delete` reference (and the per-compile `_GLOBAL_$I$` suffixes);
symcheck: 35 undefined not in config, 0 duplicate definitions (unchanged); relverify: 27,231 relocations OK, 0
mismatched. No change to codegen was needed or reverted.

## End state

The goal: a link that reproduces SCUS_973.16 byte for byte the way the original build did: every object compiled
once, its sections whole, the linkonce sections where the linker puts them, and no blob except what genuinely is
not source (the IOP modules, the VU microcode). The per-function fallback is gone (every object links whole since
link/linkonce-rest); two work lists stand between it and today's link.

### 1. Data no object defines (the data work list)

437,056 blob bytes in `.rodata`/`.data` at link/real-link (281,096 after data/crt0-tables, 216,056 on integrate/round8
with data/vtables, 211,760 on integrate/round9 with data/bss-gaps) and 487,824 bytes of `.bss`/`.sbss` gaps (60,772 after
data/bss-gaps: tu-merging.md "Phase 7", which lists what is left). Owner = the TU whose place in the
layout the gap takes: the original link put every object's sections in one command-line order (the order check in
Status: all but 7 sections follow `.text` order), so a gap between objects P and N belongs to P's tail, N's head or a
TU between them in `.text` order that places nothing in that section. Measured with a scratch script over
link/real-link's placement (gaps from the uncut section sizes: 437,324 bytes):

| block | bytes | address | owner | what it takes |
|---|---:|---|---|---|
| ~~crt0_splicewrap `.rodata`~~ | ~~98,000~~ | 0x262380..0x27A250 | class_registry.cpp | done (data/crt0-tables): the 199 class option arrays, g_mpcidarysplopt, HandleBasicSpliceEvent's event parameter types |
| ~~crt0_splicewrap `.data`~~ | ~~57,960~~ | 0x2AC880..0x2BAAE8 | class_registry.cpp | done (data/crt0-tables): g_aeopid, the 1,449 option records |
| palette16's built-in font | 132,464 | 0x27A250..0x29A7C0 | palette16 (head of its `.rodata`, before its palette D_0029A7C0) | font header {-1, 0x00290A19, 1.0f, 163} and 163 8-byte glyph records (0x27A250..0x27A778), the CLUT header D_0027A778 (16 colours: the palette D_0029A7C0), the 512x512 4-bit bitmap 0x27A790..0x29A790 (131,072 bytes, mostly 0xFF: these were "the tables filled with 0xFF"), the BMP header D_0029A790 and a font descriptor {header, glyphs, &D_002DD520 BMP, &D_002DD500 CLUT} at 0x29A7A8 (.data 0x2BE320 points at it). The structured parts are about 1.4 KB; the bitmap is an image and needs a decision on how image assets are kept in source |
| registry vtables | 100,312 at link/real-link; 35,272 left after data/vtables (174 vtables, 65,040 bytes, emitted by their key TUs), 24,944 after data/vtables-2 (44 more, 10,328 bytes), 24,864 after integrate/round10 (blend's AC5/ACG2, 80 bytes) | the six gaps of link/real-link (0x2C46E8 24,168; 0x2CA5E0 6,184; 0x2CBE38 9,808; 0x2CE4A0 4,200; 0x2CF520 18,432; 0x2D3FA0 37,520); left: the SDK `.data` at the head of the first gap (0x2C46E8.., newlib's reent data, libkernel's tables) and the linkonce area's vtables no object emits | left: 62 vtables without a config name (about 7,950 bytes); data/address-names-2 emitted 57 of them (tu-merging.md "Vtables emitted (data/address-names-2)"); left: EMITTER's and EXPLO's template-instance tables 0x2CE680 and 0x2CED18 (848 bytes) | real classes with the ELF's layout and a key function in each key TU, as data/vtables-2 did (tu-merging.md "Vtables emitted, continued") |
| palette TUs' tables | 33,024 | 0x29A800 | tail of palette16 / head of palette256 | the bitmap/glyph tables after D_0029A7C0 (defined) and before D_002A2900 |
| SDK `.rodata` | 10,084 | 0x2AA100 | SDK objects (sceSif error strings, libgcc's `__clz_tab`, libm tables, ...) | comes with the SDK objects (2 below) |
| small gaps | about 5,480 | ~60 gaps of 4..1,536 bytes (`.rodata` 2,600, `.data` 2,880) | neighbours or a TU between (largest: `.rodata` 1,536 between cheat and clock, `.data` 928 after emitter, 840 near empty_14C548) | unreferenced or unnamed statics of game TUs |

`.bss` (no bytes, but an address assignment each; all but the SDK tail, the commons and three small gaps
defined on data/bss-gaps; the commons by src/game/app/commons.cpp on data/commons-file): 400,080 from 0x3E1C70 (hud.cpp's buffers, D_003E1C70..D_003E21A0
only hud references, then the `.bss` of the TUs after it up to 0x4421A0); 60,000 at the end from 0x4437A0 (989snd
D_00443800.., libkernel's `_sif_*`, libgcc's `objects`; since data/commons-file the gap ends at 0x452098, where the commons
file's `.bss` starts); 21,536 at 0x3D4A50 (D_003D4A50/54/60, the record many TUs
reference); 4,416 at 0x2E0D00 (D_002E0D00, D_002E1E00); about 1,800 in small gaps.

Order of work: ~~the crt0_splicewrap tables~~ (done, data/crt0-tables: 155,960 bytes), then the vtables (100,312,
needs the key TUs' first virtuals; done in data/vtables, data/vtables-2 and integrate/round10 for every vtable with a config name), palette16's font (its bitmap waits for the asset decision), then the rest by TU. Each step lowers "data no object defines" and the list
of names link.py binds to absolute addresses (build/link/absolute.txt; "Absolute symbols" above).

### 2. SDK objects instead of blobs

118,224 `.text` bytes (and the SDK's share of `.rodata`, `.bss` above) come from the 329 sources that need i386
Linux binaries. The Mac builds them as progress does, in two places, and link.py links whatever objects exist:

    tools/dev.sh run python3 tools/link.py --sdk-only   # Docker: the 279 native-compiler sources
    python3 tools/link.py --sdk-only                    # natively: the 49 wibo + GNU as sources (as through Docker)
    python3 tools/link.py                               # links them as objects instead of blobs

(Each run reports the others as FAILED and keeps their objects; on an Intel Mac or a Linux machine with the i386
loader the first command builds all 329.) Each SDK source is one member of a Sony library, placed by address like
every other object, so no archive (libkernl.a, ...) is needed. Not tried yet (no i386 loader here). First Mac run
to check: 0 ld errors, 0 differing bytes, "text: SDK source that does not build here" at 0 and the SDK `.rodata`
and `.bss` gaps above gone or reduced. When that holds, tools/verify.sh can run the two `--sdk-only` steps before
the link.

### What stays a blob, and the script's final form

After 1 and 2: the IOP modules (`.irx`, 474,176) and the VU microcode (`.vutext`, 34,112) stay binary inputs; the 4
bytes of `func_00100000` need a source. Then every object section sits where the linker puts it, and the
address-ordered list in build/link/sly2.ld is the object command line: link.py can write the script as the
original's (`.text : { *(.text) *(.gnu.linkonce.t.*) }`, `.rodata : { *(.rodata.early) *(.rodata) }` and so on, objects
on the command line in TU order) and keep the address placement as the check. The 7 sections out of `.text` order
(Status) are what such a script needs special rules for, or source changes.

