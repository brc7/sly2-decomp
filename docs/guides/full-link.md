# Full PS2 link (sketch)

What it takes to link the matched sources back into SCUS_973.16, and how far the repo is: the current numbers are in
"Status" right below; the sections after it keep each branch's own measurements as history (integrate/round3: link
round 3 and data phase 2 — branch data/phase-3, the guide counts the first data pass as phase 1 — 2026-11-05).
tools/link.py links the image (since link/real-link a real link: one section per object, see "Real link
(link/real-link)"; the per-function prototype it grew from is "Prototype link"); what is left before it reproduces
the ELF the way the original build did is "End state" at the end. The symbol side is in
[tu-merging.md, "Linkage of namespace-scoped declarations"](tu-merging.md#linkage-of-namespace-scoped-declarations).

## Status (integrate/round8, 2026-11-06)

Authoritative numbers for integrate/round8: main f674c97e + link/real-link (with link/crt0-block: the crt0_splicewrap
TU as one object, the real-link mode as tools/link.py's default) + link/junk-padding (the junk.csv scraps as
`SLY2_SCRAP` in their sources, "Junk scraps and padding") + link/linkonce-pools (the Splice GC pool family as
`CGcObj<T>`/`CRefPtr<T>` instances, "Linkonce pools") + link/linkonce-misc (51 more linkonce copies: emitter.h
templates, inline accessors emitted by eopid_linkonce.cpp, inline functions after their callers, "Linkonce copies
moved"), plus the scraps the linkonce branches uncovered, emitted with `SLY2_SCRAP` as junk-padding does: ref 0x129D50,
splicemap 0x1FFA68, cm 0x14E320/0x14F3C0, emitter 0x15BF38, memcard 0x1AB748/0x1ABFF8 (2)/0x1AC848, bsp 0x1193D0 and
the nop units bsp 0x118FA8 and guardbhv 0x178CF8 (two new junk.csv rows; 80 rows are now emitted by sources). Values
of `python3 tools/link.py` (real link) from freshly built objects (build/link/obj and obj-tu deleted first);
link/real-link's values in brackets where different.

| measure | value | tool |
|---|---|---|
| functions matching | 6,662 / 6,662 (100%; 857 sources, 329 SDK sources carried over, verified on the Mac; config/matched.csv content unchanged by the run) | `tools/progress.py`, cache deleted first |
| files the branches touched, and every includer of a touched header (gc.h, basic.h, scrap.h, pair.h, varb.h, cframe.h, slotheap.h, classes.h, cm.h, emitter.h, so.h, step.h, xfm.h) or `.inc` fragment: 210 sources, every game object | functions 6,016 / 6,016, data symbols 1,555 / 1,555 MATCH | `tools/matchtool.py check` |
| nm symbol sets (those 210 objects; the 31 splice_thunks objects of the other trees included) | every object's global symbols (defined and undefined) are the merge of the four branches' against main f674c97e (a symbol is in the merge iff a branch added it, or it was there and no branch removed it): 0 deviations | scratch nmN.py |
| definitions per symbol | every global or weak function symbol is defined by exactly one object (cplcy's RcPtr_Assign 0x258040 and asega's func_002580A8, the inline counted-pointer stores of SND and the sidebag pointer, stay single definitions next to gc.cpp's CRefPtr<T> instances) | nm over the 210 objects |
| undefined references not in config (210 merged objects) | 41 [35] (link/linkonce-pools' value: the 9 view-destructor references bound by alias lines added, the 3 template members of proxy/spliceobj resolved) | scratch symcheck.py |
| duplicate strong definitions | 0 | scratch symcheck.py |
| relocations checked against the ELF | 29,602 [29,603] OK, 0 mismatched | scratch relverify.py with config/link_aliases.txt |
| link objects | 528: 522 whole (one section each), 6 in per-function fallback [452 / 76] | `tools/link.py` |
| fallback, by reason (build/link/tutext.txt) | linkonce copies kept as plain functions: 6 objects, 18 functions: tu_119BC8 11, class_registry's 3 constructors (STEPGUARD_ctor, JT_ctor, MURRAY_ctor), bis 0x2566B0, blend 0x2581B8, bif's `operator*` and tu_1553F0's VecCombo (tu-merging.md "Linkonce copies by family"); none for junk scraps, padding or grab [76: linkonce 38, scraps 33, scraps and padding 3, padding 1, grab 1] | `tools/link.py` |
| ld.lld | 0 errors; 2,019 [4,673] sections placed, 4 unplaced (464 bytes: eopid_linkonce's `.data` 292 and an SDK object's 100, bq's `__17CQueueInputMemoryPUci` and tn's `_vt$7CMPART2` linkonce copies) [6, 544], 2 `.bss` overlapping another bound to their address (tn, cplcy: 4 bytes each) | `tools/link.py` |
| address assignments in the script | 20, all in `.bss`/`.sbss`: gaps no object defines, 487,824 bytes | `tools/link.py` |
| differing bytes | 0; .text, .rodata, .data, .vutext, .irx equal; .text from objects 1,295,772 [1,294,948] | `tools/link.py` |
| blob bytes | 1,066,256 [1,067,080]: IOP modules 474,176; data no object defines 437,056; text of SDK sources that do not build here 118,224; VU microcode 34,112; junk.csv 1,620 [2,124] (the rows at TU boundaries, in the SDK and inside the 6 fallback objects); padding / not in config 1,064 [1,384]; `func_00100000` 4 | `tools/link.py` |
| config names at their config address | 7,402 / 7,402 | `tools/link.py` |
| docs links | 0 broken | `tools/docs/linkcheck.py` |

Merge conflicts. junk-padding and linkonce-pools: only the guides (full-link.md: link/real-link's Status, "What is
left" and "End state" kept, the branches' sections added after "Code order"); the config files, tu-merging.md and the
sources two branches touched (sm, so, lo; serialize) merged without conflict. Measured between the merges: 27
fallback objects after linkonce-pools. linkonce-misc: the config rows of the linkonce area (functions.csv,
matched.csv, symbol_addrs.txt, units.csv, core-library.md: adjacent rows, linkonce-pools' names for the pool rows,
linkonce-misc's for its own), basic.cpp (both branches removed a copy: the GC helpers to gc.cpp, the EMITTER getter
to emitter.cpp), full-link.md and tu-merging.md (both sections kept; the family table with linkonce-misc's column).
No branch touched tools/link.py; real-link's classification gives the new verdicts unchanged.

### Status (data/crt0-tables on link/real-link, 2026-11-06; superseded above)

data/crt0-tables (from link/real-link 1a662931): crt0_splicewrap's Splice option tables are source
(src/game/app/class_registry_options.inc, included in class_registry.cpp's data part; tu-merging.md "The
crt0_splicewrap tables (data/crt0-tables)"). Values of `python3 tools/link.py`; link/real-link's in brackets.

| measure | value | tool |
|---|---|---|
| differing bytes | 0 [0]; `.rodata` from objects 126,184 [28,184], `.data` from objects 95,896 [37,936] | `tools/link.py` |
| blob bytes | 911,120 [1,067,080]: data no object defines 281,096 [437,056] (-155,960: `.rodata` 0x262380..0x27A250 98,000, `.data` 0x2AC880..0x2BAAE8 57,960); the rest unchanged | `tools/link.py` |
| object sections placed | 4,674 [4,673]: class_registry's `.rodata.early` at 0x262380 | `tools/link.py` |
| config names at their config address | 7,605 / 7,605 [7,402]: the 203 new data symbols | `tools/link.py` |
| class_registry.cpp | 828 functions and 204 data symbols MATCH [828 and 1] | `tools/matchtool.py check` |
| symbols (class_registry, basic) | class_registry defines D_002AC880 (`.data`) and 202 `.rodata.early` objects and references 282 more functions (all config names); basic references D_0027A108 instead of D_0027A10C; nothing else changed | scratch nmcmp.sh / symcheck.py |
| relocations | class_registry + basic: 3,758 OK, 0 mismatched; every data pointer of the tables also compared by the link (0 differing bytes) | scratch relverify.py |
| everything else | as link/real-link below (528 objects, 76 in fallback, 0 ld errors, 20 address assignments) | `tools/link.py` |
| progress | 6,662 / 6,662 (the 329 SDK sources that do not build here carried over); config/matched.csv unchanged | `JOBS=3 tools/progress.py` (cache deleted) |
| docs links | 0 broken | `tools/docs/linkcheck.py` |

The rest of the old "crt0_splicewrap `.rodata`" gap, 0x27A250..0x29A7C0 (132,464 bytes), is not crt0_splicewrap's:
it is tu_1147D8's built-in font (data work list below).

### Status (data/vtables on link/real-link, 2026-11-06; superseded above)

data/vtables (from link/real-link 1a662931): the registry classes' vtables are emitted by their key TUs
([tu-merging.md, "Vtables emitted (data/vtables)"](tu-merging.md#vtables-emitted-datavtables)). link/real-link's
table below holds, with these changes:

| measure | value | tool |
|---|---|---|
| vtables from objects | 182 (`.gnu.linkonce.d._vt$*`): 174 new in 81 key TUs (65,040 bytes; 10 secondary grab-spot tables among them), 8 before (heap, bis, bq x2, dmas, gifs, missile's ACCMISS, tn's CMPART2, the last unplaced as before) | `tools/link.py`, scratch vtcheck.py |
| ld.lld | 0 errors; 4,847 sections placed (4,673 + 174), 6 unplaced (544 bytes, unchanged) | `tools/link.py` |
| differing bytes | 0; `.data` from objects 102,976 (37,936 before) | `tools/link.py` |
| blob bytes | 1,002,040 (1,067,080): data no object defines 372,016 (437,056) | `tools/link.py` |
| config names at their config address | 7,621 / 7,621 | `tools/link.py` |
| registry vtables not emitted | 17 config names: the 13 whose key TU is class_registry (0x2C8940..0x2CA4D0), GOMER, exit_warp's WARP/EXIT/CID41 | tu-merging.md |
| symcheck / relverify | unchanged against the branch start (35 unresolved, 0 duplicate strong globals) / 37,168 relocations OK, 0 mismatched | scratch symcheck.py, relverify.py |
| config/link_aliases.txt | 42 lines, unchanged (the 21 vtable lines: act.cpp's curve classes, hud's and memcard's local classes) | — |

### Status (link/real-link, 2026-11-06; superseded above)

link/real-link (from link/crt0-block adaf9219; no source changed): tools/link.py links as the original build did —
each object's `.text`, `.data`, `.rodata`, `.bss` whole and its `.gnu.linkonce.*` sections in the linkonce area —
and falls back to per-function placement only for the objects that are not contiguous in the ELF yet. Values of
`python3 tools/link.py` (default: real link) and, in brackets where different, `--per-function` (the old mode).

| measure | value | tool |
|---|---|---|
| link objects | 528: 452 whole (one section each), 76 in per-function fallback [528, all per function] | `tools/link.py` |
| fallback, by reason (build/link/tutext.txt) | linkonce copies kept as plain functions 38, junk.csv scraps inside 33, scraps and 8-byte padding 3 (language, mat, joy), padding only 1 (so), code the ELF does not have 1 (grab's local class): the same 76 objects as link/crt0-block's TU check | `tools/link.py` |
| ld.lld | 0 errors [0]; 4,673 [6,601] sections placed, 6 unplaced (544 bytes), 2 `.bss` overlapping another bound to their address (tn, cplcy: 4 bytes each) | `tools/link.py` |
| address assignments in the script | 20 [20], all in `.bss`/`.sbss`: gaps no object defines, 487,824 bytes; every other section lands where the linker puts it on its own (alignment padding from the linker, `=0` fill) | `tools/link.py` |
| differing bytes | 0 [0]; .text, .rodata, .data, .vutext, .irx equal; .text from objects 1,294,948 (1,282,044 at link/crt0-block: objects' own padding is no longer cut and blob-filled) | `tools/link.py` |
| blob bytes | 1,067,080 [same]: IOP modules 474,176; data no object defines 437,056; text of SDK sources that do not build here 118,224 (with 1,384 of padding between them); VU microcode 34,112; junk.csv 2,124; `func_00100000` 4 (link/crt0-block's accounting: 1,080,256) | `tools/link.py` |
| config names at their config address | 7,402 / 7,402 | `tools/link.py` |
| one object order for every section | the objects' `.data`/`.rodata`/`.bss` follow their `.text` order except 7 sections: the 3 `.rodata.early`, mpeg's and class_registry's `.rodata` (the first part of `.rodata`, tu-merging.md "Phase 5"), main's `.data.buildinfo`, tn's `.bss` (the bound overlap) | scratch order.py |
| tools/link.py | 640 -> 586 lines (real-link mode, the TU classification and `--sdk-only` added; the per-function TU check, byte-repeat binding, alignment lowering and owner listing gone) | `wc -l` |
| docs links | 0 broken | `tools/docs/linkcheck.py` |

### Status (link/crt0-block on integrate/round7, 2026-11-05; superseded above)

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
outside the segments; `.irx` at 0x472200 in its own PT_LOAD; the output of `tools/linkaliases.py` included; the
key-TU extras discarded.

## Real link (link/real-link, 2026-11-06)

`python3 tools/link.py [--no-compile] [--per-function]` or `--sdk-only`; tools/verify.sh runs it without options. Needs
the same tools as before (`ld.lld`, `llvm-objcopy`, disc/SCUS_973.16; "Prototype link" below says where they come
from). A full run is about 5 min here (528 sources, 2 cores, plus the fallback objects); `--no-compile` about 5 s.

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
   state" below: the 6 fallback objects (the last 18 linkonce copies; link/junk-padding, link/linkonce-pools and
   link/linkonce-misc are merged), the data no object defines, the SDK objects.

## End state

The goal: a link that reproduces SCUS_973.16 byte for byte the way the original build did: every object compiled
once, its sections whole, the linkonce sections where the linker puts them, and no blob except what genuinely is
not source (the IOP modules, the VU microcode). Planned on link/real-link, counts updated on integrate/round8; three
work lists stand between it and today's link.

### 1. Drop the per-function fallback (6 objects)

build/link/tutext.txt is the list; when it is empty, every object links whole, build/link/obj/ is not needed and
`--per-function` can go.

| reason | objects | branch |
|---|---:|---|
| linkonce copies kept as plain functions (18 functions left of 127: tu_119BC8 11 and bis 1, the container family, mostly reached only through vtables; class_registry's STEPGUARD_ctor/JT_ctor/MURRAY_ctor; blend 0x2581B8 (vtable-only); bif's `operator*` and tu_1553F0's VecCombo, reverted on link/linkonce-misc) | 6 | back to templates/inline functions so they are emitted as `.gnu.linkonce.t.*`; the Splice GC pools (58, link/linkonce-pools) and four more families (51, link/linkonce-misc) are done |
| ~~junk.csv dead-code scraps inside the object's range~~ | ~~33~~ 0 | done: link/junk-padding (`SLY2_SCRAP`), ref and splicemap on integrate/round8 |
| ~~scraps and 8 zero bytes before a function~~ | ~~3 (language, mat, joy)~~ 0 | done: link/junk-padding (nop scraps, not alignment) |
| ~~8 zero bytes before a function~~ | ~~1 (so 0x1F6A68)~~ 0 | done: link/junk-padding |
| ~~code the ELF does not have (grab.cpp's local class constructor/destructor, func_00192A28's)~~ | ~~1~~ 0 | done: link/junk-padding ("grab") |

The classification is computed from the TU objects alone, so each branch that lands lowers the count with no change
to the tool. Check per branch: fallback count, still 0 differing bytes and 0 ld errors.

### 2. Data no object defines (the data work list)

437,056 blob bytes in `.rodata`/`.data` at link/real-link (281,096 since data/crt0-tables) and 487,824 bytes of
`.bss`/`.sbss` gaps. Owner = the TU whose place in the
layout the gap takes: the original link put every object's sections in one command-line order (the order check in
Status: all but 7 sections follow `.text` order), so a gap between objects P and N belongs to P's tail, N's head or a
TU between them in `.text` order that places nothing in that section. Measured with a scratch script over
link/real-link's placement (gaps from the uncut section sizes: 437,324 bytes):

| block | bytes | address | owner | what it takes |
|---|---:|---|---|---|
| ~~crt0_splicewrap `.rodata`~~ | ~~98,000~~ | 0x262380..0x27A250 | class_registry.cpp | done (data/crt0-tables): the 199 class option arrays, g_mpcidarysplopt, HandleBasicSpliceEvent's event parameter types |
| ~~crt0_splicewrap `.data`~~ | ~~57,960~~ | 0x2AC880..0x2BAAE8 | class_registry.cpp | done (data/crt0-tables): g_aeopid, the 1,449 option records |
| tu_1147D8's built-in font | 132,464 | 0x27A250..0x29A7C0 | tu_1147D8 (head of its `.rodata`, before its palette D_0029A7C0) | font header {-1, 0x00290A19, 1.0f, 163} and 163 8-byte glyph records (0x27A250..0x27A778), the CLUT header D_0027A778 (16 colours: the palette D_0029A7C0), the 512x512 4-bit bitmap 0x27A790..0x29A790 (131,072 bytes, mostly 0xFF: these were "the tables filled with 0xFF"), the BMP header D_0029A790 and a font descriptor {header, glyphs, &D_002DD520 BMP, &D_002DD500 CLUT} at 0x29A7A8 (.data 0x2BE320 points at it). The structured parts are about 1.4 KB; the bitmap is an image and needs a decision on how image assets are kept in source |
| registry vtables | 100,312 at link/real-link; 35,272 left after data/vtables (174 vtables, 65,040 bytes, emitted by their key TUs) | the six gaps of link/real-link (0x2C46E8 24,168; 0x2CA5E0 6,184; 0x2CBE38 9,808; 0x2CE4A0 4,200; 0x2CF520 18,432; 0x2D3FA0 37,520); left: the SDK `.data` at the head of the first gap (0x2C46E8.., newlib's reent data, libkernel's tables) and the linkonce area's vtables no object emits | left: class_registry (13 vtables at 0x2C8940..0x2CA4D0), gomer.cpp, exit_warp.cpp, act.cpp's curve classes and the other unnamed vtables (GRABSPOT's at 0x2D28D8, grabspot.cpp) | class_registry defines one getter of each of its 13 classes as the member; GOMER's slot 7 signature; exit_warp's bis.h/vec.h clash with basic.h; names for the unnamed classes |
| palette TUs' tables | 33,024 | 0x29A800 | tail of tu_1147D8 / head of tu_114AE0 | the bitmap/glyph tables after D_0029A7C0 (defined) and before D_002A2900 |
| SDK `.rodata` | 10,084 | 0x2AA100 | SDK objects (sceSif error strings, libgcc's `__clz_tab`, libm tables, ...) | comes with the SDK objects (3 below) |
| small gaps | about 5,480 | ~60 gaps of 4..1,536 bytes (`.rodata` 2,600, `.data` 2,880) | neighbours or a TU between (largest: `.rodata` 1,536 between cheat and clock, `.data` 928 after emitter, 840 near tu_14C548) | unreferenced or unnamed statics of game TUs |

`.bss` (no bytes, but an address assignment each): 400,080 from 0x3E1C70 (hud.cpp's buffers, D_003E1C70..D_003E21A0
only hud references, then the `.bss` of the TUs after it up to 0x4421A0); 60,000 at the end from 0x4437A0 (989snd
D_00443800.., libkernel's `_sif_*`, libgcc's `objects`, then the template statics of CSlotPool/CSpliceHeap at
0x4520A8.., which the original emitted as commons); 21,536 at 0x3D4A50 (D_003D4A50/54/60, the record many TUs
reference); 4,416 at 0x2E0D00 (D_002E0D00, D_002E1E00); about 1,800 in small gaps.

Order of work: ~~the crt0_splicewrap tables~~ (done, data/crt0-tables: 155,960 bytes), then the vtables (100,312,
needs the key TUs' first virtuals; done but for 17 in data/vtables), tu_1147D8's font (its bitmap waits for the asset decision), then the rest by TU. Each step lowers "data no object defines" and the absolute-address list
of tools/linkaliases.py.

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

