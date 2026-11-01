# Player characters: PO, STEP, STEPPLAYER, Sly (JT), Bentley, Murray

Related: [guards-ai](guards-ai.md) (shared STEP base and BHV stack), [animation](animation.md), [camera](camera.md),
[save-memcard](save-memcard.md) (hp/mp/gadgets as save variables).

## 1. Class chain

`SO (0x30) -> PO (0x32, 0x180) -> STEP (0x33, 0x2C0) -> STEPPLAYER (0x34, 0x1320, abstract) -> JT (0x35, 0x2060) / MURRAY (0x36, 0x1B00) / BENTLEY (0x37, 0x1880)` `[C:runtime]` (descriptor table).
STEPPLAYER is new in Sly 2: Sly 1's JT sub-structs were lifted into a shared base so all three characters run the same
movement components; Sly 1 Murray was a guard subclass `[C:static types/README]`.
Player globals `[C:runtime]` for g_pjt: `g_pjt` D_002E1E40 (set in InitJt; oid 7 = save var `oidPoCurrent`), `g_pmurray` D_002F7900,
Bentley D_002DD5BC `[H r4b places.md]`.

## 2. PO and POCTL (po.cpp, 0x1C3D70)

- SW keeps up to 16 POs (SW+0x3EB8), a current PO (+0x3EFC) and an override (+0x3F00). Switching calls `OnActive(0/1)` (PO slot 78)
  and publishes the save var `oidPoCurrent` `[H r5 po.md]`. `PpoCur` 0x1C4840 returns the current PO.
- Each PO lazily gets a **POCTL** (CID 0x89, 0xE0 bytes) at PO+0x150: filtered stick polar values, button bits (+0x30),
  press timestamps `atPressed[16]` (+0x88), state (-1 none, 0 idle, 1 active); `GrfPressedWithin`, `GrfConsumePressed` `[C:verified]` names, layout `[H r5 po.md]`.
- `FInvulnerablePo` 0x1C3E90 (slot 76) = cheat bit 1 || hands-off (exact Sly 1) `[H r5 po.md]`.
- PO virtual slots: 75 FTakeDamage, 76 FInvulnerable, 77 JthsCurrent, 78 OnActive, 79 SetCtlState, 80 FCanBeHealed, 81 AddHpMp `[H r2b stepplayer.md]` (slots 78/79/76 verified).

## 3. STEP (step.cpp, 0x203948)

Walking characters: heading/turn control (steer mode +0x1A4, radTarget +0x1A8, matTarget +0x1B0), ground contact (pxpBase +0x208),
gravity ratio (+0x260, option 225) and spin ratio (+0x1F0, option 224), and the **behaviour stack** BHVSTK at +0x26C shared
with guards `[H r7b step.md]`, BHVSTK `[C:runtime]`. Exact Sly 1 matches: UpdateStepPhys, SetStepSpinRatio, SetStepGravityRatio,
FixStepAngularVelocity, RotateStepToMat `[H r7b step.md]`. `PbhvPushStep` 0x205438 / `PbhvFindStep` 0x205458 wrap the BHVSTK `[H r6b bhvstk.md]`.
Fix: existing `SetStepSpinRatio__FP4STEPf` at 0x1B17B8 is a CID 0x60 setter, not STEP's.

## 4. STEPPLAYER (TU 0x13FF70-0x144638, file name unknown; Lilly: stepplayer_raw.c)

Embedded components (Lilly layout, confirmed by init/post-load/update calls) `[H r2b stepplayer.md]`:

