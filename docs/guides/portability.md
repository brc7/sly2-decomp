# Portability register

Every mechanism this repository has for compiling its sources somewhere other than the EE (the native 64-bit port
builds these files with clang), what it does in the matching build, what a native build does with it, and why the
matching form is not simply the portable one. The rule behind all of them: the PS2 build stays byte-identical, so a
hook expands on the EE to exactly the code that was there before, and the portable form lives beside it.

History: `git log --grep='port request'` (v1.10 onwards): g_sndmgr's full definition, CStreamBis in a header,
SW_ctor slot sizes, Init list-link offsets, pointer-array and record sizes, sizeof(IKREC), uptr/sptr on the load
path, heap bounds and SLY2_OFFSETOF as hooks, level-load records with their real layouts, then (v1.13) the option
table offsets, class-descriptor sizes, LO's tagged update word, the ik/pipe_jt/gait views, SLY2_EE_LAYOUT with the
colour-table and bitmap file records, and breakable's pointer-wide action records; (v1.14) C fallbacks for the VU0 asm
on the load path (section 2) and pointer types for three object-reference option members; (v1.15, driven by the port's
Paris boot and its heap-guard run) the SNIP tables and their target members, load-path allocation sizes, real types for
the CloneLo / screenfade / vis / hud / bbmark / memcard-dialog views, LO's tagged word read pointer-wide everywhere on
the load path; (v1.16) the remaining load-path views (shape.cpp's SHAPE views, CID14 / CID98 / CID112's own fields in
classes.h for cnvo / breakable / launcher, xf.cpp and ik.cpp's XF views, prize's AddLootEntryObject, PostJtLoad's
member stores), the rail / pipe / shape / CID14 Clone forwarding, SW_ctor's last list offsets, MURRAY's ASEGARC and
the GRABSPOT base without a null test, and from the port's boot-3 list: pointer-typed message / SM / chooser /
EvaluateBezierPos arguments, FramePtr_Assign on &scriptFrame, LEVELLIST and POSTLOADREC in headers, more list
offsets, and every EnsureAsegBlendDynamic entry stride from sizeof; (v1.17) the heap guard's `new` overruns (BOOTY,
CID37, CID43, CID99, SAFEHOUSE, TURRET declare their fields; CID125's guard table from sizeof), SW_ctor's event /
callback / diract / blip slots from header records (sw.h SWEVENT, SWCALLBACK, DIRBL; sly2/blip.h BLIPBATCH), the
shown-screen list on SCREENBASE::dleShown, water.cpp's hand vtable calls as virtual calls (SO slots 71 / 72 with their
real signatures, slot 51 = EXPL::ExplodeExplso), WATER's and PIPE's fields, CloneGeom's sizes; (v1.18) water.cpp on
WATER / SO / STEP / PO (SO::soFlags is the +0x148 unit as one doubleword; SOPHYS buoyancy / waterDrag; XA's link
fields; PO lastWakeTime / wakeSound), path.cpp's zone BSP (typed REGTREE: REGBSPNODE, REGLINKLIST, PATHZONE /
PATHLINK pointers) and path-link endpoints on PATHLINK / PATHLINKEND (PATHLINK::linkIndex), geom.cpp's PLANE with
SURF's pointer words, gait.cpp on ASEGARC, and the per-frame solver's views (solve.cpp SOV / SOTREEV / TL / XP with
the real classes' layout on every target); (v1.19, the port's boot-4 per-frame list) the hand vtable calls as virtual
calls where they match and under SLY2_EE_LAYOUT where not (blend.cpp's curves, OVRA's float curve, cm.cpp, shd.cpp
UpdateShaders, stepguard, rhythm, so.cpp SetSoXf, emitter / sw / bomb / grab slot 51, robottank slot 31, proxy, the
FireMissile thunk, missile / bentley slot 33, pipe_jt slot 36, watchdog slot 73, murray slots 29 / 62, safehouse slot
76, booty slot 35), UpdateGeomWorld's C fallback, the `int`-typed pointers (ReplaceSma / ReplaceSmaEx, the splice
thunks' object arguments, HandleSpliceEvent's argument vector and result, bif.cpp's BIFK forwarders and
find-class-objects, SO::AddCustomXps' BSPs), EnsureTargetRecordById's result, SetSoIdProxy's and NewSoPartHandle's
pass-through arguments, and from request 4's structs: so.cpp's target records and CID149 / CID150 handles, relay.cpp's
RELAYJSG, REGENT's size; (v1.20) tools/cleanup/views, which replaces TU views by their header classes and keeps only
byte-identical rewrites, applied on the frame path: air.cpp (PresetJtAccelJump on JTAIR / STEPPLAYER / XF,
ProjectJtVelocityOnContacts and FCheckJtStuck on XP / XPLIST / SOTREE, UpdateJtActiveJump's transform and stick views, the jump
update's link records, the file-scope XP), step.cpp PresetStepAccel (STEP / XF / MATRIX3Q), path.cpp's obstacle
`S : XFM` (PATHZONE), mark.cpp, jt.cpp, trail.cpp (SUBGLOB / SUBGLOBX), glob.cpp, hud.cpp, act.cpp; XP / XPS in
sly2/xp.h; and from the port's request 6: EvaluateKeyedFloatCurve's key, ProjectJtVelocityOnContacts' uptr cursor,
guardscript.cpp's pointer arguments (section 3 lists what is left); (v1.22) declarations against their mangled
signatures (tools/cleanup/signatures/sigcheck.py): operator*(MATRIX4, MATRIX4) defined as the struct-returning C++
function, hand-passed result pointers bound through `SLY2_SRET_LABEL` (native adapters), bif.cpp's builtins on
(int, CRef *, CFrame *) with native CRef entries for the C-style ones (`SLY2_BUILTIN_C`), FindSwObject's context and
other int-typed pointer parameters; from the port's request 8: `SLY2_BSP_NO_LIMIT`, the VU0 microprogram 0x1C0 cull
in C, LSG's offsets, ConstructLeap's vptr-clobbering stores; (v1.23) port request 8's leftovers (C versions of UpdateStepPhys
and DriveStepguardFromAnim, RenderStepguardSelf on STEPGUARD / CM members, prize.cpp's XFs as XF, StartLeap's
sizeof(LEAP), BLIPG's fields in classes.h with blip.cpp / emitter.cpp / frm.cpp on them), C fallbacks for every
statement of the port allowlist's 'frame' class but blip.cpp's microprogram 0x10, sly2/xp.h included from so.h,
sly2/vis.h (CID5) for cm.cpp SetCmVisZone.

## 1. Guards and hooks

| hook | EE (matching build) | native build | where |
|---|---|---|---|
| `SLY2_EE_ASM` (sly2/port.h) | defined (`__R5900__`, unless `SLY2_NO_EE_ASM`): inline EE assembly allowed | undefined: the `#else` plain-C body is compiled | `grep -rln SLY2_EE_ASM src include` (sly2/vu0.h, mat.cpp, vec.cpp, xf.cpp, shd.cpp, booty.cpp, rip.cpp, sw.cpp, path.cpp, gs.cpp, geom.cpp; v1.23: step.cpp, stepguard.cpp, guardbhv.cpp, cm.cpp, glob.cpp, dirblend.cpp, jt.cpp, hud.cpp, so.cpp, shape.cpp, sticky.cpp); section 2 |
| `SLY2_EE_LAYOUT` (sly2/port.h) | defined (`__R5900__`, unless `SLY2_NO_EE_LAYOUT`): EE data layout (4-byte pointers, file records read straight into in-memory structs) and g++ 2.95 vtables | undefined: `#else` reads file records field by field, uses plain virtual calls | shd.cpp LoadClutFromBrx / LoadBmpFromBrx; the hand vtable calls that do not match as virtual calls: gait.cpp StartLeap (slot 35), cnvo.cpp MergeCid14Obstacles (31 / 29 / 9, `SLY2_VTE`), cm.cpp UpdateCmLast / GetCmCpdefi (50; the #else includes sly2/so.h), stepguard.cpp RestartStepguard (27 / 3 / 26), rhythm.cpp (35), so.cpp SetSoXf (35, with its VU0 transform in C), robottank.cpp MarkWheel / UpdateRobottank (31), proxy.cpp AcquireProxySpare (42), missile.cpp PresetSplmissAccel (33), bentley.cpp's gadget drag (33), pipe_jt.cpp (36), booty.cpp SetBootyState (35) |
| `uptr`, `sptr` (sly2/port.h) | `unsigned int`, `int` | `__UINTPTR_TYPE__`, `__INTPTR_TYPE__` (override: `SLY2_UPTR_TYPE`, `SLY2_SPTR_TYPE`) | `grep -rlwn 'uptr\|sptr' src include` |
| `SLY2_OFFSETOF(T, m)` (sly2/basic.h) | `((int)(char *)&((T *)0)->m)` | define first, e.g. `((int)__builtin_offsetof(T, m))` | layout asserts, list-link offsets, g_aeopid's `EOPID_OFF` |
| `SLY2_STORE_VPTR_AT`, `SLY2_SET_VPTR`, `SLY2_SET_VPTR_AT`, `SLY2_STORE_VPTR`, `SLY2_VTBL` (sly2/vtables.h) | word store of `_vt$<n><CLASS>` at +0xC (BASIC), +0x18 (ACT), +0x30 (CBinaryInputStream), +0xEC (RIP), 0 | override `SLY2_STORE_VPTR_AT` alone (vptr first in the Itanium ABI), or let the constructors become real ones | `grep -rln 'SLY2_SET_VPTR\|SLY2_STORE_VPTR' src` (24 files) |
| `SLY2_HW_*`, `SLY2_UNCACHED`, `SLY2_UNCACHED_ACCEL`, `SLY2_HW_OVERRIDE` (sly2/hw.h) | the PS2 addresses as integer constants (scratchpad 0x70000000, GS/DMA/VIF registers, uncached mirrors) | define the ones it emulates first, in the header named by `-DSLY2_HW_OVERRIDE='"port_hw.h"'` | gs.cpp, sprbuf.cpp, sw.cpp, hud.cpp, glob.cpp, mpeg.cpp |
| `MCJOBARG`, `MCJOBCONST` (sly2/save.h) | `long`: the callers load an MCJOBPMF constant into one 64-bit register through the packed union (ldl / ldr) | `MCJOBPMF`: the record itself, by value | memcard.cpp's CreateMemcardThread callers; the constants are defined as MCJOBPMF |
| `SLY2_SSGE_SLOT_CB` (sly2/guard.h) | `0x1c`, the original's request (the 0x10-rounded slot holds SSGE's last word) | `sizeof(SSGE)` | sw.cpp SW_ctor |
| `SLY2_ACT_SLOT_CB` (sly2/act.h) | the largest ACT kind's sizeof (0x80, ACT2) | the same expression | sw.cpp SW_ctor (not a guard: free) |
| lo.cpp `LOFLAGS` (under `SLY2_EE_LAYOUT`) | CloneLo copies LO's four flag bit-fields through an int / uint bit-field view of the unit at +0x28 | copies lo.h's fields | lo.cpp CloneLo |
| `SLY2_HEAP_BEGIN`, `SLY2_HEAP_END` (platform/memory/memory.cpp) | the end of the program image (`D_0047220F`) and the top of the 32 MB RAM (0x02000000) | a static buffer's bounds | memory.cpp StartupMemMgr |
| `CStreamBisStorage` (sly2/streambis.h) | room for a CStreamBis without g++'s implicit constructor; the callers call `CStreamBis_ctor` / `_$_10CStreamBis` themselves | provide those two C entry points (or override the two declarations); sizes follow `sizeof(CStreamBis)` | sw.cpp, hud.cpp (the class itself: brx_sw.cpp, memcard.cpp) |
| EE layout asserts (the `typedef char assert_...[cond ? 1 : -1]` lines) | compile-time checks of the EE offsets and sizes | not true on a 64-bit host: the port rewrites them into a non-failing sink (its own generator) | every sly2/ header, many TUs |
| `SLY2_GRABSPOT_BASE(T, p)` (sly2/classes.h) | `p` plus the GRABSPOT base offset of T, taken from `static_cast<GRABSPOT *>((T *)0x100)` (folds to sizeof(first base), no null test) | the same expression gives the ABI's offset | the OwnFields accessors (GetPipeOwnFields and the like, GetBarrelOwnFields) (defined at the end of classes.h), grabspot.cpp's InitGrabspot calls |
| `SLY2_SRET_LABEL(ee, native)` (sly2/port.h) | `__asm__(ee)`: the EE symbol; g++ 2.95 passes a struct result's address as the first argument, so a declaration that writes it out as a pointer parameter (or takes an explicit out-pointer as a struct result) calls the same code | `__asm__(native)`: an adapter next to the definition, outside SLY2_EE_LAYOUT, with the declaration's own spelling (arm64 passes the result address in x8, not x0) | mat.cpp operator* (26 callers, adapter MulMatrix4Into) and BuildRotateVectorsMatrix (7 callers taking the matrix as a struct result, BuildRotateVectorsMatrixValue); basic.cpp EvalExpression (EvalExpressionInto in eval.cpp). The TU defining an adapter must include sly2/port.h before the guard (eval.cpp did not: its native branch compiled on the EE until it did) |
| `SLY2_BUILTIN_C(sym)` (app/bif.cpp) | nothing: the builtin table calls the C-style body (`CRefData *sym(CRefData *ret, int cref, CRefData *args, CFrame *frame)`) | the body becomes `sym_c` and `sym` is a CRef-returning entry calling it: the type eval.cpp's BIF::function calls through | the 98 C-style builtins in bif.cpp |
| `SLY2_BSP_NO_LIMIT` (sly2/port.h) | `(~(uptr)0 >> 1)` = 0x7FFFFFFF, the literal the callers passed | the largest positive pointer-sized value: above every heap address (no guard needed) | every BSP walk caller (clip, geom, so, tn, detector, bomb, air, exit_warp, vol, mark, prize, rip, vis, robottank, grab); caller-side declarations take the limit as `uptr` (bsp.cpp's definitions compare `BSP *`) |
| `SLY2_FDIV(a, b)` (sly2/port.h, v1.24) | `((a) / (b))`: div.s, which gives +-FLT_MAX for a zero divisor (sign of a xor sign of b; also 0 / 0) | `Sly2EeFdiv`: a / b, or +-FLT_MAX with that sign when b is 0 (IEEE gives Inf / NaN) | vec.cpp FindClosestPointOnLineSegment (a zero-length segment); use it for other per-frame divisions by a length that can be 0 |
| `EOPID_PMF`, `EOPID_PMFI`, `EOPID_PMFT`, `EOPID_VPMF`, `EOPIDW` (app/class_registry_options.inc) | g++ 2.95 pointers to member functions: `{short delta; short index; pfn or delta2}` | redefine the four macros and `EOPIDW` in the native PMF ABI (Itanium: `{ptr, adj}`, ptr = address or 1 + vtable offset) | class_registry_options.inc (documented next to the macros) |

Why the matching form cannot be the portable one:

- **Inline assembly** (`SLY2_EE_ASM`): VU0 macro-mode instructions (`vmulax`, `vsqrt`, `lqc2`, `qmfc2`...), the
  128-bit MMI moves and the register pins that fix g++'s allocation cannot be produced from C by ee-gcc 2.95; the
  asm is the only spelling that matches. The C equivalent is semantics only.
- **File records** (`SLY2_EE_LAYOUT`): the level file stores colour tables and bitmaps as 0x10 / 0x18-byte records
  with 32-bit offsets, and the game reads them with `Read(sizeof(CLUTHDR), pclut)`: `sizeof(CLUTHDR)` equals the
  on-disk size only with 4-byte pointers. shd.h's `CLUTFILE` / `BMPFILE` are the file records (asserted on every
  target); the `#else` path reads one and widens it. Writing the EE path through the file struct would add copies
  and change the code.
- **Hand-written vtable calls** (`SLY2_EE_LAYOUT`): a few functions load the vtable entry (`VTE`: delta, pfn) before
  the arguments are built, which a plain virtual call does not (the schedule differs). That spelling depends on
  g++ 2.95's vtable format and the vptr at +0xC. ik.cpp and pipe_jt.cpp's calls matched as real virtual calls and
  are plain C++ now, and so does xf.cpp's ACT slot 5 (GetGoal, v1.16: the hand-written call passed no dt, but the
  caller's g is still in $f12, so `GetGoal(..., g)` is the same code), and every water.cpp call (v1.17: slots 4, 33,
  34, 51 on an EXPL, 71, 72; booty.cpp's slot 51 is the same ExplodeExplso); gait.cpp's slot 35 (SO::SetVelocity) and
  cnvo.cpp MergeCid14Obstacles' slots 31 / 29 / 9 did not and are guarded. v1.19 converted the per-frame ones: real
  virtual calls (same code) in blend.cpp (AC::Prepare / Evaluate, ACG::Evaluate), asega.cpp / xf.cpp (OVRA's
  ACG::Evaluate), shd.cpp UpdateShaders (SAA slot 8 through a BASIC-derived view: classes.h clashes with shd.cpp),
  emitter.cpp (BLIPG::SubscribeObject), sw.cpp / bomb.cpp (EXPL::ExplodeExplso), grab.cpp (CID35::OnTriggered),
  safehouse.cpp (CID100::SetState), proxy.cpp AddProxyCopy (CLoadCollector::Add), class_registry.cpp's FireMissile
  thunk (MISSILE::Fire), missile.cpp's Velocity helper, watchdog.cpp (IsPosInside), murray.cpp (TranslateToPos,
  IsFixed); the rest are guarded (section 1's table). The usual reason is the schedule: the hand form loads the
  entry (or the vtable word) before the arguments are built, a virtual call after. Hand calls inside hand-scheduled
  asm statements stay (section 3).
- **Null-free base conversions** (`SLY2_GRABSPOT_BASE`): `static_cast<GRABSPOT *>(p)` and `&static_cast<GRABSPOT &>(*p)`
  both add g++ 2.95's null test (a `movz`), which the original accessors do not have; the offset of a cast of the
  constant 0x100 does not. The accessors moved to the end of classes.h (the classes must be complete; a template
  helper is instantiated at once by g++ 2.95), which changes the order of their linkonce sections in
  class_registry.o (objcmp's `sec` view); the bytes and the link are unchanged (link.py places linkonce sections
  by name and address).
- **Pointer-sized integers** (`uptr`, `sptr`): the matched code casts pointers to `int` / `unsigned`; the signedness
  picks the EE instructions (an unsigned `& ~0x3F` needs a 64-bit mask), so `int` becomes `sptr` and `unsigned`
  becomes `uptr`, never the other way round. breakable.cpp's action record `I {int a; sptr b}` is one: action kind
  0xC keeps a pointer in `b`.
- **Offsets and sizes** (`SLY2_OFFSETOF`, `sizeof`): literals were the decomp's first spelling; the EE value is the
  same, so these are free. g_aeopid's member offsets are `EOPID_OFF(class, member)` and the class descriptors' `cb` is
  `sizeof(class)` (CloneSaa copies `instanceSize` bytes).
- **vptr stores**: constructors written as C functions (for asm labels, views or call order) store the vtable
  themselves; g++ 2.95 puts the vptr after the first base's data, the Itanium ABI first.
- **LO's tagged update word** (LO+0x40, `unknown40` / `ploupd`): either an LOUPD pointer or the flags with bit 0 set.
  Readers take it as `uptr` (or `sptr` where the matched local was `int`) through `LO::ploupd` and write the flags
  through `LOUPD::grf` or `LO::unknown40`; LO's own `int unknown40` stays (making it unsigned would change sign
  extensions elsewhere). On a little-endian host the low word holds the tag. lo.h includes sly2/port.h and defines
  LOUPD (sw.h has the same definition under the same guard). v1.15 rewrote every remaining `int v = x->unknown40;
  ... *(int *)v` copy (tools: a scratch rewriter, line-local); left: so.cpp SetSoGrfBit8 (register-pinned asm).
