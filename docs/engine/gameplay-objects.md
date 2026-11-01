# Gameplay object classes

Smaller gameplay classes that do not belong to one subsystem. Most are new in Sly 2 and only partly identified; names are mostly placeholders.
Class numbers in hex; see [class-tree](../reference/class-tree.md).

| Class | TU | What it is | Tag |
|---|---|---|---|
| BARRIER (0x56) | barrier.cpp 0x1320C8 | Sly 1 barrier minus the warp logic; impacts go through a deferred SW callback (0x132238 -> 0x132298) that fires Splice event 5; `FIgnoreBarrierIntersection` 0x132140 (tri-state) | [C:verified] (0x132140), [H r2b barrier.md] |
| BOMB (0x4F) | bomb.cpp 0x13A9E8 | Sly 1 bomb.c by slot and body: Init, PostLoad, HandleMessage, Update, custom XPs, `ApplyBombThrow`, `PrimeBomb`, `DetonateBomb` | [H r2b bomb.md] |
| MISSILE (0x50), ACCMISS (0x51), CID 0x52 | missile.cpp 0x1B1DB8 | guided projectiles derived from BOMB; vtables have 79 entries, slot 78 = Fire (cid_table says 78: CONFLICTS #4); `FIgnoreMissileIntersection` 0x1B1F40 | [C:verified] (0x1B1F40), [H r5 missile.md] |
| BUTTON (0x53), VOLBTN (0x18/0x19) | button.cpp 0x148620 | Sly 1 button.cpp; fires Splice events 5/6 on trigger/untrigger; RSMG records; extra event-listener list | [H r2b button.md] |
| SENSOR (0x54), laser beams (0x55, "LASEN"?) | sensor.cpp 0x1DE2A0 | no-trigger lists; up to 64 beams between fixed or path-moving endpoints; the nearest blocker is zapped (kind 0x1000), unlike Sly 1's alarm lasers | [H r6b sensor.md] |
| ALARM? (0x67) / booty | booty? 0x13BDF0 | cid_table names CID 0x67 (103) ALARM, but its code is the carried booty object (strings booty, oidBooty, aoidAssembly); Sly 2 ALARM is only SO + 0x40 bytes, too small for Sly 1's field list. See [game-progression](game-progression.md#5-booty-booty-tu-0x13bdf0-cid_table-says-alarm-wrong) | [H r2b booty.md] |
| CID 0x47 + 0x48-0x4C | tu_17F090 | damageable PO family: hit points (+0x180 max 10, +0x184 current), `TakeDamage` slot 75, `Die` slot 82 (area zap, explosion); 0x48 singleton with a health-bar widget (boss-like), 0x49 turret-like, 0x4A timed self-destruct, 0x4C segmented breakable | [H r4b cid47.md] |
| TURRET (0x45) | turret.cpp 0x2259D8 | pan/tilt limits (±1.5 / ±0.7), zoom, fire, camera policy at priority 9 on activation; Sly 1 turret.cpp roles | [H r8 turret.md] |
| RC car (0x4D) + ammo pickup (0x6C) | rog? 0x1D1370 | armed four-wheeled vehicle: player-driven or AI (tactics, seeks pickups, nav paths), custom wheel contacts, weapons, health meter 0x34F | [H r5b rog.md] |
| TARGET (0x12), HND (0x13), CID 0x15 | target.cpp 0x21D278, hnd.cpp | Sly 1 target.cpp one-for-one (g_dlTarget D_003E13B4); CID 0x15 camera-related list D_003E1474 | [H r8 target.md] |
| CID 0x27 | tu_1FF3B8 | XFM child with GRABSPOT-like comment; Init, PostLoad, OnRemove/OnAdd (list D_003E13D8), Clone (`CloneCid39` etc. were hex/decimal mix-ups, CONFLICTS #11) | [C:verified] (slots), [H r7] |
| CID 0x60 | tu_1B0B78 | SO walking a curve that flees the player, explodes a linked object when hit | [H r5b small_tus_r5b.md] |
| CID 0x62 | tu_1BAB50 | staged reaction object: up to 32 stages x 64 recorded actions (show/hide, camera jolt, rumble, sound, spawn) run on touch/zap (232 in the hub) | [H r5b], count [C:runtime] |
| CID 0x63 | tu_1C2D20 | launcher that throws objects at the player from a proxy pool; also the pickpocket pouch class | [C:runtime] (pouch), [H r5] |
| CID 0x64-0x66 | tu_223F70 | carryable/pushable object family with a 0..0x29 state machine (slot 76); 0x66 drops a prize (event 0x18) | [H r8 cid100_family.md] |
| CID 0x74 | tu_15DC80 | timed LO (created by PostSwLoad) | [H r3] |
| CID 0x79 + CID 0x44 | rhythm? 0x1520C8 | button-timing minigame: manager (timeline of 6 prompts against music time, hands-off, HUD track, scoring) and a STEP "performer" | [H r2 rhythm.md] |
| CID 0x7A | tu_153B08 | LoadFromBrx only | [H r3] |

## Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x1320C8-0x132570 | barrier |  |
| 0x13A9E8-0x13BDF0 | bomb |  |
| 0x148620-0x149A50 | button |  |
| 0x1520C8-0x1539A8 | rhythm? | button-timing minigame |
| 0x1539A8-0x153B08 | tu_1539A8 | static-init only + 4-byte stub |
| 0x153B08-0x153D58 | tu_153B08 (cid 0x7A) |  |
| 0x15DC80-0x15E380 | tu_15DC80 (cid 0x74 timed LO) |  |
| 0x17F090-0x182C70 | tu_17F090 (cid 0x47 damageable PO) | was labelled stepguard |
| 0x1B0B78-0x1B1DB8 | tu_1B0B78 (cid 0x60) |  |
| 0x1B1DB8-0x1B2820 | missile |  |
| 0x1BAB50-0x1BC050 | tu_1BAB50 (cid 0x62 staged reactions) |  |
| 0x1C2920-0x1C2A78 | ? |  |
| 0x1C2D20-0x1C3390 | tu_1C2D20 (cid 0x63 launcher) |  |
| 0x1D1370-0x1D5150 | rog? (cid 0x4D RC car) |  |
| 0x1DE280-0x1DFAD8 | sensor | starts 0x1DE2A0 |
| 0x1FF398-0x1FF690 | tu_1FF3B8 (cid 0x27) |  |
| 0x21D278-0x21D638 | target |  |
| 0x223F70-0x2259D8 | tu_223F70 (cid 0x64-0x66) |  |
| 0x2259D8-0x226550 | turret |  |

## Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x1320C8](../../src/hand/func_001320C8.c) | `InitBarrier` | BARRIER vt7 Init: InitSo then set an SO flag bit (grf \|= 1<<34) | 0.80 | HYP |
| [0x132140](../../src/hand/func_00132140.c) | `FIgnoreBarrierIntersection` | BARRIER vt61: match other SO against aoid[coid]/acid[ccid]; with fAllow (+0x178) matched->3 else base; without fAllow matched->2 else 1 | 0.85 | VERIFIED |
| [0x13A9E8](../../src/hand/func_0013A9E8.c) | `InitBomb` | BOMB vt7: InitSo, tDetonate=FLT_MAX, radii, zap defaults, flag, owner=self | 0.85 | HYP |
| [0x13AAA0](../../src/hand/func_0013AAA0.c) | `PostBombLoad` | BOMB vt15: PostSoLoad, and with fCustomXps (+0x1F4) allocate the custom-XP buffer (0xC0) | 0.80 | HYP |
| [0x13AAE8](../../src/hand/func_0013AAE8.c) | `HandleBombMessage` | BOMB vt5: base handler; on msg 4 about itself while not primed, PrimeBomb(0) | 0.80 | HYP |
| [0x13AB58](../../src/hand/func_0013AB58.c) | `UpdateBomb` | BOMB vt16: UpdateSo; if timer expired or speed above threshold DetonateBomb, else UpdateBombCustomXps when enabled | 0.80 | HYP |
| [0x13ADE8](../../src/hand/func_0013ADE8.c) | `AddBombCustomXps` | BOMB vt60: base SO hook, then for a custom-xp entry matching the other object add an XP (PxpAdd type 9) | 0.75 | HYP |
| [0x13B428](../../src/hand/func_0013B428.c) | `PrepareBombDetonation` | Once: record detonation pos (+0x180), save v/w, orient by mode (+0x170) using the impact normal | 0.80 | HYP |
| [0x13B5C8](../../src/hand/func_0013B5C8.c) | `PrimeBomb` | tDetonate = min(tDetonate, now + dt) | 0.85 | HYP |
| [0x13B860](../../src/hand/func_0013B860.cpp) | `DetonateBomb` | If in world: PrepareBombDetonation, send damage/explosion messages to every victim, explode effects, reclaim or remove | 0.80 | HYP |
| [0x148B38](../../src/hand/func_00148B38.c) | `SetBtnRsmg` | FAddRsmg(g_sw, arsmg +0xB4, 8, &crsmg, fOnTrigger, oidRoot, oidSM, oidGoal) | 0.80 | HYP |
| [0x148BB0](../../src/hand/func_00148BB0.c) | `InitButton` | cid 0x53 vt7: InitSo, InitBtn(+0x150), flag | 0.80 | HYP |
| [0x148C08](../../src/auto/func_00148C08.c) | `LoadButtonFromBrx` | vt14: LoadSoFromBrx then LoadBtn(+0x150, this) | 0.80 | HYP |
| [0x148C38](../../src/auto/func_00148C38.c) | `PostButtonLoad` | vt15: PostSoLoad, PostBtnLoad, SetButtonButtons(0) | 0.75 | HYP |
| [0x149270](../../src/auto/func_00149270.c) | `SetButtonRsmg` | SetBtnRsmg(+0x150,...) | 0.75 | HYP |
| [0x1492F0](../../src/hand/func_001492F0.c) | `InitVolbtn` | cid 0x18/0x19 vt7: base init, InitBtn | 0.75 | HYP |
| [0x149808](../../src/auto/func_00149808.c) | `SetVolbtnRsmg` | SetBtnRsmg | 0.75 | HYP |
| [0x1535A0](../../src/hand/func_001535A0.c) | `GRhythmTimingScore` | Score = lerp(D_002BBB60 range, 1 - \|t - tCue\| / window), window = early(+0x58) or late(+0x5C) | 0.50 | VERIFIED |
| [0x153760](../../src/hand/func_00153760.c) | `GAverageRhythmScore` | Average of the last n (+0x280) scores clamped to lm | 0.55 | VERIFIED |
| [0x153800](../../src/hand/func_00153800.c) | `ResetRhythmScore` | Zero score count/average/smoothed (+0x23C,+0x284,+0x288) | 0.50 | VERIFIED |
| [0x15DC80](../../src/hand/func_0015DC80.c) | `InitCid74` | LO::Init, register for update, flags 0xA | 0.75 | HYP |
| [0x1B1DB8](../../src/auto/func_001B1DB8.c) | `InitMissile` | MISSILE::Init (vt7): InitBomb; fFollowTrajectory(+0x264)=1; +0x260=800; +0x26C=1 | 0.90 | HYP |
| [0x1B1E00](../../src/auto/func_001B1E00.c) | `LoadMissileFromBrx` | MISSILE::LoadFromBrx (vt14): base load (0x1F31E0) + SnipLoObjects(this,1,s_asnipMissile D_002C0258) | 0.85 | HYP |
| [0x1B1E38](../../src/hand/func_001B1E38.c) | `OnMissileRemove` | MISSILE::OnRemove (vt12): OnSoRemove (0x1F2D48) then retract current asega | 0.85 | HYP |
| [0x1B1E88](../../src/hand/func_001B1E88.c) | `FireMissile` | MISSILE::Fire (vt78): store source LO (+0x210) and target pos (+0x220), start fire aseg, optionally start action | 0.80 | HYP |
| [0x1B2070](../../src/auto/func_001B2070.c) | `InitAccmiss` | ACCMISS::Init: InitMissile; svFast(+0x270)=2000, svTarget(+0x274)=5000, fFollowTrajectory=0 | 0.85 | HYP |
| [0x1BAD80](../../src/auto/func_001BAD80.c) | `OnCid98Remove` | CID 98 vt22: FreezeSo then RemoveDlEntry from class list D_003E1450 | 0.60 | VERIFIED (arbitration/conflict fix; was FreezeCid98) |
| [0x1BADB8](../../src/auto/func_001BADB8.c) | `OnCid98Add` | CID 98 vt23: UnfreezeSo then AppendDlEntry to D_003E1450 | 0.60 | VERIFIED (arbitration/conflict fix; was UnfreezeCid98) |
| [0x1D1370](../../src/hand/func_001D1370.c) | `InitRcCar` | CID 77 vt7 Init: InitPo, health 100 (x55C), ranges 1500/-1000 and 1500/2500, timers 30, state -1 | 0.75 | HYP |
| [0x1D1518](../../src/hand/func_001D1518.cpp) | `PostRcCarLoad` | CID 77 vt15 PostLoad: PostSoLoad; resolve wheel entries (XF ids, kind 13) and compute their normalized layout/width; resolve child XF bindings; resolv | 0.75 | HYP |
| [0x1D1940](../../src/auto/func_001D1940.c) | `OnRcCarActive` | CID 77 vt78(f): OnPoActive, then state 2 when activated, back to 0 when deactivated from 2 | 0.75 | HYP |
| [0x1D19A8](../../src/hand/func_001D19A8.cpp) | `UpdateRcCar` | CID 77 vt16 Update: UpdateSo; heading from world matrix; weapon ready SMAs; mode 2 player input / mode 1 AI / else idle; steering and throttle from po | 0.75 | HYP |
| [0x1FF438](../../src/auto/func_001FF438.c) | `OnCid39Add` | vt23: add to SW update array (0x1A6178) and to global DL D_003E13D8 | 0.45 | VERIFIED (arbitration/conflict fix; was OnCid27Add) |
| [0x1FF470](../../src/auto/func_001FF470.c) | `OnCid39Remove` | vt22: remove from SW update array (0x1A6150) and from DL D_003E13D8 | 0.45 | VERIFIED (arbitration/conflict fix; was OnCid27Remove) |
| [0x1FF4A8](../../src/hand/func_001FF4A8.c) | `CloneCid39` | Clone: XFM clone (0x2311B0) preserving a key field | 0.55 | VERIFIED (arbitration/conflict fix; was CloneCid27) |
| [0x21D278](../../src/auto/func_0021D278.c) | `InitTarget` | InitXfm; grftak=0x17E; sRadiusTarget=25.0; +0xF4=-1; SetLoBusyk(3) | 0.85 | VERIFIED |
| [0x21D320](../../src/auto/func_0021D320.c) | `OnTargetRemove` | OnLoRemove then RemoveDlEntry(&g_dlTarget D_003E13B4, this) | 0.85 | VERIFIED |
| [0x21D358](../../src/auto/func_0021D358.c) | `OnTargetAdd` | OnLoAdd then AppendDlEntry(&g_dlTarget, this) | 0.85 | VERIFIED |
| [0x21D390](../../src/hand/func_0021D390.c) | `CloneTarget` | saves the 8-byte DLE at 0xE0, CloneXfm (0x2311B0), restores DLE | 0.80 | VERIFIED |
| [0x21D428](../../src/auto/func_0021D428.c) | `OnCid21Remove` | OnLoRemove + RemoveDlEntry(&D_003E1474) | 0.60 | VERIFIED |
| [0x21D460](../../src/auto/func_0021D460.c) | `OnCid21Add` | OnLoAdd + AppendDlEntry(&D_003E1474) | 0.60 | VERIFIED |
| [0x21D498](../../src/hand/func_0021D498.c) | `CloneCid21` | same as CloneTarget for cid 0x15 (preserve DLE at 0xE0) | 0.55 | VERIFIED |
| [0x223F70](../../src/hand/func_00223F70.c) | `InitCid100` | InitSo; +0x?=-10, 3000.0, -1, -1; grf\|=6 | 0.60 | VERIFIED |
| [0x224380](../../src/hand/func_00224380.cpp) | `UpdateCid100Contacts` | walks contact edges, picks neighbouring actor (0x224638, 0x224508) | 0.30 | VERIFIED |
| [0x2259D8](../../src/hand/func_002259D8.c) | `InitTurret` | InitPo; lmPan=(-1.5,1.5), lmTilt=(-0.7,0.7), defaults, self pointer | 0.75 | HYP |
| [0x225A68](../../src/auto/func_00225A68.c) | `CloneTurret` | ClonePo then fix self pointer 0x1D0 | 0.60 | VERIFIED |
| [0x225AE8](../../src/hand/func_00225AE8.c) | `OnTurretActive` | OnPoActive then SetCmPolicy(g_cm,9,g_cm+0x5F0,..) or RevokeCmPolicy | 0.80 | VERIFIED |

Selected 47 of 365 named functions (confidence >= 0.75, verified, or flagged). Full list: [reference/functions/gameplay-objects.md](../reference/functions/gameplay-objects.md).

## Open questions
- In-game identity of most placeholder classes (CID 0x47 family, 0x60, 0x62, 0x64-0x66, 0x74, 0x7A). Experiment: break each class's Init at load and look at the level.
- MISSILE vtable length 78 vs 79 (CONFLICTS #4); identity of CID 0x52 (TARMISS/SPLMISS?).