| Offset | Component | TU | Role |
|---|---|---|---|
| +0x2C0 | JTAIR | air/jump 0x195950 | jumps (kinds 1 jump, 6 double, 7 jump-off, 0x11 wall run...), air steering, landing chooser, ledge grabs, stuck check |
| +0x4C0 | JTGRAB (hands at +0x900) | grab 0x191090 | grab candidates (HND, PIPE, RAIL, GRABSPOT classes), actions, hide/shimmy, private TN |
| +0x9E0 | JTPLACES | places 0x19F3E8 | MRU table of 32 recent safe footholds; `ReturnJtToSafePlace` 0x19F590 after falls |
| +0xDF0 | JTHURT (+0xDFC hpMax, +0xE00 php) | hurt 0x1A0B18 | damage intake (difficulty-scaled), knockback, death states 8-12, water |
| +0xF00 | JTMODE | mode 0x19CBF8 | look/recon mode (binocucom?), button 0x400, camera policy CPLOOK |
| +0xF50 | JTPIPE | jtpipe 0x19DE10 | pipe climbing (jts 8) |
| +0xFD0 | JTGOAL | jtgoal 0x144638 | walk-to-goal with anti-stuck warp (used by BHV168) |
| +0x11B0 | JTTHROW | throw 0x21F2E0 | carry, aim, throw arc |
| +0x1320 / +0x1C30 | GAIT blocks | gait 0x20B840 | walk/run blending, footsteps |

