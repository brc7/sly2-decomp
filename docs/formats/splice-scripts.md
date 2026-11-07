# Splice scripts in world files

Level scripts ("Splice", a compiled Scheme dialect) are stored per world entry as serialized lists. The tool `reports/splice_scripts/tool/sly2splice.py` (research workspace, not in this repo) walks, decodes,
re-encodes and pretty-prints them. Runtime side: [splice-scripting](../systems/splice-scripting.md).

Tags: `[C:static]` = matched code; `[C:parse]` = the walk consumes every byte and **re-encodes byte-identically: 10,930/10,930 entries in all 43 world files**
(2,697,151 bytes); `[C:data]` = holds for every instance; `[C:runtime]` = seen bound in live frames (runtime6); `[H]` = inferred.

## 1. Where the scripts live `[C:static]` + `[C:parse]`

Per world entry, right after `cbBlob, idBlob, fExtra`: `U32 csplice; csplice x ENTRY` (`LoadSwSpliceFromBrx` 0x1FF858), followed by the texture tables
([level-objects.md](level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx)). ENTRY = one serialized list, plus 3 x string16 (object path, file, source)
only when fExtra (never in retail). Sly 1's symidMax and extra-option count are gone.

**Binding**: object records carry `S16 isplice`; `GetSpliceFrame` 0x1FFA40 indexes the **current WE's** table (g_sw+0x39E8, cleared after the WE's objects load), so
isplice is local to its WE. Each entry is evaluated once by `LoadSpliceEntryFromBrx` 0x1FFA70 into a fresh frame; all objects with that isplice share the frame.
Many entries are used only by objects nested inside other payloads (ASEG/SM callbacks, rig children).

## 2. Serialized list (`DeserializePair` 0x12A3D8)

```
list := 0xFF (null) | CELL
CELL := U8 b; [fExtra: S32 + 6 x S16 debug info]; payload(tag = b & 0x7F); (b & 0x80 ? CELL-or-0xFF : end)
```
The car of a cell is its tagged value; bit 7 says a cdr follows; tag 31 = nested list.

| Tag | Payload | Tag | Payload |
|---|---|---|---|
| 0 None, 1 set!, 2 define, 5 assert, 6 form6, 7 if, 8 or, 9 and, 10 cond, 11 else, 12 case, 13 let, 14 while, 15 lambda, 16 begin, 17 import, 18 quote, 20 (Sly 1 Pipe?) | none | 21 | S32 |
| 22 | F32 | 23 | Bool (U32) |
| 24 | Symid (U32) | 25 | Bifk = builtin index (U32) |
| 26 Vector / 28 Clq / 30 Smp | 3 x F32 | 27 Matrix | 16 x F32 |
| 29 Lm | 2 x F32 | 31 Pair | nested list |
| 33 Basic | U32 (always 0 = null object) | other | nothing |

Versus Sly 1: one special form inserted at 6 shifts everything >= 6 by one; Sly 1's Dot (19) is not accepted; debug info is S32 + 6 x S16 instead of 5 x U16 `[C:static]`.
Observed `[C:data]`: every entry is a one-cell list whose car is `(begin ...)` (a "module" of top-level defines); forms 6, 13, 17, 20 and literal vectors/matrices never
occur (vectors are built with the `(vector ...)` builtin); object literals are always null (objects are found at run time by oid). Whole-game form histogram: define
34,763, lambda 24,194, begin 14,644, case 4,772.

## 3. Names

**Builtins** (tag 25): table D_002BBB90 = 169 x `{pfn, cargMin, fRest}`, applied by `EvalApply` 0x1272E0 `[C:static]`. Spellings come from Sly 1's builtin names via
the slot mapping; Sly 2-only builtins carry team names with `~` `[H]`. 97 of 169 are used in the whole game; 140-168 (nil stubs), 132 `set-save-var!~`, 134, 98 and all
type predicates are never used `[C:parse]`. Roles resolved by the top-50 round (code read): 97 current level index (was `load-filter~`), 98 episode, 99 world,
100/101/102 current JOB / TASK / pending TASK (confirmed live, runtime6), 103 difficulty, 137 hint box (`find-hintbox-object~`), 118 the sound manager, 114 objects
from an oid list, 41 shuffle. Most used `~` builtins: `find-sound-object~` 1454, `find-ticker-object~` 802, `load-filter~` 686.

