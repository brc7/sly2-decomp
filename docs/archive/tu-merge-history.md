# TU merge history (archive)

The per-wave notes of the merge that turned the per-function files into one source file per original translation
unit, moved here verbatim from the "Mismatch causes and fixes" section of [tu-merging.md](../guides/tu-merging.md)
when that page became a guide. Source file names use the current names (the address-named `tu_XXXXXX.cpp` files were
renamed later). The per-TU merge log itself is in git history (`git show 3493d772:docs/guides/tu-merging.md`); the
durable lessons are summarized in tu-merging.md under the mismatch table, and section references below ("the table
above", "TUs with an older multi-function file") point to tu-merging.md or that older version.

## Observations by wave

Other observations:
- `.c` files compiled as C++ matched unchanged in every other case (about 20 files): expect B4 only for by-value
  structs that are passed on, and the `!`/bool difference above.
- Six linkonce helpers carried `// SLY2_CC:`/`// SLY2_CFLAGS: -O2 -G0` overrides in their own files; in the merged
  `.cpp` they match with the default C++ flags, so the overrides were not needed (spliceutils). Check this rather than
  copying overrides into a merged file (a file-level override would apply to every function of the TU).
- Per-function gcse stand-ins (brx_sw's dead `if (fNever)` block that sizes the gcse hash table) survive the merge:
  that table size is per function, not per file.
- When rewriting doc links, match `\.(cpp|c)` in that order: `(c|cpp)` stops at `.c` and leaves `pp` behind (fixed
  in a follow-up commit of wave 1). Run `python3 -I tools/linkcheck.py` after each merge.
- Not hit yet: inline methods or class definitions that become visible in the merged TU and change a function's
  codegen (pilot blocker B6). The gifs/vifs cases suspected of it were draft-body and register-pin differences
  (wave 3, see "TUs with an older multi-function file").
- Wave 2 (asm-free TUs up to 21 functions, plus three small asm TUs): of about 110 per-function files, the only
  C-vs-C++ codegen differences were the rows above (draw, barrier, find, rumble, bas); everything else matched on the
  first compile once types were unified. Most of the work is unifying the per-file views of one object into one type.
- `.c` callers that passed a hidden result pointer by hand (`GetSpeakerSubtitleColor(&c, k)`, `GetGlobalSaveVar(&ref,
  ...)`) become by-value calls in C++ and match: g++ uses the same hidden-pointer convention for a returned non-POD
  class (subtitle, difficulty).
- Defining a class's first virtual function in the merged file makes it the class's key TU: g++ then also emits
  out-of-line copies of the class's inline members (bq: `__17CQueueInputMemoryPUci`) and, with RTTI, `__tf...`
  typeinfo functions. They have no address in the binary (the code exists nowhere there) and show as extras, not
  mismatches. Since C++ is compiled with `-fno-rtti` (build/rtti-palettes, as the game was) the `__tf` ones are gone;
  the constructor copy stays.
- A per-function `.c` that defines a mangled name as a C symbol (`InitDl__FP2DLi`, `FindSwObject__FP2SWi3OIDP2LO`)
  becomes the C++ function that mangles to it; its parameter types must then carry the exact names (`SW`, `OID`,
  `LO`, `CID`), so a TU-private view of LO is called `LO` (find). The same applies to callee declarations: a callee
  whose symbol is plain must be declared `extern "C"`, and one whose symbol is mangled must be declared with the types
  it names; a literal-mangled name declared outside `extern "C"` (`CRef EvalExpression__FP5CPairP6CFrame(...)`) gets mangled
  twice. Wave 2 fixed three such slips in a follow-up commit; check every prototype against config/functions.csv.
- A function whose current symbol has the wrong signature (bas: the constructor `__18CBinaryAsyncStreamPv` takes two
  arguments) stays a C-linkage function under that symbol until it is renamed.
- Wave 3: the `SLY2_CC`/`SLY2_CFLAGS` overrides of frame's GcBinding_InitNode and func_00257450 were not needed in
  the merged file either (as in spliceutils).
- Pool layout (HYPOTHESIS, wave 3): the GC code addresses a block table of four pointers 0x10 bytes before each
  pool's SLOTHEAP (0x452098, 0x4520C0, 0x4520E8, 0x452110, 0x452138, 0x452160 for the six pools), which suggests
  the pool object is `{ void *apvBlock[4]; SLOTHEAP slotheap; }` (0x28 bytes, matching the pool spacing).
  include/sly2/slotheap.h is unchanged; check against the pool code before changing it.
- Wave 4: the object parts of game classes (BUTTON : SO, VOLBTN/EXIT : VOL, WARP : XFM) were written as views of
  the fields used (as barrier did), with hand-made vtable-entry calls whose slots are named in comments (3
  HandleSpliceEvent, 4 SendMessage, 9 Remove, 38 an LO slot taking an LO *). This is what made exit_warp tractable
  without the real LO/XFM/VOL classes; replacing the views with sly2/lo.h classes and real virtual calls is a later
  step that needs the slot numbering checked.
- Wave 4: a shared header may define something another shared header also defines: sly2/basic.h (generated) already
  had `enum TAGK`, and it also defines CFrame and CLOCK, so sly2/cref.h cannot include it. Both now define TAGK under
  one guard (`SLY2_ENUM_TAGK`). Check `grep -n "enum X\|struct X" include/sly2/*.h` before adding a type to a header,
  and recompile every includer of a changed header (cref.h: nine files, identical matchtool output each time).
- Wave 4: CRef's setters are split between members with mangled symbols (SetS32, SetF32, operator=, operator==,
  Incref, Decref, SetTag) and C-named functions (CRef_ctor_*, CRef_Set*) that tools/rename.py has not named yet.
  Callers in merged files call the members as members and the C-named ones as `CRef_SetVector(&ref, ...)`; both give
  the same code as the per-file `ref = value` overloads (save).
- Wave 4: the five Splice value heaps (CSpliceHeap, 0x452188..0x4521E8) moved to sly2/slotheap.h with
  `SLY2_INSTANTIATE_SPLICE_HEAPS()`: TU ref uses them too and has the same 133-instruction static initializer as
  spliceutils. Frames live in pool 2 (`CSlotPool<0x28, 0x200>`) and procs in pool 3 (`CSlotPool<0x1C, 0x800>`)
  (CONFIRMED by eval's PvNewFrame/PvNewProc), methods in pool 6 (`CSlotPool<0x14, 0x100>`, ref's Decref).
- Wave 4: linkonce destructors with in-charge flags appear as plain `func_` functions: eval's func_00257078 is
  `~CFrame` (frees into pool 2 when `flags & 1`), func_002572E0 the parent-link destructor, ref's func_00257148
  `~CProc`. A `delete pframe` in a merged file calls them through the class's out-of-line destructor.
- Wave 4: the `SLY2_CC`/`-G0` overrides of eval's GcFrame_InitNode and GcProc_InitNode were not needed in the merged
  file either.
- tools/rename.py (wave 4 follow-up): it now always rebuilds the file that matches the renamed address (its
  matched.csv source) and refuses a rename whose definition there is not spelled with the old symbol (a C++ member
  such as `DMAS::AllocSw`). A merged TU is rebuilt whole and every function of it is compared by address.
- `_GLOBAL_$I$` stubs named after a global (`_GLOBAL_$I$g_dro`, `_GLOBAL_$I$g_gc`): g++ names the stub after the
  first global the original file defined; the `_GLOBAL_$I$*` alias handles them like the `func_` ones.
- Finalize hazards (wave 3): (1) never edit a file inside the TU directory in place while experimenting; `git rm`
  then refuses, and the scratch finalize had already rewritten matched.csv (vifs needed a second commit,
  bb97924b). The scratch helper now runs `git rm` first. (2) For a TU with duplicate rows, count the functions
  after removing the duplicates: the bis commit subject says 43 (rows) instead of 34 (functions); the header was
  corrected in a follow-up commit.
- Wave 5 (asm TUs): the per-function views of one object were unified into one TU-private struct per object in
  every TU (WAYPT, DIRAXIS, IKREC, LSG, DZG/DZ, SWBUSY...), with asm `"m"` operands renamed to the unified fields
  (`"m"(axes[1].direction.q)` for a per-file `"m"(axes->b)`); no function changed codegen from that. Per-file layout
  asserts of the replaced views are dropped; the unified types carry their own asserts. A shared header type was
  used where its layout fits the asm (sly2/basic.h CLOCK in clock); where the asm wants qword rows (GEOM, MATRIX4 in
  geom) a TU-private type with the header's field names is used instead.
- Wave 5: a per-function `.c` defining a mangled name whose signature does not fit the code
  (`PsoHitTestLineObjects__FiP6VECTORT1iPP2SOP3LSG` takes qwords, its symbol says `VECTOR *`) stays a C-linkage
  function under its literal symbol, as bas's constructor did. Functions whose signature fits (Startup__Fv,
  IsPs2DvdInDrive__Fv, CloneGeom__FP4GEOMP7MATRIX4T0, the OSTRM constructors __5OSTRMi/__5OSTRMPvi...) became the C++
  functions that mangle to them.