- **Bit-field copies** (CloneLo): lo.h declares LO's flag bit-fields `long` / `ulong`; g++ 2.95 folds a copy between
  same-position fields of that type into and/or masks, where the original (int / uint fields) extracts and reinserts.
  Casts do not change it, so the EE copies go through a local int-typed view under `SLY2_EE_LAYOUT`. Single-bit reads
  that the original extracts (`isInWorld`) match with `(int)x & 1`.
- **Wider native records in EE-sized storage**: static storage and allocations sized from sizeof (the memcard dialog
  storage from SCREEN, the slot heaps, the heap-guard sites); where the EE asks for less than sizeof (SSGE) the literal
  stays behind a guard.

## 2. VU0 fallbacks

Without `SLY2_EE_ASM` the VU0 macro-mode asm (and the EE-core asm next to it: MMI lane moves, FPU `sqrt.s`, word
loads and stores) has plain C equivalents:

- **sly2/vu0.h**: every shared helper (VuSub ... VuLengthXY, QwordX/Y/Z, QwordFromFloats, SqrtFloat, FpuMax/Min/Abs)
  has a C body after `#else`, on the lane helpers `VuGet` / `VuPut` / `VuMake` (a `VuLanes {x, y, z, w}` view of a
  quadword) and the statement helpers `VuCopyBits` / `VuWiden` (qmtc2, qmfc2, mtc1, mfc1, sq), `VuDotXY`,
  `VuProt3w` / `VuPextuw0`, `VuKeepW` (a "+j" operand whose w the asm does not write), `VuScaleX2`, `VuAddScaled2`.
  The header includes sly2/port.h itself, so the guard never depends on include order.
