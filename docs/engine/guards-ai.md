# Guards and AI: STEPGUARD, behaviours, perception, pickpocketing, scripts, spawners

Sly 2 replaced Sly 1's per-guard SGS state machine with a **stack of behaviour objects** (BHV) per character, plus a
guard class that hosts perception, targeting, flags and engagement rules. This page describes that design.
Related: [player](player.md) (STEP and the same BHV stack drive partners), [game-progression](game-progression.md) (game events).

## 1. STEPGUARD (stepguard.cpp, 0x205E70-0x20B840)

- CID 0x38, 0x1010 bytes, vtable 0x2DA568 (89 slots), 11 subclasses (0x39 GOMER .. 0x43) `[C:runtime]` for class/vtable.
  Only 2 new virtuals: vt88 `PloChooseStepguardTarget`, vt89 `FStepguardSenseStimulus` `[H r7b stepguard.md]`.
- Paris hub: 9 guard objects (GOMER 5, CID 0x3A x2, 0x3B, 0x3C), **only 3 live at a time**; they are pooled and re-placed as Sly moves (OnAdd/OnRemove by proximity) `[C:runtime]`.
- The guard list D_003E1444 (DL, link at guard+0xFE0) holds exactly the live guards; STEPGUARD OnAdd/OnRemove (0x2079E0/0x2079A0) maintain it `[C:runtime]`, `[C:verified arbitration]`.
- Guard manager CID 0x7D (singleton, oid 0x656, `UpdateCid7DGuardMgr` 0x20B590) removes or respawns guards that have been
  inactive for more than 0.25 s while still flagged in-world, depending on guard+0x1000 `[C:runtime]` singleton, behaviour `[H r7b]`.

### The guard flag word (+0x2C0): Sly 2's replacement for SGS
- 64-bit word, flag n = bit n (0x207A20 returns `1<<n`) `[C:runtime]`.
- BHVs raise/lower their own flag in Init/Remove (`AddStepguardSgf` 0x209890 / `RemoveStepguardSgf` 0x209970, 23 callers each);
  every change sends message 7 to the guard; flag 3 broadcasts game event 0x14, flag 0xC event 0x16 `[H r7b]`.
- **At runtime the word always equals the union of the flags of the BHVs on the guard's stack** `[C:runtime]`. Static map:
  BHV170 -> 0x1E, BHV169 -> 0x21, BHV192 -> 0x2E, BHV190 -> 0x2C; by elimination BHV158 -> 2, BHV172 -> 0xF, BHV157 or BHV167 -> 9.
- Flag meanings from code `[H r7b]`: 7 dead/KO, 5/6 damaged, 0x17 stunned/carriable (Murray takedown), 0x12/0x13/0x16 held/thrown.
  The "alert" flags 0x1D/0x20/0x2A were **not observed** at runtime (INCONCLUSIVE).

### Perception
- **Vision**: each guard owns a detector (CID 0x10, 0x180 bytes) at +0xF64; det+0xE0 = owner, det+0x100.. = up to 8 tracked
  targets {id, state, t, tFirst} with states 0 none / 1 seen / 2 lost, computed from inner/outer volumes, BSP line of sight
  and hit tests; state changes send message 0x1C; the player becoming seen broadcasts game event 0x13 `[C:runtime]` for ownership/JT target, logic `[H r3 detect.md]`.
  `UpdateStepguardDetectTargets` 0x206900 feeds it each frame with watched-class objects within 1500 units; the player characters are permanent targets.
- **Hearing/stimuli**: refcounted stimulus objects (CID 0x93, 0x60 bytes) from 7 SW templates (CID 0x75, oids 0x2C2..0x2C8),
  active list D_003E148C; listeners CID 0x2F; vt89 judges them, `PstimChooseStepguard` 0x20A408 ranks `[H r3 noise.md, r3b misc_small.md]` ("noise" is a hypothesis).
- `FStepguardSensesEnabled` 0x208D70 disables both in KO/stunned/held states `[H r7b]`.

