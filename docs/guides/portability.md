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
offsets, and every EnsureAsegBlendDynamic entry stride from sizeof (section 3 lists what is left).

## 1. Guards and hooks

| hook | EE (matching build) | native build | where |
|---|---|---|---|
| `SLY2_EE_ASM` (sly2/port.h) | defined (`__R5900__`, unless `SLY2_NO_EE_ASM`): inline EE assembly allowed | undefined: the `#else` plain-C body is compiled | `grep -rln SLY2_EE_ASM src include` (sly2/vu0.h, mat.cpp, vec.cpp, xf.cpp, shd.cpp, booty.cpp, rip.cpp, sw.cpp, path.cpp, gs.cpp); section 2 |
| `SLY2_EE_LAYOUT` (sly2/port.h) | defined (`__R5900__`, unless `SLY2_NO_EE_LAYOUT`): EE data layout (4-byte pointers, file records read straight into in-memory structs) and g++ 2.95 vtables | undefined: `#else` reads file records field by field, uses plain virtual calls | shd.cpp LoadClutFromBrx / LoadBmpFromBrx, gait.cpp StartLeap's slot-35 call, cnvo.cpp MergeCid14Obstacles' slots 31 / 29 / 9 (`SLY2_VTE`) |
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
  caller's g is still in $f12, so `GetGoal(..., g)` is the same code); gait.cpp's slot 35 (SO::SetVelocity) and
  cnvo.cpp MergeCid14Obstacles' slots 31 / 29 / 9 did not and are guarded.
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
  `__ml__FRC7MATRIX4T0` runs the VU0 microprogram at 0x60 on the EE (vcallms); its C body is the row-vector product
  out[i] = sum_k a[i][k] b[k], which is what its callers (CalculateDmat4) need; the microprogram itself is not
  modelled.
- **vec.cpp**: every asm statement (all 45 functions are covered).
- **xf.cpp**: BuildXfValueMatrix, BuildXfValueDerivativeMatrix and every VU0 statement except the `vcallms 0x60`
  sites. **shd.cpp**: ColorCorrectVector, AddBitmapToHash / AddClutToHash (whole C functions over D_003DD010 /
  D_003DD018). **booty.cpp**: every asm statement (SetBootyState and the distance helpers). **rip.cpp**: LaunchMatch.
  **geom.cpp**: TransformPointsVu0, whose EE body runs the VU0 microprograms at 0x120 / 0xF0 (vcallms); its C body
  transforms each point the way CloneGeom's `Transform` does (xyz = p M with p.w taken as 1, w kept). That reading of
  the microprograms is an assumption (the same points go through `Transform` in CloneGeom), not checked against the
  microcode.

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
  blocks in sly2/classes.h: JSG, BOMB, TURRET, CID96, CID46, CID121, CID112, CID105, LASEN, BOOTY...; v1.15 declared BOMB / MISSILE / ACCMISS / CID82 / CID37 / CID45 / CID5 and
  the DLE links of CID9 / CID40 / CID23) or point
  into TU-private types (CPD in tn.cpp, BOOTYE in booty.cpp, AMBSND in amb.cpp, SFX/SFXHOLDER and EMITTERV in
  emitter.cpp) or at the interior pointers emitter.h's accessors return. Object-reference options (otyp 0x20NN: a pointer to a
  CID NN object, which PostLoadOidField stores): v1.14 typed STEPGUARD::noiseType, STEPPLAYER::jtair.unknown160 and
  GRABSPOT::ploGroup; v1.15 JOB / TASK::stateRequester (`LO *`, through RequestTaskState / ForceTaskState and
  StateSourceScope) and NEELA's option 971 (giveUpBinding.aseg, the pointer half of ANIMBINDING's union).
  v1.16 typed CID98's options 929 / 930 / 935 and CID112's 503..550 (EOPID_OFF). The OwnFields accessors use
  SLY2_GRABSPOT_BASE since v1.16.
