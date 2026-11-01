# Boss fights

Related: [guards-ai](guards-ai.md) (STEPGUARD, which every humanoid boss derives from), [game-progression](game-progression.md)
(JOB/TASK, game events), [player](player.md) (POCTL), [cutscenes](cutscenes.md) (RELAY), [minigames](minigames.md),
CSV: `reports/bosses_minigames/bosses_minigames.csv`; the full job/task dump with titles, levels and characters: `reports/bosses_minigames/all_jobs_tasks.csv`.

> **Live update (v3.1, 2026-11-01)** `[C:live]`: a direct SW-transition warp to **L37 loads straight into the Jean Bison arena** with Sly ready to fight, and L43 loads the blimp deck (boss start not seen); see [live-pine](../guides/live-pine.md#5-level-warp). No boss behaviour has been captured yet.
>
> **Coverage.** No runtime capture reaches any boss. Everything here is static: the 41 level files (parsed by splice_browser: jobs, tasks,
> RELAYs, objects, Splice scripts and the string table) and the decomp. Tags: `[C:data]` = read from the level files (object records, task
> records, Splice code, localized strings), `[C:code]` = read in the decomp, `[H]` = hypothesis.

## 1. Summary

Sly 2 has **six boss fights**. Each is a JOB, or a group of TASKs inside a heist JOB, whose title is in the level string table `[C:data]`:

| Boss | Job (title) | Fight level | Played as | Boss object class | Driver |
|---|---|---|---|---|---|
| Dimitri | `m1_boss` "Printing Press Duel" | L05 nightclub/heist | Sly | DIMITRI 0x3B (GUARD child) | guard script + Splice hp bar |
| Rajan | `m3_boss` "Showdown with Rajan" | L12 temple/exterior | Sly, then Murray | RAJAN 0x3D (STEPGUARD child) | Splice: 5 tasks |
| The Contessa | `m5_heist` "Operation: High Road", tasks `t5_heist_boss_1_*` / `t5_heist_boss_2_*` | L17 castle/exterior | Sly | CONTESSA 0x40 (STEPGUARD child) `[H]` via PROXY | Splice: two battles |
| Neyla (RC duel on the train) | `m6_heist` "Operation: \"Choo-Choo\"", `t6_heist_rc_chopper_1/2`, `t6_heist_complete` "Defeat Neyla" | L30 train/train_c | Sly on the roof; the RC chopper is Bentley's `[H]` | `npc_rc_boss`: SO/CID98/CID112/CID113 + 13 RELAYs (no NEELA object) | Splice + RELAYs |
| Jean Bison | `m7_boss` "Brains over Brawn" | **L37 sawmill/boss** | Bentley | JEANBISON 0x3E (STEPGUARD child, no own code) | Splice: 20-hit counter, 4 phase tasks |
| Clock-La | `m8_turret` "Carmelita's Gunner" (L38) then `m8_boss` "Showdown with Clock-La" | **L43 blimp/boss** | Sly (Murray and Bentley in scripted tasks) | `npc_clockwerk` assembly: SO + SM + CID98 + VOLBTN + CID112, no STEPGUARD | Splice: SM goals, 20-hit counter |

All rows `[C:data]` (job and task names, titles, the task "level" and "character" fields, object classes) unless marked.

Level names: the `epNN` tag in the world-file names is the level-name index. Reading it against the per-world level lists in the
save schema gives one global list: 0 exterior, 2 cellar, 3 disco, 4 heist, 5 moulinrouge, 6 waterpump, 7 interior, 8 basement, 9 ballroom, 10 vault,
11 waterrails, 12 guardbreak, 13 wolftomb, 14-17 ewoktrainer/ewoktomb/ewokhall/ewokwater, 18-21 cabins/train_a/train_b/train_c, 22 mulch, 23 burn,
24 lighthouse, 25 bearcave, **26 boss**, 27 er_murray, 28 er_bentley, 29 er_jt. This fits all 41 files `[H]` (26 = boss is already `[C:runtime]` in world-sw).
Only two levels are dedicated boss levels (L37, L43). The other fights run inside ordinary levels (L05, L12, L17, L30).

**TASK field +0xC0 ("character") is the oid of the PO the player controls** `[C:data]`: 7 = Sly (`jt` root oid 7), 8 = Bentley (root 8), 9 = Murray
(root 9), and 106193 = the DANCER object in the dance tasks. This refines game-progression section 1, which reads 7 as a plain character id.

Arpeggio has a class (ARPEGGIO 0x41, 0 own vtable slots) but only one instance, in L39 (blimp interior, `npc_arpeggio`). He is not fought. The final
boss is Clock-La `[C:data]`.

## 2. Shared boss machinery (what the port must implement once)

1. **Boss = STEPGUARD with hit points.** STEPGUARD +0x338 = max hp and +0x33C = current hp. Every boss script reads them through Splice field refs
   (`STEPGUARD+0x338*`, `STEPGUARD+0x33C*`) and sets the hp bar to cur/max `[C:data]`. The hero's hp is the same field on `PpoHero` (Rajan and Contessa
   scripts test `PpoHero.STEPGUARD+0x33C <= 0`) `[C:data]`. Damage arrives through zaps (SO vt75, `SetSoZapResponse` masks in Splice) `[H]`.
