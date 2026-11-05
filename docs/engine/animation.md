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
- UpdateSw walks the ASEGA list each frame (0x130918) `[H r8 update.md]`. The list link is **ASEGA+0x24 `dleSw`** (SW::dlAsega ibDle = 0x24; 8,250 ASEGAs walked; was `_pad_35`) `[C:runtime types round 2]`; ASEGA svt = svtRate * svtMaster, ASEG tMax +0x74 / svtMaster +0x78.

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

## Devtools findings (v3, 2026-11-01)

From [devtools/anim_lab](../guides/devtools.md#anim_lab) (ANIM_FORMAT.txt; RAM checks against S04/S05/S68). Tags as given by the tool; not re-derived in v3.
- Row-vector convention, world = local x parentWorld (0x22FAA0 / 0x11C168) `[C:static]`; FK from RAM locals reproduces RAM world matrices to 0.005 units (S68) `[C:runtime anim_lab]`.
- Only XF kinds 1, 6 and 12 are animated by clips (30k+ channels) `[C:parse anim_lab]`; clips can drive child SOs (channel key names the SO by oid).
- Two-bone IK (`SolveIk` 0x186A50, `InitIkChain` 0x186930) after FK reproduces all 4 of Sly's chains in S68 to 1e-4 `[C:runtime anim_lab]`.
- ASEGA (cid 0x86): +0x14 ASEG*, +0x18 root, +0x1C tLocal, +0x40 effective speed, +0x44 speed, +0x48 master `[C:runtime S68]`; `ApplyAseg` copies ASEG+0x78 into
  ASEGA+0x48. Option 109 = playback speed (1.2 on the cane clip) `[C:static+runtime]` for clip 0x11D only; for all clips `[H]`.
- **Cane swing clip 0x11D** (JT+0x1CE0): tMax 0.833 s, speed 1.2; event 15/16 = show/hide SOPART 0x14C at t 0.133 / 0.333 (the hit window), swing trail
  0x104B, sounds 11 and 59, messages 0x164/0x165 at 0.5 `[C:parse anim_lab]` + runtime7b.

## Animation format summary (anim_lab, v3.1)

From `devtools/anim_lab/ANIM_FORMAT.txt` (tool tags in brackets: code / parse / runtime / data).
- **Conventions** `[code 0x22FAA0, 0x11C168]`: row vectors, `p_parent = p_local . L`, row 3 = translation, world = local . parentWorld. Times in seconds,
  stored as U16 ticks of 1/60 s.
- **Skeleton = the SO's XF table** (`LoadSoXfsFromBrx` 0x22D7A0) `[parse]`. An XF is one degree-of-freedom group, not a bone: kind 1/4/5/9/10 translate,
  6/7 rotate (Euler, R = Rx.Ry.Rz), 12 scale, 11 shear, 8/14 pivot, others static `[code 0x22C8C0 / 0x22CA18]`. A joint is a translate -> rotate -> scale stack
  sharing one tag; only kinds 1, 6 and 12 are animated by clips `[data, 30k+ channels]`. parentRef: -1 none, 0x0nnn own XF, 0x1nnn parent SO's XF, 0x2000 context
  top. FK reproduces RAM world matrices to 0.005 units `[runtime S68]`. Sly 139 XFs, Murray 145, Bentley 76, boar guard 71.
- **Two-bone IK** (`LoadSoIkFromBrx` 0x186670, `SolveIk` 0x186A50) runs after FK; the twist is the clip's scalar (factory-2) channel on the upper XF. All 4 of
  Sly's chains reproduced to 1e-4 in S68 `[runtime]`. Constraints and chains (tails/ropes) are parsed, not simulated.
- **Clips = ASEG (cid 0x6D)** (`LoadAsegFromBrx` 0x12F2A8): channels keyed by SO oid + XF; each channel adds an actuator to the XF (`AddXfAct` 0x230018,
  priority ASEG+0x6C) whose value **replaces** the XF value per kind `[code + runtime]`. Curves: constant, component keys and cubic Hermite keys (tangents in
  units/s, 0x117120); scalar factory 2 for IK twist. Short 0.083 s clips act as overlays on a second ASEGA.
- **ASEGA** (cid 0x86): +0x14 ASEG*, +0x18 root, +0x1C tLocal, +0x40 effective speed; option 109 = 1.2 on ASEG 0x11D copied to ASEGA+0x48 `[code + runtime, one clip]`;
  that option 109 is a general speed multiplier is `[H]`.
- Open: blending/priority between simultaneous ASEGAs, ASEGBL blends, SM-driven transitions.

## 6. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x12B3B0-0x12E600 | act | low | actuator/curve classes; act(?); curves + actuators |
| 0x12EB48-0x12F0F0 | amb | low | CID43 ambient-sound XFM; amb(?) |
| 0x12F248-0x1308A8 | aseg | high |  |
| 0x1308A8-0x131F70 | asega | high |  |
| 0x137A30-0x138F08 | blend | medium |  |
| 0x186610-0x187030 | ik | low |  |
| 0x1A0300-0x1A0B18 | dirblend | low |  |
| 0x1EFA90-0x1F1768 | sm | high | semantic/tu_docs/sm.md |
| 0x20B840-0x20CE28 | tu_20B840 | low | gait/leap/diranim |
| 0x21C820-0x21D278 | tail | low | Lilly chain.c; Lilly: chain.c |
| 0x22B720-0x22C818 | wr | high |  |
| 0x22C818-0x231100 | xf | medium | Lilly xf.c; Lilly: xf.c |

## 7. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x12B3B0](../../src/game/engine/anim/act.cpp) | `PacNewKind` | Factory: new animation-curve object of kind 0..5 (six C++ curve classes) | 0.75 | T1 | parse |
| [0x12B4F8](../../src/game/engine/anim/act.cpp) | `LoadAcConstFromBrx` | Reads a fixed-size constant record (Read) | 0.75 | T1 | parse |
| [0x12B5E0](../../src/game/engine/anim/act.cpp) | `LoadAcVecAFromBrx` | Reads U16 key count and VECTOR keys (with tangents) into a heap array | 0.75 | T1 | parse |
| [0x12BA98](../../src/game/engine/anim/act.cpp) | `LoadAcVecBFromBrx` | Second vector-keyed curve class: load | 0.75 | T1 | parse |
| [0x12BB70](../../src/game/engine/anim/act.cpp) | `AcVecB_IkeyFind` | Key segment search | 0.35 | T2 | verified |
| [0x12BF30](../../src/game/engine/anim/act.cpp) | `LoadAcFloatAFromBrx` | Scalar-keyed curve: U16 count, U8 flags, F32 keys and vectors | 0.75 | T1 | parse |
| [0x12C570](../../src/game/engine/anim/act.cpp) | `LoadAcFloatBFromBrx` | Second scalar curve class: load | 0.75 | T1 | parse |
| [0x12CB98](../../src/game/engine/anim/act.cpp) | `PacNewKind2` | Factory: new object of kind 0..2 (second curve family) | 0.75 | T1 | parse |
| [0x12CC68](../../src/game/engine/anim/act.cpp) | `LoadAcFloatConstFromBrx` | Reads one F32 | 0.75 | T1 | parse |
| [0x12CCB8](../../src/game/engine/anim/act.cpp) | `LoadAcFloatKeysFromBrx` | U16 count + F32 keys | 0.75 | T1 | parse |
| [0x12CF20](../../src/game/engine/anim/act.cpp) | `ACT_Stub3` | 8-byte shared virtual (no-op) | 0.25 | T2 | verified |
| [0x12D220](../../src/game/engine/anim/act.cpp) | `UpdateActLookAt` | Updates an aim/look-at act: finds target object (PloFindSwObject), pan/tilt (CalculateVectorPanTilt) | 0.85 | T1 | verified |
| [0x12DCA0](../../src/game/engine/anim/act.cpp) | `PactNewB` | Create act variant and attach | 0.30 | T2 | verified |
| [0x12DD28](../../src/game/engine/anim/act.cpp) | `PactNewC` | Create act variant and attach | 0.30 | T2 | verified |
| [0x12DDB0](../../src/game/engine/anim/act.cpp) | `PactNewD` | Create act variant and attach | 0.30 | T2 | verified |
| [0x12E3F0](../../src/game/engine/anim/act.cpp) | `ActUnk_12E3F0` | Wrapper of InitActKind (no callers: callback) | 0.20 | T2 | verified |
| [0x12EB78](../../src/game/engine/sound/amb.cpp) | `LoadCid43FromBrx` | slot 14: LoadXfmFromBrx (0x231170) then reads/allocates its sound list | 0.75 | T2 | parse |
| [0x12EC50](../../src/game/engine/sound/amb.cpp) | `PostCid43Load` | slot 15: PostLoLoad, allocates, registers with the stream-sound group (0x1FC408) and IambFind (0x1FE990) | 0.85 | T2 | verified |
| [0x12ED20](../../src/game/engine/sound/amb.cpp) | `OnCid43Add` | slot 23: LO::Unfreeze then starts its ambient sound (0x1FBB90/0x1FB2F8, SetSndRange) | 0.85 | T2 | arbitration |
| [0x12EDE8](../../src/game/engine/sound/amb.cpp) | `OnCid43Remove` | slot 22: LO::Freeze then StopSnd | 0.85 | T2 | arbitration |
| [0x12F248](../../src/game/engine/anim/aseg.cpp) | `InitAseg` | InitLo (0x1A5270) then InitDl of the ASEGA list; two speed/weight fields = 1.0 | 0.85 | T1 |  |
| [0x12F2A8](../../src/game/engine/anim/aseg.cpp) | `LoadAsegFromBrx` | Reads ASEG: channels (0x12B3B0/0x12CB98 per channel kind), labels, events (0x12F590) | 0.90 | T1 | parse |
| [0x12F590](../../src/game/engine/anim/aseg.cpp) | `LoadAsegEventsFromBrx` | Reads the ASEG event list (typed U8/S8/S16/S32/F32 payloads), allocates arrays | 0.85 | T1 | parse, verified |
| [0x130010](../../src/game/engine/anim/aseg.cpp) | `ResolveAsegEventObject` | Event target resolution via PloFindSwObject (root-relative or world) | 0.30 | T2 | verified |
| [0x130488](../../src/game/engine/anim/aseg.cpp) | `PasegaApplyAseg` | ApplyAsegOvr without override | 0.80 | T1 | verified |
| [0x1305B8](../../src/game/engine/anim/aseg.cpp) | `ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA` |  |  | existing |  |
| [0x130B18](../../src/game/engine/anim/asega.cpp) | `RetractAsega` | Stops an application: unlink, hands-off release, notify, Decref | 0.85 | T1 | verified |
| [0x130D30](../../src/game/engine/anim/asega.cpp) | `SeekAsega` | Seek to (SEEK kind, t, svt) then UpdateAsegaIeaCur | 0.90 | T1 |  |
| [0x131290](../../src/game/engine/anim/asega.cpp) | `HandleAsegaEvent` | Executes one ASEG event (sound, rumble 0x187E28, hands-off, apply/seek/retract other asegs, Splice, emitters...) | 0.80 | T1 |  |
| [0x131990](../../src/game/engine/anim/asega.cpp) | `SetAsegaHandsOff` | Toggles the ASEGA's hands-off flag (+0x58) with Increment/DecrementSwHandsOff | 0.85 | T1 |  |
| [0x131A20](../../src/game/engine/anim/asega.cpp) | `SetAsegaMasterSpeed` | +0x48 = g; effective speed +0x40 = speed*master*k | 0.85 | T1 |  |
| [0x131A90](../../src/game/engine/anim/asega.cpp) | `SetAsegaChannelWeight` | For channels of kind 7 that are live: kick blend state 9 and store g | 0.30 | T2 | verified |
| [0x131D68](../../src/game/engine/anim/asega.cpp) | `GetActsegGoal` | Per-frame channel time wrap (0x1317D8) and virtual update | 0.80 | T1 | verified |
| [0x137A30](../../src/game/engine/anim/blend.cpp) | `EnsureAsegBlendDynamic` | Create/extend the ASEGBL blending a set of asegs (cid 0x6E) and its MRSG tables | 0.90 | T1 | verified |
| [0x1383F0](../../src/game/engine/anim/blend.cpp) | `SetAsegblDominantBl` | When the index of the last bl with weight > 0.9999 changes, store it (+0xD4) and reset the damping of every live ACT on the blended aseg channels | 0.85 | T1 | verified |
| [0x1384C0](../../src/game/engine/anim/blend.cpp) | `ReblendAsegbl` | Normalise asega local times by tMax, recompute blend MRSG tables (CalculateBlendAmrsg per mrsgc), rescale times; mirror times from a linked asegbl (+0 | 0.85 | T1 | verified |
| [0x138970](../../src/game/engine/anim/blend.cpp) | `EvaluateBlendChnPosVel` | Blended-channel actuator virtual (p, ppos, prot, t, rate): weighted sum of each bl channel's position/rotation at time t (MRSG-mapped) | 0.60 | T1 | verified |
| [0x186670](../../src/game/engine/anim/ik.cpp) | `LoadSoIkFromBrx` | Read N IK records (kind 0x10, 0xE0 bytes: two bones + parent + vectors), init each (0x186930), create actuators (PactNewC); set SO flag bit 36 | 0.75 | T1 | parse |
| [0x1EFAC0](../../src/game/engine/anim/sm.cpp) | `LoadSmFromBrx` | Reads csms (U8) states {oid, oidNext}, csmt (U8) transitions {from, to (via IsmsFindSmRequired), grfsmt, grfsmtBreak, gProbability}, then builds table | 0.90 | T1 | parse, verified |
| [0x1F00C8](../../src/game/engine/anim/sm.cpp) | `PostSmLoad__FP2SM` | PostLoLoad; if a pending apply record exists, find its root ALO by OID and PsmaApplySm | 0.85 | existing |  |
| [0x1F0380](../../src/game/engine/anim/sm.cpp) | `PsmaApplySm` | Wrapper returning the SMA (kind 4) | 0.85 | T1 |  |
| [0x1F0438](../../src/game/engine/anim/sm.cpp) | `IsmsFindSmRequired` | Optional lookup clamped to ≥0 (28 B) | 0.85 | T1 |  |
| [0x1F04F8](../../src/game/engine/anim/sm.cpp) | `OidFromSmIsms` | asms[isms].oid, or -1 if out of range | 0.85 | T1 |  |
| [0x1F0528](../../src/game/engine/anim/sm.cpp) | `IncrefSma` | cref(+0x10)++ | 0.85 | T1 |  |
| [0x1F0538](../../src/game/engine/anim/sm.cpp) | `DecrefSma` | --cref; at 0: dtor, clear, unlink, free to pool D_003E1278 | 0.85 | T1 |  |
| [0x1F0598](../../src/game/engine/anim/sm.cpp) | `RetractSma` | If live: drop the current ASEGA, unlink from the SM list and the live list D_003E126C, send message 3, Decref | 0.85 | T1 |  |
| [0x1F0660](../../src/game/engine/anim/sm.cpp) | `SetSmaGoal` | ismsGoal = (oid==-1 ? -1 : IsmsFindSmRequired); if changed: retract the running ASEGA when the transition allows a break (flag 2), else choose a trans | 0.85 | T1 |  |
| [0x1F07C8](../../src/game/engine/anim/sm.cpp) | `GetSmaGoal` | OidFromSmIsms(psm, ismsGoal +0x3C) | 0.85 | T1 |  |
| [0x1F07E8](../../src/game/engine/anim/sm.cpp) | `GetSmaCur` | Same for ismsCur (+0x34) | 0.85 | T1 |  |
| [0x20BD78](../../src/game/gameplay/player/tu_20B840.cpp) | `InitAsegArc` | *p = -1 | 0.30 | T2 | verified |
| [0x20BF80](../../src/game/gameplay/player/tu_20B840.cpp) | `StartLeap` | construct CID 0x91 leap in place and enter state 0 | 0.85 | T1 | verified |
| [0x20CAA8](../../src/game/gameplay/player/tu_20B840.cpp) | `HandleDirAnimMessage` | CID 0x92 slot5: labels 0x17D/0x17E fForward, 0x17F/0x180 fBackward | 0.85 | T2 | verified |
| [0x21C888](../../src/game/engine/anim/tail.cpp) | `LoadSoChainsFromBrx` | reads N chains: per chain S8 count, U8, 4 floats, 2 vectors, S16 count of links; per link resolves the XF via vt slot51, creates a kind-3 hook (vt D_0 | 0.90 | T1 | parse, verified |
| [0x22B7D8](../../src/game/engine/anim/wr.cpp) | `UpdateWrStateVectors` | per WRBG vertex weights + state vectors | 0.75 | T1 | verified |
| [0x22C570](../../src/game/engine/anim/wr.cpp) | `GetWrWavelength` | 1/circle frequency (0 if 0) | 0.85 | T1 | verified |
| [0x22D348](../../src/game/engine/anim/xf.cpp) | `GetXfVelocity` | linear velocity of an XF | 0.70 | T1 | runtime |
| [0x22D7A0](../../src/game/engine/anim/xf.cpp) | `LoadSoXfsFromBrx` | reads XF records and optional animation jobs (incl. chains 0x21C888) | 0.75 | T1 | parse |
| [0x22FCC8](../../src/game/engine/anim/xf.cpp) | `ConvertXfVec` | transforms a direction between XF spaces | 0.85 | T1 | verified |
| [0x230380](../../src/game/engine/anim/xf.cpp) | `FXfRoleVelocity` | role predicate {1,2,3} | 0.30 | T2 | verified |
| [0x230598](../../src/game/engine/anim/xf.cpp) | `SlotUnsettle` | Move an XF slot from an at-rest role to its moving counterpart (3->1, 6/10->7, 12->11, 19->18) via SlotSetRole | 0.70 | T1 | verified, top50; top50 round; v2 name StopXfMotion |

Selected 60 of 267 named functions (T1 with confidence >= 0.75, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/animation.md](../reference/functions/animation.md).

## Open questions
- Which curve class is position/rotation/scale/float and which actuator class is which (break 0x12B3B0 at load and log the kind).
- ASEGA event kinds 13-16 and 20 (callees 0x1F6978..0x1F6B48 unnamed). Experiment: break 0x131290 and log the kind byte.
- File names of the two XFM-child TUs between act and aseg (0x12E600 camera XFM, 0x12EB48 CID 0x2B "amb?" with sound start/stop in OnAdd/OnRemove).
