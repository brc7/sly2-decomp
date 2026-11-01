# Minigames and vehicle sections

Related: [bosses](bosses.md), [player](player.md) (PO, POCTL, character switching), [game-progression](game-progression.md),
[cutscenes](cutscenes.md) (RELAY `SetCtlState`, `HandsOff`), [hud-ui](hud-ui.md). CSV: `reports/bosses_minigames/bosses_minigames.csv`.

> **Coverage.** Static only: level files (splice_browser jobs, tasks, RELAYs, objects, Splice) and the decomp. No runtime capture reaches any minigame
> (the only runtime job so far is `m1_sat`). Tags: `[C:data]` level files, `[C:code]` decomp, `[H]` hypothesis.

## 1. Inventory

| Minigame | Jobs / tasks (title) | Levels | Played as (task +0xC0) | Engine class | Tag |
|---|---|---|---|---|---|
| **Bentley hacking** (top-down twin-stick shooter) | `m4_hack` "Train Hack" (6 stations); `m4_heist` `t4_heist4_hack1-3`; `m5_hack` "Crypt Hack" (3); `m5_heist` `t5_heist_free_carmelita_hack`; `m7_boat_hack` "Boat Hack" (3); blimp engine room (`a_blimp_ext_er` HACKWALLs, L38) | L14 L15 L22 L32 L38 | Bentley 8 -> HACKSHIP | HACK family 0x47-0x4C | C:data+code |
| **Mounted turret** | `m2_turret` "Battle the Chopper"; `m3_turret` "Blow the Dam"; `m5_heist` "Defend the Blimp"; `m8_turret` "Carmelita's Gunner"; grapple turrets in `m1_heist` (harpoon) and `m7_boat_hack` (grapples) | L08 L12 L17 L38; L02 L32 | Murray 9; Bentley 8; Bentley 8; Sly 7 | TURRET 0x45 | C:data+code |
| **RC chopper** | `m2_rc_chopper` "RC Bombing Run"; `m4_rc_chopper` "Wall Bombing"; `m6_rc_chopper` "Aerial Assault"; `m6_heist` rc_chopper_1/2 (Neyla) | L08 L14 L29 L30 | Bentley 8 | plain PO 0x32 `rc_chopper` | C:data; code H |
| **RC combat** (robot tank duel) | `m7_robot_war` "RC Combat Club" | L33 | Murray 9 | ROBOTTANK 0x4D | C:data+code |
| **Tank** | `m5_battlezone` "Tank Showdown" (6 kills); `m5_heist` tank chase | L17 | Murray 9 | plain PO 0x32 `p_tank_driveable` | C:data; code H |
| **Dance** (rhythm) | `m2_dance_audition` "Dominate the Dance Floor"; `m2_heist_dance` "Tango with Carmelita" | L11 | the DANCER object (oid 106193) | DANCER 0x44 + CID121 0x79 rhythm manager | C:data+code |
| **Safehouse / ThiefNet** | not a job; `safe_house` in every hub | L02 L08 L12 L14 L17 L27 L32 L38 | any | SAFEHOUSE 0x46 + ThiefNet object (screen TU) | C:data+code |
| Other gadget/vehicle jobs (no dedicated class found) | `m6_paraglide`, `m7_thermals` (paraglider), `m3_waterbug` "Water Bug Run", `m3_tntbarrel`/`m8_tntbarrel`, `m8_megajump`, `m6_train_red/blue` | various | Sly / Bentley | player gadgets + Splice | `[H]` |

"Murray's minigames" in the data are the turret (`m2_turret`), the tank (`m5_battlezone`, heist tank chase), RC combat (`m7_robot_war`), the brawls
(`m1_brawl`, `m2_boardroom_brawl`, `m4_brawl`) and the carry jobs (`m5_kidnap`, `m6_kidnap_bearcubs`) `[C:data]` (task character 9). The brawls and carries use the normal MURRAY player code.

## 2. Bentley hacking

### 2.1 Classes (hack TU 0x17F090-0x182AB8+) `[C:code]` names `[C:parse]` (WE names), HACK word coined (T2)

