# View unification history (archive)

The per-round records of two cleanup phases, moved here verbatim from [tu-merging.md](../guides/tu-merging.md) when
that page became a guide: the rounds that emitted the registry vtables from their key TUs ("Vtables emitted"), and the
view-unification rounds, one subsection per family round ("Views unified"). Each subsection is named after the branch
that did the work. The only edits are source file names: the address-named `tu_XXXXXX.cpp` files were later renamed
after what they hold (robottank.cpp, hack.cpp, ...), and the text uses the current names. The method, verification
and traps distilled from these rounds are in tu-merging.md, "Views unified" and "Emitting vtables (key functions)";
the decisions they led to are in [decisions.md](../decisions.md). The "Left" and "Not attempted" lists are the work
list for further unification; some items have been done since (checked against git history, not here).

Section references such as "Representation decisions", "Phase 4" or "The container family" point to tu-merging.md.

## Vtables emitted

### Vtables emitted (data/vtables)

The registry classes' vtables (slice 2 made the classes real, but nothing emitted their vtables) come from their key
TUs now, as the original build made them: 174 vtables (65,040 bytes; 10 of them the grab-spot classes' secondary
`_vt$<CLASS>$8GRABSPOT` tables, emitted with their primary) in 81 TUs, each MATCHing the ELF slot for slot. Every
touched file keeps all functions and data MATCHing; nm symbol sets change only by the new `_vt$` definitions and the
slot functions they reference (all config names); symcheck is identical to the branch start (35 unresolved, 0
duplicate strong globals: the vtables are weak `.gnu.linkonce.d` definitions); relverify 37,168 relocations OK, 0
mismatched.

