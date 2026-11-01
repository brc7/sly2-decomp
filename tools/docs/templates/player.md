# Player characters: PO, STEP, STEPPLAYER, Sly (JT), Murray, Bentley

Related: [guards-ai](guards-ai.md) (shared STEP base and BHV stack), [animation](animation.md), [camera](camera.md),
[save-memcard](save-memcard.md) (hp/mp/gadgets as save variables), [game-progression](game-progression.md) (tasks start partner AI).

> **What changed in v2.** Runtime rounds 2-6 (PCSX2 save states S06-S62, Paris hub, an early and a late-game save) mapped the
> state machines of all three characters. The tables in sections 5-7 are `[C:runtime]` where a state was observed in RAM and
> `[C:static]` where the value is read from the state switch only. Several draft claims were refuted (Bentley's "BOMBs", JTMODE
> kind 1 = binocucom, STEPPLAYER+0xF38 photo selector, g_pjt = current player); they are marked REFUTED below. Round 7 (S63-S74) added the
> cane swing, laser damage, Thief Reflexes, Voltage Attack, ThiefNet and corrected the gadget table (+0x10 = unlock episode) and JTHURT+0x90.

## 1. Class chain

`SO (0x30) -> PO (0x32, 0x180) -> STEP (0x33, 0x2C0) -> STEPPLAYER (0x34, 0x1320, abstract) -> JT (0x35, 0x2060) / MURRAY (0x36, 0x1B00) / BENTLEY (0x37, 0x1880)` `[C:runtime]` (descriptor table).
STEPPLAYER is new in Sly 2: Sly 1's JT sub-structs were lifted into a shared base so all three characters run the same
movement components (the same JTGRAB/JTAIR/JTMODE offsets are live in Murray and Bentley) `[C:runtime]`.

| Global | Address | Holds | Tag |
|---|---|---|---|
| `g_pjt` | D_002E1E40 | **always Sly** (0xFB1580), even while Murray is played | [C:runtime] |
| `g_pmurray` | D_002F7900 | MURRAY (vtable 0x2D44C8) | [C:runtime] |
| `g_pbentley` | D_002DD5BC | BENTLEY (vtable 0x2CB230, object 0xD80E00 in the late save) | [C:runtime] |
| current player | SW+0x3EFC (`PpoCur` 0x1C4840) | the PO that has control (JT, MURRAY, BENTLEY or the safehouse PO) | [C:runtime] |
| save var `oidPoCurrent` | g_gs+0x0C | oid of the current player: 7 Sly, 8 Bentley, 9 Murray | [C:runtime] |

REFUTED: "g_pjt is the current player". Code that wants the controlled character must use SW+0x3EFC.
All three player pointers and the PO list are NULL during a level load (DeleteSw clears them) `[C:runtime S56]`.

**Which PO you play** comes from `oidPoCurrent` (g_gs save blob), not from the level: warping into L41 er_bentley loads the level with Sly `[C:live]`.

## 2. PO and POCTL (po.cpp, 0x1C3D70)

- SW+0x3EB8 = PO count, `apo[]` at SW+0x3EBC (Paris hub: BENTLEY oid 8, JT oid 7, MURRAY oid 9, SAFEHOUSE PO oid 0x4357), current PO
  SW+0x3EFC, override SW+0x3F00 (0) `[C:runtime]`. Switching calls `OnActive(0/1)` (PO slot 78) and publishes `oidPoCurrent`.
- Each PO lazily gets a **POCTL** (CID 0x89, 0xE0 bytes) at PO+0x150: filtered stick polar values (+0x18 heading, +0x1C speed), button
  bits `grfbtn` (+0x30), press timestamps `atPressed[16]` (+0x88), state +0x80 (**1 only for the current player**, 0 for the others,
  -1 none) `[C:runtime]`. The AI writes the stick fields of a non-current player's POCTL directly (section 8).
- Non-current players are out of the world and parked at the safehouse next to the character-select PO; right after a level reload
  they stand at (40000,0,72) / (39000,0,42) `[C:runtime S30, S32-S35]`.
