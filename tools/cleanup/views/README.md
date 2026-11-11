# tools/cleanup/views: replace TU-private views with the real classes

A *view* is a struct a TU declares for itself whose members sit at the EE byte offsets of a header class (`char
pad[...]` up to the fields it reads, `struct S : BASE { char pad[...]; ... }`, a header class re-declared under its
own or a local name). Function-by-function matching left hundreds of them. They hide the real types from readers and
put members at the wrong offsets in a 64-bit build. These tools find them, work out the class each one stands for,
and rewrite the TU onto the real class. A rewrite is kept only if the TU's object stays byte-identical to the
baseline's.

| script | does |
|---|---|
| `inventory.py` | lists every view in src/game with its target class, confidence, member mapping, the functions that use it and whether it is on the port's load / frame path (`views.json`, `views.csv`) |
| `rewrite.py` | rewrites the confident, fully mapped views of the given files and keeps each rewrite only if the object is identical (log: `rewrite_log.jsonl`) |
| `report.py` | totals, rewrite outcomes by reason, header findings (members in class pads, draft-named members the views name) |
| `targets.json` | hand annotations `"file.cpp:NS::VIEW": "CLASS"` for views no hint identifies (the evidence goes in a `_file.cpp` comment key) |
| `parse.py`, `common.py` | the TU struct parser; compiler, object comparison and text helpers |

## Run

```sh
. env.sh                                        # matchtool's environment (SLY2_PYTHONPATH ...)
OUT=build/views
python3 tools/cleanup/census/dump_offsets.py --out $OUT          # header layouts (ee-gcc), once per header change
python3 tools/cleanup/views/inventory.py --out $OUT [--load-path load_path.csv] [FILE...]
python3 tools/cleanup/views/rewrite.py --base-root ../base-worktree --views $OUT/views.json FILE...
python3 tools/cleanup/views/report.py --views $OUT/views.json --log $OUT/rewrite_log.jsonl --md $OUT/REPORT.md
```

Run inventory and rewrite again after a rewrite: the second pass sees the classes the first pass put in, and
propagation reaches further. The baseline worktree is checked out at the base commit before any edit (as for
objcmp). Its objects are compiled once per file and cached (`--cache`, default `$OUT/base_objs`).

## How it decides

- **Layouts come from the compiler.** For the views, a probe copy of the TU (`.viewprobe_<name>`, written next to
  it and removed afterwards) appends `SLY2_OFFSETOF` / `sizeof` initialisers for every member of every
  namespace-scope struct and is compiled with ee-gcc. Only errors on the appended probe lines count as failed probes (the TU's own warnings
  and lines of TUs with `#line` pins name the probe copy or the TU, not the probe lines). For the headers, census/dump_offsets.py does the same. No
  size table is involved. Structs defined inside function bodies cannot be probed, so they are counted but not
  inventoried.
- **Candidates:** structs with a pad member, a header-class base, a vptr word at +0xC, a header class's name (or
  `<CLASS>V` / `View` / `Island`), a hand annotation, or a struct reached from a typed view through a member (below).
  Opaque forward declarations (`struct SPACE;` never defined) count as pointer-only views.
- **Target class**, strongest first:
  1. the hand table;
  2. the struct's name;
  3. a comment naming one class;
  4. propagation: a view member that points to another view, or embeds one, maps onto a header member typed
     `C *` / `C`, so the other view is a C, or its unique least derived descendant that holds every member the
     view uses (JTAIR::player gives STEPPLAYER; STEPPLAYER::positionXf gives XF);
  5. offset fit: every used member starts at a member of the same size; within one inheritance chain the least
     derived class wins, and a base class restricts the pool to its subtree.

  Confidence is **high** for 1-4, or for a base class plus a fit of 3+ used members. It is **medium** for an offset
  fit alone (often an unrelated class with the same offsets, which would compile to the same bytes under wrong
  names, so rewrite.py skips it by default) and **low** otherwise.
- **Member mapping** uses the class's flattened layout: bases, anonymous-union aliases, embedded records and array
  elements. Among aliases at one offset it prefers the same type, then the same shape (a local `M3 { QWROW row[3]; }`
  is MATRIX3Q, not MATRIX3; a local `{qword q; float f[4]}` union is QWROW), then pointers, then the shortest
  path. Other statuses: `in pad` (the header has no member there yet), `straddles`, `stride mismatch`, `size
  mismatch`, `type mismatch` (pointer against non-pointer), `beyond` (past the class's size), `vptr`.

## How it rewrites

Per namespace block, all eligible views together first, then one at a time (leaves first), repeated while anything
succeeds:

- the definition is removed. Its newlines are kept, so later lines keep their numbers (g++ 2.95's asm ordering
  hashes file and line); afterwards a *tidy* step drops the empty lines again if the object stays identical;
- forward declarations and the view's own layout asserts (`typedef char X[... VIEW ...]`) are removed;
- the type name becomes the class, and each access `->m` / `.m` becomes the mapped member path. A small resolver
  types the base expression (variable declarations, member chains, array elements) when two views in the block use
  the same member name differently;
- a header the TU does not include yet is added after its last `#include`. The line comes out of a removed
  multi-line definition or the first empty line after the includes, so the following lines keep their numbers;
- `->m.q` on a local qword union mapped onto a plain `qword` member drops the `.q`.

Then the TU is compiled and compared with the baseline object: every allocated section's bytes, relocations by
target symbol, symbols by name, value, size and section. Anything else is reverted and the compiler's message (via a
host `g++ -fsyntax-only` of the ee-gcc preprocessed file when cc1plus dies silently) or the differing sections are
logged.

Headers are never edited by rewrite.py. When a member is missing from a header (`in pad`), or a header type is in
the way (the per-function `typedef int qword __attribute__((mode(TI)))` against basic.h's unsigned qword, a local
re-declaration of a header class in the same block), the inventory and the log say so, and the header change is
made by hand (as sly2/xp.h was for XP).

## Limits

- Offset fit alone does not identify a class. Hand annotations (or a pointer from an identified view) are what
  turn most views into rewrites. The log's "no target" / "ambiguous" rows are the work list for annotations.
- Views reached through casts or untyped pointers (`(OBJECT *)x->p`, `void *`) are not propagated.
- One wrong member makes a view unmappable (e.g. air.cpp's CONTEXT.tracks: JTAIR::abl[i + 1] with a different
  element size). Those need a hand edit or a derived view.
- Byte identity proves that the code is the same, not that the names are right: a wrong target with the same
  offsets would pass. That is why only high-confidence targets are rewritten by default.
