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

## 2. PO and POCTL (po.cpp, 0x1C3D70)

- SW+0x3EB8 = PO count, `apo[]` at SW+0x3EBC (Paris hub: BENTLEY oid 8, JT oid 7, MURRAY oid 9, SAFEHOUSE PO oid 0x4357), current PO
  SW+0x3EFC, override SW+0x3F00 (0) `[C:runtime]`. Switching calls `OnActive(0/1)` (PO slot 78) and publishes `oidPoCurrent`.
- Each PO lazily gets a **POCTL** (CID 0x89, 0xE0 bytes) at PO+0x150: filtered stick polar values (+0x18 heading, +0x1C speed), button
  bits `grfbtn` (+0x30), press timestamps `atPressed[16]` (+0x88), state +0x80 (**1 only for the current player**, 0 for the others,
  -1 none) `[C:runtime]`. The AI writes the stick fields of a non-current player's POCTL directly (section 8).
- Non-current players are out of the world and parked at the safehouse next to the character-select PO; right after a level reload
  they stand at (40000,0,72) / (39000,0,42) `[C:runtime S30, S32-S35]`.
- `FInvulnerablePo` 0x1C3E90 (slot 76) = cheat bit 1 || hands-off (exact Sly 1) `[H r5 po.md]`.
- PO virtual slots: 75 FTakeDamage, 76 FInvulnerable, 77 JthsCurrent, 78 OnActive, 79 SetCtlState, 80 FCanBeHealed, 81 AddHpMp `[H r2b]` (78/79/76 verified).

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
| 12 | Walk | S21 | full stick without 0x8 (Lilly `JTS_Gait2`) | [C:runtime] |
| 9, 10, 13, 14, 16 | rush; walk in pickpocket range; air smash (square); triangle attack; pounce | - | code only | [C:static] |
| 3, 6 | hang from hook; ? | - | no hook (HND) in the hub | [H r4 jt.md] |

