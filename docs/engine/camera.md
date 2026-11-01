# Camera

Related: [player](player.md) (the focus object supplies a CPDEFI), [rendering](rendering.md), [hud-ui](hud-ui.md).

## 1. CM, the camera singleton

- `g_cm` = D_002DDF40, class CM (CID 0x87, `CM : CMBASE : BASIC`, 0x630 bytes, vtable 0x2CA4D0) `[C:runtime]`. Not an LO in Sly 2.
  Constructed by cm.cpp's static init; destroyed by its `_GLOBAL_$D` 0x14FAA8 (so cm.cpp ends at 0x14FAC8) `[H r2 cm.md]`.
- Fields `[C:runtime]`: +0x14 grfvis (also used by the activation test), +0x20 fov (1.1), +0x3C aspect 1.2987, +0x4C near clip (52), +0x50 far
  clip (52000), +0x60 matrix / +0x90 position, +0x240..+0x24C fog (near 3000, far 30000, uFogMax 0.3, RGBA). Others
  (+0x10 pzonVis, +0x18 MRD ratio, +0x40/+0x44 tan half-angles) `[H r2 cm.md]` (Lilly layout).
- Level options on g_cm (eopid 161-173: fov, fov target, near, far, near/far fog, fog RGBA, uFogMax, MRD ratio, pos, mat,
  rotation in degrees) are read by `LoadOptionsFromBrx(&g_cm)` during load `[C:static r2 cm.md]` (decoded from g_aeopid).

## 2. The policy stack

- `acpr[8]` at CM+0x310, count +0x30C; entries are 16 bytes `{prio, pcplcy, psoFocus, pv}`, kept sorted by priority `[C:runtime]`.
  `SetCmPolicy` / `RevokeCmPolicy` (Sly 1 names) push and pop `[H r2 cm.md]`.
- Each frame `UpdateCmLast` runs the top policy's Update with the focus object's **CPDEFI** (camera definition), obtained
  from the focus object's vt50 `GetCpdefi` (STEPPLAYER's 0x141640 gives a smoothed focus height) and softened by adjust springs
  (`ResetCmAdjust`, `AdjustCmCpdefi`, `SpringCm`) `[H r2 cm.md]`.
- cm.cpp also maintains view/projection matrices and frustum (`UpdateCmMat4` 0x14DC58 Lilly-matched, `RecalcCmFrustrum` 0x14DB70,
  `BuildFrustrum` 0x14DA08), the vis zone, fog (`BuildCmFgfn`), and clips/squishes the eye against nearby collision
  (`FFindCmClearInterval`, `SquishCmEye`, `ClipCmEye`) `[H r2 cm.md]`.

## 3. Camera policies (cplcy.cpp?, 0x14FCA8-0x1520C8, plus tn.cpp)

A policy is a small C++ object `CPLCYDATA {pvHero, gFovTargetSaved, sNearClipSaved}` + vptr at +0xC. Slots: 1 Init,
2 Activate(psoFocus, pv) (base saves fov target and near clip), 3 Deactivate (restores), 4 Set, 5 Revoke, 6 Update(pcpdefi, pjoy),
7 teardown `[H r2 cplcy.md]`. Policies write the camera through `SetCmMat4`, `SetCmLookAt`, `SetCmFov`, `SetCmNearClip`.
The CM constructor builds the embedded policies; their vptrs match exactly `[C:runtime]`:

| CM offset | Policy | Role | Tag |
|---|---|---|---|
| +0x3A0 | CMPART1 | default follow camera; bottom of the stack at priority 0 | [C:runtime] |
| +0x3B0 | CMPART2 = **CPTN** | tunnel/zone camera driven by TN volumes (stack entries carry TN objects as pv) | [C:runtime] |
| +0x450 (vptr 0x2CD4E0) | **CPLOOK** | binocucom / look mode (JTMODE kind 0) at priority 7; owner = current character; pvHero, rZoomMax +0x28 = 7.0, pan/tilt +0x10/+0x14, limits +-10 / +-1 | [C:runtime S31, S47, S52] |
| +0x4C0 (vptr 0x2CD450) | (CPZOOM or CPHANG) | pushed at priority 10 while Sly hides under a table (JTGRAB state 8) | [C:runtime S24] (use) |
| +0x500 (vptr 0x2CD408) | **CPCAMBLEND** | cut-scene camera blends: priority 10, pv = the JSG's CAMMOVE (JSG+0x84) | [C:runtime S59] |
| +0x520 (vptr 0x2CD498) | **recon camera** | JTMODE kind 1 (script-started recon view) at priority 8 | [C:runtime S57] |
| +0x4A8 | CPCAMERA (ctor 0x12E760, in tu_12E600, CID 0x2C camera XFM) | fixed camera on a CID 0x2C marker; priority 10 in the safehouse character select | [C:runtime S48] |
| +0x5F0 | CPTURRET (priority 9 when a turret is active) | turret view | [H r8 turret.md] |
| +0x600 | CPTWOSHOT | two-shot framing | [H r2 cplcy.md] |

