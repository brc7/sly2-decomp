# Gameplay object classes

Smaller gameplay classes that do not belong to one subsystem. Most are new in Sly 2 and only partly identified; names are mostly placeholders.
Class numbers in hex; see [class-tree](../reference/class-tree.md).

| Class | TU | What it is | Tag |
|---|---|---|---|
| BARRIER (0x56) | barrier.cpp 0x1320C8 | Sly 1 barrier minus the warp logic; impacts go through a deferred SW callback (0x132238 -> 0x132298) that fires Splice event 5; `FIgnoreBarrierIntersection` 0x132140 (tri-state) | [C:verified] (0x132140), [H r2b barrier.md] |
| BOMB (0x4F) | bomb.cpp 0x13A9E8 | Sly 1 bomb.c by slot and body: Init, PostLoad, HandleMessage, Update, custom XPs, `ApplyBombThrow`, `PrimeBomb`, `DetonateBomb` | [H r2b bomb.md] |
| MISSILE (0x50), ACCMISS (0x51), CID 0x52 | missile.cpp 0x1B1DB8 | guided projectiles derived from BOMB; vtables have 79 entries, slot 78 = Fire (cid_table says 78: CONFLICTS #4); `FIgnoreMissileIntersection` 0x1B1F40 | [C:verified] (0x1B1F40), [H r5 missile.md] |
| BUTTON (0x53), VOLBTN (0x18/0x19) | button.cpp 0x148620 | Sly 1 button.cpp; fires Splice events 5/6 on trigger/untrigger; RSMG records; extra event-listener list | [H r2b button.md] |
| SENSOR (0x54), laser beams LASEN (0x55; runtime layout in [guards-ai](guards-ai.md#5-perception), zap 0x1000 -10 [C:runtime S64]) | sensor.cpp 0x1DE2A0 | no-trigger lists; up to 64 beams between fixed or path-moving endpoints; the nearest blocker is zapped (kind 0x1000), unlike Sly 1's alarm lasers | [H r6b sensor.md] |
| BOOTY (0x67) | booty 0x13BDF0 | carried booty object (all 41 instances in the game are WE `booty`; replaces cid_table's ALARM). See [game-progression](game-progression.md#5-booty-booty-tu-0x13bdf0) | [C:parse whole_game] (identity), [H r2b] (states) |
| CID71 (0x47) + HACKSHIP 0x48, HACKENEMY 0x49, HACKBULLET 0x4A, HACKLOCK 0x4B, HACKWALL 0x4C | tu_17F090 | the **hacking minigame** (Bentley's computer hacking): 0x47 is its damageable PO base (hit points +0x180/+0x184, `TakeDamage` slot 75, `Die` slot 82); ship / enemies / bullets / lock / walls | [C:parse whole_game] (identities), [H r4b] (fields) |
| TURRET (0x45; grapple turret in the level data) | turret.cpp 0x2259D8 | pan/tilt limits (±1.5 / ±0.7), zoom, fire, camera policy at priority 9 on activation; Sly 1 turret.cpp roles | [H r8 turret.md] |
| ROBOTTANK (0x4D, single instance `robot_tank`; was "RC car") + ammo pickup (0x6C) | tu_1D1370 | armed four-wheeled vehicle: player-driven or AI (tactics, seeks pickups, nav paths), custom wheel contacts, weapons, health meter 0x34F | [H r5b rog.md] |
| TARGET (0x12), HND (0x13), CID 0x15 | target.cpp 0x21D278, hnd.cpp | Sly 1 target.cpp one-for-one (g_dlTarget D_003E13B4); CID 0x15 camera-related list D_003E1474 | [H r8 target.md] |
| CID 0x27 | tu_1FF3B8 | XFM child with GRABSPOT-like comment; Init, PostLoad, OnRemove/OnAdd (list D_003E13D8), Clone (`CloneCid39` etc. were hex/decimal mix-ups, CONFLICTS #11) | [C:verified] (slots), [H r7] |
| CID96 (0x60) | tu_1B0B78 | mini pigeon/rat critters: SO walking a curve that flees the player; `SetCid96SpinRatio` 0x1B17B8 (was `SetStepSpinRatio`) | [H r5b small_tus_r5b.md] |
| CID98 (0x62) | tu_1BAB50 | **breakable props** (chairs, barrels, statues, crates, coin containers): up to 32 stages x 64 recorded actions run on touch/zap (232 in the hub); breaking sprays prizes from proxy pools; Murray can carry one (grabspot CID36) | [C:runtime S10-S11, S41-S43], [C:parse whole_game] |
| CID99 (0x63) | tu_1C2D20 | pickpocket dispenser / pouch (SO child of the guard) | [C:runtime] |
| CID100-102 (0x64-0x66) | tu_223F70 | gadget objects: 0x64 tossables (tommy, distract, music box), **0x65 Bentley's trigger bomb / mines** (states -1 pooled, 0 held, 1 flight, 2 stuck, 3 detonate, 5 health extractor; `SetCid100State` 0x224EC8), 0x66 guard trap (drops a prize, event 0x18) | [C:runtime runtime5a] (0x65), [C:parse whole_game] |
| CID116 (0x74) | tu_15DC80 | screen tint / fill (render kinds 14 and 1; created by PostSwLoad) | [C:static rendering] |
| CID121 (0x79) + DANCER (0x44) | rhythm 0x1520C8 | button-timing minigame: manager (timeline of 6 prompts against music time, hands-off, HUD track, scoring) and a STEP "performer" | [H r2 rhythm.md] |
| CID122 (0x7A) | tu_153B08 | LoadFromBrx only (string16 + FILEREC records) | [C:static level_parser] |
| SAFEHOUSE (0x46) | tu_1D7238 | character-select PO; see [player](player.md#8-look-mode-recon-mode-and-the-ai-partner) | [C:runtime S48] |
| SKY (0x61) | sky 0x1EF850 | sky dome (follows the camera in x/y; render kind 1) | [C:parse], [C:runtime] |
| KEY (0x5C), MEDICINE (0x5D) | prize | pickups (see [game-progression](game-progression.md#4-collectibles-and-loot-prizecpp-0x1c4e50)) | [C:runtime] |
| ACCMISS (0x51) | missile | Bentley's crossbow dart (fired via launcher CID112 0x70 = `aim_manager`) | [C:runtime runtime5a] |

## Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x1320C8-0x132570 | barrier | high |  |
| 0x13A9E8-0x13BDF0 | bomb | medium |  |
| 0x148620-0x149A50 | button | medium | BUTTON+VOLBTN |
| 0x14C548-0x14C6A0 | tu_14C548 | low |  |
| 0x1520C8-0x1539A8 | rhythm | low | rhythm/button-timing minigame (CID 0x44); button-timing minigame |
| 0x1539A8-0x153B08 | tu_1539A8 | low | cm<x<dmas; static-init only + 4-byte stub |
| 0x153B08-0x153D58 | tu_153B08 | low | CID 0x7A |
| 0x15DC80-0x15E380 | tu_15DC80 | low | CID 0x74 timed LO |
| 0x17F090-0x182C70 | tu_17F090 | low | CID 0x47 PO child + 0x48-0x4C; was labelled stepguard |
| 0x1B0B78-0x1B1DB8 | tu_1B0B78 | low | CID96 SO child (mi*?) |
| 0x1B1DB8-0x1B2820 | missile | high | tu_table said mpeg |
| 0x1BAB50-0x1BC050 | tu_1BAB50 | low | CID98 |
| 0x1C2D20-0x1C3390 | tu_1C2D20 | low | CID99 guard loot pouch (pick*?) |
| 0x1D1370-0x1D5150 | tu_1D1370 | low | CID77 RC vehicle + CID108 (rog? unconfirmed) |
| 0x1DE2A0-0x1DFAD8 | sensor | medium | starts at 0x1DE2A0; starts 0x1DE2A0 |
| 0x1FF3B8-0x1FF690 | tu_1FF3B8 | low | CID39 methods (sound<x<splice*); starts at 0x1FF3B8 |
| 0x21D278-0x21D638 | target | high |  |
| 0x223F70-0x2259D8 | tu_223F70 | low | CID100/101/102 |
| 0x2259D8-0x226550 | turret | high |  |

## Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x1320C8](../../src/game/gameplay/objects/barrier.cpp) | `InitBarrier` | BARRIER vt7 Init: InitSo then set an SO flag bit (grf \|= 1<<34) | 0.85 | T1 | verified |
| [0x132140](../../src/game/gameplay/objects/barrier.cpp) | `FIgnoreBarrierIntersection` | BARRIER vt61: match other SO against aoid[coid]/acid[ccid]; with fAllow (+0x178) matched->3 else base; without fAllow matched->2 else 1 | 0.85 | T1 | verified |
| [0x132238](../../src/game/gameplay/objects/barrier.cpp) | `NotifyBarrierImpact` | BARRIER vt55 (XP created): SO base 0x1F4FB0, then if the object has a Splice binding for event 5, EnsureSwCallback(g_sw, this, &0x132298) | 0.85 | T1 |  |
| [0x132298](../../src/game/gameplay/objects/barrier.cpp) | `SendBarrierImpactMessages` | Deferred callback: collect up to 16 distinct SOs touching the barrier, and for each set +0x17C=pso and HandleSpliceEvent(5) via vt3 (fires the Splice  | 0.60 | T1 | verified |
| [0x13A9E8](../../src/game/gameplay/objects/bomb/InitBomb.c) | `InitBomb` | BOMB vt7: InitSo, tDetonate=FLT_MAX, radii, zap defaults, flag, owner=self | 0.85 | T1 |  |
| [0x13AAA0](../../src/game/gameplay/objects/bomb/PostBombLoad.c) | `PostBombLoad` | BOMB vt15: PostSoLoad, and with fCustomXps (+0x1F4) allocate the custom-XP buffer (0xC0) | 0.80 | T1 |  |
| [0x13AAE8](../../src/game/gameplay/objects/bomb/HandleBombMessage.c) | `HandleBombMessage` | BOMB vt5: base handler; on msg 4 about itself while not primed, PrimeBomb(0) | 0.80 | T1 |  |
| [0x13AB58](../../src/game/gameplay/objects/bomb/UpdateBomb.c) | `UpdateBomb` | BOMB vt16: UpdateSo; if timer expired or speed above threshold DetonateBomb, else UpdateBombCustomXps when enabled | 0.80 | T1 |  |
| [0x13ADE8](../../src/game/gameplay/objects/bomb/AddBombCustomXps.c) | `AddBombCustomXps` | BOMB vt60: base SO hook, then for a custom-xp entry matching the other object add an XP (PxpAdd type 9) | 0.75 | T1 |  |
| [0x13B428](../../src/game/gameplay/objects/bomb/PrepareBombDetonation.c) | `PrepareBombDetonation` | Once: record detonation pos (+0x180), save v/w, orient by mode (+0x170) using the impact normal | 0.80 | T1 |  |
| [0x13B5C8](../../src/game/gameplay/objects/bomb/PrimeBomb__FP4BOMBf.c) | `PrimeBomb` | tDetonate = min(tDetonate, now + dt) | 0.85 | T1 | verified |
| [0x13B860](../../src/game/gameplay/objects/bomb/DetonateBomb.cpp) | `DetonateBomb` | If in world: PrepareBombDetonation, send damage/explosion messages to every victim, explode effects, reclaim or remove | 0.80 | T1 |  |
| [0x148B38](../../src/game/gameplay/objects/button.cpp) | `SetBtnRsmg` | FAddRsmg(g_sw, arsmg +0xB4, 8, &crsmg, fOnTrigger, oidRoot, oidSM, oidGoal) | 0.80 | T1 |  |
| [0x148BB0](../../src/game/gameplay/objects/button.cpp) | `InitButton` | cid 0x53 vt7: InitSo, InitBtn(+0x150), flag | 0.80 | T1 |  |
| [0x148C08](../../src/game/gameplay/objects/button.cpp) | `LoadButtonFromBrx` | vt14: LoadSoFromBrx then LoadBtn(+0x150, this) | 0.85 | T1 | parse |
| [0x148C38](../../src/game/gameplay/objects/button.cpp) | `PostButtonLoad` | vt15: PostSoLoad, PostBtnLoad, SetButtonButtons(0) | 0.75 | T1 |  |
| [0x149358](../../src/game/gameplay/objects/button.cpp) | `LoadCid24FromBrx` |  | 0.75 | T2 | parse, format-doc |
| [0x152100](../../src/game/gameplay/objects/rhythm/func_00152100.c) | `PostLoadDancer` | cid 0x44 vt15: base 0x203A38, BindDancerMoves (resolve aseg oids), vt79(0) | 0.40 | T2 | verified |
| [0x152810](../../src/game/gameplay/objects/rhythm/func_00152810.c) | `BindRhythmMgrGrades` | Callback: for each entry find object by oid and its child 0x625, bind with 0x1F1448, pick 3 random grade thresholds across the score range | 0.35 | T2 | verified |
| [0x1535A0](../../src/game/gameplay/objects/rhythm/GRhythmTimingScore.c) | `GRhythmTimingScore` | Score = lerp(D_002BBB60 range, 1 - \|t - tCue\| / window), window = early(+0x58) or late(+0x5C) | 0.85 | T1 | verified |
| [0x153760](../../src/game/gameplay/objects/rhythm/GAverageRhythmScore.c) | `GAverageRhythmScore` | Average of the last n (+0x280) scores clamped to lm | 0.85 | T1 | verified |
| [0x153800](../../src/game/gameplay/objects/rhythm/ResetRhythmScore.c) | `ResetRhythmScore` | Zero score count/average/smoothed (+0x23C,+0x284,+0x288) | 0.85 | T1 | verified |
| [0x17F6B0](../../src/game/gameplay/objects/tu_17F090/func_0017F6B0.c) | `ResetCid71` | Health = max; cancel pending dying callback (ClearSwCallbacks D_002A5638) | 0.40 | T2 | verified |
| [0x1800A0](../../src/game/gameplay/objects/tu_17F090/AddHackshipOidPair.c) | `AddHackshipOidPair` | Append (int, OID) to a 4-entry table at +0x234 (stride 0x18, count +0x230) | 0.50 | existing | arbitration; replaces existing UseStepguardDeathAnimation__FP9STEPGUARDi3OID |
| [0x180188](../../src/game/gameplay/objects/tu_17F090/func_00180188.c) | `InitHackbullet` | CID 0x4A vt7: InitCid47, field = 10 | 0.50 | T2 | verified, whole-game |
| [0x181F60](../../src/game/gameplay/objects/tu_17F090/func_00181F60.c) | `UpdateHackwall` | vt16: UpdateSo, per-segment shape tracking and random effects | 0.35 | T2 | verified, whole-game |
| [0x1B17B8](../../src/game/gameplay/objects/tu_1B0B78/SetStepSpinRatio__FP4STEPf.cpp) | `SetCid96SpinRatio` | Store scale vector D_002C0248*(r, r, 1/sqrt r) at +0x210 and r at +0x21C | 0.65 | existing | arbitration; replaces existing SetStepSpinRatio__FP4STEPf |
| [0x1B1DB8](../../src/game/gameplay/objects/missile/InitMissile__FP7MISSILE.c) | `InitMissile` | MISSILE::Init (vt7): InitBomb; fFollowTrajectory(+0x264)=1; +0x260=800; +0x26C=1 | 0.90 | T1 |  |
| [0x1B1E00](../../src/game/gameplay/objects/missile/LoadMissileFromBrx__FP7MISSILEP18CBinaryInputStream.c) | `LoadMissileFromBrx` | MISSILE::LoadFromBrx (vt14): base load (0x1F31E0) + SnipLoObjects(this,1,s_asnipMissile D_002C0258) | 0.90 | T1 | parse |
| [0x1B1E38](../../src/game/gameplay/objects/missile/OnMissileRemove__FP7MISSILE.c) | `OnMissileRemove` | MISSILE::OnRemove (vt12): OnSoRemove (0x1F2D48) then retract current asega | 0.85 | T1 |  |
| [0x1B1E88](../../src/game/gameplay/objects/missile/FireMissile.c) | `FireMissile` | MISSILE::Fire (vt78): store source LO (+0x210) and target pos (+0x220), start fire aseg, optionally start action | 0.80 | T1 |  |
| [0x1B1F40](../../src/game/gameplay/objects/missile/FIgnoreMissileIntersection__FP7MISSILEP2SO.c) | `FIgnoreMissileIntersection` | MISSILE vt61: BOMB base result, modified when other SO's oid is in list +0x23C or derives from a cid in list +0x250 | 0.80 | T1 | verified |
| [0x1B2070](../../src/game/gameplay/objects/missile/InitAccmiss__FP7ACCMISS.c) | `InitAccmiss` | ACCMISS::Init: InitMissile; svFast(+0x270)=2000, svTarget(+0x274)=5000, fFollowTrajectory=0 | 0.85 | T1 |  |
| [0x1BABA0](../../src/game/gameplay/objects/tu_1BAB50/func_001BABA0.cpp) | `LoadCid98FromBrx` | CID 98 vt14: allocate trigger-class list and stage table (with stage -1), LoadSoFromBrx, trim allocations to counts | 0.75 | T2 | parse |
| [0x1BAD80](../../src/game/gameplay/objects/tu_1BAB50/func_001BAD80.c) | `OnCid98Remove` | CID 98 vt22: FreezeSo then RemoveDlEntry from class list D_003E1450 | 0.85 | T2 | arbitration |
| [0x1BADB8](../../src/game/gameplay/objects/tu_1BAB50/func_001BADB8.c) | `OnCid98Add` | CID 98 vt23: UnfreezeSo then AppendDlEntry to D_003E1450 | 0.85 | T2 | arbitration |
| [0x1BB250](../../src/game/gameplay/objects/tu_1BAB50/func_001BB250.c) | `ScheduleCid98TouchEvents` | CID 98 vt58: SO base, then for contacts with allowed trigger classes schedule a reaction; chain to touching CID-98 objects | 0.45 | T2 | verified |
| [0x1D1940](../../src/game/gameplay/objects/tu_1D1370/OnRobottankActive.c) | `OnRobottankActive` | CID 77 vt78(f): OnPoActive, then state 2 when activated, back to 0 when deactivated from 2 | 0.85 | T1 | verified, whole-game |
| [0x1D3E48](../../src/game/gameplay/objects/tu_1D1370/AddRobottankCustomXps.c) | `AddRobottankCustomXps` | CID 77 vt60: SO base (no-op) then wheel contacts (0x1D3450) for every wheel if the other object passes its vt62 test | 0.50 | T1 | verified, whole-game |
| [0x1D3F30](../../src/game/gameplay/objects/tu_1D1370/func_001D3F30.c) | `Robottank_vt54` | CID 77 vt54(pso): SO base no-op then RemoveSoContactsWith for every wheel | 0.35 | T2 | verified, whole-game |
| [0x1D4D18](../../src/game/gameplay/objects/tu_1D1370/func_001D4D18.c) | `SetRobottankSlotValue` | Set field +0x574 of 12-byte slot i | 0.25 | T2 | verified, whole-game |
| [0x1D4DE0](../../src/game/gameplay/objects/tu_1D1370/func_001D4DE0.c) | `CopyRobottankWeapon` | CopyAb 0x9C bytes from +0x14 | 0.40 | T2 | verified, whole-game |
| [0x1DE3B0](../../src/game/gameplay/objects/sensor/AddSensorNoTriggerClass.c) | `AddSensorNoTriggerClass` | Append a cid to the no-trigger class list (+0x170, max 7) | 0.55 | T1 | verified |
| [0x1DE478](../../src/game/gameplay/objects/sensor/LoadLasenFromBrx.cpp) | `LoadLasenFromBrx` | LASEN::LoadFromBrx (vt14): temp-allocate 0x5400 (64 beams x 0x150), LoadSoFromBrx (Splice adds beams), shrink to count | 0.75 | T1 | parse |
| [0x1FF438](../../src/game/gameplay/objects/tu_1FF3B8/func_001FF438.c) | `OnCid39Add` | vt23: add to SW update array (0x1A6178) and to global DL D_003E13D8 | 0.85 | T2 | arbitration |
| [0x1FF470](../../src/game/gameplay/objects/tu_1FF3B8/func_001FF470.c) | `OnCid39Remove` | vt22: remove from SW update array (0x1A6150) and from DL D_003E13D8 | 0.85 | T2 | verified, arbitration |
| [0x1FF4A8](../../src/game/gameplay/objects/tu_1FF3B8/func_001FF4A8.c) | `CloneCid39` | Clone: XFM clone (0x2311B0) preserving a key field | 0.80 | T2 | verified |
| [0x21D278](../../src/game/gameplay/objects/target.cpp) | `InitTarget` | InitXfm; grftak=0x17E; sRadiusTarget=25.0; +0xF4=-1; SetLoBusyk(3) | 0.85 | T1 | verified |
| [0x21D320](../../src/game/gameplay/objects/target.cpp) | `OnTargetRemove` | OnLoRemove then RemoveDlEntry(&g_dlTarget D_003E13B4, this) | 0.85 | T1 | verified, arbitration |
| [0x21D358](../../src/game/gameplay/objects/target.cpp) | `OnTargetAdd` | OnLoAdd then AppendDlEntry(&g_dlTarget, this) | 0.85 | T1 | verified, arbitration |
| [0x21D390](../../src/game/gameplay/objects/target.cpp) | `CloneTarget` | saves the 8-byte DLE at 0xE0, CloneXfm (0x2311B0), restores DLE | 0.85 | T1 | verified |
| [0x21D428](../../src/game/gameplay/objects/target.cpp) | `OnCid21Remove` | OnLoRemove + RemoveDlEntry(&D_003E1474) | 0.85 | T2 | arbitration |
| [0x21D460](../../src/game/gameplay/objects/target.cpp) | `OnCid21Add` | OnLoAdd + AppendDlEntry(&D_003E1474) | 0.85 | T2 | arbitration |
| [0x21D498](../../src/game/gameplay/objects/target.cpp) | `CloneCid21` | same as CloneTarget for cid 0x15 (preserve DLE at 0xE0) | 0.85 | T2 | verified |
| [0x223F70](../../src/game/gameplay/objects/tu_223F70/func_00223F70.c) | `InitCid100` | InitSo; +0x?=-10, 3000.0, -1, -1; grf\|=6 | 0.85 | T2 | verified |
| [0x223FE0](../../src/game/gameplay/objects/tu_223F70/func_00223FE0.c) | `LoadCid100FromBrx` | LoadSoFromBrx then psophys(0xE8)->0xD4 = 0xD8 = 2.0 | 0.75 | T2 | parse |
| [0x224380](../../src/game/gameplay/objects/tu_223F70/func_00224380.cpp) | `UpdateCid100Contacts` | walks contact edges, picks neighbouring actor (0x224638, 0x224508) | 0.85 | T2 | verified |
| [0x224610](../../src/game/gameplay/objects/tu_223F70/func_00224610.c) | `FCid100State7Or11` | returns state in {7,11} | 0.60 | T2 | verified |
| [0x225A68](../../src/game/gameplay/objects/turret/CloneTurret.c) | `CloneTurret` | ClonePo then fix self pointer 0x1D0 | 0.85 | T1 | verified |
| [0x225AE8](../../src/game/gameplay/objects/turret/OnTurretActive.c) | `OnTurretActive` | OnPoActive then SetCmPolicy(g_cm,9,g_cm+0x5F0,..) or RevokeCmPolicy | 0.85 | T1 | verified |

Selected 60 of 359 named functions (T1 with confidence >= 0.75, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/gameplay-objects.md](../reference/functions/gameplay-objects.md).

## Open questions
- In-game identity of most placeholder classes (CID 0x47 family, 0x60, 0x62, 0x64-0x66, 0x74, 0x7A). Experiment: break each class's Init at load and look at the level.
- MISSILE vtable length 78 vs 79 (CONFLICTS #4); identity of CID 0x52 (TARMISS/SPLMISS?).
