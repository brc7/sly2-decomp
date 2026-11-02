# Game progression: jobs, tasks, the event bus, difficulty, collectibles

Related: [save-memcard](save-memcard.md) (everything here persists as save variables), [world-sw](world-sw.md), [hud-ui](hud-ui.md),
[cutscenes](cutscenes.md) (RELAY sequences fired by tasks), [splice scripts format](../formats/splice-scripts.md).

> **What changed in v2.** Runtime round 6 (S57-S62) captured the first job (`m1_sat`, three satellite dishes in the Paris hub) from
> start to finish plus the start of the next one in L03. The task state machine, the dependency graph, the completion banner, the
> HUD counter and the "% complete" formula are now `[C:runtime]`. The JOB/TASK save naming question is resolved.

## 1. JOB and TASK (game.cpp, 0x1613A8-0x166270)

The class words JOB (CID 0x84) and TASK (CID 0x85) are team coinage (no class-name strings), but the objects' save names are known:
each TASK's save-variable refs (+0xC8/+0xD0/+0xD8/+0xE0/+0xE8 = focus_count, complete_count, is_complete, state, from_memcard) and each JOB's
(+0xD0/+0xD8/+0xE0) are `{kind, prec}` pairs with `*prec = field_sym << 16 | parent_sym`, and the parent symbol is the object's save name.
**`m<N>_<name>` = JOB, `t<N>_<name>` = TASK** `[C:runtime runtime6]` (`UpdateTaskSaveVars` 0x1643B0, `UpdateJobSaveVars` 0x163460).
The same 11 JOBs and 99 TASKs load in every Paris level (WE `f_nightclub_macro`: WE 59 in L02, WE 161 in L03) `[C:runtime]`. Paris jobs:
`m1_sat, m1_recon, m1_brawl, m1_bug, m1_follow_dimitri, m1_moulin_power, m1_neela, m1_disco, m1_waterpump, m1_heist, m1_boss` (boss disabled, +0x50 = 1).

### TASK (LO child, 0x110)

