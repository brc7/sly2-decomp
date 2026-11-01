# Open questions and suggested experiments

Consolidated from the per-TU write-ups, CONFLICTS.txt and the verifier/runtime passes. Grouped by subsystem; the most important first
within each group. "Exp." = a concrete experiment (PCSX2 breakpoint, memory watch, patch, or a save-state pair: take state A, perform one
action, take state B, diff with `slyram.py`/`savevars.py`/`probe.py`).

## Top 5 (highest leverage)

1. **The BHV behaviour classes.** ~43 classes (CID 0x9C-0xC6) drive all guard and partner AI, and the BHV TU 0x174898-0x17F090 (189 functions)
   is unnamed. Only BHV156 (root), 169 (script runner), 171 (search), 172/190 (reaction) have roles. Exp.: save-state pairs while a guard
   *spots* Sly, chases, searches and gives up (none of S02-S05 has a spotting); break 0x1EEE00 (push, a1 = cid) and log stacks with `probe.py`.
2. **What happens inside a level transition.** No save state covers a load. The order DeleteSw -> SW::SW -> LoadSwFromBrx -> PostSwLoad ->
   checkpoint restore (0x20E890 / 0x20E740) is from code reading only, and vt9-on-every-object after a world entry is unexplained.
   Exp.: breakpoints on 0x20E068, 0x20D9D0, 0x13DA18, 0x20E740, 0x20DF80 entering a level; log `cid/oid` at 0x13FB40 for a census.
3. **Render pass table semantics.** The 26 RPL kinds and 12 passes (D_002BDD20 / D_002BDD88) are the main hook for a native renderer; only
   a few passes are understood. Exp.: dump the sorted RPL list per frame at 0x157FA0; patch a kind's pass to 0, or a pass hook to `jr ra`, and see what disappears.
4. **JOB/TASK and the game event ids.** The progression model (CID 0x84/0x85, event bus 0x1619A0) is coherent but its names are coinage and
   most event ids are unmapped; no state has a running job. Exp.: break 0x1619A0 and log (id, arg) through one complete job; save states
   before/after starting a job (watch SW+0x3B0C/+0x3B10, g_game+0x10..0x18, `t*_*.state`).
5. **Sly's state enum and gadgets.** jts values beyond 0 (stand) and 11 (pickpocket) are not mapped, nor gadget ids, the gadget table
   D_002BCD00, or the guard special states they cause. Exp.: log (a1, a2) at `SetJtJts` 0x18C250 while running, jumping, hanging, climbing;
   break `JT::UseGadget` 0x18DA48 / `SetStepguardSpecial` 0x207030 with each gadget.

## Object model
- LO slots 17, 20, 21, 26/27, 32, 37-39, 42-45: shape-only names. Compare overriding methods across subclasses.
- The second oid table at g_sw+0x1838 (1,752 non-object records at runtime): what is it? (INCONCLUSIVE)
- Exp.: watch g_sw+0x2C (active roots) while walking away from a guard: the guard's root should leave the list and `OnStepguardRemove` 0x2079A0 fire.

