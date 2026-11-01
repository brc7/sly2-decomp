# Animation: segments, applications, blends, actuators, transforms, state machines

Related: [player](player.md), [guards-ai](guards-ai.md) (animation bindings), [cutscenes](cutscenes.md).

## 1. ASEG and ASEGA (aseg.cpp 0x12F248, asega.cpp 0x1308A8)

- **ASEG** (CID 0x6D, LO child, 0xC0): an animation segment loaded from the level (1,239 in the Paris hub, second most common class) `[C:runtime]` counts.
  Slots: 7 Init 0x12F248, 14 LoadFromBrx 0x12F2A8, 15 PostLoad 0x12F8F0 (shared with ASEGBL, CID 0x6E, a blended segment) `[H r1 anim.md]`.
- **ASEGA** (CID 0x86, BASIC child, 0x60, refcounted: slots 1/2 Incref/Decref, 4 Notify): an *application* of a segment to an object tree;
  +0x40 effective speed = speed (+0x44) x master (+0x48) x k (+0x4C); +0x58 hands-off flag; live list D_003E1218 `[H r1 anim.md]`.
  `ApplyAsegCur` 0x1305B8 (matched hand file) is the usual entry; `PasegaApplyAseg` 0x130488 wraps `ApplyAseg` 0x12F9B8 `[C:verified]` (r1r2 verifier: wrapper level).
- ASEGA events: `HandleAsegaEvent` 0x131290 switches on the event kind; actions are implemented in sw.cpp (0 sound start, 1 sound stop,
  7/8 show/hide with fade, 13-17/20 object toggles); events 0x25-0x28 drive the guard mover lock and BHV busy flags `[H r7 sw.md, r6b bhvstk.md]`.
- UpdateSw walks the ASEGA list each frame (0x130918) `[H r8 update.md]`.

## 2. Curves and actuators (act? 0x12B3B0)

72 functions of C++ curve and actuator classes (vptr at +0: not BASIC); families are clear from vtables, individual types are not.
Actuator slot 5 is a **goal query** with out-params (current goal, zero velocity), not a per-frame update `[C:verified]` (r1r2 verifier pattern 2).
Sly 1 had ACTVAL/ACTREF/ACTADJ/ACTBANK/ACTLA/ACTSEG; the mapping is open `[H r1 anim.md]`.

## 3. Blends (blend.cpp 0x137A30) and directional blends

- Sly 1 blend.c: `EnsureAsegBlendDynamic` 0x137A30 (verified), `FillBlAmrsg`, `CalculateBlendAmrsg`, `ImrsgLookUp`; `ReblendAsegbl` now takes only the ASEGBL;
  three new virtuals of blend-channel actuator classes (vtables 0x2CB588, 0x2CB5A8) `[H r2b blend.md]`.
- Player blends: jump (5 entries), swing (3), climb, throw blends via `EnsureAsegBlendDynamic`; 8-direction locomotion blend measured from clip root motion (dirblend TU 0x1A0300) `[H r4b]`.
- **GAIT** (embedded in STEPPLAYER +0x1320/+0x1C30): walk/run weights (`UpdateGaitBlend` 0x20BB58) and footstep sounds (`UpdateGaitFootsteps` 0x20BC08, range 500-2000) `[H r7c gait_leap.md]`.
- **LEAP** (CID 0x91): animation-driven jump to a target (label 0x22 triggers launch); **DIRANIM** (CID 0x92) picks the best of 4 directional ASEGs `[H r7c]`.

## 4. XF slots and transforms (xf.cpp 0x22C818, Lilly: xf.c)

Each SO has an array of XF records (0x100 bytes: MATRIX4 mat, MATRIX4 matWorld, then act/role/oid/parent/stamp) `[C:static layouts.md]`, `[C:runtime]` for matWorld.
`LoadSoXfsFromBrx` reads them; `UpdateSoSlots` updates them per frame by role (velocity, chase, spring, chain, Hermite); actuator hook lists
sorted by priority replace Sly 1's ACT; XF ids resolve through local / parent / context-stack lookup (SO slot 51 / XFM slot 49, 0x22D6B0) `[H r8 xf.md]`.
Ropes/tails: chains of XF links simulated with damping, bend limits, gravity and substeps (role 0x10; tail? TU 0x21C820) `[H r8 tail_chain.md]`.
Two-bone IK: `SolveIk` 0x186A50 (law of cosines), used by UpdateSoSlots and hand IK while hanging `[H r4b ik.md]`.

## 5. State machines: SM and SMA (sm.cpp 0x1EFA90-0x1F1768)

