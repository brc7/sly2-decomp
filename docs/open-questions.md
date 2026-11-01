# Open questions and suggested experiments

Consolidated from the per-TU write-ups, CONFLICTS.txt, the verifier and arbitration passes, runtime rounds 1-6, the format work and the master-name
reviews. Grouped by subsystem; most important first. "Exp." = a concrete experiment: a PCSX2 breakpoint or memory watch, a pnach patch
([guides/playtesting](guides/playtesting.md)), a save-state pair (take A, do one thing, take B, diff with the runtime scripts), or a harness spec once the
PINE harness runs. Questions answered since the first draft are in the table at the end.

## Top 5 (highest leverage)

1. **The game event ids.** Events leave no trace in snapshots; only the Splice-handled ids (9, 11, 16, 19, 23, 24, 26, 29, 61) and a few code paths are known.
   Exp.: breakpoint `BroadcastGameEvent` 0x1619A0 and log (id, arg) through one complete job (first harness experiment candidate).
2. **End-to-end GS check of the rendering model.** Everything in [rendering](engine/rendering.md) is read from code and VU disassembly; the shadow-volume face sign,
   the TEX2 CLUT, and the producers of kinds 2/8/11/13 still need a GS dump. Exp.: rendering EXPERIMENTS E1 (zero one kind's RPLs per screenshot), E2/E3 (GS dump of a
   frame with Sly and guards), E4 (relight readback after changing ambient).
3. **Unseen character states.** jts 3/6/9/10/13/14/16, JTHURT 8/9/11/12, Murray 6/8/9/12, Bentley 1/2/4-11/13/14, hover pack, adrenaline burst; HND hooks (none in the hub).
   Exp.: save states in a hook level and during each Bentley/Murray gadget; log (a1, a2) at `SetJtJts` 0x18C250 / `SetMurrayState` 0x1B6078.