### Targeting and engagement
- vt88 picks a target: forced target (+0x390, option 260), then force/ignore/prefer class lists, with hysteresis `[H r7b]`.
- Each target has a shared engagement record (0xAC bytes, list D_003E145C). `FTargetRecordAllowsGuard` 0x20A818 lets only one engaged
  guard be "busy" at a time and, in camera mode, only inside the frustum: **"guards take turns attacking, on screen"**. Patch point for mods `[H r7b]`.
- Attacks are CID 0x78 objects (list +0xEB8/+0xEDC; range LM [0,300], cooldown, angle tolerance) `[C:runtime]` for objects/values (+0xF48/F4C = 300/1200, +0xF50.. = ±45°/±75°).
- Special/gadget states (+0xFBC, `SetStepguardSpecial` 0x207030): shrink, time freeze, rage (guards target guards), with timeouts 10/30/5 s; gadget identities are a hypothesis `[H r7b]`; +0xFBC = -1 at runtime (no gadget) `[C:runtime]`.
- Damage: vt75 `FTakeStepguardDamage`, hit value +0x338 (40 for GOMER) `[C:runtime]` value.
- Animation bindings: up to 32 records at +0x4D0 plus three 26-entry zap-kind tables mapping flag sets to ASEG/SM/payloads, chosen by `PanimbindBest` 0x209AD0 `[H r7b]`.
- REFUTED: +0x2E8 is not "mode"; it points to a CID 0x71 object (GOMER) `[C:runtime]`. +0x2EC = CID 0x71 oid 0x2CB in all guards `[C:runtime]`.

## 2. The behaviour stack (bhvstk TU 0x1EEC60)

- Every STEP embeds `BHVSTK {STEP *pstep; int cpbhv; BHV *apbhv[8]}` at +0x26C; slots are 0xF0 bytes apart; the guard's root
  behaviour is BHV156 (CID 0x9C) `[C:runtime]`.
- Lifecycle `[H r6b bhvstk.md]`: STEP Init -> `InitBhvstk`; Clone -> `CloneBhvstk`; PostLoad -> 8 slots allocated; every frame
  `UpdateBhvstk` (top first); messages `HandleBhvstkMessage` (top first, stop at the first handler). Behaviours are
  placement-new'd by a CID factory (`PbhvNew`, CIDs 0x9C-0xC6) and linked prev/next; popping one pops everything above.
