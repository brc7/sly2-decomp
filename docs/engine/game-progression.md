# Game progression: jobs, tasks, the event bus, difficulty, collectibles

Related: [save-memcard](save-memcard.md) (everything here persists as save variables), [world-sw](world-sw.md), [hud-ui](hud-ui.md).

## 1. JOB and TASK (game.cpp, 0x1613A8-0x166270)

The class names JOB (CID 0x84) and TASK (CID 0x85) are **team coinage** based on behaviour `[H r3b game.md]`; the binary has no
class-name strings. Counts in the Paris hub: 11 JOBs, 99 TASKs `[C:runtime]`. The save schema has 78 `m<N>_<name>` records
(focus_count, is_complete, time_played, suck_value) and 795 `t<N>_<name>` records (focus_count, complete_count, is_complete,
state, from_memcard) `[C:runtime]`, which fits "m = job/mission, t = task" `[H]`.

**TASK** (LO child, 0x110) is a node in a dependency graph `[H r3b game.md]`:
- prerequisites +0x90/+0x94/+0x98, successors +0x9C/+0xA0/+0xA4; owning JOB +0x7C; optional marker LO +0x80 (job beacon, shown while available);
  time limit +0x78, music +0x5C, warp +0xB8, level +0xBC, character to play as +0xC0 (written to `oidPoCurrent`);
- state +0x54: -1 unset, 0 locked, 1 available, 2/3 complete. Runtime: {0: 94, 1: 2, 3: 3} in the hub, and the available-task DL
  SW+0x3B00 holds exactly the 2 available tasks; `t0_*` intro tasks have save `state` 3 `[C:runtime]`.

**JOB** (LO child, 0x100) groups tasks (DL +0x80): start task +0x64, end task +0x68; current while start is complete and end is not.
It holds a complete-event list (broadcasts 0x1D) and a fail-event list (`FailJob`: reload the world entry at warp 0x189, abort on teardown),
a completion sequence (HUD banner under hands-off, autosave, next task or warp), and elapsed time +0xA4 `[H r3b game.md]`.

Both classes carry per-state **visibility records** (`AddLo`/`RemoveLo` world objects) and **RSMG records** (drive state machines);
every state change applies them (`ApplyStateLoVisibility`, `TriggerStateRsmg`). Splice fills them via thunks 0x10E910-0x10F370 `[H r3b]`.

SW progression slots: +0x3AFC task count (99) `[C:runtime]`, +0x3B00 available DL `[C:runtime]`, +0x3B0C current job, +0x3B10 current task
(both NULL in free roam, consistent), +0x3B14 pending task, +0x3B18 job to abort, +0x3B1C key of `dt`, +0x3B20 key of `loot.ccoin`,
+0x3B58[9]/+0x3B7C[9] keys of `episode.<w>.dt` / `percent_complete` `[C:runtime]` (keys resolved by name).

**g_game** D_002DEB30 (0x1C bytes): event arg +8, oids of current job / task / pending task +0x10/+0x14/+0x18 (-1 in free roam `[C:runtime]`), pending save +0x1C `[H r3b]`.
`StartupGame` 0x1613A8, `InitGame` 0x1613F8 (zeroes g_game, oids -1, ResetGs, InitDifficulty) `[H r3b]`.

## 2. The game event bus

`BroadcastGameEvent(&g_game, id, arg)` 0x1619A0 (54 callers) delivers to `HandleGameEvent`, the difficulty system, every available
task of the current job (SendMessage 0x11) and the current job `[H r3 game.md, r3b]`. It replaces Sly 1's direct OnGame*/OnDifficulty* calls.

| Id | Meaning (where known) | Tag |
|---|---|---|
| 0 / 1 / 2 / 4 / 5 | startup / world teardown / load begin / post-load / transition done | [H r3b] |
| 0x0A | recon photo taken | [H r7c] |
| 0x0C | look-mode HUD | [H r4b mode.md] |
| 0x10 | hero died (STEPPLAYER vt93) | [H r2b] |
| 0x13 | player spotted (detector) | [H r3 detect.md] |
| 0x14 / 0x16 | guard flag 3 / 0xC raised | [H r7b] |
| 0x18 | prize drop (CID 0x66) | [H r8 cid100_family.md] |
| 0x19 | pickpocket | [H r6b] |
| 0x1C-0x27 | job activated, complete-event, announce, aborted, finished, begin/end, task begin/end, state changed, rejected | [H r3b] |
| 0x28 / 0x2C | pause menu shown / hidden | [H r5b small_tus] |
| 0x33 | CID 0x3C alarm | [H r5b] |
| 16, 29, 34, 37, 3 | difficulty inputs (deltas; sample shift; bind) | [H r3 difficulty.md] |

