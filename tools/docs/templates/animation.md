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

{{TUS}}

## 7. Key functions

{{KEYFUNCS:0x1305B8 0x131290 0x137A30 0x1F00C8 max=60 minc=0.75}}

## Open questions
- Which curve class is position/rotation/scale/float and which actuator class is which (break 0x12B3B0 at load and log the kind).
- ASEGA event kinds 13-16 and 20 (callees 0x1F6978..0x1F6B48 unnamed). Experiment: break 0x131290 and log the kind byte.
- File names of the two XFM-child TUs between act and aseg (0x12E600 camera XFM, 0x12EB48 CID 0x2B "amb?" with sound start/stop in OnAdd/OnRemove).