2. **Game events** used by every fight (decimal ids, delivered to the job and task `evt17/game-event` handlers) `[C:data]`:
   - 21 = a guard took damage (arg = the guard): update the hp bar.
   - 23 / 24 = a guard was defeated (arg = the guard): complete the battle task, sometimes after a delay (Rajan 4 s).
   - 16 = hero died: `RestartJob`, or fire a retry RELAY (Dimitri on retry counts 1, 6, 11, ... `TASK+0x64 mod 5 == 1`; Bison in L37 always fires RELAY 3277).
   - 15 = hero hp change. Rajan and the Contessa use it to detect a "capture" (hero hp <= 0 while the intro task count is <= 2) and play a capture RELAY.
   - 29 = fire the shared RELAY 1749. 31 = reset the heist job and force its intro task (all boss jobs).
   - 55 / 56 (Contessa battle 2 only) toggle a TN target `[C:data]` (meaning `[H]`).
3. **Boss health bar** = a WIDGET (one per world, e.g. 1930 Paris, 2340 India, 2796 Prague, 3275 Canada, 3467 blimp) shown with
   `ShowWidget`, `PlayWidgetAnim(847)` and `SetWidgetAnimPhase(phase, 847)`: anim 847 is the bar fill `[C:data]`.
4. **Guard script objects** (CID113 0x71): `StartGuardScript(cid113)` and `SetGuardBhvguardMode1(cid113)` (Splice methods 441/442) give a boss its
   pattern. Dimitri uses 1923, Rajan 97679/97680, the Contessa 2793 `[C:data]`. The CID113 record format is not decoded `[H: a waypoint and behaviour list]`.
5. **Boss music**: `PushMusic(n)` on battle start and `StopMusic(n)` at the end: 32 Dimitri, 22 Rajan, 13 Contessa, 6 Bison, 12 Neyla chase, 20 turret `[C:data]`.
6. **Proxies**: the Rajan, Contessa and Bison fights take the boss from a PROXY (`EnsureProxyCopy`/`LockProxyPlo`/`PloCurProxy`, the PROXY system
   documented in world-sw), so retries respawn a fresh copy `[C:data]`. Which WE each proxy instantiates is not in the splice_browser output `[H: npc_boss_*]`.

## 3. Per-boss detail

### 3.1 Dimitri: `m1_boss`, L05 (nightclub "heist" level)