- **SM** (CID 0x6F, LO child): states `SMS {oid, oidNext}` and weighted transitions `SMT {from, to, grfsmt, grfsmtBreak, gProbability}` loaded
  from the level (`SM::LoadFromBrx` 0x1EFAC0, Lilly-ported) `[H semantic sm.md]`; anchors `PostSmLoad` 0x1F00C8, `PsmaFindSm` 0x1F03C8, `IsmsFindSmOptional` 0x1F0400 `[C:static]`.
- **SMA** (CID 0x88, refcounted application on a root object): plays the ASEGA of the current transition and picks the next one toward a goal state.
  New in Sly 2: precomputed state-distance tables per transition mask (shortest path to goal, weighted-random ties), a mask filter (`SetSmaMask`),
  an ETA query, and helpers to replace/release an SMA held in a field `[H semantic sm.md]`.
- Users: JOB/TASK RSMG records, buttons, guards (animation bindings), JT (SMA completion message 3 drives state changes), dialog lip-sync roles.

## 6. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x12B3B0-0x12E600 | act? | curves + actuators |
| 0x12EB48-0x12F0F0 | amb? (cid 0x2B) |  |
| 0x12F248-0x1308A8 | aseg |  |
| 0x1308A8-0x131F70 | asega |  |
| 0x137A30-0x138F08 | blend |  |
| 0x186610-0x187030 | ik |  |
| 0x1A0300-0x1A0B18 | dirblend TU |  |
| 0x1EFA90-0x1F1768 | sm |  |
| 0x20B840-0x20CE28 | tu_20B840 (GAIT/LEAP/DIRANIM) |  |
| 0x21C820-0x21D278 | tail? (rope chains) | Lilly: chain.c |
| 0x22B720-0x22C818 | wr |  |
| 0x22C818-0x231100 | xf | Lilly: xf.c |

