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

- `acpr[8]` at CM+0x310, count +0x30C (`ccpr` = camera policy-stack depth; earlier headers called +0x30C `cmodeHero`, types round 2 corrected it `[C:runtime]`); entries are 16 bytes `{prio, pcplcy, psoFocus, pv}`, kept sorted by priority `[C:runtime]`.
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

{{TUS}}

## 5. Key functions

{{KEYFUNCS:max=60 minc=0.7}}

## Open questions
- Which of CPZOOM/CPHANG is CM+0x4C0 (the other has no observed use); meaning of policy slot 7.
- Identity of CID 0x15 (camera-related XFM with its own list D_003E1474, used by camera code 0x1421F8..).
- Experiments: break 0x14FF38 (ActivateCplook) entering binocucom; break 0x151700 during a cut-scene blend and dump the CAMMOVE.
