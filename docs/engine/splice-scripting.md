# Splice scripting runtime

Splice is Sucker Punch's Scheme-like level-scripting language, inherited from Sly 1. Levels carry compiled Splice
data; objects carry a Splice frame of bindings; scripts react to events, read and write object options, and
build higher-level structures such as RELAY sequences ([cutscenes](cutscenes.md)) and JOB/TASK records ([game-progression](game-progression.md)).

## 1. Layout of the runtime

| Part | Where | Notes | Tag |
|---|---|---|---|
| Native binding thunks | 0x1002A8-0x110A28, `src/splicewrap/` (~400) | convert script args (`RefSetArgsFromSplice` 0x201360) and call an engine function | [C:static] |
| Builtins (BIF) | bif.cpp 0x11F440-0x1258F0 | 169-entry table D_002BBB90 | [C:verified] |
| Nil-stub builtins | 0x1258F0-0x126188 | 29 byte-identical builtins returning Nil (slots 140-168): compiled-out dev builtins? | [H r1 bif.md] |
| Evaluator | eval.cpp 0x126438-0x127B18 | Sly 1 eval.cpp one-to-one | [C:verified] |
| Values | ref.cpp 0x1291A8-0x12A3D8 (`CRef`) | 8-byte tagged value | [C:verified] |
| Frames, GC, pairs, procs, sidebags, serialize, utils, varb | 0x127B18-0x12B3B0 | Sly 1 splice/*.cpp in the same alphabetical order | [H r1 splice_lib.md] |
| Level Splice section | 0x1FF7E8-0x1FFCF8 (splicemap?) | `InitSwSpliceFrame` 0x1FF7E8, `LoadSwSpliceFromBrx` 0x1FF858, `DeleteSwSplice` 0x1FF830, `PframeFromIsplice` 0x1FFA40 (used by PloNew) | [H r7 misc_small_tus.md] |
| Object options | spliceobj.cpp 0x1FFCF8-0x201620 | get/set object properties by option id | [H r7 spliceobj.md] |
| Allocators | 0x256B48-0x257FC8 | `PxxxNew`, heap unmark/free/shutdown (moved out of the splice TUs) | [H r1 splice_lib.md] |

Every TU's static-init stub registers the Splice slot-heap pools (pair nodes 0x14 B x 0x2000 at D_004520A8, and pools of
0x18/0x28/0x1C/0xC/0x14 bytes), which is why 209 TUs end with the same two stubs `[H r1 splice_lib.md, core_lib.md]`.

## 2. Values: CRef and tags

`CRef` = `{TAGK tagk; union {int n; float g; void *pv;}}` `[C:static]`. Boxed kinds live in refcounted slot-heap
boxes; pairs/procs/methods carry a refcount at +0; objects (`BASIC`) are refcounted through vtable slot 1.
Sly 2 renumbered the tags (one special form inserted at 6) `[C:verified]` (verify B: tag values are certain):

| Tag | Kind | | Tag | Kind |
|---|---|---|---|---|
| 0x15 | S32 | | 0x1C | CLQ (quadratic) |
| 0x16 | F32 | | 0x1D | LM (limits) |
| 0x17 | Bool | | 0x1E | SMP (smoothing) |
| 0x18 | Symid | | 0x1F | Pair |
| 0x19 | Bifk (builtin id) | | 0x20 | Proc |
| 0x1A | Vector | | 0x21 | Basic (object) |
| 0x1B | Matrix | | 0x22 Method, 0x23 ?, 0x24 Void, -1 Nil | |

Special forms (head tags) `[C:verified]`: 1 set!, 2 define, 5 assert, 6 (new, unknown), 7 if, 8 or, 9 and, 10 cond, 11 else,
12 case, 13 let, 14 while, 15 lambda, 16 begin, 17 import (returns Nil: resolved at load time), 18 quote. Sly 1's macro forms (3/4) have no handler.

## 3. The evaluator

`RefEval` 0x127838: atoms evaluate to themselves, symbols via `RefEvalSymbol`, lists dispatch on the head tag;
anything else is an application (`RefEvalApply` 0x1272E0): builtin via the table (checks `cargMin`/`fRest`), method
(tag 0x22), or a lambda in a fresh frame whose parent is the closure frame `[C:verified]`.
Frames: `{cref, cframe parents, apframe[], bindings}`; `PrefFindBinding` recurses into parents with a busy flag to break cycles `[H r1 splice_lib.md]`.
GC: `CGc` (g_gc; TU 0x128138) marks live objects from roots and collects at level transitions `[H r1 splice_lib.md]`. Fix: `_GLOBAL_$I$g_gc` sits on frame.cpp's stub 0x128118; g_gc's own stub is 0x128B40.

**Evaluator quirks** (splice_vm, ported from the matched C and diff-tested) `[C:static]`:
- `if` (0x126758) takes the THEN branch only when the payload word **== 1**, whatever the tag: `(if 2 ..)` and `(if '(x) ..)` take the else branch.
  `and`/`or`/`cond`/`not` instead treat only Bool #f as false.
- `while` (0x126F78) evaluates **only the first body expression** and always returns Void.
- `+` and `-` accumulate ints in a **float** (`(+ 16777216 1)` = 16777216); `(- 5)` = 5; `=` compares raw payload words, so `(= 1 1.0)` is #f.
- **Tag 20 (0x14) is the rest-parameter dot** in lambda lists: `cargMin` = params before it, `fRest` = whether it is there (0x127068). Extra args beyond
  cargMin are never evaluated unless fRest.
- Unbound symbols evaluate to nil without error; `set!` on an unbound name does nothing; `define` prepends (shadows).

## 4. Builtin table D_002BBB90

169 records `{PFN pfn; int cargMin; int fRest;}`, index = BIFK, signature `CRef RefOpXxx(int carg, CRef *aref, CFrame *pframe)` `[C:verified]`.
Compared with Sly 1's 125-entry table (which gives the Splice spellings) `[H r1 bif.md]` (slot mapping verified on code):

| Slots | Content |
|---|---|
| 0-40 | identical to Sly 1 (`+` .. `list`) |
| 41 | new: list shuffle |
| 42-52 | `append` .. `get-element` (shifted +1); `vector`/`matrix` now varargs; `random-seed` removed |
| 53-95 | `random` .. `modulo`; 54-61 unary math via `RefUfo` 0x1224A8; 62 `atan2` |
| 96 | `current-time` (g_clock.t) |
| 97-104 | new zero-arg getters (load filter / level index D_003E1110 and two variants, the SW job/task slots +0x3B0C..+0x3B14, a difficulty float, a stub) |
| 105-107 | `schedule-callback`, new cancel-callback, `defer-object-update` |
| 108-111 | `addo`/`ensureo`/`seto!`/`geto` (object options) |
| 112-122 | find-object family (optional search-root arg; 114 new: list of OIDs -> objects; 117 g_sw, 118 g_sndmgr, 119 g_cm) |
| 123-130 | hit-test, convert-object-position/vector/matrix, near-clip-center, start-sound, start-rumble |
| 131-139 | new: get/set save var (path list), find-binding-or-noop, no-op, bitwise and/or, object D_003DA0C0, power-up acquired get/set (`powerUps.mppukfAcquired`) |
| 140-168 | nil stubs (Sly 1's stop-sound, emit-smoke-cloud, set-music-register... may be among them) |

## 5. Objects and scripts

- **Options.** Objects expose numbered properties through the option table `g_aeopid` D_002AC880 (0x28-byte records:
  otyp, grf, field offset or getter/setter pointer-to-member pairs) `[C:static]`. Level files set them at load
  (`LoadOptionsFromBrx` 0x13F810, method options when `grf & 0x400`) and scripts read/write them with
  `RefGetPeopid`/`RefSetPeopid`; `PeopidFind` binary-searches a per-class table D_00279AD0; ids above 0x4D0 go to the
  sidebag (`RefGet/Set/Add/EnsureOption`) `[H r7 spliceobj.md]`. Sly 2 option indices do not line up with Sly 1 OPTIDs
  `[C:static r7b]`. Examples: g_cm 161-173 (fov, clip, fog, pos, rot) `[C:static r2 cm.md]`; STEP 224 spin ratio, 225 gravity ratio; STEPGUARD 252/253 hit values, 260 target `[H r7b]`.
- **Sidebag setters** `EnsureLoSidebagBool/Int/Float/Clq/Lm/Oid/Vector` 0x132BC8.. (Sly 1 order) `[H r2b basic.md]`.
- **Events.** `HandleSpliceEvent` (BASIC slot 3, 0x1331F8): if the object's frame binds the event symbol, build the
  argument list (`s_aotypEvtParm`) and evaluate now, or schedule it (`fPost`) into the time-sorted list `[H r2b basic.md]`.
  Event numbers seen: 0/1 post-load and per-update (LO PostLoad/Update), 5 trigger, 6 untrigger (buttons, barriers),
  0x1A STEPPLAYER slot 104, 0x1B bomb thrown `[H r2b NOTES]`.
- **Scheduled callbacks**: `ScheduleSwSpliceCallback` 0x133040 / `CancelSwSpliceCallback` 0x133130; consumed by `ProcessSwCallbacks` ([world-sw](world-sw.md#6-deferred-callbacks-and-the-splice-schedule)).
- **Save variables** are CRef-valued: `RefGetSaveVarV("name", ..., NULL)` ([save-memcard](save-memcard.md)).
- **No names on disc.** The splice symbol table at 0x2B3B00 holds hashed ids, not strings, so thunk targets cannot be
  named from Splice spellings `[H r2b NOTES]`. But symids 30-989 are named by the save-schema symbol table in every world file, and the whole game's
  scripts decode and round-trip byte-for-byte (10,930 entries in 43 files) `[C:parse]`: see [formats/splice-scripts.md](../formats/splice-scripts.md)
  for the serialized form, event numbering (symids 0-28 are events), option labels and the decoder.
- **Builtins resolved by the top-50 round** `[H top50]` (code-read): 97 current level index (was "load-filter~"), 98 current episode, 99 current world,
  100/101/102 current JOB / current TASK / pending TASK (g_sw+0x3B0C/+0x3B10/+0x3B14, confirmed live by runtime6), 103 dynamic difficulty, 137 the HUD hint box
  (D_003DA0C0, `find-hintbox-object~`). Option 251 = ASEGA/SMA root object, 253 = ASEGA speed, 380/381 = hpMax/hp, 1093 = a task's job, 446 = start follow
  (STEPPLAYER) / root BHV point (STEPGUARD): optids are polymorphic per class.
- **Live scripts** `[C:runtime runtime6]`: all mission JOB/TASK scripts live in WE `f_nightclub_macro`; handlers seen bound in live frames: evt0 post-load,
  evt17 game-event, evt19 activate, evt20 deactivate, evt6 untrigger. Cutscene RELAYs have no Splice of their own.

## 6. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x11F440-0x1258F0 | bif | high | Splice builtins; BIF table D_002BBB90 |
| 0x1258F0-0x126188 | bif2 | low | nil-returning stub builtins 140-168; 29 nil-returning builtins |
| 0x126438-0x127B18 | eval | high |  |
| 0x127B18-0x128138 | frame | high |  |
| 0x128138-0x128B60 | gc | medium | constructs g_gc |
| 0x128B60-0x128CB8 | method | low | empty TU; method(?) |
| 0x128CB8-0x128F70 | pair | medium |  |
| 0x128F70-0x1291A8 | proc | medium |  |
| 0x1291A8-0x12A3D8 | ref | high |  |
| 0x12A3D8-0x12A980 | serialize | medium |  |
| 0x12A980-0x12AE18 | sidebag | medium |  |
| 0x12AE18-0x12B0E8 | spliceutils | medium |  |
| 0x12B0E8-0x12B3B0 | varb | medium |  |
| 0x1FF7E8-0x1FFCF8 | splicemap | low | splice-section load |
| 0x1FFCF8-0x201620 | spliceobj | high |  |

## 7. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x120370](../../src/hand/func_00120370.c) | `RefOpIntEqual` | Splice builtin "=": bool(tag0==tag1 payload ints equal) | 0.93 | T1 | verified |
| [0x120398](../../src/hand/func_00120398.c) | `RefCmp` | Numeric compare of aref[0],aref[1] (int->float promote) by CMPK 0..3 (<,<=,>,>=) -> Bool | 0.92 | T1 | verified |
| [0x120488](../../src/auto/func_00120488.c) | `RefOpL` | Splice builtin "<": RefCmp(aref,0) < | 0.93 | T1 | verified |
| [0x1204B8](../../src/auto/func_001204B8.c) | `RefOpLE` | Splice builtin "<=": RefCmp(aref,1) <= | 0.93 | T1 | verified |
| [0x1204E8](../../src/auto/func_001204E8.c) | `RefOpG` | Splice builtin ">": RefCmp(aref,2) > | 0.93 | T1 | verified |
| [0x120518](../../src/auto/func_00120518.c) | `RefOpGE` | Splice builtin ">=": RefCmp(aref,3) >= | 0.93 | T1 | verified |
| [0x120548](../../src/hand/func_00120548.cpp) | `RefOpEqv` | Splice builtin "eqv?": shallow eqv: same tag and same payload / vector compare | 0.93 | T1 | verified |
| [0x1205B8](../../src/hand/func_001205B8.cpp) | `RefEqualHelper` | Recursive structural equality (lists, vectors, matrices) -> Bool | 0.90 | T1 | verified |
| [0x1207F0](../../src/auto/func_001207F0.c) | `RefOpEqual` | Splice builtin "equal?": calls RefEqualHelper (recursive) | 0.93 | T1 | verified |
| [0x1208A0](../../src/hand/func_001208A0.c) | `RefOpIsSymbol` | Splice builtin "symbol?": tag==0x18 | 0.93 | T1 | verified |
| [0x120A20](../../src/hand/func_00120A20.cpp) | `RefOpIsProcedure` | Splice builtin "procedure?": tag==0x20\|\|0x19 | 0.93 | T1 | verified |
| [0x120A58](../../src/hand/func_00120A58.c) | `RefOpAreNear` | Splice builtin "near?": FFloatsNear(a,b,eps) | 0.93 | T1 | verified |
| [0x120B08](../../src/hand/func_00120B08.cpp) | `RefOpCons` | Splice builtin "cons": new pair node (ref=a0, next=a1 if list) | 0.93 | T1 | verified |
| [0x120CC8](../../src/hand/func_00120CC8.c) | `RefOpSetCar` | Splice builtin "set-car!": RefOpSetCadr(...,0x23) | 0.93 | T1 | verified |
| [0x120CF8](../../src/hand/func_00120CF8.c) | `RefOpSetCdr` | Splice builtin "set-cdr!": RefOpSetCadr(...,0x24) | 0.93 | T1 | verified |
| [0x120E10](../../src/hand/func_00120E10.cpp) | `RefOpIsMember` | Splice builtin "member?": loop calling RefOpEqual on each element | 0.93 | T1 | verified |
| [0x120F80](../../src/auto/func_00120F80.c) | `RefOpList` | Splice builtin "list": returns the arg list itself (varargs) | 0.93 | T1 | verified |
| [0x1212F8](../../src/hand/func_001212F8.cpp) | `RefOpAppend` | Splice builtin "append": concatenates list args (copies all but last) | 0.93 | T1 | verified |
| [0x121710](../../src/hand/func_00121710.cpp) | `RefOpMap` | Splice builtin "map": builds (proc a b) via helper 0x121578, eval 0x127838 per elem, collects results | 0.93 | T1 | verified |
| [0x121938](../../src/hand/func_00121938.cpp) | `RefOpFilter` | Splice builtin "filter": same, keeps elems where result true | 0.93 | T1 | verified |
| [0x121F00](../../src/hand/func_00121F00.cpp) | `RefOpMatrix` | Splice builtin "matrix": 0/1/3/4 args -> MATRIX4 identity-based | 0.93 | T1 | verified |
| [0x1220B8](../../src/hand/func_001220B8.c) | `RefOpClq` | Splice builtin "clq": 3 floats -> tag 0x1C | 0.93 | T1 | verified |
| [0x122168](../../src/hand/func_00122168.c) | `RefOpLm` | Splice builtin "lm": 2 floats -> tag 0x1D | 0.93 | T1 | verified |
| [0x122530](../../src/auto/func_00122530.c) | `RefOpSqrt` | Splice builtin "sqrt": RefUfo(arg,0) | 0.93 | T1 | verified |
| [0x122560](../../src/auto/func_00122560.c) | `RefOpSin` | Splice builtin "sin": RefUfo(arg,1) | 0.93 | T1 | verified |
| [0x122590](../../src/auto/func_00122590.c) | `RefOpCos` | Splice builtin "cos": RefUfo(arg,2) | 0.93 | T1 | verified |
| [0x1225C0](../../src/auto/func_001225C0.c) | `RefOpTan` | Splice builtin "tan": RefUfo(arg,3) | 0.93 | T1 | verified |
| [0x1225F0](../../src/auto/func_001225F0.c) | `RefOpAsin` | Splice builtin "asin": RefUfo(arg,4) | 0.93 | T1 | verified |
| [0x122620](../../src/auto/func_00122620.c) | `RefOpAcos` | Splice builtin "acos": RefUfo(arg,5) | 0.93 | T1 | verified |
| [0x122650](../../src/auto/func_00122650.c) | `RefOpAtan` | Splice builtin "atan": RefUfo(arg,6) | 0.93 | T1 | verified |
| [0x122680](../../src/auto/func_00122680.c) | `RefOpRadNormalize` | Splice builtin "rad-normalize": RefUfo(arg,7) | 0.93 | T1 | verified |
| [0x1228F0](../../src/hand/func_001228F0.cpp) | `RefOpVectorDistanceSquared` | Splice builtin "vector-distance-squared": squared distance | 0.93 | T1 | verified |
| [0x122DE0](../../src/hand/func_00122DE0.c) | `RefOpMatrixTranspose` | Splice builtin "matrix-transpose": 0x11C1D8 | 0.93 | T1 | verified |
| [0x123118](../../src/hand/func_00123118.cpp) | `RefOpMatrixDecomposeToRadianNormal` | Splice builtin "matrix-decompose-to-radian-normal": list(rad, normal) via DecomposeRotateMatrixRad | 0.93 | T1 | verified |
| [0x123498](../../src/hand/func_00123498.cpp) | `RefOpClqFit` | Splice builtin "clq-fit": FitClq | 0.93 | T1 | verified |
| [0x123688](../../src/hand/func_00123688.c) | `RefOpCeiling` | Splice builtin "ceiling": (int)(g+1) | 0.93 | T1 | verified |
| [0x1236F8](../../src/hand/func_001236F8.c) | `RefOpAbs` | Splice builtin "abs": abs int/float | 0.93 | T1 | verified |
| [0x123EB0](../../src/hand/func_00123EB0.cpp) | `RefOpObjectOption` | Shared body of addo/ensureo/seto!/geto: switch on BIFK 0x6C..0x6F -> RefAddOption/RefEnsureOption/RefSetOption/RefGetOption | 0.95 | T1 | verified |
| [0x123FB8](../../src/hand/func_00123FB8.c) | `RefOpAddO` | Splice builtin "addo": RefOpObjectOption(op 0x6C) -> RefAddOption | 0.93 | T1 | verified |
| [0x123FE8](../../src/hand/func_00123FE8.c) | `RefOpEnsureO` | Splice builtin "ensureo": op 0x6D RefEnsureOption | 0.93 | T1 | verified |
| [0x124018](../../src/hand/func_00124018.c) | `RefOpSetO` | Splice builtin "seto!": op 0x6E RefSetOption | 0.93 | T1 | verified |
| [0x124048](../../src/hand/func_00124048.c) | `RefOpGetO` | Splice builtin "geto": op 0x6F RefGetOption | 0.93 | T1 | verified |
| [0x124578](../../src/hand/func_00124578.cpp) | `RefOpFindNearestObjects` | Splice builtin "find-nearest-objects": 0x15E678 k=4 + RefPairFromAplo | 0.85 | T1 | verified |
| [0x124F00](../../src/hand/func_00124F00.cpp) | `RefOpHitTestObjects` | Splice builtin "hit-test-objects": RefOpHitTestObjectsImpl(0x7B=own BIFK) -> list of (lo pos normal) | 0.85 | T1 | verified |
| [0x125060](../../src/hand/func_00125060.c) | `RefOpConvertObjectVector` | Splice builtin "convert-object-vector": ConvertAloVec 0x1A6FC0 | 0.85 | T1 | verified |
| [0x126490](../../src/hand/func_00126490.cpp) | `RefEvalSet` | (set! sym expr): evaluate expr, CFrame::RefSetBinding (0x127CF0) | 0.90 | T1 | verified |
| [0x126F78](../../src/hand/func_00126F78.cpp) | `RefEvalWhile` | (while cond body) | 0.92 | T1 | verified |
| [0x1272E0](../../src/hand/func_001272E0.cpp) | `RefEvalApply` | Function application: evaluates operator; Bifk -> D_002BBB90 table {pfn,cargMin,fRest}; Method; or lambda (new frame binding params, rest list); then  | 0.93 | T1 |  |
| [0x127838](../../src/hand/func_00127838.cpp) | `RefEval` | Core evaluator: self-evaluating atoms, symbols, special-form dispatch on head TAGK (1..18), else apply; import (17) returns Nil, quote (18) returns da | 0.95 | T1 |  |
| [0x127CF0](../../src/hand/func_00127CF0.c) | `CFrame_RefSetBinding` | PrefFindBinding(sym,1) and assign; returns Void | 0.88 | T1 | verified |
| [0x127D78](../../src/hand/func_00127D78.c) | `CFrame_FFindBinding` | PrefFindBinding wrapper: copies binding into *pref, returns found | 0.85 | T1 | verified |
| [0x128148](../../src/auto/func_00128148.c) | `CGc_UpdateRecyclable` | Resets 6 splotheap 'recyclable' cursors (D_002DD288=D_002DD280 ... stride 0x10) | 0.85 | T1 | verified |
| [0x129248](../../src/hand/func_00129248.c) | `CRef_ctor` | Constructor: tag 0x1A from a vector passed by value in a qword register | 0.85 | T1 | verified |
| [0x129518](../../src/hand/func_00129518.c) | `CRef_SetS32` | Decref then tag 0x15 = n | 0.85 | T1 | verified |
| [0x129558](../../src/hand/func_00129558.c) | `CRef_SetF32` | Decref then tag 0x16 = g | 0.85 | T1 | verified |
| [0x129598](../../src/hand/func_00129598.c) | `CRef_SetBool` | Decref then tag 0x17 | 0.85 | T1 | verified |
| [0x129618](../../src/hand/func_00129618.c) | `CRef_SetBifk` | Decref then tag 0x19 | 0.85 | T1 | verified |
| [0x129808](../../src/hand/func_00129808.c) | `CRef_SetClq` | Decref, tag 0x1C | 0.85 | T1 | verified |
| [0x12A3D8](../../src/hand/func_0012A3D8.cpp) | `PpairSerializeIn` | Deserializes a compiled Splice expression tree from the level stream (typed U8 tag, S32/S16/F32/U32 payloads, jump table 0x2A4500) | 0.97 | T1 | parse, format-doc |
| [0x200598](../../src/hand/func_00200598.cpp) | `RefSetPeopid` | Setter twin: write a Splice value into an object option | 0.85 | T1 | verified |

Selected 60 of 268 named functions (T1 with confidence >= 0.85, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/splice-scripting.md](../reference/functions/splice-scripting.md).

## Open questions
- Special form 6 (candidates: a debug form, `when`). Experiment: break 0x1266C8 during level load and dump the head list.
- Spellings of builtins 104, 114, 118, 133 (97-103 and 137 have roles); the 29 stubs 140-168 are never called by any of the 43 levels' scripts `[C:parse]`.
- Tag 0x23 (compared by value in `operator==`).
- Which of the six static pools is frame/proc/method/sidebag.
