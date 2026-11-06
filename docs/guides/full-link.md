# Full PS2 link (sketch)

What it takes to link the matched sources back into SCUS_973.16, and how far the repo is: the current numbers are in
"Status" right below (one table, for the latest integration round); the sections after it keep each branch's own measurements as history (integrate/round3: link
round 3 and data phase 2 — branch data/phase-3, the guide counts the first data pass as phase 1 — 2026-11-05).
tools/link.py links the image (since link/real-link a real link: one section per object, see "Real link
(link/real-link)"; the per-function prototype it grew from is "Prototype link"); what is left before it reproduces
the ELF the way the original build did is "End state" at the end. The symbol side is in
[tu-merging.md, "Linkage of namespace-scoped declarations"](tu-merging.md#linkage-of-namespace-scoped-declarations).

## Status (data/address-names-2, 2026-11-06)

The one current table; the Status tables of the earlier branches and rounds are in this file's history (last at
integrate/round10 690b28cf). data/address-names-2 (from integrate/round10 690b28cf) takes the names link.py binds to
absolute addresses from 492 to 274 and of those, what is left is SDK (Mac), commons, VU/scratchpad offsets that are
not image addresses, and 9 game objects waiting for decisions ("Absolute symbols" right below); it gives 57 more
vtables config names and emits them from their key TUs ([tu-merging.md, "Vtables emitted
(data/address-names-2)"](tu-merging.md#vtables-emitted-dataaddress-names-2)). Every value from freshly built objects
(build/link and build/progress_cache.json deleted first); integrate/round10's in brackets where different.

| measure | value | tool |
|---|---|---|
| functions matching | 6,662 / 6,662 (100%; 856 sources, 329 SDK sources carried over, verified on the Mac); config/matched.csv content unchanged by the run | `JOBS=3 tools/progress.py`, cache deleted, run alone |
| all game sources (210) | functions 6,017 / 6,017, data symbols 2,187 / 2,187 [2,118] MATCH | `tools/matchtool.py check` |
| vtables from objects | 284 `.gnu.linkonce.d._vt$*` sections [228], each MATCH against the ELF and emitted in ascending ELF order per object; 2 left in the linkonce data area (EMITTER's/EXPLO's template-instance tables 0x2CE680, 0x2CED18: tu-merging.md) | `tools/matchtool.py check`, llvm-readelf |
| absolute symbols | 274 [492]: SDK functions the SDK objects define (Mac) 109, SDK data and SDK-only references 53, SDK name mismatches 8, commons 69 [84], VU-memory/scratchpad offsets 26, game objects not defined 9 [78]; plus 17 labels on the `.vutext`/`.irx` blobs (not absolute since this branch) | `tools/link.py` (build/link/absolute.txt) |
| undefined references not in config (209 merged objects), duplicate strong definitions | 4 [5]: the four `_gp` (tn's `Init__5CPLCY` resolved); 0 | scratch symcheck.py |
| relocations checked against the ELF | 52,803 [52,051] OK, 0 mismatched | scratch relverify.py |
| link objects | 527: 527 whole, 0 in per-function fallback | `tools/link.py` |
| ld.lld | 0 errors; 1,286 [1,221] object sections placed, 2 [3] unplaced (140 bytes: an SDK object's `.data` 100, bq's linkonce copy 40; tn's CMPART2 vtable is placed now), 2 `.bss` overlapping another bound to their address (tn, cplcy) | `tools/link.py` |
| `.bss`/`.sbss` gaps no object defines | 60,772 bytes in 6 address assignments | `tools/link.py` |
| differing bytes | 0; .text, .rodata, .data, .vutext, .irx equal; from objects .text 1,295,844, .rodata 128,544 [128,176], .data 180,852 [173,628] | `tools/link.py` |
| blob bytes | 822,888 [830,480]: IOP modules 474,176; data no object defines 193,760 [201,352]; text of SDK sources that do not build here 118,224; VU microcode 34,112; junk.csv 1,576; padding / not in config 1,036; `func_00100000` 4 | `tools/link.py` |
| config names at their config address | 8,066 / 8,066 [7,982]: the 57 vtables, 2 template statics, `_ctype_`; renamed rows (printf, sprintf, PvAllocSlot__FP8ALLOCREQ, __4CRefRC4CRef, the five syscall/libgraph names, sceSifLoadModuleBuffer) | `tools/link.py` |
| docs links | 0 broken | `tools/docs/linkcheck.py` |

## Absolute symbols (data/address-names-2)

tools/link.py lists every name it binds to an absolute address in build/link/absolute.txt (address, ELF section,
class, the referencing objects) and prints `absolute symbols: N` by class. A name no object defines is bound to its
address (address names by the address in the name, config names by config, the rest by the ELF's relocated value),
commons to the ELF's address; a name that points into the VU microcode or the IOP modules is defined by blobs.o as a
symbol on its blob section instead (those two stay binary for good, "What stays a blob" below), so it moves with
the blob. What changed, by class (start 492, integrate/round10):

| class | before | after | how |
|---|---:|---:|---|
| reference to a name whose address an object defines under another name | 70 + 30 | 0 | asm labels to the defined symbol (58 vtable names, `__builtin_new`, `_$_10CStreamBis`, `g_pfrmOpen`, `CopyAb__FPvT0Ui`, `__pure_virtual`, ...), the defining object renamed where config names it so (`__4CRefRC4CRef`, `PvAllocSlot__FP8ALLOCREQ`, printf/sprintf, the GC pool words' template statics) |
| interior of a defined object | 11 | 0 | gs's double buffer as four objects; elsewhere an asm label with an addend (`__asm__("D_002DDF40+0x60")`): a separate symbol to g++, the owner to the assembler (tu-merging.md, Phase 8) |
| vtables no object emitted (address-named references) | 45 | 0 | emitted by their key TUs (57 with the unreferenced ones) |
| other game objects not defined | 33 | 9 | bsp's sentinel planes/leaves, util's limits, mat's/vec's matrix and vector constants, hud's M23, grabspot's flag, emitter's empty reference blocks |
| commons | 84 | 69 | the address-named duplicates of template statics merged into them; the rest stay (below) |
| `.vutext`/`.irx` labels | 17 | 0 (17 blob labels) | link.py defines them on the blobs |
| VU-memory / scratchpad offsets (0x0..0x3AB0) | 26 | 26 | not image addresses: absolute is right |
| SDK | 179 | 170 | `_ctype_+1`, `_impure_ptr`, `get_eh_context` by name; five syscall/libgraph sources and sceSifLoadModuleBuffer define the names their callers use |

Left, and who decides:

- **Mac (SDK objects)**: 109 SDK function names resolve when `--sdk-only` objects link; 53 SDK data names and
  SDK-only references come with the SDK objects (989snd's words and strings, libkernel/libgraph tables, newlib's
  reent data, `_ctype_`). 8 SDK names do not match the defining source's name and stay absolute even then:
  strtol (func_00234580.c), RemoveIntcHandler, sceSifGetReg, Deci2Call, GetMemorySize, `_InitTLB`, DisableIntc (the
  syscall stubs func_002347xx..002351E0) and sceSifSetReg (0x23A920, which config calls sceSifExecNotifyHandler):
  renames of SDK sources the naming pass deferred to the Mac (docs/reference/name-confidence.csv). Also to verify
  on the Mac: func_00237B80.c and _sceMpegCscStoreRefImage.c (gnu-reorder) now call SignalSema / FlushCache by name.
- **User decisions**: tu_1147D8's built-in font (D_0027A778, D_0029A790, font.cpp's D_0029A7A8/D_002A28E8) and
  tu_114AE0's bitmap block (D_0029A8C0, D_002A28D0): game artwork, how image assets are kept in source. The commons
  (69: the Splice GC pool words and pages, CSlotPool/CSpliceHeap's template statics and their counters, 989snd's
  and libkernel's buffers, prog/hud/screen's words): g++ 2.95 emits class and template statics as COMMON and ld
  allocates them at the end of `.bss`/`.sbss` in an order ld.lld does not reproduce, so link.py assigns each its
  ELF address for byte identity. Without the assignments the link still works (ld places them), only not at the
  ELF's addresses. Moving them under the link's control needs either a link.py rule that places each object's
  COMMON input in order, or one source that defines them all (strong definitions win over the commons); a design
  choice, not taken here.
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
| merged game TUs | 209 | 5,568 | yes | the objects the link check covers |
| game per-function directories (air/, splice_thunks/, crt0/) | 76 | 76 | yes | superseded: air was merged, splice_thunks went into class_registry.cpp (link/crt0-block); crt0 is the row below |
| generated Splice wrappers (src/generated/splicewrap) | 373 | 373 | yes, inside class_registry.cpp | the option/property binding thunks the crt0-block records point at; `.inc` fragments of the crt0_splicewrap TU since link/crt0-block |
| SDK (src/sdk: libc, libgcc, libkernel, libcdvd, libgraph, libmc, libmpeg, 989snd, ...) | 646 | 646 | partly | 333 sources of the repo (SDK files that need GNU as for `SLY2_ASM: gnu-reorder` or the native ee-gcc 2.96, and asm files) do not build in the Linux workspace and are verified on the Mac |
| crt0 | 1 | 1 (`_start`) | yes | src/game/platform/boot/crt0/_start.c; `func_00100000` before it is unmatched |
| dead code | - | - | - | junk.csv ranges: emit the original bytes to keep addresses |
| VU microcode, IRX modules | - | - | - | blobs |
| data nobody defines | - | - | - | 466 absolute address names over all 932 objects (467 after data/address-names, 888 before it; 779 for the 209 merged objects at integrate/round3), see below |

matched.csv has 6,663 rows (all `match`). Named data objects that the 209 merged objects define, by section (data
symbols with a known address, bytes): .data 845 objects / 0x81E9 (16.7% of the section), .rodata 205 / 0xF63 (1.3%,
plus the anonymous literals of every object), .sbss 3 / 0xC, .bss 179 / 0xEB4E0 (63.1%): 1,232 objects, about 1,236
of the data survey's ~1,740 with the six vtables (tu-merging.md, "Data"). Before data phase 2 (link/round3): .data
466 / 0x38F5, .rodata 100 / 0xC75, .sbss 3 / 0xC, .bss 140 / 0xE5BF4.

## Symbols (state after link round 3 and data phase 2)

- 531 undefined references not in config remain in the 209 objects (superseded: 443 on integrate/round5, see Status; 507 since build/rtti-palettes: the 22 typeinfo
  references and two alias-table vtables are gone, see "-fno-rtti and the palettes" below): 289 are listed in config/link_aliases.txt
  (vtables and destructors of namespace-scoped views, a few declarations source cannot bind) and 242 are referenced
  only from key-TU extras that the ELF does not have (unreferenced linkonce vtables and typeinfo functions). The extras
  must not be resolved but dropped: `--gc-sections`, or a `/DISCARD/` for those `.gnu.linkonce.*` sections. Data
  phase 2 left this set unchanged: no symbol of the alias table is among the newly defined objects (its six vtables
  are of real classes, the table's 214 are of views; 11 alias targets, e.g. D_002DEA4C, are defined by objects).
- types/views-2 (the registry classes made real; superseded by Status): 31 unresolved references remain (from 439 at build/rtti-palettes
  8d31590d): config/link_aliases.txt is down to 42 lines (21 view vtables: act.cpp's curve classes, hud's and
  memcard's local classes; brx_sw's destructor; the six other declarations). class_registry's factories reference
  the 191 real vtables `_vt$<len><NAME>`, which config/symbol_addrs.txt names as data (size (slots + 2) * 8, entry 0
  and the terminator null); no object defines them except missile.cpp's `_vt$7ACCMISS` (its key TU), which matches
  the ELF byte for byte. The key-TU extras of jt.cpp and sm.cpp are gone (Pclassinfo is now the classes' first
  virtual, never defined as a member) and missile's references name real functions, so "referenced only from
  extras" dropped from 242 to the 2 `gcc2_compiled.` markers plus tn.cpp's CMPART2. 26,974 relocations agree with
  the ELF, 0 mismatched.
- 1,146 references to 616 address names nobody defines (1,860 to 1,133 before data phase 2).
  `tools/linkaliases.py <objects>` derives them: 24 `func_` names are config functions under another name
  (`func_002333B4 = memset;`, declared under the address name by 70 of the 209 sources) and become aliases; the rest
  (779 names with the alias targets; 1,296 before data phase 2) become absolute addresses: .data 323, .rodata 162,
  .bss 190, .sbss 31, .text 29, .vutext 14, .irx 3, and 27 small values (0x0..0x3AB0, VU-memory / scratchpad offsets).
  **After data/address-names (2026-11-05)**, over all 932 objects tools/link.py builds (the 209-object count above
  is a subset): 917 address names (888 absolute, 29 aliases) -> 469 (467 absolute, 2 aliases); absolute by section
  .data 345 (265 of them the vtable block), .rodata 39, .bss 30, .text 10, .vutext 14, .irx 3, small values 26. Air's
  data, the commons at the end of `.bss`/`.sbss`, interiors (base+offset), PMF constants and hud's records are now
  defined or resolved through objects; the rest and the reasons: tu-merging.md, Data, "Phase 3: address names".
  **After data/interior-fields**: 468 (466 absolute: sw.cpp defines D_003E1C34; 2 aliases); symcheck's unresolved
  undefined references 467 (unchanged), relverify 27,043 relocations OK, 0 mismatched (measured without
  `-fno-rtti`; superseded: 443 and 27,044 on integrate/round5, see Status).
- Two objects define different functions under the same file-scope destructor name (`_$_3SPM`, `_$_3OBJ`): per-object
  `objcopy --redefine-syms` files from `tools/linkaliases.py --redefine DIR`.
- 26,377 relocations agreed with the ELF after data phase 2 (field minus addend = the symbol's config address,
  through the alias table; 25,768 before it). The 2 that did not (config/functions.csv and config/symbol_addrs.txt
  disagreed on the libcdvd names at 0x242A20, 0x242AB8 and 0x242AE0) are settled: 0x242A20 sceCdGetDiskType2
  (S-command 3, raw), 0x242AB8 sceCdGetDiskType (its -1 -> 0 wrapper, called by FValidCdMmode), 0x242AE0
  sceCdGetError (S-command 4, called by bas.cpp, ReadCdDirect and tu_1848A8's func_001849A0, the last two now spelled
  so in source); see docs/guides/tu-merging.md, "Linkage of namespace-scoped declarations". air.cpp (merged last)
  adds 544 relocations, all agreeing.
- Duplicate strong definitions among the 209 objects (and among the 449 per-function and generated objects): 0.

## Two ways to link

1. **Overlay link (first step).** Start from the original image and place each built object's code at its original
   address: one output section per TU (or per function) at a fixed address, data left in the original image,
   address names resolved as absolute addresses (valid exactly because the data stays where it was). The result
   can be compared byte for byte with the ELF; a change that keeps a function's size fits in place, a bigger one goes
   into a code cave or an appended section with the call sites relinked. This needs no data from source and works
   with the SDK still unbuilt (its code comes from the image, its symbols from config).
2. **Real link.** Every section from objects, in the ELF's order, so code and data can move. Needs: an object for
   every byte (SDK, dead code as `.word` blobs or the junk ranges recompiled, VU and IRX blobs as binary input),
   every `D_` defined by some object (today 467 are not: 265 in the vtable block, 43 blob labels or small values), the TU order inside each output section reproduced from
   tools/layout/units.csv (functions) and the data addresses (the anonymous .rodata literals land wherever their
   object lands, so the order is checked by comparing the image, not by symbols), and the absolute address names
   replaced by symbols of the objects that define the data.

Linker script, from the layout above: `ENTRY(_start)`; `.text` at 0x100000 with the crt0 block first
(`func_00100000`, `_start`, ...), then the TUs in address order (linkonce copies homed by address, see
tu-merging.md "Inline functions and linkonce copies"); `.vutext` with the DVP overlay sections; `.rodata` at 0x262380;
`.data` at 0x2AC880; `.sbss` at 0x2DD280 with `_gp = 0x2E5270`; `.bss` at 0x2DD380; `.stack` (0x20000) at 0x452200
outside the segments; `.irx` at 0x472200 in its own PT_LOAD; the output of `tools/linkaliases.py` included (retired
in integrate/round10: link.py binds the address names itself); the key-TU extras discarded.

## Real link (link/real-link, 2026-11-06)

`python3 tools/link.py [--no-compile] [--per-function]` or `--sdk-only`; tools/verify.sh runs it without options. Needs
the same tools as before (`ld.lld`, `llvm-objcopy`, disc/SCUS_973.16; "Prototype link" below says where they come
from). A full run is about 3 to 5 min here (528 sources, 2 cores, plus the fallback objects); `--no-compile` about 5 s.
A source that fails to compile loses its object (a stale one is deleted, never linked), the link still runs (its
bytes come from the ELF as blobs) and prints its summary, and link.py exits 1 listing the failing sources, so
verify.sh's link step fails (link/linkonce-rest; before, the previous object was kept and the link passed).

1. **Objects as the original build made them.** Every buildable source of config/matched.csv through matchtool's
   `compile_src`, without `-ffunction-sections` (build/link/obj-tu/): one `.text`, `.data`, `.rodata`, `.bss` per
   object (plus the named sections a few sources ask for: `.rodata.early`, `.data.buildinfo`) and a
   `.gnu.linkonce.*` section per vague-linkage instance.
2. **Which objects link whole.** For each object, every function of its `.text` with a config address must sit at
   that address relative to one base, which is what a contiguous TU in the ELF means. The others are classified
   the way link/contiguous-tus's TU check did (the same 76 objects on link/real-link, 6 on integrate/round8; reasons
   per object in build/link/tutext.txt):
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

## Prototype link (link/prototype, 2026-11-05)

`python3 tools/link.py [--no-compile]` (one script plus the template tools/link.ld.in). Needs `ld.lld`
and `llvm-objcopy` (LLVM 18 here; no MIPS GNU binutils were installable in the workspace) and your disc/SCUS_973.16.
The script finds them via `$LLVM_BIN`, then PATH, then Homebrew's keg-only `llvm`/`lld` (macOS: `brew install llvm`;
if Homebrew's llvm aborts with `Library not loaded: .../libz3...dylib`, its build is stale against a newer z3: `brew upgrade llvm`).
Everything it writes goes to build/link/ (ignored): objects, build/link/blobs.o (byte ranges cut from your ELF), the
linker script sly2.ld, sly2.elf, ld.log, and the diagnostics overlaps.txt, conflicts.txt, unplaced.txt, wrongaddr.txt,
mismatch.txt (since link/real-link: sections.txt, tutext.txt, wrongaddr.txt, mismatch.txt; "Real link" below). A full run is ~6 min of compiling (932 sources, 2 cores) plus ~20 s; `--no-compile` reuses the objects.

What it does:

1. **Objects.** Every source of config/matched.csv that builds here, through matchtool's `compile_src` (same flags,
   per-file overrides and relative path), plus `-ffunction-sections` (and, until data/order-2, `-fdata-sections`;
   now each object's `.data`/`.rodata` is one section in source order, as in a real link). Checked at the time: with those flags all
   6,334 functions with a config address still MATCH, and 1,236 of 1,238 data symbols (the 2 others fail the same way
   without the flags, see below). The 329 SDK sources needing GNU as / native ee-gcc 2.96 are not built.
2. **Placement.** Each object section's address is derived from the ELF, not assumed: votes from every symbol with a
   config address (weighted by size), then propagated through relocations: for a placed section, the ELF's relocated
   field (jal target, %hi/%lo pair, word) minus the object's addend gives the address of the target section or symbol.
   The same propagation gives addresses to 53 undefined names no config file knows (`_impure_ptr`, `_ctype_`, the slot
   pool init guards `__sn_t9CSlotPool...`, `__as__4CRefPv`...) and to COMMON symbols, which become absolute
   assignments in the script (as do config names that only unbuilt code defines).
3. **Script.** One output section per allocated ELF section at its address (from the ELF's headers); in it, object
   sections in address order with `. = ADDR;`, and every byte no object covers filled from blobs.o; `_gp` from
   .reginfo; an absolute address for every name no object defines (address names by the address in the name, config
   names by config, the rest by the ELF's relocated value; tools/linkaliases.py and config/link_aliases.txt did part
   of this until integrate/round10). Object fixes before linking (in build/link/fix/): llvm-objcopy rewrites each
   object (gcc 2.95's as puts local symbols after globals, which ld.lld rejects); sections holding one symbol are cut
   to the symbol's size (gas pads each section to its alignment, which with one section per object would overlap the
   next object). Linkonce copies: the first placed copy is linked, the other copies' symbols are bound to its address.
   `--gc-sections` with every placed section `KEEP`: the key-TU extras (unreferenced vtables, typeinfo) drop out.
4. **Compare.** sly2.elf against the ELF byte by byte per section, with the owner (object section or blob) and the
   reason (from the relocation at the differing word) of each differing byte; config names at the wrong address in
   the output (this is what checks .sbss/.bss, which have no bytes).

### Measured (integrate/round3 d864c1bc sources; superseded by Status)

"from objects" counts only bytes that object sections supply; the rest is blob and matches by construction.

| section | bytes | equal | match | from objects | equal | match |
|---|---:|---:|---:|---:|---:|---:|
| .text | 1,416,688 | 1,415,991 | 99.95% | 1,282,044 | 1,281,347 | 99.95% |
| .vutext | 34,112 | 34,112 | 100% (blob) | 0 | | |
| .rodata | 304,356 | 303,004 | 99.56% | 21,552 | 20,200 | 93.73% |
| .data | 199,088 | 199,069 | 99.99% | 33,505 | 33,486 | 99.94% |
| .irx | 474,176 | 474,176 | 100% (blob) | 0 | | |
| .sbss/.bss | (no bytes) | | | | | |

Config names at their config address in the output: 7,022 of 7,075 (wrong: .bss 50, .sbss 2, .rodata 1).

**data/interior-fields (2026-11-05, without `-fno-rtti`; superseded by Status)**, same tool in the Linux workspace; the branch start (data/address-names
f45178d9) measures .text 1,415,908 equal (objects 1,281,264) and 7,216 of 7,269 names here. After it (memcard, mpeg
and sw `.bss` laid out as the ELF, tu-merging.md "Phase 4: interior fields"):

| section | bytes | equal | match | from objects | equal | match |
|---|---:|---:|---:|---:|---:|---:|
| .text | 1,416,688 | 1,416,092 | 99.96% | 1,282,044 | 1,281,448 | 99.95% |
| .rodata | 304,356 | 303,029 | 99.56% | 23,564 | 22,237 | 94.37% |
| .data | 199,088 | 199,068 | 99.99% | 33,981 | 33,961 | 99.94% |

Config names at their config address: 7,239 of 7,273 (wrong: .bss 31, .sbss 2, .rodata 1).

With `--tu-data` (no `-fdata-sections`: each object's .data/.rodata laid out in source order, as a real link would;
the only mode since data/order-2):
.text 99.80% (objects 99.78%), .rodata 98.14% (objects 11,604 of 17,280 = 67.15%), .data 99.34% (objects 29,655 of
30,972 = 95.75%); config names at their address 6,652 of 7,133 (wrong: .data 308, .rodata 121, .bss 50). This is
the data-order work list for the real link.

Blob-covered bytes (default mode), by reason: IOP modules (.irx) 474,176; data no object defines 448,387; text of SDK
sources that do not build here 118,224; VU microcode 34,112; text padding / not in config 14,276 (mostly inter-function
alignment padding left by `-ffunction-sections`); dead code (junk.csv) 2,136; unmatched function 4
(`func_00100000`); matched but not in an object 4. The non-allocated `.DVP.*` overlay sections are not produced.

Placement: 7,703 object sections placed; 159 sections that repeat bytes another object already places (the generated
splicewrap files each carry their own copy of a string of the one original crt0 TU) and 6 .bss sections lying under a
section whose layout differs are bound to their address instead of linked; 9 sections have a stricter alignment than
their ELF address allows (alignment lowered: guardbhv .rodata 16, hurt's three 8-byte .data objects at 4-aligned
addresses, ...); 2 sections overlap and go to `.unplaced` (class_registry .rodata, hurt .rodata); 83 unplaced (3,072
bytes: key-TU vtables/typeinfo of JT, ACCMISS, SM, CMPART2, eopid_linkonce's `g_eopidref_*` pointers, grab.cpp's local
class destructor, 100 bytes of snd_SendCurrentBatch .data).

### -fno-rtti and the palettes (build/rtti-palettes, integrate/round4 bd60d4cd sources)

C++ now compiles with `-fno-rtti` and the two palettes carry `__attribute__((section(".rodata")))` (tu-merging.md,
"Data", resolved decisions). Same script, default mode; before = bd60d4cd without the change (identical to the
round-3 numbers above), after = this branch:

| section | bytes | equal before | equal after | match after | from objects before | from objects after | equal before | equal after | match after |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| .text | 1,416,688 | 1,415,991 | 1,416,000 | 99.95% | 1,282,044 | 1,282,044 | 1,281,347 | 1,281,356 | 99.95% |
| .rodata | 304,356 | 303,004 | 303,026 | 99.56% | 21,552 | 21,688 | 20,200 | 20,358 | 93.87% |
| .data | 199,088 | 199,069 | 199,086 | 99.999% | 33,505 | 33,505 | 33,486 | 33,503 | 99.99% |

Differing bytes 2,068 -> 2,020: the 17 typeinfo-slot `.data` bytes (class 2) are gone, and so are the palettes'
8 `.rodata` bytes plus the 9 `.text` bytes of the two initializers' %hi/%lo (each TU's `.bss` had been placed at
the palette address, the CLUT/BMP objects with it); tn's `.rodata` layout bytes went from 21 to 7 as a side effect
of the placement (not investigated). Config names at their config address: 7,024 of 7,074 (wrong: .bss 48, .sbss 2; before 7,022 of
7,075 with .rodata 1, .bss 50). Placed object sections 7,697 (7,703), unplaced 69 / 2,432 bytes (83 / 3,072): the
`__tf*` sections of the key TUs are no longer emitted. No `__tf*`/`__ti*`/`__rtti_*` symbol is defined or referenced
by any object. ld.lld: 0 errors.

### The registry classes made real (types/views-2, from build/rtti-palettes 8d31590d; superseded by Status)

Same script, default mode, against the build/rtti-palettes numbers above: `.text` 1,416,000 and `.rodata` 303,026
equal bytes (unchanged), `.data` from objects 34,145 bytes (33,505 + the 640 of missile.cpp's `_vt$7ACCMISS`, the
first registry vtable an object supplies), 34,143 equal (all 640 new bytes equal; the 2 differing are the old ones).
Placed object sections 7,698 (7,697 + that vtable), unplaced 66 / 544 bytes (69 / 2,432: the key-TU vtables of JT
and SM are no longer emitted). Config names at their config address: 7,025 of 7,075 (`_vt$7ACCMISS` the added one;
wrong: .bss 48, .sbss 2, as before). ld.lld: 0 errors.

### Remaining mismatch classes (default mode, 2,068 bytes before build/rtti-palettes, 2,020 after; 1,910 on integrate/round5)

1. **Object sections whose internal layout differs from the ELF** (1,296 .rodata bytes, plus 697 .text bytes of
   %hi/%lo referencing them; on data/bss-order 1,263 + 299, every `.bss` in place). Anonymous literal pools (18 objects; 15 of them interleave with the object's named
   .rodata in the ELF, so in this mode it is an artifact of `-fdata-sections`, in `--tu-data` mode a source data-order
   issue: memcard, guardbhv, text, screen, air, stepplayer, jt, prize ...) and file-level .bss whose order differs
   from the ELF (`-fdata-sections` does not split .bss in g++ 2.95): gs, joy, dl, shd, screen, game, sprbuf
   (build/link/conflicts.txt lists the competing addresses; mpeg, memcard and sw fixed on data/interior-fields: g++
   emits objects with a constructor at their definition and the rest in first-declaration order, tu-merging.md
   "Phase 4: interior fields"). **data/bss-order**: every `.bss` now as the ELF (dl, gs, glob, shd, prog, screen,
   game, stepguard, joy, grab, sprbuf); in `--tu-data` mode the `.rodata` of text, guardbhv, game, stepguard,
   screen, air, stepplayer, jt, prize, murray, sound, relay, tu_17F090 and booty is byte-identical. The default mode's
   `.rodata` bytes cannot improve by source order: `-fdata-sections` takes the named constants out of the object's
   `.rodata` and leaves its literals and jump tables together, while the ELF interleaves them; tu-merging.md "Phase
   5: data order" has the emission rules, the decision to keep the flag for now, and the remaining list. **data/order-2**: the remaining objects fixed
   and `-fdata-sections` dropped (tu-merging.md "Phase 6: one link mode"); class 1 is down to class_registry's
   literal pool (112 `.text` bytes).
2. **Typeinfo slots in vtables** (17 .data bytes): our vtables pointed at `__tf*` typeinfo functions, the ELF's slot
   is 0. Resolved (build/rtti-palettes): `-fno-rtti` is in the C++ flags, the vtables match and the typeinfo extras
   (and `__rtti_si`/`__rtti_user`) are gone. matchtool's data check masks relocated words, which is why this never
   showed there; the slot is now an unrelocated 0 and compared. The flag changed 12 class_registry factories (a
   secondary vtable shared with the base's), fixed in the views; a full progress run stays 6,662/6,662.
3. **Bytes differ with no relocation** (53 .rodata + 2 .data): stepguard .rodata 24, bentley 7, aseg, hud, act,
   tu_1BAB50, relay, xf 2. The two zero-filled const arrays with a dynamic initializer (tu_1147D8 D_0029A7C0,
   tu_114AE0 D_002A2900: `.bss` in ours, `.rodata` in the ELF; 8 + 9 bytes counted under class 1) are resolved with a
   section attribute (build/rtti-palettes); matchtool's data check reports both MATCH.
4. **Code order for a TU-section link** (no bytes here, since every function is placed alone): 893 objects' code is
   one contiguous run in source order; 38 are split by other objects' code: class_registry (24 runs, interleaved
   with splice_thunks/splicewrap in the crt0 block) and the TUs that keep a linkonce-area copy as a plain function
   (eval 6, sidebag 5, frame 5, ref 4, basic 4, emitter 4, bif 4, spliceobj 3, splicemap 3 ...). Diagnosis and the
   check without `-ffunction-sections`: "Code order (link/contiguous-tus)" below.

### Code order (link/contiguous-tus, 2026-11-05)

What a real link (one `.text` per object, as the original build) needs from the sources, measured on
integrate/round6 1d4d7c44 (integrate/round7, one data mode: the same 76 verdicts, build/link/tutext.txt identical). `tools/link.py` now ends with a second TU check, "without -ffunction-sections": each
object's `.text` sections are laid out back to back in source order with their alignments (what gas does with a
single `.text`) and must share one base address; the per-object verdicts go to build/link/tutext.txt. It agrees object
for object with a scratch check that compiled all 932 sources without `-ffunction-sections` and compared each
object's one `.text` with the ELF (same 76 failures).

| verdict | objects |
|---|---:|
| `.text` links as one run at its ELF address | 855 |
| split by other objects' code | 38 |
| dead code (config/junk.csv scraps) inside the object's range | 33 |
| gap/padding differs | 4 |
| unplaced section | 1 |

**Why the 38 are split** (every stray run was located against functions.csv and units.csv):

- **Linkonce copies kept as plain functions: all 38.** 127 functions that the original emitted with vague linkage
  (`.gnu.linkonce.t.*`, in the linkonce area 0x254F00..0x259DE4) are plain `.text` functions in our sources, homed in
  a TU that uses them (tu-merging.md, "Linkonce instances"). For 37 objects this is the only cause: splice/gc 12,
  core/containers/tu_119BC8 11, app/bif 10, engine/fx/emitter 10, gameplay/player/grabspot 8, splice/eval 8,
  splice/sidebag 6, splice/spliceutils 6, gameplay/objects/rhythm 5, splice/frame 5, object/spliceobj 4, trail 3,
  glob 3, basic 3, ref 3, bsp 2, cm 2, sound 2, splicemap 2, game 2, and one each in po, step, bis, asega, blend, xf,
  cplcy, mark, screen, guardbhv, tu_1553F0, pipe, rail, proxy, proc, varb, memcard. For gc, spliceutils and
  tu_119BC8 the linkonce copies outnumber the TU's own code, so link.py's run list shows the TU's real range
  (0x128138, 0x12AE18, 0x119BC8) as the stray one. The families (tu-merging.md, "Linkonce copies by family") point at
  a few templates: the six Splice GC pools (the `Gc<Pool>_PushFront/_InitNode/_Unlink` triples, `PvNew<Pool>`,
  the counted-pointer stores and destructors, gc.cpp's sweeps, spliceutils' `GcShutdown<Pool>Pool`), the
  EMITTER/EXPLO set around sly2/emitter.h, small accessors next to eopid_linkonce's, and math/text-box inlines.
- **The crt0 block: class_registry's other 21 runs** (plus its 3 constructors STEPGUARD_ctor, JT_ctor, MURRAY_ctor,
  which are linkonce copies). Done on link/crt0-block: class_registry.cpp is now the whole TU and its `.text` one run
  at 0x100218 apart from the three constructors (Status). The original TU crt0_splicewrap (0x100218..0x1147D8) is one source file whose
  functions our repo spreads over three places: class_registry.cpp (424), src/game/app/splice_thunks/ (31 C++
  files) and src/generated/splicewrap/ (373 generated C files). In 0x100218..0x110AB0 class_registry's 2 asm
  fragments and 33 option thunks alternate with thunks and wrappers in the original source order; the factories and
  getters from 0x110AB0 on are one run. class_registry is not several TUs and crt0 does not sit inside it (`_start`
  at 0x100008 comes before the block, its own object): the block has to become one object.
- **Functions that belong to another TU: none.** Every stray run is either in the linkonce area or in the crt0 block,
  so no move between merged TUs was made.

**The other 38 failures** (only seen without `-ffunction-sections`): 33 objects contain junk.csv dead-code scraps
inside their range (bytes no source emits: tu_1A4E08, bas, dl, dynarray, tu_11AFE0, bez, util, vec, sm, wr, font, rip,
dmas, gifs, gs, vifs, shd, clip, shape, hud, stepguard, tu_144B28, tu_20CE28, tu_17F090, jt, stepplayer, cheat, lo,
cd, slotheap, text, thread, sw); 4 have, besides scraps, 8 zero bytes before a 16-byte-aligned function that our object
aligns to 8 (language 0x1A2DA0, mat 0x11C500, so 0x1F6A68, joy 0x188330 StaticInit_joy); grab.cpp's local class
constructor/destructor (func_00192A28's) are code the ELF does not have. For a real link the scraps need source
(the dead functions or a `.word` block) at their place. Done on link/junk-padding: "Junk scraps and padding" below.

**Fixes, in order:** (1) the linkonce copies back to templates/inline functions in headers (tu-merging.md "Inline
functions and linkonce copies": a non-`inline` function template is emitted per instance as linkonce and never
inlined into callers, which is why emitter.h's set kept matching; free inline functions would be inlined by callers
that now call them), one family at a time, renaming to the mangled names (link/linkonce-pools, link/linkonce-misc:
the five families below; an inline function defined after its callers keeps them calls); (2) the crt0 block as one object (done on
link/crt0-block: the wrappers included as `.inc` fragments in address order, compiled as C++ with C linkage; their
code is unchanged); (3) the junk scraps and the 16-byte alignments. (1) and (3) were not done on
this branch (timebox: diagnosis only); (3) is done on link/junk-padding (next section).

### Junk scraps and padding (link/junk-padding, 2026-11-06)

The 38 objects that failed the check without `-ffunction-sections` for reasons other than linkonce copies and the
crt0 block, fixed in their sources (from main f674c97e). `tools/link.py`'s check, before -> after:

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

### Linkonce pools (link/linkonce-pools on integrate/round7 f674c97e, 2026-11-06)

(Measured with the per-function TU check of main f674c97e, before the real-link and junk-padding merges; the
integrated count is in Status. The two scraps of splicemap and ref are
emitted by their sources since integrate/round8.)

Fix (1) for the first family: the 58 Splice GC pool copies (tu-merging.md, "The Splice GC pool family") are template
and inline instances again (`CGcObj<T>` in sly2/gc.h, `CRefPtr<T>` in sly2/basic.h, the implicit ~CFrame/~CProc),
each in its own `.gnu.linkonce.t.<symbol>` section, emitted by splice/gc.cpp (spliceobj and ref emit one each).
`tools/link.py` on the branch:

| measure | before (round 7) | after |
|---|---:|---:|
| TU check with `-ffunction-sections`: objects one section / split | 893 / 38 | 904 / 27 |
| TU check without `-ffunction-sections`: one run / not | 855 / 76 | 864 / 67 |
| of the 67: split by other objects' code / junk scraps inside / gap / unplaced | 38 / 33 / 4 / 1 | 27 / 35 / 4 / 1 |
| sections placed (6 unplaced, 544 bytes, both) | 6,760 | 6,760 |
| differing bytes | 115 | 115 |

Eleven objects no longer split: gc, spliceutils, eval, frame, sidebag, proc, varb, spliceobj, proxy, and splicemap and
ref, which now fail only for a junk.csv scrap inside their range (0x1FFA68, 0x129D50: 8 bytes each), hence 33 -> 35.
bif and basic stay split by copies of the other families (bif's VU math, basic's GetEmitterClassDesc_linkonce). The 27
left: class_registry (crt0 block) and the EMITTER/EXPLO, accessor, container, math/text-box and "other" families.

### Linkonce copies moved (link/linkonce-misc, 2026-11-06)

(Measured on main f674c97e with its per-function TU check; on integrate/round8 the scraps of emitter, cm, memcard,
bsp and guardbhv named below are emitted by their sources and the fallback is 6 objects, Status.)

From main f674c97e (integrate/round7 sources, `-fno-rtti`), the EMITTER/EXPLO, accessor, math/text-box and "other"
families of the plain-function linkonce copies (tu-merging.md, "Linkonce copies by family") went back to linkonce
form: 51 functions (EMITTER/EXPLO 12 as function templates in sly2/emitter.h; 12 data-only references, the GRABSPOT
owner getters and two g_aeopid setters, as free inline functions emitted by eopid_linkonce.cpp; 27 called helpers as
inline functions defined after their callers). The pools family (link/linkonce-pools), tu_119BC8/bis, blend, the
class_registry constructors (link/crt0-block) and the reverted VecCombo / operator* are not in it.

| measure | before | after | tool |
|---|---:|---:|---|
| objects split by other objects' code (TU check without `-ffunction-sections`) | 38 | 18 | `tools/link.py` |
| objects one run at their ELF address | 855 | 870 | `tools/link.py` |
| objects with junk.csv scraps inside / gap or padding differences | 33 / 4 | 36 / 6 | `tools/link.py` |
| differing bytes | 115 | 115 | `tools/link.py` |
| unplaced bytes (eopid_linkonce's .data, 12 more address references) | 544 | 592 | `tools/link.py` |

The 20 objects no longer split: po, step, emitter, grabspot, pipe, rail, glob, trail, bsp, mark, guardbhv, asega, xf,
cm, cplcy, screen, sound, rhythm, memcard, game. Five of them now show what the copies hid: junk.csv scraps inside
emitter (0x15BF38), cm (0x14E320, 0x14F3C0) and memcard (0x1AB748, 0x1ABFF8, 0x1AC848), and 8-byte alignment gaps
in bsp (0x118FA8, 0x1193D0) and guardbhv (0x178CF8), fix (3) above. Still split: the pools family's objects (bif, gc,
eval, frame, sidebag, ref, spliceutils, splicemap, spliceobj, proc, varb, proxy, basic), class_registry, tu_119BC8,
bis, blend, tu_1553F0. All 932 objects were rebuilt from scratch: their section bytes change only by the moved copies
(compared section by section against the objects of f674c97e) and their nm symbol sets only by the renames.

### Linkonce copies rehomed (link/linkonce-rest, 2026-11-06)

From integrate/round8 e344ef63 (522 objects whole, 6 in fallback for 18 linkonce copies kept as plain functions). The
details, the template shape and what each form needed are in tu-merging.md "The container family (link/linkonce-rest)".

| copies | home | form |
|---|---|---|
| tu_119BC8's 11 and bis's 0x2566B0 (0x2564E0..0x256AF0) | sly2/arrayv.h, instantiated by tu_119BC8.cpp | members of CDynArrayV<char *>, CHashIndex<char *> and CHash<char *, int> (vtables 0x2CA660, 0x2CA620; listener base 0x2CA640), explicitly instantiated; Resize an explicit specialization with weak/section attributes |
| blend 0x2581B8 (return 5, vtable 0x2CB5A8 slot 4) | app/eopid_linkonce.cpp | an inline function whose address eopid_linkonce takes, as for the GRABSPOT getters |
| bif's operator* 0x256D28, tu_1553F0's VecCombo 0x258480 | their files | plain definitions with `weak` + `section(".gnu.linkonce.t.<symbol>")` |
| class_registry's STEPGUARD_ctor, JT_ctor, MURRAY_ctor | class_registry.cpp | the same attributes on the three constructor definitions |

| measure | before | after | tool |
|---|---:|---:|---|
| objects whole / in per-function fallback | 522 / 6 | 528 / 0 | `tools/link.py` |
| fallback reasons | linkonce copies 6 | none (bis and tu_119BC8 showed junk.csv scraps once their copies left: emitted with `SLY2_SCRAP`) | `tools/link.py` |
| differing bytes | 0 | 0 | `tools/link.py` |

### Data order (data/bss-order on integrate/round5 before types/views-2/-3, 2026-11-05; superseded by Status)

Numbers of the branch alone: integrate/round5 (round 4 + build/rtti-palettes (`-fno-rtti`, palette section attributes)
+ data/address-names and data/interior-fields) + data/bss-order (data order inside the objects, tu-merging.md
"Phase 5: data order"), all measured with `-fno-rtti`. The integrate/round5 values before data/bss-order are given in brackets.
The `--tu-data` rows are the mode data/order-2 made the only one (the option is gone).

| measure | value | tool |
|---|---|---|
| functions matching | 6,662 / 6,662 (100%; 329 SDK sources carried over, verified on the Mac) | `tools/progress.py`, cache deleted first |
| touched files (141 of the two data branches) | functions 4,431 / 4,431, data symbols 1,423 / 1,423 MATCH | `tools/matchtool.py check` |
| undefined references not in config (209 merged objects + eopid_linkonce) | 443 (212 namespace views, 231 other; class_registry 192, jt 101) | scratch symcheck.py |
| duplicate strong definitions | 0 | scratch symcheck.py |
| relocations checked against the ELF | 27,044 OK, 0 mismatched | scratch relverify.py with config/link_aliases.txt |
| address names over the 932 link objects | 464 [468]; 218 alias-table lines | `tools/linkaliases.py` |
| ld.lld | 0 errors; 7,913 sections placed, 69 unplaced (2,432 bytes) | `tools/link.py` |
| .text equal | 1,416,389 / 1,416,688 (99.98%) [1,416,099]; from objects 1,281,745 / 1,282,044 (99.98%) | `tools/link.py` |
| .rodata equal | 303,037 / 304,356 (99.57%) [same]; from objects 22,245 / 23,564 (94.40%) | `tools/link.py` |
| .data equal | 199,086 / 199,088 (99.999%) [same]; from objects 33,991 / 33,993 (99.99%) | `tools/link.py` |
| differing bytes | 1,620 [1,910]: .rodata layout 1,263 (the `-fdata-sections` artifact, below), .text %hi/%lo to those 299 [589], .rodata no relocation 53 (+3 R_MIPS_32), .data 2 | `tools/link.py` |
| config names at their config address | 7,282 / 7,282 [7,242 / 7,273; wrong .bss 29, .sbss 2] | `tools/link.py` (list: build/link/wrongaddr.txt) |
| `--tu-data` (real-link data layout): .text / .rodata / .data equal | 1,415,300 (99.90%) [1,413,936] / 301,625 (99.10%) [298,405] / 198,222 (99.57%) [197,806]; from objects .rodata 21,217 / 23,948 (88.60%) [12,157 / 18,108], .data 29,066 / 29,932 (97.11%) [28,634 / 29,916] | `tools/link.py --tu-data` |
| `--tu-data` differing bytes | 4,985 [9,985] | `tools/link.py --tu-data` |
| `--tu-data` config names at their config address | 6,933 / 7,282 (wrong .rodata 46, .data 303) [6,847 / 7,334; wrong .rodata 143, .bss 29, .data 313, .sbss 2] | `tools/link.py --tu-data` |
| docs links | 0 broken | `tools/docs/linkcheck.py` |

No source change was needed under `-fno-rtti`: the two data branches' code matches with the flag as it did without.

## What is left, in order

1. ~~Decide `-fno-rtti` (class 2)~~: done (build/rtti-palettes).
2. ~~Data order from source~~: done (data/bss-order and data/order-2, tu-merging.md "Phase 5" and "Phase 6";
   `-fdata-sections` dropped, one link mode). class_registry's literal pool and serialize's jump-table word: done
   (link/crt0-block; the link now differs from the ELF in no byte).
3. ~~Merge the crt0 block (class_registry + splice_thunks + splicewrap) into its one TU~~ (done, link/crt0-block).
4. ~~A real-link mode~~ (done, link/real-link: one section per object, per-function fallback). The rest is "End
   state" below: ~~the 6 fallback objects~~ (none since link/linkonce-rest), the data no object defines, the SDK
   objects.

## End state

The goal: a link that reproduces SCUS_973.16 byte for byte the way the original build did: every object compiled
once, its sections whole, the linkonce sections where the linker puts them, and no blob except what genuinely is
not source (the IOP modules, the VU microcode). Planned on link/real-link, counts updated on integrate/round8; three
work lists stand between it and today's link.

### 1. Drop the per-function fallback (6 objects; 0 since link/linkonce-rest)

build/link/tutext.txt is the list; when it is empty, every object links whole, build/link/obj/ is not needed and
`--per-function` can go.

| reason | objects | branch |
|---|---:|---|
| ~~linkonce copies kept as plain functions (18 functions left of 127: tu_119BC8 11 and bis 1, the container family, mostly reached only through vtables; class_registry's STEPGUARD_ctor/JT_ctor/MURRAY_ctor; blend 0x2581B8 (vtable-only); bif's `operator*` and tu_1553F0's VecCombo, reverted on link/linkonce-misc)~~ | ~~6~~ 0 | done: the Splice GC pools (58, link/linkonce-pools), four more families (51, link/linkonce-misc), the last 18 (link/linkonce-rest, "Linkonce copies rehomed"); `--per-function` and build/link/obj/ are no longer needed for the link |
| ~~junk.csv dead-code scraps inside the object's range~~ | ~~33~~ 0 | done: link/junk-padding (`SLY2_SCRAP`), ref and splicemap on integrate/round8 |
| ~~scraps and 8 zero bytes before a function~~ | ~~3 (language, mat, joy)~~ 0 | done: link/junk-padding (nop scraps, not alignment) |
| ~~8 zero bytes before a function~~ | ~~1 (so 0x1F6A68)~~ 0 | done: link/junk-padding |
| ~~code the ELF does not have (grab.cpp's local class constructor/destructor, func_00192A28's)~~ | ~~1~~ 0 | done: link/junk-padding ("grab") |

The classification is computed from the TU objects alone, so each branch that lands lowers the count with no change
to the tool. Check per branch: fallback count, still 0 differing bytes and 0 ld errors.

### 2. Data no object defines (the data work list)

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
| tu_1147D8's built-in font | 132,464 | 0x27A250..0x29A7C0 | tu_1147D8 (head of its `.rodata`, before its palette D_0029A7C0) | font header {-1, 0x00290A19, 1.0f, 163} and 163 8-byte glyph records (0x27A250..0x27A778), the CLUT header D_0027A778 (16 colours: the palette D_0029A7C0), the 512x512 4-bit bitmap 0x27A790..0x29A790 (131,072 bytes, mostly 0xFF: these were "the tables filled with 0xFF"), the BMP header D_0029A790 and a font descriptor {header, glyphs, &D_002DD520 BMP, &D_002DD500 CLUT} at 0x29A7A8 (.data 0x2BE320 points at it). The structured parts are about 1.4 KB; the bitmap is an image and needs a decision on how image assets are kept in source |
| registry vtables | 100,312 at link/real-link; 35,272 left after data/vtables (174 vtables, 65,040 bytes, emitted by their key TUs), 24,944 after data/vtables-2 (44 more, 10,328 bytes), 24,864 after integrate/round10 (blend's AC5/ACG2, 80 bytes) | the six gaps of link/real-link (0x2C46E8 24,168; 0x2CA5E0 6,184; 0x2CBE38 9,808; 0x2CE4A0 4,200; 0x2CF520 18,432; 0x2D3FA0 37,520); left: the SDK `.data` at the head of the first gap (0x2C46E8.., newlib's reent data, libkernel's tables) and the linkonce area's vtables no object emits | left: 62 vtables without a config name (about 7,950 bytes); data/address-names-2 emitted 57 of them (tu-merging.md "Vtables emitted (data/address-names-2)"); left: EMITTER's and EXPLO's template-instance tables 0x2CE680 and 0x2CED18 (848 bytes) | real classes with the ELF's layout and a key function in each key TU, as data/vtables-2 did (tu-merging.md "Vtables emitted, continued") |
| palette TUs' tables | 33,024 | 0x29A800 | tail of tu_1147D8 / head of tu_114AE0 | the bitmap/glyph tables after D_0029A7C0 (defined) and before D_002A2900 |
| SDK `.rodata` | 10,084 | 0x2AA100 | SDK objects (sceSif error strings, libgcc's `__clz_tab`, libm tables, ...) | comes with the SDK objects (3 below) |
| small gaps | about 5,480 | ~60 gaps of 4..1,536 bytes (`.rodata` 2,600, `.data` 2,880) | neighbours or a TU between (largest: `.rodata` 1,536 between cheat and clock, `.data` 928 after emitter, 840 near tu_14C548) | unreferenced or unnamed statics of game TUs |

`.bss` (no bytes, but an address assignment each; all but the SDK tail, the `.sbss` commons and three small gaps
defined on data/bss-gaps): 400,080 from 0x3E1C70 (hud.cpp's buffers, D_003E1C70..D_003E21A0
only hud references, then the `.bss` of the TUs after it up to 0x4421A0); 60,000 at the end from 0x4437A0 (989snd
D_00443800.., libkernel's `_sif_*`, libgcc's `objects`, then the template statics of CSlotPool/CSpliceHeap at
0x4520A8.., which the original emitted as commons); 21,536 at 0x3D4A50 (D_003D4A50/54/60, the record many TUs
reference); 4,416 at 0x2E0D00 (D_002E0D00, D_002E1E00); about 1,800 in small gaps.

Order of work: ~~the crt0_splicewrap tables~~ (done, data/crt0-tables: 155,960 bytes), then the vtables (100,312,
needs the key TUs' first virtuals; done in data/vtables, data/vtables-2 and integrate/round10 for every vtable with a config name), tu_1147D8's font (its bitmap waits for the asset decision), then the rest by TU. Each step lowers "data no object defines" and the list
of names link.py binds to absolute addresses (build/link/absolute.txt; "Absolute symbols" above).

### 3. SDK objects instead of blobs

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

After 1-3: the IOP modules (`.irx`, 474,176) and the VU microcode (`.vutext`, 34,112) stay binary inputs; the 4
bytes of `func_00100000` need a source. Then every object section sits where the linker puts it, and the
address-ordered list in build/link/sly2.ld is the object command line: link.py can write the script as the
original's (`.text : { *(.text) *(.gnu.linkonce.t.*) }`, `.rodata : { *(.rodata.early) *(.rodata) }` and so on, objects
on the command line in TU order) and keep the address placement as the check. The 7 sections out of `.text` order
(Status) are what such a script needs special rules for, or source changes.