- Wave 5: dynarraystr's SLY2_CC/`-G0`/`-ffast-math -mvu0-use-vf0-vf31` overrides (three linkonce instances) were not
  needed in the merged file either; its register pin (`register HENT *list asm("s1")`) stays.
- Wave 5: a per-function file's `#define` aliases (waypoint's `#define cpso L.cpso`) leak into later functions of a
  merged file; `#undef` them after the function.
- Wave 6 (23 asm TUs of 10..15 functions): 15 matched on the first compile once the views were unified; every miss
  was a declaration or C-vs-C++ difference (mismatch table). Patterns that recurred:
  - Per-function `.c` prototypes often list a callee's arguments in register order (`EvaluateShapeAtDistance(shape, &pos,
    &dir, s)`, `ChooseJtAttackTarget(p, mask, offset, &n, &v, heading, dt, cone)`, `FindSyncInterval__FffiP4MRSGPf(c, a,
    &t, t, tMax)`). Declare the callee once with its real order and pass the arguments in that order; the reordered
    calls matched everywhere (rail, cane, blend, pipe, pipe_jt), including the hand-scheduled callers.
  - Helpers with the same name and different asm in two functions of a TU must be renamed, not merged: overload
    resolution would silently pick the wrong one (hang: AnticipateJtForce's `Sub(VU_VECTOR, VU_VECTOR)` next to
    a `Sub(qword, qword)`; throw: CPUFloat/Scalar/Sub with different pins; pipe_jt: Direction/Normal). Identical
    helpers are defined once.
  - Use the shared header types when they cover the fields: mode is written against sly2/stepplayer.h's JTMODE,
    JTMODEHUD, STEPPLAYER and POCTL (the per-file P/S views mapped onto them field for field); a type that the
    headers only forward-declare (WIDGET) is completed in the TU as a view. turret and sidebag use sly2/cref.h's
    CRef in place of private ones (asm-labelled copy constructors go away).
  - When per-function files read one field in different forms (bsp: a node's surface as `SURF *`, as `qword *` and
    as a pointer to rebase), an anonymous union keeps each read as it was; MEM forms matter in the hand-tuned
    functions.
  - A file that needs a TU-private VECTOR/qword can drop sly2/basic.h and declare the one enum it used (bsp: LSGK).
  - Literal-named functions whose signature fits became the C++ functions that mangle to them (frm: nine; prog:
    End/Draw).
  - The SLY2_CC/-G0/-ffast-math overrides of sidebag's linkonce helpers were not needed either.
  - dzg: dzgcore's DZG cannot simply grow: SolveInequalities has a DZG on its stack, and a 0x64-byte DZG changes its
    frame (0x130 -> 0x150, 28 of 86 off).
- Wave 7 (20 asm TUs of 16..25 functions): 10 matched on the first compile once the views were unified, the rest
  after one or two fixes from the mismatch table. What recurred:
  - Shared headers where they cover the fields: brx (sly2/lo.h, so.h, sw.h), xform (xfm.h XFM, so.h XF/SO), joy
    (joy.h JOY, every per-file view mapped onto its named fields), missile (a Lilly BOMB/MISSILE grown with the
    C files' fields). Real LO virtual calls gave the same code as the hand-made vtable calls in every case tried
    (brx, xform, missile). Slots checked against lo.h's declaration order: 7 Init, 11 AddHierarchy, 14 LoadFromBrx,
    23 Unfreeze, 25 SetIdProxy, 29 TranslateToPos, 30 RotateToMat, 31 PmatWorld, 32 PxfLocal, 33 GetVelocity,
    37 ApplyProxy; slot 40 is the one CloneLoWithParent calls "clone from" (lo.h names it CloneHierarchy, and Clone is 41).
    xfm.h names XFM slots 49/50 without parameters, so those stay hand calls (no header change).
  - BHV layout (bhvstk): vtable +0xC, owner +0x10, the behaviour below +0x14, the behaviour above +0x18 (what CloseBhv
    pops), busy flag +0x1C, busy count +0x20; BHVSTK {owner, depth, eight 0xF0-byte slots} at +0x26C of a guard. The
    "+0x18 running sub-behaviour / stack" union of bhvsearch, dimitri and carmelita is that pointer.
  - vec has no object views but 20 per-file helper sets whose names collide with different asm (Sub/Cross/Dot with
    other operand orders, allocation-only operands, clobbers). Each function's helpers stay as matched in a namespace
    named after the function (`namespace V_GetNormalVector { ... extern "C" qword GetNormalVector(...) {...} }`);
    extern "C" definitions inside a namespace keep their plain symbols, and external declarations go to one
    extern "C" block at file scope (global variables declared inside a namespace would be mangled). No codegen change.
    Elsewhere colliding helpers were renamed (water SubB/ZeroXyz, sticky VUFM/VUFQ, spawner GetXV/SqrtV).
  - The SLY2_CC/-G0/-ffast-math overrides of xform (six files) and memory's two linkonce setters were not needed.
  - Per-file views often typed one field two ways (int and pointer, a time and an id): dimitri's +0x4C was typed
    int but func_00155A68 reads the object's position from it. When a unified field is read as a pointer anywhere,
    type it as a pointer; anonymous unions where both reads matter (dimitri's BHVO, water's motion node).
  - g++ 2.95's symbol for `::operator delete(void *)` is `__builtin_delete`: memory defines it as the operator.
- Wave 8 (22 asm TUs of 21..38 functions, inline-asm TUs smallest first; dzg, jthands, spliceobj and sensor left for
  a dedicated pass): gait, shdanim, breakable and menu matched on the first compile once the views were unified,
  the rest after one to four fixes from the mismatch table. What recurred:
  - Most mismatches were per-caller views of one callee (argument order or count, return type) and asm outputs
    written into union or struct members; both have rows in the table above.
  - Shared helpers used where identical: vu0.h VuSub/VuSubRef/VuDot (blip), VuSub (critter), VuAdd/VuSub
    (recon); util.h GMax (font), GAbs (solve, util, tn), GClamp (util, rhythm), GMin (tn), FIsPtr (gadget).
    A helper with the same name but another form stays TU-private and renamed (rhythm and po GMaxR `a < b ? b : a`;
    mat's three sqrt helpers, volatile and not).
  - Header types where they cover the fields: basic.h CLOCK (asega), SMP/SMPA/CLQ/LM/RGBA/VECTOR (util), aseg.h and
    dli.h (aseg, asega), stepplayer.h GAIT (gait), xfm.h TN, cm.h CMPART2/CM and UpdateCptn's CPD for all of tn,
    gifs.h GIFS for both GIFS users of mpeg (one had declared a non-virtual private copy).
  - A header move was reverted: adding CHN/OVR/OVRA/AC/ACTSEG to aseg.h breaks about 15 per-function Lilly ports
    that define their own CHN/OVR/OVRA/KEY, so ACTSEG/OVRA/AC stay identical TU-private copies in aseg.cpp and
    asega.cpp.
  - basic.cpp keeps private views of BASIC/LO on purpose: defining BASIC's members there would make it BASIC's key TU.
  - mpeg's StaticInit stores the audio output's vtable into the global player before the pools (as heap's level heap
    does): the player D_002F66B0 is defined in mpeg.cpp with an inline constructor that stores the address by hand.
  - The callback PMF passed by value from `.c` files is a packed long (`struct LONGU { long l; }
    __attribute__((packed)); extern const LONGU D_...;` passing `.l`): aseg, booty, tn, rhythm.
  - po's linkonce PemitbEnsureEmitter matched without its file's SLY2_CC / -G0 / -ffast-math overrides.
- Drafting tool hazard: never `git stash`/`stash pop` between finalising a TU and committing it. `stash pop` restores
  the files but not the index, so the commit took only the new `.cpp` (wave 2's draw commit had to be rebuilt).