Users: look mode pushes CPLOOK at priority 7 (recon camera at 8) `[C:runtime]`; the wall press pushes the grab context's private TN (JTGRAB+0x68) `[C:runtime]`; looking blends fov 1.1 -> 1.0 and near 52 -> 49; turrets push at 9; TN volumes
(`SetTnTns`/`SetTnTnsOverride`, tn.cpp 0x220D40, Sly 1 counterpart matched function by function) push CPTN `[H r8 tn.md]`.
In the Paris hub save states the stack held CMPART1 plus 1-3 CPTN entries (TN oids 0x4B30, 0x22D48) `[C:runtime]`.

## 4. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x12E600-0x12EB48 | tu_12E600 | low | camera-policy XFM CID44 |
| 0x14D3D8-0x14FAC8 | cm | high | ends with $D stub 0x14FAA8; TU really ends 0x14FAC8 (0x14FAA8 = cm _GLOBAL_$D) |
| 0x14FCA8-0x1520C8 | cplcy | high | camera policies |
| 0x220D40-0x223F70 | tn | high |  |

## 5. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x12E760](../../src/auto/func_0012E760.c) | `CPCAMERA_ctor` | Constructs the CPLCY-derived camera policy embedded in this class (__5CPLCY 0x14FCA8 then vptr) | 0.60 | T1 | verified |
| [0x14D3D8](../../src/auto/func_0014D3D8.c) | `CM_ctor` | CM constructor: BASIC header, vptr D_002CA4D0 (CM vtable), then constructs the 9 embedded camera policies at +0x3A0..+0x600 | 0.90 | T1 |  |
| [0x14D470](../../src/auto/StartupCm__Fv.c) | `SetupCm__FP2CM` |  | 0.90 | existing | arbitration; replaces existing StartupCm__Fv |
| [0x14D478](../../src/hand/func_0014D478.c) | `UpdateCmLast` | Per-frame camera update: for the top policy of acpr[] (+0x310), fetch focus CPDEFI via focus->vt50 GetCpdefi into cm+0x270, ResetCmAdjust/AdjustCmCpde | 0.85 | T1 |  |
| [0x14D650](../../src/hand/func_0014D650.c) | `ResetCmPolicies` | Calls vt7 (unknown 'reset/close' slot) on every policy in acpr[], then rUnk620=0, rUnk624=10.0 | 0.40 | T2 | verified |
| [0x14D6D8](../../src/hand/func_0014D6D8.cpp) | `InitCm` | Camera init: rgba fog 0x80303030, aspect 1.2987, near/far 100/100000, identity mat, RecalcCmFrustrum, SetCmLookAt, CutCm, Init on each embedded policy | 0.90 | T1 |  |
| [0x14D8C0](../../src/hand/func_0014D8C0.c) | `CutCm` | nCut(+0x300) = max(nCut,1): request a camera cut (no smoothing) this frame | 0.85 | T1 | verified |
| [0x14D8F8](../../src/hand/func_0014D8F8.c) | `CombineEyeLookAtProj` | matViewProj = proj * inverse(eye matrix with game->camera axis swap: x=-y, y=-z, z=x) | 0.85 | T1 |  |
| [0x14DC58](../../src/hand/func_0014DC58.cpp) | `UpdateCmMat4` | Recomputes matViewProj (+jolt shake for 0.5 s), its inverse, frustum side planes (normal and wide) and transposed VU0 cull rows, then UpdateCmVisZone | 0.90 | T1 | verified |
| [0x14E078](../../src/hand/func_0014E078.c) | `UpdateCmVisZone` | Gathers up to 32 zone objects (0x15E9C8 query), scores each against cm pos (0x2273F0), sorts, and selects the best via SetCmVisZone if its kind==1 | 0.80 | T1 |  |
| [0x14E270](../../src/hand/func_0014E270.c) | `BuildCmFgfn` | Builds the GS fog function (FGFN) from sNearFog/sFarFog/uFogMax (+0x240..0x248): linear map to 0..4080, +8417280 bias | 0.80 | T1 |  |
| [0x14E328](../../src/hand/func_0014E328.c) | `SetCmPolicy` | Insert {cpp,pcplcy,psoFocus,pv} into priority-sorted acpr[8] (+0x30C count); call pcplcy vt4 Set(pv); if new top: prev vt3 Deactivate, tModeStart=cloc | 0.95 | T1 | runtime |
| [0x14E4C8](../../src/hand/func_0014E4C8.c) | `RevokeCmPolicy` | Remove acpr[] entries matching grf mask (1 cpp, 2 pcplcy, 4 psoFocus, 8 pv), vt5 Revoke each; if top removed: Deactivate it and Activate new top | 0.90 | T1 |  |
| [0x14E6D0](../../src/hand/func_0014E6D0.c) | `SpringCm` | Critically-damped spring of *ppos/*pv toward target with (rSpring,rDamping) and clock dt; snaps and zeroes v when nCut | 0.85 | T1 |  |
| [0x14E768](../../src/hand/func_0014E768.c) | `AdjustCmCpdefi` | Springs dposAdjust/dposFocus (+0x2C0/+0x2E0) toward CalcCmAdjust; per STEPPLAYER mode +0x4F4 (0/1/2) different gains; writes cpdefi.posAdjust = posBas | 0.85 | T1 |  |
| [0x14E920](../../src/auto/func_0014E920.c) | `ResetCmAdjust` | dposAdjust = CalcCmAdjust(); zero vAdjust (+0x2D0) and vFocus (+0x2F0) | 0.85 | T1 | verified |
| [0x14E968](../../src/hand/func_0014E968.c) | `GetCmCpdefi` | If pso == psoAdjust(+0x260) copy cached cpdefi (+0x270) else pso->vt50 GetCpdefi and posAdjust = posBase + CalcCmAdjust | 0.85 | T1 |  |
| [0x14EA08](../../src/hand/func_0014EA08.c) | `FFindCmClearInterval` | Clips segment pos->dir against each SO's camera BSP (0x118668), merges intervals (0x14C9F8), returns first clear gap > duMin in *puFrom/*puTo | 0.85 | T1 |  |
| [0x14EB70](../../src/hand/func_0014EB70.cpp) | `FindCmClosestClearPos` | Samples 9 tilts x D_002BD8B0[i] pans at radius g, padding 5/g, returns closest clear position | 0.90 | T1 |  |
| [0x14EDA0](../../src/hand/func_0014EDA0.c) | `SquishCmEye` | Collects sphere-vs-BSP contacts (0x1A93C8) for the eye, builds n x n normal dot matrix + constraint values via pfn, solves (0x11AD30) and returns v +  | 0.80 | T1 |  |
| [0x14F040](../../src/hand/func_0014F040.c) | `SvSquishCmEyeConstraint` | Constraint for SquishCmEye: GSmooth(dist-to-plane, sNearCorner(+0x48) - k) ; return dot(vEye, normal) - smoothed | 0.85 | T1 | verified |
| [0x14F110](../../src/hand/func_0014F110.cpp) | `ClipCmEye` | vEye=(next-prev)/dtPrev; gather camera-colliding SOs in a box around prev; SquishCmEye; return prev + vClip*dt | 0.85 | T1 |  |
| [0x14F268](../../src/hand/func_0014F268.c) | `ConvertCmScreenToWorld` | Screen (x,y,depth) -> world: axis swap, scale by tanHalfX/Y (+0x40/+0x44), multiply by CM::mat | 0.85 | T1 |  |
| [0x14F308](../../src/hand/func_0014F308.c) | `ConvertCmWorldToScreen` | World -> screen: inverse(CM::mat) * pos, axis swap, divide by tanHalf and depth (if > 1e-4) | 0.85 | T1 |  |
| [0x14F3C8](../../src/hand/func_0014F3C8.c) | `SetCmPos` | Option setter (g_aeopid[171], otyp 2 vector): keep rotation, set CM::mat pos row, SetCmMat4 | 0.85 | T1 |  |
| [0x14F478](../../src/hand/func_0014F478.c) | `GetCmRotDeg` | Option getter (g_aeopid[173]): euler angles of CM::mat (0x11BFF0) in degrees | 0.85 | T1 | verified |
| [0x14F5A0](../../src/hand/func_0014F5A0.c) | `SetCmFov` | fov(+0x20)=fovTarget(+0x24)=f, dfov=0; RecalcCmFrustrum if changed > 1e-4 | 0.90 | T1 | verified |
| [0x14F610](../../src/auto/func_0014F610.c) | `SetCmNearClip` | sNearClip(+0x4C)=f; RecalcCmFrustrum | 0.85 | T1 |  |
| [0x14F670](../../src/auto/func_0014F670.c) | `SetCmFarClip` | sFarClip(+0x50)=f; RecalcCmFrustrum | 0.85 | T1 |  |
| [0x14F690](../../src/auto/func_0014F690.c) | `SetCmNearFog` | sNearFog(+0x240)=f; RecalcCmFrustrum | 0.85 | T1 |  |
| [0x14F6B0](../../src/auto/func_0014F6B0.c) | `SetCmFarFog` | sFarFog(+0x244)=f; RecalcCmFrustrum | 0.85 | T1 | verified |
| [0x14F6D0](../../src/auto/func_0014F6D0.c) | `SetCmUFogMax` | uFogMax(+0x248)=f; RecalcCmFrustrum | 0.85 | T1 |  |
| [0x14F6F0](../../src/hand/func_0014F6F0.c) | `SetCmRgbaFog` | rgbaFog(+0x24C)=*prgba; 0x1ED278 on it; RecalcCmFrustrum | 0.80 | T1 |  |
| [0x14F750](../../src/hand/func_0014F750.c) | `JoltCm` | tJolt(+0x618)=clock.t, nUnk61c=arg: starts the 0.5 s camera shake applied by UpdateCmMat4 | 0.80 | T1 |  |
| [0x14FCA8](../../src/auto/func_0014FCA8.c) | `CPLCY_ctor` | CPLCY base constructor: vptr(+0xC) = D_002CD570 | 0.85 | T1 |  |
| [0x14FCC8](../../src/hand/func_0014FCC8.c) | `ActivateCplcy` | Default slot 2 Activate(psoFocus, pv): pvHero(+0)=psoFocus; save g_cm fovTarget (+4) and sNearClip (+8) | 0.80 | T1 |  |
| [0x14FCE8](../../src/auto/func_0014FCE8.c) | `DeactivateCplcy` | Default slot 3 Deactivate: pvHero=0; SetCmNearClip(saved); fovTarget=saved | 0.80 | T1 |  |
| [0x14FD40](../../src/auto/func_0014FD40.c) | `UpdateCplcy` | Default (empty) slot 6 Update(pcpdefi, pjoy) | 0.85 | T1 | verified |
| [0x14FD50](../../src/auto/func_0014FD50.c) | `FActiveCplcy` | True if pcplcy is g_cm's current top policy (D_002DE254 = g_cm.acpr[0].pcplcy) | 0.85 | T1 |  |
| [0x14FDC0](../../src/auto/func_0014FDC0.c) | `UpdateCmpart1` | Slot 6 of D_002CD528: just calls CPLCY::Update (default policy does nothing) | 0.85 | T1 | verified |
| [0x14FDE0](../../src/auto/func_0014FDE0.c) | `CPLOOK_ctor` | CPLOOK ctor (vtable D_002CD4E0): rZoomMax(+0x28)=7.0, psndZoom/psndUnk38 = 0 | 0.80 | T1 |  |
| [0x14FF38](../../src/hand/func_0014FF38.c) | `ActivateCplook` | Binocucom/look mode enter: start zoom sounds 0x26/0x25, pan/tilt from current camera (or focus), pan/tilt limits, uZoom=0, fov zoom range, near clip 4 | 0.90 | T1 | runtime |
| [0x150160](../../src/hand/func_00150160.c) | `UpdateCplook` | Look-mode update: zoom from joy (+0x1074) with sound pitch/volume, fov lerp, pan/tilt from sticks (invert option via splice 0x3D4A60), limits, SetCmLo | 0.85 | T1 |  |
| [0x150610](../../src/hand/func_00150610.c) | `DeactivateCplook` | Stop/release zoom sounds, SetCmCut with pan+PI/-tilt (camera continues behind), then CPLCY::Deactivate | 0.85 | T1 |  |
| [0x150728](../../src/auto/func_00150728.c) | `CPZOOM_ctor` | CPZOOM ctor (vtable D_002CD498): rZoomMax(+0xB4)=7.0, uZoomTarget(+0x18)=-1, sounds 0 | 0.80 | T1 |  |
| [0x150EE8](../../src/auto/func_00150EE8.c) | `SetCpzoomUZoomTarget` | uZoomTarget(+0x18)=g | 0.85 | T1 | verified |
| [0x150EF0](../../src/hand/func_00150EF0.c) | `SnapCpzoom` | Reset smoothed center/eye to current targets, uZoom=uZoomTarget (if >=0), pose = target pose | 0.85 | T1 | verified |
| [0x151208](../../src/auto/func_00151208.c) | `CPHANG_ctor` | CPHANG ctor (vtable D_002CD450) | 0.85 | T1 | verified |
| [0x151340](../../src/hand/func_00151340.cpp) | `UpdateCphang` | Sway spring: stick deflection or, in state 9, random targets every few seconds; moves target XF (0x12D108) and SetCmMat4 from the camera transform | 0.85 | T1 | verified |
| [0x151648](../../src/hand/func_00151648.c) | `ActivateCpcamblend` | pcammove(+0x10)=pv; pcammove->pevent vt1 (begin event) | 0.85 | T1 | verified |
| [0x220D40](../../src/hand/func_00220D40.c) | `InitCptnAngles` | if both angles are the sentinel, set both to GLimitLm(atan2(h, len)) | 0.30 | T2 | verified |
| [0x220ED0](../../src/hand/func_00220ED0.c) | `InitTn` | InitXfm; camera defaults (2.0,1.0,2.0,1.0), tns=-1, clq/lm defaults D_002C41C0/D_002C41D0, priority 7, busyk 1, grf\|=2 | 0.85 | T1 | verified |
| [0x220FD0](../../src/hand/func_00220FD0.c) | `LoadTnFromBrx` | LoadXfm-style XF read, ReadGeom + ReadBspc (tunnel BSP), then tables allocated via PvAllocSlot | 0.85 | T1 | parse |
| [0x221178](../../src/hand/func_00221178.c) | `SetTnTns` | tns change: In -> SetCmPolicy(g_cm, pri+2, &g_cm.cptn(+0x3B0), ppo, ptn) + splice event 5; Out -> RevokeCmPolicy(8) + event 6; store tns and tTns=g_cl | 0.85 | T1 | verified |
| [0x221268](../../src/hand/func_00221268.c) | `SetTnTnsOverride` | if changed store and call UpdateTnCallback(ptn,0) | 0.85 | T1 |  |
| [0x221298](../../src/hand/func_00221298.cpp) | `UpdateTnCallback` | current player (0x1C4A48 PpoCur); retarget; inside test via tunnel BSP (0x118FB0) unless override; switch unless jumping | 0.85 | T1 |  |
| [0x221388](../../src/hand/func_00221388.c) | `UpdateTn` | UpdateLo then, if tnsOverride, PostSwCallback(g_sw, ptn, UpdateTnCallback) | 0.85 | T1 |  |
| [0x2213D8](../../src/auto/func_002213D8.c) | `OnTnRemove` | OnLoRemove then SetTnTns(Out) | 0.85 | T1 | verified, arbitration |
| [0x221980](../../src/auto/func_00221980.c) | `GetTnFocusX` | returns tnfn.vecFocus.x | 0.60 | T1 | verified |
| [0x223A10](../../src/hand/func_00223A10.c) | `GDistPlanePos` | signed distance of pos to a plane | 0.60 | T1 | verified |

Selected 60 of 137 named functions (T1 with confidence >= 0.7, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/camera.md](../reference/functions/camera.md).

## Open questions
- Which of CPZOOM/CPHANG is CM+0x4C0 (the other has no observed use); meaning of policy slot 7.
- Identity of CID 0x15 (camera-related XFM with its own list D_003E1474, used by camera code 0x1421F8..).
- Experiments: break 0x14FF38 (ActivateCplook) entering binocucom; break 0x151700 during a cut-scene blend and dump the CAMMOVE.
