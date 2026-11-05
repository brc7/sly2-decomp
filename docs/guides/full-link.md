# Full PS2 link (sketch)

What it takes to link the matched sources back into SCUS_973.16, and how far the repo is (link/round3,
2026-11-05). Nothing here is built yet: no linker script, no link step. The symbol side is in
[tu-merging.md, "Linkage of namespace-scoped declarations"](tu-merging.md#linkage-of-namespace-scoped-declarations).

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
| data nobody defines | - | - | - | 1,296 address names (`D_XXXXXXXX`), see below |

matched.csv has 6,663 rows (all `match`). Named data objects that the 209 merged objects define, by section (data
symbols with a known address, bytes): .data 466 objects / 0x38F5 (7.3% of the section), .rodata 100 / 0xC75 (1.0%,
plus the anonymous literals of every object), .sbss 3 / 0xC, .bss 140 / 0xE5BF4 (61.6%).

## Symbols (state after link round 3)

- 531 undefined references not in config remain in the 209 objects: 289 are listed in config/link_aliases.txt
  (vtables and destructors of namespace-scoped views, a few declarations source cannot bind) and 242 are referenced
  only from key-TU extras that the ELF does not have (unreferenced linkonce vtables and typeinfo functions). The extras
  must not be resolved but dropped: `--gc-sections`, or a `/DISCARD/` for those `.gnu.linkonce.*` sections.
- 1,860 references to 1,133 address names nobody defines. `tools/linkaliases.py <objects>` derives them: 24 `func_`
  names are config functions under another name (`func_002333B4 = memset;`, declared under the address name by 70 of
  the 209 sources) and become aliases; the rest (1,296 names with the alias targets) become absolute addresses: .data 697, .rodata 266, .bss 229,
  .sbss 31, .text 29, .vutext 14, .irx 3, and 26 small values (0x0..0x3AB0, VU-memory / scratchpad offsets).
- Two objects define different functions under the same file-scope destructor name (`_$_3SPM`, `_$_3OBJ`): per-object
  `objcopy --redefine-syms` files from `tools/linkaliases.py --redefine DIR`.
- 25,768 relocations agree with the ELF (field minus addend = the symbol's config address, through the alias table);
  2 do not because config/functions.csv and config/symbol_addrs.txt disagree on the libcdvd names at 0x242A20,
  0x242AB8 and 0x242AE0. A link resolves `sceCdGetError` / `sceCdGetDiskType` by whichever object defines them, so
  this has to be settled before the link.
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
   every `D_` defined by some object (today 1,296 are not), the TU order inside each output section reproduced from
   tools/layout/units.csv (functions) and the data addresses (the anonymous .rodata literals land wherever their
   object lands, so the order is checked by comparing the image, not by symbols), and the absolute address names
   replaced by symbols of the objects that define the data.

Linker script, from the layout above: `ENTRY(_start)`; `.text` at 0x100000 with the crt0 block first
(`func_00100000`, `_start`, ...), then the TUs in address order (linkonce copies homed by address, see
tu-merging.md "Inline functions and linkonce copies"); `.vutext` with the DVP overlay sections; `.rodata` at 0x262380;
`.data` at 0x2AC880; `.sbss` at 0x2DD280 with `_gp = 0x2E5270`; `.bss` at 0x2DD380; `.stack` (0x20000) at 0x452200
outside the segments; `.irx` at 0x472200 in its own PT_LOAD; the output of `tools/linkaliases.py` included; the
key-TU extras discarded.

## What is left, in order

1. Settle the libcdvd name inconsistency in config (functions.csv vs symbol_addrs.txt).
2. Run the symbol check over the per-function directories and the generated wrappers (70 references to look at).
3. Write the overlay-link script for the game objects and compare the image (blobs for everything else).
4. Data from source, TU by TU (the `D_` names are the work list), toward the real link.