- **Busy protocol**: a busy behaviour increments `cBusyAbove` of all lower ones (and the guard's busy count +0xFFC); lower
  behaviours' Update checks it `[C:runtime]` (S05: BHV190/172 busy, lower cBusyAbove = 2, guard+0xFFC = 2).
- BHV (CID 0x9B, 0x30 base: +0x10 pstep, +0x14 prev, +0x18 next, +0x1C fBusy, +0x20 cBusyAbove) has its **own** slot meanings
  (not LO's): 7 Init, 8 Start, 9 Close/pop, 10 Update, 11 UpdateMotion?, 12 GrfGroundProbe, 13 FHandleMessage, 14 UpdateInput? `[H r6b bhvstk.md]` (8, 11, 12, 14 are guesses).

Observed stacks `[C:runtime]`:

| Situation | Stack (bottom -> top) | Flags |
|---|---|---|
| patrol (default) | 156, 170, 169, 192, 186, 167, 157 | bits 9, 30, 33, 46 |
| guard near Sly right after a pickpocket (S02) | 156, 170, 158 | bits 2, 30 |
| two guards reacting to the clue-bottle pickup (S05) | 156, 170, 169, 192, 186, **190**, **172** (busy) | bits 15, 30, 33, 44, 46 |

Behaviour classes with known roles (most names unknown) `[H]`: BHV156 root; BHV169 guard-script runner; BHV171 search (react,
go to last known position, look, visit 4 search points, give up); BHV172/190 reaction to a disturbance (runtime); BHV180/181
flee logic of CID 0x3B; BHV182 keep pace along a route (tail missions?); BHV174 CID 0x3C watch/alarm; BHV164 hide; BHV165, BHV166-168
path following family; BHV176/177 held/stunned; BHV168, 193-198 player behaviours. Method map of ~30 classes: r3 guard_behaviours.md
(lists slot -> function; BHV TU 0x174898-0x17F090 is still unnamed).

## 3. Guard scripts and spawners

- **Guard script** (CID 0x71, tu_1709A8): a list of 0x1C-byte commands built from Splice (16 appenders): set context, teleport,
  message, wait, go to target (path/rail/pipe aware), look/move, play animation, label/jump, busy on/off, push BHV172/BHV157, ensure relay alarm.
  BHV169 executes them; the root BHV156 runs a script in mode 5 (`StartGuardScript` 0x171CF8) `[H r3b guard_script_spawner.md]`.
- **Off-screen spawner** (CID 0x77, tu_173158): picks spawn points that are concealed (frustum edge, out of range, occluded),
  activates a template guard there with a goto script; re-armed after 30/50 s; gated by the last alert event. Probably reinforcements `[H r3b]`.
- **Fleeing guard** (CID 0x3B + BHV180/181) runs away along routes from the player (thief/civilian?) `[H r3b]`.
- **Watchers** (tu_20CE28): stuck watch (zap kind 0x4000 if all probes blocked) and hazard watch (zap if falling inside a hazard) `[H r7c guard_watch.md]`.

## 4. Pickpocketing (pickpocket TU 0x1F1768)

- Each guard carries a loot list: 16-byte entries `{oid, flags, key, count}` at +0x404, count +0x484, next index +0x488 `[C:runtime]`.
  Coin entries are oid 0x2B3 with a count; loot entries oid 0x2B5 with key 0x45A+k; **key 0x45A+k maps to save var `loot.mplootkc[k]`** `[C:runtime]` (the guard robbed between S02 and S03 lost its (0x2B5, 0x45E) entry and `mplootkc[4]` went 0 -> 1).
- `GenerateStepguardLoot` rolls 2-3 coin entries plus maybe one weighted loot item from per-world tables at PostLoad; a pouch object
  (CID 0x63, an SO child; REFUTED "pouch PO") launches loot toward Sly; glints show the state (alerted, coins, loot) `[H r6b pickpocket.md]`, pouch class `[C:runtime]`.
- `PickpocketStepguard` is fired from JT's message handler and broadcasts game event 0x19 `[H r6b]`. Sly's jts is 11 while pickpocketing `[C:runtime]`.

## 5. STEP class reminders
STEP (CID 0x33) is the shared base of players and guards; see [player.md](player.md#3-step-stepcpp-0x203948). Guard-specific
steering helper ("mover") at STEPGUARD+0xFD0 used by BHVs `[H r7b]`.

## 6. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x14A008-0x14B660 | route/chase TU |  |
| 0x14FAA8-0x14FCA8 | tu_14FAC8 (cid 0x40 guard) | starts at 0x14FAC8 |
| 0x153D58-0x154A60 | detect? | detector cid 0x10 |
| 0x1553F0-0x1564E8 | tu_1553F0 (cid 0x3B fleeing guard) |  |
| 0x1564E8-0x157200 | noise? (stimuli) |  |
| 0x16F148-0x16F2D0 | gomer |  |
| 0x1709A8-0x171F58 | tu_1709A8 (guard script cid 0x71) |  |
| 0x171F58-0x173158 | tu_171F58 (BHV171 search) |  |
| 0x173158-0x174898 | tu_173158 (cid 0x77 spawner) |  |
| 0x174898-0x17F090 | BHV classes TU | was labelled stepguard |
| 0x1B9F80-0x1BAB50 | tu_1B9F80 (cid 0x3C guard + BHV174) |  |
| 0x1CBEB0-0x1CC158 | tu_1CBEB0 (cid 0x3D guard) |  |
| 0x1EEC60-0x1EF850 | bhvstk TU (BHV base) |  |
| 0x1F1768-0x1F2108 | pickpocket TU |  |
| 0x203948-0x205E70 | step |  |
| 0x205E70-0x20B840 | stepguard |  |
| 0x20CE28-0x20D5D0 | tu_20CE28 (guard watchers) |  |

## 7. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x153D58](../../src/hand/func_00153D58.c) | `InitCid10` | XFM::Init (0x231100), registers for update (0x1A6CA8(p,1)), sets flag 2 | 0.75 | PLAUSIBLE |
| [0x154080](../../src/hand/func_00154080.c) | `SetDetectorTargetState` | Changes a target entry state: records times, notifies the detector through its SendMessage virtual (msg 0x1C, args entry/old/new) and, when a player-c | 0.60 | VERIFIED |
| [0x154158](../../src/hand/X509v3_data_type_by_NID.c) | `X509v3_data_type_by_NID` |  |  | existing; fix: FindTargetState (e.g. NGetSensorTargetState) |
| [0x1564E8](../../src/hand/func_001564E8.c) | `InitCid75` | LO::Init (0x1A5270) then defaults g54/lm/g60=0.25 and flag 0x40 | 0.75 | HYP |
| [0x156CC8](../../src/hand/func_00156CC8.c) | `InitCid2F` | XFM::Init then 0x1A6CA8(p,0), flag 2 | 0.75 | HYP |
| [0x16F148](../../src/hand/RenderGomerSelf__FP5GOMERP2CMP2RO.cpp) | `InitGomer` | GOMER::Init: calls STEPGUARD::Init (0x205EB8) then sets a GOMER flag at +0xE78 | 0.75 | HYP |
| [0x1709F8](../../src/auto/func_001709F8.c) | `InitCid71` | cid 0x71 (LO child, 0x60) Init: LO Init, then InitDl of the command list at +0x50 (link +0x18) | 0.75 | PLAUSIBLE |
| [0x173158](../../src/hand/func_00173158.c) | `InitCid77` | cid 0x77 (LO child, 0xB0) Init: LO Init, defaults 5500/1/1/1, respawn delays 30 and 50 s | 0.75 | HYP |
| [0x1731B8](../../src/hand/func_001731B8.c) | `LoadCid77FromBrx` | Preallocates spawn-point (0x1C each), template and offset arrays, LO load, shrinks | 0.70 | HYP |
| [0x1EEC60](../../src/auto/func_001EEC60.c) | `InitBhvstk` | BHVSTK init: pbhvstk->pstep = pstep | 0.80 | HYP |
| [0x1EEC70](../../src/hand/func_001EEC70.c) | `PostBhvstkLoad` | Allocate the 8 BHV slots (CbBhvMax()=0xF0 bytes each, align 0x10) from the world heap into apbhv[0..7] | 0.75 | HYP |
| [0x1EECD0](../../src/hand/func_001EECD0.c) | `ClearBhvstk` | Pop every behaviour: if cpbhv>0 pop apbhv[0] (which pops everything stacked above it), then cpbhv=0 | 0.85 | HYP |
| [0x1EED10](../../src/hand/func_001EED10.c) | `UpdateBhvstk` | Call BHV vt10 (Update) on every stacked behaviour, top (last pushed) first | 0.80 | HYP |
| [0x1EED78](../../src/hand/func_001EED78.c) | `HandleBhvstkMessage` | Offer a step message (msgid,pv) to the behaviours, top first, until one's vt13 returns nonzero (consumed) | 0.85 | HYP |
| [0x1EEE00](../../src/hand/func_001EEE00.c) | `PbhvPushBhvstk` | Push a new behaviour of class cid: take slot apbhv[cpbhv++], memset CbBhvMax, construct via PbhvNew, set pstep and pbhvPrev (prev->pbhvNext=new), call | 0.85 | HYP |
| [0x1EEEE0](../../src/hand/func_001EEEE0.c) | `PbhvFindBhvstk` | Return the first stacked behaviour (bottom first) derived from cid, else NULL | 0.85 | HYP |
| [0x1EEF68](../../src/auto/func_001EEF68.c) | `BHV::Init (InitBhv)` | BHV vt7 base: empty. Called by PbhvPushBhvstk right after construction | 0.70 | HYP |
| [0x1EEF98](../../src/auto/func_001EEF98.c) | `BHV::Update (UpdateBhv)` | BHV vt10 base: empty per-frame update | 0.70 | HYP |
| [0x1EF0B8](../../src/auto/func_001EF0B8.c) | `BHV::FHandleMessage (FHandleBhvMessage)` | BHV vt13 base: return 0 (message not consumed) | 0.75 | HYP |
| [0x1EF0D8](../../src/hand/func_001EF0D8.c) | `PopBhv` | Given a BHV* slot: if non-null call its vt9 (Close), decrement its step's cpbhv, null the slot | 0.70 | HYP |
| [0x1EF138](../../src/hand/func_001EF138.c) | `PbhvNew` | Placement factory: switch(cid 0x9C..0xC6) memset size, set BASIC header + vtable for that BHV subclass | 0.75 | HYP |
| [0x203948](../../src/hand/func_00203948.c) | `InitStep` | STEP vt7 Init: InitPo, rGravity(0x260)=1, SetStepSpinRatio(1), SetStepRadTarget(0), base radius 0x220=30, 0x214=-50, InitBhvstk(+0x26C,this) | 0.85 | HYP |
| [0x2039C0](../../src/auto/func_002039C0.c) | `LoadStepFromBrx` | STEP vt14: LoadPoFromBrx, cache local base pos (+0x240 = ConvertAloPos(parent,this,0)), 0x1F8300 | 0.80 | HYP |
| [0x203A08](../../src/auto/func_00203A08.c) | `CloneStep` | STEP vt41 Clone: ClonePo then re-point the BHVSTK owner (+0x26C) at the clone | 0.80 | HYP |
| [0x203A38](../../src/hand/func_00203A38.c) | `PostStepLoad` | STEP vt15: PostSoLoad, alloc 0x294, attach two child objects (oids +0x58/+0x5C), find phys SOPART by oid +0x264 -> CloneStepPhys + hide it, allocate t | 0.80 | HYP |
| [0x203AD0](../../src/auto/func_00203AD0.c) | `RenderStepSelf` | STEP vt19 RenderSelf: just RenderSoSelf | 0.75 | HYP |
| [0x203E08](../../src/hand/func_00203E08.c) | `UpdateStepPhys` | Per point: smooth the body shape toward the phys shape with GSmooth(old,new,dt,s_smpPhys D_002C3C68) and store (new-old)/dt as point velocity | 0.80 | HYP |
| [0x203FC0](../../src/hand/func_00203FC0.c) | `UpdateStep` | STEP vt16: UpdateSo, UpdateStepPhys, then turn-rate speed factor +0x180 (CLQ D_002C3C80 of \|radTarget-heading\| clamped 0..1, or 1), stamp +0x20C while | 0.85 | HYP |
| [0x2041F8](../../src/auto/func_002041F8.c) | `RotateStepToMat` | STEP vt30: RotateSoToMat then radTarget = current heading (SetStepRadTarget) and POCTL+0x18 = heading | 0.75 | HYP |
| [0x204CA8](../../src/hand/func_00204CA8.c) | `FixStepAngularVelocity` | RadSmooth(heading, radTarget, 0, &smpSpin, &w.z) then SetAngularVelocity (vt36) | 0.80 | HYP |
| [0x204D48](../../src/hand/func_00204D48.c) | `SetStepSpinRatio` | smpSpin = default D_002C3CB8 with svFast/svSlow *r and dtFast /sqrt(r); store r at +0x1F0 | 0.85 | HYP |
| [0x204DA8](../../src/hand/func_00204DA8.cpp) | `SetStepGravityRatio` | Clamp r>=0.1, SetSoMass(m*rOld/r), scale gravity vector by r/rOld, store rGravity +0x260 | 0.90 | HYP |
| [0x205EB8](../../src/hand/func_00205EB8.c) | `InitStepguard` | STEPGUARD vt7 Init: InitStep, ensure guard manager, create the vision detector (PloNew cid 0x10 oid 0x288 -> +0xF64), default ranges/angles (+-45deg,  | 0.85 | HYP |
| [0x206100](../../src/auto/func_00206100.c) | `CloneStepguard` | STEPGUARD vt41: CloneStep keeping own detector, re-own mover, copy detector state through its vt (slot 41), deep-copy bindings (0x208250) | 0.80 | HYP |
| [0x2061D8](../../src/hand/func_002061D8.c) | `PostStepguardLoad` | STEPGUARD vt15: PostStepLoad, bind oid table, SetStepGravityRatio(+0x260), attach children, resolve anim bindings (0x2080F0) and attack oids (0x20AAA8 | 0.80 | HYP |
| [0x206900](../../src/hand/func_00206900.cpp) | `UpdateStepguardDetectTargets` | Unless in a blind state, collect objects of the watched classes (+0x394) within 1500 xy, sort by distance, re-add them as detector targets keeping pre | 0.55 | HYP |
| [0x207030](../../src/hand/func_00207030.c) | `SetStepguardSpecial` | Gadget/status override state +0xFBC (time +0xFC0): 8/0xD shrink (scale act -> 0.5), 0xE time-frozen (BHV157 ASEGA speed 0, velocity 0), 0x22/0x25 rage | 0.50 | HYP |
| [0x207350](../../src/hand/func_00207350.c) | `UpdateStepguard` | STEPGUARD vt16: UpdateStep; if in world and not time-frozen: special timeouts, detect targets, finish attack anim, aim, pickpocket, update components, | 0.85 | HYP |
| [0x2075D0](../../src/hand/func_002075D0.c) | `HandleStepguardMessage` | STEPGUARD vt5: HandlePoMessage; msgs 4/5 attach/detach an object on component +0x49C; forward every message to the BHV stack (0x1EED78) | 0.75 | HYP |
| [0x207690](../../src/hand/func_00207690.cpp) | `RenderStepguardSelf` | STEPGUARD vt19: draw the globset; when the camera policy is active, apply a 0.85 s squash/stretch pulse after +0x360 (pivot +0x240) and distance-based | 0.75 | HYP |
| [0x2079A0](../../src/hand/func_002079A0.c) | `OnStepguardRemove` | STEPGUARD vt22: base (0x1F3E00), unlink from global guard list D_003E1444, stamp +0xFF8 | 0.50 | VERIFIED (arbitration/conflict fix; was OnStepguardRemoveFromWorld) |
| [0x2079E0](../../src/hand/func_002079E0.c) | `OnStepguardAdd` | STEPGUARD vt23: base (0x1F3EA8), append to guard list D_003E1444, +0xFF8 = FLT_MAX | 0.50 | VERIFIED (arbitration/conflict fix; was OnStepguardAddToWorld) |
| [0x208E70](../../src/hand/func_00208E70.c) | `GuardEvalInteractMode` | Rank how the hero can interact with this guard: 4 engaged with hero, 3 hero in pickpocket glint volume (+0x3F4/+0x3F8), 2 close (d<=0.7*f378, dz in LM | 0.55 | HYP |
| [0x209890](../../src/hand/func_00209890.c) | `AddStepguardSgf` | Set guard flag n, send message 7 (n,-1) to self, broadcast game event for flags in D_002C3D98 (3->0x14, 0xC->0x16) | 0.60 | HYP |
| [0x209970](../../src/hand/func_00209970.cpp) | `RemoveStepguardSgf` | Clear guard flag n, message 7 (-1,n) | 0.60 | HYP |
| [0x20A818](../../src/hand/func_0020A818.cpp) | `FTargetRecordAllowsGuard` | Engagement token: fail if disabled or another engaged guard is busy (+0xF3C); kind 0 requires guard ready or inside camera frustum within heading tole | 0.40 | HYP |

Selected 46 of 404 named functions (confidence >= 0.7, verified, or flagged). Full list: [reference/functions/guards-ai.md](../reference/functions/guards-ai.md).

## Open questions
- Names/roles of the ~43 BHV classes (BHV TU 0x174898 unnamed). Experiment: break 0x1EEE00 (push) and log a1 = cid while alerting/losing guards; watch guard+0x270 (cpbhv).
- Meaning of flags 0x1D/0x20/0x2A (alert?) and of the reaction BHV190/BHV172 (investigate?). Needs a state where a guard actually spots Sly.
- Which gadget is which special state (shrink/freeze/rage).
- Is CID 0x77 the reinforcement spawner? Trigger an alarm and break 0x174490.
- BHV slot names 8, 11, 12, 14.
