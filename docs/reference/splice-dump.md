# Dumping and re-encoding Splice scripts

`tools/splice/dump.py` extracts every level's compiled Splice scripts from **your own disc image** and prints them as
readable Scheme, one file per level, with a JSON AST for tooling. `tools/splice/encode.py` compiles a listing (or the
JSON) back to the serialized form and checks it against the disc byte for byte. Both are stdlib-only Python 3.

The output is game content. Write it to a folder outside the repository and never commit it; the tools warn when the
output folder is inside the repo. The language itself is described in [splice-language.md](splice-language.md), the
VM in [systems/splice-scripting.md](../systems/splice-scripting.md), per-class options in
[splice-options.md](splice-options.md).

## 1. Running it

On the Mac, with the ISO in `/Applications/PCSX2/games/` (substitute your image's file name):

```sh
cd <this repository>
ISO="/Applications/PCSX2/games/Sly 2 - Band of Thieves (USA).iso"
OUT=~/Documents/code/src/sly2/research/splice

python3 tools/splice/dump.py "$ISO" --list                    # the level table: index, name, file location
python3 tools/splice/dump.py "$ISO" 2 > "$OUT/paris.scm"      # one level to stdout (2 = Paris hub)
python3 tools/splice/dump.py "$ISO" all -o "$OUT" --json --stats   # every level: L02_nightclub.scm + .json ...
python3 tools/splice/encode.py "$ISO" 2 "$OUT/L02_nightclub.scm" --check   # text -> bytes, compare with the disc
python3 tools/splice/test_splice.py                           # self-test on synthetic data, no disc needed
```

`dump.py DISC LEVEL... [-o DIR] [--json] [--raw] [--width N] [--stats]`:

| Argument | Meaning |
|---|---|
| `DISC` | a 2048-byte-sector ISO, or a directory of sparse extents named `<lba>_<sectors>.bin` (the port's sparse format) |
| `LEVEL` | level-table indices, ranges (`2-7`) or `all`; no level, or `--list`, prints the table |
| `-o DIR` | one `L<nn>_<world>[_ep<k>].scm` per level (and `.json` with `--json`); without it one level goes to stdout |
| `--json` | the AST as JSON (with `-o` in addition to the listing; without `-o` instead of it) |
| `--raw` | no names: symbols `sN`, builtins `#bif:N`, no annotations (still re-encodable) |
| `--stats` | validation report on stderr (section 5) |

Level indices are those of the level table: 0 prologue, 1 front end, 2 Paris hub, 3-7 its episodes, 8 the next hub,
and so on (`--list` names each by world and episode; 18 is an empty record). A Paris dump takes a few seconds; most of
it is LZSS decompression.

## 2. Reading a listing

```text
;;; ==== world entry 12 <name>  root oid N  copies 1  script table 0x...  (3 entries)
;; -- entry 2 of <name> [0x..., 48 bytes]  bound to: TASK oid 1234
(begin                                     ; an invented example, not from the game
  (define evt-activate
    (lambda (self)
      (seto! self 1300 #|sidebag|# 1))))
```

- **World entry** headers name the entry, its root object, its number of copies (proxy pool) and the byte range of its
  script table in the decompressed world file. Entries without scripts are omitted.
- **Entry** headers give the entry's index (the `isplice` that object records carry) and the top-level objects bound
  to it (class and oid). "No top-level object" means only objects nested inside other objects' payloads use it (the
  dump does not parse object payloads), or nothing does.
- The body is the entry's list, normally one `(begin ...)` of `define`s evaluated once at load.

Spellings:

| Printed | Meaning |
|---|---|
| `define`, `lambda`, `if`, ... | special forms (tags 1-18); `form6` and `tag20` for the two forms without a known keyword |
| `find-object`, `geto`, ... | builtins, spelled as in [splice-language.md](splice-language.md) section 3; `name~` is a label that is not an original name; where that page has no spelling the label is derived from the C++ function (`BuiltinGetDevFlag` -> `get-dev-flag~`) |
| `evt-trigger`, `evt7` | event symbols 0-28 (`evt-NAME` where the meaning is known: post-load, update, label, trigger, untrigger, hit, game-event, activate, deactivate) |
| `self` | symbol 1002, the first parameter of most handlers (our spelling) |
| plain words | symbols 30-989, named by the level's save-schema table |
| `sN` | any other symbol (no name exists on disc), or a save-schema name that would clash with a builtin or form |
| `()` | the None value (tag 0): empty parameter lists, `'()` |
| `#nil` | a nested list that is null (0xFF); does not occur in retail data |
| `#t` `#f` `#bool:N` | booleans (word 0 or 1; anything else printed raw) |
| `#null-obj` `#obj:N` | object literals (always null in retail data) |
| `#vector(x y z)` `#matrix(...)` `#clq(...)` `#lm(...)` `#smp(...)` | literal boxes (never used by retail scripts, which call the constructors) |
| `#f32:XXXXXXXX` | a float with no exact decimal form (NaN, infinity), as its bit pattern |
| `#<unknown tag N>` | a tag the loader does not know; the game reads no payload for it, so the walk continues |
| `#cut` | a 0xFF where a continuation cell was announced (the game ends the list there); does not occur in retail data |

Annotations are block comments, `#|...|#`, so the listing stays valid input for `encode.py`:

- after an option id (second argument of `addo` / `ensureo` / `seto!` / `geto`): the option's label from
  [splice-options.md](splice-options.md), qualified by class (`TASK.RequestTaskState method(2)`) when the receiver's
  class is known statically (`self` of an entry whose bound objects share one class, `find-object` of an oid found
  in the level, a variable assigned from one of those, the world, camera, sound manager and hint box builtins);
  otherwise the label if every class agrees, else up to three alternatives marked `(by class)`. `sidebag` marks ids
  above 0x4D0 (per-object script variables). Labels are code-derived names for finding the implementation, not
  original script spellings, and the class guess is a static hint;
- after an oid (first argument of `find-object` and its relatives, elements of an oid list): the class and world
  entry of the top-level object with that oid;
- after a class id (`find-class-objects`): the class name;
- after the string id passed to the hint box (`PostNotice`, option 979): the localized text from the level file.

## 3. JSON

`{"level", "name", "fid", "lba", "size", "format": "sly2-splice-ast/1", "world_entries": [...]}`. Each world entry has
`index`, `name`, `root_oid`, `copies`, `splice_offset` (the table's offset in the decompressed file), `objects` (top-level
objects bound to an entry: `cid`, `cls`, `oid`, `entry`) and `entries`: one list per script entry. A list is `null`
(the 0xFF null list) or an array of cells; a cell is `{"tag": N, "kind": name, ...}` with one payload:

| kind (tag) | payload keys |
|---|---|
| forms (0-18, 20) | none |
| `int` (21), `bool` (23), `object` (33) | `value` |
| `float` (22) | `float` (text as printed) and `bits` (hex of the IEEE word, authoritative) |
| `symbol` (24), `builtin` (25) | `value` (id / index) and `name` (as printed) |
| `vector` `matrix` `clq` `lm` `smp` (26-30) | `floats` and `bits` (lists) |
| `list` (31) | `list`: a nested list (array or `null`) |
| `cut` (-1), `unknown` | none |

`encode.py --json FILE.json` compiles it back without the disc (`--check DISC` to compare).

## 4. The binary format, as decoded

**Finding the files** (all from the disc; [formats/disc-records.md](../formats/disc-records.md)): the executable
`SCUS_973.16` is found in the ISO 9660 root directory and mapped through its PT_LOAD headers. Its static file record
at 0x2A38A8 locates the boot world (level 0); a file record is 16 words, `lba = (aw0 ^ aw13) + 0xCA0`,
`size = aw1 ^ aw15`. The world-name strings come from the pointer table at 0x2A39E8, and the 1,449 option records
`g_aeopid` (stride 0x28: otyp, grfeopid, argument count at +0x18) from 0x2AC880; the world-file head needs them to step
over the options of the world and camera. World files are LZSS-compressed as a whole
([formats/compression-lzss.md](../formats/compression-lzss.md); ring zero-filled).

**Walking a world file** ([formats/world-file.md](../formats/world-file.md), [formats/level-objects.md](../formats/level-objects.md)):
world-entry table, sound section, save schema (its symbol table names symbols 30-989), level table (every level's
file record), localized strings, plane tree, options of the world and of the camera, Align(0x10); then per world
entry: `S32 cbBlob, S32 id, S32 fExtra`, **`U32 csplice; csplice x LIST`** (`LoadSwSpliceFromBrx`; with fExtra, never
set in retail, each entry is followed by 3 string16), the texture tables, an extra table, the object records
(`S8 k; k x S16; S16 cid; S32 oid; S16 isplice; S32 cb; cb bytes`; `isplice` names the script entry), Align(0x10).

**A list** (`DeserializePair`, `src/game/splice/serialize.cpp`):

```text
LIST := 0xFF                       null list
      | CELL
CELL := U8 b  [fExtra: S32 + 6 x S16 debug info]  PAYLOAD(b & 0x7F)  (b & 0x80 ? LIST : end)
```

Each cell is one `CPair`; bit 7 says another cell follows (a 0xFF there ends the list early: `#cut`). Payloads, all
little-endian:

| Tag | Meaning | Payload |
|---|---|---|
| 0 | None (the empty list value) | none |
| 1, 2, 5, 6, 7-18 | special forms: `set!` `define` `assert` form6 `if` `or` `and` `cond` `else` `case` `let` `while` `lambda` `begin` `import` `quote` | none |
| 20 | tag20 (Sly 1's pipe?), accepted, never used | none |
| 21 | integer | S32 |
| 22 | float | F32 |
| 23 | boolean | U32 (stored as word != 0) |
| 24 | symbol | U32 symbol id |
| 25 | builtin | U32 index into `g_mpbifkbif` |
| 26 / 28 / 30 | vector / CLQ / SMP | 3 x F32 |
| 27 | matrix | 16 x F32 |
| 29 | LM | 2 x F32 |
| 31 | nested list | LIST |
| 33 | object | U32 (pointer; always 0) |
| anything else | unknown | none (the switch's default arm reads nothing) |

**What the loader allocates** per world entry with a non-empty table: one `SPLCS` (0x10 bytes) and an entry array of
8 bytes per entry on the world heap, one `CPair` per cell, and one frame per entry whose list is not null
(`LoadSpliceEntryFromBrx`). Symbols are plain ids; nothing is allocated for them.

## 5. Validation

`--stats` prints, per level: the tag histogram and unknown-tag count; builtins used and whether each resolves to an
entry of the `bif.cpp` table; calls whose operand count is below the builtin's minimum or above it without rest
arguments; symbols used and named; the allocation counts above; the decode -> encode comparison of every world
entry's table. Once per run it compares the option records read from the executable with
`class_registry_options.inc`. Results on the levels checked so far:

| Level | Script entries (frames) | Cells (CPairs) | Builtin calls (distinct) | Unknown tags | Arity mismatches | Round trip |
|---|---|---|---|---|---|---|
| 0 prologue | 154 | 7,970 | 1,008 (46) | 0 | 0 | 239/239 world entries; text and JSON 40/40 tables |
| 1 front end | 1 | 44 | 5 (3) | 0 | 0 | 3/3; 1/1 |
| 2 Paris hub | 378 | 29,010 | 3,951 (67) | 0 | 0 | 287/287; 66/66 |

All 1,449 option records on the disc agree with the repository's table (otyp, flags, argument count). The research
decoder this tool descends from walked and re-encoded all 43 world files byte-identically
([formats/splice-scripts.md](../formats/splice-scripts.md)).

## 6. Modding: encode.py and what is missing

`encode.py DISC LEVEL FILE.scm [-o OUT.bin] [--check]` tokenizes the listing (comments and annotations ignored),
resolves names in the reverse direction (forms, builtin spellings, `#bif:N`, named symbols, `sN` / `evtN`), and
encodes each world entry's table. Structure comes from the header comments: `;;; ==== world entry K` starts world entry
K and `;; -- entry I of` starts entry I, so keep those lines when editing. Integers print in decimal and floats in the
shortest form that reads back to the same 32-bit value, which is why an unedited listing re-encodes byte-identically.
`-o` writes, per world entry, `U16 index, U32 byte count`, then the table exactly as `LoadSwSpliceFromBrx` reads it.

Not done yet, needed to play a modified script:

1. **Re-laying out the world file**: a changed table changes the size of its world entry, so everything after it
   moves, and each world entry ends with Align(0x10) relative to the decompressed stream. The loader reads the file
   front to back and object records carry sizes rather than offsets, so re-emitting the walk (copy every byte,
   substitute the tables, re-align) should be enough; whether any payload stores absolute offsets is unverified.
2. **LZSS compression**: any encoder that emits valid flag/literal/reference groups works (the decoder accepts
   literals only, at the cost of 1/8 growth); the size goes in the level's file record.
3. **Putting it on a disc**: every world file's level table (and, for a few worlds, the executable) holds the file's
   record, so a larger file needs either a free extent and patched records everywhere, or a port that loads loose
   files.

New symbols need ids no other script uses (any `sN` from an unused range; names are only labels), and new option ids
or builtins need a non-matching build ([cookbook/splice-command.md](../cookbook/splice-command.md)).

## 7. Limitations

- NTSC-U (SCUS-97316) only: the executable's data addresses are fixed. Other regions need their own addresses.
- 2048-byte-sector ISO images or sparse extents; raw 2352-byte BIN images must be converted first.
- Object binding lists top-level object records only; nested objects (rig children, ASEG/SM callbacks) are not parsed,
  so some entries show no binding and some oids no annotation (Paris: 343 of 561 `find-object` oids annotated).
- Receiver classes for option labels are a static guess (Paris: about 900 option uses class-qualified, about 370 left
  as alternatives, about 380 sidebag ids). A label like `SO has no option 1019` means the guessed class has no record
  for that id: either the guess is wrong or the script reads a missing option.
- Symbols other than 30-989, 1002 and the known events have no names on disc (fExtra, which would carry debug
  strings, is 0 in retail), so most locals print as `sN`.
- The LZSS ring is zero-filled; nothing proves that no stream references an unwritten slot, but every walked level
  parses to its end.
