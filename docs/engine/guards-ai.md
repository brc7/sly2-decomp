# Guards and AI: STEPGUARD, behaviours, perception, combat, pickpocketing, scripts

Sly 2 replaced Sly 1's per-guard SGS state machine with a **stack of behaviour objects** (BHV) per character, plus a
guard class that hosts perception, targeting, flags and engagement rules. This page describes that design.
Related: [player](player.md) (STEP and the same BHV stack drive AI partners), [game-progression](game-progression.md) (game events).

> **What changed in v2.** Runtime rounds 2 and 4 (S06-S41) observed calm, suspicious, alert, attacking, juggled, KO'd and removed
> guards. The BHV class words (BHVGUARD, BHVPATROL, ...) come from the r9 behaviour screen and were each confirmed on live stacks;
> the awareness modes of the root behaviour are now a runtime table. Round 1's reading of S05 as "a reaction to the bottle" is
> REFUTED (it is navigation over a pathlink). Round 7a (S63-S70, Paris L03 and the India hub L08) added guard species, ranged attacks
> (BHV173 = BHVSHOOT), the alarm-clock lure through stimuli, and the laser rigs.

## 1. STEPGUARD (stepguard.cpp, 0x205E70-0x20B840) and its subclasses

- CID 0x38, 0x1010 bytes, vtable 0x2DA568 (89 slots) `[C:runtime]`. Only 2 new virtuals: vt88 `PloChooseStepguardTarget`,
  vt89 `FStepguardSenseStimulus` `[H r7b stepguard.md]`.
- Subclasses (v2 names from the 43-level object data, CONVENTIONS 7.11; `[C:parse]` for the sole-class evidence, names are coinages):
  0x39 GOMER (the instances are swarmers), **0x3A GUARD** (plain guard; sole class of `npc_{moose,rhino,boar,vulture,pelican,pig}_guard`),
  0x3B DIMITRI, 0x3C NEELA, 0x3D RAJAN, 0x3E JEANBISON, 0x3F CARMELITA, 0x40 CONTESSA, 0x41 ARPEGGIO, 0x42 BEAR, 0x43 BEARCUB.
  One class per named character; the boss fights reuse them.
- **Species are proxy copies of one template per WE** (species = `PXR(guard+0x3C)->idProxy >> 16` = WE index) `[C:runtime runtime7a]`:

  | Species (WE) | Class | hp | capability +0xFE8 | Launchers (CID 0x70) | Attack entries (CID 0x78) |
  |---|---|---|---|---|---|
  | `npc_swarmer_rat` (Paris), `npc_swarmer_monkey` (India) | GOMER 0x39 | 40 | 0xF | `aim_manager_guard_pebble` | 1 |
  | `npc_swarmer_goat` | GOMER | 30 | 0xF | `guard_weapon` + 2 x `guard_pebble` | 4 (500-1000 x2, 100-500 x2) |
  | `npc_surprise_cobra` (ambush snake; calm = idle stack) | GOMER | 5 | 0 | none | 1 (0-600) |
  | `npc_boar_guard` (Paris), `npc_rhino_guard` (India) | GUARD 0x3A | 40 | 0x2 | `aim_manager_guard_weapon` | 4 (0-5000 x2, 0-1150, 0-400) |

  Every GUARD (boar, rhino) carries an in-world LIGHT child at the hand ("flashlight"); the detector does not reference it (cosmetic `[H]`). There are no elephant guards
  (elephants in L08 are SOs). L08 has 3 GOMER templates + 1 GUARD template -> 11 + 6 live guards.
- Paris hub: 9 guard objects, **only 3 in the world at a time**; they are pooled and re-placed as Sly moves (OnAdd/OnRemove by proximity) `[C:runtime]`.
  Templates are cloned at runtime (GUARD 2 live vs 1 record) `[C:runtime]`.
- The guard list D_003E1444 (DL, link at guard+0xFE0) holds exactly the in-world guards; STEPGUARD OnAdd/OnRemove (0x2079E0/0x2079A0) maintain it
  `[C:runtime]`, `[C:verified arbitration]`.
- Guard manager CID 0x7D (singleton, oid 0x656, `UpdateCid7DGuardMgr` 0x20B590) removes or respawns guards inactive for > 0.25 s `[C:runtime]` singleton, behaviour `[H r7b]`.

Guard fields confirmed at runtime `[C:runtime]`:

| Offset | Field |
|---|---|
| +0x26C / +0x270 | BHVSTK / stack depth |
| +0x2C0 | 64-bit flag word (section 2) |
| +0x2E8 | pointer to a CID 0x71 (oid 0x2CB) object in GOMERs (REFUTED: "mode") |
| +0x2EC | CID 0x71 guard script oid 0x2CB (all guards) |
| +0x330 | 1 after a KO (0 in fresh guards) |
| +0x338 / +0x33C | max hp (per species: 40 / 30 goat / 5 cobra; 360/8000 for DIMITRI/NEELA) / current hp |
| +0x38C / +0x3CC | target (0 -> JT when angry) / shared engagement record |
| +0x404 / +0x484 / +0x488 | loot list / count / next (section 5) |
| +0xDC0 / +0xDC4 | KO sub-state / time: -1 -> 0 (KO) -> 2 -> 4 (removed) (meaning HYPOTHESIS) |
| +0xEB8 / +0xEDC | attack oids / attack objects (CID 0x78 = CID120) |
| +0xF3C | 0 -> 1 set by BHVSTRIKE Add |
| +0xF60 | firing window (1 while BHVSHOOT may fire) [C:runtime S67] |
| +0xFE8 | capability bits (0xF swarmers, 0x2 GUARD (no shoot bit), 0 cobra) [C:runtime] |
| +0xF48 / +0xF4C | 300 / 1200 (ranges); +0xF50.. = +-45 / +-75 deg |
| +0xF64 | detector (CID 0x10 DETECTOR) |
| +0xF68 | root BHV point field (Splice optid 446 on STEPGUARD) |
| +0xFBC | special (gadget) state, -1 none |
| +0xFE0 / +0xFEC / +0xFFC | guard-list link / carry grabspot / busy count |