- **mat.cpp**: InvertMatrix3Cofactor, NormalizeRotateMatrix3, LoadRotateMatrixSinCos, BuildAxisAngleRotationMatrix,
  BuildRotationMatrixFromVector, LoadRotateMatrixPanTilt, BuildOrthonormalMatrixZ, TiltMatUpright,
  BuildRotateVectorsMatrix, MulMatrix3Matrix3, DecomposeRotationMatrixToVector, operator*(MATRIX4, VU_FLOAT), `ml`,
  `__ml__FRC7MATRIX4T0`, LoadMatrixFromPosRotScale / Scalar, InvertMatrix4Rigid, SmoothMatrix, GetMaxRowLengthMatrix3
  (whole functions or their asm helpers; the named-return helpers are ordinary returns on the C side).
  `__ml__FRC7MATRIX4T0` runs the VU0 microprogram at 0x60 on the EE (vcallms) with a's rows in vf1-vf4 and b's in
  vf5-vf8; the microcode makes result row i = sum_k b[i][k] a.r[k], the row-vector product **b * a**. Until v1.24 the C
  body formed a * b (the port's boot-10: CM::matViewProj came out wrong natively and VU1 clipped everything); see
  "v1.24" below.
- **vec.cpp**: every asm statement (all 45 functions are covered).
- **xf.cpp**: BuildXfValueMatrix, BuildXfValueDerivativeMatrix and every VU0 statement except the `vcallms 0x60`
  sites. **shd.cpp**: ColorCorrectVector, AddBitmapToHash / AddClutToHash (whole C functions over D_003DD010 /
  D_003DD018). **booty.cpp**: every asm statement (SetBootyState and the distance helpers). **rip.cpp**: LaunchMatch.
  **geom.cpp**: TransformPointsVu0, whose EE body runs the VU0 microprograms at 0x120 / 0xF0 (vcallms); its C body
  is the microcode's: all four lanes p.x r0 + p.y r1 + p.z r2 + r3 (p.w taken as 1; until v1.24 it kept the input's w). **geom.cpp UpdateGeomWorld** (v1.19): the plane loop (microprogram 0x150 over the transposed inverse,
  pinned EE loads) is under SLY2_EE_ASM; the C body takes each normal through the inverse transpose of the 3x3 part
  (InvertMatrix3Cofactor), renormalises it and rebuilds the plane through its anchor point (InitPlaneFromNormalPos),
  as CloneGeom does, which is what microprogram 0x150 does (checked in v1.24, below). InitPlaneFromNormalPos itself (bsp.cpp) is still asm only.
  **VU0 microprogram 0x1C0** (v1.22, the sphere-against-view cull of glob.cpp RenderSoGlobset / SubmitGlobsetRelight
  and rendertree.cpp CullSceneRenderList): sly2/vu0.h VuCullSphere (C side only) is the microcode as the port
  transcribed it from .vutext (four side-plane tests, then the range test with a fade band of 400 at its far end;
  visibility in [0, 1] and centre - camera); glob.cpp's GetSphereVisibility[Sync] and rendertree.cpp's StartCull /
  WaitCullVisibility / GetCullSphereOffset have #else bodies on it (rendertree keeps the two result registers as file
  state between the start and the reads). Checked on the host against a hand case only; the VU0 harness does not
  model microprograms.