## 7. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x12D220](../../src/hand/func_0012D220.c) | `ActLookAt::Update` | Updates an aim/look-at act: finds target object (PloFindSwObject), pan/tilt (CalculateVectorPanTilt) | 0.30 | VERIFIED |
| [0x12EC50](../../src/hand/func_0012EC50.c) | `CID43::PostLoad` | slot 15: PostLoLoad, allocates, registers with the stream-sound group (0x1FC408) and IambFind (0x1FE990) | 0.50 | VERIFIED |
| [0x12ED20](../../src/hand/func_0012ED20.c) | `OnCid43Add` | slot 23: LO::Unfreeze then starts its ambient sound (0x1FBB90/0x1FB2F8, SetSndRange) | 0.45 | VERIFIED (arbitration/conflict fix; was CID43::Unfreeze) |
| [0x12EDE8](../../src/hand/func_0012EDE8.c) | `OnCid43Remove` | slot 22: LO::Freeze then StopSnd | 0.45 | VERIFIED (arbitration/conflict fix; was CID43::Freeze) |
| [0x12F248](../../src/auto/func_0012F248.c) | `InitAseg` | InitLo (0x1A5270) then InitDl of the ASEGA list; two speed/weight fields = 1.0 | 0.85 | HYP |
| [0x12F2A8](../../src/hand/func_0012F2A8.cpp) | `LoadAsegFromBrx` | Reads ASEG: channels (0x12B3B0/0x12CB98 per channel kind), labels, events (0x12F590) | 0.85 | HYP |
| [0x12F590](../../src/hand/func_0012F590.cpp) | `LoadAsegEventsFromBrx` | Reads the ASEG event list (typed U8/S8/S16/S32/F32 payloads), allocates arrays | 0.80 | VERIFIED |
| [0x12F9B8](../../src/hand/func_0012F9B8.cpp) | `ApplyAsegOvr` | Core of applying an animation segment to an ALO hierarchy: allocates an ASEGA from the pool, links it, binds channels, sets speed, fast-forwards event | 0.80 | HYP |
| [0x130488](../../src/hand/func_00130488.c) | `ApplyAseg` | ApplyAsegOvr without override | 0.75 | WRONG (verify: PasegaApplyAseg) |
| [0x1305B8](../../src/hand/ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA.cpp) | `ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA` |  |  | existing |
| [0x130918](../../src/hand/func_00130918.c) | `UpdateAsega` | Per-frame advance: time wrap (0x1317D8), event dispatch (0x130F88), retract when finished | 0.75 | HYP |
| [0x130B18](../../src/hand/func_00130B18.c) | `RetractAsega` | Stops an application: unlink, hands-off release, notify, Decref | 0.85 | VERIFIED |
| [0x130D30](../../src/hand/func_00130D30.c) | `SeekAsega` | Seek to (SEEK kind, t, svt) then UpdateAsegaIeaCur | 0.90 | HYP |
| [0x131290](../../src/hand/func_00131290.cpp) | `HandleAsegaEvent` | Executes one ASEG event (sound, rumble 0x187E28, hands-off, apply/seek/retract other asegs, Splice, emitters...) | 0.80 | HYP |
| [0x131990](../../src/auto/func_00131990.c) | `SetAsegaHandsOff` | Toggles the ASEGA's hands-off flag (+0x58) with Increment/DecrementSwHandsOff | 0.85 | HYP |
| [0x131A20](../../src/auto/func_00131A20.c) | `SetAsegaMasterSpeed` | +0x48 = g; effective speed +0x40 = speed*master*k | 0.85 | HYP |
| [0x131D68](../../src/hand/func_00131D68.cpp) | `AsegaChnUpdate` | Per-frame channel time wrap (0x1317D8) and virtual update | 0.25 | WRONG (verify: GetActsegGoal (ACT slot 5)) |
| [0x137A30](../../src/hand/func_00137A30.cpp) | `EnsureAsegBlendDynamic` | Create/extend the ASEGBL blending a set of asegs (cid 0x6E) and its MRSG tables | 0.90 | VERIFIED |
| [0x1384C0](../../src/hand/func_001384C0.c) | `ReblendAsegbl` | Normalise asega local times by tMax, recompute blend MRSG tables (CalculateBlendAmrsg per mrsgc), rescale times; mirror times from a linked asegbl (+0 | 0.75 | PLAUSIBLE |
| [0x1EFAC0](../../src/hand/func_001EFAC0.cpp) | `SM::LoadFromBrx` | Reads `csms` (U8) states {oid, oidNext}, `csmt` (U8) transitions {from, to (via IsmsFindSmRequired), grfsmt, grfsmtBreak, gProbability}, then builds t | 0.85 | HYP |
| [0x1F00C8](../../src/hand/PostSmLoad__FP2SM.cpp) | `PostSmLoad` | PostLoLoad; if a pending apply record exists, find its root ALO by OID and PsmaApplySm | 0.85 | HYP |
| [0x1F01B8](../../src/hand/func_001F01B8.cpp) | `ApplySmCore` | Takes an SMA from the g_sw pool (+0x3DDC), memsets 0x50, links it into SM and world lists, runs the ctor, sets the start state, notifies, chooses a tr | 0.85 | HYP |
| [0x1F0380](../../src/hand/func_001F0380.c) | `PsmaApplySm` | Wrapper returning the SMA (kind 4) | 0.85 | HYP |
| [0x1F03C8](../../src/hand/PsmaFindSm__FP2SMP3ALO.cpp) | `PsmaFindSm` | Linear search of the SMA list by root ALO | 0.85 | HYP |
| [0x1F0400](../../src/hand/IsmsFindSmOptional__FP2SM3OID.cpp) | `IsmsFindSmOptional` | Index of the state with that OID, or -1 | 0.85 | HYP |
| [0x1F0438](../../src/auto/func_001F0438.c) | `IsmsFindSmRequired` | Optional lookup clamped to ≥0 (28 B) | 0.85 | HYP |
| [0x1F04F8](../../src/hand/func_001F04F8.c) | `OidFromSmIsms` | `asms[isms].oid`, or -1 if out of range | 0.85 | HYP |
| [0x1F0528](../../src/auto/func_001F0528.c) | `SMA::Incref` | `cref(+0x10)++` | 0.85 | HYP |
| [0x1F0538](../../src/auto/func_001F0538.c) | `SMA::Decref` | `--cref`; at 0: dtor, clear, unlink, free to pool D_003E1278 | 0.85 | HYP |
| [0x1F0598](../../src/hand/func_001F0598.c) | `RetractSma` | If live: drop the current ASEGA, unlink from the SM list and the live list D_003E126C, send message 3, Decref | 0.85 | HYP |
| [0x1F0660](../../src/hand/func_001F0660.cpp) | `SetSmaGoal` | `ismsGoal = (oid==-1 ? -1 : IsmsFindSmRequired)`; if changed: retract the running ASEGA when the transition allows a break (flag 2), else choose a tra | 0.85 | HYP |
| [0x1F07C8](../../src/auto/func_001F07C8.c) | `GetSmaGoal` | `OidFromSmIsms(psm, ismsGoal +0x3C)` | 0.85 | HYP |
| [0x1F07E8](../../src/auto/func_001F07E8.c) | `GetSmaCur` | Same for `ismsCur` (+0x34) | 0.85 | HYP |
| [0x1F0850](../../src/hand/func_001F0850.cpp) | `SetSmaSvt` | Stores +0x44; SetAsegaSpeed on the running ASEGA | 0.85 | HYP |
| [0x1F0948](../../src/hand/func_001F0948.cpp) | `SeekSma` | Set the goal, abort the current transition, choose again immediately (`ChooseSmaTransition(p, 1)`) | 0.85 | HYP |
| [0x1F0AD0](../../src/hand/func_001F0AD0.c) | `PsmtChooseWeighted` | Weighted random pick: `GRandInRange(0, Σ gProbability)` then cumulative walk | 0.85 | HYP |
| [0x1F0B80](../../src/hand/func_001F0B80.c) | `PsmtChooseTowardGoal` | Collects transitions from cur (or reversible, flag 1) that cut the distance to the goal by one hop, then weighted pick | 0.85 | HYP |
| [0x1F0CF0](../../src/hand/func_001F0CF0.c) | `PsmtChooseFrom` | Collects all transitions out of a state under the mask, then weighted pick | 0.85 | HYP |
| [0x1F0D88](../../src/hand/func_001F0D88.c) | `ChooseSmaTransition` | Main driver: if no goal or at goal, random wander; else toward goal. Starts the transition's ASEG via ApplyAseg (0x12F9B8), sets ismsNext, notifies | 0.85 | HYP |
| [0x1F0FA0](../../src/auto/func_001F0FA0.c) | `EndSmaTransition` | `pasegaCur=0; ismsCur=ismsNext; ismsNext=-1; NotifySmaSpliceOnEnterState(old, new)` | 0.85 | HYP |
| [0x1F1078](../../src/hand/func_001F1078.cpp) | `HandleSmaMessage` | On msg 2 (ASEGA finished) for the current ASEGA: EndSmaTransition, ChooseSmaTransition | 0.85 | HYP |
| [0x1F10F0](../../src/hand/func_001F10F0.cpp) | `SkipSma` | While a transition runs: if dt < remaining, seek forward and stop; else end and re-choose | 0.85 | HYP |
| [0x1F13A8](../../src/hand/func_001F13A8.c) | `NotifySmaSpliceOnEnterState` | Builds {psma, oidFrom, oidTo} and sends message 10 (notify) and 8 (Splice event with 2 args) through vtable slots 4/3 | 0.85 | HYP |
| [0x1F1448](../../src/hand/func_001F1448.c) | `ReplaceSma` | Retract and Decref `*ppsma`; if a new SM is given, PsmaApplySm, Incref, store | 0.85 | HYP |
| [0x1F14F8](../../src/hand/func_001F14F8.c) | `ReplaceSmaEx` | Same with explicit kind | 0.85 | HYP |
| [0x1F15B8](../../src/hand/func_001F15B8.c) | `ReleaseSma` | Retract, Decref, null | 0.85 | HYP |
| [0x21C888](../../src/hand/func_0021C888.c) | `LoadSoChainsFromBrx` | reads N chains: per chain S8 count, U8, 4 floats, 2 vectors, S16 count of links; per link resolves the XF via vt slot51, creates a kind-3 hook (vt D_0 | 0.60 | VERIFIED |
| [0x22B720](../../src/hand/func_0022B720.c) | `InitWr` | InitLo; -1 fields, g=-1.0, busyk, grf | 0.75 | HYP |
| [0x22B7D8](../../src/hand/func_0022B7D8.cpp) | `UpdateWrStateVectors` | per WRBG vertex weights + state vectors | 0.75 | VERIFIED |
| [0x22C570](../../src/hand/func_0022C570.c) | `GetWrWavelength` | 1/circle frequency (0 if 0) | 0.85 | VERIFIED |
| [0x22DE88](../../src/hand/func_0022DE88.cpp) | `UpdateSoSlots` | per-frame step of all XF slots of an SO (roles, chase, springs, chains) | 0.75 | HYP |
| [0x22FCC8](../../src/hand/func_0022FCC8.c) | `ConvertXfVec` | transforms a direction between XF spaces | 0.45 | VERIFIED |

Selected 52 of 273 named functions (confidence >= 0.75, verified, or flagged). Full list: [reference/functions/animation.md](../reference/functions/animation.md).

## Open questions
- Which curve class is position/rotation/scale/float and which actuator class is which (break 0x12B3B0 at load and log the kind).
- ASEGA event kinds 13-16 and 20 (callees 0x1F6978..0x1F6B48 unnamed). Experiment: break 0x131290 and log the kind byte.
- File names of the two XFM-child TUs between act and aseg (0x12E600 camera XFM, 0x12EB48 CID 0x2B "amb?" with sound start/stop in OnAdd/OnRemove).