- `FInvulnerablePo` 0x1C3E90 (slot 76) = cheat bit 1 || hands-off (exact Sly 1) `[H r5 po.md]`.
- PO virtual slots: PO vtable 0x2D5898 has 81 slots; slot 75 = 0x1F7588 is **inherited from SO** (same pointer as SO slot 75), so PO adds 6 slots, 76-81 `[C:static v3.1 re-check]`: 76 FInvulnerable (0x1C3E90), 77 JthsCurrent (0x1C3ED0), 78 OnActive, 79 SetCtlState, 80 FCanBeHealed, 81 AddHpMp (0x1C4228) `[H r2b]` (78/79/76 verified). Where FTakeDamage sits is open (v3 said slot 75: WRONG, red-team #131).

### Pad bits (POCTL `grfbtn`)

| Bit | Meaning | Tag |
|---|---|---|
| 0x40 | jump (X); pull-up from a ledge | [C:runtime] |
| 0x20 | circle: action / grab / wall press / pick up | [C:runtime] |
| 0x80 | square: attack | [C:runtime] |
| 0x10 | triangle: alternate attack (Murray uppercut) | [C:runtime] |
| 0x08 | run modifier (held = run, released = walk at full stick) | [C:runtime] |
| 0x04 / 0x01 / 0x02 | gadget slots 0 / 1 / 2 (button table D_002C3258 = {4,1,2}) | [C:runtime] |
| 0x400 | look mode (binocucom, JTMODE kind 0); not "gadget" as murray.md said | [C:runtime] + code |

The mapping to physical buttons (R1, L2, L1, R2, R3) follows the PS2 pad bit order and is HYPOTHESIS.

## 3. STEP (step.cpp, 0x203948)

Walking characters: heading/turn control (steer mode +0x1A4, radTarget +0x1A8, matTarget +0x1B0), ground contact (pxpBase +0x208),
gravity ratio (+0x260, option 225), spin ratio (+0x1F0, option 224), and the **behaviour stack** BHVSTK at +0x26C shared with guards
`[C:runtime]` (BHVSTK). Exact Sly 1 matches: UpdateStepPhys, SetStepGravityRatio, FixStepAngularVelocity, RotateStepToMat `[H r7b step.md]`.
`PbhvPushStep` 0x205438 / `PbhvFindStep` 0x205458 wrap the BHVSTK `[H r6b bhvstk.md]`.
Velocity: an SO's velocity is the XF at SO+0x58 (`GetXfVelocity` 0x22D348: XF+0x94 kind, +0x98 role, +0x9A state, +0xA0 parent,
+0xB0 velocity, +0xD0 v1); Murray runs at about 790 units/s `[C:runtime]`. `AddSoAcceleration` 0x1FAC30 (was "AddSoVelocity") adds to the
XF dv at +0xF0 `[H top50]`. Fix: `SetStepSpinRatio` 0x1B17B8 is really `SetCid96SpinRatio` (arbitration).

## 4. STEPPLAYER (TU 0x13FF70-0x144638, file name unknown; Lilly: stepplayer_raw.c)