**v1.23 (port request 8 and the port's ee_asm_allowlist 'frame' class)**: step.cpp UpdateStepPhys (whole function:
the phys part's points through inverse(body world) * phys world, SmoothValue per component, velocity = (next - old)
/ dt) and stepguard.cpp DriveStepguardFromAnim (whole function on ANIMBINDING / LEAP / CID146::velocity / STEPGUARD);
RenderStepguardSelf's Length; cm.cpp GDistPlane; glob.cpp StoreSquaredLengthBits and UnpackSubglobBones (the bone loop's
microprogram 0x60 as mat.cpp's operator* documents it); stepguard.cpp LengthXY and the empty-asm macros OPAQUE_COUNT /
PAD1 (nothing natively); guardbhv.cpp Moved, QW_LENGTH_COPY, UpdateBhvjuggled's velocity save, HandleBhvguardMessage's
knock-back axis, ChooseBhvTargetsTarget's setup (its `lui 0x2C` / `lui 0x2A` address arithmetic is D_002BE710 /
D_002A55FC by name); dirblend.cpp BlendPositive / BlendNegative; jt.cpp UpdateJtAttachCorrection's row stores;
hud.cpp SlotSound; so.cpp SetSoGrfBit8 (the tagged LO word: unknown40 when bit 0 is set, else LOUPD::grf); shape.cpp
GetShapeBounds' vmove / li.s; sticky.cpp FindMeshClosestPoint (whole function; u and v as the EE computes them:
u = (d . (n x e2)) / (e1 . (n x e2)), v = (n . (d x e1)) / the same, e1 = a - c, e2 = b - c, n = e1 x e2, d = input - c)
and its Unit helper. Tested with the harness (scratch v123/vu0test/t_v123.cpp: the asm texts of GDistPlane,
StoreSquaredLengthBits, LengthXY, Length / Moved, the knock-back axis, the blend axes, sticky's triangle statements
(their lqc2 / sqc2 / qmfc2 parts replaced by operands) and closest point, UpdateStepPhys' transform; 100,000 random
cases each, maximum error 0). Left then: blip.cpp UpdateMs10 (`vcallms 0x10`); UnpackSubglobBones' reading of 0x60 was mat.cpp's (wrong, see v1.24).

**v1.24 (the microprograms from the microcode)**: the VU0 microcode is one upload chain in .vutext, D_00259DF0 (DMA cnt
tag, VIF NOP, MPG of 84 instruction pairs at VU0 address 0). Disassembled (a scratch disassembler; its reading of 0x1C0
reproduces the port's boot-8 transcription, I = 400 and all), the entry points are:

| entry | instructions | does | C side |
|---|---|---|---|
| 0x10 | 2..10 | b' = r0 b.x + r1 b.y + r2 b.z + r3 (xyz, rows vf31..vf28; w = b.w), a' = a + b' dt.x (xyz; w = a.w), bounds min / max of a' (vf26 / vf25) | blip.cpp UpdateMs10 #else (new) |
| 0x60 | 12..27 | for N = vf5..vf8: vfN = vf1 N.x + vf2 N.y + vf3 N.z + vf4 N.w (xyzw) | mat.cpp operator*(a, b) = **b * a** (fixed), glob.cpp UnpackSubglobBones = inverse bind * world (fixed; was world * inverse); xf.cpp's two asm-only sites are local * parent (comments fixed) |
| 0xF0 / 0x120 | 30..33 / 36..39 | vf5 / vf6 = vf1 p.x + vf2 p.y + vf3 p.z + vf4 (xyzw) | geom.cpp TransformPointsVu0 (fixed: w is computed, not kept) |
| 0x150 | 42..53 | v = vf1 n.x + vf2 n.y + vf3 n.z (xyz), v.w = v . vf4 (the anchor), then RSQRT and v * Q (xyzw) | geom.cpp UpdateGeomWorld (confirmed) |
| 0x1C0 | 56..83 | sly2/vu0.h VuCullSphere's reading | confirmed |

Verification: a scratch harness (scratch v124/vutest, outside the repository) builds the port's VU interpreter
(sly2-port port/vu1/vu1_interp.cpp, read-only; it runs VU0 micro code from a micro memory loaded with the ELF's 84
instruction pairs) and compares each C side, written with the fallbacks' operation order, on 20,000 random cases per
entry (0x1C0: a third forced visible): no mismatch beyond a few ulps of the summed terms (the VU rounds toward zero
and accumulates in ACC). The old a * b product fails every case, so the harness discriminates. The other fallbacks
(macro-mode statements) keep their v1.14..v1.23 harness results.

Conventions: the C follows the VU0 operation order (one rounding per VU operation: `(a.x b.x + a.y b.y) + a.z b.z`,
`v * (1 / d)` rather than `v / d`), never fuses (build with `-ffp-contract=off`), and uses IEEE floats; the VU's
clamping of Inf/NaN to +-FLT_MAX and its truncating rounding are not modelled (the port's float policy decides
those). The VU's `vsqrt` and the FPU's `sqrt.s` take |x| (`VuSqrt`). Lanes the asm leaves as scratch are zero on the C
side; a "+j" operand keeps the lanes the asm does not write (`VuKeepW`). Most rewrites are one guarded statement with
the C on the same operands (tools: a pattern table from asm text to C template), so the code around them is shared
by both branches; register pins (`register qword v __asm__("vf1")`) stay in the shared code.

Testing (a host harness outside the repository): a reference model runs the decomp's asm text itself (operands
bound as gcc binds them, pinned VF registers, dest masks, broadcast fields, ACC and Q) through the port's VU0
library (sly2-port port/vu0/vu0.h, IEEE mode), and each C fallback is compared with it on random inputs (mixed
magnitudes and signs, exact 0 and +-1, tiny lengths at the 1e-4 / 1e-8 thresholds, parallel and opposite vectors):
the mat.cpp functions as compiled natively from this tree with the port's flags (100,000 cases each), sly2/vu0.h's
helpers (200,000 each), and every asm-to-C statement rewrite used in vec, xf, shd, booty and rip (100,000 cases per
form). Tolerance: relative 1e-4 (absolute 1e-5 near zero); the measured maximum error is 0 for every function and
form (same operation order, same IEEE rounding). The harness also flags the port's own sly2/vu0.h override: its
VuAddScaled drops the 1.0 the VU adds to w (vadda with vf0), its VuSqrtX gives 0 for a negative input (the VU takes
the root of |x|), and its VuDivByX gives 0 for a zero divisor (the VU gives the largest float).

Also from the port's boot run (v1.14): xf.cpp's ResolveXfId / ResolveXfIdInSo / ResolveXfIdInParent return `XF *`
(the In* functions returned nothing although SO::GetXfFromIndex and XFM::ResolveXfInParent use the result; the
tail calls already left it in $v0, so returning it is free), index the table as `XF *` (the 0x100 stride is
sizeof(XF)), and PushXfContext / the context stack D_004437A8 hold `XF *`; shd.cpp's LoadShadersFromBrx takes the
GSB as `GSB *` and passes it to UploadPermShaders as such, and the SAA table is sized `count * sizeof(SAA *)`.

## 3. Known remaining hazards

From the v1.12 list (scratch REMAINING.md), minus what v1.13 fixed:

- **g_aeopid**: about 220 member offsets (unique class/offset) stay `EOPID_INT`: they fall inside class pads (the `char pad...[]`
  blocks in sly2/classes.h: JSG, BOMB, TURRET, CID96, CID46, CID121, CID112, CID105, LASEN, BOOTY...; v1.15 declared BOMB / MISSILE / ACCMISS / CID82 / CID37 / CID45 / CID5, v1.17 BOOTY / TURRET / WATER / PIPE (their rows are EOPID_OFF), and
  the DLE links of CID9 / CID40 / CID23) or point
  into TU-private types (CPD in tn.cpp, BOOTYE in booty.cpp, AMBSND in amb.cpp, SFX/SFXHOLDER and EMITTERV in
  emitter.cpp) or at the interior pointers emitter.h's accessors return. Object-reference options (otyp 0x20NN: a pointer to a
  CID NN object, which PostLoadOidField stores): v1.14 typed STEPGUARD::noiseType, STEPPLAYER::jtair.unknown160 and
  GRABSPOT::ploGroup; v1.15 JOB / TASK::stateRequester (`LO *`, through RequestTaskState / ForceTaskState and
  StateSourceScope) and NEELA's option 971 (giveUpBinding.aseg, the pointer half of ANIMBINDING's union).
  v1.16 typed CID98's options 929 / 930 / 935 and CID112's 503..550 (EOPID_OFF). The OwnFields accessors use
  SLY2_GRABSPOT_BASE since v1.16.