**The mechanism.** g++ 2.95 emits a class's vtable (in `.gnu.linkonce.d._vt$...`) in the TU that defines the class's
key function, its first declared non-inline, non-pure virtual. For each class: the key TU is the TU whose place in TU
order matches the vtable's place in the ELF's linkonce area (the area is in TU order; where a class's own virtuals
are defined in several TUs the order decides: HND is hnd.cpp's, not grabspot's, because GRABSPOT's own vtable
(0x2D28D8, grabspot's) lies between; JT jt.cpp's, SO so.cpp's, XFM xform.cpp's: both candidates fit, the one with the
class's functions was taken). In that TU one virtual whose function the TU defines as a C-named function becomes the
member definition, and its declaration moves to the front of the class (sly2/classes.h or the class's header):

```cpp
// was: extern "C" void InitContessa(GUARD *pguard) { InitStepguard(pguard); pguard->n4A4 = 1; }
void CONTESSA::Init()                     // header: virtual void Init() __asm__("InitContessa"); declared first
{
      GUARDV *pguard = (GUARDV *)this;    // the TU's view, renamed (see below)
      InitStepguard(pguard);
      pguard->n4A4 = 1;
}
```

The member's asm label is the function's own symbol, the body is unchanged, and the view pointer is a cast of `this`:
no conversion changed code. Further parameters keep their names (`MSGID msgid_m` and `int msgid = (int)msgid_m;` where
the view's type differs). A function in a per-function namespace is moved out of it and opens it
(`using namespace M_x; M_x::S *p = (M_x::S *)this;`), with using-declarations for the names the namespace shares with
the file scope (`using M_x::D_003DD490;`) and a qualified call where a class member hides a free function
(shape.cpp: `M_func_001E9AC0::Add`, hidden by LO::Add). Shortest bodies at file scope were taken first.

Rules found on the way (each one failed once):

- **Only overrides may move.** New virtuals take their slots in declaration order, so the moved declaration must be
  an override or a new virtual preceded only by overrides. lo.cpp's first pass moved Freeze (a new virtual of LO) and
  shifted every LO-derived vtable; step.cpp's moved VUnk84. matchtool's data check did not show it: it masks the
  relocated words, and a vtable is nothing but relocated words and zero deltas. Each vtable was therefore also checked
  slot by slot (scratch vtcheck.py: every R_MIPS_32 target's config address against the ELF word, and the section
  size against config), and link.py's comparison checks the same.
- **Inline member functions are emitted in the key TU.** g++ 2.95 gives a class's inline members their out-of-line
  copy in its key TU (global, not linkonce). BASIC's and PO's constructors (inline in classes.h) appeared in basic.cpp
  and po.cpp: those TUs do not include classes.h (their classes come from basic.h/po.h). HACKSHIP's and ROBOTTANK's
  constructors (class_registry inlines them) are defined after their classes under `#ifndef SLY2_KEY_TU_<CLASS>`, and
  hack/robottank define the macro before including classes.h.
- **Emission order is the reverse of definition order.** A TU's vtables come out in the reverse order of their
  classes' definitions (forward declarations do not count: tried). classes.h's classes are therefore in emission
  order: each key TU's classes in descending ELF address order, bases first (a topological sort; the 13 classes whose
  key TU is class_registry first, the classes without a vtable name before them). aseg.h defines SM before SMA, and
  guard.h defines CID120 before STEPGUARD (classes.h's family, moved there so stepguard.cpp's three come out as in the
  ELF). Every key TU emits its vtables in the ELF's order.
- **A key TU needs the real class.** Each key TU includes classes.h (or only the class's own header: basic.h, po.h,
  so.h, xfm.h, aseg.h, sw.h, jt.h) after its includes; the TU-private views with the same names as anything classes.h
  brings were renamed with a `V` suffix (Representation decisions, types/views-3), C++ symbols that carry a renamed
  type keep their symbol with an asm label on the first declaration (a labelled declaration right before the
  definition where there was none), and private enums identical to basic.h's (OID, SEEK, ENSK) were removed (CID,
  signed in the views, is `CIDV`). emitter.h's level-heap allocator became `PvAllocLevelHeap(AREQ *)
  __asm__("func_001AE2A0")`: its extern "C" declaration conflicted with the merged files' own extern "C" declarations
  of func_001AE2A0 on their AREQ copies. bomb.cpp called `PrimeBomb__FP4BOMBf(...)` by its mangled name (g++ 2.95
  resolves an identifier through the assembler name); once BOMB was renamed the calls became `PrimeBomb(...)`.
  missile.cpp keeps its field views of BOMB/MISSILE/ACCMISS (they already carry the virtual layouts); its SPLMISS view
  is now `CID82` (the registry name) with CID82's virtuals, and MISSILE::Init / CID82::Init are the key functions.
- SAA's slot 7 and its six overrides take the `SAAF *` parameter their Sly 1 symbols show
  (`InitShuffle__FP7SHUFFLEP4SAAF`), so the Init functions could become the members.

Not emitted (17 config names): the 13 whose key TU is class_registry (BEARCUB, BEAR, ARPEGGIO, JEANBISON, JSG, CID118,
CID101, CID20, CID25, GUARD, EXPLO, ASEGBL, CID154: first in the linkonce area, 0x2C8940..0x2CA4D0, their only own
virtual defined anywhere is the getter; left to the class_registry work), GOMER (its one own slot 7 holds
`RenderGomerSelf__FP5GOMERP2CMP2RO`, three parameters, where the slot's member is `Init()`: no member definition
reproduces the code), and exit_warp's WARP/EXIT/CID41 (exit_warp includes bis.h, whose vec.h VECTOR (aligned) clashes
with basic.h's). Also not emitted: GRABSPOT's own vtable (0x2D28D8, grabspot.cpp's; no config name), and the
unnamed vtables between (act.cpp's curve classes, ...). config/link_aliases.txt is unchanged (42 lines; the 21
vtable lines are act.cpp's 19 namespace-scoped curve classes and hud's/memcard's function-local classes: they need
real names and `_vt$` config names first). The scratch tools of this branch: survey/keytu (key TU per class),
prep/vrename (include and V renames), convert (key function), fixamb/fixsym (ambiguities, labels), vtcheck, order
(emission order), batch (all of it per TU, commit when everything holds).

### Vtables emitted, continued (data/vtables-2)

data/vtables-2 (from link/eopid-real-refs 1a7c0130) emits 44 more vtables (10,328 bytes) and removes
config/link_aliases.txt: every name the objects reference is now bound in source or config. Each vtable is checked
slot by slot (scratch vtcheck.py: 226 vtables, 0 bad but tn's `_vt$7CMPART2`, which has no config name), every object
emits its vtables in ascending ELF order (scratch ordercheck.py) and the emitting objects come in TU order (scratch
tuorder.py: 225 vtables, 0 out of order); every function and data symbol of the 210 game sources MATCHes.

- **class_registry's 19** (0x2C8940..0x2CA550): the 13 left above plus the six in the same run without config names
  (WIDGET130, POCTL, CID150, CID149, CM, CID152: `_vt$9WIDGET130` ... added). The getters `extern "C" CLASSINFO
  *GetBearcubClassDesc()` became `CLASSINFO *BEARCUB::Pclassinfo()` (Pclassinfo is already each class's first
  virtual). Emission order needed the headers rearranged: sly2/classes.h now includes basic.h and lo.h, defines
  CID152, includes cm.h, defines CID154, then includes aseg.h, so.h, po.h, emitter.h before players.h; CID149 moved
  into so.h between CID148 and CID150, WIDGET130 after CID101; cm.h no longer includes aseg.h (it used nothing of it).
- **GOMER** (gomer.cpp): the slot-7 function's config symbol `RenderGomerSelf__FP5GOMERP2CMP2RO` carries a draft
  three-parameter signature, but the code only forwards a1/a2 to InitStepguard: `void GOMER::Init() {
  STEPGUARD::Init(); ... }` reproduces it (Init declared first in GOMER).
- **WARP, EXIT, CID41** (exit_warp.cpp): bis.h declares VECTOR/VECTOR4/MATRIX3/MATRIX4 instead of including mat.h
  (its readers take them by pointer; bis.cpp includes mat.h itself), which removes the clash of vec.h's 16-byte
  VECTOR with sly2/basic.h's. Key functions WARP::LoadFromBrx, EXIT::Init, CID41::LoadFromBrx; views EXITV/WARPV.
- **The act family** (new sly2/act.h): ACT (vptr +0x18) and AC/ACG (curves, vptr +0x0) with their kinds. Names:
  ACT, ACTSEG, AC, ACG are Sly 1's (HYPOTHESIS); ACT1..ACT9, AC0..AC5, ACG0..ACG2 are DRAFT names after the
  factories' kind numbers (func_0012E238, PacNewKind, PacNewKind2; jt.cpp's ACT1/ACT5 views use the same). act.cpp
  emits its 17 (0x2CA6A0..0x2CA9C8), asega ACTSEG, tail ACT3, ik ACT7, and since integrate/round10 blend.cpp AC5
  (0x2CB5A8) and ACG2 (0x2CB588), declared again in blend.cpp (Representation decisions; with them AC5's inline slot 4
  func_002581B8, which retires integrate/round9's file-scope pointer). The factories construct **in place** (`new
  (PvNewAc(sizeof(AC0))) AC0`, then the kind stores): as class members, the allocators and inline constructors got
  out-of-line copies in act.cpp, the classes' key TU (the rule above). Key functions defined in per-function
  namespaces moved out as the members and open the namespace (`using namespace M_x;` plus using-declarations for every
  name the namespace shares with the file scope).
- **hud's and memcard's local classes**: hud's constructor func_00211C58 is `WIDGET130::WIDGET130()` (two fields
  added to classes.h's WIDGET130, draft names). memcard's status widget is `CMcStatus : SCREEN` (DRAFT names; new
  sly2/screen.h with SCREEN's 14-slot layout from 0x2D85E8, SCREEN's constructor and ResetMode out of line), its
  vtable 0x2D36D0 emitted by memcard (key function SetMode, slot 14). ResetMode as a member, not a C declaration
  taking `MCDLG *`: the latter put the field store in the call's delay slot.
- **The rest of the alias table**: `__builtin_new` is memory.cpp's `void *operator new(unsigned int)` (config
  0x1AE4F8 renamed from func_001AE4F8); frm.cpp's frame pointer is `g_pfrmOpen` (config name at 0x2DEA4C; draw.cpp
  already used it and frm.cpp MATCHes renamed); hud's `D_C0` is the address name `D_000000C0`; brx_sw's stream view
  is `CStreamBis` and its destructor `_$_10CStreamBis` (config name of 0x184950, DRAFT class name as
  CStreamBis_Close/Refill; streambis defines it by asm label on its C definition); CPairPtr/CVarbPtr (pair.h,
  varb.h, and serialize.cpp's copy) are explicit specializations of CRefPtr (table "Views unified"). With no line
  left, config/link_aliases.txt is deleted. integrate/round10 retired tools/linkaliases.py as well: it only derived
  the address names, which link.py's `symbols()` binds itself (an absolute address for every name no object
  defines; `ADDR_NAME` moved into link.py).

Measured (fresh build/link): tools/link.py 0 differing bytes, blob bytes 845,256 -> 834,928 (data no object defines
216,056 -> 205,728), 2,238 sections placed (+44), 7,884 config names at their address; symcheck 5 unresolved (tn's
`Init__5CPLCY` and four `_gp`; 39 at the branch start: the alias table's names and the view vtables), 0 duplicate
strong globals; relverify 51,943 relocations OK, 0 mismatched (without an alias file); nm against the branch start:
only the vtables (U -> V/W), the slot functions they reference, the renamed references (`_$_8CPairPtr` ->
`_$_t7CRefPtr1Z5CPair`, the view vtables -> the real ones, `D_C0`, `func_001AE4F8`/`func_00184950`/`D_002DEA4C`
definitions under their new names) and the `_GLOBAL_$D$`/`$I$` stubs' file hashes. Not done: 62 more vtables in the
linkonce data area have no config name yet (about 7,950 bytes: STEPPLAYER's, the camera policies', the screens',
the rips', GRABSPOT's, LEAP's, WIDGET's...), the next candidates.

### Vtables emitted (data/address-names-2)

data/address-names-2 (from integrate/round10 690b28cf) gives 57 of the 62 vtables without a config name a draft
`_vt$` name (class names DRAFT where the comment says so; config/symbol_addrs.txt, the blocks after the data/vtables
ones) and emits them from their key TUs with the mechanism above, so the address-named references to them (45 of
the names link.py bound to absolute addresses) are gone. Each vtable MATCHes the ELF (matchtool's data check with
the config size; link.py's comparison checks the relocated words), every object emits its vtables in ascending ELF
order (scratch check over all objects: 287 config-named vtables in objects, 0 out of order), and every touched
file and the includers of every touched header MATCH (functions and data). Per family:

| family | vtables | key TU | key function (declared first, defined as the member) | notes |
|---|---:|---|---|---|
| camera policies (sly2/cm.h) | 8 + 1 | cplcy, tn | CPLCY::Init, CMPART1::Init, CPLOOK/CPZOOM::VSlot7, CPHANG::Deactivate, CPCAMBLEND/CPTURRET/CPTWOSHOT::Activate; tn's CMPART2::Update | CPLCY gets its seven slots (Init/Activate/Deactivate/Set/Revoke/Update/VSlot7); the classes in emission order; CMPART2's other overrides make tn's `_vt$7CMPART2` (an unreferenced extra before) the ELF's 0x2DB858; tn's unresolved `Init__5CPLCY` gone; CPCAMBLEND's unused `Enter` virtual removed |
| CPCAMERA | 1 | camera | Activate (func_0012E798) | |
| registry classes whose getter is class_registry's | 10 | noise, asega, grabspot, sensor, so, gait, shdanim, prize, hud, stepplayer | CID147::VUnk00, ASEGA::VUnk00, GRABSPOT::FGetGrabRange, SENSOR::Init, CID148::VUnk00, LEAP::HandleMessage, SAA::VSlot7, DPRIZE::FTakeDamage, WIDGET::RenderSelf, STEPPLAYER::VUnk57 | Pclassinfo (the class_registry getter) is the first virtual otherwise; an override, or the class's first new virtual, moved in front of it |
| sound instances (new sly2/snd.h) | 3 | sound | CID151::Start (StartSnd), BANKSND::VUnk00, STREAMSND::VUnk00 | CID151 moved out of classes.h (which includes snd.h at its place) with its 16 slots, 8 of them pure |
| screens (sly2/screen.h) | 10 | screen, menu, memcard | SCREENBASE::Update, HUDMETER::Update, SCREEN::PostLoad, GADGETMENU/THIEFNET::VSlot13, JOBLIST/SCRN52D::PostLoad, TITLE::SetMode, PAUSEMENU::VSlot10, CMemcardDialog::PostLoad | the 11-slot base SCREENBASE (0x2D86D0) the HUD meter and the pause menu derive from; screen.cpp's views renamed SCREENV/GADGETMENUV |
| RIP kinds | 11 | rip | RIP::Init, FLYING/SHRAPNEL::FBounce, STUCK/MATCH/SMACK/REMIT::Update, GLINT/RIPT1::Init, RIPT0::Touch, LEAF::PostEmit | rip.cpp's file-scope RIP view is the root class: its fields, then the eight VTRIP slots, so the vptr lands at +0xEC (sizeof 0xF0 asserted); RIPF for the two words past it |
| load collectors | 2 | brx | CLoadCollectorArray::Add (already a member), CLoadCollectorProxy::Add | the hand-made vtable call became a virtual call and the local proxy's vptr store the implicit constructor's: same code |
| byte-queue outputs | 2 | mpeg | CQueueOutputIop/Ipu::FAsyncDrain (the mangled names they had) | CMPEG's inline constructor gets the audio output's vptr from the member's implicit constructor |
| heaps, streams | 3 | heap, bis, streambis | CSwHeap::PvAlloc, CMemInputStream::Fill, CStreamBis::Fill | CSwHeap moved to heap.h; the level heap object's inline constructor is its own derived type's (CSwHeapG): an inline member of CSwHeap gets an out-of-line copy in the key TU; CStreamBis declared with just its two slots (streambis reads the object through its private CBinaryInputStream view) |
| relay's job-sequence instance | 1 | relay | RELAYJSG::VUnk00 (AddJsgRef) | DRAFT name; BASIC's class descriptor in slot 6, so not the registry's JSG |
| arrays (sly2/arrayv.h) | 4 | dynarraystr | CDynArrayStr::Init (func_00119BD0) | explicit instantiations of CDynArrayV/CArrayListener/CHashIndex<char *> emit the three template vtables; the CHashIndex member of CDynArrayStr, instantiated before the class, gives the ELF's order |
| EXPL | 1 | emitter | EXPL::VSlot51 (ExplodeExplExplso, empty) | |

Rules found on the way:

- **Emission order across includes**: a TU's vtables come out in the reverse order of the classes' definitions, so
  the order of the headers matters, not only of the classes in one header. tn.cpp includes cm.h before xfm.h (TN's
  vtable before CMPART2's); classes.h defines CID147 between CID117 and CID47 (noise emits CID47, CID147, CID117).
  For class templates the point that counts is the instantiation (the base of a class, a member's type, an explicit
  instantiation).
- **A C function and a member with the same assembler name coexist** in one TU (the member labelled with the C
  function's symbol, the C declaration kept for the callers): what InitCmpart1 calls through the C name is the
  member CPLCY::Init. A view class with an *inline* member whose mangled name is that symbol does not (splicemap,
  rumble, menu, booty: the copy constructor becomes a declaration).
- **Implicit constructors of the base classes leave no trace** when the base's vtable is never needed (brx's local
  CLoadCollectorProxy, CMPEG's audio output, the level heap): g++ 2.95 drops the base's vptr store, no reference to
  the base's (non-existent) vtable remains.
- Still not emitted: EMITTER's and EXPLO's template-instance tables (0x2CE680, 0x2CED18; their slot 6 is
  `GetEmitterClassDesc_linkonce<T>`, a second table per class that needs the template class the header comment
  mentions; 848 bytes).

## Views unified

### Views unified (types/views-1)

Duplicated private views replaced by one shared type where the layouts agree (code reduction, first slice). Every
change kept all functions and data of every touched file and every includer MATCHing.

| type | header | views removed | files | link aliases removed | config renames |
|---|---|---:|---:|---:|---|
| STACKSCOPE | sly2/stackscope.h | 69 | 44 (+ memory.cpp defines it) | 33 (29 namespace dtors, _$_5FRAME, _$_3CTX, amb _$_3OBJ, then _$_10STACKSCOPE) | 0x1AE4A0 `_$_10STACKSCOPE` |
| CPair, CPairPtr | sly2/pair.h | 4 files x 2 + 13 bif namespaces x 3 (with their CRef) | 6 | 18 (13 namespace, 5 per-object _$_3SPM) | 0x256D40 `_$_8CPairPtr` |
| CVarb, CVarbPtr | sly2/varb.h | 3 x 2 | 3 | 3 (per-object _$_3SPM) | 0x257250 `_$_8CVarbPtr` |
| CFrame/CProc destructors | (config only) | 0 | 7 | 2 | 0x257078 `_$_6CFrame`, 0x257148 `_$_5CProc` |
| CLOCK | sly2/clock.h | 175 | 47 | 0 | — |

Per-function namespaces emptied: none (every namespace still holds its function). Reverted: the inline-wrapper
STACKSCOPE form (15 functions off), `CPairPtr &operator=` (1 function), varb.h before the slot pools (2 static
initializers), CLOCK in six files with asserts on their views. Not attempted: CRef in the remaining per-function
views (112 definitions), VTE/VT vtable-entry views, VU_FLOAT, M3/M4, the inline-release pointer owners.
config/link_aliases.txt: 292 -> 234 lines (the view-destructor section is down to brx_sw's `_$_3OBJ`, a different
function). symcheck: unresolved references 532 -> 463 (the branch start measured 532 with air merged).

### Views unified (types/views-2): the registry classes

Slice 2: the classes whose vtables the alias table carried, made real (no data-table stopgap). Every function and data
symbol of every touched file MATCHes (class_registry 424/424 and D_002BAAE8, missile with the emitted
`_vt$7ACCMISS`); the header changes were checked by a full progress run (6,662/6,662).

| family | header | views removed | files | link aliases removed | config |
|---|---|---:|---:|---:|---|
| registry classes (BASIC/LO/XFM/SO/PO/STEP/STEPPLAYER/BHV/... all 198 but SW) | sly2/classes.h (164 + 4 helpers), basic.h, lo.h, xfm.h, so.h, po.h, step.h, stepplayer.h, jt.h, players.h, guard.h, aseg.h, task.h, cm.h, emitter.h | 198 (class_registry's `namespace REG`) | 16 | 191 (all `_vt$Q23REG...`, secondary GRABSPOT tables included) | 191 `_vt$` data names in symbol_addrs.txt |
| header virtual layouts (LO/SO hierarchy, BHV, SM, ...) | the same headers | 0 | 14 | 0 | — |
| BOMB/MISSILE/ACCMISS virtual layout | missile.cpp (views kept: fields) | 0 | 1 | 0 | `_vt$7ACCMISS` compared (MATCH) |

Labels on the header virtuals also resolve the references of the key-TU extras: jt.cpp and sm.cpp no longer emit
`_vt$2JT`/`_vt$2SM` (Pclassinfo is now their first virtual), missile.cpp's `_vt$7ACCMISS` names only real functions.
config/link_aliases.txt: 232 -> 42 lines (vtables 212 -> 21: act.cpp's 19 curve classes, hud's and memcard's local
classes). symcheck: unresolved references 439 -> 31 (the 191 registry vtables, and 217 placeholder member names the
key-TU extras of jt/sm/missile referenced); relverify: 26,974 relocations OK, 0 mismatched. LEAP/CID146 live in
their own sly2/leap.h (guard.h and players.h include it): step.h is included by gait.cpp, which keeps a
field-level LEAP view. Reverted: none of the header or registry changes changed code; the only failed forms were build errors
(a BASIC constructor defined in basic.h, `CRefPtr()` clearing, explicit specializations after the instantiation), see
Representation decisions. Not attempted: act.cpp's three curve-class factories (21 lines; the classes have no registry
entry and no header), SW (sw.h on data/interior-fields), folding the per-file views of registry classes that carry
fields (missile's BOMB/MISSILE/ACCMISS, light.cpp's LIGHT, the GRABSPOT copies in proxy/sw/xf/jt) onto the shared
classes, emitting the vtables from class_registry (getters as members).

### Views unified (types/views-3): SW, the silent cc1plus exits, the interior macros

Slice 3. Every function and data symbol of every touched file and of every includer of a touched header MATCHes
(matchtool over the 113 includers of basic.h/sw.h/cm.h; full progress run), and each touched object's nm symbol set is
unchanged against the branch start except where intended (sw.cpp references `_vt$2SW` instead of `D_002DC018`).

| change | header | views removed | files | interior macros | link aliases |
|---|---|---:|---:|---:|---:|
| SW a real class (constructor SW_ctor, vtable `_vt$2SW`, slots 6-9 labelled) | sly2/sw.h | 1 (class_registry's `REG::SW`, the last `namespace REG` view) | 3 (class_registry, sw.cpp, classes.h comment) | 0 | 0 (SW never had one: NewSw calls SW_ctor) |
| the eight files cc1plus crashed on (basic, booty, find, save, frm, pipe_jt, throw, recon) | sw.h / cm.h | their private SW, CM, BASIC, DL, CLOCK, OID/CID, qword views (removed or V-renamed, Representation decisions) | 8 | 15 | 0 |
| CFrame out of basic.h; eval, frame on sw.h | sly2/cframe.h (new) | 0 | 3 (+ proxy includes cframe.h) | 2 | 0 |
| sensor, rtcam (private enums), bif (16-byte VECTOR), po (mat.h) | sw.h / cm.h | enums, qword, bif's MATRIX4; po's mat.h include | 4 | 11 | 0 |
| SW fields: RENDERTGT artgt[2] (was _pad_338); comments carrying the files' names for slEvent/slotheapEvent/cEventWorld (basic's callback list), savevars (save's SVSTORE), the settings and episode keys, ploUnk4638/cCid37 (booty), slotheapUnk4088 (frm), dlSoTreeQueue2 (sensor) | sw.h | — | — | — | — |

**Root cause of the silent exits** (all eight files): cc1plus segfaults on the redefinition of a class with virtual
functions (two lines reproduce it; without `virtual` g++ prints the ordinary redefinition error). types/views-2 made
BASIC and the registry classes polymorphic, so a TU-private `struct BASIC`/`SW`/`LO` after a shared header became a
crash; the eight files had such views (BASIC in basic and find, SW in basic, booty and save, LO in frm and recon
once cm.h brings lo.h, CM in recon, ASEG/ASEGA in throw and pipe_jt once cm.h brings aseg.h). Not a header bug: each file's clashing views had to go. Diagnosis method in the
mismatch table ("g++ 2.95 exits with status 1 and no message").

Interior macros: 47 -> 19 (the 28 in the 14 files that could not include the headers are converted; what is left has
no header type: the sound manager, g_game, the screen objects, frm's VIFS, the class-descriptor row, see "Phase 4").
config/link_aliases.txt: 42 lines (30 alias entries) unchanged; symcheck unresolved 35 (unchanged), relverify 0
mismatched. Reverted for codegen: none (every conversion matched on its first compile once the clashes were resolved).
Not attempted: act.cpp's 19 curve-class vtables (namespace-scoped classes of PacNewKind2/PacNewKind22/func_0012E238:
move them to file scope under real names with `_vt$` names in config, as slice 2 did for the registry classes),
hud's and memcard's local classes, the 19 macros without a header type, unifying the three CFrame views.

### Views unified (names/vu-f1-base): BASIC, LO, XFM, SO, PO and the list types

The base family of the view-unification slices: private views of BASIC, LO, XFM/SHAPE/PROXY/TN, SO/SOPART/SOTREE/XF and
PO/POCTL, and the DL/DLI/SL lists. Every function and data symbol of every touched file and of every includer of the
touched headers MATCHes, and each touched object's nm symbol set is unchanged.

| change | where | how |
|---|---|---|
| hand vtable calls (`p->vt[n].pfn((char *)p + p->vt[n].d, ...)`) | prize, framecapture, frm, update, safehouse, po, rtcam, button, barrier, bomb, waypoint, busy, lo.cpp, so.cpp, relay, stepguard, xf, rip, shape, cplcy, carmelita, spawner, hack, ... | real virtual calls on the shared classes (slot -> member: LO 1..48, SO 49..75, PO 76..81); a derived view that needs a later slot declares it (`virtual void SetState(int)` = DPRIZE slot 76) |
| object views padded over the base (`char pad[0xC]; VTE *vt; char p[0x150 - 0x10]; ...` or `char pad[0x180]; ...`) | the DPRIZE family (prize), HACK family (hack: HACK : PO), RELAY (relay), BUTTON/VOLBTN, BARRIER, WAYPT, BOMB, SAFEHOUSE, CID104, RTCAM/RTO, the guard attack, PATHLINK | derived views of the shared class (`struct S : SO { int state; ... }`); the class's own fields stay in the view (their classes have no header with fields) |
| LO/SO/PO-named and V-named field views | lo.cpp and so.cpp (most of their small functions), po.cpp, busy, proxy, find, callback, game, sound, murray, jt, stepbhv, bif, ... | the shared type with its field names; bit tests on the +0x28 / +0x148 words become the bit-fields (`(int)p->isInWorld & 1` keeps the original shift/and pair) |
| DL/DLI/DLE/SL copies, local InitDli/PvNextDli | 20 files | basic.h DL/SL/DLE and dli.h DLI/InitDli/PvNextDli |
| LM/CLQ/VECTOR/VECTOR4/MATRIX4/RGBA/SMP copies identical to basic.h | 40 files | removed (synonym fields renamed: minValue/maxValue, unused, red/green/blue/alpha, fastSpeed/slowSpeed/fastDuration) |
| Lilly-port copies of family types | tn, mode, jt, so, glob, relay, grab, mark, proxy, xf, air | moved into the headers: xfm.h EDGE/SURF/BSP, PROXYINIT/PROXYSPARE/PROXYREST/PROXYSRC; so.h SUBGLOBX, XPLIST, SOBOUNDS |

Header changes (names from the views, each with its evidence in the header comment): lo.h LO +0x30 `nextSpliceTime`
(UpdateLo), bit 58 `keepsOwnVisMask`, bit 59 `clearsPlacesOnRemove`, PXR +0x24 `isCopy`; xfm.h GEOM `positions` is
`VECTOR4 *` (16-byte vertices), SHAPE `incomingTangents`/`outgoingTangents`/`segmentHint`/`chain`; so.h REGENT +0x10
`pathzone` (CID17 = PATHZONE), SOPART bit 51 `hasPlaces`, SO `unknownBits148A`/`unknownBits148B` int-typed (so.cpp's
getters sign-extend them as 32-bit fields), SOBOUNDS/SUBGLOBX/XPLIST defined.

Measured (scratch metrics.py over src/game): struct/class/union definitions 7,220 -> 6,759; family-named views 541 ->
322; hand-vptr views 495 -> 406; non-derived views padded over a base (>= 0x50) 1,388 -> 1,257; 70 files, +1,739/-2,941.

Matching-only islands kept (scratch islands.csv): frm.cpp's RAII list iterator `DLIIsland` (InitDli + explicit pop is
4/98 off), lo.cpp InitLo's `LOIsland` (the parent's in-world bit copied through lo.h's bit-field compiles to a mask,
14/75 off), so.cpp GetSoTreeFlag2 / GetSoIgnoreFixed `SOTREEIsland`/`SOIsland` (sign-extending shift pairs on the
whole word).

Evidence for header signatures that need classes.h in step (not changed here: the overriders there would stop
overriding): LO slot 20 RenderAll is called with (int, float, float, SOBOUNDS *) (so.cpp RenderSoAll, lo.cpp
RenderLoAtMatrix); slot 30 RotateToMat receives a 3x3 MATRIX3 (bomb, spawner, lo.cpp); SO slot 73 IsPosInside takes
a quadword position and returns int (rip UpdateRip, class_registry's thunk); BASIC slot 3's fifth argument is a CRef *
reply (po.cpp CanPoBeHealed); update.cpp calls LO slot 17 as the paused-object update. SO +0x58/+0x5C are the
position/rotation XFs (fieldrename refused `positionXf`: the token is used in so.cpp's member bodies).

Not attempted (representation-specific or other families' classes): the qword/union matrix views of the hand-tuned
so.cpp transform functions (TranslateSoToPos, RotateSoToMat, SetSoAngularVelocity, SetSoXf), sensor.cpp's whole-word
SO/SOPART/SOTREE views, gadget's qword XF view, glob.cpp's self-contained Lilly namespace, hud's register-passed RGBA
words, the player/guard/camera classes' padded views (their families' headers), listener objects whose slot 51 is not
declared (prize's LISTENER views).

### Views unified (names/vu-f1-base-2): base family, round 2

The second base-family pass (same rules and checks; in addition every touched object's relocations, compared by symbol
name, and its nm symbol set are identical to the branch start, so a wrong overload or virtual slot cannot pass as a
MATCH because the call targets are masked). All 211 src/game sources MATCH with identical nm and relocations.

| change | where | how |
|---|---|---|
| hand vtable calls on LO/SO/PO objects | sw (PostSwLoad, ShouldKeepSoPair, ExecuteSwTransition, TriggerOneRsmg), game (countdown widget, task marker), so.cpp (SetSoParent, DeliverZapToSo), proxy (AddProxyCopy, SetSoIdProxy, InitSwProxySpares, SO_ResetForProxy), xf (WakeXfOwner, SetXfRole), render, sky, emitter, prize, hack, lootpouch | real virtual calls; the HACK view declares its own slots 82 (`Die`) and 83, the pouch item view slot 76 |
| SO/SOPART/SOTREE/XF field views in so.cpp | zap delivery and responses, IsPosInSo, CollectSoParts, SetSoZapMode, the region-record, gravity and SAA-list helpers, AdjustSoXps, ExtendSoTreeBounds, SetSoGravity, GetSoVelocity, the XF handle accessors | the shared types; so.cpp's XFH and STEPREC views became so.h CID148/CID149 and REGENT |
| views padded over the base | robottank (robot tank, RC pickup), path (path zones : XFM, path links : LO), filerec, prize (sprayed prize, clue proxy : PROXY), sky, lootpouch | derived views of the shared class |
| LO/SO views in the header-less player files | jtgoal (contacts through SOTREE/XPLIST, the faded object an SO), places (place objects LO, spots SOPART, the ground an SO) | `#include "sly2/so.h"`; the player-type views are left to the player family |

Header changes: lo.h/so.h slot 39 `SetTransformFromXf(XF *pxf)` (SetSoXf(so, pxf) and SetLoTransformFromXf take the
XF; lootpouch passes the launch frame); so.h GLOBSET +0x4C `relightStamp` (render.cpp SubmitSwRelightAll compares it
with the world's relight stamp, glob.cpp BindGlobset sets it), REGENT +0x4/+0x8/+0xC floats (EnsureSoPhysRegionRecord
defaults 75/150 and 1), CID148 +0x10 `cref`, CID149 +0x20 `pso` / +0x24 `pxf`.

Matching-only islands added (scratch islands.csv): render.cpp `RGBAIsland` (DrawWorldLabel's RGBA with constructors),
hack OnHackshipActive `LOIsland` and so.cpp GetSoZapMode / GetSoNoJtPlace `SOIsland` (whole-word bit tests; the
bit-fields compile to other shift sequences).

Measured (scratch metrics.py over src/game): struct/class/union definitions 6,759 -> 6,627; family-named views 322 ->
312; hand-vptr views 406 -> 379; non-derived views padded over a base 1,257 -> 1,189; 19 files, +425/-634.

Left for later, with the reason: classes.h signatures (LO slot 20 RenderAll args: so.cpp RenderSoAll, lo.cpp
RenderLoAtMatrix; slot 30 RotateToMat MATRIX3: launcher; SO slot 73 IsPosInside: rip UpdateRip; WIDGET slot 49 Show
takes an int: game.cpp ShowCountdownHud, SetJobCompletionPhase; EXPL slot 51 takes the EXPLSO: emitter); the
hand-tuned asm functions that read the vtable pointer themselves (step MatchStepXf, so.cpp SetSoXf, booty case 5);
the qword-representation views (shape.cpp paths, solve.cpp, launcher, bbmark's AAOBR = SOTREE); glob.cpp's Lilly
namespace; the task/job views of game.cpp (sly2/task.h); the header-less player files' remaining LO/SO views (cane
NODE = LO with Add/Remove/MatchTransform, its ALO target list = TARGET, its POCTL view = po.h POCTL; hang NODE;
pipe_jt PATH and INPUT = POCTL), left to the player family's pass on those files to keep the merge simple.

### Views unified (names/vu-f1-base-3): base family, round 3

Same rules and checks as round 2 (every touched object MATCHes with identical nm symbols and relocations compared by
symbol name; at the end all 210 src/game sources). This round went after the hand-made vtable calls first.

| change | where | how |
|---|---|---|
| reference-counted holders | sm.cpp (four `Lock` copies), relay.cpp (`LOCK` in UpdateRelayScript/RemoveJsg) | one file-scope `SMAHOLD` (AddRef in the constructor, Release in the destructor) on aseg.h SMA; relay uses its existing `JSGREF` on JSGV |
| SMA/SM/SMRT views in sm.cpp | SetSmaSvt, SeekSma, SnapSma, HandleSmaMessage (now `(SMA *, MSGID, void *)`), SkipSma, RetractSma, NotifySmaSpliceOnEnterState, Replace/ReleaseSma, GetSmStateOid, FindSmDistanceTable, CountSmTransitionHops | aseg.h SMA/SM/ASEGA and sm.cpp's SMS/SMRT; real AddRef/Release/SendMessage/HandleSpliceEvent |
| behaviours pushed with PushStepBehavior | stepbhv (follow, walk-to), bhvsearch, dimitri, neela, carmelita, stepplayer (StartStepplayerFollow) | the views derive from step.h BHV (`struct O : BHV { ... }`, `owner` for the old `guard` word); `child->Start()` replaces the slot-8 calls and bhvsearch's `StartPushedBhv` template |
| LO/SO/PO objects called by hand | stepplayer, murray (carried object, hit effect), bentley (dart, explosion objects), jt (contact objects, sub-objects), air, hurt, relay (JSG tunnel = TN, step holds = STEP slots 83/84), cm (policy reset), cplcy, sound (CID151/STREAMSND/BASIC), hack (hack enemies : PO), robottank (robot tank : PO, part's physics root IsFixed), hud (widgets : LO), bif (find builtins on LO/SW) | real virtual calls on the shared classes; `isInWorld` for the bit-60 tests |
| emitter.cpp SetEmitterParticleCount | its EMITTERV | emitter.h EMITTER (k, c, fDirty) |

Header change: basic.h declares `~BASIC()` (never defined; _$_5BASIC 0x258348 is its body, emitted by screen.cpp over
its own view). Its symbol is `_$_5BASIC`: g++ 2.95 rejects an asm-labelled destructor here (PO()'s base cleanup in
classes.h then finds no `BASIC::__dt`), so an object that destroys a BASIC-derived object binds it with
`// SLY2_ALIAS: _$_5BASIC=DestroyBasic` (since names/basic-dtor the config name is `_$_5BASIC` itself). A scratch TU defining `SW g;` references only `SW_ctor` and `_$_5BASIC`. All
210 sources compile to identical objects with the declaration.

Measured (scratch metrics.py over src/game): struct/class/union definitions 6,627 -> 6,510; family-named views
312 -> 309; hand-vptr views 379 -> 330; non-derived views padded over a base 1,189 -> 1,172; 22 files, +365/-577 before this section.

Left, with the reason: the CID36 carry spot's +0x180 carried object (murray: classes.h pads it); CPLCY Set/Revoke are
called with the stack entry's argument (cm.cpp SetCmPolicy/RevokeCmPolicy) but cm.h declares them without one; SMT is
incomplete (sm.cpp SetSmaGoal reads its +0xC flags), ALO is only forward-declared (SendSmaMessage calls slot 5 on the
SMA's root); hand vtable stores (ApplySmCore, NewRelayJsg) need the views; murray SpawnMurrayHitEffectUnchecked passes
an int where TranslateToPos takes a quadword (a real call changes the code); the classes.h signatures listed in
round 2 (RenderAll, RotateToMat, IsPosInside, WIDGET Show) block rip UpdateRip, stepplayer PostLoadStepplayer and the
widget Show calls in game.cpp/robottank/screen; the qword-representation SHAPE/XF views (shape.cpp, tail.cpp) and the
hand-tuned player functions (murray, bentley, jt, hurt, grab) whose P views are STEPPLAYER layouts.

### Views unified (names/vu-f1-base-4): base family, round 4

Same rules and checks as rounds 2 and 3 (every touched object MATCHes with identical relocations compared by symbol
name and an identical nm symbol set, now including symbol sizes from `nm -S`). No header changed: this round rewrote
private views in 37 sources. Its target was the views padded over a base class.

| change | where | how |
|---|---|---|
| object views padded over a base (`struct S { char pad[0x54]; int state; ... }`) | game (JOB/TASK), guardbhv/stepguard/stepbhv (guards, attacks, detectors, behaviours), hud/screen/memcard (widgets), murray/jt/bentley/stepplayer/grab/hurt/air (heroes), step, so, path, shape, relay, prize, grabspot, robottank, sound (stream sounds), and 15 smaller files | `struct S : B { char pad[0x54 - 0x50]; int state; ... }`: B is the largest base of the object's class whose size fits in the pad (BASIC 0x10, BHV 0x30, LO 0x50, XFM 0xE0, SO 0x150, PO 0x180, SHAPE 0x190, STEP 0x2C0, STEPPLAYER 0x1320; classes.h CID35 for the carry grab spot, snd.h STREAMSND for stream sounds). The leftover pad keeps both offsets (`0x54 - 0x50`), so the view's other offsets read as before |
| how a view's class was decided | per view | from what the function does with the object: guard functions (`Stepguard*`, a behaviour's `guard` field), InitLo/PostLoLoad/UpdateLo calls, IsBasicDerivedFrom, Init/Clone calls (InitShape, CloneXfm, CloneSo), and offsets that match a header (SHAPE +0xE4 isClosed, DETECTOR +0xE8/+0xFC, RELAYV's alarm list at +0x84). Views whose class is not shown (internal records such as XF, SOPHYS and SOTREE views, the SNDMGR global, rich-text and sprite records) stay as they are |
| views that are a header type | guardbhv (SetBhvpathShape, IsBhvpatrolpathAtEnd: xfm.h SHAPE; SetBhvanimState: aseg.h ASEG), relay (ResetRelay, SetRelayCommandLimit: the file's RELAYV; ClearJsgSma, ClearJsgStepHolds: its JSGV) | the view is removed and the type used with its field names |
| pushed behaviours called by hand | guardbhv (21 views and the `StartPushedBhv` calls in their functions), stepguard (ResetStepguardToHome) | `struct O : BHV { ... }` with `o->Start()` / `o->Close()`; MoveBhvRelocateToPoint keeps the template: its one view is both the guard and the pushed leap |
| LO objects called by hand | stepplayer (LaunchStepplayer, WillStepplayerLand), hurt (PresetJtAccelZap, func_001A28B0), grab (PresetJtAccelHide), stepguard (PostStepguardLoad's indicators: Remove; func_00209EE0: Get/SetVelocity) | the view derives from LO and makes the real virtual call |

Layout asserts written for a view (the m2c generator's `sizeof`/`__alignof__` lines) follow the new type where it grew
to the base's alignment: murray func_001B7700 (`X` 0x184 -> 0x190 and 16-aligned, `P` 16-aligned), relay's PATH
(0xE8 -> 0xF0), grab PresetJtAccelHide's `O` (0xEC -> 0xF0). Includes added: step.h in sound.cpp, lo.h in screen.cpp,
memcard.cpp and difficulty.cpp.

Measured (scratch metrics.py over src/game): struct/class/union definitions 6,510 -> 6,480; family-named views
309 -> 308; hand-vptr views 330 -> 301; non-derived views padded over a base 1,172 -> 635; 37 files, +735/-786.

Left, with the reason: a view in a function whose namespace defines its own struct with the base's name (prize
EmitDprizeGlint's local `LO`: inside a derived struct the name means the base); stepbhv func_00146B18's `PO` (as an
LO it picks the C++ `GetLoWorldPos(LO *)` overload, which returns a different qword type, so the call target would
change); the SNDMGR views of sound.cpp (destroyed through ~BASIC, but nothing else shows that it is a BASIC);
views of SO fields inside the SO range (`struct B { char pad[0xE8]; SOPHYS-like *pc; }`), which want each field mapped to
so.h's name rather than a base; hand-vptr views whose vt field has layout asserts (murray func_001B6CC0, jt
func_0018DF60, robottank robot tank, hack) or whose calls are inside asm (murray SetMurrayFlopSubstate); the
screen/memcard/game widget Show/Hide calls (WIDGET slot 49 takes an int, classes.h) and the screen objects' own slots
10/11 (screen family).

### Views unified (names/vu-f1-base-5s): base family, round 5

Re-applied on top of the other families' rounds, including the HUD round and the cross-family signature pass (the earlier attempts, names/vu-f1-base-5 and -5r, conflicted with them).
Same checks as round 4 (MATCH, identical relocations by symbol name, identical `nm -S`), run on all 210 game sources
because basic.h and so.h changed.

| change | where | how |
|---|---|---|
| private vtable-entry records (`struct VTE { short d; short pad; void (*pfn)(...); };`, also named VE/VT/PRGVT, with `delta`/`fn`/`pf` spellings, the `{entry} a[N]` arrays, the one- and two-slot records padded to slot k, and the per-slot record unions of grab.cpp and gadget) | about 70 sources | one shared `VTE` in basic.h (`void *pfn`). A call that went through a typed `pfn` now casts it to exactly that type, `((void (*)(void *, int))p->vt[49].pfn)(...)`; a padded record becomes the slot index (pad / 8). Their offset asserts were dropped (basic.h asserts the size) |
| GLOBSET +0x2C/+0x30/+0x34 | so.h, glob.cpp, so.cpp | `numShaderOids`, `shaderOids`, `selectedShader` (glob.cpp loads the oid list and builds shader unpacks from it; SetSoAnimIndex stores -(index + 3)); SetSoAnimIndex/ClearSoAnimIndex take SO |
| XF +0xA8 and +0xC0..+0xFF | so.h | +0xA8 is a union by role: `reference`, `arrivalTime` (roles 9/0x11), `plink` (CHAINLINK), `pikrec` (IKREC), `pconstr` (XFMCONSTR); +0xC0 a union of the old `restValue/rate/unknownE0/acceleration`, the phase-1 control values `segmentStart/segmentStartTangent/segmentEnd/segmentEndTangent` and `rotationRows[4]`, with offset asserts. SO::Clone's chain case uses `plink` |
| base views replaced by the header type | so.cpp (SetSoAnimIndex, MatchSoXf), proxy.cpp (SO_Slot27), glob.cpp (BindGlobset: SO and XF) | real fields (children, ploRemovedFirst, model, psobounds, physics->pregent, siblingLink, worldMatrix) and real virtual calls (Get/SetVelocity, Get/SetAngularVelocity, OnProxyRelease, GetXfFromIndex); game.cpp ApplyStateLoVisibility and so.cpp RenderSoAll were already on the header types |

Measured (scratch metrics.py over src/game): definitions 3,382 -> 3,241; family-named views 249 -> 246; padded views
284 -> 281; 78 files, +453/-728.

Left, with the reason: blend.cpp's VTE (the file does not include basic.h); memcard's PMF records (pointers to member,
not vtable entries); throw.cpp's VE (one entry record, a union of several function types); the private records of
prize.cpp and grabspot.cpp (left for a follow-up because another agent is editing those files).

### Views unified (names/vu-physics): the physics family, so.cpp and shape.cpp

Same checks as base round 5 (every touched object MATCHes, identical relocations by symbol name, identical `nm -S`),
run on all 210 game sources because so.h and xfm.h changed.

| change | where | how |
|---|---|---|
| quadword rows as union members | so.h XF (`localMatq`, `worldMatq`, `velq`), SOPHYS (`inertiaq`); xfm.h SHAPE (`vertexPosq`) | the VU0 code reads whole rows through the Q member instead of a cast (island-review rule). xf.cpp's world XF initializer gained one brace level per matrix |
| new and named fields | so.h `SURFX` (a soft part's surface triangle), LVO documented as per-vertex velocity/acceleration, SO +0x148 `dhpHit`, +0x149 `dhpTouch`, bit 35 `isRenderable`; xfm.h `SHAPECHAIN` (0x90) with `SHAPE::chain` typed | from InitSo, SetSoHitDamage, HandleSoTouchZaps, SetSoBit35Hierarchy, EnsureSoPartMesh/UpdateSoPartMesh, InitChain and AddChainSample. Guesses are marked (inferred) |
| so.cpp views replaced | UpdateSoTreeBounds, LoadSoFromBrx, FreeSoContacts, EnsureSoPartMesh, UpdateSoPartMesh, CloneSoPartGeomInserting, HandleSoTouchZaps, TranslateSoToPos, RotateSoToMat, SetSoAngularVelocity, ScaleSoInertia, the three AccelSoToward* functions (spring coefficients are basic.h CLQ), SetSoEyesClosed, GetSoCameraDefaults (cm.h CPDEFI), SetSoGrfBit8, InitSo, SetSoBit35Hierarchy, the XF-handle actuator functions (act.h ACT/ACT4), FindZapResponder (LO::parent), ExpireSwZapRecords (sw.h SW) | real fields and real virtual calls; a namespace's duplicate extern "C" declarations of functions declared at file scope were removed (they make the call ambiguous once the types agree) |
| shape.cpp views replaced | UpdateShapeXfWorld, SetShapeChainMode, GetShapeChainLength, AddChainSample, the file-scope SHAPECHAIN | SHAPE and SHAPECHAIN |

Kept, with the reason (scratch islands.csv): LoadSoFromBrx's slot-51 call stays a hand call (as a virtual call in the
part loop the vptr stays in a register); SetSoXf's `SOIsland` (its slot-35 entry is loaded by hand-scheduled asm);
NewSoXfHandle's `CID149Island` and NewSoPartHandle's `CID150Island` (hand construction: the vptr store and raw clears of
BASIC's two CRefPtr words). Not attempted: EnsureSoBounds and the shape evaluators (register-pinned VU0 transcriptions
with hand vtable calls); the XP contact views (XP has no header yet and several other families define it at file
scope); path.cpp's PATHZONE/PATHLINK views (those classes are opaque in classes.h, another family's header).

### Views unified (names/vu-f5-camera): the camera family

The private views of CM, the camera policies (CPLCY, CPLOOK, CPZOOM, CPHANG, CPCAMBLEND, CPTURRET, CPTWOSHOT,
CPCAMERA), CPR, CPDEFI and the camera move records, replaced by sly2/cm.h's types. Every function and data symbol of
every touched file and of every includer of cm.h MATCHes, and each object's nm symbol set is unchanged.

| change | header | views removed | files |
|---|---|---:|---:|
| CAMEVENT, CAMPOSE, CAMMOVE moved into cm.h (five identical copies) | cm.h | 15 | 5 |
| cplcy.cpp: every policy function on the policy classes (ActivateCplcy, the CPLOOK/CPZOOM/CPHANG/CPCAMBLEND/CPTURRET/CPTWOSHOT functions) | — | 38 (CM/G views, the policy views X/A/S/T, CAMMOVE/CAMPOSE views K/PATH/B, two basic.h LM copies; the CAMEVENT vtable-call helper O kept) | 1 |
| cm.cpp: every CM function on CM/CPR/CPDEFI; SetCmPolicy, RevokeCmPolicy, UpdateCmLast, ResetCameraPolicies call the CPLCY virtuals for real (they matched on the first compile) | cm.h: CPLCY::Set/Revoke take the entry argument | 39 (CM/P/CMC/CMX/CMR/CMJ, CPR views E, CPLCY views O/VTE, CPDEFI views S/CPDEFIV, InitCm's V3/LM) | 1 |
| one-off g_cm views in other files (CMV, CMVIEW, CAMERA, Camera, CAM, CM, G/GS, CPDEFIV, CPCAMERAV) | — | 29 | 22 |

cm.h field names taken from the views: `fovSmoothing` (+0x2C, SMPA, was unknown2C..38), `joltKind` (+0x61C),
`stereoEyeOffset`/`stereoFocusDistance` (+0x620/+0x624), `zoomStopSound` (CPLOOK +0x38 / CPZOOM +0xC4: sfx 0x25, made on
activation). Struct definitions in src/game .cpp: 121 deleted, 12 CMIsland added (two of them are the renamed views of sky
and spawner).

CMIsland (matching-only, Representation decisions 1.4): the original reads g_cm's position and matrix rows as 128-bit
members at base `&g_cm` plus the field offset (`lq 0x90(base)`). CM declares them as VECTOR4/MATRIX4 floats; a cast of
those fields (`*(qword *)&g_cm.pos`) makes g++ fold the offset into the address constant (`lq 0(base+0x90)`, 1-2
instructions off), and through a pointer the cast also changes scheduling (blip: MEM_IN_STRUCT). Where that happened
the read goes through a small file-local `CMIsland` (a struct with qword members at the CM offsets) and everything
else uses CM: sound, light, prize, lo, busy, waypoint, emitter, blip, cplcy, recon (one read each or two); sky
(base `&g_cm.mat`, offset 0x30) and spawner (the old view, renamed: its asm reads the rows from base `&g_cm`).
Revisited in names/vu-f5-camera-2 (below): CM::matq, a qword-row view in the matrix union, removed all twelve.

Not converted: render.cpp and places.cpp (their private qword/RGBA/LM/VECTOR types clash with basic.h, so they
cannot include cm.h), camera's CID44 view (classes.h names no CID44 fields; not this family's header), rip, hud,
relay (G views at +0x500/+0x510 and the matrix), glob/rendertree (VIEW: the frustum planes, g_cm read as a culling
view), bif's CM, rtcam's RTCAM, and the `char D_002DDF40[]` + offset spellings (exit_warp, turret, relay,
asega, guardbhv, breakable, carmelita, bentley). All but camera's CID44 view and rtcam's RTCAM (an XFM-derived render-target
camera object, not CM) were converted in names/vu-f5-camera-2.

### Views unified (names/vu-f5-camera-2): the camera family, round 2

Every function and data symbol of every touched file and of every includer of cm.h/classes.h (and every user of
`D_002DDF40`, 123 files) MATCHes, and each object's nm symbol set is unchanged against the branch start.

| change | header | views removed | files |
|---|---|---:|---:|
| CM::matq: the matrix as quadword rows (`CMMATQ { qword rot[3]; qword pos; }`) in CM's matrix union; the CMIsland views go (docs/decisions.md 1.8) | cm.h | 12 CMIsland | 12 |
| rip, relay, hud, bif, render, places, glob, rendertree: CM/CAMPOSE/CAMMOVE/g_cm in place of their camera views | — | 15 (rip G x3, CAMERA; relay T, SUB, G, G2; glob VIEW, ENV; rendertree VIEW; bif CM; render CM; places D; hud's opaque G) | 8 |
| `char D_002DDF40[]` + offset and opaque handles (MANAGER/CAMERA in grab) replaced by `&g_cm` and its policy members (cplook, cpzoom, cpcamblend, hangPolicy, turretPolicy, part3a0) | — | 19 declarations | 16 |
| recon's `D_002DDFA0 __asm__("D_002DDF40+0x60")` separate-symbol alias | — | 1 | 1 |

How the original's address forms come out with CM's members (all g++ 2.95, checked per function):

| original access | spelling |
|---|---|
| `lq 0x90(base)`, base `&g_cm` (position) / `0x60+0x10*i` (rows) | `g_cm.matq.pos`, `g_cm.matq.rot[i]` |
| `lq 0x30(base)`, base `&g_cm.mat` (sky, relay; recon's row 1 at `0x10`) | `CMMATQ *pmatq = &g_cm.matq; pmatq->pos` (a local pointer stays the base) |
| `lq 0(base)`, base `&g_cm + 0x90` (folded: emitter, tn, stepguard...) | `*(qword *)&g_cm.pos` (unchanged) |
| the frustum's transposed plane normals as three qwords (CullSceneRenderList, glob's StartCull) | `(qword *)g_cm.agFrustumXyz` (a pointer value only; a union member did not change codegen either, but cost nothing to avoid) |

Things learned: (1) an inline helper in cm.h, even unused, shifts g++'s local label numbers in every includer (cm.cpp's
function-static `s_rgba.87` became `.90`: nm differs, code does not); a template is compiled only where called, but
the local pointer made the helper unnecessary. (2) Template helpers of sly2/vu0.h deduce one quadword type: where a
file's signed `qword` meets cm.h's (basic.h's unsigned `qword`), the call names it (`VuSub<qword>(a, g_cm.matq.pos)`).

render.cpp, places.cpp, rendertree.cpp, bif.cpp, asega.cpp, brx_sw.cpp, update.cpp, sw.cpp now include cm.h. The clashes
with basic.h were resolved in the files (Representation decisions 1.6): their own `typedef int qword` dropped for
basic.h's (no codegen change in any of them), render's private RGBA/LO/SW/LM views renamed RGBAV/LOV/SWV/LMV, places'
LO/JT/CLQ/LM renamed LOV/JTV/CLQV/LMV and vec.h's 16-byte aligned VECTOR kept as a local `VECTORV` (basic.h's VECTOR
has no alignment). Struct definitions in src/game .cpp: 26 camera views and 19 handle declarations removed (render's
and places' renamed views are not counted); no island remains in the family.

Left: camera's CID44 camera-object view (classes.h; not this family), rtcam's RTCAM (render-target camera
object, an XFM; no header), main.cpp's `extern CM D_002DDF40` (a typed handle; main does not include cm.h), and the
inventory's false positives in cm.cpp/cplcy.cpp (CMG, the g_cm definition wrapper; SW/ACT/STEPPLAYER views).

### Views unified (names/vu-f6-render): the render family

The private views of the emitter block and its records, the rips, the render list, the model sub-mesh records, the
colour tables / bitmaps / TEX0 register and the GS block allocator, replaced by shared header types. Every function and
data symbol of every touched file and of every includer of the changed headers (the 75 classes.h includers for
emitter.h) MATCHes, and each object's nm symbol set is unchanged.

| change | header | views removed | files |
|---|---|---:|---:|
| SUBGLOBX (12 identical copies), SUBGLOBLOAD (2) | new sly2/glob.h | 14 | 11 |
| RPL, the render-list entry (`pbCall`/`pvCall`: the two DMA CALL targets) | new sly2/render.h (self-contained) | 6 | 4 |
| EMITTER's fields (mode, count, rate, pause, group list/link, current rate, density, box radius...); EMITB as Sly 1 lays it out: EMITO origin record (+0x10, a union for the point position / data word), EMITVEL velocity record (+0x50; Sly 1's EMITV, renamed because EMITV is in the accessors' mangled names), EMITV particle record (+0xC0) as a union of the rip and blip records selected by particleKind, with EMITROLL/EMITREMIT payloads; EMITREF indices for aploRef; EMITMESH/EMITTRI, EMITVX, EXPLSO and its flags | sly2/emitter.h | emitter.cpp 60 definitions (7 EMITTER views, the EMITB/record views EMITBV/EP/E/S/B/X/P/EMITREFS/EMITO/SAV/EO, EMITMESH/EMITVX copies, EMITDEF::EMITO and their helper types); blip 1, grab 3, po 2 | 4 |
| RIP and its kind classes moved out of rip.cpp; RIP gains startTime, renderObject, the scale curve, orientation, velocity, angularVelocity, touchObject; FLYING/MATCH/REMIT/SHRAPNEL fields; `RIP::Init(pos, scale, psoTouch)` in Sly 1's parameter order (InitMatch's symbol), `PostEmit(EMITB *)` | new sly2/rip.h | 14 (rip.cpp's per-function RIP/RIPGV/SHRAPNEL/OBJ views, emitter.cpp's RIPV/RIP) | rip, emitter |
| TEX0 (one word or GS fields), CLUT, BMP | new sly2/shd.h (self-contained) | 9 | shd, font, gs, rtcam |
| GSB/GSBB (0x630) | new sly2/gs.h (self-contained) | 4 | gs, font, rtcam |

Hand-made vtable calls that became real virtual calls and still match: EMITTER group creation (RotateToMat, slot 30),
ConvertEmitoPosVec and OriginateSplineSinkParticles (GetLocalTransform, slot 32), EmitRips and WakeSoWater (RIP::Init,
PostEmit). EMITTER::HandleMessage and REMIT::PostEmit became members. EmitRips matched only once RIP::Init took its
arguments as (position, scale, touch object): with (position, touch, scale) the scale's call moves and the allocation
shifts. Struct definitions in src/game .cpp: 7290 -> 7162; render-family names (inv.py): 165 -> 82.

Islands (matching-only, recorded in scratch islands.csv): emitter.cpp's two default/splash initializers keep their
union-shaped views (`EMITDEF::EMITVELIsland`, WakeSoWater's `EMITVIsland`/`EMITVELIsland`): the unions there are
scheduling barriers the header records do not have, and the header EMITV (anonymous union of the two records) as an
aggregate initializer makes cc1plus exit silently. RIP::Init reads the clock through a bare-float view of g_clock.time
(10/23 off through CLOCK).

Not converted: views of RIPG, BLIPG, EXPLG and EXPLS fields (classes.h names none of their fields; not this family's
header: rip.cpp's RIPGV, emitter.cpp's BLIPG/RIPG/PRG/EXPLGV/EXPLS views, the EMITG views typed on them); RENDERTGT
views (sly2/sw.h keeps TEX0 as a plain word; shd.h's TEX0 union could replace it); the namespaced Lilly views in
glob.cpp, shd.cpp, light.cpp, blip.cpp (SHD, DMA/DMAS/VIFS/GIFS packet classes, PACKET, LIGHT, SAA); palette16 /
palette256's constructor-initialized CLUT/BMP (a constructor in shd.h's CLUT would change every includer); the hand
vtable calls of rip.cpp's hand-tuned functions (BounceRipgRips, UpdateRipg, RenderRipgSelf, HandleRipBounce, the
Render* bodies).

### Views unified, round 2 (names/vu-f6-render-2): shaders, packet streams, light packets, rip calls

The namespaced Lilly / per-function views of the render family's records in shd, glob, blip, light, palette16 and
palette256 (and so.cpp's views of the light record), and the hand-made vtable calls of rip.cpp, replaced by shared types
and real calls. Every function and data symbol of each touched file and of every includer of a changed header MATCHes,
and each object's nm symbol set is unchanged.

| change | header | views removed | files |
|---|---|---:|---|
| CLUT/BMP constructors from a header (`CLUT(void *pvHeader)` calls CopyClutHeader; likewise BMP), CLUT/BMP `clevel` (+0x05, the level filter) and BMP `cb` (+0x10) | sly2/shd.h | 4 | palette16, palette256 |
| TEX (0x14: grftex +0x04, cbmp +0x06 = the frame count, cclut +0x07; glob's Lilly TEXF had cibmp/ciclut at +4/+5, the loaders show otherwise), SHDP (upload record), SHD (0x2C), SHDBLIP (the compact record blips use, linked on SW::slShd; our name) | sly2/shd.h | shd 30 (SHDV, the loaders' X/Y/E/Z/E20/E28, LoadShdFromBrx's L/C/ENT, BuildShaderTex0List's Src/S3/Dst, BuildShaderGifPackets' ENTRY/PACKET/RGBA/TEXTURE, the upload functions' E1/E2/T1/T2/BS/X/DMAS/M, the hash functions' S); blip 1 | shd, blip |
| the frame's VIF1 stream D_002DEA28 as vifs.h's VIFS | (vifs.h) | glob 4 DMA, blip VIFS | glob, blip |
| render-list entry | sly2/render.h RPL | blip DMA | blip |
| SOBOUNDS, the per-object light packets (SO::psobounds, only declared by so.h) | new sly2/light.h | light LIGHTQ, glob 2 SHD (the "SHD" of RenderSoGlobset / BuildModelDrawPackets is this record), so.cpp SOBOUNDS/Q/B | light, glob, so |

Real calls that match: blip's VIFS/DMAS members (all but one AddVifUnpack that passes a fifth word: 4 instructions move
as a member call), GIFS::PackAD in PackTexGifs, VIFS::AddVifUnpack in AddShaderVifUnpack, UploadPermShaders' GIFS/VIFS
locals with member calls and a STACKSCOPE local (the views were 0x30 bytes; the real 0x28/0x24-byte classes give the
same frame; `result = 0` must sit between the two constructors), BuildUploadBitmapGifs a C++ function again
(`BuildUploadBitmapGifs__FiP3GSBP4GIFS` from the real GSB/GIFS types). rip.cpp: RIP::Update and SO::Remove in UpdateRipg,
RIP::Render in RenderRipgSelf, RIP::HandleBounce in BounceRipgRips, RIP::Project in ProjectRipgTransform, RIP::Init in
UpdateRipt1 (the hand call's (float, qword, pointer) order is the same register assignment as Init's (qword, float,
pointer)). For these rip.h's `Update()` and `Render()` lost their parameters: the dispatchers pass none, UpdateRip and
RenderRip take only the rip, and no override reads a time step or camera (Sly 1's `RenderShrapnel__FP8SHRAPNELP2CM`
symbol still names a CM). SHDBLIP's colour is a union with a channel array: PropagateBlipgShader indexes it, and only an
array member keeps the colour's address in a register (9/85 off through a cast).

Inventory (inv.py render-family names in src/game .cpp): 82 -> 60; struct/class/union definitions in src/game .cpp/.inc
(`grep -E '^\s*(typedef\s+)?(struct|class|union)\s+\w*\s*(:[^{;]*)?\{'`): 6938 -> 6873.

Not converted, and why: LIGHT (light.cpp's LIGHTV) and SAA (shdanim's SAAV/SAAF/SAACLS, glob's SAAVIRT/SAAFIELDS) need
fields in sly2/classes.h (LIGHT +0xE0..0x170 and SAA +0x10..0x50 are padding there; not this family's header);
glob's M_LoadSubglobType0FromBrx (a 1450-line Lilly port with its own QW/VECTOR/RGBA/OID/SAA/LO/SO universe) keeps its
DMAS/VIFS/TEX/SHD/CLUT copies until that universe gives way to the shared headers; glob's relight PACKET/PACKETS and
light.cpp's PACKET are different records (no shared evidence); shd.cpp's CopyClutHeader/CopyBitmapHeader (file
headers, not CLUT/BMP objects), BuildWorldBackgroundImage's GIFS view, AddShaderVifUnpackQword, SHDREC; rip.cpp's
remaining hand calls (BounceRipgRips' slot-9 Remove on the P view, UpdateRip's slots 7/73, ReleaseRip's slots 3/5,
CreateStuck's Init on its O view, MATCH's slot 8, REMIT's slot 51) sit on views that would need RIPG/SO/STUCK fields.

### Views unified, round 3 (names/vu-f6-render-3): the type-0 sub-glob loader, shader headers, rip calls

LoadSubglobType0FromBrx (glob.cpp's 1450-line Lilly port) on the shared headers instead of its own universe, the
remaining shd.cpp views, and more of rip.cpp's hand-made vtable calls as real calls. Every function and data symbol of
each touched file and of every includer of a changed header MATCHes; each object's nm symbol set, its relocations by
symbol name and its symbol table (readelf) are unchanged from the branch start (matchtool masks call targets: the
relocation diff is what caught a namespace-mangled `IgsAllocGsb` declaration once).

| change | header | views removed | files |
|---|---|---:|---|
| LoadSubglobType0FromBrx on basic.h's typedefs/RGBA/VECTOR/VECTOR4/MATRIX4/SL/OID/CID, qw.h's QW, dmas.h/vifs.h's DMAS/VIFS/UPK, shd.h's SHD/TEX, so.h's GLOBSET/GLOB/SUBGLOB/POSE/POSESET/XF and the file's BND and CBinaryInputStream (gains ReadMatrix4) | (shared) | 24 | glob |
| CLUTSET/CLUTPAL/CLUTCOLOR, a type-0 sub-glob's per-vertex palettes (LoadClutsetFromBrx's T/E8/E6 were the same records) | sly2/glob.h | 6 | glob |
| BATCH, VERTREC (CompareSortRecs' SORTREC: its int "batch" fields are the BATCH pointers, the `>> 6` their 0x40 stride), WRBG/WEKI/WRBGBATCH completed at file scope (now sly2/wr.h's); LoadSubglobInstancesFromBrx on SUBGLOB/GLOB/SUBGLOBX/BATCH | (glob.cpp file scope) | 4 | glob |
| CLUTHDR/BMPHDR (the stored 0x10/0x18-byte headers; CLUT and BMP derive from them): CopyClutHeader/CopyBitmapHeader, the loaders read `sizeof` the header | sly2/shd.h | 4 | shd |
| COPYREC (0x14), the shader-animation copy record of D_003DA798: ApplyCopyRecord, RelocateShaderUploadRecords | sly2/shd.h | 3 (SHDREC, the 12-byte COPYREC, E) | shd |
| AddShaderVifUnpackQword on SHD/SHDP with a real VIFS::AddVifUnpack; BuildWorldBackgroundImage on a real GIFS local (eleven member calls), sw.h's SW and gs.h's GSB | (shared) | 6 | shd |
| STUCK::ploHost (+0xF0), RIP::clqScale (the scale curve as basic.h's CLQ, beside the qword and the three floats): RenderStuck, CreateStuck (a real RIP::Init); ReleaseRip a C++ function with real RIP::OnRemove and BASIC::HandleMessage; MATCH::Update's real LO::Add | sly2/rip.h | 14 | rip |

Notes. LoadSubglobType0FromBrx keeps its store-then-advance word append as a local helper (`AddDmaIntStore`):
vifs.h's AddDmaInt advances first, which reorders 66% of the function; named apart because ADL finds vifs.h's for a
VIFS argument (the same ADL made the namespace's memset copy ambiguous: removed). The shader colour is read as
SHDRGBA (shd.h stays self-contained). CreateStuck reads the group's emit scale through the file's RIPGV (RIPG is
padding in classes.h) and adds to the position's z through a float view of the qword; Polynomial must take the CLQ's
address (the target forms `&p->clqScale` once and loads through it).

Inventory (inv.py): 60 -> 48; struct/class/union definitions in src/game .cpp/.inc: 6873 -> 6838; `git diff --stat`:
287 insertions, 657 deletions.

Kept, and why: LoadSubglobType0FromBrx's function-local records (SPECPARAMS, BATCHFILE, BATCHPART0/BATCHGRF/BATCHHDR0,
BATCHREC/BATCHRECS, UVF, RGBA5551, UVLINK; no other user) and its SWALLOCC allocator wrappers (no shared allocator
header); SAAVIRT/SAAFIELDS (SAA's fields are being added to sly2/classes.h by the AI family). WRBG/WEKI/WRBGBATCH, completed
at glob.cpp file scope here, had the same layouts as the anim family's sly2/wr.h; at integration glob.cpp's copies
were removed and it includes wr.h.
rip.cpp: REMIT::Update's slot-51 call needs EXPL::ExplodeExplso(EXPLSO *) and classes.h's EXPLG/EXPLS overrides
changed together (changing only emitter.h's would move the overrides to new slots); UpdateRip's slot 73 (so.h's
IsPosInside has no parameters) and the touch object's +0xE8; BounceRipgRips' slot 9 and NewRipGroup's slots 7/11/15
sit on RIPG views whose fields classes.h's RIPG does not have.

### Views unified, round 4 (names/vu-f6-render-4): LIGHT, SAA, RIPG, the emission records, the packet streams

The LIGHT, SAA and RIPG views on the fields the AI round added to sly2/classes.h, the emitter's group/emission records,
more of rip.cpp's per-function views as members of rip.h's classes, and the remaining views of the packet streams
(DMAS/GIFS/VIFS), the render-list entry and the blip shader record in other families' files. Every function and data
symbol of each touched file and of every includer of a changed header (95 files) MATCHes; each object's nm symbol set,
relocations by symbol name and symbol table (with sizes) are unchanged from the branch start.

| change | header | views removed | files |
|---|---|---:|---|
| LIGHT: highlightHsv gains its float channels, sNear/sFar overlay Sly 1's `LM lmFallOff` (SetLightFalloff copies it whole); light.cpp on LIGHT (parent stamp = XF::worldStamp, list link = LO::classListLink, a directional light's direction = matWorld row 0 read as a qword) | sly2/classes.h | 4 (LIGHTV, LIGHTO, V3, LMK) | light |
| SAA/EYES slot 10 `int DoesMatchOid(int *poid)` (DoesSaaMatchOid's signature; glob.cpp passes &SHD::oid where its view passed the shader); LOOP/PINGPONG/SHUFFLE/EYES/SCROLLER/CIRCLER fields (and EYESS); an SAA's instances are shd.h's COPYREC | sly2/classes.h | 9 (SAAVIRT, SAAFIELDS, six derived kind views, SAIE) | glob, shdanim |
| RIPG: +0x150 groupKind (was ripgState: NewRipGroup's kind), +0x154 boundsPadding, boundsData = min/max/centre | sly2/classes.h | rip's RIPGV and the per-function O/S/P/G/Q views | rip |
| RIP::touchObject is an SO; RIP +0xC4 damping; FLYING +0xF4 facesCamera | sly2/rip.h | rip ~20 per-function views (S/M/B/X/R/OBJ/MATCH...) | rip |
| EMITG (group pointers + subscriber, 0xC) and EMITGEN (per-particle arrays, 0x20: EmitParticles keeps two on its stack), both named by the mangled symbols | sly2/emitter.h | 13 (five EMITX/Y/EMITR, two BUF, TIMES, LOC, L2, emitter's RIPG and SO-class views, grab's EMITX) | emitter, grab |
| the subscribed emitter of a blip group as EMITTER/EMITB | (emitter.h) | 2 (LOV, EX) | blip |
| the frame's packet streams as gifs.h/vifs.h classes with real member calls; render-list entries as RPL; FindShaderById's record as SHDBLIP | (gifs.h, vifs.h, render.h, shd.h) | 21 (hud 8 R, 6 E, BUF, X; framecapture GIFS; rendertree DMA; screenfade R, Q) | hud, framecapture, rendertree, screenfade, screen |

Real calls that match: LIGHT's slot 31 (GetWorldMatrix, twice), SAA slot 10 (twice), RIPG slots 7/11/15 in
NewRipGroup, slot 9 (Remove) in BounceRipgRips and EmitRips, RIP slot 7 (Touch) in UpdateRip, RIP::Init as MATCH::Init's
base call, RIP::PostEmit as FLYING::PostEmit's; 72 hand-mangled DMAS/GIFS/VIFS calls became member calls
(BeginRpHud, BuildSpritePictureTexPacket on a real GIFS local, the frame capture, the fade, DrawUi's Flush,
AddSaaCopyRecord's UnpackHelper). Became members: RIPG::Init, RIP::Update, RIP::Render, FLYING::PostEmit/Render,
MATCH::Init/Render, LEAF::HandleBounce, RIPT0::Init (their symbols are the labels the classes already carried).

Matching-only forms (islands.csv): UpdateRip reads LO::isInWorld through its whole 64-bit bit-field word (the bit-field
test is a mask: 27/52 off); NewRipGroup keeps the GLOBSET constructor declared as returning `this` (with a void result
the vtable address takes $v0: 4/62 off) and stores the two vtable pointers by hand (the in-place construction);
PvtripFromRipt stores each kind's vtable pointer by hand (RIP's +0xEC word); InitBublet keeps the broadcast's stack
home (its frame).

Left by design: LIGHT's slot 30 (RotateToMat), UpdateRip's slot 73 (IsPosInside) and REMIT's slot 51 (EXPL) stay hand
calls for the cross-family signature pass; the BLIPG/EXPLG/EXPLS views (emitter's PRG, BLIPG, EXPLGV and EXPLS view,
blip's BLIPV/BLIPGV: classes.h names none of their fields and they are not this round's classes); STATE (BounceRipgRips'
view of the SOTREE fields it reads); the hand-tuned rip bodies' views (HandleRipBounce, ProjectRipTransform,
RenderRipAtPose, RenderShrapnel, RenderGlint, the SMACK/LEAF/bublet transforms); blip's one by-hand AddVifUnpack (five
words); the RENDERTGT views (sw.h); glob/light PACKET(S) (different records); shd's TEX1/CLAMP (one user); main.cpp's
DMA (a SIF transfer); framecapture's TEX (the capture texture's GS words, not shd.h's TEX); rendertree's RENDER tree
(one TU); the LIGHTM matrix in blip (not a LIGHT).

Inventory (inv.py): 47 -> 35; struct/class/union definitions in src/game .cpp/.inc: 4737 -> 4663; `git diff --stat`:
695 insertions, 854 deletions in 15 source and header files (this section aside).

### Views unified, round 2 (names/vu-f7-anim-2): XF users, clip channels, ALO

Round 2 of the anim family (from e8238a350), driven by the view census. Every function and data symbol of the 19
touched files and of every includer of sly2/aseg.h, act.h and wr.h MATCHes, and each of the 115 objects has the branch
start's symbol table (nm -S, sizes included) and relocations by symbol name. Private struct/class/union definitions in
src/game .cpp files: 4804 -> 4724.

| type | header | change | views removed |
|---|---|---|---:|
| ALO | sly2/aseg.h | defined as an empty `struct ALO : LO` (Sly 2 has no ALO; the Sly 1 symbols mangle the name: ApplyAsegCore, ReplaceCurrentAseg...): SMA::paloRoot / ASEGA::rootObject are used as the LO they are (sm.cpp SendSmaMessage, asega.cpp, aseg.cpp lost their `(LO *)` casts); cane.cpp's and gait.cpp's private `ALO` views renamed ATTACKOBJ / LEAPSUBJECT (three casts keep ReplaceCurrentAseg's mangled name in gait) | -- |
| SMT | sly2/aseg.h | size/offset asserts, field comments, GRFSMT bits `fsmtReversible` (1), `fsmtRetractable` (2), `fsmtRouteMask` (0xfffc), used by sm.cpp | -- |
| XF (sly2/so.h, unchanged) | -- | xf.cpp's slot functions (ResetXfValueRef, the value/derivative matrix builders, UpdateXfJobRange, SettleXfSlot, Add/Remove/SortXfActs, frame conversions, GetXfVelocity, EvaluateXfSlotMatrixDerivative, GetXfSlotMatrixAndDerivative, EvaluateXfSlotValue, UpdateSoXfWorldMatrices, LoadSoXfsFromBrx, ResolveXfId), aseg.cpp PredictAsegTransform / FindAsegChannel, act.cpp's goal setters, UpdateActLookAt and UpdateMatrixAct, ik.cpp, wr.cpp take XF/SO; the VU code moves the matrices and XF::velocity as qwords through casts of the MATRIX4/VECTOR4 members at the use (as jthands.cpp) | ~45 |
| ACT | sly2/act.h | xf.cpp's act lists call Attach/Detach/SwitchTarget/Update as real virtuals; cplcy.cpp's CPHANG::pact1 is read as ACT1::goalValue | 7 |
| CHN, AC, EA, ASEGA, SMA | aseg.h, act.h | the clip-channel readers (step.cpp GetStepAsega*, bentley, murray SetMurrayAttack, grab, pipe_jt, throw, prize RenderDprizeSelf, critter GetAsegSpeedForObject) take FindAsegChannel's CHN and evaluate its curve with a real `AC::Evaluate(g, dtScale, pv, pdv)` (floats first); FindAsegLabel returns EA (murray x5); ASEGA/SMA views in murray, hurt, prize, critter | ~30 |

Kept, with the reason: LoadSoXfsFromBrx calls SO slot 51 (GetXfFromIndex) through the vtable by hand (the real
virtual call reallocates the loop registers: 349 words off); GetXfSlotMatrixAndDerivative's slot-5 call stays by hand
(GetGoal is called without its dt). Blocked on so.h's XF (base family): asega.cpp's and constraint.cpp's XF views and
jthands' XFACT read XF +0xA8 as a float or as an IKREC/CHAINLINK/constraint pointer (a role-selected union: arrival
time in roles 9/0x11, the chain link or IK record of their acts, the reference XF otherwise), act.cpp's curve
Prepare views (four `OBJ`), UpdatePathAct (ACT4::Update: the local matrix as rows plus position) and AttachKind9Act write +0xC0..+0xFF as per-component floats, VECTORs and words (the slot's
segment block: rest value, Bezier control values, rotation rows by phase), and the qword-row matrix views (glob, shape,
solve, emitter, launcher, gadget, rendertree, hang, air, prize) need MATRIX4/VECTOR4 qword forms. so.h's names
for +0xC0..+0xF0 (restValue, rate, unknownE0, acceleration) are the four Bezier control values of a phase-1 segment
(EvaluateBezierPos(t1 - t0, *timeSource - t0, *speedSource, +0xC0, +0xD0, +0xE0, +0xF0)) and the two rest values of
LoadSoXfsFromBrx (+0xC0, +0xE0). WR's layout (wr.cpp's WRV) waits for WR to leave classes.h. blend.cpp (which
cannot include basic.h: its own qword/OID/CHN/ASEG declarations) is not attempted.

### Views unified (names/vu-f8-sound-ui): CRef, the sound types, the screens

The sound / UI / Splice-value family. Every function and data symbol of every touched file and of every includer of
a touched header MATCHes, and each touched object is identical to the branch start's (every allocated section's
bytes, every relocation's offset, type and symbol, the symbol table: a stricter check than nm). Private
struct/class/union definitions in the 30 touched .cpp files: 3,616 -> 3,274; the family's named views (CRef/CREFV/PAIRV,
SNDMGR*/SNDV/SNDREF/EASND, SCREENV/GADGETMENUV, ...): 253 -> 69.

| type | header | change | views removed |
|---|---|---|---:|
| CRef | sly2/cref.h | `CRefData`, the POD base (tag + payload union, `float *ag` added) for the C-style bodies; CRef gains `operator=(int/float)` (SetS32/SetF32 by label), `SetMatrix`, `SetSymid`, `SetBifk`, the inline `CRef(bool)` and `CRef(float)` constructors | 173 (bif 99, game, screen, memcard, prize, stepplayer, sw, menu, rumble, serialize, splicemap, class_registry, booty, hud, cplcy, stepguard, relay, lo, po, basic, guardbhv), plus bif's 10 NODE pair views |
| SND (CID151), BANKSND, STREAMSND, SNDSOURCE, SNDREF/SNDREFC, EASND | sly2/snd.h | the fields every view agrees on; STREAMSND's record a union by stream kind (FILEREC or SNDSTREAM); `GetKind` returns int, `SetCulled(int)`; STREAMSND's C-named constructor and pool `operator new` | ~60 in sound.cpp, 8 EASND (7 unused Lilly copies) |
| SNDMGR, SNDSTREAM, SNDFILE, DIALOGEVENT | sly2/sndmgr.h (new; separate from snd.h's 76 includers) | the documented 0x250 layout; `g_sndmgr` declared (D_003DD040) | ~30 SNDMGR views in sound.cpp, menu.cpp's, 5 interior macros (menu, cd, save) |
| SCREEN, HUDMETER | sly2/screen.h | HUDMETER's fields (0x50) | screen.cpp's SCREENV/GADGETMENUV |

Spellings that matched on the first compile: the hand vtable calls (`p->vtable[n].pfn(p + d)`) as the header's virtual
calls (AddRef/Release/Start/GetKind/Play/... : same code); the views' tag-first inline constructors `CRef(0x15, n)` as
cref.h's `CRef(n, 0x15)`; the sound views' counted pointers as SNDREF (no constructors: a default constructor makes
g++ emit a construction loop for g_sndmgr's music stack, 140/141 off); an expanded pair release as `delete` of the
CPair (splicemap). Watch for: `ref = pointer` compiles to cref.h's `operator=(bool)` when the view had a pointer
setter (class_registry's object results, BuildListFromObjects): matchtool masks the call target, the relocation
comparison catches it; such assignments are spelled `SetBasic(...)`.

Island (scratch islands.csv): stepplayer's UpdateStepplayerBootySave keeps its CRef as `CRefIsland` (its copy
constructor inline over the void*-returning copy function; cref.h's out-of-line one moves the stack slots, 37/286
off). Kept views: UpdateSndPositional/UpdateStreamsndPositional read LO's +0x28 word whole (lo.h's isInWorld
bit-field compiles to a mask test); the C-named constructors store the vptr by hand (SNDOBJ, GADGETMENU_ctor);
g_sndmgr's definition keeps its construct-and-destruct form (SNDMGRBASE/SNDMGRG: the destructor order puts the
counted pointers in the derived class). Not attempted: the SP* inline-release owners and the CFrame/CProc/CSidebag
views of splice/ (tied to gc.cpp's linkonce instances), WIDGET (classes.h, another family), screen.cpp's
per-function views (~100), CRichText/CFont. For sw.h's owner: FILEREC's key words are aw[0], aw[1], aw[13], aw[15].

### Views unified, continued (names/vu-f8-sound-ui-2): the screens, CFrame

Round 2 of the sound / UI / Splice family (from 5c3253ce). Every function and data symbol of the touched files and of
every includer of a touched header MATCHes, and each of the 114 objects compiled from them (screen.cpp, menu, memcard,
bif, proxy, splice/, the includers of cref/pair/varb/cframe/screen/snd/sndmgr.h) is identical to the branch start's
(allocated section bytes, relocations by symbol name, symbol table). screen.cpp's private struct/class/union
definitions: 152 -> 78 (bif.cpp 94 -> 92).

| type | header | change | views removed |
|---|---|---|---:|
| SCREENBASE, SCREEN, HUDMETER | sly2/screen.h | SCREENBASE's joypad (+0x10, JOY) and slot 9 as `SetFocus(int fFocus, SCREENBASE *pscrOther)` (menu.cpp's PAUSEMENU override renamed with it); SCREEN completed to its 0x40 bytes: the message popup of SetScreenMode (`nPopup` -1 none / 0 prompt / 1 timed, `tPopupEnd`, `idsPopup`); HUDMETER's five animation ids named (they were `anims[5]`) | -- |
| GADGETMENU, THIEFNET, JOBLIST, GMITEM, TNITEM, JLENTRY | screen.cpp (the screens' key TU; their classes stay there for the vtable order) | full layouts (0x60, 0x270, 0x128: JOBLIST's selected job +0x110 is what the file calls D_003DA4D0 / s_jobselslot); GMITEM's highlight animations; the gadget tables D_002C2DE8.. are GMITEM (were GADGETITEM, same bytes) | 74 definitions (the OBJ/S/E/ENT views and the hand vtable types they needed) in 50 functions: the constructors, popup timer, stick handlers, focus, reset, HUD meter, gadget menu (8), ThiefNet (14), job list (10), title |
| CFrame | sly2/cframe.h | +0x24 object-frame flag, `FindBindingPtr` (splice/frame.cpp's member), size 0x28 asserted | bif.cpp's BuiltinFindBindingOrNoop (its call-frame `O` was the frame) |

Spellings that matched on the first compile: hand vtable calls on the screen as virtual calls (`p->vt[10]` Show,
`[11]` Hide, `[9]` SetFocus, `[14]` SetMode); the vptr store of the C-named constructors stays by hand
(`*(void **)((char *)p + 0xC) = vtable`); GMITEM's flag word as the bit-fields (`fEquippable` bit 0, `fAvailable`,
`fAcquired`: `flags & 4` and `pe->fAcquired` give the same code); RefreshThiefnetItems' unsigned count compare as
`(unsigned int)p->citem > 0x40`; the job-list globals and ThiefNet's popup word (`#define D_003DA180`) as members of
`extern THIEFNET D_003DA150` / `JOBLIST D_003DA3C0`. A namespace-local `extern "C"` declaration identical to a
file-scope one makes calls ambiguous (g++ 2.95): such declarations were deleted, and SetGadgetMenuState's recursive
call is `::SetGadgetMenuState` (same function).

Kept (other families' types, noted for their owners): the widget the screens point at (`pchild`, `void *`; the bodies
read it through their WIDGET views, cast at the use: classes.h, the AI family's this round) and HUDMETER's hero
(`HERO(p)`, a cast to the STEPPLAYER view); the notice banner (CID154, classes.h: its state at +0x10 is not SCREEN's
layout); M_func_00258348's BASIC/CVarb/CSidebag release view (~BASIC's linkonce copy, _$_5BASIC). Not attempted: the splice/
CFrame/CProc/CSidebag/CPair/CVarb views and SP* owners of frame.cpp, eval.cpp, ref.cpp, proc.cpp, varb.cpp, pair.cpp,
sidebag.cpp (tied to gc.cpp's linkonce instances, the next step); callback.cpp's one-field CFrame view (including
cframe.h there clashes: basic.h's SL, gc.h's PtNew against its C declaration); CRichText/CFont (font.cpp's classes
need RGBA/QW/GIFS/TEX0 in a header; mpeg.cpp's and subtitle's views).

### Views unified, continued (names/vu-f8-sound-ui-3): the Splice nodes and their counted pointers

Round 3 of the sound / UI / Splice family (from fdc4f2e9), splice/ and the files that share its headers. Every
function and data symbol of the 45 objects compiled from splice/, callback.cpp and every includer of the changed
headers MATCHes, and each object is identical to the branch start's (allocated section bytes, relocations by symbol
name, the symbol table with binding and section, so every linkonce section name too). The family's views of the six
GC node kinds and their counted pointers (CPair/CVarb/CFrame/CProc/CSidebag/CMethod, SPW/SPO/SPF/SPZ/SPX/SPY/SPS/
SPN/SPREL/SPPAIR/SPFRAME, FRAMEV/GCHDR/GCNODE/METHOD): 61 -> 23. Struct/class/union definitions in the 12 touched
.cpp files: 159 -> 121.

| type | header | change | views removed |
|---|---|---|---:|
| CPair, CVarb | sly2/pair.h, varb.h | the nodes derive from `CGcObj<T>` (count `m_cref`, link word; deleting one runs CGcObj's unlink and frees into the pool, the code the hand-written `operator delete` gave); PtNew/Unlink declared as specializations | pair.cpp's, varb.cpp's |
| CRefOwner<T>, CRefPtrFns<T> | sly2/sp.h (new) | the inline-release owner (`T *p`; destructor `if (p && --p->m_cref == 0) delete p`; `Set` is CRefPtr<T>::Attach and `operator=` its assignment, called by symbol through CRefPtrFns<T>; `CRefOwner(spNull)` starts cleared) | 16 owner views in pair, varb, frame, sidebag, proc, eval, ref, serialize, splicemap, spliceobj, callback |
| CFrame | sly2/cframe.h | complete: declared `~CFrame()`, AssignBinding/FindBinding, SetSingleParent/AddParent/RemoveParent (frame.cpp's former C functions, now members by their C symbols), the binding link as varb.h's CVarbPtr (cframe.h includes varb.h first), MakeRoot/PtNew/Unlink specializations | frame, eval, ref, callback, splicemap (FRAMEV) |
| CProc | sly2/proc.h (new) | `CGcObj<CProc>` with CRefOwner links (ref.cpp's inline `~CProc` releases all three inline) and a declared destructor | proc, eval, ref (with SPX/SPY/SPZ/SPREL) |
| CSidebag | sly2/sidebag.h (new) | `CGcObj<CSidebag>` with a CRefOwner<CVarb> binding link; `CSidebag_AddBinding` declared | sidebag (with GCHDR), spliceobj |
| CMethod | sly2/method.h (new) | spliceobj.cpp's class moved; its own `operator delete` dropped (CGcObj's) | ref (CMethod, GCNODE), eval (METHOD), spliceobj |

**Where the linkonce copies come from.** g++ 2.95 emits an inline function or a template member, weak, in
`.gnu.linkonce.t.<symbol>`, in every TU that needs an out-of-line copy: an explicit instantiation, a call it does not
inline, or an address taken. Here splice/gc.cpp emits every instance (the CGcObj<T> members of the six kinds,
CRefPtr<T>'s attach for five kinds and its destructor and assignment for pairs, bindings and frames, and the frame
destructor), splice/ref.cpp the proc destructor (an `inline` body defined after its callers), object/spliceobj.cpp
CRefPtr<CMethod>'s attach; every other object only references them, and the per-object symbol tables must stay so.
A TU that only calls a member keeps calling it when it sees one of: an explicit specialization declaration of the
member (`template <> CPair *CGcObj<CPair>::PtNew();`: same symbol, no body, no copy), an explicit specialization of
the class whose members are declared only (pair.h's and varb.h's CRefPtr<CPair>/<CVarb>), or a destructor that is
declared and never defined (CFrame, CProc). g++'s `extern template` extension does not stop the emission in this
compiler (measured). Whether a call is inlined depends on order: a body is inlined only if it was compiled before the
caller (implicit destructors are synthesized when first needed, so ~CFrame, synthesized while ~CRefPtr<CFrame> is
compiled, is called from it and emitted; ~CProc, defined at the end of ref.cpp, is called from the functions above it).

**Why two counted-pointer spellings.** In the original every link and every owner was CRefPtr<T>, one template with an
inline destructor. A TU that inlined it in an owner and also called it out of line (the release of a list's next link
inside the inlined node destructor) emitted its own copy, which the linker dropped. Measured on pair.cpp: the real
template as CloneTo's owner emits weak copies of the destructor, PtNew, the constructor, PushFront and Attach. So the
headers spell the link (destructor and assignment out of line: the class specializations of pair.h/varb.h, basic.h's
template with the declared destructors of CFrame/CProc) separately from the owner (sp.h's CRefOwner<T>, the same one
word with the template's inline destructor); gc.cpp keeps its own views of the six nodes because it is the TU that
must see the real template and the implicit destructors to emit them.

Spellings that matched on the first compile: C declarations of the instances' mangled names (`CPair
*PtNew__t6CGcObj1Z5CPair();`) are "ambiguous" with the static member once the class template is instantiated in the
TU (g++ compares assembler names), so the calls are `CGcObj<CPair>::PtNew()`; the method release `if (--p->m_cref ==
0) delete p` gives the former null test, unlink and free; `world->scriptFrame = CGcObj<CFrame>::PtNew()` and
`->MakeRoot()` for splicemap's FramePtr_Assign/MakeRoot calls; callback.cpp's `PtNew` + `Attach` returning an owner
as `SPF frame(CGcObj<CFrame>::PtNew())` (this removed its clash with gc.h; its SL view is basic.h's SL). EvalApply's
8-byte owners and spliceobj's 16-byte-aligned ones are local derived structs of CRefOwner<T> (a pad word; an
`aligned(16)` attribute) with an `operator=` forwarding to the base's (a derived class's implicit assignment hides
it). Watch for: spliceobj's `list.Set(0)` was the assignment, and CRefOwner's `Set` is the attach: matchtool masks
call targets, the relocation comparison caught it. With the nodes' `operator delete` now CGcObj's (a template member,
instantiated where it is used), pair.h/varb.h/sidebag.h may be included before SLY2_INSTANTIATE_SLOT_POOLS(); a
non-template class whose inline `operator delete` names a pool still moves that pool's initializer first (spliceobj's
CMethod as first moved to method.h: 11 words off).

Kept (noted for their owners): gc.cpp's six node views (above); bif.cpp's 14 SPN/SP owner views (candidates:
`typedef CRefOwner<CPair>`, the `int pad[3]` ones as derived structs); proxy.cpp's M_CloneProxy frame views (RC/SP in
a union with the allocation size); callback.cpp's SPFV (a frame slot read through, owner unknown); the CSidebag release
views outside splice/ (lo.cpp's RF, screen.cpp's ~BASIC (_$_5BASIC), asega.cpp's homed CRefPtr<CSidebag> assignment);
basic.h's comment that frame.cpp/eval.cpp define their own CFrame views is stale.

### Views unified (names/vu-objects): the hack and robot tank classes

The gameplay-objects family, round 1 (from 9197b908): the registry classes of the two largest object TUs get their real
layouts in new headers, and the per-function views of them go. Every function and data symbol of hack.cpp,
robottank.cpp, class_registry.cpp and the other 74 includers of sly2/classes.h MATCHes, and each object is identical
to the branch start's (`nm -S`: symbols with sizes, binding and section; relocations by symbol name). The vtables and
class_registry's factories are unchanged (the classes keep their sizes, virtual layouts and constructors).
Struct/class/union definitions: hack.cpp 149 -> 34, robottank.cpp 148 -> 112.

| type | header | change | views removed |
|---|---|---|---|
| HACK, HACKSHIP, HACKBULLET, HACKENEMY, HACKLOCK, HACKWALL | sly2/hack.h (new; classes.h includes it at the classes' emission position) | real layouts with offset asserts; records HACKE (anim entry), HACKSHIPPART, HACKRULE (the enemy's per-state enter/stay limits), HACKWALLSEG (0x30, AddHackwallSegment) and HACKWALLPIECE (0x70, the allocated piece); HACK +0x1A0 is a union of the qword InitHack stores and the CLQ TakeHackDamage evaluates (decision 1.8's form); slot 82 is `Kill(LO *ploKiller)` (the killer lands in ZPR +0x08); HACKSHIP's and ROBOTTANK's cleared constructor word is the engine sound (CID151) | all 76 functions of hack: HACKV/HACKSHIPV/HACKENEMYV/HACKLOCKV/HACKSEG and the per-function S/O/P/E/Q/R/T/N/M/CTX/F views; real virtual calls for Kill, Reset, GetVelocity, SetVelocity, SetAngularVelocity, RotateToMat, GetLocalTransform, SetParent and the health widget's Show/Hide |
| ROBOTTANK, CID108 | sly2/tank.h (new, same arrangement) | real layouts: ROBOTTANKWHEEL (0x60), ROBOTTANKSIDE (0x14), ROBOTTANKAMMO (0xC), the drive/traction terms, health and widget, the AI's target/path/selection; the weapons are JSG objects (`struct JSG *apjsgWeapon[8]`) | the file-scope ROBOTTANKV/RCPICKUP/XPLISTV and the views of 24 functions (Init, the pickup search and collect, path reset, wheel contacts and bounds, the wheel/side/ammo setters, health meter, the weapon ammo helpers' owner, all CID108 functions) |

**Why the hack classes no longer need separate views.** Decision 1.1 kept HACKSHIP/HACKENEMY/HACKLOCK as separate
views over PO because their words at +0x1C8 seemed to lie inside HACK's tail padding. They do not: HACK's descriptor
size is 0x1C0, no HACK function touches +0x1C0, and each subclass's state and state time (+0x1C0/+0x1C4) are its own
first fields, so the derived classes start at 0x1C0 and every offset falls where the views had it.

Kept: the views of other families' records (XP contact sides, the contact request, a SOPART read with a 64-bit flag
word, the LO flag word read whole: the LOIsland reads of decision 1.4), the hand-tuned helpers, and in robottank the
large tuned functions (PostRobottankLoad, UpdateRobottank, UpdateRobottankAi, PresetRobottankAccel, the pinned
func_001D3450, RenderRobottankSelf, TakeRobottankDamage), SetRobottankState (its widget Show/Hide sites belong to the
cross-family pass) and the weapon (JSG) views. Not started: prize.cpp, grabspot.cpp. Natural spellings that matched
where the views had arithmetic: RenderHackwallSelf reads `wall->aseg[i].dlPiece.head` (was
`((N**)((int)wall+i*0x30))[0x1DC/4]`); InitRobottank stores `0.0f` into the float the view wrote as an int.

### Views unified (names/vu-hud): WIDGET, its subclasses and the sprite records

The HUD/widget family (from 9197b9081). Every function and data symbol of the touched files and of every includer of
classes.h, screen.h, widget.h and hud.h (83 objects) MATCHes, and each object is identical to the branch start's
(allocated section bytes, relocations by symbol name, the symbol table with sizes). Struct/class/union definitions in
src/game: 4,104 -> 3,916 (hud.cpp 345 -> 180); 11 sources, +598/-990.

| type | header | change | views removed |
|---|---|---|---:|
| WIDGET (0x110) | sly2/widget.h (new; classes.h includes it at WIDGET's place in the emission order) | real fields for 0x50..0x110 (state machine, labels, sprite groups, timeout, size and position, priority, exclusive list link, deferred load, sprite heap, argument texts, followed object, transform, render queue); relay.cpp's Lilly names in the field comments | hud.cpp's file-scope WIDGETV and ~60 per-function S/O/Q/T/L/P8/C8/OWNER/POSITION views; recon, wipe, relay (2), mode, menu, memcard (6), letterbox, sw.cpp (3) |
| WIDGET127 / 129 / 130 / 131 | sly2/classes.h | modal box (isOpen, keepsGamePaused), item counter (two 16-entry OID/TASK lists), recon prompt (five choice words; recon), episode menu (episodemenu's fields; menuState/menuSub/menuStateTime to keep WIDGET's state names free) | episodemenu's EPMENU, recon's WIDGETV |
| SPRITEGRP, SPRITEANIM, SPRITESLOT, SPRITEKEY, SPRITEITEM, SPRITEDESC, SPRITEXF, WIDGETLOAD; WIDGET_STATE / WIDGET_REQUEST | sly2/hud.h (new) | the records the widget plays (group 0x30, animation 0x18, slot 0x18, key 0x20, item 0x70, descriptor 0x40, 2x3 transform 0x20 aligned 16) and the deferred sprite-file load record (FILEREC + owner, heap marks, link) | ~70 A/E/H/C/F/Y/Z/QQ/M23/N views in hud.cpp |
| SCREEN::pchild | sly2/screen.h | `WIDGET *` (was `void *`); two casts at screen.cpp's assignments | -- |

The widget functions became members where the class declares the slot (WIDGET::Show/Hide/Update/PostLoad/
LoadFromBrx, WIDGET127::Show/Init/Update, WIDGET128::Init, WIDGET129::PostLoad/Update); base calls are qualified
(`WIDGET::Update()`, `LO::Init()`: direct calls, same code) and the hand-made slot 49/50/16/21/3 calls are virtual
calls (Show, Hide, Update, SetFlag8, HandleSpliceEvent) everywhere but game.cpp, robottank.cpp and screen.cpp (a
cross-family pass converts their Show/Hide call sites; their W/SUB/O views map onto WIDGET: visibility = state,
pageAnim* = the three labels at +0x70). Spellings that matched on the first compile: SPRITEXF (`float a[6]`, aligned 16)
for the qword-pair and `int a[8]` matrix copies; `&owner->spriteGroups[index]` for the `+ index * 0x30` byte
arithmetic; `int` item counts read into an `unsigned int` local for AddItemCounterOid's unsigned compare. Watch for:
an identical `extern "C"` declaration in a namespace and at file scope makes calls ambiguous (the namespace copies
were deleted); a namespace typedef of `uint`/`ulong` is ambiguous with common.h's under `using namespace`; sw.cpp
cannot include classes.h (its Lilly-port GRABSPOT: cc1plus would crash), hence widget.h.

Island (scratch islands.csv): RenderWidget (WIDGET::RenderSelf) keeps its view as `WIDGETIsland`: with a WIDGET-typed
pointer (even `this`) the packet-align loop's registers move (291/316 off; Begin/Finish on WIDGET each match alone).
Kept: WIDGET_ctor's view (the C-named constructor stores the vptr and BASIC's words by hand), the CLOCKIsland views of
SetWidgetState/UpdateWidget/StopSpriteSlot. Not attempted: the rich-text copy (CRichText2: its views disagree on int
vs float box fields and the colour representation), LoadSpriteGroups/LoadSpriteDescriptor's sprite internals (tuned:
the GROUP/ANIM/REUSE/KEY/ITEM views, the descriptor payload union), the key interpolation functions with register
pins (InterpolateSpriteSlot, HandleSpriteKeyEvents, ApplySpriteKeyPoses, AdvanceSpriteAnim), InitSpriteItem
(its +0x34..+0x58 item words), the STRINGGEN getters' player views. For sw.h's owner: SW::pepmenu is the WIDGET131
(typed `struct EPMENU *`; episodemenu reads it through one cast).

### Cross-family signatures (names/xfamily-sigs)

The header changes the per-family slices could not make alone (a base declaration and its overriders or callers in
different families' headers), made in lockstep so no virtual slot moved. Every function and data symbol of all 210
src/game sources MATCHes after each step, and each object's nm symbol set, relocations by symbol name and symbol
table with sizes are identical to objects built at a9b1a4ade.

| change | headers | real calls / views removed |
|---|---|---|
| EXPL slot 51 `ExplodeExplso(EXPLSO *)`, EXPLS slot 52 `FireExplso(EXPLSO *)` | emitter.h, classes.h (EXPLG, EXPLS) | emitter.cpp ExplodeWithDefaults, ExplodeExplParams, ExplodeExplgExplso (child call), EXPLS::ExplodeExplso (now a member); rip.cpp REMIT::Update |
| LO slot 20 `RenderAll(MATRIX4 *pmatWorld, float gFirst, float gSecond, SOBOUNDS *)` (RenderSoGlobset's arguments: 0 matrix = the object's own, 0 bounds = its own record) | lo.h, so.h | RenderSoAll is SO::RenderAll; lo.cpp RenderLoAtMatrix |
| LO slot 30 `RotateToMat(MATRIX3 *)` | lo.h, xfm.h, so.h, step.h, classes.h (LIGHT) | light.cpp EnsureSwDefaultLight; callers holding a MATRIX3 pass it uncast |
| SO slot 73 `int IsPosInside(qword pos)` | so.h, classes.h (WATER) | rip.cpp RIP::Update, class_registry's POINT_IN_BSP_QUICK thunk; IsPosInWater forwards the position. UpdateRip's +0xE8 read is SO::physics |
| WIDGET slot 49 `Show(int)` / slot 50 `Hide()` calls; WIDGET +0x8C `visibleTimeout` | classes.h | game.cpp (RestartHudWidget1/2, ShowCountdownHud, SetJobCompletionPhase), robottank SetRobottankState (after the pinned region), screen.cpp (hud meter, notice, gadget menu, ThiefNet, screen 0x52D, title; screen.cpp now includes classes.h) |
| `QWROW`, `MATRIX3Q`, `MATRIX4Q` (decision 1.8); STEP::matqTarget | basic.h, step.h | murray UpdateMurrayStrafe (its U/M3 views and the cast); 78 per-function row unions in 28 files |
| WR's layout (from wr.cpp's WRV) with WRE, ONZ/ONZE, WRECIRCLE/WREBEND | wr.h (classes.h forward-declares WR; class_registry includes wr.h) | wr.cpp's WRV, VECTORV and its V/M4 row views |
| SO +0x58/+0x5C `positionXf` / `rotationXf` | so.h | 13 files' uses renamed |

CPLCY Set/Revoke already took the entry argument (names/vu-f1-base-3). Not done:
- g_sw as a plain `SW` (dropping SWG): done in names/basic-dtor. The function at 0x258348 is BASIC's destructor under
  its own symbol `_$_5BASIC` (config, matched.csv and units.csv renamed from DestroyBasic). screen.cpp still emits it
  over a C view: a user-written `BASIC::~BASIC() {}` stores the vptr on entry, which the target does not (g++ 2.95
  leaves the vptr store out only of an implicit destructor's empty body: the original's ~BASIC was implicit), and an
  implicit one would be inlined into its callers. The view is declared `__asm__("_$_5BASIC")` and
  `__attribute__((weak, section(".gnu.linkonce.t._$_5BASIC")))` (bif.cpp's operator* pattern): an asm-labelled inline
  is not emitted from calls to `~BASIC`. The screens', sound's and memcard's static-object views call
  `((BASIC *)this)->BASIC::~BASIC()` (flag 2); cm.cpp's DestroyCm and sound.cpp's Decref* (flags x and 0) call a C
  view `BASIC_dtor` labelled `_$_5BASIC`. cm.cpp's CMG stays: a plain `CM` needs the implicit ~CM, which is inlined
  (and needs SND complete for CRefPtr<SND>) where the target calls DestroyCm (0x258290) out of line.
- XF's matrices as MATRIX4Q unions (xf.cpp, UpdateSoXfWorldMatrices casts): XF is being changed on another branch.
- The remaining private row unions: at file scope, redefined in the same scope, or kept because QWROW's unsigned
  `qword` changes a signedness (`&u.q` passed as a signed `qword *`) or the code (shape.cpp StepChainRk4/StepChain).
- The other widget Show/Hide hand calls (hud.cpp, menu.cpp, memcard.cpp, episodemenu.cpp: their own WIDGET views).

### Views unified (names/vu-objects-2): the prize and grab-spot classes

The gameplay-objects family, round 2 (from 34a45b35). Every function and data symbol of prize.cpp, grabspot.cpp,
robottank.cpp, air.cpp and the 77 includers of sly2/classes.h MATCHes, each object identical to the branch start's
(`nm -S`, relocations by symbol name); vtables and factories unchanged. Struct/class/union definitions: prize.cpp
132 -> 71, grabspot.cpp 61 -> 16, robottank.cpp 112 -> 108, air.cpp 208 -> 207.

| type | header | change | views removed |
|---|---|---|---|
| DPRIZE, COIN, MEDICINE, KEY, LOOT (+LOOTE), CLUE, CLUEPROXY, VAULT | sly2/prize.h (new; classes.h includes it at DPRIZE's position, the eight classes kept in their relative order, so prize.cpp emits its vtables as before) | real layouts with offset asserts; slot 76 is `SetState(int)` in all five DPRIZE classes; VAULT is a PO (prize.cpp's VAULTV wrongly derived it from DPRIZE): its dial sub-state, digit targets/animations, dialogs and per-state relays (the D_002C0EC8 oid table's offsets agree) | all prize.cpp functions but the save-key ones: the file-scope DPRIZEV/CLUEV/LOOTV/VAULTV/CLUEPROXYV and the per-function S/O/P/KEYV/LOB views, including the tuned ProjectDprizeTransform, CheckDprizeBounce, EmitDprizeGlint, UpdateVaultInput, SetVaultDialState |
| GRABSPOT, CID12, CID34, CID38, BARREL, CID35, CID36, CID27 | sly2/classes.h (in place) | GRABSPOT's data words named (xf.cpp's GRABSPOTDATA: tunnel, busy radius, priority, player mask); GetOwner returns `LO *`, GetClosestParam/GetGrabPos have their override signatures; each class's grab-list link is a `DLE dleGrab` (sw.cpp's InitDl offsets 0x1B4/0x104/0x174/0x154 confirm it; it is what Clone preserves); CID35's RSMG and task-pair tables, CID36's linked objects, BARREL's carry state and carrier | all grabspot.cpp functions' class views and the file-scope GRABPOLE/GRABSPOT35/GRABSPOT36/BARRELV; air.cpp's Lilly CID88 copy |
| ROBOTTANK | sly2/tank.h | — | the weapon-list views of FindReadyRobottankWeapon, FindRobottankWeapon, CloneRobottank, AddRobottankWeapon |

Spellings that matched: real virtual calls where the views called through the vtable by hand (DPRIZE SetState,
SO::GetXfFromIndex for ProjectDprizeTransform's slot 51, GRABSPOT GetOwner/GetClosestParam/GetGrabPos, BARREL
SetVelocity/HandleSpliceEvent/SetParent, CID35 SendMessage); float words a VU asm broadcasts as bits read as
`*(int *)&prize->bounceRadius`; `VuSub<qword>` where a header qword (unsigned) meets the file's signed one. Did not
match: passing the GRABSPOT base by conversion (`InitGrabspot(wall)`, also through a reference) adds a null test,
so the Init functions pass `&obj->ptn` (its first field). Kept: the views of other families' records (the prize's two
XFs as quadword channels, the vault's dial-timer XF whose +0xA8 so.h names as a pointer, rip and listener vtables, the
carrying player's JTMODE and attach XFs, JSG weapon records, ZPR-like message blocks), TakeRobottankDamage and the
other hand-tuned tank functions. Not done: grab.cpp's Lilly copies of the grab-spot classes (CID11/12/13/19/27/34/38/39
and three GRABSPOT copies with other signatures; the file does not include classes.h and the copies mix player-family
types), ALARM's view in AddLootEntryObject.