**Symbols** (tag 24) are global across levels `[C:data]`:
- **0-28 are event symbols.** `HandleBasicSpliceEvent` 0x1331F8 looks up symid k in the object's frame and calls `(k <object> args...)` with arguments typed by
  `s_aotypEvtParm` D_0027A108[1 + `s_aiEvtParm` D_0027A1D8[k]] (read from 0x27A10C) `[C:static]`. A handler is `(define <k> (lambda (self arg...) ...))`. Meanings: 0 post-load, 4 label (ASEG/RELAY
  label callbacks), 5 trigger / 6 untrigger (`HandleLoSpliceEvent` 0x1A61E0 also broadcasts game events 0x1A/0x1B), 14 hit, 17 game event (JOBs; case 16 = hero died ->
  RestartJob), 19 activate / 20 deactivate (TASK/JOB) `[H]`, with 0, 6, 17, 19, 20 bound in live mission frames `[C:runtime]`. Whole game: evt19 3358, evt4 3036, evt0 2100, evt5 1418.
- **30-989** are named by the save-schema symbol table in every level head (`LoadSaveSchemaFromBrx` 0x1D7B88: `U16 n; n x {U16 symid; string16 name}`, identical in all
  files). Save-var builtins use them as key paths (`BuiltinGetSaveVar` 0x125398 -> trie keyed by symbol id) `[C:static]`; the same names serve as script variables (`murray`,
  `bentley`, `flags`, `episodeBest`).
- 1002 = the first parameter of 1,224 handlers ("self"); 990/1038 likewise. Everything else (990-4057) has no name on disc (printed `sNNN`).

**Options** (`addo/ensureo/seto!/geto`, builtins 108-111; second argument = S32 option id; method calls are `((geto obj OPT) args...)`) `[C:static]` (`BuiltinObjectOption` 0x123EB0):
- ids <= 0x4D0: class options via per-class tables `g_mpcidarysplopt` D_00279AD0 (199 x {c, SPLOPT*}) -> `g_aeopid` D_002AC880. 1,091 distinct optids; 210 bind to different
  records on different classes (polymorphic: TRIGGER, APPLY, SEEK, state; e.g. 446 = `StartStepplayerFollow` on STEPPLAYER, the root BHV point on STEPGUARD) `[C:static]`.
- ids > 0x4D0: the object's sidebag (`GetBasicOption` 0x200BA0); scripts use 1255-1875 (`sidebag-N`; N = symid of the custom name `[H]`).
- Label resolution order (`splnames.py`, research workspace, not in this repo): hand overrides, Sly 1 option names, the native target of the option's thunk, getter/setter pointer-to-member names, documented
  fields, else `Class+0xOFF`. Full table (research workspace, not in this repo): `splice_scripts/OPTION_TABLE.tsv` (1,449 bindings). Whole game: 1,016 option ids used, 142 still offset-labelled (top: ASEGA+0x18
  x3358 = option 251 ASEGA/SMA root object, TASK+0x7C x1642 = the task's job, JOB+0x7C, JOB+0xF0, BOMB+0x1F0) (research workspace, not in this repo: `whole_game/splice/options_used.csv`;
  `top50_round/splice_builtins_options.csv` proposes names for 29).

**Object ids**: the first argument of `find-object(s)` / `find-nearest-object(s)` is an oid; oids are global (sly 7, bentley 8, murray 9; the same jobs/relays in
every Paris level). The listing prints `@oid<Class:WE>`; L02 names 529 of 611 oid literals; game-wide 10,535 of 18,439 references resolve to a class `[C:parse]`.

## 4. What the scripts do (Paris hub, readings `[H]` unless the text shows it)

- Mission logic lives in WE `f_nightclub_macro` (91 TASK, 28 RELAY, 11 JOB entries): JOB evt17 handlers restart on hero death and advance tasks; TASK evt19 handlers fire
  RELAYs (`FireRelayFromPlayer`), show ticker hints, start radio lines (`StartDialog`), and call `RequestTaskState` 2-5. This matches the live mission (runtime6): e.g.
  `t1_sat_dish1` evt19 fires RELAY 0x6E3; `t1_recon_brawl` evt19 calls option 446 on Murray to follow Sly `[C:runtime]`.
- Level rigs: volume buttons add/remove objects; ASEG label scripts choreograph ambushes; handlers call the base handler first (`(find-binding-or-noop~ 'evt5)`,
  script "inheritance").
- Props: breakables (CID98) free physics constraints on hit; awnings bounce the player (`LaunchStepplayer`); the pipe-room puzzle propagates a sidebag "pressure".
  Per-WE summaries (research workspace, not in this repo): `splice_scripts/PARIS_HUB_SCRIPTS_SUMMARY.txt`, `whole_game/splice/script_index_by_we.txt`.

## 5. Modding notes

`write_list` produces valid bytes for a modified entry, but the world file must then be re-laid out (later sections shift, Align(0x10) per WE, LZSS re-compression).
Not done yet.

## 6. Open

Event meanings beyond the hints; names for symids >= 990 (no debug strings: fExtra = 0 everywhere); forms 6 and 20; 113 used options without a proposed name.