- **Literal sizes**: SW_ctor heaps without a type yet (Refnode 0xC, Xplist 0x14 (so.h's XPLIST is 0x10), Xp 0xC0,
  Jsg 0xF0, Xfh 0x30, Leap 0x40; v1.17 typed Event / Callback / Diract / Blip / the blip splines: the blip slot is
  sizeof(BLIPBATCH) rounded to its 0x40 alignment, an inference) and the list offsets of
  the SL offsets (v1.16 made every SW_ctor DL offset SLY2_OFFSETOF: PIPE / RAIL / BARREL::dleGrab, TARGETREC::dleTargetrec,
  CID47::dleListener (was LISTENERKEY key), CID98::dleBreakable, CID14::dleMerged / dlePending), slotheap.h
  engine/Splice pools, the post-load pool, PATHZONE dlRegent, DIALOGEVENT, BOOTYE 0x2C, aseg.cpp's kind-1 box origin
  (0x20), save.cpp's SVENT (cent * 10), the heap-guard sites outside the named functions (sly2+0x523381 / 0x542fc1 /
  0x5b92c5 in the port's log). The SNIP tables (BindLoOidTable) are done (murray's row at 0x1854 is MURRAY::arcLeap.paseg, ASEGARC in
  stepplayer.h since v1.16; gait.cpp reads the arc as ASEGARC since v1.18).
