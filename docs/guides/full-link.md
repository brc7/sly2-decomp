# Full PS2 link (sketch)

What it takes to link the matched sources back into SCUS_973.16, and how far the repo is: the current numbers are in
"Status" right below; the sections after it keep each branch's own measurements as history (integrate/round3: link
round 3 and data phase 2 — branch data/phase-3, the guide counts the first data pass as phase 1 — 2026-11-05). A prototype link exists (tools/link.py, branch link/prototype; see "Prototype link" below). The symbol side is in
[tu-merging.md, "Linkage of namespace-scoped declarations"](tu-merging.md#linkage-of-namespace-scoped-declarations).

## Status (link/crt0-block on integrate/round7, 2026-11-05)

Authoritative numbers for link/crt0-block (main f674c97e = integrate/round7 + tooling, plus this branch): the
crt0_splicewrap TU as one object (src/game/app/class_registry.cpp includes the 373 generated wrappers and absorbs the
31 splice_thunks files, tu-merging.md "The crt0_splicewrap TU (link/crt0-block)") and two source fixes the full link
exposed (serialize's PpairSerializeIn case labels, func_001047A0's OTYP array). The link compares equal to the ELF
in every section. The integrate/round7 values are given in brackets where they differ.

| measure | value | tool |
|---|---|---|
| functions matching | 6,662 / 6,662 (100%; 857 sources [1,261], 329 SDK sources carried over, verified on the Mac; config/matched.csv unchanged by the run) | `tools/progress.py`, cache deleted first |
| files link/crt0-block touched | class_registry.cpp 828 functions + D_002BAAE8, serialize.cpp 3 functions: all MATCH | `tools/matchtool.py check` |
| nm symbol sets | class_registry's object: the union of the 405 objects it replaces (old class_registry, 373 wrappers, 31 thunks) except 4 references renamed to their config names (InitTag__4CRefi, `__as__4CRefPv`, `__as__4CRefb`, `__as__4CRefi` -> SetTag__4CRef4TAGK, CRef_SetBasic, CRef_SetBool, SetS32__4CRefi) and the `_GLOBAL_$I$` suffix; serialize unchanged | scratch nm comparison |
| link objects | 528 [932] (404 sources fewer: the wrappers are `.inc` fragments, splice_thunks/ is gone) | `tools/link.py` |
| undefined references not in config (210 merged objects) | 35 (unchanged) | scratch symcheck.py |
| duplicate strong definitions | 0 | scratch symcheck.py |
| relocations checked against the ELF | 29,603 [27,231] OK, 0 mismatched (more checked because the wrappers and thunks are now in a merged object) | scratch relverify.py with config/link_aliases.txt |
| address names over the link objects | 303: 255 absolute, 48 aliases; 27 alias-table entries (unchanged) | `tools/linkaliases.py` |
| ld.lld | 0 errors; 6,601 [6,760] sections placed, 6 unplaced (544 bytes); 2 [65] sections overlap another but repeat its ELF bytes | `tools/link.py` |
| .text equal | 1,416,688 / 1,416,688 (100%) [1,416,576]; from objects 1,282,044 / 1,282,044 | `tools/link.py` |
| .rodata equal | 304,356 / 304,356 [304,353]; from objects 28,184 / 28,184 [27,829 / 27,832] | `tools/link.py` |
| .data equal | 199,088 / 199,088 (100%); from objects 37,936 / 37,936 | `tools/link.py` |
| differing bytes | 0 [115: .text %hi/%lo to class_registry's literal pool 112, serialize's jump-table word 3] | `tools/link.py` |
| config names at their config address | 7,402 / 7,402 | `tools/link.py` (list: build/link/wrongaddr.txt) |
| TU check with `-ffunction-sections` | 489 objects one section, 0 in another order, 38 split by other objects' code (class_registry by its 3 linkonce constructors only, [24 runs]) | `tools/link.py` |
| TU check without `-ffunction-sections` | 451 objects one run at their ELF address; 76 not: split 38, junk.csv dead code inside 33, gap/padding 4, unplaced section 1. class_registry: all its other 824 `.text` sections one run at 0x100218 (scratch check skipping STEPGUARD_ctor/JT_ctor/MURRAY_ctor, which link/linkonce-misc turns into inlines) | `tools/link.py` |
| interior base+offset macros | 19 in 10 files (unchanged) | grep `(char *)base_D_` |
| docs links | 0 broken | `tools/docs/linkcheck.py` |

### Status (integrate/round7: data/order-2 + link/contiguous-tus, 2026-11-05; superseded above)

Authoritative numbers for integrate/round7: integrate/round6 1d4d7c44 (round 5 + types/views-3 + data/bss-order:
`-fno-rtti`, the registry classes and SW real, data order inside the objects) + data/order-2 (tu-merging.md, "Phase 6:
one link mode": tools/link.py links without `-fdata-sections`, the only mode) + link/contiguous-tus (the TU check
without `-ffunction-sections`, "Code order (link/contiguous-tus)" below). Numbers elsewhere in this guide that disagree
are earlier measurements (marked "superseded"); the integrate/round6 values (old `-fdata-sections` mode) are given in
brackets where they differ.

| measure | value | tool |
|---|---|---|
| functions matching | 6,662 / 6,662 (100%; 329 SDK sources carried over, verified on the Mac) | `tools/progress.py`, cache deleted first |
| files data/order-2 touched (66 sources; it touched no header) | functions 2,515 / 2,515, data symbols 1,035 / 1,035 MATCH | `tools/matchtool.py check` |
| nm symbol sets (all 932 link objects) | every object's global symbols (defined and undefined) are the three-way merge of the parents' (integrate/round6, data/order-2) against the merge base 0902c4cc: 0 deviations | scratch nm3.py |
| undefined references not in config (209 merged objects + eopid_linkonce) | 35 (21 namespace views, 19 of them act.cpp's; 14 other: `_gp` in 4 objects, from the linker script; `D_C0`; the `gcc2_compiled.` markers; `__builtin_new`; tn's `Init__5CPLCY`; draw's `g_pfrmOpen`; brx_sw's `_$_3OBJ`; proxy's and spliceobj's three template members) | scratch symcheck.py |
| duplicate strong definitions | 0 | scratch symcheck.py |
| relocations checked against the ELF | 27,231 [27,125] OK, 0 mismatched | scratch relverify.py with config/link_aliases.txt |
| address names over the 932 link objects | 303 [319]: 255 absolute [271], 48 aliases; 27 alias-table entries (config/link_aliases.txt unchanged) | `tools/linkaliases.py` |
| ld.lld | 0 errors; 6,760 sections placed, 6 unplaced (544 bytes) [7,914 / 66] | `tools/link.py` |
| .text equal | 1,416,576 / 1,416,688 (99.99%) [1,416,389]; from objects 1,281,932 / 1,282,044 | `tools/link.py` |
| .rodata equal | 304,353 / 304,356 [303,037]; from objects 27,829 / 27,832 [22,245 / 23,564] | `tools/link.py` |
| .data equal | 199,088 / 199,088 (100%) [199,086]; from objects 37,936 / 37,936 [34,631 / 34,633] | `tools/link.py` |
| differing bytes | 115 [1,620]: .text %hi/%lo to class_registry's literal pool 112 (its literals interleave with the splicewrap objects' in the ELF), .rodata 3 (a jump-table word of serialize's PpairSerializeIn) | `tools/link.py` |
| config names at their config address | 7,402 / 7,402 [7,283 / 7,283] | `tools/link.py` (list: build/link/wrongaddr.txt) |
| TU check with `-ffunction-sections` | 893 objects one section, 0 in another order, 38 split by other objects' code | `tools/link.py` |
| TU check without `-ffunction-sections` | 855 objects one run at their ELF address; 76 not: split 38, junk.csv dead code inside 33, gap/padding 4, unplaced section 1 (build/link/tutext.txt identical to link/contiguous-tus's) | `tools/link.py` |
| interior base+offset macros | 19 in 10 files (unchanged from integrate/round6) | grep `(char *)base_D_` |
| docs links | 0 broken | `tools/docs/linkcheck.py` |

Fixes the merges needed: data/order-2 (based on data/bss-order, before types/views-2/-3) conflicted in five sources,
tu_185130, noise, tu_14A008, tu_171F58 and throw: order-2 moved their `.data` definitions into one address-order block
and their FLT_MAX-style constants to `RO_ORDER` definitions, views-3 had renamed the files' private views (`LM` ->
`LMV`, `CLQ` -> `CLQV`) and replaced the g_sw/g_clock interior macros by typed fields. Resolution: order-2's layout,
spelled with views-3's view names and without the removed macros (also in the auto-merged `.data` blocks of noise,
tu_14A008 and tu_171F58, which still said `LM`). No other source change; every data/order-2 number carries over
except those the views branches moved (`.data` from objects, address names, sections placed). link/contiguous-tus
merged without conflict: its check hooks into `tu_check`, which order-2's single-mode link.py still runs, and gives
the same 76 verdicts.

