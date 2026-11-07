# Guide: names and readable code

How the code is named and commented, how to find a name for an unknown function, how to apply a rename without breaking
the match, and how progress is measured.

## 1. Readable-code conventions

Goal (the maintainer's): code that programmers, modders and agents can read without this guide, for a faithful port,
for mods, and for learning from the game's design. Sly 1 names and Sucker Punch style are evidence about meaning, not a
style to preserve. Spirit over letter. Everything below must keep the match: the object file for every function and
data symbol stays byte-identical (matchtool), and every file is re-checked after editing.

**Names**
- Locals, parameters, struct/class fields: descriptive `camelCase` English (`listener`, `noiseType`, `offset`,
  `heightRange`); booleans read as predicates (`isInWorld`, `hasTarget`, `canJump`); counts `numX` / `xCount`; indices
  `i`/`j` only for short loops, otherwise `guardIndex`. No Hungarian prefixes (`p`, `c`, `f`, `g`, `pfn`...) in new names.
- Functions: `PascalCase` verb phrases saying what they do (`CanListenerHearNoise`, `GetWorldPosition`,
  `UpdateGuardAlertTimer`, `LoadSceneFromBrx`). Predicates start with `Is/Has/Can/Should`. Class members keep the same
  rule (`GUARD::UpdateAlert`).
- Existing names: rename when cryptic (Hungarian return-type prefixes like `FCheckLm`, `PloFind`, `GGetX`, `NGetY`;
  unexplained abbreviations; `func_XXXXXXXX`); keep names that are already clear.
- Types keep their current names for now (`LISTENER`, `NOISETYPE`; renaming a type changes every mangled symbol that
  uses it: a later, separate pass). The Sucker Punch abbreviations they use are in the
  [engine glossary](../engine.md#10-glossary). Macros/constants: `UPPER_SNAKE`.
- Class placeholders are decimal (`CID39` for cid 0x27, `BHV171`): mixing hex and decimal caused real mix-ups.
- Unknowns: give the best descriptive name the evidence supports and mark it `(inferred)` in the doc comment; never
  leave `func_XXXXXXXX` or `unk_XX` when a reasonable meaning can be read from the code. Offset-coded fields
  (`x4C8`, `_pad_357`) get a descriptive name or, if truly unknown, `unknown4C8`.

**Doc comments**
- One `//` comment block before every function: what it does and why it matters (inputs, outputs, side effects, which
  system calls it), not a line-by-line paraphrase. Mark inferred meaning with `(inferred)`.
- A short header comment at the top of each file: the subsystem's role and its main entry points.
- Comments inside functions only where code alone can't carry the meaning (units, coordinate spaces, magic numbers).
- Evidence tags (`[C:runtime]`, `[C:static]`, `[H]`, ...) keep the meaning given in [engine.md](../engine.md#1-how-knowledge-is-marked).

**Matching constraints** (what is not free)
- Function and global names are linker symbols: rename them with `tools/rename.py` (config + all callers), never by hand.
  `extern "C"` names are the symbol; C++ names mangle parameter TYPES (not parameter names). For a C++ function give the
  new base name only (`tools/rename.py FCheckLm__FP2LMf IsWithinLimits`); the tool keeps the signature part (section 3).
- Local/parameter/field names never reach the object file. Types (`int` vs `bool`, signedness, widths), control-flow
  shape and statement order can change codegen: a readability rewrite of logic must still match or be reverted.
- Line numbers matter for a few functions (path-hash quirk: `#line` pins). Adding comment lines can shift them: re-check
  the whole file after any edit; keep existing `#line` pins correct.
- After renames compare each touched object's `nm` symbol set (only the intended renames may differ): matchtool alone
  does not see a renamed callee.

## 2. Finding a name, and how far to trust it

For an unknown function, in order of yield:
1. Read the Sly 2 body and its file's header comment: decisive in about 60% of cases.
2. Name the callees (the body's real call graph, not stale externs); that usually fixes the subsystem.
3. Compare with the Sly 1 counterpart **and its two neighbours**: Sly 1 file order is largely kept, and shifted runs are
   the most common error. Sly 1 is a source of meaning, not of signatures (Sly 2 passes VU vectors by value).
4. Check the signature (arity, float vs pointer, setter vs getter), constants and ELF data tables, and call-site
   arguments (`&D_002DDF40` = `g_cm`, `D_003DD490` = `g_sw`).
5. Functions with no callers are vtable slots, startup-table entries, message hooks or callbacks; the LO slot roles in
   [engine.md](../engine.md#6-the-object-model) make a subclass's Init/Load/Update mechanical.

Calibration from four independent verifier rounds (80 proposals each, re-derived from the code without the proposer's
evidence): proposals at confidence >= 0.8 were 97-100% right; 0.5-0.8 were 80-100% (errors mostly class-identity slips);
below 0.5 the name was right ~60% of the time but the purpose line ~85-95%. Function size and alignment had no predictive
value. When a name turns out wrong, check its neighbours: errors come in shifted runs.

Runtime observation overturned several confident static readings (JTMODE kind 1 is recon, not binocucom; `g_pjt` is always
Sly, not the current player; Bentley's "bombs" are a dart and a gadget object; the "checkpoint manager" is the transition
request; `AddSoVelocity` is `AddSoAcceleration`). Lesson: a structural reading of a single code path is not a role; get a
second signal before naming with confidence, otherwise write `(inferred)`.

## 3. Applying names: `tools/rename.py`

```sh
tools/rename.py OLD NEW [--mangled M]       # OLD = current name or 0xADDR; NEW = plain or gcc 2.95 mangled name
tools/rename.py --csv batch.csv             # rows: addr,new_name[,new_mangled]
#   --dry-run [--diff]  --on-fail revert|stop  --log FILE  --commit-msg FILE  --jobs N  --allow-existing-uses  --allow-unverifiable
```
Needs the matchtool environment. Exit 0 = all applied/already applied, 1 = something refused or reverted, 2 = stopped.
The tool's docstring lists exactly what one rename changes.

Policies:
- **Plain names when the signature is not confirmed**: an unmangled C-linkage identifier everywhere, never a guessed or
  placeholder mangling. The linker symbol is a claim about the binary and matchtool cannot check references, so a wrong
  mangling would record something false. Upgrading later is another rename. Confident mangled names for a
  `func_XXXXXXXX` are used verbatim; renaming a function that already has a mangled name keeps C++ linkage (below).
- One rename changes, in one lexer pass per file: code identifiers, the name inside string literals (asm labels, inline
  asm), comments (to the plain name), the file name, `config/symbol_addrs.txt`, `functions.csv` and `matched.csv`. It also
  sweeps the `func_XXXXXXXX` placeholder, finishing earlier partial renames, and `tools/layout/units.csv` follows by
  address. It does **not** edit docs/.
- The lexer treats `namespace N { ... }` like `extern "C" { ... }` (definitions in the merged-file pattern
  `namespace M_Func { extern "C" { ... } }` are found; a definition in a namespace with no enclosing `extern "C"` is
  refused, its symbol carries the namespace), and lets a string span lines only directly after `asm(` /
  `__asm__ volatile(` (gcc 2.95 accepts raw newlines in asm templates).
- **Safety**: name validation (C identifiers; a gcc 2.95 mangling grammar); collision checks (config, code tokens,
  case-insensitive file names, in-batch chains); **every touched file is rebuilt before and after and compared per
  address** (a rename can change codegen through gcse's symbol-hash order); on failure the offending renames are pulled
  out and retried one at a time. Besides the per-address compare, every rebuilt object's symbol table (defined and
  undefined) must equal the old one with exactly the intended renames applied: this catches a renamed callee or a call
  that now binds to another overload, which matchtool cannot see (`_GLOBAL_$I$` stubs are compared without their random
  suffix). Idempotent and resumable. Files that cannot be built here (SDK sources needing the native
  ee-gcc 2.96 or GNU as) are refused unless `--allow-unverifiable`.
- Naming `main` is a source change: gcc inserts an implicit `__main` call into `main` (hence `GameMain`).

**C++-linkage renames.** When OLD's symbol is a g++ 2.95 mangled name and NEW is a plain name, the function keeps C++
linkage and only its base name changes (`--plain` gives a C-linkage symbol instead):

```sh
tools/rename.py FCheckLm__FP2LMf IsWithinLimits      # -> IsWithinLimits__FP2LMf; OLD may be 0xADDR, NEW the full new mangling
```

- In gcc 2.95's encoding nothing after the `__` split depends on the base name, so the new symbol is `NEW + OLD[split:]`.
  The pre-check is that it demangles back to NEW with the same class; the ground truth is the object (the defining file
  must define the new symbol, every symbol table may differ only by OLD -> NEW). A changed signature or class is a source
  edit, not a rename, and is refused.
- **Unique base** (no other config function has that base or plain name): every code/comment token of the base is renamed.
- **Shared base, member** (`Skip__18CBinaryInputStreami` vs `Skip__18CBinaryAsyncStreami`): the spellings that certainly
  name this class's member are renamed up front (`CLS::Name`, declarations inside every `struct/class CLS` body including
  TU-private views, unqualified calls inside `CLS::` bodies); the remaining call sites are **found by compiling** (each
  call that still means `CLS::Skip` fails to compile and that line is renamed, for a few rounds). A call that silently
  binds to an inherited same-named method fails the symbol check.
- **Shared base, free function** (overloads, or a C function of the same name): refused; rename by hand or rename the
  other function first.
- **Virtual members**: the C++ name is the vtable slot shared by the hierarchy. If an `__asm__` label fixes the symbol
  (most members in `include/sly2/*.h`), only the label and config change; otherwise refused. Renaming the slot itself
  across the hierarchy is a hand edit.
- **Refused**: constructors, destructors, operators, members of class templates and function templates, new base names
  containing `__` or ending in `_`, and new base names already used in code or as another function's base.
- `_GLOBAL_$I$` / `StaticInit_<tu>` stubs keep their names: they are compiler-generated and every merged TU maps them
  through `SLY2_ALIAS`. A header edit rebuilds every includer, so labels in widely included headers are slow to rename.

**Reviewing a bulk rename**: read the log counts and every non-applied reason; skim `git diff -M30% --stat`; then review
the **names**, not the edits (plain-policy list, mangled signatures vs the Sly 2 body). One batch per commit, with the JSON
log kept next to it.

### Struct fields: `tools/fieldrename.py`

Field names never reach the object file, so the compiler decides which `->f` means `S::f`. Input is a reviewed plan CSV
(header,struct,offset,old_name,new_name,confidence,basis):

```sh
tools/fieldrename.py --csv PLAN.csv --header clock.h --dry-run      # print every edit site, change nothing
tools/fieldrename.py --csv PLAN.csv --header clock.h [--struct CLOCK] [--fields a,b] [--min-confidence med] \
    [--check] --log batch.json --commit-msg msg.txt                 # apply + verify, or restore
```
One run is one batch, all or nothing; commit each batch on its own (the tool does not commit). It refuses rows whose new
name would shadow or clash (keyword, `#define`, a token already in the hierarchy's bodies); renames the declarations in
the header and in every TU-private view of the same struct (one-line token swaps, so `#line` pins hold); then compiles
every TU that sees an edited definition and rewrites exactly the tokens g++ reports (`has no member named`, `undeclared`
for implicit `this`, mem-initializers), until all compile; finally every function's words and relocations, every data
symbol and the object's `nm` set must be unchanged, or the tree is restored. Limits: comments are not touched;
uninstantiated template bodies and `#define` bodies are only warned about; a template instantiated for several types
needs the field renamed in all of them in one batch. The tool's docstring has the details.

## 4. Progress metric: `tools/readability.py`

- `python3 tools/readability.py [PATH ...] [--csv FILE] [--top N]` (default paths `src/game include/sly2`; standard
  library only; reads files, builds nothing). Prints one row per path and a TOTAL row; `--csv` writes one row per
  file, `--top N` lists the files with the most open items. Columns: `files`/`hdr` (files / with a header comment),
  `funcs` (definitions with a body, inline struct members included), `doc` (prose comment block directly above),
  `annot` (only a generated `@sly2-annot` draft above), `undoc`, `func_` (still `func_XXXXXXXX`), `func_refs`
  (placeholder tokens in code), `hung` (Hungarian return-type prefixes: `FCheckLm`, `PloFind`, `GGetX`...), `short`
  (single-letter params/locals other than i/j), `m2c` (`temp_`, `var_`, `sp10`, `arg0`), `offs` (`x4C8`, `unk_..`,
  `_pad_..`, `pad14` occurrences), `inferred` (markers).
- It is regex/brace based, not a parser: its docstring lists the heuristics and their limits (macro-generated code
  is invisible; `funcs` counts the many per-file inline view helpers, so it exceeds the 6,662 linked functions;
  `m2c`/`offs` count occurrences, not distinct names; the Hungarian test misses suffix tags and unlisted prefixes).
  Use it for trends, not as an exact census. verify.sh does not run it (verify.sh is about correctness).
- Baseline (names/base 7da894e7) and after the first readability wave (integrate/names-r1 3493d772):

      scope            files  hdr  funcs   doc  annot  undoc  func_  func_refs  hung  short    m2c   offs  inferred
      base TOTAL         248  248   8933   789   4396   3748   2011      11384  1230  13548      6  10289         0
      names-r1 TOTAL     249  249   9049  5398    414   3237    147       1899  1141  11622      6   9812      1228

  i.e. documented in prose 8.8% -> 59.7% of definitions, `func_XXXXXXXX` 22.5% -> 1.6%, Hungarian 13.8% -> 12.6%;
  every file has a header comment.

## 5. Shared headers (`include/sly2/`)

- Every shared header has a role-first header comment and doc comments on its structs and on the fields whose meaning
  is clear. `(inferred)` marks meanings read from names and usage rather than confirmed by code or runtime;
  `[R]`/`[S]`/`[H]` tags in older comments mean runtime-confirmed / static / hypothesis.
- vu0.h holds the only inline asm in the shared headers, and inserting lines above an asm statement moves its line
  number, which the path-hash quirk feeds into gcse ordering. Edit it only by appending below the last asm line, then
  re-check its includers.
- Renaming a shared field: rename it together with every TU-private view that spells it, and check nm sets and
  matchtool for every includer.