- **From the port's boot-3 list, still open after v1.18**: frm.cpp's group list offset 0x454 (BLIPG's fields are still
  blip.cpp's BLIPGV: classes.h declares BLIPG's bytes as the 32 empty-constructor BLIP records that give its class
  constructor's loop, so declaring the real fields needs that loop reproduced another way); path.cpp's CG views
  (FindNavTriangleQuick / Safe `struct T`, the obstacle-array `S : XFM` views, IsNearPathlinkEndpoint's +0x218 object);
  (v1.20: the obstacle `S : XFM` in FindPathzoneObstacles is PATHZONE); bis.cpp's FACE (the same record as xfm.h SURF / geom.cpp PLANE, but bis.cpp cannot include sly2/xfm.h: bis.h's vec.h
  VECTOR is 16-byte aligned, basic.h's 12 bytes; unifying needs bis.cpp off vec.h or SURF off VECTOR); rail / pipe /
  freegrabspot / hnd flat views; blend.cpp's ASEG / CHN views; shape.cpp's chain steppers' CHAIN over SHAPECHAIN (native
  layout already; the hand-tuned steppers use a signed local qword, so binding them to SHAPECHAIN's qword members would
  add reference temporaries); launcher.cpp's SOV (its own vtable declarations with signed-qword slot signatures); the
  TU views of the v1.17 classes (booty.cpp BOOTYV / CID37V with their HERO / MATCH companions and hand vtable calls,
  turret.cpp TURRETV, lootpouch.cpp SPRAYER). PATHLINK names its zones `CID17 *` (basic.h's forward name) while the
  class is PATHZONE; path.cpp bridges with PathzoneOf. Done in v1.18: water.cpp's flat SO view (the 64-bit unit at +0x148 is
  SO's flag unit: SO::soFlags, bits 31 / 32 SO::wasInWater / isInWater), path.cpp's REGTREE / B / path-link endpoint
  views, geom.cpp's PLANE words, gait.cpp's BLCURVE / LEAPTUNE.
- **From the port's boot-4 list (per-frame path)**: done in v1.18: solve.cpp (SOV / SOTREEV / TL / XP laid out as SO /
  SOTREE / XPLIST / XP on every target, asserted against the headers; ApplySoRestingContactVelocity's asm offsets from
  operands; XFV and SOPHYSV already had native layout). bbmark.cpp uses SO / SOTREE since v1.15. Done in v1.19: the hand
  vtable calls outside asm (section 1; blend.cpp's XB slot 9 is LO::Remove), geom.cpp UpdateGeomWorld's C fallback,
  the `int`-typed pointers (sm.cpp ReplaceSma / ReplaceSmaEx and ApplySmToObject / ApplySmWithKind's object; the
  class_registry thunks' object arguments of script types 0x2030 / 0x2002; BASIC / LO::HandleSpliceEvent's result as
  `CRef *` and HandleBasicSpliceEvent's argument vector as `void **`; bif.cpp's six BIFK forwarders, BuiltinSetCadr and
  BuiltinObjectOption as `(int cref, CRef *args, CFrame *frame)`; find-class-objects' context `LO *`; the find-object
  family's optional root as the head of a CPair list; SO::AddCustomXps(SOPART *, int cbsp, BSP *absp, BSP *pbsp,
  XP **) on every overrider; OVRA::animatedField, the `float *` LO::GetAnimatedField returned), EnsureTargetRecordById
  returning `TARGETREC *`, SetSoIdProxy passing fChildren and NewSoPartHandle passing the oid explicitly, MISSILE::Fire
  / BLIPG::SubscribeObject / CID35::OnTriggered with their arguments, and of request 4's structs: so.cpp's target
  records (TARGETREC; FindTargetRecordById by target object; allocation offsetof(guards) + 32 pointers) and CID149 /
  CID150 handles (so.h classes, sizeof; SW_ctor's slot sizeof(CID149)), relay.cpp NewRelayJsg on RELAYJSG (SW_ctor's
  Jsg slot sizeof(RELAYJSG)), EnsureSoPhysRegionRecord's sizeof(REGENT).
  Done in v1.20 (tools/cleanup/views; every rewrite byte-identical): air.cpp PresetJtAccelJump's CONTEXT / ACTOR /
  BODY / SMP (JTAIR, STEPPLAYER, XF, SMP), ProjectJtVelocityOnContacts' and FCheckJtStuck's contact, contact-list,
  tree, player and jump views (XP, XPLIST, SOTREE, STEPPLAYER, JTAIR), UpdateJtActiveJump's transform, stick and
  opaque frame / clip views (XF, POCTL, ASEG), the jump update's link records (SOPART, GRABSPOT, ACT8, XF), the landing
  chooser's XF, the file-scope XP / XPD and GRABSPOTV; step.cpp PresetStepAccel's S / O / M3 (STEP, XF, MATRIX3Q) and three
  smaller views; path.cpp FindPathzoneObstacles' `S : XFM` (PATHZONE's CG); mark.cpp's namespace EDGE / XP; trail.cpp's
  record and output views (SUBGLOB / SUBGLOBX: the record index arithmetic now uses their sizeof); glob.cpp's XF / U / M4; jt.cpp
  SMP / LM; hud.cpp CLOCKIsland; act.cpp's local VECTORs. XP / XPS moved from mark.cpp to sly2/xp.h (with normal's
  VECTOR4 alias normalVec); solve.cpp, dzg.cpp, sensor.cpp, jt.cpp and step.cpp still define their own XP (the first
  three at file scope, so xp.h is not in so.h yet). Request 6: EvaluateKeyedFloatCurve's key is
  `(ACKEYF *)(i * (int)sizeof(ACKEYF) + (sptr)t->akey)` (`&t->akey[i]` changes the code), ProjectJtVelocityOnContacts'
  cursor and row address are `uptr`, guardscript.cpp's StartGuardScript (LO *, CID113 *) and GetBhvScriptTargetPose /
  GetGuardScriptTargetPose (LO *target, qword *posOut, float *headingOut).
  Done in v1.21 (every change byte-identical): glob.cpp RenderSoGlobset and BuildModelDrawPackets on so.h's GLOBSET,
  GLOB, SUBGLOB, glob.h's SUBGLOBX, BND, wr.h's WRBG and the owner SO (the model cull's range at the owner's +0x38
  is LO::busyRadius; GLOBSET::boundsCenterq is the centre as one quadword; the skin's warp matrices are its WR's
  warpMatrices / uvMatrices, the first still by byte offset, now SLY2_OFFSETOF(WR, warpMatrices)); 432 of the 452
  per-namespace signed `typedef int qword` removed (basic.h's unsigned qword applies; the 20 left change the code:
  memory, geom, path, blend, mat, vec, so (4), shape (4), rip (2), act (2), bez, bis); SENSOR / LASEN members (with
  LASENCAP), LEAP's fields and SOTREE::grf declared; dzg.cpp, sensor.cpp and solve.cpp read contacts as xp.h's XP
  (XPS::psoRoot is the side's object at +0x08; xp.h aliases sBeam and gLimitMin / gLimitMax over the feature
  pointers); sly2/frm.h FRM for frm.cpp, draw.cpp (FRM::drawChain) and phasemem.cpp (the buffers and the DL
  members); hw.h SLY2_CACHED(a) (hud.cpp's six render-list call folds and its packet bounds) and SLY2_VU_ADDR(pv)
  (hud.cpp Mscal, VIFS::AddVifMscal); spawner.cpp / detector.cpp take IntersectSwBoundingBox's result as
  `int objectCount, SOPART **objects`; sly2/lsg.h's LSG for bsp.cpp and clip.cpp (clip.cpp's copy had ints where
  the surface / edge / link pointers are, so natively MergeSegmentLists read ClipSegmentBsp's spans at the wrong
  offsets: the port's SnapToGround miss); the view tool's probe now inventories files with #line pins (hud,
  robottank, grab, rip, stepguard, stepplayer, critter, so, solve) and a second tool run rewrote 94 more views in
  39 files. air.cpp's CONTEXT `tracks[i]`: the header is right (JTAIRBL is {weight, animation, word}; abl[0].weight
  is the scripted clip's gain); the view's 12-byte window starts 8 bytes into abl[0], so each track's weight is
  abl[i + 1].weight, which the tool cannot map (an index shift); a hand edit.
  Still open after v1.21: xp.h is not included from so.h yet (gadget, robottank, bentley, hurt, jt, jtgoal and
  places.cpp define their own file-scope contact views); step.cpp AddStepAxisXps' local SO; cm.cpp SetCmVisZone's
  `+0xE0` (CID5::grfvis; classes.h still renumbers s_rgba: CID5 needs a narrower header, CID5 : XFM); POSE / RENDER;
  prog.cpp's own FRM view; the header pads in GRABSPOT users (HND, PIPE, RAIL), CID96 (critter.cpp), JSG
  (robottank.cpp) and JOY (grab.cpp) that the fuller inventory now reports.
  Done in v1.22 (every change byte-identical): signatures (tools/cleanup/signatures/sigcheck.py, worst-class
  declarations 1468 -> 1298, cross-declaration conflicts 360 -> 276; int-vs-pointer 195 -> 85, wrong-count 122 -> 26, struct-return 25 -> 9 plus 10
  sret-adapted): operator*(MATRIX4, MATRIX4) is the C++ function (NRV body), its 26 hand-passed-pointer callers and
  BuildRotateVectorsMatrix's 7 struct-result callers bind through SLY2_SRET_LABEL, basic.cpp's EvalExpression too;
  bif.cpp's builtins all take (int cref, CRef *args, CFrame *frame) and the 98 C-style ones have native CRef entries
  (SLY2_BUILTIN_C); FindSwObject / FindSwObjectByClass's context LO * (12 declarations), EnsureAsegBlendDynamic's
  int * outputs, CreateLo's parent SO *, SmoothValue's float *, ClampToLimits / IsWithinLimits' LM *, the option
  forwarders (BASIC *, int). From the port's request 8: SLY2_BSP_NO_LIMIT (25 call sites, limit parameters uptr);
  VU0 microprogram 0x1C0 in C (section 2); sly2/lsg.h's offsets from lsgk on (+0x48, apsurf +0x4C), and the LSG views
  of mark, shape, water, guardbhv, bentley, grab, jt and gadget replaced by it (views pass); the file-scope XP copies
  of gadget, hurt, jtgoal and places replaced by xp.h; gait.cpp ConstructLeap no longer clears LEAPV's +0 / +4 words
  after storing the vptr (natively they were the vptr: STEPGUARD's embedded LEAP had a null vtable in
  StartAnimBinding).
  Still open after v1.22: mangled names in the config that do not describe the Sly 2 function (sigcheck flags them
  whatever the declarations say): BuildRotateVectorsMatrix (qword arguments, out-pointer first, as in Sly 1's mangled name), InitZpr (callers pass two sizes), CalculateEmitdvMatrix; brx.cpp's five-argument
  FindSwObject form and sw.cpp's two three-argument forms (the caller leaves a3 as it is); the hidden-pointer
  forms that are consistent with their definition (BuiltinObjectOption / BuiltinSetCadr, CompareNumericArgs,
  ApplyUnaryFloatOp, EnsureBasicOption); the wrong-type views of STACKSCOPE, CRefPtr<CPair>::Attach,
  DecomposeRotateMatrixRad and NormalizeRotateMatrix3 (local record types with the same ABI); the SDK's
  snd_SendIOPCommand* (not built here). Port request 8 left: DriveStepguardFromAnim / UpdateStepPhys C versions,
  RenderStepguardSelf's offsets, prize CHANNEL (targets.json says XF; the stamp word and the +0x3C store are not
  mapped), path planning views, emitter EXPLS / EMITP, AddXp / FindShapeClosestPoint / bhvstk int pointers, button /
  rail / pipe, blip.cpp and the remaining glob / rendertree views, StartLeap's memset(0x70). Views: air.cpp CONTEXT
  `tracks[i]` (the two CONTEXT views disagree on TRACK's members; needs a hand rewrite of the whole view), step.cpp
  AddStepAxisXps' local SO, the XP copies in mark, solve, rip, step, bentley, stepplayer, robottank, sensor, grab, jt
  (so xp.h is not in so.h yet), POSE / RENDER, cm.cpp SetCmVisZone's +0xE0.
  Done in v1.23 (every change byte-identical; cm.o RENAMED only, see below): port request 8: UpdateStepPhys and
  DriveStepguardFromAnim C versions (section 2; leap.h declares CID146::velocity at +0x10), RenderStepguardSelf on
  STEPGUARD (LO::visibilityMask, SO::rootTransform / model / opacity / psobounds, STEP::vecPivotOffset,
  STEPGUARD::pulseStartTime) and &CM::tunnelPolicy, prize.cpp ProjectDprizeTransform's CHANNEL view replaced by XF (the
  stamp word is XF::worldStamp, the +0x3C store the local position's w, the velocity XF::velq), gait.cpp StartLeap
  clears sizeof(LEAP); BLIPG declares its members in classes.h (the constructor's 32-step loop now comes from the age
  table's BLIPGAGE element constructor) and blip.cpp's BLIPGV, emitter.cpp's two BLIPG views and frm.cpp's 0x454 are gone
  (the port can enable blip groups); every 'frame' allowlist statement but blip.cpp UpdateMs10 has a C fallback
  (section 2); so.h includes sly2/xp.h (robottank, bentley and jt.cpp's XP copies removed; step.cpp AdjustStepNewXp on
  STEP / XP); CID5 moved to sly2/vis.h so cm.cpp SetCmVisZone reads CID5::grfvis (including it renumbers two
  compiler-local names in cm.o, s_rgba.81 -> .111 and _$tmp_0.82 -> .112, not in the config: bytes and link unchanged);
  brx_sw.cpp's LoadSwObjectsFromBrx / CloneLoWithParent parent as SO *; STEP::targetAngularVelocity (was unknown1E0).
  From the port's request 9 (boot-9), also in v1.23: GLOBSET::boundsCenterq is EE-only (a qword in the union
  16-aligned GLOBSET natively and moved every SO field; glob.cpp's native branch copies boundsCenter into a local
  quadword); xp.h's kind-14 limits overlay one feature pointer each (gLimitMin with pvFeature0, gLimitMax with
  pvFeature1: natively two adjacent floats both sat in pvFeature0); sw.cpp's aloupd is 0x1000 * sizeof(LOUPD); hud.cpp
  passes uptr to SLY2_CACHED. Request 9 still open: vec.cpp's qwordi prototypes; the EE-layout views in step.cpp
  (AdjustStepXpVelocity, HasStepFloorNearby, MatchStepXf), air.cpp SetJtAirState (CONTEXT / BODY / MOTION, TRACK
  stride, the mode-21 vtable word), relay.cpp SGG, grabspot.cpp T / Q, screen.cpp's notice views, cm.cpp SquishCmEye /
  ClipCameraEye, prize.cpp's collision and bounce views, path.cpp FindClosestPointInCg (CG, the 12-byte LEAF / TRI
  records: sizeof; FindNavTriangleSafe's missing flags argument), emitter.cpp MESHO and WakeSoWater, guardbhv's
  SOTREE view and the path following after StepBhvpathAlongShape, the 0x60-byte LSG views (prize, screen, guardbhv,
  robottank, grab, hack, jt, bentley, air, path); STIMULUS vs CID147 (one record, two layouts) and
  ChooseStepguardStimulus' `&g_sw.aslPxrOid[410]`; the int-typed pointer parameters (CanBhvcombatShoot,
  FindShapeClosestPointFromDistance in tn / guardbhv).
  Still open after v1.23: port request 8's path planning views (FindPathzonePathCurve, GRAPH / G as CG), emitter.cpp
  EXPLS / EMITP, AddXp / FindShapeClosestPoint / bhvstk int pointers, button / rail / pipe, the remaining glob /
  rendertree views; blip.cpp UpdateMs10 (microprogram 0x10); air.cpp CONTEXT `tracks[i]`; step.cpp AddStepAxisXps'
  local SO; POSE / RENDER; namespace-local XP views that are not the contact record (rip.cpp, stepplayer.cpp);
  sigcheck's struct-return forms (bif.cpp BuiltinObjectOption / BuiltinSetCadr, AreRefsStructurallyEqual,
  basic.cpp EnsureBasicOption: consistent with their definitions, which take the result pointer explicitly) and the
  remaining int-vs-pointer declarations (85 -> 83; mismatching declarations 1298 -> 1295, cross-declaration conflicts
  276 -> 274). Views inventory (tools/cleanup/views): candidates 1039 -> 999 (frame path 172 -> 164), TU structs 2560 -> 2518.
  Done in v1.24 (every change byte-identical; class_registry.cpp differs only in renamed references): **the VU0
  microprogram fallbacks from the microcode** (section 2: operator*(MATRIX4, MATRIX4) is b * a, UnpackSubglobBones
  inverse bind * world, TransformPointsVu0 computes w, UpdateMs10 has a C body; 0x150 and 0x1C0 confirmed on the port's
  VU interpreter). Port request 10 / the boot-10 run: StepBhvpathAlongShape on BHVPATH / STEP / SOPHYS / REGENT /
  PATHZONE / SHAPE (the other bodies through PATHZONE::dlRegent and REGENT::dlePathzone; REGENT's floats are
  heightOffset / radius / mass, their option accessors Get/SetSoRegionHeightOffset / Radius / Mass); BHVPATH::blocker is
  the CID14 obstacle (UpdateBhvpath's T view gone; the flag word is LO::isInWorld, +0x1D8 CID14::owner); STEP::routeShape
  (SHAPE *, was void *unknown294); SLY2_FDIV (section 1) in FindClosestPointOnLineSegment; emitter.cpp EmitBlips on
  BLIPG / BLIPBATCH / BLIPBUF / BLIPBEZ and the EMITV default records by member natively (the EE word images put 25.0f in
  pblipclrs: the port's SetBlipgEmitb symptom); safehouse StepCharSelect compares the slots' oid words;
  ShouldStepplayerIgnoreIntersection on (STEPPLAYER *, SO *) returning the flags; TriggerRsmg / TriggerOneRsmg on SW /
  RSMG / LO *; SetSoGrfBit8's fallback without an int * to the tagged word; hud.cpp BuildSubtitleText /
  BuildSubtitleButtonText (char *) and RenderSpritePayload (pointer matrix / params). Port request 8 / 9: path.cpp's
  navigation graph typed (classes.h CGV / CGE / CGT / CBSP; CGT is the 12-byte EE triangle: three vertex pointers) and
  every walker on CG (FindNavTriangleQuick / Safe, ClipEdgeToCbsp, IsEdgeBlockedByCbsp, FindPathAStar,
  GetClosestPointOnNavTriangle, FindPathPoints, LoadPathzoneFromBrx, InitDefaultPathzoneCg, ComputeCgEdgeCosts);
  FindPathzonePathCurve (PATHZONE *, qword, qword, SHAPE *); FindClosestPointInCg / FindPathzoneClosestPoint take the
  flags their callers pass and hand them to FindNavTriangleSafe; FindPathzoneClosestFreePoint on PATHZONE / CID14 / LSG
  (its S : XFM view served both, and its EE layout assert is gone); air.cpp SetJtAirState on JTAIR / JT / BARREL / XF /
  JTPLACE (the CONTEXT view's rGravitySaved read gave Sly a mass of ~4e-26 natively; tracks[i] is abl[i + 1].weight)
  with C fallbacks for its mode-17 and mode-21 asm; STIMULUS / STIMTEMPLATE merged into CID147 / CID117 (source,
  subject, level, next / previous; CID117::kind 0 audible / 1 visible; noise.cpp's emitters take LO *source; SW::apstimk
  is CID117 *; ChooseStepguardStimulus walks SW::dlStimulus); vec.cpp's C prototypes on basic.h's qword; AddXp's feature
  words as void * in every declaration (mark.cpp's MarkSoContactsBspBsp keeps its VV vectors on the EE, qword natively);
  FindShapeClosestPoint / FindShapeClosestPointFromDistance's pointer parameters (17 declarations; GetShapeClosestPoint
  takes a qword); HandleBhvstkMessage / HandleBhvMessage's void * payload; button.cpp's sizeof(LO *) arrays; rail.cpp
  and pipe.cpp on RAIL / PIPE and STEPPLAYER (RAILV / PIPEV / T / P gone; GetPipeGrabPosition has a C fallback, no
  literal 0x2C0D04 natively); IntersectSwBoundingBox's object array as SO ** in guardbhv CanBhvcombatShoot, stepguard,
  jt, air, bentley, grab; EXPLS declares sfx / countRange / keepsGroups and FireExplsExplso uses it (EMITP is LO *[5]);
  grabspot.cpp's wall / pole callbacks and FilterAttackLineOfSightHit's hit object on STEPPLAYER / SO; relay.cpp's SGG
  is RELAY under the mangled name; step.cpp AddStepAxisXps on STEP / SOTREE / XPLIST; the views tool kept 16 more
  rewrites (bif, hud, guardbhv, stepguard, jt). Signatures: mismatching declarations 1295 -> 1293, int-vs-pointer
  83 -> 82, cross-declaration conflicts 274 -> 273 (most of this round's pointer fixes are C-named declarations, which
  sigcheck does not cover). Views inventory: candidates 992 -> 936 (frame path 164 -> 141), TU structs 2512 -> 2430.
  Still open after v1.24: hud.h SPRITEDESC vs LoadSpriteDescriptor's record and its six views; cm.cpp SquishCmEye /
  ClipCameraEye (the object array as int; its local SO views clash with so.h, so SOPART cannot be included yet:
  SOPART::bspc.unknown08 is the root node index); tn.cpp's int IntersectSwBoundingBox context; prize.cpp's collision /
  bounce views (the candidate record has a 64-bit flag word at +0x10 where so.h's SOPART declares plvo: needs evidence);
  guardbhv's candidate record Y; path.cpp's ROW (an LSG with the triangle in apsurf[0]; native-consistent, not
  unified); screen.cpp notice views; the remaining glob / rendertree views; POSE / RENDER; the struct-return forms (bif.cpp
  BuiltinObjectOption / BuiltinSetCadr, AreRefsStructurallyEqual, basic.cpp EnsureBasicOption: their definitions and
  every caller pass the result pointer explicitly, so a native build is consistent; left); guardscript.cpp's
  EnsureSggAlarm declaration (int alarm) and the generated splicewrap thunks' s32 arguments.
  Still open after v1.20 (the tool's inventory, build/views/views.csv, has the per-view reason):
  - air.cpp UpdateJtActiveJump / the jump update: CONTEXT (JTAIR; `tracks[i]` is `abl[i + 1]`, a 12-byte TRACK over
    JTAIRBL), ACTOR (STEPPLAYER; the namespace's signed `typedef int qword` against basic.h's unsigned qword breaks
    the VuSub / VuDivByX calls), SOURCE / OBJECT (JTAIR::jumpObject / unknown160 are `LO *`; BARREL / CID40 per the
    port, members not covered by those headers), FOCUS (TN::cameraDefinitions is a CPD, the view's DATA reads +4), the
    PATH / SPLINE samplers (secondary-base vptr); PresetJtAccelJump's default case and modes 20 / 22 are asm without
    a C fallback; hud.cpp FinishSpriteSoundEvent's `lw %0,8(%1)` without a C fallback.
  - step.cpp AdjustStepXpVelocity / AdjustStepNewXp (signed qword typedef; their local XP shadows xp.h),
    AddStepAxisXps (a local SO re-declaration in the block), UpdateStep / AddStepCustomXps (no identified class).
  - stepplayer.cpp, cm.cpp, blend.cpp, hurt.cpp, busy.cpp, ms.cpp, sw.cpp, proxy.cpp, shape.cpp: no view with a
    confident target yet (offset fit is ambiguous); they need hand annotations in tools/cleanup/views/targets.json.
  - hand vtable calls inside hand-scheduled asm statements (they need the statement's C fallback first): air.cpp
    (slot 35), guardbhv.cpp BlendSet (35), murray.cpp SetMurrayFlopSubstate (35), jthands.cpp (35),
    stepplayer.cpp's message 0x160 case (35), throw.cpp LaunchVectorCall (35), step.cpp's other-object follow (29),
    missile.cpp FireSplmiss (33, twice); booty.cpp's LINKE / LINKE3 slot 1 on a class not identified (args pos, 0,
    1.0f); murray.cpp's Q / E / X contact views, robottank.cpp P / OWNER, grab.cpp ACTOR, bomb.cpp POV still carry
    a vtable word (unused by the converted calls).
  - RefThunk_EmitSwNoise's third argument: script type 0x2002 (an object) where noise.cpp stores an int "loudness"
    (CID147 +0x40, NewNoise / CreateNoise / ReplaceHeldNoise); likely the noise's source object, unconfirmed (left int).
    The generated splicewrap RefThunk_NewSoPartHandle.inc passes the object through `s32`.
  - request 4's structs not done: blend.cpp's ASEG / XB / LINK / CHNE / CHN / KEY views (want ASEGBL / ASEGA / ACTSEG /
    XF / EA; the curve calls are done), its `n * 12` / `cTotal * 8` / `frameEventCount << 4` sizes; callback.cpp EVCB;
    proxy.cpp AcquireProxySpare's flat views; trail.cpp / glob.cpp ResetGlobAttachment; shape.cpp SetChainEndObject and
    the int-typed shape arguments; path.cpp FindNavTriangleQuick / Safe `struct T` (CG) and the obstacle `S : XFM`;
    bis.cpp CBinaryInputStream_ReadBspc OWNER / ITEM; cm.cpp CID5 views and AdjustCmCpdefi's +0x4F4; busy.cpp,
    hud.cpp ApplySpriteKeyPoses, ms.cpp GLOBOWNER / GLOBREF, sw.cpp intersect / filter views and dlStimulus,
    stepguard.cpp PMOVER / ANIMBINDING / DIRANIM, hurt.cpp UpdateJtActiveZap, step.cpp UpdateStep / AdjustStepXps /
    AddStepCustomXps views, stepplayer.cpp component offsets, jt.cpp SmoothJtCameraCurveTarget, LIGHT's fields
    (NewLight's overrun), PostLoadStepplayer alorefSplash.
- **SHD header read**: LoadShdFromBrx reads the first 0x18 bytes of the level's shader record straight into SHD
  (file-record pattern, not under SLY2_EE_LAYOUT yet).
- **Dangerous views** (after v1.16). Done in v1.16: shape.cpp's ten SHAPE views (plus SetChainEndObject, NewSampledShape
  and the SHAPERANGES views), cnvo.cpp CID14V, breakable.cpp's TRIG and its contact, zap-record and explode-message views, xf.cpp's O / S / P / B and
  the ACT slot-5 call, ik.cpp A / B / FRAME / XF_REF, launcher.cpp S / LAUNCH, prize.cpp AddLootEntryObject's two views, jt.cpp
  PostJtLoad's flat stores, grabspot.cpp's InitGrabspot(&x->ptn) and GRFPLAYER (v1.18: water.cpp's flat SO view, solve.cpp). Still open: rip.cpp
  `M_ProjectRipTransform::S` (a RIP view: different pointer slots before +0x40, local M3/PHYS types over RIPM3 and
  the motion record, asm with literal offsets); shape.cpp's CID 106 rope-mesh views (RenderSelf and the sampler before it: S with +0xDC /
  +0x150, CONFIG / STATE / MESH; not SHAPE) and its two chain steppers' CHAIN views (native-correct layout, local
  names); rail.cpp RAILV / pipe.cpp PIPEV (flat views of RAIL / PIPE; PIPE declares the +0x1C0 axis since v1.17); launcher.cpp's SOV (an SO view with its own
  vtable declarations); carmelita.cpp / bhvsearch.cpp's list walkers (NODE views with next174 / next1B4); so.cpp
  FindTargetRecordById's T. Other flat views with void * pads (BLIPGV, BOOTYV, TRAIL, SOV, LASENV) only read LO's
  tagged word through LO; their other fields are still at EE offsets.
- **glob.cpp**: address arithmetic is `uptr` since v1.15; DMA chains keep 32-bit EE addresses: CopyRelocateDmaChain adds an `int` delta to REF tags' address
  words, and the shader packet tags pack `(ulong)(int)pshd->pbPacket << 32`. A native build needs an address model
  for chains (offsets from a heap base, or an emulated 4 GB EE address space) before its DMA/VIF interpreter can
  follow them; not a decomp-side change.
- **Inline asm without a C fallback**: a rough count (a scratch scanner: asm statements outside `#ifdef SLY2_EE_ASM`,
  register-pin declarations excluded) finds about 5,000 statements in 135 src/game files; the per-frame heavy ones are
  hud.cpp (about 330), jt.cpp, air.cpp, guardbhv.cpp, robottank.cpp, rip.cpp, shape.cpp, throw.cpp, murray.cpp,
  grab.cpp, so.cpp, step.cpp (100 to 240 each), then stepplayer, glob, prize, stepbhv, hack, hurt, bentley, cm, bif,
  stepguard, cplcy, emitter, relay, solve, gadget, sticky, path (55 to 100); bsp.cpp InitPlaneFromNormalPos (used by
  UpdateGeomWorld's fallback and CloneGeom) is one statement. Named before: shd.cpp BuildShaderGifPackets (EE-core bit manipulation with pinned
  registers), rip.cpp ProjectRipTransform and the other RIP integrators (hand-scheduled fused asm: dozens of
  multi-instruction statements mixing VU0, FPU and loads), the `vcallms` sites (xf.cpp, geom.cpp, glob.cpp, blip.cpp,
  rendertree.cpp: VU0 microprograms, ported per entry), gs.cpp sync/ei, the cane/murray/sticky/ik loaders and the
  player/guard files (air, guardbhv, shape, jt...); register pins (clang rejects about 400).
- **int to pointer**: about 23 (mpeg `pm->n8`, cane.cpp `(char *)0x002e0000`); breakable.cpp
  AddBreakableTriggerClass returns its record pointer as `int`.

## 4. Future clean port

Guards stay in the simplest form: `#if defined(SLY2_EE_LAYOUT)` / `#ifdef SLY2_EE_ASM`, `#else`, `#endif`, one macro per condition,
never nested inside another portability guard and never compound (`&&`, `||`). That keeps them mechanical: a port
can produce a flag-free native source tree with unifdef,

```sh
unifdef -USLY2_EE_ASM -USLY2_EE_LAYOUT -m $(git ls-files 'src/*.cpp' 'src/*.c' 'include/*.h')
```

and then simplify freely (drop the EE paths, the asm labels and the layout asserts). The definitions in sly2/port.h
are the only compound conditions (`__R5900__` and not the opt-out macro), and they are what `-U` replaces. A new
guard follows the same pattern: one macro in sly2/port.h, defined on the R5900 compiler, with an opt-out.