## World / level
- Real file name of the LoadSwFromBrx TU (sorts bq < x < brx); WE fields x5C/x60/x14; load filter semantics beyond "level index".
- Is the boot step queue 0x1A4E08 the same structure as the game sequence queue D_002E1E88?
- Hands-off/lock counters SW+0x3D78..+0x3D84 are only "consistent" (all 0). Exp.: save state during a cutscene.
- CID 0x74 created by PostSwLoad; D_00443740 == 1 in 0x20E040.
- `RestoreChkmgrFromCheckpoint` and `FAddRsmg` signatures contradict Sly 1 (CONFLICTS #10).

## Main loop / startup
- 0x14D470 `StartupCm` vs Sly 1 `SetupCm` (CONFLICTS #3).
- Unverified startup-table entries: 0x21F100, 0x14B660, 0x15EF00, 0x1C90D0, 0x14CFE0, 0x1FCEA0, 0x138FC0, 0x211528, 0x1B2940.
- Overlay D_002DD644: wipe, fade, or loading cover? Exp.: break 0x1477B0 returning 0 during a level fade.

## Splice
- Special form 6; builtins 97-104, 114, 118, 133, 137; are the 29 nil stubs dead? Exp.: break 0x1266C8 and 0x1258F0..0x125FF0.
- Tag 0x23; which static pool is frame/proc/method/sidebag.

## Rendering
- Sub-glob type loader numbering/dispatch (0x16CAA0/0x16CF78/0x16E438); class "0x505" uploaded in pass 4.
- Colour correction flag D_003E12C8: option or level flag? Exp.: toggle before load and compare colours.

## Camera
- Assignment of embedded policies +0x450/+0x4C0/+0x500/+0x520 to CPLOOK/CPZOOM/CPHANG/CPCAMBLEND; policy slot 7; CID 0x15.
- Exp.: break 0x14FF38 (ActivateCplook) entering binocucom; dump the CAMMOVE at 0x151700 during a cut-scene blend.

## Player
- Unnamed STEPPLAYER slots 82-87, 90-92; JTMODE kind 1 = binocucom? (break 0x19CC50); recon selector confidence ~0.6 (break 0x202768 a1 == 8).
- PIPE vs HND among CID 0x0B / 0x13; in-game identity of each GRABSPOT class (break their Init at load).
- Health widget loop anim 0x340 seen in S02/S03.

## Guards
- Alert flags 0x1D/0x20/0x2A not observed; meaning of the reaction BHV190/BHV172; BHV slot names 8, 11, 12, 14.
- Is CID 0x77 the reinforcement spawner? Exp.: trigger an alarm, break 0x174490.
- Engagement token rule (0x20A818): confirm "one guard attacks at a time, on screen" with two guards engaging.

## Progression / collectibles
- Real names of CID 0x84/0x85; identities of CID 0x5C/0x5D; booty states; why % complete counts only tasks with successors.
- CONFLICTS #2: 0x1C7A10 `AddAlarmSensor` (writes LOOT's +0x244) still disputed between verifier and r5.

## Save / memcard
- SAVE state enum and dialog states; schema kinds 5-10. Exp.: pull the card at slot select, break 0x1ACB58.
- Exp.: save-state pair across a save/load cycle to confirm the 0x5464-byte slot layout (`g_save` 0x2E65E0 + 0x24).

## Sound
- SUKR RPC commands 0xB/0xC; 989snd commands 0x18/0x21/0x5A/0x5C (break 0x232838, log a0); stream "pairs" (lip-sync?); bank format.

## HUD / UI
- Which screen class is which (break Show 0x1D98B0, read vtable); STRINGGEN entries 44-103.

## Physics / navigation
- SO slots 49-60 and 65-72; suspect `fIgnoreLocked` label on 0x1F2768.
- Exp.: break 0x1F9410 pushing a crate; 0x1C0D70 as a guard crosses rooms; 0x1C21D8 when a chase crosses zones.

## Animation / cutscenes
- Curve/actuator class identities in act? (0x12B3B0); ASEGA event kinds 13-16, 20 (break 0x131290).
- RELAYCK names for ~90 kinds; real file name of the RELAY/JSG TU.

## Class identities and TU names (cross-cutting)
- 86 unidentified CIDs (class-tree "?"), most resolvable by matching their LoadFromBrx/Init slot bodies to Sly 1.
- MISSILE vtable length 78 vs 79 (CONFLICTS #4).
- Placeholder names (`InitCid84` style) must be replaced once classes are identified, never committed as final (CONFLICTS #7).
- Re-generate the TU table with the 3-stub (`_GLOBAL_$D`) rule (CONFLICTS #9).

## Resolved since the first drafts (for the record)
| Question | Resolution | Tag |
|---|---|---|
| LO slots 22/23 (CONFLICTS #1, #5) | 22 = OnRemove, 23 = OnAdd; fire when FIsLoInWorld flips (structure + proximity) | [C:verified] arbitration |
| vtable stride / cid_table vtable_len (#6) | 8-byte slots; vtable_len already counts slots | [C:runtime] |
| Clone slot | 40 CloneHierarchy, 41 Clone | [C:runtime] |
| 989snd wrapper names (#8) | r8's command-id decoding | [C:verified] |
| Health location | `sly.hpCur` = 0x3D4AB0 via JT+0xE00 | [C:runtime] |
| SW+0x3B20 collectible counter | `loot.ccoin` key | [C:runtime] |
| Option key k0 | `flags.lang` (not vibration; vibration is k7) | [C:runtime] |
| CMPART2 at CM+0x3B0 | the TN (CPTN) policy | [C:runtime] |
| S04 "guards alerted" (states/info.txt) | REFUTED: S04 is patrol; the reaction is in S05 | [C:runtime] |