4. **Modding round trip for world files - first live test FAILED in part (v3).** The pipeline (writer, LZSS compressor, disc patcher) is built and
   verified offline 43/43, but the first modded ISO lost all SFX and the moved car did not visibly move. See
   [Modding live test](#modding-live-test-2026-11-01) below.
5. **The respawn point and level-reload rules.** A death reloads the level with save vars kept; which marker places Sly (start marker vs JTPLACES vs a checkpoint
   task's data) is unknown. Exp.: die after reaching a checkpoint task in a mission; watch TASK +0xC0 and the placement.

## Object model / world
- LO slots 17, 20, 21, 26/27 (26 = `ResetLoForProxy` proposed by the top-50 round), 32, 37-39, 42-45.
- Second oid table at g_sw+0x1838 (1,752 non-object records).
- 10 PROXY records in L02 whose idSource matches no WE.x10; the PROXYSPARE bookkeeping behind `PspareAcquire` (copy choice consistent, not located).
- Real file name of the LoadSwFromBrx TU (sorts bq < x < brx) and of the RELAY/JSG TU.
- What the overlay D_002DD644 is (NULL in all 55 snapshots, including mid-load).
- Whether the boot step queue 0x1A4E08 and the game sequence queue D_002E1E88 are one structure.

## Rendering
- See Top 5 #2. Also: which models set the foreground bit 34 (E5); when the render-to-texture slots run (E6); whether anaglyph stereo is reachable (E8).
- Blip curve meanings and the three variants; B-family optional stage uses (morph, rim fade).
- Mapping of GLOBSET file types 1-3 onto render subtypes 1-3; SAA payloads; CLAMP per shader.

## Camera
- Which of CPZOOM / CPHANG is CM+0x4C0; policy slot 7; CID21 (0x15).

## Player
- Top 5 #3. Unnamed STEPPLAYER slots 82-87, 90-92; JT slots 49/50/55/57/85/90-98/103/106.
- Trigger-bomb, fists-of-flame and Voltage Attack damage paths; how the cane hit part (SOPART 0x14C) becomes a zap; dart hitting a guard; what returns a thrown prop; D_003E1BEC.
- The other player BHVs (193, 195-198) at runtime.

## Guards
- Flags 0x20 / 0x2A; awareness mode 3 and modes other than 1/2/4/5/7/11; what promotes suspicious -> alert.
- Roles of BHV161, 162, 175, 176, 178, 179 (circle; never seen), 183, 187-189, 191 (placeholders).
- Rhino sword-missile launch path; projectile damage fields; the InflictZap de-dup list remover.
- Engagement on-screen rule (`FTargetRecordAllowsGuard` 0x20A818) with one guard off-screen.
- Is CID119 (0x77) the reinforcement spawner? Exp.: trigger an alarm, break 0x174490.
- The guard +0xDC0 KO sub-state block.

## Progression / collectibles
- Top 5 #1. JOB visibility/RSMG record offsets; complete-event and fail-event lists; `g_game+8`.
- How CID35 trigger spots are triggered (button vs proximity); booty states at runtime.

## Save / memcard
- The autosave in progress (memcard busy between FinishJob and the next load); SAVE state enum and dialog states; schema kinds 5-10.
- Exp.: save-state pair across a save/load cycle to confirm the 0x5464-byte slot layout.

## Sound / audio
- Bank sound index -> game sfx id join (world sound section `aisnd`); non-TONE grain types; SUKR commands 0-12; VPK per-channel byte 0.
- A batch decode of all 1,880 manifest files (only one sample per kind was checked).

## HUD / UI
- Which screen class is which (Show 0x1D98B0); STRINGGEN entries 44-103; gadget-select menu (D_003DA0F0), ThiefNet loot tab, job map and title screens (no snapshot yet).

## Physics / navigation
- SO slots 49-60 and 65-72; `WakeSo` 0x1F5078 / `SlotUnsettle` 0x230598 confirmation (Exp.: break 0x230598 during a jump/land, roles 10 -> 7).

## Animation / cutscenes
- ASEGA event kinds 13-16, 20; RELAYCK names for the ~70 kinds not seen live; `StartJtMode(7, 8)` argument roles.

## Names and classes (cross-cutting)
- Human decisions from master_names_v2 REPORT s4: the two `FBounceLeaf` bodies (0x1CF968 vs 0x1D0BF8); the two rich-text classes (`CRichText` vs hud-TU class,
  `CRichText2` proposed); `LoadClutsetFromBrx` vs `LoadSubglobBindingsFromBrx`; approval of the structural-T1 and BHV class-word rules and of the 25 whole-game class words.
- Apply PENDING_FIXES (EXIT/WARP method names, class names VOLBTN/LASEN/PATHLINK/HND/..., top-50 merges, runtime6 renames) and update `types/cid_table.csv` labels.
- Compile arbitration's new manglings in the scratch build before committing (`SetupCm__FP2CM`, `SolveDzg__FP3DZGi`, `FAddRsmg__FP2SWP4RSMGiPii3OIDN25`, ...).
- 12 functions with no proposal at all; 113 used Splice options without a name; MISSILE vtable 78 vs 79 slots (CONFLICTS #4).
- Rename-tool policy questions (`rename_tool/OPEN_QUESTIONS.txt`): plain names, asm-label aliases, `main`, functions.csv sync, data symbols.

## Modding live test (2026-11-01)

Source: modding/MODDING.txt (pipeline v1, demo `mod_L02_floating_car`: PROXY oid 138404 in WE 286 moved +350 z, 4 bytes changed, L02 rewritten
**inplace** with the retail record size) and modding/debug_sfx/FINDINGS.txt (debug agent, marked "work in progress - preliminary").

- **Observed** `[C:live]`: the modded ISO boots and Paris loads, but **all SFX are missing** for the level, and the car **did not visibly move**.
- **Root-cause candidate (SFX)** `[H debug_sfx]`: our compressed L02 stream is shorter than retail and padded with zeros to the record size, so ~100 KB
  (49 sectors) of the level file are still unread when `LoadSwFromBrx` finishes; the async CD read is still busy when `PostSoundLoad` 0x1FD5E8 calls
  `snd_BankLoadByLoc` 0x231FB0, which returns 0 on "CD BUSY!" -> no SFX bank for the whole level. Spot-check v3: the string
  "snd_BankLoadByLoc: CD BUSY!" exists in the ELF (vaddr 0x2AA280) and master_names has `PostSoundLoad` -> bank load `[C:static]`; the timing link
  itself is **not verified** (no RAM evidence from the live state was available here). Note the ~100 KB equals the documented L02 inplace
  headroom (100,239 bytes), which fits the explanation. Likely fixes to test: pad with a stream the game finishes reading (shrink the read size in the
  FILEREC copies = `resize` mode), or make our output the retail length.
- **Car did not move** `[H]`: no root cause yet. Candidates from MODDING.txt section 4: (ii) the x60 spare-copy pooling poses copies from something other
  than the PROXY placement; or the edited oid is not the car seen on screen; or (if the read issue above truncates data) the level was not loaded from
  our bytes as expected. Exp.: read the car's XF matWorld (XFM+0xD0 / SO XF+0x70) over PINE in the modded game and compare with +350 z.
- Not affected: compressor/writer correctness offline (43/43 under the game-decoder model).

## Resolved since the first draft
| Question | Resolution | Tag |
|---|---|---|
| (v3) Stream field g50 | duration in seconds (7,849/7,970 records within 0.05 s of the size-predicted duration) | [C:parse] audio_browser |
| (v3) TASK +0x5C "music" | it is the objective text id (eopid 1265) | [C:static] + [C:decode] splice_browser |
| (v3) Does a rate-slot write change game speed? | not alone: rDt (+8) is a cached product, recomputed only by SetClockRate; live test 0.25 -> no effect until rDt written | [C:live] + [C:static] |
| (v3) Can the warp request be written directly? | yes: SW+0x3BF0..+0x3C3C written over PINE warped Paris -> India L08 | [C:live] + [C:static] |
| (v3) Memory-card save checksum? | none: save%d = 0x5408 bytes = raw g_gs + schema fingerprint | [C:static] save_editor |
| (v3) Episode menu lower bound | can reach 1 (v2 "never below 2" was wrong) | [C:static] |
| BHV behaviour classes (old Top 5 #1) | 14 classes role-confirmed on live stacks (BHVGUARD ... BHVPATROLPATH, BHVKO, BHVJUGGLED); awareness modes 1/2/4/7/11 mapped; flag word = union of stack flags in 59/59 states | [C:runtime] rounds 2, 4 |
| What happens inside a level transition (old #2) | corrected order: file opened before DeleteSw; ResetClock, AddGrfusr(8), CMpeg::ExecuteOids inside; loading screen started by LoadSwFromBrx; FILEREC request XOR-obfuscated | [C:runtime S56] + code |
| Render pass table semantics (old #3) | all 26 kinds and 12 passes described; VU programs disassembled (relight formula, outlines = 1-px AA silhouette lines, shadows = Z-fail volumes in the green channel) | [C:static] |
| JOB/TASK and save naming (old #4) | m<N> = JOB, t<N> = TASK; task states 0/1/2/3; % complete = state-3 tasks with successors; banner pauses clock slot 7; autosave from FinishJob | [C:runtime runtime6] |
| Sly's state enum and gadgets (old #5) | jts 0/1/2/4/5/7/8/11/12 observed; JTAIR/JTGRAB/JTHURT sub-states; gadget table D_002BCD00 decoded; guard specials = gadget ids | [C:runtime] rounds 2-4 |
| JTMODE kind 1 = binocucom? | REFUTED: binocucom is kind 0 (CPLOOK prio 7) for all three characters; kind 1 is the scripted recon view (CM+0x520, prio 8) | [C:runtime] |
| CM+0x450/0x4C0/0x500/0x520 assignment | 0x450 CPLOOK, 0x500 CPCAMBLEND, 0x520 recon camera (0x4C0 used under tables) | [C:runtime] |
| PIPE vs HND (CID 0x0B / 0x13) | 0x0B PIPE (lamp posts), 0x0D RAIL (wires), 0x0C walls, 0x1B tables; no HND in the hub | [C:runtime] |
| CID 0x5C / 0x5D identities | KEY / MEDICINE | [C:runtime] + [C:parse] |
| cid 0x67 "ALARM" | BOOTY (41/41 instances are WE booty) | [C:parse] + arbitration |
| Sub-glob loader numbering | type 1 0x16E438, 2 0x16CAA0, 3 0x16CF78 (jal switch) | [C:static] + [C:parse] |
| Class "0x505" in the relight pass | LIGHT objects (CID 4), 3 qw each | [C:static] |
| Colour-correction flag D_003E12C8 | it is the anaglyph-stereo flag (g_sw+0x3E38); not used in normal rendering | [C:static] |
| Writer/use of D_002DE5A4 (VU qw 0xD) | only consumed by unreachable debug-break hooks | [C:static] |
| World file trailing bytes | LZSS ring echo (decompressor run-off), never read | [C:parse] |
| Sound bank / stream formats | 989snd SBlk v3, VPK (0x1000 interleave), VAGp 22050 Hz | [C:parse+decode] |
| Splice builtins 97-103, 137 | level index, episode, world, current job/task/pending task, difficulty, hint box | [H top50] (code) + [C:runtime] (100-102) |
| SW+0x3D7C "user lock" | the no-pause counter (`IncrementSwNoPause`) | [C:runtime runtime6] |
| Checkpoint manager in sw.cpp | REFUTED: 0x20DF80 = SetSwTransition; death = level reload, no checkpoint record | [C:runtime] + arbitration |
| `StartupCm` vs `SetupCm` (CONFLICTS #3) | SetupCm (empty), two Sly 1 call sites, no startup-table entry | [C:verified arbitration] |
| `AddAlarmSensor` (CONFLICTS #2) | `AddLootEntryOid` (LOOT +0x244) and `AddBootyEntryOid` | [C:verified arbitration] |
| `RestoreChkmgrFromCheckpoint` / `FAddRsmg` signatures (CONFLICTS #10) | `SetSwTransition(SW*, TRANS*)`; FAddRsmg signature fixed | [C:verified arbitration] |
| Overlay = loading cover? | no (NULL during a load) | [C:runtime] |
| Guard species and ranged attacks | one template per species (swarmer rat/monkey/goat, surprise cobra, boar/rhino GUARD); BHV173 = SHOOT; goat rocks = BOMB, rhino swords = ACCMISS | [C:runtime runtime7a] |
| "noise" stimuli / gadget lure | confirmed: stimulus templates with radius/height/ramp; the alarm clock's stimulus re-targets searching guards; combat guards emit alert calls | [C:runtime runtime7a] |
| Slow-motion gadgets 0x1D / 0x2A | Thief Reflexes / Time Rush: clock slot 1 at 0.25 / 1.35; MP drain 2.0 / 1.0 per game second | [C:runtime runtime7b] |
| Gadget table +0x10 | unlock episode (ThiefNet stock rule), not shop order | [C:runtime runtime7b] |
| Episode unlock and displayed % | max episode from dt == 0 and previous < 100%; display = 95% x percent + 5% vault power-up | [C:runtime runtime7b] |
| Cane attacks in the jts table | jtcs overlay (12 = ground combo hit) on jts 0/5; hit window via SOPART 0x14C | [C:runtime runtime7a/7b] |
| JTHURT+0x90 | post-hit mode (damage forced to 0), not "alive" | [C:runtime runtime7a] |
| AI partner control | top BHV slot 14 writes the POCTL; Murray [BHV194 follow, BHV168 walk-to-goal]; started by Splice optid 446 | [C:runtime runtime6] |
| Earlier rows (LO slots 22/23, vtable stride, clone slots, 989snd names, health location, SW+0x3B20, option k0, CMPART2 = CPTN, S04 not alert) | unchanged from the first draft | [C:verified] / [C:runtime] |