Embedded components (Lilly layout; every row's base offset is now live-checked in at least one character) `[C:runtime]`:

| Offset | Component | TU | Role and runtime-checked fields |
|---|---|---|---|
| +0x2C0 | JTAIR | air 0x195950 | +0x2C4 **nKind**, +0x2CC plo (homing target), +0x2DC **nState**, +0x2E8 fCanDoubleJump (air.md had kind/state swapped); edge part +0x388, posEdge +0x390, normal +0x3A0 |
| +0x4C0 | JTGRAB | grab 0x191090 | +0x4C8 nState, +0x4CC nKind, +0x4D4 plo, +0x4D8 pspot (plo+0x190 for CID 0x0C/0x0D, plo+0x130 for CID27), +0x4DC uParam, +0x4E0 sParam, +0x4EC hfdk; private TN at JTGRAB+0x68 |
| +0x9E0 | JTPLACES | places 0x19F3E8 | cplace +0x9E4 (32 = full), aplace[32] at +0x9F0, 0x20 each {plo, pspot, ppart, tRemembered, qwPos}; newest first; recorded every 0.1-0.2 s while standing; qwPos = root z - 75 (foot) |
| +0xDF0 | JTHURT | hurt 0x1A0B18 | owner, nState +4, tState +8, hpMax +0xC (JT+0xDFC), php +0x10 (JT+0xE00 -> save var), pending PMF callback +0x1C/+0x20, zap record +0x30 {source, contact part, target, kind, dhp, hit pos +0x20, vector +0x30}, **+0x90 = post-hit mode (1 = further damage forced to 0)** (REFUTED: round 3's "alive" flag; runtime7a), +0x94 tHit |
| +0xF00 | JTMODE | mode 0x19CBF8 | owner, +0xF04 state (1 active, -1 off), +0xF08 tEnter, +0xF18 ASEGA, **+0xF20 kind (0 binocucom, 1 recon)**, +0xF24 hud {pjtmode, state +0xF28, widgets +0xF2C/+0xF30}, +0xF38 -> JT+0x1D40 recon widget record |
| +0xF50 | JTPIPE | pipe_jt 0x19DE10 | +0xF54 nState, +0xF5C pshape (PIPE), +0xF60 s (arc length) |
| +0xFD0 | JTGOAL | jtgoal 0x144638 | walk-to-goal with anti-stuck warp (used by BHV168) `[H r2b]` |
| +0x10DC / +0x10E4 | interaction | stepplayer | +0x10DC = current interaction candidate (guard), +0x10E4 = interaction mode (4 engaged, 3 pickpocket volume, 1 in range, 0 none) |
| +0x1180 | GADGETSLOT[3] | stepplayer | slot ids (= save var `<char>.apukCur[3]`), tActivated per slot |
| +0x11B0 | JTTHROW | throw 0x21F2E0 | carry, aim (state 4), aim target +0x1290, launch velocity +0x12A0; held object at +0x11C0 |
| +0x1320 / +0x1C30 | GAIT blocks | gait 0x20B840 | walk/run blending, footsteps `[H r7c]` |

REFUTED: STEPPLAYER+0xF38 is not a "photo-target selector"; it points at the JT+0x1D40 recon widget record (JTMODEHUD) `[C:runtime S31]`.

Systems owned by STEPPLAYER:
- **HP/MP** bound to save vars `<char>.hpCur` / `<char>.mpCur` (vt103 returns the namespace) `[C:runtime]`:

  | Character | hp save var | hp max | mp max |
  |---|---|---|---|
  | Sly | `sly.hpCur` g_gs+0x50 | 40 | 100 |
  | Bentley | `bentley.hpCur` g_gs+0x68 | 40 | 100 |
  | Murray | `murray.hpCur` g_gs+0x80 | 60 | 100 |

  A death reload refills hp to max (`BindStepplayerHpMp`); a medicine pickup (CID 0x5D) refills Sly 37 -> 40 `[C:runtime]`.
- **Gadgets**: 3 GADGETSLOTs at +0x1180 bound to buttons D_002C3258 = {4,1,2}; `g_game+0xC` = id of the last gadget used `[C:runtime]`.
  The gadget table **`g_agadgetdesc` D_002BCD00** = 43 x 0x20 bytes `{mode (-1 passive, 2 button, 1 other), owner oid 7/8/9, coin price,
  MP cost, **unlock episode** (-1 = never sold: vault rewards and specials), name string id, desc string id, icon?}` `[C:runtime runtime4]`, +0x10 corrected by
  runtime7b (ThiefNet lists ids 7-42 with `episodeBest >= unlock episode` that are not owned; REFUTED: "shop order"); save var `powerUps.mppukfAcquired`
  (g_gs+0x98..+0x9F, 43 bits): bit n = gadget n owned. **Modes**: 2 press (one-shot), 1 hold (slot state 1 while held), -1 passive `[C:runtime]`.
  Full table with prices and unlock episodes: `runtime7b/gadgets_confirmed.csv` (prices runtime-confirmed for 8, 13, 24, 29). Virtual hooks: 99 FCanUseGadget, 100 UseGadget, 101 UpdateGadget, 102 StopGadget, 105 OnGadgetsChanged.
- **Damage kinds** (zap record `kind`) seen at runtime: 0x100 guard projectile (rhino sword missile -4, goat rock -10), 0x200 guard melee (-3 small, -10 hits),
  0x1000 laser beam (-10; source LASEN), 0x2000 water (-9/-10 per immersion), 0x2 Murray takedown (-20), 0x200000 juggle (floors hp at 5), 0x80000 crossbow dart,
  grfzpk 4 slam (-20) `[C:runtime]`. `FTakeJtDamage` 0x1A19A0 maps kinds to JTHURT states `[C:static]` (table in section 5).
- **Interaction**: `UpdateStepplayerInteractCandidates` 0x140558 picks the best guard each frame into +0x10DC and sets the mode at +0x10E4
  (S02/S03/S06 mode 3 in a pickpocket volume, S07/S09 mode 4 engaged) `[C:runtime]`. +0x10DC is shared by Sly and Murray (not a Murray lock-on field).
- **Booty**: carries one booty object (+0x11A8), save vars `<char>.oidBooty`, `booty.aoidAssembly[15]` `[H r2b booty.md]`.

STEPPLAYER virtual slots beyond PO: 82-87 STEP-level hooks (partly unnamed), 88/89 `SetPlayerState`/`GetPlayerState` on a generic 0..8 state,
93 OnHeroDied (game event 0x10), 95 SetPlayerInteractMode, 96 GetStunHold, 97 GetCmFocusPos, 98 hands-off query, 99-102/105 gadgets,
103 save-var namespace, 104 Splice event 0x1A `[H r2b stepplayer.md]`.

## 5. Sly (JT, jt.cpp 0x188488 + component TUs)

JT vtable 0x2D2E98 (107 slots); state `jts` at +0x18C8, sub-state `jtbs` +0x18D0 (-1 except 4/5 in fight stance), cane state `jtcs`
+0x18D8 (-1 except 2 on a pipe) `[C:runtime]`. `SetJtJts` 0x18C250 is the switch; Lilly's JTS enum in that hand file is confirmed
wherever observed.

| jts | Name | Observed in | Notes | Tag |
|---|---|---|---|---|
| 0 | Stand | S02, S04-S06, S08, S10-S11, S30, S31 | binocucom (S31) = jts 0 + JTMODE running | [C:runtime] |
| 1 | Run | S20 | grfbtn 0x8 held | [C:runtime] |
| 2 | Air | S12-S16, S19, S25 | sub-state in JTAIR (table below); ledge hang is jts 2, not its own jts | [C:runtime] |
| 4 | Hurt / zap | S26-S29 | swim, knockback, death and drown are JTHURT sub-states | [C:runtime] |
| 5 | FightStance | S07, S09 | entered by `JtChooseGroundState` when `FFightStanceInRange`; jtbs 4 idle, 5 moving (was "JTS_Unk05") | [C:runtime] |
| 7 | Grab / target point | S17, S18 (wire), S23 (wall press), S24 (under table) | sub-state in JTGRAB | [C:runtime] |
| 8 | Pipe | S22 | jtcs 2, JTPIPE nState 1 | [C:runtime] |
| 11 | Pickpocket | S03 | guard loot transfer (guards-ai section 4) | [C:runtime] |
| 12 | Walk | S21 | full stick without 0x8 (Lilly `JTS_Gait2`); live: stick alone -> 12, stick + R1 (0x8) -> 1 Run `[C:live]` | [C:runtime] [C:live] |
| 13 | Air smash | live | jump + SQUARE: jts 2 -> 13 -> 0 (was code only) | [C:live] |
| 9, 10, 14, 16 | rush; walk in pickpocket range; triangle attack; pounce | - | code only; TRIANGLE (and CIRCLE) on the ground in Paris left jts 0 live, so 14 is context-dependent or misnamed | [C:static] |
| 3, 6 | hang from hook; ? | - | no hook (HND) in the hub | [H r4 jt.md] |

**JTAIR sub-states** (`nState` +0x2DC; `nKind` +0x2C4 = 1 for every jump seen) `[C:runtime]`: 1 rising (S12), 4 falling (S13, S14),
3 double-jump flight (S15, S16; fCanDoubleJump 1 -> 0; a double jump never stores kind 6), 10 homing onto a grab target (S19, plo = the RAIL),
0x11 ledge hang (S25; entered from falling via `FindJtEdgeGrabContact`, code 0x197500; X after 0.25 s -> `SetJtAirJump(3)` pull-up; 0x400 opens
look mode from the ledge). REFUTED: info.txt's "S14 rising". Live (input-driven) `[C:live]`: jump = nState 0 (1-2 frames) -> 1 -> 4 -> -1 on landing, kind 1; double jump = 2 (2 frames) -> 3. States 0 and 2 are new transient states.

**Double-jump apex** `D_002BF738` = **215.0** in the retail ELF (v3 and playtesting.md said 315.0) `[C:static v3.1]` `[C:live]`; `JtPlanLaunchVz` reads it. Live: rise 315.5 units at 215, 430.6 at 430 (repeatable), so jump height scales with it `[C:live]`.

**JTGRAB sub-states** `[C:runtime]`: wire = state 3 kind 0 on RAIL (CID 0x0D) oid 0x9EBD; wall press = state 0 kind 0 on CID 0x0C, hfdk 3,
circle held (pushes the grab context's private TN camera policy); under a table = state 8 kind 9 on CID27 (0x1B, VOL + GRABSPOT)
(pushes the CM+0x4C0 policy at priority 10); state 0xB = "use gadget on object" (Murray Fists of Flame, Bentley trigger bomb).

**JTHURT sub-states** (+0xDF4) `[C:runtime]` unless marked:

| State | Meaning | Observed | Callback pending |
|---|---|---|---|
| 3 | in water, alive (rig set up, hp -10 per immersion) | S26 | `ReturnJtToSafePlaceCb` 0x1A16A8 -> `ReturnJtToSafePlace` 0x19F590 (JTPLACES) |
| 7 | knockback from kinds 0x100/0x200 (state from `GetJtKnockbackVelocity`) or 0x8000; hp may reach 0, death follows when the clip ends | S27, S28 | `JtHurtEndCb` 0x1A1F68 |
| 8 / 9 | die forward / backward (by fall direction, from 8) | - | [C:static SetJtHurtState 0x1A0B40] |
| 10 | drown (dead) | S29 | cleared |
| 1 / 2 | zap kinds 0x800 / 0x1000 (laser): velocity zeroed | S64 (state 2, laser) | `JtHurtFallCb` 0x1A1FA8 |
| 4 / 12, 5 / 11 | kinds 0x400 / 0x4000 (alive / dead) | - | [C:static FTakeJtDamage] |
| 11, 12 | other death states (clip flags 0x21) | - | [C:static] |

**Death -> respawn = level reload** `[C:runtime S29->S30]`: SW frame counter 70582 -> 48 and `g_clock.t` 704 -> 0.85 (both reset; the global
`g_cframe` keeps counting), places table 32 -> 2, runtime spawns gone (8156 -> 8145 objects), JTHURT cleared, hp to max, objects re-allocated at
identical addresses. Save vars are kept (ccoin, afClue, `episode.nightclub.dt` keeps accumulating). **There is no separate checkpoint record**;
see [world-sw](world-sw.md#7-checkpoints-refuted). Sly reappears about 1000 units from the safehouse (start-marker source HYPOTHESIS).

Other JT facts:
- `UpdateJt` 0x18BCE0 (slot 16) order: STEPPLAYER Update -> `UpdateJtPre` (grab candidates, mode input, `JtChooseGroundState`, per-jts dispatch)
  -> actuators/gait -> `UpdateJtStand` (foot IK) -> smoothing -> cane -> attach checks -> `UpdateJtLean` `[H r4 jt.md]`.
- **Cane attacks are a jtcs overlay on jts 0 (or 5)**, not a jts `[C:runtime S68]` `[C:live]` (SQUARE on the ground: jtcs -1 -> 12 for about 0.5 s while jts stays 0): jtcs 12 = first ground combo hit (`JtcsChooseJtAttack`; 13 = alternate
  swing; combo table D_002BF448: 12 -> 14, 13 -> 15), entry time +0x18DC, swing ASEGA +0x18E0 on clip JT+0x1CE0 (ASEG 0x11D, tMax 0.833). Clip events: t 0 add the swing
  trail (CID105 0x104B); **t 0.133 `ShowSoPartsById(Sly, 0x14C)` opens the hit window, t 0.333 hides it** (SOPART 0x14C = part 17 at JT+0x1A88, entered into the collision
  tree while shown); t 0.5 messages 0x164/0x165 (finished, fight stance). How a contact becomes a zap is not traced. `SetJtJtcs` 0x18E540 `[C:static]`.
- Gadget records at JT+0x1FB0.. with ids 0x1A/0x25/0x26/0x29 `[C:runtime]`. `UseJtGadget` 0x18DA48 `[C:runtime runtime7b]` + code:
  - **Thief Reflexes** (29, hold): `SetClockRate(g_clock, slot 1, 0.25)` (Time Rush 42: 1.35), sfx 0x67 (Time Rush 0x66) into JT+0x2044, game event 0x3C; MP drains 2.0 per
    game second (Time Rush 1.0) through `DrainMp` 0x1441E8 with a fraction accumulator +0x1308. Everything (Sly included) runs on the slowed `g_clock.dt`; only the
    camera clock (slot 8) keeps normal speed. Ends on release or when `FCanUseGadget` fails (jts 4, look mode) -> `EndJtGadget` 0x18DD78 restores slot 1.
  - **Voltage Attack** (35) / Insanity Strike (34): `SetJtGrabState(0xB)`, anim event 0x27C charges 10 MP and starts a looping overlay ASEGA (x1310, x1314 = id) whose
    events add the arc mesh SO 0x1059 and a sound emitter (CID43 0x104E) to the cane; no timer (toggle off, character switch, msg 11). The damage modifier was not found.
  - **Alarm Clock** (26): throws a CID100 copy of `tossable_distract` that sticks to the world and creates a stimulus (radius 3000, 3 s ramp); searching guards are
    re-targeted to it ([guards-ai](guards-ai.md#5-perception)); reclaimed after ~6.9 s.
- Position = SO `pxfRoot`(+0x60)->matWorld row 3; JT mass 0.25, gravity z -2275; x19AC = 15.0f `[C:runtime]`.
- `SetJtHfdk` 0x1940B0 takes the hide sub-state struct (its +0 is the JT), not JT* (arbitration).

## 6. Murray (murray.cpp 0x1B4728)

State `x13D0`, entry time `x13D4`. No per-state function pointers exist in MURRAY: the active routine is the switch in
`UpdateMurrayStateMachine` 0x1B4AA0 (4 -> `UpdateMurrayLift` 0x1B92F8, 11 -> `UpdateMurrayAttack` 0x1B8988, 13 -> `UpdateJtGrabInput` 0x1911A8,
0/1/10 -> the input block) `[C:static]`, cross-checked with the snapshots `[C:runtime]`.

| State | Meaning | Observed | Details | Tag |
|---|---|---|---|---|
| 0 | idle | S34, S36, S47 | Fists of Flame and look mode run on top of state 0 | [C:runtime] |
| 1 | run | S33 | pad 0x8, blend x1320 | [C:runtime] |
| 2 | jump | S44 | JTAIR kind 1; x131C stamped at jump and at dive | [C:runtime] |
| 3 | air slam (flop) | S45, S46 | substate x13D8: 1 dive (anim event 0x1A4, vz -880), 2 impact; contacts with normal z > 0.7 collected in x13E4 get `InflictZap` (grfzpk 4, -20); event 0x1A5 = EmitSwNoise; no radius query | [C:runtime] + code |
| 4 | lift | S41 | x144C kind, x1450 sub (1 = object launched to the hands), x1448 grab event, grip records x13FC/x1400 | [C:runtime] |
| 5 | carry | S42 | prop parented to x13EC (Murray child SO oid 0x1FF = carry attach node); x1458 carry substate | [C:runtime] |
| 6 | scripted struggle | - | | [H r5b murray.md] |
| 7 | throw | S43 | release on anim event 0x1EB (`ThrowMurrayCarried` 0x1B7970); xy speed 3500 = x1414; fallback target 1500 units ahead (x1410) when no TARGET is in range | [C:runtime] |
| 8 | toss / alt toss (gadget 19 Diablo Fire Slam) | - | | [C:static] |
| 9 | strafe / lock-on | - | | [H] |
| 10 | walk | S32 | full stick, no 0x8, blend x1540 (murray.md called it "analog run") | [C:runtime] |
| 11 | attack | S38-S40 | kind x1810: 0/1 square combo, 2/3 triangle uppercut (hand/side choice in `StartMurrayAltAttack` 0x1B85D0), 4 takedown finisher; clip ap17FC[kind]; anim flags x1818/1C/20; v1830 = predicted root-motion end | [C:runtime] kinds 3, 4 |
| 12 | power move (gadget 20 Berserker Charge) | - | | [C:static] |
| 13 | generic grab (JTGRAB) | S37 | wall press: JTGRAB state 0 kind 0 on CID 0x0C, same as Sly's | [C:runtime] |
| 21 | turnbuckle launch (gadget 16) | S35 | `UseGadget` case 16 -> `SetMurrayState(21)` | [C:runtime] |

**Takedown** `[C:runtime S38-S41]` + code: the uppercut zap (kind 0x200000) puts the guard in BHVJUGGLED (177) with hp floored at 5;
`FTryMurrayTakedown` 0x1B70A0 then needs guard flag 0x17, BHV177 +0x50 < 3 and +0x5C == Murray; square -> state 11 kind 4 (kill, zap kind 0x2,
-20), circle -> BHV177 state 2 + `StartMurrayLiftGuard` on guard+0xFEC (not observed). See [guards-ai](guards-ai.md#4-combat-ko-and-takedowns).
**Fists of Flame** (gadget 15) `[C:runtime S36]`: `UseGadget(15)` sets x9BC = 15 and `SetJtGrabState(+0x4C0, 0xB)`; anim event 0x27C charges the MP
(100 -> 90) and starts an overlay ASEGA in x1310 (x1314 = 15). There is **no timer** (segment tMax = FLT_MAX): it ends when the gadget is pressed again
or on `OnStepplayerActive(0)` 0x144480 (character switch).
Field corrections `[C:runtime]`: x13EC is the carry attach node (not a "lift allowed" flag); x1850 is an int -1 (not a script pointer); x1830 is a vector.

Murray gadgets (D_002BCD00) `[C:runtime runtime4]`: 15 Fists of Flame (400 coins, 10 MP), 16 Turnbuckle Launch (400, 0 MP), 17 Juggernaut Throw
(passive = power-up 0x11), 18 Atlas Strength (passive 0x12), 19 Diablo Fire Slam (state 8), 20 Berserker Charge (state 12), 21 Guttural Roar
(JTGRAB 0xB on x10DC -> guard special 0x15), 22 Raging Inferno Flop (state 3 alt flop, x13E8). Late save slots {0, 16, 15}.

## 7. Bentley (bentley.cpp 0x134648)

State machine at +0x13D0 `[C:static 0x134850/0x135248]`, observed values `[C:runtime S49-S53]`:

| State | Meaning | Observed |
|---|---|---|
| 0 | idle (inputs: 0x40 jump, 0x400 look, 0x10 + `FCanUseGadget` -> 6, 0x20 grab) | S49, S52, S53 |
| 1 / 7 | locomotion (POCTL+0x30 flag 8 -> 1 else 7) | - |
| 2 | JTAIR | - |
| 3 | JTGRAB (nState 0xB on the bomb = trigger press) | S51 |
| 4 | JTHURT | - |
| 5 | near interact target | - |
| 6 | gadget use (+0x16B0 sub 0 start / 1 / 2 hold; held object +0x186C) | - |
| 8 | +0x1800 machine | - |
| 9 / 10 / 11 | scripted | - |
| 12 | JTTHROW (state 4 = aim, holding the bomb at +0x11C0) | S50 |
| 13 | pipe | - |
| 14 | timed clip (gadget 10 Adrenaline Burst, 2.0 s) | - |

Gadgets: slots {7 Trigger Bomb, 10 Adrenaline Burst, 12 Hover Pack}; six gadget records at +0x1824 `{id, template PROXY, live instance}`
read by `PgadgetBentleyFromId` 0x136A28 (was `CbFromSaak`) `[C:runtime]` (x16B4 = record pointer).

REFUTED: "Bentley throws real BOMBs primed with PrimeBomb(4.0)" `[C:runtime runtime5a]`:
- **Trigger Bomb** (gadget 7) = CID 0x65 (CID101, child of CID100 "gadget object", 0x1B0 bytes), pooled oid 0xF5B: +0x150 state, +0x15C gadget id,
  +0x164 SM, +0x16C owner, +0x170 auto-fire range 3000, +0x1A0 surface stuck to. States (`SetCid100State` 0x224EC8): -1 pooled, 0 held (parented to
  Bentley), 1 flight, 2 stuck, 3 detonate, 5 Health Extractor release; each state sets SMA goal D_002C4398[state]. Second gadget press ->
  `SetJtGrabState(0xB, bomb)` -> anim event 0x27C -> 0x2245B8 -> state 3; also fires if the owner is more than 3000 away. 0x224610/0x224688 test the
  gadget id, not the state (their names are wrong).
- **Crossbow dart** = ACCMISS (CID 0x51 : MISSILE : BOMB), pooled clones of oid 0x245 from PROXY 0x247 (Bentley +0x16DC). Fired by
  `FireBentleyProjectile` 0x1357C0 from the binocucom (pad 0x8 while aim state +0x16BC = 0) through the launcher CID 0x70 oid 0x249
  (`LaunchSoFromLauncher`); `PrimeBomb(4.0)` belongs to the dart (tDetonate = fire time + 4.0); speed 6000; zap kind 0x80000.

## 8. Look mode, recon mode and the AI partner

**Look mode is shared** by all three characters `[C:runtime S31, S47, S52/S53, S57, S61]`: the same JTMODE component at +0xF00 and one camera policy.

| JTMODE kind | Meaning | Camera | Locks |
|---|---|---|---|
| 0 | binocucom (player-started, pad 0x400) | CPLOOK at CM+0x450, priority 7, owner = current character; zoom max 7.0 | none |
| 1 | recon view (script-started by the RELAY command `StartJtMode(7, 8)`) | recon camera CM+0x520, priority 8 | input lock + no-pause counter (SW+0x3D84, +0x3D7C) |

REFUTED: "kind 1 = binocucom". Per-character pieces: the HUD widget pair +0xF2C/+0xF30 (Sly 0x35F/0x368, Murray 0x361/0x36A, Bentley 0x360/0x369),
the readout text (`BuildBinocReadoutText` 0x210FE0), and Bentley's dart machine (+0x16BC), which runs only while `FStepplayerInJtmode`.

**AI-controlled partner** `[C:runtime S62]` + code: `UpdateStepplayer` 0x1403D0 calls the top BHV's **slot 14** with the PO's POCTL (when +0xDF4 == -1
and JTGOAL is idle); the BHV writes the stick (POCTL +0x18 heading = atan2 toward the goal, +0x1C speed). Murray following Sly in L03 runs the stack
**[BHV194 follow, BHV168 walk-to-goal (top)]**: BHV194 state +0x60, leader +0x30 = JT, params +0x40/+0x44 = 85/180, follow point +0x50, bands +0x68 =
0.25/250/350/1.0/1.2 (InitBhv194 defaults), child +0x18 = BHV168; BHV168 state +0x4C (2 = arrived), goal +0x80. There is **no BHV156 root on player
stacks**. Started by Splice: TASK `t1_recon_brawl` evt19 calls option 446 on Murray with (JT, (85,180,0)); for STEPPLAYER, optid 446 is
`StartStepplayerFollow` 0x142540 (clear stack, push BHV 0xC2 = 194, Start); for STEPGUARD the same optid is 0x20A100 `SetStepguardRootBhvPoint`.
A later task calls it with the null object to stop following. Other player BHVs (168, 193 scripted go-to, 195-197 approach/interact, 198 Murray)
remain `[H r2b player_bhv.md]`.

**Safehouse character select** (SAFEHOUSE, CID 0x46, oid 0x4357 = D_003D4A54, vtable 0x2D80D0, Init 0x1D7238) `[C:runtime S48]`: becomes the current PO
(state +0x180 = 1 selecting). SM +0x184 (oid 0x6ACB) states 7 Sly / 8 Bentley / 9 Murray / 10 ThiefNet; entries +0x18C/+0x190 `{state, button 0x40,
target}` fire RELAY 0x2298E (Sly), 0x2298C (Bentley), 0x2298A (Murray) or open the ThiefNet shop (0x378, D_003DA150). The initial highlight comes
from `oidPoCurrent`; the camera is CPCAMERA (CM+0x4A8) at priority 10 on a CID 0x2C marker.

## 9. Grabbable world objects

- **GRABSPOT** is a mix-in (`X, GRABSPOT`) with a secondary 6-slot vtable. Runtime identities `[C:runtime]`: the wire is RAIL (CID 0x0D), the lamp post
  is PIPE (CID 0x0B), walls are CID 0x0C, the table is CID27 (0x1B, VOL + GRABSPOT); carried props use CID35/36 grabspots (CID 0x23/0x24) whose
  +0x180 is the carried object and +0x10C the grabber (vt51 `Cid35OnGrab` 0x184330 -> `StartMurrayLiftGrabspot` 0x1B7000). CID 0x13 (HND) has 0
  instances in the hub. OnAdd/OnRemove put them on the grab lists walked by `GatherJtGrabCandidates` 0x194A40 `[C:verified arbitration]`.
- There are **no JMTs (CID 0x28) in the hub** (D_003E1420 empty): Sly's "target points" are the GRABSPOT classes above `[C:runtime]`.
- CID35 objects also serve as **mission trigger spots** (`TriggerCid35` 0x184130 -> `ApplyTaskStates` -> `RequestTaskState(task, 4)`) `[C:runtime runtime6]`.

## 10. Source files

{{TUS}}

## 11. Key functions

{{KEYFUNCS:0x18C250 0x18BCE0 0x188488 0x1C4840 0x1403D0 0x142540 0x1B4AA0 0x1B6078 0x1A0B40 0x19F590 0x1357C0 0x224EC8 0x136A28 max=80 minc=0.8}}

## Open questions
- jts 3/6/9/10/13/14/16 and JTHURT 8/9/11/12 not observed (needs a hook/HND level and fall deaths).
- Bentley states 1/2/4-11/13/14; hover pack; adrenaline burst; trigger-bomb damage path; dart hitting a guard.
- Murray toss (8), struggle (6), strafe (9), power move (12); a flop that lands on a guard.
- Respawn point after a death reload (start marker vs JTPLACES).
- Voltage/insanity damage path; cane contact -> zap path; D_003E1BEC (written by gadgets, never read statically).
- Unnamed STEPPLAYER slots 82-87, 90-92; JT slots 49/50/55/57/85/90-98/103/106.