## The target: the ELF's layout

Entry `_start` at 0x100008, `_gp` = 0x2E5270 (from `.reginfo`), two PT_LOAD segments:

| section | address | size | content | from where in a link |
|---|---|---:|---|---|
| .text | 0x100000 | 0x159DF0 | EE code | objects; 98.8% of the bytes are matched functions (functions.csv ranges of the matched.csv rows); the rest are 153 dead-code ranges in config/junk.csv (1,924 bytes), `func_00100000` (4 bytes before `_start`) and padding |
| .vutext | 0x259DF0 | 0x8540 | VU microcode, with 21 `.DVP.overlay..*` sections and `.DVP.ovlytab` describing the VU-memory overlays | not source: a blob from the ELF (or the original DVP assembler sources, which we do not have) |
| .rodata | 0x262380 | 0x4A4E4 | constants, strings, jump tables, vtables | objects (named data and anonymous literals) + unbuilt data |
| .vudata | 0x2AC864 | 0 | | |
| .data | 0x2AC880 | 0x309B0 | initialized data; starts with the crt0-block binding records (0x2AC880-0x2BAAE8, docs/architecture.md) | objects + unbuilt data |
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
outside the segments; `.irx` at 0x472200 in its own PT_LOAD; the output of `tools/linkaliases.py` included; the
key-TU extras discarded.