Tasks: `t1_dimitri_boss_intro` (L05, Sly) -> `t1_dimitri_boss_start` -> `t1_dimitri_boss_fight` -> `t1_dimitri_boss_complete` ("Get the Clockwerk Tail Feathers") `[C:data]`.
- intro/start `evt19`: `ADD` DIMITRI 122332, `RestartStepguard`, `StartGuardScript(1923)`. On the first attempt it TRIGGERs RELAY 1919 / 1921, on retries 1922 `[C:data]`.
- fight: shows WIDGET 1930 at full, music 32. Game event 21 for Dimitri sets the bar to hp/max. Event 23 or 24 completes the task `[C:data]`.
- **Class** DIMITRI 0x3B (0x1020 bytes, parent GUARD 0x3A) adds a **route table**: `LoadDimitriFromBrx` 0x1553F0 allocates 0x480 bytes of route records,
  `AddDimitriRoute` 0x155530 appends 4-oid records, `PostDimitriLoad` 0x155450 resolves them, and `PrteDimitriFarthest` 0x155868 picks the route whose near end is
  **farthest from the player** `[C:code]`. So Dimitri flees along authored routes away from Sly. The route records are also used by `m1_follow_dimitri` (DIMITRI is in L02 too) `[H]`.

### 3.2 Rajan: `m3_boss`, L12 (temple exterior)

Tasks `[C:data]`: `t3_boss_intro` (Sly) -> `t3_boss_lightning` ("Get to Rajan") -> `t3_boss_sly_capture` (Murray, 9) -> `t3_boss_pool` -> `t3_boss_complete` ("Defeat Rajan", Murray).
- Job `evt19`: removes the heist rigs, then for each class id in `(39 13 19 11 9)` removes every instance (clears the level of other gameplay objects), adds the CID39 and
  PIPE objects of the boss set, music 22 `[C:data]`.
- Phase 1 (lightning, Sly): game event 15 + hero hp <= 0 -> mark captured, after 0.66 s fire RELAY 2158 (Sly is captured; Murray takes over) `[C:data]`.
- Phase 2 (pool, Murray): acquires the Rajan copy from PROXY 97688, sets its detector and STEPGUARD+0x3D4 flags, points TN 97689 at him, removes the
  WATER 97690 (pool drained or entered) `[H]`, turns on `SetRajanCameraFocus` (camera policy 6 framing Murray and Rajan; RAJAN vt90 0x1CBF90 `[C:code]`) and
  sets `SetCmMrdRatio 2`. The hp bar follows event 21, and **below 80% hp** RELAY 2341 fires once (mid-fight scripted event). Event 23/24 -> complete after 4 s `[C:data]`.
- **Class** RAJAN 0x3D: `InitRajan` 0x1CBEB0; `Rajan_FSenseStimulus` (vt89) doubles sense range and arc for stimulus 0x246 `[C:code]`.

### 3.3 The Contessa: `m5_heist` boss tasks, L17 (castle exterior)

Sequence inside the heist `[C:data]`: paraglide to blimp -> free Carmelita (hack, L22) -> Neyla chase -> turret "Defend the Blimp" (Bentley) ->
**boss 1** (`t5_heist_boss_1_intro/battle/complete`, "Battle the Contessa") -> tank chase (Murray) -> **boss 2** (`t5_heist_boss_2_*`, "Retrieve the Clockwerk Eye").
- Battle 1: the Contessa copy = `PloCurProxy` of PROXY 60263. Guard script CID113 2793, WIDGET 2796, music 13, TN 60264 aimed at her. **Below 75% hp** RELAY 2797 fires once.
  Event 15 hero capture as for Rajan (RELAY 2807) `[C:data]`.
- Battle 2: PROXY 60277. TNs 60278-60280 aimed at her. Game events 55/56 add or remove TN 60280 and call two job sidebag hooks. Defeat (event 23/24) completes it at once `[C:data]`.
- **Class** CONTESSA 0x40: `InitContessa` 0x14FAC8 (STEPGUARD init, +0x4A4 = 1); `UpdateContessa` 0x14FAF8 (base update, then if +0xF80 == -1 and +0x208 != 0 calls 0x205910 with
  table D_002BD940) `[C:code]`. What that call does is not named `[H: a projectile or hypnosis attack]`.