| Offset | Field | Tag |
|---|---|---|
| +0x54 / +0x58 | state / tState (time of the last change) | [C:runtime] |
| +0x70 | auto-activate when it becomes available (`t1_sat_complete` = 1) | [C:runtime] |
| +0x78 | time limit | [H r3b] |
| +0x5C | **objective text id** (TASK eopid 1265 binds to +0x5C, [C:static] OPTION_TABLE; the values decode to objective strings in each level's string table, [C:decode] devtools/splice_browser). v2 said "music" [H r3b]: **corrected** | [C:static] |
| +0x7C | owning JOB | [C:runtime] |
| +0x80 | marker LO (WAYPT, CID 0x6B; start tasks only, e.g. oid 0x4440); `UpdateTask` 0x163B48 shows it while state == 1 | [C:runtime] |
| +0x90 / +0x94 / +0x98 | prerequisites: oid array / count / TASK* array | [C:runtime] |
| +0x9C / +0xA0 / +0xA4 | successors: oid array / count / TASK* array | [C:runtime] |
| +0xB8 / +0xBC | warp / level | [H r3b] |
| +0xC0 | **oid of the PO the player controls** for the task (7 = Sly/`jt`, 8 = Bentley, 9 = Murray; other POs in minigames, e.g. the RC chopper; -1 = keep the current one); written to `oidPoCurrent`. v3 read it as a 3-character index: corrected per [bosses](bosses.md) / [minigames](minigames.md) | [C:runtime] + [C:data] |
| +0xC8..+0xE8 | save-variable refs | [C:runtime] |

**Task states** `[C:runtime]`:

| State | Meaning |
|---|---|
| -1 | unset (all tasks of a freshly loaded world until PostSwLoad) |
| 0 | locked |
| 1 | available (exactly the members of the available-task DL SW+0x3B00) |
| 2 | complete inside the running job (save `is_complete` = 1 already) |
| 3 | complete after the job finished (`FinishJob` -> `SetTaskStateCascade(end, 3)`) |
| requests 4 / 5 | activate / deactivate (`RequestTaskState`; RELAY `SetTaskState(task, 5)` ends a task without completing others) |

The current task can be NULL mid-job (S60: dish 2's relay ended with `SetTaskState(dish2, 5)`; dish 3 was available but not current).

### JOB (LO child, 0x100)

Fields `[C:runtime]` unless marked: start task +0x64, end task +0x68 (current while start is complete and end is not), task DL +0x80 (ibDle 0x84),
count +0x7C (= save `m<N>.focus_count`), elapsed +0xA4 (= save `time_played`), banner widget oid +0xAC (WIDGET128 0x6D6), intro/outro labels +0xB0/+0xB4
(0x4411/0x4412), widget pointer +0xB8, shared widget +0xBC (oid 0x379), next task +0xC0, completion phase +0xC4, tPhase +0xC8 (`UpdateJob` 0x163078).
Complete-event and fail-event lists and the per-state visibility/RSMG records of JOBs sit at other offsets than TASK's (not decoded) `[H r3b]`.

### Visibility and RSMG records (how a task changes the world)

Each TASK carries records `{oidLo, oidContext, grfStates (1 << state), level}`: the object is in the world only while the task's state bit is in the mask,
and RSMG records drive state machines on state changes `[C:runtime]`. Example `t1_sat_dish1`: trigger spot 0x1E441 (CID35), PROXY 0x1E442, CID 0x15
0x1E444/45, CID 0x18 0x1E446 and 0x1E44A with mask `1<<1` (in the world only while dish 1 is available); RSMG {state 0/1 -> SM 0x17DE2 goal 0x1513
(intact), 2/3 -> goal 0x1512 (destroyed)}.

### Worked example: job `m1_sat` (S57-S62) `[C:runtime]`

Chain: `t1_roof_intro -> t1_sat_intro -> t1_sat_dish1 (0x6E2) -> t1_sat_dish2 (0x6E4) -> t1_sat_dish3 (0x6E6) -> t1_sat_complete (0x6E9) -> t1_recon_intro` (next job).

| t (s) | Event | States intro/d1/d2/d3/complete |
|---|---|---|
| 46.18 | `t1_sat_intro` activated (count 1) -> its Splice evt19 triggers RELAY 0x6B2 (Bentley on the recon view) -> state 2; dish 1 available | 2 1 0 0 0 (S57, S58) |
| ~135 | Sly uses the dish-1 **trigger spot** (CID35 at site C): `TriggerCid35` 0x184130 -> `ApplyTaskStates` (+0x15C count, +0x160 `{task oid, 4}`) -> `RequestTaskState(dish1, 4)` -> evt19 -> `FireRelayPlayer(RELAY 0x6E3)` (S59: cutscene at command 11) | 2 1 0 0 0 |
| 144.85 / 172.53 | the relays destroy dishes 1 and 2 (`ApplySm(..., 0x1512)`) and set `SetTaskState(dishN, 2)` | 2 2 2 1 0 (S60, HUD 2/3) |
| 228.86 | dish 3 -> 2; `t1_sat_complete` becomes available, auto-activates (+0x70) and triggers RELAY 0x6EA | |
| 234.86 | RELAY 0x6EA sets `t1_sat_complete` -> 2 -> `OnJobTaskComplete` -> `CompleteJob` -> `SetJobCompletionPhase(1)`: banner + clock frozen | 2 2 2 2 2 (S61) |
| later | banner label ends -> `FinishJob`: tasks -> 3, game event 0x20, `AutoSaveGame`; L03 loads; `t1_recon_brawl` activated at L03 t = 21.51 | 3 3 3 3 3 (S62) |

Only the trigger spot of the available task is in the world, so the dish order is fixed (C, B, A) although the HUD reads like a free counter.
The objective objects are plain SOs (WE 152 `f_satellite_dish`, cloned under three site SOs) with an SM (0x1513 intact / 0x1512 destroyed) and burst
children (EXPLG, EXPLS, EMITTER, CID 0x2B) that enter the world when destroyed; they have no Splice of their own.

### Job completion sequence `[C:runtime S61]` + code

`SetJobCompletionPhase(1)`: mark busy, LO flags 0x102, `IncrementSwHandsOff(SW, 2)` (hands-off + input lock), show WIDGET128 0x6D6 with intro label
0x4411. WIDGET128 has `fPause` (+0xB0 = 1): showing it sets D_002DD318 = 1 and **clock rate slot 7 = 0, so the game clock stops** (t = tPhase).
When the label ends, `UpdateJob` phase 1 calls `FinishJob` (tasks -> 3, event 0x20, `AutoSaveGame`, next task or warp). The autosave itself was not
captured (SAVE state/goal 6/6 and memcard thread -1 in S61 and S62) `[C:static]` for the FinishJob -> AutoSaveGame call.

### SW and g_game progression slots

SW+0x3AFC task count (99), +0x3B00 available DL, +0x3B0C current JOB, +0x3B10 current TASK, +0x3B14 pending TASK (the task with a player
character, `t1_sat_intro` throughout the job), +0x3B18 job to abort, +0x3B1C key of `dt`, +0x3B20 key of `loot.ccoin`, +0x3B58[9]/+0x3B7C[9]
keys of `episode.<w>.dt` / `percent_complete` `[C:runtime]`.
**g_game** D_002DEB30 (0x1C bytes): +8 = 4 in every mission state (HYPOTHESIS: argument of the last rejected request), +0xC last gadget id,
+0x10/+0x14/+0x18 oids of current job / task / pending task (mirror the SW slots exactly; -1 when NULL) `[C:runtime]`.
Splice builtins 100/101/102 return the three SW slots (spellings `find-current-job~` etc.), 97 the current level index, 98 the episode, 99 the
world, 103 the difficulty `[H top50]` (code-confirmed by the top-50 round).

### % complete

`NEpisodePercentComplete` 0x161660 counts TASKs **with successors** (91 of 99 in Paris) in **state 3**; recomputed from RAM it matches
`episode.nightclub.percent_complete` going 2 -> 7 between S61 and S62 `[C:runtime]`. So progress counts only after a job's banner has finished.

## 2. The game event bus

`BroadcastGameEvent(&g_game, id, arg)` 0x1619A0 (54 callers) delivers to `HandleGameEvent`, the difficulty system, every available task of the
current job (message 0x11) and the current job `[H r3b]`. Events leave no trace in a RAM snapshot; the live consumers are Splice handlers
(evt17 "game-event"): `m1_sat` handles 16 (hero died -> `RestartJob`), 26 and 61; `m1_recon` handles 9, 11, 16, 19, 23, 24, 26, 29, 61 `[C:runtime]`
(handlers bound in the live frames). Splice game-event ids are decimal.

| Id | Meaning (where known) | Tag |
|---|---|---|
| 0 / 1 / 2 / 4 / 5 | startup / world teardown / load begin / post-load / transition done | [H r3b]; 5 [C:static] (ExecuteSwTransition) |
| 0x0A | recon photo taken | [H r7c] |
| 0x0C | look-mode HUD | [H r4b] |
| 0x10 (16) | hero died (STEPPLAYER vt93) -> jobs restart | [C:runtime] handler |
| 0x13 | player spotted (detector) | [H r3] |
| 0x14 / 0x16 | guard flag 3 / 0xC raised | [H r7b] |
| 0x18 | prize drop (CID 0x66) | [H r8] |
| 0x19 | pickpocket | [H r6b] |
| 0x1A / 0x1B | Splice event 5 / 6 (trigger / untrigger) on an LO (`HandleLoSpliceEvent` 0x1A61E0) | [C:verified arbitration] |
| 0x1C-0x27 | job activated, complete-event, announce, aborted, finished, begin/end, task begin/end, state changed, rejected | [H r3b] |
| 0x20 | job finished (FinishJob) | [C:static runtime6] |
| 0x28 / 0x2C | pause menu shown / hidden | [H r5b] |
| 0x3C | gadget used (arg = gadget id, also in g_game+0xC) | [C:runtime runtime7b] |
| 0x33 | CID60 alarm | [H r5b] |

## 3. Dynamic difficulty (difficulty.cpp, 0x154A60)

Difficulty is the average of 3 saved samples (`suck_history.mission_0..2`), changed by deltas from D_002BDC00 on events, clamped by an LM;
`NScaleByDifficulty` interpolates gameplay values `[H r3 difficulty.md]`, save vars `[C:runtime]`. `UpdateGameTimers` eases difficulty after 480-720 s
on one job `[H r3b]`. Splice builtin 103 reads it (`GGetDifficulty`) `[H top50]`.

## 4. Collectibles and loot (prize.cpp 0x1C4E50)

| Class | Role | Tag |
|---|---|---|
| CID 0x5A DPRIZE (abstract) | Sly 1 coin.c order; state +0x150 (0 resting, 2 collected, 3/4 in flight, 5 ?) | [C:runtime] states |
| CID 0x5B COIN | pool of 32 copies of proxy source "coin"; sprayed copies chosen among state-2 copies | [C:runtime] |
| CID 0x5C KEY | key (1 in the hub) | [C:runtime] |
| CID 0x5D MEDICINE | health refill (proxy source "medicine", 8 copies) | [C:runtime] |
| CID 0x5E LOOT | pickpocket item (4); launched at the player (+0x154 launch time); entry table +0x240 (32 x 0x2C), current +0x244 (`AddLootEntry`, `AddLootEntryOid` 0x1C7A10) | [C:runtime], names [arbitration] |
| CID 0x5F CLUE | clue bottle pool (7); collected -> bit in `episode.<w>.level.<l>.afClue`; HUD counters SW+0x4764/+0x4768 | [C:runtime] |
| CID 0x4E VAULT | clue-bottle safe (combination via POCTL presses); save flag `fVault` | [H r5], class [C:parse] |
| CID 0x62 (CID98) | breakable props (chairs, barrels, crates): broken by a zap (+0x16C zapper, +0x170 zap parameter, +0x15C 0 -> 1, +0x168 0xE); prizes come from proxy pools | [C:runtime S10-S11] |

Breaking a container (S10 -> S11) sprayed 5 coins (copies 0x11, 0x14-0x17 of "coin") and one medicine; nothing was allocated (pools only) `[C:runtime]`.
`SprayDprizes` 0x1C6FD8 sprays from proxy pools `[H r5]`. Coin award = `AwardDprize91` 0x1C67A0 (+1 to the save counter; see the coins x10 demo patch in
[guides/playtesting](../guides/playtesting.md)).

## 5. Booty (booty TU 0x13BDF0)

CID 0x67 is **BOOTY** (all 41 instances in the game are WE `booty`; v2 name replaces cid_table's "ALARM") `[C:parse whole_game]`: a carried
object (idle, in container, transfer, carried, dropped and returning after 5 s, marker); entry table at +0x158 (`AddBootyEntry` 0x13C3B8,
`AddBootyEntryOid` 0x13C448); a prize-animation table of 14 entries chosen by Dprize id (Splice) `[H r2b]`. The RELAY `*Alarm` functions are a different
object (do not rename them BOOTY).

## 6. Objective counters, job markers and the episode menu

- **WIDGET129** (CID 0x81) is the objective counter. The "0/3" text is **not stored**: `UpdateWidget129` 0x211F90 counts the complete tasks in its list B
  (+0x194 count, +0x19C 8-byte entries) every frame and sprintf's done/total/remaining into D_002C3EC0 +0/+0x30/+0x60 ('0/3/3', '2/3/1', '3/3/0') `[C:runtime]`.
  It is visible (state 2) while any listed task is available. Other counters are loaded hidden (0x70B `t1_brawl_box1-3`, 0x72E `t1_moulin_key1-6`).
- **WIDGET128** = job completion banner (fPause, section 1). WAYPT (CID 0x6B): job markers `mission_start_jt/bentley/murray` `[C:parse]`.
- **Episode select** `[C:runtime S74]` + code (`PostWidget131Load` 0x15CDA8): the menu index is the world index (0 intro, 1 nightclub/Paris, 2 palace/India hub,
  3 temple, 4 prison, 5 castle, 6 train, 7 sawmill, 8 blimp; entry levels L00, L02, L08, L12, L14, L17, L27, L32, L38). The highest selectable episode starts at 8 and goes down
  while `episode[i].dt == 0` and `episode[i-1].percent_complete < 100`; the loop decrements first and breaks when the index is < 2, so the index **can reach 1** (a fresh save shows only episode 1) `[C:static v3 re-read of func_0015CDA8.c]` (v2 said "never below 2": **corrected**, found by devtools/save_editor). Cheat flag D_002BE26C unlocks all. `episodeBest` is not used by the menu; it gates the
  ThiefNet stock. **Displayed percent** (`FormatEpisodePercent` 0x15D8F8) = `percent_complete * 95 / 100`, + 5 if the episode's vault power-up (gadget 32 + ep, table
  D_002BE2F8) is owned (ids 33-38 owned = fVault in episodes 1-6) `[C:runtime]`.
- **WIDGET131** (CID 0x83, singleton D_003E1C04) episode menu in the front-end world L01 (fields: selected +0x110, max +0x118, mode +0x11C (2 browse, 10 start)): mode 10 = "start episode" plays sprite label 0x539
  (D_002BE080[ep][10]) for 10.6 s; then `UpdateWidget131` 0x15CF30 fires the episode's intro EXIT (oids 0x531-0x538, table D_002BE2A8) if
  `episode.<w>.dt == 0`, else `WipeToWorldWarp(PrecWorldEntry(ep), 0x189, -1, 0, 0x30A)` `[C:runtime S55/S56]`.

See also [bosses](bosses.md) and [minigames](minigames.md) for every boss and minigame job/task.

## Devtools findings (v3, 2026-11-01)

From [devtools/save_editor](../guides/devtools.md#save_editor) (73 save states; curated table out/var_meanings.csv, 47 rows with tags):
- **Clue bottles**: all 30 bottles of an episode live in `<world>.level.exterior.afClue` (the hub level); `afClue` of other levels stays 0 `[C:runtime save_editor]`.
- **Vault flag**: `fVault` lives in one level per episode (disco, basement, interior, vault, guardbreak, cabins for eps 1-6) and is 1 exactly where vault
  gadgets 33-38 are owned `[C:runtime save_editor]`; eps 7-8 vault level unknown.
- `from_memcard` is a card-load marker: S31->S57 and S56->S65 show a card reload, then the loaded world clears it `[C:runtime]`. A completed
  episode keeps `t<N>_complete` at state 1, and dead-end side tasks at 1 `[C:runtime save_editor]`.
- Save timelines by play time: save 1 = S02-S31 then S57-S64; save 2 = S32-S56 then S65-S74 `[C:runtime save_editor]`.
- Task objective text (TASK +0x5C) and the episode-menu lower bound were **corrected** above (section 4 table, section 6).
- Task/job edits apply only at world load (`RestoreTaskState` 0x164270 at game event 5); coins/gadgets/HP apply immediately (coins `[C:live]`).

## 7. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x13BDF0-0x13D558 | booty | low | carried-item/booty system; cid_table/tu_table said alarm; cid_table says ALARM: wrong |
| 0x154A60-0x1553F0 | difficulty | medium |  |
| 0x1613A8-0x166270 | game | high | tu_table said turret; was labelled turret |
| 0x1C4E50-0x1C8CC0 | prize | high | tu_table said alarm; was labelled alarm |
| 0x229EF0-0x22B040 | waypoint | medium |  |

## 8. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x13BE58](../../src/game/gameplay/progression/booty/LoadBootyFromBrx.c) | `LoadBootyFromBrx` | cid 0x67 vt14: LoadSoFromBrx into a stack-allocated entry table (0x580), then copy the used entries (0x2C each) to SW heap | 0.75 | T1 | parse |
| [0x13C448](../../src/game/gameplay/progression/booty/AddAlarmAlbrk__FP5ALARM3OID.cpp) | `AddBootyEntryOid` |  | 0.80 | existing | arbitration; replaces existing AddAlarmAlbrk__FP5ALARM3OID |
| [0x13CFD8](../../src/game/gameplay/progression/booty/func_0013CFD8.c) | `OnCid37Add` | cid 0x25 vt23: base 0x183FB8 then EnsureSwCallback(D_002A4BB0) | 0.85 | T2 | arbitration |
| [0x13D248](../../src/game/gameplay/progression/booty/NBootyContainerKind.c) | `NBootyContainerKind` | Read saved kind for this container (default +0x184); remap 0x2BF..0x2C1 -> 0x2BC..0x2BE, 7..9 -> -1 | 0.85 | T1 | verified |
| [0x1613A8](../../src/game/world/state/game/StartupGame.c) | `StartupGame` | Once-per-boot game startup from main: language-dependent init (sceScfGetLanguage), broadcast game event 0, init the game sequence queue D_002E1E88 | 0.75 | T1 |  |
| [0x1613F8](../../src/game/world/state/game/InitGame.c) | `InitGame` | Resets the 0x1C-byte GAME record g_game (D_002DEB30): zero, oidJob/oidTask/oidTaskPending (+0x10/+0x14/+0x18) = -1; ResetGs(g_gs,0); InitDifficulty; D | 0.55 | T1 | verified |
| [0x161478](../../src/game/world/state/game/UpdateGameTimers.cpp) | `UpdateGameTimers` | Per-tick: adds dt to the total-play-time save var (g_sw+0x3B1C) and to the current episode's play-time var (g_sw+0x3B58[NWorldCur()], 9 episodes); add | 0.75 | T1 |  |
| [0x161660](../../src/game/world/state/game/NEpisodePercentComplete.cpp) | `NEpisodePercentComplete` | Episode completion percent: over all TASKs (cid 0x85) with successors (cA0!=0), 100*complete(state 3)/count, clamped to 1..99 unless 0 or all | 0.65 | T1 |  |
| [0x1618F8](../../src/game/world/state/game/func_001618F8.c) | `ShowSwWidget3BC4Timed` | Shows HUD widget g_sw+0x3BC4 (clears +0x8C, vt49 Show(-1)) and sets its hide time g_sw+0x476C = now + dt | 0.40 | T2 | verified |
| [0x1619A0](../../src/game/world/state/game/BroadcastGameEvent.c) | `BroadcastGameEvent` | Game event bus: HandleGameEvent(g_game), difficulty 0x154A90, every AVAILABLE task (list g_sw+0x3B00) of the current job (SendTaskGameEvent), then the | 0.85 | T1 | verified |
| [0x161FC8](../../src/game/world/state/game/GrfFromState.c) | `GrfFromState` | 1<<state, 0 for -1 | 0.85 | T1 | verified |
| [0x162448](../../src/game/world/state/game/InitJob.c) | `InitJob` | JOB (cid 0x84) Init: LO Init, difficulty-ease thresholds 480/720 s, ids -1, InitDl(+0x80, link 0x84) for its tasks | 0.70 | T1 | verified |
| [0x1624A8](../../src/game/world/state/game/LoadJobFromBrx.cpp) | `LoadJobFromBrx` | JOB LoadFromBrx: preallocates 4 record arrays (visibility, RSMG, two event-id lists), LO load, shrinks to the counts read | 0.75 | T1 | parse, verified |
| [0x162600](../../src/game/world/state/game/FUN_001e53f8.c) | `AddJobCompleteEvent` | Splice: appends a game-event id to the job list at +0x54/+0x58 (max 32); HandleJobGameEvent fires event 0x1D when it arrives | 0.55 | existing | arbitration; replaces existing FUN_001e53f8 |
| [0x162908](../../src/game/world/state/game/EndCurrentJob.c) | `EndCurrentJob` | If p is the current job, EndJob(p) | 0.85 | T1 | verified |
| [0x162BB8](../../src/game/world/state/game/HandleJobGameEvent.cpp) | `HandleJobGameEvent` | Job's game-event hook: if live SendMessage 0x11(ev,arg) and check its complete-event list (-> 0x162DD0) and fail-event list (-> FailJob); if not live, | 0.60 | T1 | verified |
| [0x163078](../../src/game/world/state/game/UpdateJob.c) | `UpdateJob` | JOB Update: completion sequence phases 1 (intro label on widget) -> 2/3 -> done, advancing when the widget label finishes | 0.60 | T1 |  |
| [0x1633E8](../../src/game/world/state/game/FIsCurrentJob.c) | `FIsCurrentJob` | p == g_sw+0x3B0C (current job) | 0.85 | T1 | verified |
| [0x163460](../../src/game/world/state/game/UpdateJobSaveVars.cpp) | `UpdateJobSaveVars` | Writes job count (+0x7C), complete flag and elapsed time (+0xA4) into its three save-var refs (+0xD0/+0xD8/+0xE0) | 0.50 | T1 |  |
| [0x1635A0](../../src/game/world/state/game/InitTask.c) | `InitTask` | TASK (cid 0x85) Init: LO Init, ids -1, counts tasks in g_sw+0x3AFC | 0.70 | T1 |  |
| [0x1635F8](../../src/game/world/state/game/LoadTaskFromBrx.cpp) | `LoadTaskFromBrx` | TASK LoadFromBrx: visibility/RSMG arrays, then u8-counted oid lists of prerequisites (+0x90) and successors (+0x9C) | 0.75 | T1 | parse |
| [0x1638F8](../../src/game/world/state/game/PostTaskLoad.c) | `PostTaskLoad` | TASK PostLoad: link into its job's task list, resolve prerequisite and successor oids to TASK pointers, apply inactive visuals if live, restore two sa | 0.65 | T1 |  |
| [0x163B48](../../src/game/world/state/game/UpdateTask.c) | `UpdateTask` | TASK Update: time-limited available task sends timeout (SendTaskTimeout); shows its marker LO (+0x80, AddLo) only while available and no job is curren | 0.60 | T1 |  |
| [0x164018](../../src/game/world/state/game/FIsCurrentTask.c) | `FIsCurrentTask` | p == g_sw+0x3B10 | 0.70 | T1 |  |
| [0x164098](../../src/game/world/state/game/OidFromTaskState.c) | `OidFromTaskState` | Maps task state 0..3 to oid 0x612..0x615 (the default SM goal per state), else -1 | 0.60 | T1 | verified |
| [0x1643B0](../../src/game/world/state/game/UpdateTaskSaveVars.cpp) | `UpdateTaskSaveVars` | Writes task fields (two ints, complete flag, state, false) into five save-var refs and refreshes its job's vars | 0.50 | T1 |  |
| [0x1657A8](../../src/game/world/state/game/AnnounceTaskJob.c) | `AnnounceTaskJob` | Broadcasts event 0x1E with the task's job | 0.85 | T1 | verified |
| [0x165B40](../../src/game/world/state/game/RecomputeTaskState.c) | `RecomputeTaskState` | Derives a successor's state from its prerequisites (locked unless all complete; locked->available) | 0.85 | T1 | verified |
| [0x1C4E50](../../src/game/gameplay/progression/prize/func_001C4E50.c) | `InitDprize` | DPRIZE::Init (vt7): InitSo + 0x1F7918 setup | 0.85 | T1 | verified |
| [0x1C4EF8](../../src/game/gameplay/progression/prize/func_001C4EF8.c) | `LoadDprizeFromBrx` | DPRIZE::LoadFromBrx (vt14): LoadSoFromBrx + snip objects (0x1A64E8) | 0.85 | T1 | parse |
| [0x1C4F60](../../src/game/gameplay/progression/prize/func_001C4F60.c) | `PostDprizeLoad` | DPRIZE vt15 PostLoad: base + resolve linked objects via SW lookup | 0.85 | T1 | verified |
| [0x1C5148](../../src/game/gameplay/progression/prize/func_001C5148.cpp) | `ProjectDprizeTransform` | DPRIZE vt17: damped spring/hover integration of the prize position (state 6 -> base) | 0.75 | T1 |  |
| [0x1C55E0](../../src/game/gameplay/progression/prize/func_001C55E0.cpp) | `CheckDprizeBounce` | query nearby solids (0x20FAA0) and resolve deepest contact (bounce) | 0.70 | T1 |  |
| [0x1C5E48](../../src/game/gameplay/progression/prize/func_001C5E48.cpp) | `UpdateDprize` | DPRIZE vt16 Update: base update, sparkle (0x1C6308), bounce, collect | 0.80 | T1 |  |
| [0x1C5FA0](../../src/game/gameplay/progression/prize/func_001C5FA0.cpp) | `RenderDprizeSelf` | DPRIZE vt19 RenderSelf (bob/spin then SO render) | 0.85 | T1 | verified |
| [0x1C60B8](../../src/game/gameplay/progression/prize/func_001C60B8.c) | `SetDprizeDprizes` | DPRIZE state machine setter (unlock proxy copy, attach/detach, state timestamps) | 0.85 | T1 | verified |
| [0x1C68F0](../../src/game/gameplay/progression/prize/func_001C68F0.c) | `SetMedicineDprizes` | CID 93 setter (uses PpoCur - gives to current character) | 0.35 | T2 | verified, whole-game |
| [0x1C6AB0](../../src/game/gameplay/progression/prize/func_001C6AB0.c) | `OnMedicineSmackDone` | 8-byte helper | 0.60 | T1 | verified, whole-game |
| [0x1C6B28](../../src/game/gameplay/progression/prize/func_001C6B28.c) | `SetKeyDprizes` | CID 92 setter (sound on collect) | 0.35 | T2 | verified, whole-game |
| [0x1C6BD0](../../src/game/gameplay/progression/prize/func_001C6BD0.c) | `LaunchSprayedDprize` | re-parent spawned prize (0x1A58D0) and init motion | 0.60 | T1 | verified |
| [0x1C6CE8](../../src/game/gameplay/progression/prize/func_001C6CE8.c) | `PdprizeNewSpray` | make one prize of kind 0x5B/0x5D from its proxy pool (PloAcquireProxy + LockProxyPlo) | 0.50 | T1 | runtime |
| [0x1C76D0](../../src/game/gameplay/progression/prize/func_001C76D0.c) | `LoadLootFromBrx` | CID 94 LoadFromBrx (base + copies table on stack heap) | 0.75 | T1 | parse, verified |
| [0x1C7988](../../src/game/gameplay/progression/prize/func_001C7988.c) | `AddLootEntry` | calls 0x1C7A08 stub | 0.70 | T1 | arbitration |
| [0x1C7A10](../../src/game/gameplay/progression/prize/AddAlarmSensor__FP5ALARM3OID.cpp) | `AddLootEntryOid` | Append OID to the list behind this+0x244 (count +0x1C, max 3) | 0.85 | existing | arbitration; replaces existing AddAlarmSensor__FP5ALARM3OID |
| [0x1C7EA8](../../src/game/gameplay/progression/prize/func_001C7EA8.c) | `ClueVt27` | CID 95 vt27 | 0.30 | T2 | verified |
| [0x1C8350](../../src/game/gameplay/progression/prize/func_001C8350.c) | `UpdateVault` | CID 78 vt16: per-state update (input / open) | 0.45 | T2 | verified |
| [0x1C8730](../../src/game/gameplay/progression/prize/func_001C8730.c) | `SetVaultState` | state transition (camera/sound/hands-off) | 0.35 | T2 | verified |
| [0x22A908](../../src/game/gameplay/progression/waypoint/func_0022A908.cpp) | `RenderWaypt` | fades the job marker, draws 3 child SOs scaled by distance, job name plate | 0.85 | T1 | verified |

Selected 48 of 193 named functions (T1 with confidence >= 0.7, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/game-progression.md](../reference/functions/game-progression.md).

## Open questions
- Game event ids beyond the Splice-handled ones (needs a breakpoint on 0x1619A0; snapshots cannot show events).
- JOB visibility/RSMG record offsets; complete-event and fail-event lists.
- g_game+8 meaning; the button-vs-proximity trigger of CID35 spots.
- The autosave in progress (memcard busy) between FinishJob and the next load.