## 2. The guard flag word (+0x2C0)

- 64-bit word, flag n = bit n (0x207A20 returns `1<<n`) `[C:runtime]`.
- BHVs raise/lower their flag in Init/Remove (`AddStepguardSgf` 0x209890 / `RemoveStepguardSgf` 0x209970); every change sends message 7 to the
  guard; flag 3 broadcasts game event 0x14, flag 0xC event 0x16 `[H r7b]`.
- **The word always equals the union of the flags of the BHVs on the guard's stack** (59/59 guard-states, rounds 1-2) `[C:runtime]`.

| Flag | Raised by | Meaning | Tag |
|---|---|---|---|
| 2 | BHVIDLE 158 (+0x3C) | idle/stand leaf | [C:runtime] |
| 3 | BHVSTRIKE 159 (+0x3C) | attack animation (broadcasts game event 0x14) | [C:runtime] |
| 7 | BHVKO 163 | dead/KO | [C:runtime] |
| 9 | BHVANIM 157 (+0x3C) | moving (animation leaf) | [C:runtime] |
| 0xF | BHVLEAP 172 (+0x3C) | leap | [C:runtime] |
| 0x17 | BHVJUGGLED 177 | juggled / carriable (Murray takedown) | [C:runtime] |
| 0x18 | BHVATTACK 164 | attack execution | [C:runtime] |
| 0x1C | BHVGOTO 186 (+0x38) | running while tracking the player (-1 when walking) | [C:runtime] |
| 0x1D | BHVCOMBAT 165 | alert / combat mode | [C:runtime] |
| 0x1E | BHVPATROL 170 | calm mode | [C:runtime] |
| 0x1F | BHV171 | suspicious / search mode | [C:runtime] |
| 0x21 | BHV169 | guard-script runner | [C:runtime] |
| 0x2C | BHVPATHLINK 190 | traversing a pathlink | [C:runtime] |
| 0x2E | BHVPATROLPATH 192 | patrol-route walker | [C:runtime] |
| 5/6, 0x12/0x13/0x16 | - | damaged; held/thrown | [H r7b] |
| 0x20, 0x2A | - | not observed | open |

## 3. The behaviour stack and the guard modes

- Every STEP embeds `BHVSTK {STEP *pstep; int cpbhv; BHV *apbhv[8]}` at +0x26C, slots 0xF0 apart `[C:runtime]`. Behaviours are placement-new'd by a
  CID factory (`PbhvNew`, CIDs 0x9C-0xC6) and linked prev/next; popping one pops everything above; `UpdateBhvstk` runs top first and messages stop at the
  first handler `[H r6b bhvstk.md]`.
- **Busy protocol**: a busy behaviour increments `cBusyAbove` of all lower ones and the guard's busy count +0xFFC `[C:runtime]` (S05, S09).
- BHV (CID 0x9B, base 0x30: +0x10 pstep, +0x14 prev, +0x18 next, +0x1C fBusy, +0x20 cBusyAbove) has its **own** slot scheme (canon r9, applied in
  master_names_v2): 7 Init, 8 Start, 9 Close, 10 Update, 11 Update<Cls>Motion, 12 GrfGroundProbe, 13 FHandleMessage, 14 UpdateInput (players: the POCTL writer), 15 Set<Cls>State `[H r9]`, 14 `[C:runtime runtime6]`.

**Awareness mode.** The root BHVGUARD (156) holds the mode at +0x30 and stack slot 1 is the mode behaviour `[C:runtime]`:

| Mode (root +0x30) | Slot-1 BHV | Typical stacks (bottom -> top) | Flags | Seen |
|---|---|---|---|---|
| 1 calm | BHVPATROL 170 (holds the guard script) | patrol [156 170 169 192 186 167 157]; idle [156 170 158]; on a pathlink [... 192 186 190* 172*] | 9, 30, 33, 46 (patrol) | S02-S06, S36 |
| 2 suspicious | BHV171 (search: +0x30 state 1 go / 2 look, +0x34 tState, +0x38/+0x3C look timers, +0x44 watched stimulus) | [156 171 158]; walking [156 171 186 167 157] | 0x1F, 2 | S08, S65 |
| 4 alert | BHVCOMBAT 165 (+0x30 target, +0x34 state 0 close / 1 melee / 2 ranged / 3 pursue / 4 give up / 5 hands-off, +0x38 tState, +0x3C attack entry, +0x44 path zone, +0x50 last-known pos) | pursue [156 165 186 167 157]; melee [156 165 164* 159*]; ranged [156 165 173 157*] | 9, 28, 29 / 3, 24, 29 / 17, 29 | S07, S09, S66, S67, S70 |
| 7 juggled | BHVJUGGLED 177 | [156 177*] | 0x17 | S38 |
| 11 KO | BHVKO 163 (copies the killing zap at +0x50..) | [156 163*] | 7 | S39, S40 |
| 5 | script mode (`StartGuardScript` 0x171CF8) | | | [H r3b] |