### 3.4 Neyla's RC chopper: `m6_heist`, L30 (train_c)

`t6_heist_train_1` (work through the train) -> `t6_heist_rc_chopper_1` ("Protect Sly from Neyla") -> `t6_heist_train_2` -> `t6_heist_rc_chopper_2` -> `t6_heist_complete`
("Defeat Neyla") `[C:data]`. The rc tasks call a job hook with a target oid (49646, 49606) and post ticker notices 3249/1091. The enemy `npc_rc_boss` has no STEPGUARD class:
it is SO + CID98 + CID112/CID113 driven by 13 RELAYs `[C:data]`. The RC chopper is the generic PO `rc_chopper` (see [minigames](minigames.md#3-rc-chopper)). How damage is counted is not traced `[H]`.

### 3.5 Jean Bison: `m7_boss`, L37 (sawmill boss)

Tasks `[C:data]` (all Bentley, 8): `t7_boss_intro` (L37) -> `t7_boss_battle` (shows object 32246 while state 1-2) -> `t7_boss_battle_phase1..4` -> `t7_boss_battle_complete`
("Defeat Jean Bison", warps to L32) -> `t7_boss_get_to_battery` -> `t7_boss_complete` ("Stow aboard the battery").
The **job script is the boss brain** `[C:data]` (Splice in WE `c_sawmill_macro`):
- Post-load (load filter 37): binds Bentley (8), the boss via PROXY 32248, hp WIDGET 3275 and the goose GOMER pool 32423, and installs five hooks.
- **Hit counter**: hook 1459 runs on Bison damage. If the damage source is **not Bentley** and Bison's hp dropped below the last value, `hits += 1`
  (Bentley damages him only indirectly, through bombs and traps such as `c_bison_bomb` and `c_ewok_trap_circular_saw/log/fire_pad` `[H]`). The bar = (20 - hits)/20.
- **Phases**: at hits 4, 12, 16 and 20, if Bentley is alive 1 s later, `RequestTaskState(phaseN, 4)`. Each phase task fires a RELAY (3003, 3004, 3005, 3286) for the transition
  cutscene. At hit 8 the goose spawner stops. At 20 it stops for good.
- **Adds**: hook 1463 spawns a goose (`PloAcquireProxy` on GOMER 32423) every 2.0 s. On battle start Bison gets parameter range `(lm 2.5 3.5)` via sidebag 1464 `[H: attack interval]`.
- Battle start (hook 1461): music 6, bar shown full, RELAY 3281. End (hook 1462): bar hidden, RELAY 3281 alarm 33436, geese removed, music stopped.
- Death (event 16): remove the geese, fire RELAY 3277 (L37). In L32 it is `RestartJob`.
- **Class**: JEANBISON 0x3E has **0 own vtable slots** (pure STEPGUARD), so the port needs no Bison-specific engine code. Everything is Splice plus STEPGUARD `[C:code]` (cid table).

### 3.6 Clock-La: `m8_turret` (L38) then `m8_boss` (L43)

- **Turret phase** `m8_turret` (L38, Sly at the `a_turret` TURRET 0x45): "Shoot down Clock-La". The 7.7 KB job script scripts Clock-La's attack passes over the blimp
  (5 x 3 pairs of path objects) and fires `a_laser_missile` (ACCMISS) and the turret BOMBs 17384/17385 `[C:data]`.
- **Boss level** `m8_boss` (L43) `[C:data]`: `t8_debris_fall_intro` -> `t8_debris_fall_chase` -> `t8_debris_fall_on_back` -> `t8_debris_bash_head` -> `t8_debris_fall_complete`
  ("Catch up to Clock-La") -> `t8_open_head_intro` -> `t8_murray_open_head` -> `t8_bentley_activate_destruction` -> `t8_heist_done` ("Destroy Clock-La").
- Clock-La is an **object assembly** (WE `npc_clockwerk` under PROXY 10943 `a_blimp_boss_sweep`): 12 SO, 9 SM, 6 CID98 (zappable targets), 6 VOLBTN, 3 CID112, 27 PROXY.
  There is no actor class `[C:data]`.
- **Head-bash phase** (6.1 KB task script): the hp bar starts at **25%** (`s1443 = 0.25`, continuing from the turret phase `[H]`) with **20 hits** left. Two sides (left/right),
  each with a VOLBTN stand pad (7741/7743) and an eye target CID98 (8292/8348) made zappable (`SetSoZapResponse 0x200087E 8`). Standing on a pad sets SM 7693 goals
  (4491/4486) and schedules, 1 s later, an SM goal 10837 on that side's SM (7689/7691). Clock-La fires homing missiles from PROXY pools 8276/8290 through launcher CID112 8274
  (`PRIME 3.0`, `MISSILE+0x220` aim point clamped to +-0.3/+-0.15 of the forward axis, `FireMissile`), each side again after 6-8 s. When **both** sides are smashed
  (`SEEK 16908`), the task completes `[C:data]`.
- The last phases are relay-driven (Murray opens the head, Bentley triggers the destruction) `[C:data]`.

## 4. Mini-bosses and chases (not titled as bosses)

| Encounter | Job / tasks | Level | Played as | Notes |
|---|---|---|---|---|
| Contessa tank chase | `m4_heist` `t4_heist7_chase` ("Chase the Contessa") | L14 | Murray | `npc_cantessa_tank` SO/SM `[C:data]` |
| Neyla chase | `m5_heist` `t5_heist_neela_chase_*` | L17 | Sly | keep-up VOLBTN 60244 parented to the Neyla proxy 8436, music 12 `[C:data]` |
| Carmelita's tank | `m5_heist` `t5_heist_tank_chase_*` ("Disable Carmelita's Tank") | L17 | Murray | `[C:data]` |
| Old Grizzle Face | `m7_bear` | L32 | Murray | BEAR 0x42 (no own code), PROXY `c_sawmill_ext_bear` `[C:data]`; fight mechanics `[H]` |
| Neyla (blimp) | `npc_boss_neela` WE also in L39/L43 | - | - | the NEELA class (0x3C) object exists only in L11/L17/L22 `[C:data]` |

## 5. What the port must implement

1. STEPGUARD hp (+0x338/+0x33C), zap damage and the game events 15/16/21/23/24 with the guard as argument. Every boss hp bar depends on them.
2. The CID113 guard-script player (`StartGuardScript`, `SetGuardBhvguardMode1`), plus DIMITRI routes (4 functions) and the RAJAN/CONTESSA overrides (5 functions).
3. PROXY copies (`EnsureProxyCopy`, `LockProxyPlo`, `PloCurProxy`, `PloAcquireProxy`) for respawning bosses and adds.
4. WIDGET bar anim 847, `PushMusic`/`StopMusic`, RELAY `FireRelayPlayer`/`TRIGGER`, `schedule-callback`/`cancel-callback~`.
5. SM goal driving (`SetSmaGoal`, `SEEK`, `APPLY`/`RETRACT`), VOLBTN, `SetSoZapResponse`, and MISSILE `PRIME`/`FireMissile` for Clock-La.
6. TURRET (minigames page) for the L38 phase and the RC chopper PO for Neyla.
Jean Bison and Clock-La need **no boss-specific engine code**. They are entirely Splice over generic classes `[C:code]` (0 own vtable slots, or no actor class at all).

## 6. Open questions
- PROXY source WE of each boss proxy (97688, 60263, 60277, 32248, 10943): needs the proxy record decode.
- CID113 guard-script format; what `UpdateContessa`'s 0x205910 call and Rajan's stimulus 0x246 are.
- Damage-source argument of the Bison hook (second argument) and how `npc_rc_boss` takes damage.
- None of this has runtime confirmation. A save state inside L37 or L43 would confirm hp values (e.g. Bison max hp vs the 20-hit counter).