| CID | Class | Parent | Instances (WE) | Role |
|---|---|---|---|---|
| 0x47 | HACK (abstract, 0x1C0) | PO | - | shared hp (`HACK_TakeDamage` 0x17F1D0, `HACK_Die` 0x17F5F8, `ResetHack` 0x17F6B0, `AdjustHackXp` 0x17F1A0) |
| 0x48 | HACKSHIP (0x2B0) | HACK | `hack_ship` (L14 L15 L22 L32), `hack_ship_bounce` (L38); one per level, oid 17344 | **the player ship** |
| 0x49 | HACKENEMY (0x390) | HACK | `hack_enemy`, `hack_generator*`, `hack_gunship*`, `hack_mega_gunship*`, `hack_dreadnought`, `hack_mine_dropper*` | enemies and spawners (generators) |
| 0x4A | HACKBULLET (0x1E0) | HACK | `hack_bullet`, `hack_mine` | homing shots and mines |
| 0x4B | HACKLOCK (0x250) | HACK | none in any level `[C:data]`; `hack_lock` is ASEG-only | goal lock (code present, see 2.3) |
| 0x4C | HACKWALL (0x2A0) | HACK | segments in `p_prison_ext_hack`, `p_prison_int_heist`, `p_castle_heist_rig`, `c_sawmill_ext_boat_hack`, `a_blimp_ext_er` | destructible maze walls |

### 2.2 Input and ship behaviour

`UpdateHackship` 0x17F930, state 1 (active) `[C:code]`: reads the ship's POCTL (PO+0x150):
- heading `g218 = POCTL+0x18` and velocity `v220 = f(POCTL+0x18, POCTL+0x1C * k, 0)`. **The left stick drives the ship** (POCTL +0x18 heading, +0x1C speed are `[C:runtime]` in player.md).
- for each of its gun entries (+0x230 count, +0x234 stride 0x18): aims the actuator at `POCTL+0x28` and sets the gun SMA goal to **0x68 (fire) when POCTL+0x2C > 0**, else 0x66.
  **The right stick aims and fires** `[C:code]` for the dataflow; that +0x28/+0x2C are the right-stick polar values is `[H]` (they mirror the left pair).
- an engine sound or anim at +0x2A4 follows speed (`g2AC` eased toward POCTL+0x1C).
- `PresetHackshipAccel` 0x17FB50 (vt70): skipped in state 3, otherwise applies the heading rotation and velocity to the SO.
- `SetHackshipState` 0x17FD08: states 0..5 (widget anim 0x34F = health bar, sounds, actuators). `HACKSHIP_TakeDamage` 0x17FBE8 ignores hits in states 2-4, in hands-off
  or under a global flag; otherwise damage, health bar, **game event 0xF**, state 2 (hit) or 4 (defeated) `[C:code]`.

### 2.3 Enemies, walls, lock
- `UpdateHackenemy` 0x180668: per-state rules, tracks a live target (aim and turn). `SetHackenemyEntry*`, `SetHackenemyScale/Range` are Splice options (spawner parameters) `[C:code]`.
- `UpdateHackbullet` 0x1801B8: turn-rate-limited homing, self-destructs on a timer or on contact (`AdjustHackbulletXp`) `[C:code]`.
- HACKWALL: segments of pieces laid along shapes (`LayoutHackwallPieces`, `UpdateHackwallSegments`). Damage breaks the nearest segment (`HACKWALL_TakeDamage` 0x1828A8, explosion,
  game event) `[C:code]`.
- HACKLOCK: `UpdateHacklock` 0x181478 waits on the HACKSHIP singleton D_002DFB80, then drives it (`TriggerHackshipFromHacklock` 0x181728 -> `SetHackshipState(1)`) and RSMG records `[C:code]`.
  No level instantiates it `[C:data]`.

