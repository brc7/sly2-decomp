# Design decisions and compromises

The decisions currently in force that a maintainer must know while the code has to keep matching byte for byte. Each
entry says **what** the rule is, **why** (usually a g++ 2.95 behaviour), **where** it shows (files or symbols), and its
**status**: *permanent* (follows from matching the original or the user's policy) or *temporary* (lifted when the
condition named is met). Source of each entry: [guides/tu-merging.md](guides/tu-merging.md) (history and the full
evidence tables), [guides/full-link.md](guides/full-link.md) and [MATCHING_NOTES.md](MATCHING_NOTES.md). Draft for the
maintainer's review.

How code should read once it matches (names, comments, locals) is a separate topic:
[guides/naming-workflow.md section 1](guides/naming-workflow.md#1-readable-code-conventions).

## 1. Representing shared types

**1.1 Separate structs where views conflict.** When functions disagree about a struct's layout, they keep separate
structs rather than one merged guess. No speculative unions. *Why:* a wrong merge either breaks the match or hides a real
difference. *Where:* every `.cpp` with private views (`HACK` and its subclasses in `tu_17F090.cpp` are separate views
because a derived struct would shift every field by the base's tail padding). *Status:* temporary per type: a view folds
into the shared type when a header names its fields and the function still matches.

**1.2 Derived structs for extended layouts; supersets for duplicated copies.** When one function sees more of a struct
than another, the extension is a derived struct so matched code keeps the base unchanged (`DZGXP : DZG` in
`sly2/dzg.h`, `SENSOR : SO`, `LASEN : SENSOR`). Duplicated private copies of one type in a file are merged into the
superset (grab's `GRABSPOT` with all its virtuals). *Status:* permanent rule; individual derived views are temporary.

**1.3 Unions only where a mode field selects the bytes.** A union is allowed when a kind/mode field selects what the
bytes hold and the code shows it: `BEAMEND`'s tail in `sensor.cpp`, the guard-script command argument in
`tu_1709A8.cpp`, `EOPID` in `sly2/eopid.h` (by option flags), the `CRef` payload in `sly2/cref.h` (by tag). *Status:*
permanent rule.

**1.4 Matching-only islands.** Where no natural spelling matches, a small documented form that exists only to reproduce
the original codegen is kept (an int view of a float store, a 16-byte aligned pair pointer, a register-pinned asm
transcription), with a comment saying so. Accepted and refused forms are listed in
[MATCHING_NOTES.md section 5](MATCHING_NOTES.md#5-what-is-accepted-in-matched-source). *Where:* `dzg.cpp`,
`spliceobj.cpp`, `jthands.cpp`, `mat.cpp`, `glob.cpp`. *Status:* temporary per island; revisit when the code moves to
shared types.

**1.5 Per-function namespaces.** A function whose private views conflict with the file's types, or are part of its
tuning, keeps them verbatim in `namespace M_<function> { ... }`. Plain-named definitions stay `extern "C"` inside. A
function whose symbol is mangled cannot live in a namespace under its own types (the namespace would change the
mangling), so it becomes a C-linkage definition of its literal mangled symbol
(`extern "C" int IsEmitterPaused__FP7EMITTER(EMITTERV *pemitter)`); the symbol string is unchanged, so this is reversible.
*Where:* most large TUs (`grep -l "namespace M_" src/game`). *Status:* temporary: each namespace empties when its
views move to shared types.

**1.6 V-renamed views.** A private view that clashes by name with a shared header type is removed if the layouts agree,
otherwise renamed with a `V` suffix (`BASICV`, `LOV`, `ASEGAV`; occasionally `Q`/`A`). A C++ symbol that mentions the
view keeps its original mangled name through an asm label on the declaration
(`IsBasicDerivedFrom(BASICV *, CIDV) __asm__("IsBasicDerivedFrom__FP5BASIC3CID")`). *Why:* cc1plus crashes on a
redefinition of a polymorphic class, and the mangled name must not change. *Status:* temporary: the V-named views are
the to-do list for view unification.

**1.7 Registry classes are real classes; key functions decide vtable emission.** The registry classes live in
`sly2/classes.h` and the class headers with their real virtual layouts; each virtual is a member declaration with an
asm label naming the function the ELF vtable holds there. Each class's first declared virtual is one its key TU
defines, and classes are declared in emission order, so each key TU emits its vtables as the original did. Inline
constructors that must not be seen by the key TU sit under `#ifndef SLY2_KEY_TU_<CLASS>`. *Status:* temporary in form:
labels become plain members as functions become real members.

## 2. Linkage and symbols

**2.1 Asm labels bind declarations to real symbols.** A declaration under a draft, view or Sly 1 name is bound to the
symbol the ELF calls with `__asm__("<symbol>")`; the source name stays when it is the better name. Works for free
functions, constructors, members, operators and globals. Two exceptions: never on a destructor (g++ 2.95 then drops the
destructor call) and not on template explicit specializations (ignored). A header label is checked against every
includer. *Where:* `grep '__asm__("' src/game include`. *Why:* relocations are masked by the match check, so only the
native link sees a wrong symbol. *Status:* temporary per label: it goes when the identifier is renamed to the symbol
(tools/rename.py).

**2.2 Addend labels.** Where a base-plus-offset spelling changes codegen (g++ shares or folds the base), a field inside
another object is declared as its own symbol whose label carries an addend:
`extern MEMCARD worker __asm__("<owner symbol>+0x<offset>");` (`memcard.cpp`, `menu.cpp`, `hud.cpp`,
`tu_201D78.cpp`). g++ sees a separate symbol; the assembler relocates against the owner. The same trick with `+0` names
a symbol whose plain label would make g++ emit the file's own inline member under it (`splicemap.cpp`,
`tu_1553F0.cpp`). *Status:* temporary: until the owner's field can be spelled directly without changing code.

**2.3 Weak `.gnu.linkonce` definitions.** Six out-of-line inline copies whose inline form does not emit or does not
match are plain definitions with `__attribute__((weak, section(".gnu.linkonce.t.<symbol>")))`: `VecCombo` and
`operator*` (`tu_1553F0.cpp`, `bif.cpp`), `STEPGUARD_ctor` / `JT_ctor` / `MURRAY_ctor` (`class_registry.cpp`) and
`CHash<char *, int>::Resize` (`sly2/arrayv.h`, `tu_119BC8.cpp`). This is the object form of a linkonce copy (weak
symbol, own section) with names and code unchanged. *Status:* temporary: each becomes a real inline/template member
when its callers or its spelling allow (conditions in tu-merging.md "Representation decisions").

**2.4 Linkonce copies by reference or explicit instantiation.** Other inline copies are emitted the natural way: an
inline declared before its callers and defined after them (end of file), template members explicitly instantiated in
the TU that owned them (`emitter.cpp`, `tu_119BC8.cpp`, `splice/gc.cpp`), or the address taken in a table
(the option table in `class_registry_options.inc` names the inline accessors). Each copy has exactly one emitting object. *Status:* permanent
mechanism; the HYPOTHESIS is that this mirrors the original header order.

**2.5 SLY2_ALIAS for generated symbols.** g++ names the static initializer `__static_initialization_and_destruction_0`
in every object and the `_GLOBAL_$I$` / `_GLOBAL_$D$` stubs after the source path, so config cannot bind them. A
`// SLY2_ALIAS: obj=config` line near the top of the file renames them before comparison (matchtool, progress.py).
`SLY2_INSTANTIATE_SLOT_POOLS()` goes right after the includes (naming a pool earlier reorders the initializer).
*Where:* nearly every game TU; `sly2/slotheap.h`. *Status:* permanent while the generated names are file-path based.

## 3. Data layout and sections

**3.1 RO_ORDER.** A named `.rodata` constant is defined just before the function the ELF puts after it, inside
`namespace RO_ORDER { extern const float r_D_X __asm__("D_X") = ...; }`. g++ emits a named constant at its definition
and a function's literals with the function, so this orders `.rodata` as the ELF does; the namespace plus label keeps
the definition invisible to the code (no constant folding). *Where:* the data-phase files
(`grep -l "namespace RO_ORDER" src/game`). *Status:* permanent (only spelling; rename the identifiers freely).

**3.2 Custom section names.** `.rodata.early` holds the named constants of the first part of `.rodata` (`cd.cpp`,
`menu.cpp`, `memcard.cpp`, `hud.cpp`, `sw.cpp`, `game.cpp`, `tu_1A4E08.cpp`, `tu_15CD68.cpp`,
`class_registry_options.inc`); `.data.buildinfo` holds `main.cpp`'s build strings. The original placed these apart from
their TU's literals / `.data` for an unknown reason; tools/link.py places sections by name. *Status:* permanent until
the original mechanism is known; the names themselves are ours and easy to change.

**3.3 Palette `.rodata` attribute.** The two startup palettes (`tu_1147D8.cpp`, `tu_114AE0.cpp`) carry
`__attribute__((section(".rodata")))`: the original compiler put a const object with a dynamic initializer in
`.rodata`, ours puts it in `.bss`. *Status:* permanent (compiler difference).

**3.4 SLY2_SCRAP units.** Dead-code scraps (`config/junk.csv` rows ending "emitted by its TU source") are one
instruction left of a removed function plus alignment; the TU emits each in place as `SLY2_SCRAP("addiu $sp, $sp,
0x30");` (`include/sly2/scrap.h`), after the previous function and before any data definition. *Why:* no C produces an
instruction without its return. They are not functions; progress.py skips them. *Status:* permanent.

**3.5 SCOPEDIJ: namespace-scope record instead of a local class.** `grab.cpp` declares a function's frame record at
namespace scope (`SCOPEDIJ`, placeholder name) because g++ 2.95 emits a local class's implicit constructor and
destructor out of line (functions not in the ELF). *Status:* permanent rule (applies to any local class with a
non-trivial member); rename the type when its real name is known.

**3.6 commons.cpp.** `src/game/app/commons.cpp` defines the game's COMMON symbols (the Splice GC kinds' list words and
block tables, the slot-pool and splice-heap template statics and their sentries, a few class statics) as ordinary
objects in `.sbss` / `.bss` blocks in address order. GNU ld allocated commons in hash-table order after all `.bss`,
which no object order reproduces; strong definitions take their place. Not an original TU, no functions, listed in
tools/link.py's data sources, in no matched.csv row. The pools are storage under the mangled name, not C++ member
definitions (a member definition would generate constructor code the ELF lacks). *Status:* permanent (user decision);
the SDK's own commons stay with the SDK sources.

## 4. Build flags

**4.1 `-fno-rtti` (and `-fno-exceptions`).** In the C++ build flags (tools/matchtool.py, used by progress.py and
link.py). *Why:* every vtable in the ELF has 0 in the typeinfo slot and no RTTI code exists. Side effect: without RTTI a
class that overrides nothing of a secondary base reuses the base's vtable, so a view class must override at least one
of a secondary base's virtuals exactly when its ELF secondary vtable differs. *Status:* permanent.

**4.2 class_registry's SLY2_CFLAGS.** `class_registry.cpp` (the whole crt0/Splice-thunk TU) carries
`// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31 -fno-exceptions -fno-rtti -fpermissive`: the included generated
Splice wrappers (`src/generated/splicewrap/*.inc`, one namespace each via `SLY2_SPLICEWRAP_BEGIN`) are C-style drafts
that need `-fpermissive`. The line *replaces* the default flags, so a change to the global CFLAGS must be repeated
here. *Status:* temporary: until the wrappers are valid C++.

**4.3 Per-file compiler headers for the SDK.** SDK sources name their compiler and flags in `SLY2_CC:` /
`SLY2_CFLAGS:` / `SLY2_ASM:` header comments ([MATCHING_NOTES.md section 2](MATCHING_NOTES.md#2-sony-sdk-code-compilers-and-build-annotations)).
*Status:* permanent.

**4.4 Path-hash pins.** g++ 2.95 hashes the presumed file name and line into the gcse order of inline asm, so a few
asm functions match only at a given (path byte sum, line) window. They keep a `#line N "<old path>"` pin before the asm
helper and a `#line` back after it (`tu_1D1370.cpp`). Moving or renaming such a helper can break the match; so can
renaming a global whose symbol name feeds the same hash (`g_pfrmOpen` in `DrawSw`). *Status:* permanent.

## 5. What is not source

**5.1 Build-time blobs.** tools/link.py cuts byte ranges from the user's own ELF (`build/link/blobs.o`, never
committed) for what no object covers:

| Blob | Status |
|---|---|
| VU microcode (`.vutext`) and IOP modules (`.irx`) | permanent: binary inputs (no DVP sources); names inside them are labels on their blob sections |
| Built-in font and bitmap artwork (`tu_1147D8.cpp`, `tu_114AE0.cpp`, `font.cpp` blocks) | temporary: awaiting the user's decision on how image assets are kept (the repo never commits game artwork) |
| SDK sources that do not build on Linux | temporary: linked as objects where the Mac's `--sdk-only` build provides them |
| Data no object defines yet | temporary: shrinks as data is defined in source |

**5.2 No game data in the repo.** Users bring their own disc; config and tools extract what the build needs. *Status:*
permanent (user policy).