- **Literal sizes**: SW_ctor heaps without a type yet (Event/Callback 0x14, Refnode 0xC, Xplist 0x14 (so.h's XPLIST is
  0x10), Xp 0xC0, Jsg 0xF0, Xfh 0x30, Leap 0x40, Diract 0xD0, Blip 0x10C0, the blip splines) and the list offsets of
  the SL offsets (v1.16 made every SW_ctor DL offset SLY2_OFFSETOF: PIPE / RAIL / BARREL::dleGrab, TARGETREC::dleTargetrec,
  CID47::dleListener (was LISTENERKEY key), CID98::dleBreakable, CID14::dleMerged / dlePending), slotheap.h
  engine/Splice pools, the post-load pool, PATHZONE dlRegent, DIALOGEVENT, BOOTYE 0x2C, aseg.cpp's kind-1 box origin
  (0x20), save.cpp's SVENT (cent * 10), the heap-guard sites outside the named functions (sly2+0x523381 / 0x542fc1 /
  0x5b92c5 in the port's log). The SNIP tables (BindLoOidTable) are done (murray's row at 0x1854 is MURRAY::arcLeap.paseg, ASEGARC in
  stepplayer.h since v1.16; gait.cpp still reads the arc through its BLCURVE / LEAPTUNE views).
- **From the port's boot-3 list, still open after v1.16**: SW_ctor's Event / Callback (0x14) and Diract (0xD0) slot
  sizes (the records are TU-private: basic.cpp SWCB, callback.cpp CALLBACK, gait.cpp's blend; a header type each
  is needed first); screen.cpp InitScreenList's 0x14 (a DLE there would overlap SCREEN::idText at +0x18: the list's
  element type needs checking); frm.cpp's 0x454 / 0x1090 (blip.cpp's private BLIPG / BLIP records); the heap-guard
  `new` overruns (BOOTY, CID37, CID43, CID99, SAFEHOUSE, TURRET pads; CID125::PostLoad's 0x24); water.cpp's views
  and slots 51 / 71 / 72 with WATER's fields in classes.h; PIPE's +0x1C0 axis; path.cpp's FindPathzoneAtPos B view,
  the REGTREE pad views and the path-link endpoint views; SURF (xfm.h) vs FACE (bis.cpp); geom.cpp PLANE / GEOM and
  CloneGeom's numSurfaces * 32; rail / pipe / freegrabspot / hnd flat views; blend.cpp's ASEG / CHN views.
- **SHD header read**: LoadShdFromBrx reads the first 0x18 bytes of the level's shader record straight into SHD
  (file-record pattern, not under SLY2_EE_LAYOUT yet).
- **Dangerous views** (after v1.16). Done in v1.16: shape.cpp's ten SHAPE views (plus SetChainEndObject, NewSampledShape
  and the SHAPERANGES views), cnvo.cpp CID14V, breakable.cpp's TRIG and its contact, zap-record and explode-message views, xf.cpp's O / S / P / B and
  the ACT slot-5 call, ik.cpp A / B / FRAME / XF_REF, launcher.cpp S / LAUNCH, prize.cpp AddLootEntryObject's two views, jt.cpp
  PostJtLoad's flat stores, grabspot.cpp's InitGrabspot(&x->ptn) and GRFPLAYER. Still open: water.cpp SOW (a flat
  SO/WATER view with hand vtable calls through `VE`; the 64-bit word at +0x148 has no single member) and rip.cpp
  `M_ProjectRipTransform::S` (a RIP view: different pointer slots before +0x40, local M3/PHYS types over RIPM3 and
  the motion record, asm with literal offsets); shape.cpp's CID 106 rope-mesh views (RenderSelf and the sampler before it: S with +0xDC /
  +0x150, CONFIG / STATE / MESH; not SHAPE) and its two chain steppers' CHAIN views (native-correct layout, local
  names); rail.cpp RAILV / pipe.cpp PIPEV (flat views of RAIL / PIPE); launcher.cpp's SOV (an SO view with its own
  vtable declarations); carmelita.cpp / bhvsearch.cpp's list walkers (NODE views with next174 / next1B4); so.cpp
  FindTargetRecordById's T. Other flat views with void * pads (BLIPGV, BOOTYV, TRAIL, SOV, LASENV) only read LO's
  tagged word through LO; their other fields are still at EE offsets.
- **glob.cpp**: address arithmetic is `uptr` since v1.15; DMA chains keep 32-bit EE addresses: CopyRelocateDmaChain adds an `int` delta to REF tags' address
  words, and the shader packet tags pack `(ulong)(int)pshd->pbPacket << 32`. A native build needs an address model
  for chains (offsets from a heap base, or an emulated 4 GB EE address space) before its DMA/VIF interpreter can
  follow them; not a decomp-side change.
- **Inline asm without a C fallback**: shd.cpp BuildShaderGifPackets (EE-core bit manipulation with pinned
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