### 2.4 How a hack is entered (L14 `m4_hack`, `[C:data]`)
1. Each `t4_hack_stationN` is made available in order. The station is `p_hack_station` (CID35 trigger spot + RELAY + SM).
2. Activating the station task calls job hook 1286 with two ids (17350/17351 for station 1, ...): `SetSmaGoal` on the hack-arena SM 17348 (selects that station's course `[H]`).
3. RELAY 2410 runs: `HandsOff(1,3)`, Bentley plays ASEG 7629 at the terminal, WIDGET 2409 transition anims 783 -> 785, `AddObject` hack arena SO 17568 + TN 33852 + HACKSHIP 17344,
   **`SetCtlState(17344)`** (the controller now drives the ship), `RemoveObject` Bentley (8), counter WIDGET129 2407 and a snow emitter, `CutCamera`.
4. The arena lives in the same level file (WEs `p_prison_ext_hack` etc.). Ship death = game event 16 with the ship as argument: the job resets the hack (`s1334`) instead of `RestartJob`.
   Event 26 from a class derived from CID 26 fails the job `[C:data]`.
5. **Exit**: RELAY 2411 (same oid in L14 and L17) adds Bentley back and runs `SetCtlState(8)` (control back to Bentley) `[C:data]`. RELAY 2556 (L14) and 2762 (L17) are other
   `SetCtlState(17344)` entries (heist hacks) `[C:data]`.
6. `t4_hack_complete` ("Hack all six stations") -> RELAY 2426.

**Port needs**: the 6 HACK classes (about 70 functions), `SetCtlState` (controller hand-off to a non-player PO), WIDGET transitions, SM goals for the course, the game events.

## 3. RC chopper
- `rc_chopper` WE (oid 50723 in L08, L14, L29, L30) = **PO 0x32** with WIDGET, VOLBTN, EMITTER, ASEG, PROXY, EXPLG. There is no dedicated class `[C:data]`. Enemies: `npc_rc_fighter`, `npc_rc_shooter`,
  `npc_rc_jammer` (SO, CID98, BOMB, BARRIER) and Neyla's `npc_rc_boss` (L30) `[C:data]`.
- `m2_rc_chopper` script: job hook 1265 shows WIDGET 2064 (bar anim 847), `ADD`s PROXY 49605 (the chopper copy), matches it to its parent, `SetSoCnstrForce 1`, plays ASEG 17467. Completion
  sets `SW+0x3C88 = 49771` (a warp/return point `[H]`) `[C:data]`.
- **Flight code not identified** `[H: PO base vt70/vt16 with the generic SO physics, steered by POCTL; or an unnamed Bentley gadget path]`. The port must find it before these four jobs can run.

## 4. Mounted turret (TURRET 0x45, turret TU 0x2259D8-0x226550, conf high) `[C:code]`
- `InitTurret` 0x2259D8: pan limits (-1.5, 1.5), tilt limits (-0.7, 0.7). `OnTurretActive` 0x225AE8 (vt78) switches the camera to policy 9 (**CPTURRET**: `UpdateCpturret` 0x151950 follows the
  turret's world matrix, fov from TURRET+0x1BC).
- `UpdateTurret` 0x225B60 = aim (`UpdateTurretAim` 0x225BA0: pan/tilt from the stick or toward a target, clamped) + zoom (0x225E98) + fire (`FireTurret` 0x226250: on the fire mask, spawn a projectile,
  0x1CB250/0x1D6B00, and send Splice event 5).
- Scoring is Splice. Example `t5_heist_turret_destroy_planes` (L17): 20 planes, the bar (WIDGET 2783 anim 60261) rises 1/20 per kill, a random taunt every 3 kills, at 0 the task completes `[C:data]`.
  `m2_turret` spawns the tiger chopper (`i_tiger_chopper`, CID98 + seeker CID82 and ACCMISS bullets) after 10 s plus the dialogue length `[C:data]`.
- Grapple turrets (`f_grapple_turret` with a RAIL, `c_grapple_turret`) reuse TURRET for harpoon shots `[C:data]`.

## 5. RC combat: ROBOTTANK 0x4D (tu 0x1D1370) `[C:code]`
Health 100 (+0x55C max, +0x560 current, meter WIDGET anim 0x34F); up to 8 wheels, 8 weapons (0xB0 records) and 4 attachments, filled by Splice. **Mode 2 = player**
(`ReadRobottankInput` 0x1D2150: throttle from the POCTL stick, each weapon's button mask fires), **mode 1 = AI** (`UpdateRobottankAi` 0x1D2610: tactics 0 approach/orbit, 1 attack, 2 pursue,
3 back away; seeks CID108 ammo pickups). Wheel physics in `PresetRobottankAccel`/`AddRobottankWheelXps`. Zaps subtract hp and knock back (`HandleRobottankZap`). State 3 = destroyed.
One instance (`robot_tank`, L33). The opponent is presumably a cloned copy in mode 1 `[H]` (`CloneRobottank` deep-copies weapons).

## 6. Tank (`m5_battlezone`)
`p_tank_driveable` = PO 0x32 + WIDGET + CID112 (oid 72886, L17). Enemies `npc_un_tank_battlezone` (SO/SM/CID98) cloned from 6 PROXYs in `p_castle_battlezone` (job list of
`(proxy, xfm, xfm)` spawn triples). Shots: ACCMISS `tank_bullet_murray`. Tasks `t5_battlezone_kill_tank_1..6` `[C:data]`. Drive code not identified (as for the RC chopper) `[H]`.

## 7. Dance: DANCER 0x44 + rhythm manager CID121 0x79 (rhythm TU 0x1520C8-0x1539A8) `[C:code]`
- The task character field is the DANCER oid 106193, so the player "is" the dancer `[C:data]`. The rhythm manager singleton (D_002DE570, object 17900 in `i_palace_ballroom_rig`):
  `UpdateRhythmMgr` 0x152970 runs on **music time** (sound manager clock 0x1FE3F8). State 1 plays cue moves; state 2 posts msg 0x34 per prompt and scores misses.
- `SetRhythmMgrPrompts` 0x1531A8: up to 6 prompt records on a 440-px timeline (x from 100, y 468.8). `DrawRhythmMgr` 0x152C58 draws the 4 button glyphs per prompt.
- `JudgeRhythmPress` 0x1533B0: too early = fail; repeat = penalty; correct button = success move plus timing score (`GRhythmTimingScore` 0x1535A0: lerp on 1 - |t - tCue| / window,
  windows 0.4/0.4 s). Scores average over a ring of 16 (8-sample window, difficulty-scaled negatives); grades are oids 0x626-0x628.
- `SetDancerState` 0x147410 grabs the screen (D_002DD644 = the dancer) for a split/flip transition (`DrawDancerScreen`), changes the clock rate and takes hands-off.

## 8. Safehouse and ThiefNet `[C:code]`
- SAFEHOUSE 0x46 (`safe_house`, oid 17239 in every hub): `UpdateSafehouse` 0x1D72A8, in state 1, reads the joystick (pad D_002DFC00 or D_002E0D00) to cycle a selection SMA (character select,
  `HandleSafehouseMessage` -> `SetCharSelectState`). On a button press for the selected entry (list +0x18C, max 12 `{object, button, oid}`): oid **0x378 opens ThiefNet**
  (`SetThiefnetTab(D_003DA150, 0)`); other oids are looked up and fired (0x1DFD58).
- ThiefNet (screen TU): tab 0 = buy power-ups 7..0x2A unlocked by episode (`RefreshThiefnetItems` 0x1DBD10; the episodeBest gate is in game-progression section 6), tab 1 = sell loot.
  `UpdateThiefnet` 0x1DB870 does the coin check, confirm, deduct and `SetPowerupAcquired`. Opening stops the clock and takes hands-off (`SetThiefnetTab` 0x1DC260). Text callbacks
  0x1DC790-0x1DCAB0 fill the rows.

## 9. What the port must implement (priority order)
1. **`SetCtlState` / controller hand-off to arbitrary POs** and the task +0xC0 "play as oid" field. The hack (RELAYs 2410/2411) and the dance (RELAYs 1947/1960 `SetCtlState(106193)`)
   are entered this way `[C:data]`. No L08 RELAY does `SetCtlState` on the turret or `rc_chopper`, so those are entered another way `[H: PO activation from a VOLBTN or a Splice hook such as m2_rc_chopper sidebag 1265]`.
2. HACK family (6 classes), TURRET + CPTURRET, ROBOTTANK, DANCER + rhythm manager, SAFEHOUSE + ThiefNet: all decompiled and named, T1/T2.
3. The unidentified drive code for `rc_chopper` and `p_tank_driveable` (plain PO instances): find which code reads their POCTL.
4. Splice/RELAY features: `SetSmaGoal`, WIDGET anim phases, PROXY spawning, ticker `PostNotice`, `schedule-callback`, game events 15/16/26/61.

## 10. Open questions
- RC chopper and driveable tank movement code; the meaning of POCTL +0x28/+0x2C (right stick?).
- HACKLOCK has code but no instances; whether `hack_lock` ASEGs replace it.
- Which object marks `t4_hack_stationN` complete when the course is beaten (RELAY 2411 restores Bentley; its trigger is not traced).
- How the turret and RC chopper take control (no `SetCtlState` RELAY).
- The hack arena SM goal ids (17350.., 60231.., 77162..) as course selectors.