Task SendMessage ids: 0x11 game event, 0x12 rejected, 0x13 begin, 0x14 end, 0x15 state changed, 0x16 state changing, 0x17 timeout `[H r3b]`.

## 3. Dynamic difficulty (difficulty.cpp, 0x154A60)

Difficulty is the average of 3 saved samples, changed by deltas from table D_002BDC00 on events, clamped by an LM;
`NScaleByDifficulty` interpolates gameplay values (damage, flee lookahead, ...) `[H r3 difficulty.md]`. The three samples are the save
vars `suck_history.mission_0..2` (and each job has `suck_value`) `[C:runtime]` schema, which matches Sly 1's "suck" vocabulary.
`UpdateGameTimers` eases difficulty once the current job has run 480 s, fully by 720 s ("stuck too long") `[H r3b]`.

## 4. Collectibles and loot (prize.cpp 0x1C4E50)

| Class | Role | Tag |
|---|---|---|
| CID 0x5A DPRIZE (abstract) | Sly 1 coin.c order: Init, Load, PostLoad, HandleMessage, ProjectTransform, bounce, collect, Update, Render | [H r5 prize.md] |
| CID 0x5B | **coin** pool (32 in the hub; 3 live when sprayed in S03; state +0x150 3 -> 2 when collected; `loot.ccoin` 6 -> 9) | [C:runtime] |
| CID 0x5C / 0x5D | other DPRIZE kinds (0x5D health/MP pickup: calls vt80 FCanBeHealed, vt81 AddHpMp) | [H r5, verify r3r5] |
| CID 0x5E LOOT | loot item; launched at the player when pickpocketed (+0x154 launch time) | [C:runtime] |
| CID 0x5F CLUE | **clue bottle** pool (7, repositioned around Sly); collected -> bit set in `episode.<w>.level.<l>.afClue` | [C:runtime] |
| CID 0x4E VAULT | clue-bottle safe driven by POCTL presses (combination); save flag `fVault` | [H r5 prize.md] |