**JTAIR sub-states** (`nState` +0x2DC; `nKind` +0x2C4 = 1 for every jump seen) `[C:runtime]`: 1 rising (S12), 4 falling (S13, S14),
3 double-jump flight (S15, S16; fCanDoubleJump 1 -> 0; a double jump never stores kind 6), 10 homing onto a grab target (S19, plo = the RAIL),
0x11 ledge hang (S25; entered from falling via `FindJtEdgeGrabContact`, code 0x197500; X after 0.25 s -> `SetJtAirJump(3)` pull-up; 0x400 opens
look mode from the ledge). REFUTED: info.txt's "S14 rising".

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
- **Cane attacks are a jtcs overlay on jts 0 (or 5)**, not a jts `[C:runtime S68]`: jtcs 12 = first ground combo hit (`JtcsChooseJtAttack`; 13 = alternate
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

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x134648-0x1378D8 | bentley | medium | tu_table said shdanim |
| 0x13FF70-0x144638 | stepplayer | low | STEPPLAYER own code; file name unknown (b[r-u]*); Lilly: stepplayer_raw.c |
| 0x144638-0x144B28 | jtgoal | low | walk-to-goal component |
| 0x144B28-0x1471C0 | tu_144B28 | low | player BHV168/193-198; BHV168, BHV193-198 |
| 0x182C70-0x1848A8 | grabspot | medium | GRABSPOT family |
| 0x184F50-0x185130 | hnd | low |  |
| 0x185130-0x186610 | tu_185130 | low | meshsurf? CIDs 8/9 |
| 0x188488-0x18E540 | jt | high | tu_table said joy; was labelled joy |
| 0x18E540-0x18F908 | cane | medium |  |
| 0x18F908-0x191090 | hang | medium |  |
| 0x191090-0x195950 | grab | medium |  |
| 0x195950-0x19CBF8 | air | medium | jump/air |
| 0x19CBF8-0x19DE10 | mode | low |  |
| 0x19DE10-0x19F0A8 | pipe_jt | low | JT pipe state code |
| 0x19F0A8-0x19F3E8 | tu_19F0A8 | low | guardctl? |
| 0x19F3E8-0x1A0300 | places | low |  |
| 0x1A0B18-0x1A2BD0 | hurt | low |  |
| 0x1B4728-0x1B9F80 | murray | high |  |
| 0x1C3390-0x1C3D70 | pipe | high |  |
| 0x1C3D70-0x1C4E50 | po | high |  |
| 0x1CB908-0x1CBEB0 | rail | high |  |
| 0x1CC158-0x1CD5F0 | jthands | low | Lilly label jthands; link order says re* |
| 0x1D7238-0x1D7A30 | tu_1D7238 | low | PO CID70 with SM-state selection |
| 0x201D78-0x203948 | tu_201D78 | low | recon-photo prompt |
| 0x21F2E0-0x220D40 | throw | low | JTTHROW |

## 11. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x134770](../../src/auto/func_00134770.c) | `LoadBentleyFromBrx` | BENTLEY vt14: STEP LoadFromBrx base (0x2039C0) then 0x1A64E8(p, 0x76, D_002BC390) (load class-specific data table) | 0.75 | T1 | parse |
| [0x1357C0](../../src/hand/func_001357C0.c) | `FireBentleyProjectile` | Update aim point, clone the selected projectile template (+0x16DC/+0x16E0), launch it toward the aim point (0x1D6B00), set angular velocity, PrimeBomb | 0.45 | T2 |  |
| [0x135AD8](../../src/hand/func_00135AD8.c) | `HandleBentleyMessage` | BENTLEY vt5: HandleStepplayerMessage then Bentley-specific msg 9 kinds (0x216 spawn object from template, ...) | 0.85 | T1 | verified |
| [0x136A28](../../src/hand/CbFromSaak__F4SAAK.c) | `PgadgetBentleyFromId` |  | 0.70 | existing | arbitration; replaces existing CbFromSaak__F4SAAK |
| [0x136CB0](../../src/auto/func_00136CB0.c) | `BENTLEY_PchzSaveVarRoot` | BENTLEY vt103: returns "bentley" (save-variable namespace used for hpCur/mpCur/apukCur) | 0.85 | T1 | verified |
| [0x13FF70](../../src/hand/func_0013FF70.c) | `InitStepplayer` | STEPPLAYER vt7 Init: STEP Init (0x203948) then inits embedded components at +0x2C0,+0x4C0,+0x9E0,+0xDF0,+0xF00,+0x11B0,+0xF50,+0xFD0 and defaults (+0x | 0.85 | T1 | verified |
| [0x1403D0](../../src/hand/func_001403D0.c) | `UpdateStepplayer` | STEPPLAYER vt16 per-frame: STEP update 0x203FC0, input/actions, vehicle/ride component (+0xFD0), interact-target pick 0x142AF8, gadget/menu button han | 0.65 | T1 |  |
| [0x140F90](../../src/hand/func_00140F90.c) | `HandleStepplayerMessage` | STEPPLAYER vt5 message handler (msg 3 reset, msg 9 subkinds 0x15E/0x161/0x162/0x27B/0x27C/0x1C7/0x1C8, msgs 4/5/1/11) then forwards to 0x1EED78/0x1C3E | 0.85 | T1 | verified |
| [0x141A50](../../src/hand/func_00141A50.c) | `UpdateStepplayerGadgets` | Per frame for the 3 gadget slots (+0x1180, buttons D_002C3258 = 4,1,2): on press check vt99 FCanUseGadget and MP cost (D_002BCD00[id].xC vs MpCur) - e | 0.75 | T1 | runtime |
| [0x141CD0](../../src/hand/func_00141CD0.c) | `ChargeGadgetMp` | If id != -1 subtract the gadget's MP cost (D_002BCD00[id].d) from MpCur; stamp tGadgetUsed (+0x1304) | 0.75 | T1 | runtime |
| [0x142540](../../src/hand/func_00142540.c) | `StartStepplayerFollow` | Splice: clear message list, and if a then create behaviour 0xC2 (0x205438) with params and run its vt8 \| runtime6: Splice optid 446 on STEPPLAYER: cle | 0.30 | T2 | pending-fix; pending fix; v2 name StartStepplayerBhvC2 |
| [0x1440D0](../../src/hand/func_001440D0.c) | `BindStepplayerHpMp` | Bind hp/mp to save vars hpCur/mpCur under vt103 root (+0xE00, +0x12FC); if unset or forced, reset to max | 0.80 | T1 | runtime |
| [0x144480](../../src/hand/func_00144480.c) | `OnStepplayerActive` | vt78: PO OnPoActive, then vt105; when deactivated stop +0x1310 action and +0x1314=-1 | 0.80 | T1 | runtime |
| [0x144638](../../src/auto/func_00144638.c) | `InitJtgoal` | JTGOAL init: store owning STEPPLAYER at +0 | 0.85 | T1 | verified |
| [0x146298](../../src/auto/func_00146298.c) | `FBhv196Idle` | state == -1 | 0.85 | T2 | verified |
| [0x182CC0](../../src/auto/func_00182CC0.c) | `GRABSPOT_FGetGrabRange` | Default: no wrap range (return 0) | 0.85 | T1 | verified |
| [0x182EB8](../../src/auto/func_00182EB8.c) | `OnCid12Add` | vt23: LO Unfreeze + append to grab list D_003E13F0 | 0.85 | T2 | arbitration |
| [0x182EF0](../../src/auto/func_00182EF0.c) | `OnCid12Remove` | vt22: LO Freeze + remove from D_003E13F0 | 0.85 | T2 | arbitration |
| [0x183370](../../src/auto/func_00183370.c) | `OnCid34Add` | vt23: Unfreeze + append to its grab list | 0.85 | T2 | arbitration |
| [0x1833A8](../../src/auto/func_001833A8.c) | `OnCid34Remove` | vt22: Freeze + remove from grab list | 0.85 | T2 | arbitration |
| [0x1836F8](../../src/auto/func_001836F8.c) | `OnCid38Add` | vt23: Unfreeze + append to grab list | 0.85 | T2 | arbitration |
| [0x183730](../../src/auto/func_00183730.c) | `OnCid38Remove` | vt22: Freeze + remove from grab list | 0.85 | T2 | arbitration |
| [0x183998](../../src/auto/func_00183998.c) | `OnCid88Add` | vt23: UnfreezeSo + append to grab list | 0.85 | T2 | arbitration |
| [0x1839D0](../../src/auto/func_001839D0.c) | `OnCid88Remove` | vt22: FreezeSo + remove from grab list | 0.85 | T2 | arbitration |
| [0x183FB8](../../src/auto/func_00183FB8.c) | `OnCid35Add` | vt23 (shared with CID 0x24): Unfreeze + list | 0.85 | T2 | arbitration |
| [0x183FF0](../../src/auto/func_00183FF0.c) | `OnCid35Remove` | vt22 (shared with 0x24/0x25): Freeze + list | 0.85 | T2 | arbitration |
| [0x1844A0](../../src/auto/func_001844A0.c) | `OnCid27Add` | vt23: Unfreeze + append to D_003E1438 | 0.85 | T2 | arbitration |
| [0x1844D8](../../src/auto/func_001844D8.c) | `OnCid27Remove` | vt22: Freeze + remove from D_003E1438 | 0.85 | T2 | arbitration |
| [0x185150](../../src/hand/func_00185150.c) | `LoadCid8FromBrx` | CID 8/9 vt14: LoadXfmXfFromBrx, read vertex count + vectors (triangle mesh) into PvAllocSw, LoadOptionsFromBrx | 0.75 | T2 | parse, verified |
| [0x188488](../../src/hand/func_00188488.c) | `InitJt` | JT vt7 Init: base InitStepPlayer 0x13FF70, defaults (x19AC=15, jts/jtbs fields -1, limits -100/100/50/-10/250, LM 1..4500), gadget records x1FB0..x1FD | 0.90 | T1 | runtime, verified |
| [0x1885B8](../../src/hand/func_001885B8.c) | `LoadJtFromBrx` | JT vt14: STEP LoadFromBrx 0x2039C0, set x240 vector=(0,0,-75), BindLoOidTable(pjt,0xAD,table) | 0.90 | T1 | runtime, parse |
| [0x188F20](../../src/hand/func_00188F20.c) | `HandleJtMessage` | JT vt5 message handler: base STEPPLAYER 0x140F90 then switch msg (3 = SMA/ASEGA finished for x18E0/x1030 -> SetJtJts or 0x18E540; 9, ...) | 0.85 | T1 | verified |
| [0x189C40](../../src/hand/func_00189C40.cpp) | `RebuildJtXmg` | Foot placement for one foot: when foot clip changes rebuild XMG (sphere reachable by leg, vertical probe line) | 0.85 | T1 | verified |
| [0x18BCE0](../../src/hand/func_0018BCE0.c) | `UpdateJt` | JT vt16 Update: STEPPLAYER Update 0x1403D0, UpdateJtPre, actuator/aseg updates, UpdateJtStand, feet, rope/curve checks (SetJtJts 0x16/3/8), UpdateJtLe | 0.80 | T1 |  |
| [0x18C250](../../src/hand/func_0018C250.cpp) | `SetJtJts` | Set Sly's state (jts x18C8) and sub-state (jtbs x18D0): exit/enter logic for every state | 0.95 | T1 | runtime, verified |
| [0x18D760](../../src/hand/func_0018D760.cpp) | `JT_FCanUseGadget` | Can Sly use gadget id now (per-state allow lists; aiming state 0x17 allows only carried pocket item) | 0.90 | T1 | verified |
| [0x18F688](../../src/hand/func_0018F688.c) | `ChooseJtRushTarget` | ChooseJtAttackTarget(grf 8, 0.25, 1.0) and turn toward the found target (atan2), FixStepAngularVelocity | 0.85 | T1 | verified |
| [0x191A10](../../src/hand/func_00191A10.cpp) | `UpdateJtActiveHide` | Per-frame update of the grab context: dispatch on nState (hang/shimmy/climb kinds), choose nKind, SetJtHfdk, curve param clamp (0x193C98), commit anim | 0.75 | T1 | verified, arbitration |
| [0x193A68](../../src/hand/func_00193A68.c) | `GetJtRailLanding` | For a curve (rail) target at param t: landing position offset and a velocity along the curve tangent clamped by GSmooth limits 50 from the ends | 0.70 | T1 | verified |
| [0x193DE8](../../src/hand/func_00193DE8.c) | `UpdateJtInternalXpsHide` | Add contact constraints for the held object: state 4 -> vt82(0x23, plo, pos), state 3 rail/edge -> constraint along curve from velocity | 0.70 | T1 | verified, arbitration |
| [0x1940B0](../../src/hand/SetJtHfdk__FP2JT4HFDK.cpp) | `SetJtHfdk` | (existing name) Set hide/grab facing kind hfdk (+0x2C) and tHfdk=g_clock.t; kind 3 -> x34=1; 1/2 -> x34=2 if owner TN matches | 0.75 | existing | arbitration; replaces existing SetJtHfdk__FP2JT4HFDK |
| [0x194170](../../src/hand/func_00194170.c) | `PresetJtAccelHide` | If grab state 3 on a curve object: smooth the velocity component along the curve direction (GSmooth D_002BF700) and AddSoVelocity | 0.70 | T1 | verified, arbitration |
| [0x194F48](../../src/hand/func_00194F48.c) | `CompareJtGrabCand` | qsort comparator: -1 if a->g < b->g else 1 | 0.85 | T1 | verified |
| [0x1956D0](../../src/hand/func_001956D0.c) | `PloAcquireJtGrabMarker` | Acquire proxy id (PloAcquireProxy), unparent, Remove, fade in 0.25 s, place at candidate pos with identity rotation and zero velocity | 0.85 | T1 | verified |
| [0x195A00](../../src/hand/func_00195A00.cpp) | `SetJtAirJump` | Start a flight of kind nKind (n2 = directional clip, plo = object): reset record unless double jump (6), launch velocity from JtPlanLaunchVz, push alo | 0.80 | T1 | runtime |
| [0x1999D0](../../src/hand/func_001999D0.cpp) | `FCheckJtEdgeGrab` | Contact direction vs a shape feature: vertex -> sum FCheckJtSingleEdge over edges at the point (CpedgeFromGeomPoint, max 64) + BSP clip test; edge ->  | 0.70 | T1 | verified |
| [0x199D48](../../src/hand/func_00199D48.c) | `PxpFindJtBestEdge` | Pick the best supporting ledge contact of the player (walk contact groups, FCheckJtEdgeGrab, BSP point test), returning side and normal | 0.70 | T1 | verified |
| [0x19CA20](../../src/auto/func_0019CA20.c) | `RemoveJmtHierarchy` | JMT vt12: RemoveLoHierarchy then remove from g_dlJmt | 0.75 | T1 | verified |
| [0x19DF70](../../src/hand/func_0019DF70.cpp) | `PresetJtAccelPipe` | Accelerate Sly toward the pipe goal matrix (GetPipeGoalMat 0x1C3500 + Bezier frame) in states 0/4 | 0.85 | T1 | verified |
| [0x19E950](../../src/hand/func_0019E950.c) | `UpdateJtInternalXpsPipe` | Add the pipe constraint: evaluate curve at s, vt82(7, curve, pos, tangent, local point) | 0.85 | T1 | verified |
| [0x19F120](../../src/auto/func_0019F120.c) | `UpdateSwGuardOverride` | Each frame (from UpdateSw): if the g_sw+0x4748 override flag is set and older than D_002BF938 s, clear it (0x19F170(psw,0)) | 0.85 | T1 | verified |
| [0x19F590](../../src/hand/func_0019F590.c) | `ReturnJtToSafePlace` | Put the player back at a safe place (best recorded place if none given): ballistic launch toward it (DtBallisticMax, SetJtAirJump, JtbsChooseJtLanding | 0.55 | T1 | runtime |
| [0x1A0B40](../../src/hand/func_001A0B40.cpp) | `SetJtHurtState` | Hurt sub-state switch (leave/enter): JT-specific settings for CID 0x35, face/launch from hit direction, retract/apply hit clips, death states 8..12 | 0.85 | T1 | runtime, verified |
| [0x1B4888](../../src/hand/func_001B4888.c) | `LoadMurrayFromBrx` | MURRAY vt14: STEP LoadFromBrx 0x2039C0, BindLoOidTable(p,0x89,D_002C0270), allocates two 0xE0 hand/grip records looking up child XFs 0x200-0x205 (PxfF | 0.80 | T1 | parse |
| [0x1B4AA0](../../src/hand/func_001B4AA0.cpp) | `UpdateMurrayStateMachine` | Per-frame input-driven state machine (loops until state stable): idle/move -> jump (Cross 0x40), lift (Circle 0x20), punch (Square 0x80), alt attack ( | 0.70 | T1 | runtime-corrected, runtime |
| [0x1B6078](../../src/hand/func_001B6078.cpp) | `SetMurrayState` | Murray state switch (x13D0, 0..21): exit work for old state, entry work (clip selection, velocity reset, component state), shared ApplyAsegCur/compone | 0.65 | T1 | runtime |
| [0x1B7050](../../src/hand/func_001B7050.c) | `StartMurrayLiftGuard` | Same as 0x1B7000 with clip kind 1 (alternate lift clip x1444) | 0.85 | T1 | runtime, verified |
| [0x1B8450](../../src/hand/func_001B8450.c) | `MURRAY_FIgnoreIntersection` | MURRAY vt61(pso): base flags 0x144388; adds bit 3 for an object that is (part of) the recently released one x1430, or defers to JTTHROW filter in stat | 0.85 | T1 | verified |
| [0x1B8988](../../src/hand/func_001B8988.c) | `UpdateMurrayAttack` | State 11 combo: chain Square (0<->1) or 0x10 (2<->3) within combo window; takedown check; movement cancel | 0.85 | T1 | runtime, verified |
| [0x1B92F8](../../src/hand/func_001B92F8.c) | `UpdateMurrayLift` | State 4: wait for grab event (x1448), abort if object gone; after attach go to carry (state 5) when anim ends | 0.85 | T1 | runtime, verified |
| [0x1C3390](../../src/hand/func_001C3390.c) | `InitPipe` | PIPE::Init (vt7): InitShape + GRABSPOT init (+0x190); defaults | 0.85 | T1 | verified |
| [0x1C33E8](../../src/auto/func_001C33E8.c) | `OnPipeAdd` | PIPE vt23: base then append to pipe list D_003E13C0 | 0.85 | T1 | arbitration |
| [0x1C3420](../../src/auto/func_001C3420.c) | `OnPipeRemove` | PIPE vt22: base then remove from D_003E13C0 | 0.85 | T1 | arbitration |
| [0x1C34B8](../../src/hand/func_001C34B8.c) | `ClonePipe` | PIPE vt41 Clone | 0.85 | T1 | verified |
| [0x1C3DC8](../../src/hand/func_001C3DC8.c) | `LoadPoFromBrx` | PO::LoadFromBrx (vt slot 14): LoadSoFromBrx (0x1F31E0) then one stream helper 0x118420 | 0.85 | T1 | parse |
| [0x1C3E20](../../src/hand/func_001C3E20.c) | `HandlePoMessage` | PO::HandleMessage (vt slot 5): HandleLoMessage base, then on msgid 5 stop the wake ambient sound at +0x174 and release it | 0.88 | T1 | verified |
| [0x1C40F8](../../src/auto/func_001C40F8.c) | `FIsPoActive` | True if PO == effective current PO (override +0x3F00 else +0x3EFC) | 0.85 | T1 | verified |
| [0x1C4228](../../src/hand/func_001C4228.c) | `AddPoHpMp` | Virtual slot 81 (Lilly AddHpMp(dhp,dmp)): forwards (dhp,dmp) as message 0xC through vt slot 3 | 0.85 | T1 | verified |
| [0x1C4280](../../src/hand/func_001C4280.c) | `POCTL_ctor` | POCTL ctor: vptr D_002CA240, Reset, ppo(+0x7C)=ppo, nState(+0x80)=-1, append to g_sw POCTL list D_003E133C | 0.85 | T1 | verified |
| [0x1C42E8](../../src/hand/func_001C42E8.c) | `ResetPoctl` | Zero stick values (dx/dy/rad/s left+right), grfbtn=0, ClearJoyPressed(0xFFFF) | 0.85 | T1 | verified |
| [0x1C4468](../../src/hand/func_001C4468.c) | `UpdatePoctl` | Per-frame controller update: state 0 reset; state 1 read g_joy (or alt joy D_002E0D00) sticks+buttons (grfbtn = held & ~mask \| ppo->grfbtnForce), buil | 0.85 | T1 | verified |
| [0x1C4660](../../src/hand/func_001C4660.c) | `POCTL_GrfPressedWithin` | Mask of buttons in grf pressed within the last dt seconds (t - atPressed[i] <= dt) | 0.85 | T1 | verified |
| [0x1C46C0](../../src/auto/func_001C46C0.c) | `POCTL_GrfConsumePressed` | GrfPressedWithin then ClearJoyPressed on the result (consume the press) | 0.85 | T1 | verified |
| [0x1C4840](../../src/auto/func_001C4840.c) | `PpoHero` | Returns g_sw current PO (SW+0x3EFC) | 0.65 | T1 | runtime |
| [0x1C4A48](../../src/auto/func_001C4A48.c) | `PpoCur` | Effective current PO: override SW+0x3F00 if set else SW+0x3EFC | 0.90 | T1 | runtime, verified |
| [0x1CB9B0](../../src/auto/func_001CB9B0.c) | `OnRailAdd` | RAIL vt23: LO base (0x1A6178) then append to global rail list D_003E13CC | 0.85 | T1 | arbitration |
| [0x1CB9E8](../../src/auto/func_001CB9E8.c) | `OnRailRemove` | RAIL vt22: LO base (0x1A6150) then remove from D_003E13CC | 0.85 | T1 | verified, arbitration |
| [0x1CBA20](../../src/hand/func_001CBA20.c) | `CloneRail` | RAIL::Clone (vt41): CloneShape keeping own GRABSPOT link | 0.85 | T1 | verified |
| [0x202378](../../src/hand/func_00202378.cpp) | `UpdateReconTargetVisibility` | 4-ray occlusion test from origin to target -> state 1 clear / 2 blocked / 3 partial | 0.85 | T1 | verified |
| [0x2035A8](../../src/hand/func_002035A8.c) | `LoadReconTargetFromBrx` | CID 0x2E LoadFromBrx: defaults 75 / 0.25 / 5000, copy defaults | 0.75 | T1 | parse |

Selected 80 of 641 named functions (T1 with confidence >= 0.8, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/player.md](../reference/functions/player.md).

## Open questions
- jts 3/6/9/10/13/14/16 and JTHURT 8/9/11/12 not observed (needs a hook/HND level and fall deaths).
- Bentley states 1/2/4-11/13/14; hover pack; adrenaline burst; trigger-bomb damage path; dart hitting a guard.
- Murray toss (8), struggle (6), strafe (9), power move (12); a flop that lands on a guard.
- Respawn point after a death reload (start marker vs JTPLACES).
- Voltage/insanity damage path; cane contact -> zap path; D_003E1BEC (written by gadgets, never read statically).
- Unnamed STEPPLAYER slots 82-87, 90-92; JT slots 49/50/55/57/85/90-98/103/106.