Systems owned by STEPPLAYER `[H r2b stepplayer.md]`:
- **HP/MP** bound to save vars `<char>.hpCur` / `<char>.mpCur` (vt103 returns the namespace: "sly", "murray", Bentley's string).
  JT+0xE00 points at `g_gs+0x50` = `sly.hpCur` (40), JT+0xDFC = 40 max, +0x1300 = 100 MP max `[C:runtime]`.
- **Gadgets**: 3 GADGETSLOTs at +0x1180 bound to buttons D_002C3258 = {4,1,2}, saved as `<char>.apukCur[0..2]`; MP costs
  table D_002BCD00; virtual hooks 99 FCanUseGadget, 100 StartGadget/UseGadget, 101 UpdateGadget, 102 StopGadget, 105 OnGadgetsChanged.
  Power-ups acquired: save var `powerUps.mppukfAcquired[43]` `[C:runtime]` (schema).
- **Interaction**: picks the nearest/best guard each frame (`UpdateStepplayerInteractCandidates` 0x140558 using
  `GuardEvalInteractMode` 0x208E70: 4 engaged, 3 pickpocket volume, 2 close, 1 in range) -> vt95 `SetPlayerInteractMode`.
- **Booty**: carries one booty object (+0x11A8), save vars `<char>.oidBooty`, `episode.<w>.level.<l>.booty.aoidAssembly[15]`, `loot.mplootkc[k]`.
- **AI control**: `UpdateStepplayer` calls the top BHV's slot 14 with the POCTL, so scripted/partner movement reuses the guard
  behaviour machinery (BHV168 walk-to-goal, 193 scripted go-to, 194 follow leader, 195-197 approach and interact, 198 Murray) `[H r2b player_bhv.md]`.

STEPPLAYER virtual slots beyond PO: 82-87 STEP-level hooks (partly unnamed), 88/89 `SetPlayerState`/`GetPlayerState` on a
generic 0..8 state that each character maps to its own states, 93 OnHeroDied (game event 0x10), 95 SetPlayerInteractMode,
96 GetStunHold, 97 GetCmFocusPos, 98 hands-off query, 99-102/105 gadgets, 103 save-var namespace, 104 Splice event 0x1A `[H r2b stepplayer.md]`.

## 5. Sly (JT, jt.cpp 0x188488 + component TUs)

- JT vtable 0x2D2E98 (107 slots); state `jts` at +0x18C8, sub-state `jtbs` +0x18D0, cane state `jtcs` +0x18D8 `[C:runtime]`
  (jts 0 standing, **11 mid-pickpocket**; jtbs/jtcs -1). `SetJtJts` 0x18C250 is the state switch `[C:verified]`.
- Generic player state 0..8 maps to jts 0, 2, 7, 4, 0x12, 0x13, 0x14, 0x17, 8 (stand, air, grab, hurt, scripted..., aim/throw, pipe) `[H r4 jt.md]`.
  jts 3 = hang from hook (hang TU), 8 = pipe, 11 = attach/pickpocket `[C:runtime]` for 11.
- `UpdateJt` 0x18BCE0 (slot 16) order: STEPPLAYER Update -> `UpdateJtPre` (gather grab candidates, mode input,
  `JtChooseGroundState`, per-jts dispatch to the component files) -> actuator/gait updates -> `UpdateJtStand` (foot IK:
  `RebuildJtXmg`, `FMatchJtXmg`, Sly 1 matches) -> smoothing -> cane -> rope/curve attach checks -> `UpdateJtLean` `[H r4 jt.md]`.
- Cane (cane TU 0x18E540): `SetJtJtcs` 0x18E540 with Sly 1's sweep transition table D_002BF448; Sly 2 adds combo states 12..23; target choice from the TARGET list D_003E13B4 `[H r4b cane.md]`.
- Gadgets: JT gadget records at +0x1FB0.. with ids 0x1A/0x25/0x26/0x29 `[C:runtime]`; `JT::UseGadget` 0x18DA48 includes a
  slow-motion branch (clock rate + sounds 0x66/0x67) undone by `EndGadget` for gadgets 0x1D/0x2A `[H r4 jt.md]`.
- Position = SO `pxfRoot`(+0x60)->matWorld row 3; JT mass 0.25, gravity z -2275 `[C:runtime]`.

## 6. Bentley (bentley.cpp 0x134648) and Murray (murray.cpp 0x1B4728)

- **Bentley** (vtable 0x2CB230): state machine at +0x13D0 with sub-machines (+0x16B0, aim +0x16BC, +0x1800), screen-ray aim point,
  throws real BOMBs cloned from templates and primed with `PrimeBomb(4.0)`; 6 gadget records at +0x1824 (0x136A28; existing name
  `CbFromSaak` is wrong) `[H r2b bentley.md]`.
- **Murray** (vtable 0x2D44C8, 23 own slots): 22-state machine `SetMurrayState` 0x1B6078: idle, move, jump, air slam (area zap),
  lift, carry, scripted struggle, throw, toss, strafe/lock-on, attack combo (anim events 0x240-0x242), power move (UseGadget 20),
  component states, JTTHROW aim `[H r5b murray.md]`. Power-ups 0x11/0x12 affect Murray.

## 7. Grabbable world objects

- **GRABSPOT** is a mix-in (`X, GRABSPOT` multiple inheritance) with a secondary 6-slot vtable (wrap range, closest param,
  grab position, score, sparkle position, ?). Classes: CID 0x0B PIPE, 0x0C, 0x0D RAIL, 0x13 HND (hook), 0x1B, 0x22 (pole), 0x23-0x26, 0x27, 0x58 (carryable) `[H r4b grabspot.md]`;
  PIPE/RAIL/HND identities `[H r5 rail_pipe.md, r4b hnd.md]`. OnAdd/OnRemove put them on per-class grab lists walked by `GatherJtGrabCandidates` 0x194A40 `[C:verified arbitration]`.
- Jump targets: JMT = CID 0x28 (list D_003E1420); mesh-surface classes CID 0x8/0x9 `[H r4b air.md, meshsurf.md]`.
- Recon photo targets: CID 0x2E with a selector at STEPPLAYER+0xF38 (binocucom photo, ~0.6 confidence) `[H r7c recon_prompt.md]`.
- A safehouse character selector (tu_1D7238, PO subclass storing self in D_003D4A54) `[H r6 small_1D57A8]`.

## 8. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x134648-0x1378D8 | bentley |  |
| 0x13FF70-0x144638 | stepplayer TU (b[r-u]*) | Lilly: stepplayer_raw.c |
| 0x144638-0x144B28 | jtgoal TU |  |
| 0x144B28-0x1471C0 | player BHV TU | BHV168, BHV193-198 |
| 0x182C70-0x1848A8 | grabspot TU |  |
| 0x184F50-0x185130 | hnd |  |
| 0x185130-0x186610 | tu_185130 (mesh surface cid 8/9) |  |
| 0x188488-0x18E540 | jt | was labelled joy |
| 0x18E540-0x18F908 | cane TU |  |
| 0x18F908-0x191090 | hang TU |  |
| 0x191090-0x195950 | grab TU |  |
| 0x195950-0x19CBF8 | air/jump TU |  |
| 0x19CBF8-0x19DE10 | mode TU |  |
| 0x19DE10-0x19F0A8 | jtpipe TU (pipe context) |  |
| 0x19F0A8-0x19F3E8 | tu_19F0A8 (global player/guard broadcasts) |  |
| 0x19F3E8-0x1A0300 | places TU |  |
| 0x1A0B18-0x1A2BD0 | hurt TU |  |
| 0x1B4728-0x1B9F80 | murray |  |
| 0x1C3390-0x1C3D70 | pipe (PIPE class) |  |
| 0x1C3D70-0x1C4E50 | po |  |
| 0x1CB908-0x1CBEB0 | rail |  |
| 0x1CC158-0x1CD5F0 | tu_1CC158 (jthands/reach) |  |
| 0x1D7238-0x1D7A30 | tu_1D7238 (character selector?) |  |
| 0x201D78-0x203948 | tu_201D78 (recon photo prompt) |  |
| 0x21F2E0-0x220D40 | throw? (JTTHROW) |  |

## 9. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x134648](../../src/hand/func_00134648.c) | `InitBentley` | BENTLEY vt7: InitStepplayer then Bentley defaults (camera offsets 45/125, two 0x20B840 sub-objects at +0x1320/+0x1600, gadget slot kinds 7/9/11 ...) | 0.85 | HYP |
| [0x1347A8](../../src/hand/func_001347A8.c) | `PostBentleyLoad` | BENTLEY vt15: PostLoadStepplayer, find child SO 0xB, bind anim blends, init crosshair/aim helper (0x136D28), AttachStepplayerToStartMarker(0x16), SetB | 0.80 | HYP |
| [0x134DA8](../../src/hand/func_00134DA8.c) | `UpdateBentley` | BENTLEY vt16: UpdateStepplayer, then (if in world) Bentley state machine and per-state actions | 0.80 | HYP |
| [0x136A28](../../src/hand/CbFromSaak__F4SAAK.c) | `CbFromSaak__F4SAAK` |  |  | existing; fix: PgadgetBentleyFromId (BENTLEY gadget-record lookup; not CbFromSaak) |
| [0x13FF70](../../src/hand/func_0013FF70.c) | `InitStepplayer` | STEPPLAYER vt7 Init: STEP Init (0x203948) then inits embedded components at +0x2C0,+0x4C0,+0x9E0,+0xDF0,+0xF00,+0x11B0,+0xF50,+0xFD0 and defaults (+0x | 0.70 | VERIFIED |
| [0x1406E8](../../src/hand/func_001406E8.c) | `STEPPLAYER::SetCtlState` | vt79: PO SetPoCtlState, then 0x1F82B8(p, n==1?0:-1, 0); on becoming current (n==1) clear the message list +0x26C (0x1EECD0) | 0.80 | HYP |
| [0x140F90](../../src/hand/func_00140F90.c) | `HandleStepplayerMessage` | STEPPLAYER vt5 message handler (msg 3 reset, msg 9 subkinds 0x15E/0x161/0x162/0x27B/0x27C/0x1C7/0x1C8, msgs 4/5/1/11) then forwards to 0x1EED78/0x1C3E | 0.65 | VERIFIED |
| [0x141A50](../../src/hand/func_00141A50.c) | `UpdateStepplayerGadgets` | Per frame for the 3 gadget slots (+0x1180, buttons D_002C3258 = 4,1,2): on press check vt99 FCanUseGadget and MP cost (D_002BCD00[id].xC vs MpCur) - e | 0.70 | HYP |
| [0x141C00](../../src/auto/func_00141C00.c) | `STEPPLAYER::FCanUseGadget` | vt99 default: returns 0 (no gadgets usable) | 0.85 | HYP |
| [0x1440D0](../../src/hand/func_001440D0.c) | `BindStepplayerHpMp` | Bind hp/mp to save vars hpCur/mpCur under vt103 root (+0xE00, +0x12FC); if unset or forced, reset to max | 0.75 | HYP |
| [0x182EB8](../../src/auto/func_00182EB8.c) | `OnCid12Add` | vt23: LO Unfreeze + append to grab list D_003E13F0 | 0.60 | VERIFIED (arbitration/conflict fix; was CID12::Unfreeze) |
| [0x182EF0](../../src/auto/func_00182EF0.c) | `OnCid12Remove` | vt22: LO Freeze + remove from D_003E13F0 | 0.60 | VERIFIED (arbitration/conflict fix; was CID12::Freeze) |
| [0x183370](../../src/auto/func_00183370.c) | `OnCid34Add` | vt23: Unfreeze + append to its grab list | 0.55 | VERIFIED (arbitration/conflict fix; was CID34::Unfreeze) |
| [0x1833A8](../../src/auto/func_001833A8.c) | `OnCid34Remove` | vt22: Freeze + remove from grab list | 0.55 | VERIFIED (arbitration/conflict fix; was CID34::Freeze) |
| [0x1836F8](../../src/auto/func_001836F8.c) | `OnCid38Add` | vt23: Unfreeze + append to grab list | 0.55 | VERIFIED (arbitration/conflict fix; was CID38::Unfreeze) |
| [0x183730](../../src/auto/func_00183730.c) | `OnCid38Remove` | vt22: Freeze + remove from grab list | 0.55 | VERIFIED (arbitration/conflict fix; was CID38::Freeze) |
| [0x183998](../../src/auto/func_00183998.c) | `OnCid88Add` | vt23: UnfreezeSo + append to grab list | 0.55 | VERIFIED (arbitration/conflict fix; was CID88::Unfreeze) |
| [0x1839D0](../../src/auto/func_001839D0.c) | `OnCid88Remove` | vt22: FreezeSo + remove from grab list | 0.55 | VERIFIED (arbitration/conflict fix; was CID88::Freeze) |
| [0x183FB8](../../src/auto/func_00183FB8.c) | `OnCid35Add` | vt23 (shared with CID 0x24): Unfreeze + list | 0.50 | VERIFIED (arbitration/conflict fix; was CID35::Unfreeze) |
| [0x183FF0](../../src/auto/func_00183FF0.c) | `OnCid35Remove` | vt22 (shared with 0x24/0x25): Freeze + list | 0.50 | VERIFIED (arbitration/conflict fix; was CID35::Freeze) |
| [0x1844A0](../../src/auto/func_001844A0.c) | `OnCid27Add` | vt23: Unfreeze + append to D_003E1438 | 0.55 | VERIFIED (arbitration/conflict fix; was CID27::Unfreeze) |
| [0x1844D8](../../src/auto/func_001844D8.c) | `OnCid27Remove` | vt22: Freeze + remove from D_003E1438 | 0.55 | VERIFIED (arbitration/conflict fix; was CID27::Freeze) |
| [0x188488](../../src/hand/func_00188488.c) | `InitJt` | JT vt7 Init: base InitStepPlayer 0x13FF70, defaults (x19AC=15, jts/jtbs fields -1, limits -100/100/50/-10/250, LM 1..4500), gadget records x1FB0..x1FD | 0.85 | VERIFIED |
| [0x188608](../../src/hand/func_00188608.c) | `PostJtLoad` | JT vt15 PostLoad: base 0x1400B8, BindLoOidTable, look up ~10 named child ALOs/actuators (0x1F4C58 by id+kind) into x10B8/x19C0..x19E0, sub-object post | 0.80 | HYP |
| [0x188F20](../../src/hand/func_00188F20.c) | `HandleJtMessage` | JT vt5 message handler: base STEPPLAYER 0x140F90 then switch msg (3 = SMA/ASEGA finished for x18E0/x1030 -> SetJtJts or 0x18E540; 9, ...) | 0.80 | VERIFIED |
| [0x189C40](../../src/hand/func_00189C40.cpp) | `RebuildJtXmg` | Foot placement for one foot: when foot clip changes rebuild XMG (sphere reachable by leg, vertical probe line) | 0.85 | VERIFIED |
| [0x18BCE0](../../src/hand/func_0018BCE0.c) | `UpdateJt` | JT vt16 Update: STEPPLAYER Update 0x1403D0, UpdateJtPre, actuator/aseg updates, UpdateJtStand, feet, rope/curve checks (SetJtJts 0x16/3/8), UpdateJtLe | 0.80 | HYP |
| [0x18C250](../../src/hand/func_0018C250.cpp) | `SetJtJts` | Set Sly's state (jts x18C8) and sub-state (jtbs x18D0): exit/enter logic for every state | 0.90 | VERIFIED |
| [0x18D760](../../src/hand/func_0018D760.cpp) | `JT::FCanUseGadget` | Can Sly use gadget id now (per-state allow lists; aiming state 0x17 allows only carried pocket item) | 0.90 | VERIFIED |
| [0x18E540](../../src/hand/func_0018E540.cpp) | `SetJtJtcs` | Set Sly's cane state (x18D8): exit old (remove sweep effect unless continuing, stop reach track), state 3 resolves to fore/back sweep 4/5 or 6/7, ente | 0.90 | HYP |
| [0x18F2B0](../../src/hand/func_0018F2B0.cpp) | `ChooseJtAttackTarget` | Search the target list D_003E13B4 (flags&mask) for the best TARGET within 300 of Sly's projected position (pos+v*dt + rotated offset) and heading cone | 0.80 | HYP |
| [0x18F560](../../src/hand/func_0018F560.c) | `ChooseJtSweepTarget` | ChooseJtAttackTarget(grf 2, dt 0.15, drad 3.15 if jts==0 else 1.6), blend u=clamp((z+50)/110) into the two sweep BLs, reblend, face the target when st | 0.85 | HYP |
| [0x18F688](../../src/hand/func_0018F688.c) | `ChooseJtRushTarget` | ChooseJtAttackTarget(grf 8, 0.25, 1.0) and turn toward the found target (atan2), FixStepAngularVelocity | 0.85 | HYP |
| [0x18F740](../../src/hand/func_0018F740.c) | `ChooseJtSmashTarget` | ChooseJtAttackTarget(grf 4, 0.25, 3.15) and face target | 0.85 | HYP |
| [0x1903B8](../../src/hand/func_001903B8.cpp) | `PresetJtAccelHang` | Add the hang force, then accelerate Sly's orientation toward tilt (toward hook) * spin(radTarget) with AccelSoTowardMat | 0.80 | HYP |
| [0x190BE0](../../src/hand/func_00190BE0.c) | `GetJtHangHeading` | *prad = atan2(m[0].y, m[0].x) of the current hook object's (x18C4) world matrix | 0.85 | HYP |
| [0x194A40](../../src/hand/func_00194A40.cpp) | `GatherJtGrabCandidates` | Every frame collect the player's grab candidates from the world grab lists selected by grf (HND, PIPE, CID27, HSHAPE, CID34/38/35, RAIL, CID39), refin | 0.80 | HYP |
| [0x199408](../../src/hand/FCheckJtSingleEdge__FP2JTP2SOP4EDGEP6VECTOR.cpp) | `FCheckJtSingleEdge` | (existing) For an edge between a walkable (n.z>0.7) and a wall face (n.z<0.55) compute the grab direction | 0.85 | HYP |
| [0x1997D8](../../src/hand/func_001997D8.cpp) | `FCheckJtEdgeNormal` | Sweep 11 probe segments around the edge point against every layer's shapes; true if >=2 hit (valid ledge normal) | 0.55 | VERIFIED |
| [0x1999D0](../../src/hand/func_001999D0.cpp) | `FCheckJtEdgeGrab` | Contact direction vs a shape feature: vertex -> sum FCheckJtSingleEdge over edges at the point (CpedgeFromGeomPoint, max 64) + BSP clip test; edge ->  | 0.70 | VERIFIED |
| [0x199D48](../../src/hand/func_00199D48.c) | `PxpFindJtBestEdge` | Pick the best supporting ledge contact of the player (walk contact groups, FCheckJtEdgeGrab, BSP point test), returning side and normal | 0.70 | VERIFIED |
| [0x19A098](../../src/hand/func_0019A098.cpp) | `FCheckJtStuck` | Return true if Sly is wedged: at most once per second and >=1 s after state change, solve contact normals against gravity (SolveInequalities) and test | 0.80 | HYP |
| [0x1B4728](../../src/hand/func_001B4728.c) | `InitMurray` | MURRAY vt7 Init: STEPPLAYER Init, defaults (x220=50, state fields -1, throw range 1500/3500), 4 locomotion blends (0x20B840 x1320/1460/1540/1630), JTT | 0.85 | HYP |
| [0x1B49E0](../../src/hand/func_001B49E0.c) | `PostMurrayLoad` | MURRAY vt15: STEPPLAYER PostLoad, post-load the 4 locomotion blends, strafe direction table (0x1A0300 x16F0/x17F0), JTTHROW blends (0x21F388), optiona | 0.80 | HYP |
| [0x1B50B0](../../src/hand/func_001B50B0.c) | `UpdateMurray` | MURRAY vt16 Update: STEPPLAYER Update, gather grab candidates (0x194A40 g_sw,p,0xF8FA), state machine, per-state locomotion (blends x1320/x1540 driven | 0.80 | HYP |
| [0x1B6078](../../src/hand/func_001B6078.cpp) | `SetMurrayState` | Murray state switch (x13D0, 0..21): exit work for old state, entry work (clip selection, velocity reset, component state), shared ApplyAsegCur/compone | 0.60 | HYP |
| [0x1B68B8](../../src/hand/func_001B68B8.c) | `HandleMurrayMessage` | MURRAY vt5: base HandleStepplayerMessage; msg 9 (anim events from own ASEGA): 0x1A4 flop dive, 0x1A5/0x1C8 stimulus, 0x1EB release thrown object, 0x1C | 0.80 | HYP |
| [0x1B98A0](../../src/hand/func_001B98A0.c) | `MURRAY::FCanUseGadget` | MURRAY vt99: can gadget/power k be used in the current state (per-state allow lists, components 2/13/20) | 0.80 | HYP |
| [0x1C3390](../../src/hand/func_001C3390.c) | `InitPipe` | PIPE::Init (vt7): InitShape + GRABSPOT init (+0x190); defaults | 0.85 | VERIFIED |
| [0x1C33E8](../../src/auto/func_001C33E8.c) | `OnPipeAdd` | PIPE vt23: base then append to pipe list D_003E13C0 | 0.75 | VERIFIED |
| [0x1C3420](../../src/auto/func_001C3420.c) | `OnPipeRemove` | PIPE vt22: base then remove from D_003E13C0 | 0.75 | VERIFIED |
| [0x1C3458](../../src/hand/func_001C3458.c) | `PostPipeLoad` | PIPE vt15: base PostLoad + default length | 0.80 | HYP |
| [0x1C34B8](../../src/hand/func_001C34B8.c) | `ClonePipe` | PIPE vt41 Clone | 0.80 | VERIFIED |
| [0x1C3D70](../../src/hand/func_001C3D70.c) | `InitPo` | PO::Init (vt slot 7): InitSo; pzi.fCollectEnabled(+0x15C)=1; pzi.paloCollect(+0x168)=this; set flag 2 in grfUnk40 | 0.88 | HYP |
| [0x1C3DC8](../../src/hand/func_001C3DC8.c) | `LoadPoFromBrx` | PO::LoadFromBrx (vt slot 14): LoadSoFromBrx (0x1F31E0) then one stream helper 0x118420 | 0.80 | HYP |
| [0x1C3E20](../../src/hand/func_001C3E20.c) | `HandlePoMessage` | PO::HandleMessage (vt slot 5): HandleLoMessage base, then on msgid 5 stop the wake ambient sound at +0x174 and release it | 0.88 | VERIFIED |
| [0x1C3E90](../../src/hand/func_001C3E90.c) | `FInvulnerablePo` | PO virtual slot 76: true if invulnerability cheat bit (D_002E1E00&1) or world hands-off (FSwHandsOff(&g_sw)) | 0.90 | HYP |
| [0x1C3ED8](../../src/hand/func_001C3ED8.c) | `SetPoCtlState` | PO virtual slot 79 (Lilly SetCtlState): on first use register PO in g_sw PO list (+0x3EB8, max 16) and allocate its POCTL (+0x150, 0xE0); state 1 make | 0.80 | HYP |
| [0x1C3FF0](../../src/hand/func_001C3FF0.c) | `OnPoActive` | PO virtual slot 78 (Lilly OnActive(int)): if PO has its own camera TN (+0x16C) (de)activate it, else SetCmPolicy / RevokeCmPolicy on g_cm | 0.85 | HYP |
| [0x1C40F8](../../src/auto/func_001C40F8.c) | `FIsPoActive` | True if PO == effective current PO (override +0x3F00 else +0x3EFC) | 0.40 | VERIFIED |
| [0x1C4228](../../src/hand/func_001C4228.c) | `AddPoHpMp` | Virtual slot 81 (Lilly AddHpMp(dhp,dmp)): forwards (dhp,dmp) as message 0xC through vt slot 3 | 0.55 | VERIFIED |
| [0x1C4280](../../src/hand/func_001C4280.c) | `POCTL::POCTL(PO*)` | POCTL ctor: vptr D_002CA240, Reset, ppo(+0x7C)=ppo, nState(+0x80)=-1, append to g_sw POCTL list D_003E133C | 0.85 | VERIFIED |
| [0x1C42E8](../../src/hand/func_001C42E8.c) | `POCTL::Reset` | Zero stick values (dx/dy/rad/s left+right), grfbtn=0, ClearJoyPressed(0xFFFF) | 0.60 | VERIFIED |
| [0x1C4468](../../src/hand/func_001C4468.c) | `POCTL::Update` | Per-frame controller update: state 0 reset; state 1 read g_joy (or alt joy D_002E0D00) sticks+buttons (grfbtn = held & ~mask \| ppo->grfbtnForce), buil | 0.75 | VERIFIED |
| [0x1C4660](../../src/hand/func_001C4660.c) | `POCTL::GrfPressedWithin` | Mask of buttons in grf pressed within the last dt seconds (t - atPressed[i] <= dt) | 0.85 | VERIFIED |
| [0x1C46C0](../../src/auto/func_001C46C0.c) | `POCTL::GrfConsumePressed` | GrfPressedWithin then ClearJoyPressed on the result (consume the press) | 0.85 | VERIFIED |
| [0x1C4738](../../src/hand/func_001C4738.c) | `POCTL::ClearJoyPressed` | Set atPressed[i] = -FLT_MAX for every bit in grf | 0.85 | HYP |
| [0x1C4840](../../src/auto/func_001C4840.c) | `PpoCur` | Returns g_sw current PO (SW+0x3EFC) | 0.80 | HYP |
| [0x1C4A48](../../src/auto/func_001C4A48.c) | `PpoActive` | Effective current PO: override SW+0x3F00 if set else SW+0x3EFC | 0.50 | VERIFIED |
| [0x1CB908](../../src/auto/func_001CB908.c) | `InitRail` | RAIL::Init (vt7): InitShape + init GRABSPOT base at +0x190; +0x1C4=1, +0x1C0=1000 | 0.85 | HYP |
| [0x1CB950](../../src/hand/func_001CB950.c) | `PostRailLoad` | RAIL vt15 PostLoad: base then if length (+0x38)==1e10 compute path length + margin | 0.80 | HYP |
| [0x1CB9B0](../../src/auto/func_001CB9B0.c) | `OnRailAdd` | RAIL vt23: LO base (0x1A6178) then append to global rail list D_003E13CC | 0.70 | VERIFIED |
| [0x1CB9E8](../../src/auto/func_001CB9E8.c) | `OnRailRemove` | RAIL vt22: LO base (0x1A6150) then remove from D_003E13CC | 0.70 | VERIFIED |
| [0x1CBA20](../../src/hand/func_001CBA20.c) | `CloneRail` | RAIL::Clone (vt41): CloneShape keeping own GRABSPOT link | 0.75 | VERIFIED |
| [0x1CC5E8](../../src/hand/func_001CC5E8.cpp) | `InitJtHands` | Build hands record: hand-frame matrix, sampled A/B curves per hold clip (PredictAsegEffect), blend | 0.85 | HYP |

Selected 75 of 646 named functions (confidence >= 0.8, verified, or flagged). Full list: [reference/functions/player.md](../reference/functions/player.md).

## Open questions
- Full Sly 2 JTS enum (build from SetJtJts's switch; log (a1,a2) at 0x18C250 while moving).
- Unnamed STEPPLAYER slots 82-87, 90-92; JT slots 49/50/55/57/85/90-98/103/106.
- Gadget record table D_002BCD00 layout; which gadget ids are which (slow motion = 0x1D/0x2A?).
- Is JTMODE kind 1 the binocucom recon mode? (break `StartJtMode` 0x19CC50).
- Which of CID 0x0B/0x13 is PIPE vs HND (cid_table guesses); map GRABSPOT CIDs to in-game objects.