## Prototype link (link/prototype, 2026-11-05)

`python3 tools/link.py [--no-compile]` (one script plus the template tools/link.ld.in). Needs `ld.lld`
and `llvm-objcopy` (LLVM 18 here; no MIPS GNU binutils were installable in the workspace) and your disc/SCUS_973.16.
The script finds them via `$LLVM_BIN`, then PATH, then Homebrew's keg-only `llvm`/`lld` (macOS: `brew install llvm`;
if Homebrew's llvm aborts with `Library not loaded: .../libz3...dylib`, its build is stale against a newer z3: `brew upgrade llvm`).
Everything it writes goes to build/link/ (ignored): objects, build/link/blobs.o (byte ranges cut from your ELF), the
linker script sly2.ld, sly2.elf, ld.log, and the diagnostics overlaps.txt, conflicts.txt, unplaced.txt, wrongaddr.txt,
mismatch.txt. A full run is ~6 min of compiling (932 sources, 2 cores) plus ~20 s; `--no-compile` reuses the objects.

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
   .reginfo; tools/linkaliases.py's lines. Object fixes before linking (in build/link/fix/): llvm-objcopy rewrites
   each object (gcc 2.95's as puts local symbols after globals, which ld.lld rejects) and applies the
   `[source]` renames of config/link_aliases.txt; sections holding one symbol are cut to the symbol's size (gas pads
   each section to its alignment, which with one section per object would overlap the next object). Linkonce
   copies: the first placed copy is linked, the other copies' symbols are bound to its address. `--gc-sections` with
   every placed section `KEEP`: the key-TU extras (unreferenced vtables, typeinfo) drop out.
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
(the dead functions or a `.word` block) at their place.

**Fixes, in order:** (1) the linkonce copies back to templates/inline functions in headers (tu-merging.md "Inline
functions and linkonce copies": a non-`inline` function template is emitted per instance as linkonce and never
inlined into callers, which is why emitter.h's set kept matching; free inline functions would be inlined by callers
that now call them), one family at a time, renaming to the mangled names; (2) the crt0 block as one object (done on
link/crt0-block: the wrappers included as `.inc` fragments in address order, compiled as C++ with C linkage; their
code is unchanged); (3) the junk scraps and the 16-byte alignments. None was
done on this branch (timebox: diagnosis only).

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
   (link/crt0-block; the link now differs from the ELF in no byte). The `D_` names nobody defines stay the work
   list for the real link.
3. ~~Merge the crt0 block (class_registry + splice_thunks + splicewrap) into its one TU~~ (done, link/crt0-block),
   and turn the linkonce-area copies back into inline/template instances (link/linkonce-pools, -misc), so `.text` can link without `-ffunction-sections` ("Code order
   (link/contiguous-tus)": 38 objects split, all by linkonce copies, class_registry also by the crt0 block; 38
   more by junk scraps and alignment).
4. The SDK objects built on the Mac (118,224 blob bytes of .text today).