`*` = busy. Transitions seen: calm -> alert when the detector sees Sly (S06 -> S07: target +0x38C 0 -> JT, engagement record set, BHVGOTO starts raising
0x1C, BHVWALKPATH goal +0xC8 = JT); suspicious -> attacking (S08 -> S09). In S41 the KO'd guard is out of the world with an empty stack.

**BHV classes** (CID, v2 word, role). `[C:runtime]` = role confirmed on a live stack; others `[H r9]`:

| CID | Class | Role | Tag |
|---|---|---|---|
| 0x9C | BHVGUARD 156 | root awareness controller; zap handler 0x176BE8 (zap 0x200000 -> hp = max(hp+dhp, 5) -> mode 7) | [C:runtime] |
| 0x9D | BHVANIM 157 | flag-keyed animation/locomotion leaf (flag 9 = moving) | [C:runtime] |
| 0x9E | BHVIDLE 158 | idle/stand leaf | [C:runtime] |
| 0x9F | BHVSTRIKE 159 | attack-animation leaf (busy) | [C:runtime] |
| 0xA0 | BHVHIT 160 | hit reaction | [H r9 0.6+] |
| 0xA3 | BHVKO 163 | knocked out / dead; flung, then removed | [C:runtime] |
| 0xA4 | BHVATTACK 164 | attack execution (chosen CID 0x78 attack at +0x3C) (REFUTED: "hide") | [C:runtime] |
| 0xA5 | BHVCOMBAT 165 | alert/combat mode (REFUTED: "path following") | [C:runtime] |
| 0xA6 | BHVPATH 166 | path base | [H r9] |
| 0xA7 | BHVWALKPATH 167 | path/goal follower (+0x30 SHAPE, +0xC8 goal object) | [C:runtime] |
| 0xA9 | BHV169 | guard-script runner (+0x30 CID 0x71) | [C:runtime] role |
| 0xAA | BHVPATROL 170 | calm mode container | [C:runtime] |
| 0xAB | BHV171 | suspicious / search (react, go to last known position, look, 4 search points, give up) | [C:runtime] role |
| 0xAC | BHVLEAP 172 | leap to +0x50 (REFUTED: "react to disturbance") | [C:runtime] |
| 0xAD | BHV173 = BHVSHOOT | ranged attack (+0x30 state, +0x34 tStart, +0x38 target): goat aiming at Sly on a high target point; entered from BHVCOMBAT state 2 when `FBhvcombatCanShoot` 0x175D00 says the target is unreachable | [C:runtime S67] |
| 0xAE | BHV174 | CID60 (NEELA) watch/alarm | [H r5b] |
| 0xB1 | BHVJUGGLED 177 | uppercut pop-up / juggle and carry (r9's BHVHELD) | [C:runtime] |
| 0xB4/0xB5 | BHV180/181 | DIMITRI flee logic | [H r3b] |
| 0xB6 | BHV182 | keep pace along a route | [H r2b] |
| 0xB8/0xB9 | BHVRAIL 184 / BHVPIPE 185 | rail / pipe traversal | [H r9 0.6+] |
| 0xBA | BHVGOTO 186 | go-to / look-at tracker (+0xA8 target) | [C:runtime] |
| 0xBE | BHVPATHLINK 190 | traverse a pathlink (CID 0x7C) | [C:runtime] |
| 0xC0 | BHVPATROLPATH 192 | walk a patrol route SHAPE | [C:runtime] |
| 0xA8, 0xC1-0xC6 | BHV168, 193-198 | player behaviours (168 walk-to-goal and 194 follow are runtime-confirmed on Murray, see [player](player.md#8-look-mode-recon-mode-and-the-ai-partner)) | [C:runtime] 168/194 |

BHV173 is now runtime-confirmed (role SHOOT; the v2 list still uses the placeholder). Other coined words (BHV161, 162, 175, 176, 178, 179, 183, 187-189, 191) were reverted to placeholders (CONVENTIONS 7.4). The 21 Sly 1 ALO
names that the early drafts put in this TU are gone (e.g. 0x178E98 = `InitBhvwalkpath`, 0x17D360 = `InitBhvgoto`).

## 4. Combat, KO and takedowns

- vt88 picks a target: forced target (+0x390, option 260), then force/ignore/prefer class lists, with hysteresis `[H r7b]`.
- **Engagement records** (0xAC bytes, list D_003E145C): two alerted guards share one record (+0xC target JT, +0x28 count 2, +0x2C guard list), and only one
  is busy attacking `[C:runtime S09]`: **"guards take turns attacking"**. `FTargetRecordAllowsGuard` 0x20A818 is the rule (on-screen restriction `[H r7b]`).
- Attacks are CID 0x78 objects (+0xEB8/+0xEDC): +0x50/+0x54 range, +0x58/+0x5C height, +0x68 angle tolerance, +0xAC owner, +0xB4 ASEG, +0xB8 ASEGA `[C:runtime]` (fitted to values; other fields `[H]`).
- **Ranged attacks** `[C:runtime S66-S67]`: goats throw `rock_throw_ammo` = BOMB (pool of 4) from their pebble launcher (zap 0x100, -10); rhinos launch `sword_missile` = ACCMISS
  (speed 10000, kind 0x100, -4 on Sly) from `aim_manager_guard_weapon` without entering BHVSHOOT (capability 0x2), probably through the attack entry's ASEG `[H]`.
  Launcher layout (`LaunchSoFromLauncher` 0x1D6B00) `[C:static]`: +0x50 posTarget, +0x60 source, +0x70/+0x80 predict, +0x90/+0xA0 offset, +0xA8 spin, +0xB0 pitch; launcher parent = the guard.
- Combat guards emit radius-4000 stimuli (template 4) with themselves as source: **alert calls** (`EmitStepguardStimulus` 0x20B140 from UpdateBhvcombat) `[C:runtime S67]`.
- **Juggle and takedown** (Murray, `[C:runtime S38-S41]`): the uppercut zap (0x200000) sends the guard to mode 7 / BHVJUGGLED with hp floored at 5
  (BHVJUGGLED's msg 0xC handler also floors it); juggle hits cannot kill. The finisher (zap kind 0x2, dhp -20) sends it to mode 11 / BHVKO, which flings
  it away (|v| ~860) and removes it about 1.7 s later.
- Special/gadget states (+0xFBC, `SetStepguardSpecial` 0x207030): **the values are gadget ids** `[C:runtime runtime4]`: 8 Size Destabilizer, 0xD Reduction
  Bomb, 0xE Temporal Lock, 0x15 Guttural Roar, 0x22 Insanity Strike, 0x25 Rage Bomb (timeouts 10/30/5 s `[H r7b]`).
- Animation bindings: up to 32 records at +0x4D0 plus three 26-entry zap-kind tables, chosen by `PanimbindBest` 0x209AD0 `[H r7b]`.

## 5. Perception

- **Vision**: each guard owns a DETECTOR (CID 0x10) at +0xF64; det+0xE0 = owner, det+0xFC count, det+0x100 entries `{plo, state, t, tFirst}` with states
  0 none / 1 seen / 2 lost; JT, MURRAY and BENTLEY are always present `[C:runtime]`. A guard turning angry has its JT entry go to state 1 (first seen 0.9 s
  earlier); a second alerted guard that lost Sly has state 2 `[C:runtime S07, S09]`. State changes call slot 3 (HandleSpliceEvent 0x1C, 3 args) via
  `SetDetectorTargetState` 0x1540F4; `GetDetectorTargetStateById` 0x154158 is its getter (was an OpenSSL label) (arbitration).
- **Hearing/stimuli** (the "noise" hypothesis is confirmed) `[C:runtime S08, S65, S67]`: refcounted stimulus objects (CID 0x93; `NewNoise` 0x156548: +0x10 ref, +0x14 template,
  +0x20/+0x30 pos, +0x40 source, +0x48 serial (++SW+0x3D68), +0x4C tCreated) on the active list DL 0x3E1488, made from templates (CID 0x75: +0x50 kind, +0x54 radius, +0x58/+0x5C
  height range, +0x60 ramp time; `SRadiusNoise` 0x156898). SW templates 0x2C2..0x2C8 have radii 1500/1500/750/2500/4000/1500/1000. Searching guards watch a stimulus in
  BHV171+0x44 and are re-targeted to newer ones (`SetBhv171Watched` 0x1725E0). **Alarm Clock**: the thrown clock's own CID 0x75 child (radius 3000, height -200..500, 3 s ramp)
  creates the stimulus; a monkey and a rhino that were already searching Sly's noises switched to it. Murray's slam emits one (event 0x1A5 = EmitSwNoise) `[C:static]`.
- **Lasers** (LASEN, CID 0x55; 7 rigs in L03, none in L08) `[C:runtime S63-S64]` + `[C:static 0x1DE478/0x1DE5D8/0x1DF3D0/0x1DF260]`: +0x198 beam count, +0x19C beams (0x150 each:
  two endpoints of 0xA0 {oid, kind -1/0 fixed/1 path, point or PATH*, cursor, up to 6 motion segments {start, speed, accel, duration, elapsed}}, +0x140 full length, +0x144
  visible length, +0x148 contacts), +0x1C0 speed scale. Moving beams use one constant-speed wrapping segment or an accelerate/cruise/decelerate profile; only rigs near Sly
  advance. A cut beam (495.6 -> 370) zaps the blocker: `ZapLasenBeams` -> `InflictZap {source LASEN, contact part, target, kind 0x1000, dhp -10}`.
- `FStepguardSensesEnabled` 0x208D70 disables both in KO/stunned/held states `[H r7b]`.

## 6. Pickpocketing (pickpocket TU 0x1F1768)

- Loot list: 16-byte entries `{oid, flags, key, count}` at +0x404, count +0x484, next +0x488 `[C:runtime]`. Coin entries are oid 0x2B3 with a count;
  loot entries oid 0x2B5 with key 0x45A+k; **key 0x45A+k = save var `loot.mplootkc[k]`** `[C:runtime]`.
- The pouch is CID 0x63 (CID99, an SO child; REFUTED "pouch PO"); loot launches toward Sly (LOOT +0x154 launch time) `[C:runtime]`. Sly's jts is 11 while pickpocketing.
- `GenerateStepguardLoot` rolls coin entries plus maybe one weighted loot item at PostLoad `[H r6b pickpocket.md]`.

## 7. Guard scripts and spawners

- **Guard script** (CID 0x71 = CID113, tu_1709A8): a list of 0x1C-byte commands built from Splice; BHV169 executes them `[H r3b]`, ownership `[C:runtime]`.
  Splice also applies CID113 scripts to proxies during ambush choreography (WE `f_nightclub_exterior_rig`) `[C:parse splice listing]`.
- **Off-screen spawner** (CID 0x77 = CID119, tu_173158): concealed spawn points, re-armed after 30/50 s, gated by the last alert `[H r3b]`.
- **Watchers** (tu_20CE28): stuck watch and hazard watch `[H r7c]`.

## Devtools findings (v3, 2026-11-01)

- **Per-species attack sets are level-filtered template children** `[C:parse+runtime entity_viewer]`: the CID 0x78 attack entries under a species template
  that pass the level's load filter (D_003E1110 = Lnn) equal the attack oids seen at runtime for rhino, goat (11 entries in the data, 4 load in L08 = the
  runtime 4), cobra (L08), rat and boar (L03) ([devtools/entity_viewer](../guides/devtools.md#entity_viewer)). The load-filter index itself is re-checked in v3 (2 / 8 in the states).
- ASEG channels of the 23 rhino clips name the rhino SO's own oid and an XF index < its XF count `[C:parse entity_viewer]`.

## 8. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x14A008-0x14B660 | tu_14A008 | low | route + BHV182 + CID 0x3F |
| 0x14FAC8-0x14FCA8 | tu_14FAC8 | low | guard-subclass CID 0x40 (2 fns); starts at 0x14FAC8 |
| 0x153D58-0x154A60 | detector | medium | guard vision detector CID16; file name guessed; detector cid 0x10 |
| 0x1553F0-0x1564E8 | tu_1553F0 | low | CID 0x3B STEPGUARD subclass + BHV180/181 |
| 0x1564E8-0x157200 | noise | low | stimulus/noise events heard by guards (hypothesis) |
| 0x16F148-0x16F2D0 | gomer | high |  |
| 0x1709A8-0x171F58 | tu_1709A8 | low | CID 0x71 guard script + BHV169 |
| 0x171F58-0x173158 | tu_171F58 | low | BHV171 guard search |
| 0x173158-0x174898 | tu_173158 | low | CID 0x77 off-screen guard spawner |
| 0x174898-0x17F090 | guardbhv | medium | guard behaviour (BHV) classes; not stepguard; r9_bhv_screen: guard behaviour classes BHV156-192 (tu_table "stepguard:8" is wrong); was labelled stepguard |
| 0x1B9F80-0x1BAB50 | tu_1B9F80 | low | CID60 + BHV174 |
| 0x1CBEB0-0x1CC158 | tu_1CBEB0 | low | CID61 (ra*?) |
| 0x1EEC60-0x1EF850 | bhvstk | low | BHV base + behaviour stack |
| 0x1F1768-0x1F2108 | pickpocket | low | guard pickpocket loot |
| 0x203948-0x205E70 | step | medium |  |
| 0x205E70-0x20B840 | stepguard | medium |  |
| 0x20CE28-0x20D5D0 | tu_20CE28 | low | guard stuck/hazard watch |

## 9. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x14A008](../../src/game/gameplay/ai/tu_14A008.cpp) | `LoadRouteFromBrx` | cid 0x7B (LO, 0x60) vt14: stack-alloc a 256-entry point table, LoadLoFromBrx (Splice options fill it via 0x14A178), copy used part to the heap | 0.75 | T1 | parse |
| [0x14A478](../../src/game/gameplay/ai/tu_14A008.cpp) | `FBhv182OffGround` | true if guard not in air and no ground found within 100/-250 | 0.85 | T2 | verified |
| [0x153D58](../../src/game/gameplay/ai/detector.cpp) | `InitDetector` | XFM::Init (0x231100), registers for update (0x1A6CA8(p,1)), sets flag 2 | 0.75 | T1 | verified |
| [0x154080](../../src/game/gameplay/ai/detector.cpp) | `SetDetectorTargetState` | Changes a target entry state: records times, notifies the detector through its HandleSpliceEvent (slot 3) virtual (msg 0x1C, args entry/old/new) and,  | 0.85 | T1 | verified |
| [0x154158](../../src/game/gameplay/ai/detector.cpp) | `GetDetectorTargetStateById` |  | 0.75 | existing | arbitration; replaces existing X509v3_data_type_by_NID |
| [0x1553F0](../../src/game/gameplay/ai/tu_1553F0.cpp) | `LoadDimitriFromBrx` | cid 0x3B (STEPGUARD subclass) LoadFromBrx: allocates 0x480 bytes for route records, STEPGUARD load, sets +0x1018 = INT_MAX | 0.75 | T1 | parse, verified, whole-game |
| [0x1559F8](../../src/game/gameplay/ai/tu_1553F0.cpp) | `CloseBhv181` | BHV181 slot 9 | 0.80 | T2 | verified |
| [0x16F148](../../src/game/gameplay/ai/gomer.cpp) | `InitGomer__FP5GOMER` | GOMER::Init: calls STEPGUARD::Init (0x205EB8) then sets a GOMER flag at +0xE78 | 0.85 | existing | arbitration; replaces existing RenderGomerSelf__FP5GOMERP2CMP2RO |
| [0x1709F8](../../src/game/gameplay/ai/tu_1709A8.cpp) | `InitCid113` | cid 0x71 (LO child, 0x60) Init: LO Init, then InitDl of the command list at +0x50 (link +0x18) | 0.75 | T2 | verified |
| [0x171CF8](../../src/game/gameplay/ai/tu_1709A8.cpp) | `StartGuardScript` | Sets the guard's BHV156 to mode 5 with the given script | 0.40 | T2 |  |
| [0x173158](../../src/game/gameplay/ai/tu_173158.cpp) | `InitCid119` | cid 0x77 (LO child, 0xB0) Init: LO Init, defaults 5500/1/1/1, respawn delays 30 and 50 s | 0.75 | T2 | verified |
| [0x1731B8](../../src/game/gameplay/ai/tu_173158.cpp) | `LoadCid119FromBrx` | Preallocates spawn-point (0x1C each), template and offset arrays, LO load, shrinks | 0.75 | T2 | parse |
| [0x174920](../../src/game/gameplay/ai/guardbhv/InitBhvanim.c) | `InitBhvanim` | BHVANIM Init: base Init, state=-1, flag(+0x3C)=0 (shared by BHV160) | 0.80 | T1 | runtime |
| [0x174950](../../src/game/gameplay/ai/guardbhv/StartBhvanim.c) | `StartBhvanim` | BHVANIM Start: raise guard flag +0x3C (AddStepguardSgf), pick anim binding for the current flags (PanimbindCur), enter state 0 (playing) or 1 (none -> | 0.80 | T1 | runtime |
| [0x1749C0](../../src/game/gameplay/ai/guardbhv/SetBhvanimState.c) | `SetBhvanimState` | BHVANIM state switch: 0 = become mover driver and StartAnimBinding (phase-continue walk cycle when flag 9), 1 = ClearBhvBusy, save walk phase, StopAni | 0.75 | T1 | runtime |
| [0x174B28](../../src/game/gameplay/ai/guardbhv/CloseBhvanim.c) | `CloseBhvanim` | BHVANIM Close: base Close, state 1 (stop anim), lower flag +0x3C | 0.80 | T1 | runtime |
| [0x174B68](../../src/game/gameplay/ai/guardbhv/UpdateBhvanim.c) | `UpdateBhvanim` | BHVANIM Update: finish (state 1) when the anim binding is done | 0.80 | T1 | runtime |
| [0x174BD0](../../src/game/gameplay/ai/guardbhv/BHVANIM_FHandleMessage.c) | `BHVANIM_FHandleMessage` | BHVANIM message handler: msg 9 = ASEG event of the playing binding: 0x15E root-motion translation, 0x25/0x26 lock/unlock mover, 0x27/0x28 set/clear bu | 0.80 | T1 | runtime |
| [0x174F08](../../src/game/gameplay/ai/guardbhv/UpdateBhvanimMotion.c) | `UpdateBhvanimMotion` | BHVANIM motion: drive guard velocity from the anim root motion (DriveStepguardFromAnim) while playing, else zero; face +0x38 look-at object | 0.75 | T1 | runtime |
| [0x174FB8](../../src/game/gameplay/ai/guardbhv/FBhvanimDone.c) | `FBhvanimDone` | BHVANIM state == 1 (animation finished) | 0.80 | T1 | runtime |
| [0x1751C0](../../src/game/gameplay/ai/guardbhv/InitBhvstrike.c) | `InitBhvstrike` | BHVSTRIKE Init: BHVANIM Init with flag 3 (attacking; flag 3 broadcasts game event 0x14) | 0.90 | T1 | runtime, verified |
| [0x1758A0](../../src/game/gameplay/ai/guardbhv/PentryChooseStepguardAttack.cpp) | `PentryChooseStepguardAttack` | Pick the first enabled attack entry whose range/height/angle/LOS accept the target, confirm via Splice event 0x18, then start it (SetStepguardAttack)  | 0.85 | T1 | verified |
| [0x1766D8](../../src/game/gameplay/ai/guardbhv/InitBhvguard.c) | `InitBhvguard` | BHVGUARD Init: state kind -1 | 0.75 | T1 | runtime |
| [0x176708](../../src/game/gameplay/ai/guardbhv/StartBhvguard.c) | `StartBhvguard` | BHVGUARD Start: enter mode 0 (immediately becomes patrol) | 0.75 | T1 | runtime |
| [0x176750](../../src/game/gameplay/ai/guardbhv/SetBhvguardMode.c) | `SetBhvguardMode` | BHVGUARD mode switch (record {kind, object, ...}): pops the child and pushes the behaviour for the mode: 1 patrol, 2 search(stimulus), 3 alert anim(0x | 0.75 | T1 | runtime |
| [0x176BE8](../../src/game/gameplay/ai/guardbhv/BHVGUARD_FHandleMessage.cpp) | `BHVGUARD_FHandleMessage` | BHVGUARD message handler: msg 12 (zap/damage) -> choose hurt/knockdown/KO by zap kind, reaction tables +0x8D0/+0xA70/+0xC10 and hit points; special za | 0.90 | T1 | runtime-corrected, runtime, verified |
| [0x177618](../../src/game/gameplay/ai/guardbhv/UpdateBhvguard.cpp) | `UpdateBhvguard` | BHVGUARD Update: top-level guard AI: patrol -> (stimulus) search -> (target) alert anim -> combat; returns to patrol when the child finishes; script/e | 0.75 | T1 | runtime |
| [0x177DC0](../../src/game/gameplay/ai/guardbhv/FBhvguardModeSame.c) | `FBhvguardModeSame` | Compare two mode records (kind plus the kind's key field) | 0.90 | T1 | runtime, verified |
| [0x178D90](../../src/game/gameplay/ai/guardbhv/SetBhvpathShape.c) | `SetBhvpathShape` | Set the SHAPE to follow and direction: find the start parameter (closest point) and, for open shapes, the end point | 0.85 | T1 | verified |
| [0x178EE0](../../src/game/gameplay/ai/guardbhv/UpdateBhvwalkpath.cpp) | `UpdateBhvwalkpath` | BHVWALKPATH Update: BHVPATH update, then set walk-anim speed and mover heading toward the target point; stuck timer when not moving | 0.90 | T1 | runtime, verified |
| [0x179A58](../../src/game/gameplay/ai/guardbhv/InitBhvpatrol.c) | `InitBhvpatrol` | BHVPATROL Init: raise flag 0x1E (patrolling), timers off | 0.80 | T1 | runtime |
| [0x179AA0](../../src/game/gameplay/ai/guardbhv/StartBhvpatrol.c) | `StartBhvpatrol` | BHVPATROL Start: take the patrol script (+0x2E8, or +0x2EC with capability 2); resume at nearest script point; walk (0) or stand (2) or hands-off (3) | 0.80 | T1 | runtime |
| [0x179B60](../../src/game/gameplay/ai/guardbhv/CloseBhvpatrol.c) | `CloseBhvpatrol` | BHVPATROL Close: stop, lower flag 0x1E | 0.80 | T1 | runtime |
| [0x179BA0](../../src/game/gameplay/ai/guardbhv/UpdateBhvpatrol.c) | `UpdateBhvpatrol` | BHVPATROL Update: alternate walking the script for a random time and standing idle for a random time; stand forever when the script ends; hands-off pa | 0.80 | T1 | runtime |
| [0x179D30](../../src/game/gameplay/ai/guardbhv/SetBhvpatrolState.c) | `SetBhvpatrolState` | BHVPATROL state switch: 0 push BHV169 script runner on the route (saves resume point), 1/2 push BHVIDLE, 3 push BHVIDLE flag 1 | 0.75 | T1 | runtime |
| [0x17BED0](../../src/game/gameplay/ai/guardbhv/func_0017BED0.c) | `InitBhv179` | BHVCIRCLE Init: raise flag 0x19, state -1 | 0.85 | T2 | verified |
| [0x17C928](../../src/game/gameplay/ai/guardbhv/func_0017C928.c) | `SetBhv183State` | BHVESCORT state: 0 push BHVGOTO to the offset point, 1 push BHVGOTO with run flag 0x1C to the object, 2 idle hands-off | 0.85 | T2 | verified |
| [0x17D360](../../src/game/gameplay/ai/guardbhv/InitBhvgoto.c) | `InitBhvgoto` | BHVGOTO Init: flag param +0x38=-1, state -1, path-graph version stamp | 0.80 | T1 | runtime |
| [0x17D3B0](../../src/game/gameplay/ai/guardbhv/StartBhvgoto.c) | `StartBhvgoto` | BHVGOTO Start: raise the caller-chosen flag (e.g. 0x1C run), stand, plan the route | 0.80 | T1 | runtime |
| [0x17D408](../../src/game/gameplay/ai/guardbhv/CloseBhvgoto.c) | `CloseBhvgoto` | BHVGOTO Close: state -1, lower its flag | 0.80 | T1 | runtime |
| [0x17D6C8](../../src/game/gameplay/ai/guardbhv/UpdateBhvgoto.c) | `UpdateBhvgoto` | BHVGOTO Update: re-plan when stale or the path graph changed; at a pathlink start traverse it; stuck -> relocate | 0.80 | T1 | runtime |
| [0x17DD70](../../src/game/gameplay/ai/guardbhv/func_0017DD70.c) | `InitBhv189` | BHVSTALK Init: raise flag 0x2B, state -1 | 0.85 | T2 | verified |
| [0x17E3A0](../../src/game/gameplay/ai/guardbhv/InitBhvpathlink.c) | `InitBhvpathlink` | BHVPATHLINK Init: raise flag 0x2C, state -1 | 0.75 | T1 | runtime |
| [0x17E888](../../src/game/gameplay/ai/guardbhv/func_0017E888.c) | `InitBhv191` | BHVRELOCATE Init: raise flag 0x2D | 0.85 | T2 | verified |
| [0x17EB48](../../src/game/gameplay/ai/guardbhv/InitBhvpatrolpath.c) | `InitBhvpatrolpath` | BHVPATROLPATH Init: raise flag 0x2E, param -1 | 0.90 | T1 | runtime, verified |
| [0x1EEC60](../../src/game/gameplay/ai/bhvstk.cpp) | `InitBhvstk` | BHVSTK init: pbhvstk->pstep = pstep | 0.85 | T1 | runtime |
| [0x1EEFC0](../../src/game/gameplay/ai/bhvstk.cpp) | `SetBhvBusy` | If !fBusy: fBusy=1; if pstep is a STEPGUARD (cid 0x38) increment its busy count (0x20B438, r7b IncrementStepguardBusy); increment cBusyAbove on every  | 0.85 | T1 | runtime |
| [0x1EF038](../../src/game/gameplay/ai/bhvstk.cpp) | `ClearBhvBusy` | Inverse of SetBhvBusy (fBusy=0, STEPGUARD busy count-- via 0x20B478, cBusyAbove-- below) | 0.85 | T1 | runtime |
| [0x1EF0D8](../../src/game/gameplay/ai/bhvstk.cpp) | `PopBhv` | Given a BHV* slot: if non-null call its vt9 (Close), decrement its step's cpbhv, null the slot | 0.85 | T1 | verified |
| [0x2039C0](../../src/game/actor/step.cpp) | `LoadStepFromBrx` | STEP vt14: LoadPoFromBrx, cache local base pos (+0x240 = ConvertAloPos(parent,this,0)), 0x1F8300 | 0.85 | T1 | parse |
| [0x204C60](../../src/game/actor/step.cpp) | `OnStepSoContactFreed` | STEP vt54: base no-op then drop the cached ground contact +0x208 if it is the freed one | 0.85 | T1 | verified |
| [0x204D48](../../src/game/actor/step.cpp) | `SetStepSpinRatio` | smpSpin = default D_002C3CB8 with svFast/svSlow *r and dtFast /sqrt(r); store r at +0x1F0 | 0.85 | T1 | verified |
| [0x2052E0](../../src/game/actor/step.cpp) | `MatchStepXf` | STEP vt38: copy pose of another object (vt31 matrix) incl. offset -+0x240 (TranslateToPos/RotateToMat), reset velocities; else SO::MatchXf | 0.85 | T1 | verified |
| [0x206518](../../src/game/gameplay/ai/stepguard/ResetStepguardToHome.c) | `ResetStepguardToHome` | STEPGUARD vt26: proxy/spawn reset: copy max values to current (HP etc.), clear BHVs, mover and flags, re-cache home pos/heading (+0x300/+0x310), capab | 0.85 | T1 | verified |
| [0x206748](../../src/game/gameplay/ai/stepguard/ClearStepguardState.c) | `ClearStepguardState` | STEPGUARD vt27: base, detach (0x20B210), clear counters, targets and target lists, special state -1, pickpocket state -1 | 0.85 | T1 | verified |
| [0x206900](../../src/game/gameplay/ai/stepguard/UpdateStepguardDetectTargets.cpp) | `UpdateStepguardDetectTargets` | Unless in a blind state, collect objects of the watched classes (+0x394) within 1500 xy, sort by distance, re-add them as detector targets keeping pre | 0.55 | T1 |  |
| [0x207030](../../src/game/gameplay/ai/stepguard/SetStepguardSpecial.c) | `SetStepguardSpecial` | Gadget/status override state +0xFBC (time +0xFC0): 8/0xD shrink (scale act -> 0.5), 0xE time-frozen (BHV157 ASEGA speed 0, velocity 0), 0x22/0x25 rage | 0.50 | T1 |  |
| [0x2079A0](../../src/game/gameplay/ai/stepguard/OnStepguardRemove.c) | `OnStepguardRemove` | STEPGUARD vt22: base (0x1F3E00), unlink from global guard list D_003E1444, stamp +0xFF8 | 0.85 | T1 | arbitration |
| [0x2079E0](../../src/game/gameplay/ai/stepguard/OnStepguardAdd.c) | `OnStepguardAdd` | STEPGUARD vt23: base (0x1F3EA8), append to guard list D_003E1444, +0xFF8 = FLT_MAX | 0.85 | T1 | arbitration |
| [0x207A20](../../src/game/gameplay/ai/stepguard/GrfFromSgf.c) | `GrfFromSgf` | Bit mask of guard flag n (1<<n, 0 for -1) | 0.85 | T1 | runtime |
| [0x2080A0](../../src/game/gameplay/ai/stepguard/ResolveZapReactions.c) | `ResolveZapReactions` | Resolve all 26 entries of a table (0x209130) | 0.85 | T1 | verified |
| [0x208E70](../../src/game/gameplay/ai/stepguard/GuardEvalInteractMode.c) | `GuardEvalInteractMode` | Rank how the hero can interact with this guard: 4 engaged with hero, 3 hero in pickpocket glint volume (+0x3F4/+0x3F8), 2 close (d<=0.7*f378, dz in LM | 0.55 | T1 |  |
| [0x2090D0](../../src/game/gameplay/ai/stepguard/ReleaseAnimBinding.c) | `ReleaseAnimBinding` | Free kind-2/3 payload objects (0x20C030/0x20CA20) and mark binding empty (-1) | 0.85 | T1 | verified |
| [0x2097C8](../../src/game/gameplay/ai/stepguard/LockStepguardMover.c) | `LockStepguardMover` | mover[1] = 1 (heading locked) | 0.85 | T1 | verified |
| [0x209818](../../src/game/gameplay/ai/stepguard/SetStepguardMoverRadTarget.c) | `SetStepguardMoverRadTarget` | unless locked, SetStepRadTarget(owner, rad) | 0.85 | T1 | verified |
| [0x209890](../../src/game/gameplay/ai/stepguard/AddStepguardSgf.c) | `AddStepguardSgf` | Set guard flag n, send message 7 (n,-1) to self, broadcast game event for flags in D_002C3D98 (3->0x14, 0xC->0x16) | 0.85 | T1 | runtime |
| [0x209970](../../src/game/gameplay/ai/stepguard/RemoveStepguardSgf.cpp) | `RemoveStepguardSgf` | Clear guard flag n, message 7 (-1,n) | 0.85 | T1 | runtime |
| [0x20A730](../../src/game/gameplay/ai/stepguard/AddTargetRecordGuard.c) | `AddTargetRecordGuard` | Append guard to record (stamp time and reset state on first) | 0.85 | T1 | verified |
| [0x20A818](../../src/game/gameplay/ai/stepguard/func_0020A818.cpp) | `FTargetRecordAllowsGuard` | Engagement token: fail if disabled or another engaged guard is busy (+0xF3C); kind 0 requires guard ready or inside camera frustum within heading tole | 0.40 | T2 |  |
| [0x20D1A0](../../src/game/gameplay/ai/tu_20CE28.cpp) | `EnableStuckWatch` | enabled = 1 | 0.85 | T1 | verified |

Selected 70 of 590 named functions (T1 with confidence >= 0.7, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/guards-ai.md](../reference/functions/guards-ai.md).

## Open questions
- Flags 0x20/0x2A; BHV161/162/175/176/178/179 (circle; still unseen)/183/187-189/191 roles.
- The rhino's launch path (attack entry vs +0xF60 reader); projectile damage fields on ACCMISS (+0x1EC/+0x1F0); the InflictZap de-dup list D_003E15C0 remover.
- Mode 3 and modes above 4 other than 7/11; what triggers the suspicious -> alert promotion (stimulus vs detector timer).
- Engagement on-screen restriction (two guards, one off-screen).
- Is CID119 the reinforcement spawner? Trigger an alarm and break 0x174490.
- Guard +0xDC0 KO sub-state block.
