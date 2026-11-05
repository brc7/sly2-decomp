# Full PS2 link (sketch)

What it takes to link the matched sources back into SCUS_973.16, and how far the repo is: the current numbers are in
"Status" right below; the sections after it keep each branch's own measurements as history (integrate/round3: link
round 3 and data phase 2 — branch data/phase-3, the guide counts the first data pass as phase 1 — 2026-11-05). A prototype link exists (tools/link.py, branch link/prototype; see "Prototype link" below). The symbol side is in
[tu-merging.md, "Linkage of namespace-scoped declarations"](tu-merging.md#linkage-of-namespace-scoped-declarations).

## Status (integrate/round5 1bf80489, 2026-11-05)

Authoritative numbers for integrate/round5: round 4 + build/rtti-palettes (`-fno-rtti`, palette section attributes)
+ data/address-names and data/interior-fields, all measured with `-fno-rtti`. Numbers elsewhere in this guide that
disagree are earlier measurements (marked "superseded").

| measure | value | tool |
|---|---|---|
| functions matching | 6,662 / 6,662 (100%; 329 SDK sources carried over, verified on the Mac) | `tools/progress.py`, cache deleted first |
| touched files (141 of the two data branches) | functions 4,431 / 4,431, data symbols 1,423 / 1,423 MATCH | `tools/matchtool.py check` |
| undefined references not in config (209 merged objects + eopid_linkonce) | 443 (212 namespace views, 231 other; class_registry 192, jt 101) | scratch symcheck.py |
| duplicate strong definitions | 0 | scratch symcheck.py |
| relocations checked against the ELF | 27,044 OK, 0 mismatched | scratch relverify.py with config/link_aliases.txt |
| address names over the 932 link objects | 468 (466 absolute, 2 aliases); 218 alias-table lines | `tools/linkaliases.py` |
| ld.lld | 0 errors; 7,909 sections placed, 69 unplaced (2,432 bytes) | `tools/link.py` |
| .text equal | 1,416,099 / 1,416,688 (99.96%); from objects 1,281,455 / 1,282,044 (99.95%) | `tools/link.py` |
| .rodata equal | 303,037 / 304,356 (99.57%); from objects 22,245 / 23,564 (94.40%) | `tools/link.py` |
| .data equal | 199,086 / 199,088 (99.999%); from objects 33,979 / 33,981 (99.99%) | `tools/link.py` |
| differing bytes | 1,910: .rodata layout 1,263, .text %hi/%lo to those 589, .rodata no relocation 53 (+3 R_MIPS_32), .data 2 | `tools/link.py` |
| config names at their config address | 7,242 / 7,273 (wrong: .bss 29, .sbss 2) | `tools/link.py` |
| docs links | 0 broken | `tools/docs/linkcheck.py` |

No source change was needed under `-fno-rtti`: the two data branches' code matches with the flag as it did without.

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
| game per-function directories (air/, splice_thunks/, crt0/) | 76 | 76 | yes | not merged yet; 70 undefined references not in config (not analysed) |
| generated Splice wrappers (src/generated/splicewrap) | 373 | 373 | yes | the option/property binding thunks the crt0-block records point at |
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

`python3 tools/link.py [--no-compile] [--tu-data]` (one script plus the template tools/link.ld.in). Needs `ld.lld`
and `llvm-objcopy` (LLVM 18 here; no MIPS GNU binutils were installable in the workspace) and your disc/SCUS_973.16.
Everything it writes goes to build/link/ (ignored): objects, build/link/blobs.o (byte ranges cut from your ELF), the
linker script sly2.ld, sly2.elf, ld.log, and the diagnostics overlaps.txt, conflicts.txt, unplaced.txt,
mismatch.txt. A full run is ~6 min of compiling (932 sources, 2 cores) plus ~20 s; `--no-compile` reuses the objects.

What it does:

1. **Objects.** Every source of config/matched.csv that builds here, through matchtool's `compile_src` (same flags,
   per-file overrides and relative path), plus `-ffunction-sections -fdata-sections`. Checked: with those flags all
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

With `--tu-data` (no `-fdata-sections`: each object's .data/.rodata laid out in source order, as a real link would):
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

### Remaining mismatch classes (default mode, 2,068 bytes before build/rtti-palettes, 2,020 after; 1,910 on integrate/round5)

1. **Object sections whose internal layout differs from the ELF** (1,296 .rodata bytes, plus 697 .text bytes of
   %hi/%lo referencing them). Anonymous literal pools (18 objects; 15 of them interleave with the object's named
   .rodata in the ELF, so in this mode it is an artifact of `-fdata-sections`, in `--tu-data` mode a source data-order
   issue: memcard, guardbhv, text, screen, air, stepplayer, jt, prize ...) and file-level .bss whose order differs
   from the ELF (`-fdata-sections` does not split .bss in g++ 2.95): gs, joy, dl, shd, screen, game, sprbuf
   (build/link/conflicts.txt lists the competing addresses; mpeg, memcard and sw fixed on data/interior-fields: g++
   emits objects with a constructor at their definition and the rest in first-declaration order, tu-merging.md
   "Phase 4: interior fields").
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
   (eval 6, sidebag 5, frame 5, ref 4, basic 4, emitter 4, bif 4, spliceobj 3, splicemap 3 ...).

## What is left, in order

1. ~~Decide `-fno-rtti` (class 2)~~: done (build/rtti-palettes).
2. Data order from source, TU by TU, measured with `--tu-data` (conflicts.txt, mismatch.txt); the `D_` names nobody
   defines stay the work list for the real link.
3. Merge the crt0 block (class_registry + splice_thunks + splicewrap) into its one TU, and turn the linkonce-area
   copies back into inline/template instances, so `.text` can link without `-ffunction-sections`.
4. The SDK objects built on the Mac (118,224 blob bytes of .text today).