`SprayDprizes` 0x1C6FD8 sprays coins from proxy pools `[H r5]`. Clue HUD counters SW+0x4764/+0x4768 (0/30 -> 1/30) `[C:runtime]`.
Fix: `AddAlarmSensor` 0x1C7A10 writes LOOT's +0x244 (beyond ALARM's size) so it belongs to LOOT (CONFLICTS #2, open: the disputes verifier kept the old name).

## 5. Booty (booty TU 0x13BDF0, cid_table says "ALARM": wrong)

CID 0x67 is a carried booty object (state machine: idle, in container, transfer, carried, dropped and returning after 5 s, marker);
CID 0x25 containers hold one booty and persist its kind; save vars `booty.aoidAssembly`, `oidBooty`, `loot.mplootkc` `[H r2b booty.md]`.

## 6. Job markers and episode menu

- WAYPT (CID 0x6B): job marker objects with fade, distance curtain and a name plate `[H r8 waypoint.md]`.
- WIDGET131 (CID 0x83) episode menu prints per-episode percent and play time from the save vars, autosaves on leaving mode 0 `[H r3b misc_small.md]`.

## 7. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x13BDF0-0x13D558 | booty? | cid_table says ALARM: wrong |
| 0x154A60-0x1553F0 | difficulty |  |
| 0x1613A8-0x166270 | game | was labelled turret |
| 0x1C4E50-0x1C8CC0 | prize | was labelled alarm |
| 0x229EF0-0x22B040 | waypoint |  |

## 8. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x13C448](../../src/hand/AddAlarmAlbrk__FP5ALARM3OID.cpp) | `AddAlarmAlbrk__FP5ALARM3OID` |  |  | existing; fix: AddBootyEntryOid? (adds an OID to the current entry of the cid-0x67 booty object |
| [0x13CFD8](../../src/hand/func_0013CFD8.c) | `OnCid37Add` | cid 0x25 vt23: base 0x183FB8 then EnsureSwCallback(D_002A4BB0) | 0.35 | VERIFIED (arbitration/conflict fix; was BootyContainer::VUnk22?) |
| [0x1613A8](../../src/auto/func_001613A8.c) | `StartupGame` | Once-per-boot game startup from main: language-dependent init (sceScfGetLanguage), broadcast game event 0, init the game sequence queue D_002E1E88 | 0.75 | HYP |
| [0x1613F8](../../src/hand/func_001613F8.c) | `InitGame` | Resets the 0x1C-byte GAME record g_game (D_002DEB30): zero, oidJob/oidTask/oidTaskPending (+0x10/+0x14/+0x18) = -1; ResetGs(g_gs,0); InitDifficulty; D | 0.55 | HYP |
| [0x161478](../../src/hand/func_00161478.cpp) | `UpdateGameTimers` | Per-tick: adds dt to the total-play-time save var (g_sw+0x3B1C) and to the current episode's play-time var (g_sw+0x3B58[NWorldCur()], 9 episodes); add | 0.75 | HYP |
| [0x1619A0](../../src/hand/func_001619A0.c) | `BroadcastGameEvent` | Game event bus: HandleGameEvent(g_game), difficulty 0x154A90, every AVAILABLE task (list g_sw+0x3B00) of the current job (SendTaskGameEvent), then the | 0.70 | HYP |
| [0x162448](../../src/auto/func_00162448.c) | `InitJob` | JOB (cid 0x84) Init: LO Init, difficulty-ease thresholds 480/720 s, ids -1, InitDl(+0x80, link 0x84) for its tasks | 0.70 | HYP |
| [0x1633E8](../../src/auto/func_001633E8.c) | `FIsCurrentJob` | p == g_sw+0x3B0C (current job) | 0.70 | HYP |
| [0x1635A0](../../src/hand/func_001635A0.c) | `InitTask` | TASK (cid 0x85) Init: LO Init, ids -1, counts tasks in g_sw+0x3AFC | 0.70 | HYP |
| [0x164018](../../src/auto/func_00164018.c) | `FIsCurrentTask` | p == g_sw+0x3B10 | 0.70 | HYP |
| [0x1C4E50](../../src/hand/func_001C4E50.c) | `InitDprize` | DPRIZE::Init (vt7): InitSo + 0x1F7918 setup | 0.85 | VERIFIED |
| [0x1C4EF8](../../src/auto/func_001C4EF8.c) | `LoadDprizeFromBrx` | DPRIZE::LoadFromBrx (vt14): LoadSoFromBrx + snip objects (0x1A64E8) | 0.80 | HYP |
| [0x1C4F60](../../src/hand/func_001C4F60.c) | `PostDprizeLoad` | DPRIZE vt15 PostLoad: base + resolve linked objects via SW lookup | 0.75 | VERIFIED |
| [0x1C5148](../../src/hand/func_001C5148.cpp) | `ProjectDprizeTransform` | DPRIZE vt17: damped spring/hover integration of the prize position (state 6 -> base) | 0.75 | HYP |
| [0x1C55E0](../../src/hand/func_001C55E0.cpp) | `CheckDprizeBounce` | query nearby solids (0x20FAA0) and resolve deepest contact (bounce) | 0.70 | HYP |
| [0x1C5E48](../../src/hand/func_001C5E48.cpp) | `UpdateDprize` | DPRIZE vt16 Update: base update, sparkle (0x1C6308), bounce, collect | 0.80 | HYP |
| [0x1C5FA0](../../src/hand/func_001C5FA0.cpp) | `RenderDprizeSelf` | DPRIZE vt19 RenderSelf (bob/spin then SO render) | 0.65 | VERIFIED |
| [0x1C60B8](../../src/hand/func_001C60B8.c) | `SetDprizeDprizes` | DPRIZE state machine setter (unlock proxy copy, attach/detach, state timestamps) | 0.75 | VERIFIED |
| [0x22A908](../../src/hand/func_0022A908.cpp) | `RenderWaypt` | fades the job marker, draws 3 child SOs scaled by distance, job name plate | 0.70 | VERIFIED |

Selected 19 of 193 named functions (confidence >= 0.7, verified, or flagged). Full list: [reference/functions/game-progression.md](../reference/functions/game-progression.md).

## Open questions
- Real SP names of CID 0x84/0x85; is the m/t save naming exactly JOB/TASK?
- Why "% complete" counts only tasks with successors.
- Ids of CIDs 0x5C/0x5D (charm? gadget power?) and booty's exact states.
- Experiment: break 0x1619A0 through a mission and log (id, arg) to complete the event table.
