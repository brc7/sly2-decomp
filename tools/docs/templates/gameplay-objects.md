# Gameplay object classes

Smaller gameplay classes that do not belong to one subsystem. Most are new in Sly 2 and only partly identified; names are mostly placeholders.
Class numbers in hex; see [class-tree](../reference/class-tree.md).

| Class | TU | What it is | Tag |
|---|---|---|---|
| BARRIER (0x56) | barrier.cpp 0x1320C8 | Sly 1 barrier minus the warp logic; impacts go through a deferred SW callback (0x132238 -> 0x132298) that fires Splice event 5; `FIgnoreBarrierIntersection` 0x132140 (tri-state) | [C:verified] (0x132140), [H r2b barrier.md] |
| BOMB (0x4F) | bomb.cpp 0x13A9E8 | Sly 1 bomb.c by slot and body: Init, PostLoad, HandleMessage, Update, custom XPs, `ApplyBombThrow`, `PrimeBomb`, `DetonateBomb` | [H r2b bomb.md] |
| MISSILE (0x50), ACCMISS (0x51), CID 0x52 | missile.cpp 0x1B1DB8 | guided projectiles derived from BOMB; vtables have 79 entries, slot 78 = Fire (cid_table says 78: CONFLICTS #4); `FIgnoreMissileIntersection` 0x1B1F40 | [C:verified] (0x1B1F40), [H r5 missile.md] |
| BUTTON (0x53), VOLBTN (0x18/0x19) | button.cpp 0x148620 | Sly 1 button.cpp; fires Splice events 5/6 on trigger/untrigger; RSMG records; extra event-listener list | [H r2b button.md] |
| SENSOR (0x54), laser beams LASEN (0x55; runtime layout in [guards-ai](guards-ai.md#5-perception), zap 0x1000 -10 [C:runtime S64]) | sensor.cpp 0x1DE2A0 | no-trigger lists; up to 64 beams between fixed or path-moving endpoints; the nearest blocker is zapped (kind 0x1000), unlike Sly 1's alarm lasers | [H r6b sensor.md] |
| BOOTY (0x67) | booty 0x13BDF0 | carried booty object (all 41 instances in the game are WE `booty`; replaces cid_table's ALARM). See [game-progression](game-progression.md#5-booty-booty-tu-0x13bdf0) | [C:parse whole_game] (identity), [H r2b] (states) |
| CID71 (0x47) + HACKSHIP 0x48, HACKENEMY 0x49, HACKBULLET 0x4A, HACKLOCK 0x4B, HACKWALL 0x4C | tu_17F090 | the **hacking minigame** (Bentley's computer hacking): 0x47 is its damageable PO base (hit points +0x180/+0x184, `TakeDamage` slot 75, `Die` slot 82); ship / enemies / bullets / lock / walls | [C:parse whole_game] (identities), [H r4b] (fields) |
| TURRET (0x45; grapple turret in the level data) | turret.cpp 0x2259D8 | pan/tilt limits (±1.5 / ±0.7), zoom, fire, camera policy at priority 9 on activation; Sly 1 turret.cpp roles | [H r8 turret.md] |
| ROBOTTANK (0x4D, single instance `robot_tank`; was "RC car") + ammo pickup (0x6C) | tu_1D1370 | armed four-wheeled vehicle: player-driven or AI (tactics, seeks pickups, nav paths), custom wheel contacts, weapons, health meter 0x34F | [H r5b rog.md] |
| TARGET (0x12), HND (0x13), CID 0x15 | target.cpp 0x21D278, hnd.cpp | Sly 1 target.cpp one-for-one (g_dlTarget D_003E13B4); CID 0x15 camera-related list D_003E1474 | [H r8 target.md] |
| CID 0x27 | tu_1FF3B8 | XFM child with GRABSPOT-like comment; Init, PostLoad, OnRemove/OnAdd (list D_003E13D8), Clone (`CloneCid39` etc. were hex/decimal mix-ups, CONFLICTS #11) | [C:verified] (slots), [H r7] |
| CID96 (0x60) | tu_1B0B78 | mini pigeon/rat critters: SO walking a curve that flees the player; `SetCid96SpinRatio` 0x1B17B8 (was `SetStepSpinRatio`) | [H r5b small_tus_r5b.md] |
| CID98 (0x62) | tu_1BAB50 | **breakable props** (chairs, barrels, statues, crates, coin containers): up to 32 stages x 64 recorded actions run on touch/zap (232 in the hub); breaking sprays prizes from proxy pools; Murray can carry one (grabspot CID36) | [C:runtime S10-S11, S41-S43], [C:parse whole_game] |
| CID99 (0x63) | tu_1C2D20 | pickpocket dispenser / pouch (SO child of the guard) | [C:runtime] |
| CID100-102 (0x64-0x66) | tu_223F70 | gadget objects: 0x64 tossables (tommy, distract, music box), **0x65 Bentley's trigger bomb / mines** (states -1 pooled, 0 held, 1 flight, 2 stuck, 3 detonate, 5 health extractor; `SetCid100State` 0x224EC8), 0x66 guard trap (drops a prize, event 0x18) | [C:runtime runtime5a] (0x65), [C:parse whole_game] |
| CID116 (0x74) | tu_15DC80 | screen tint / fill (render kinds 14 and 1; created by PostSwLoad) | [C:static rendering] |
| CID121 (0x79) + DANCER (0x44) | rhythm 0x1520C8 | button-timing minigame: manager (timeline of 6 prompts against music time, hands-off, HUD track, scoring) and a STEP "performer" | [H r2 rhythm.md] |
| CID122 (0x7A) | tu_153B08 | LoadFromBrx only (string16 + FILEREC records) | [C:static level_parser] |
| SAFEHOUSE (0x46) | tu_1D7238 | character-select PO; see [player](player.md#8-look-mode-recon-mode-and-the-ai-partner) | [C:runtime S48] |
| SKY (0x61) | sky 0x1EF850 | sky dome (follows the camera in x/y; render kind 1) | [C:parse], [C:runtime] |
| KEY (0x5C), MEDICINE (0x5D) | prize | pickups (see [game-progression](game-progression.md#4-collectibles-and-loot-prizecpp-0x1c4e50)) | [C:runtime] |
| ACCMISS (0x51) | missile | Bentley's crossbow dart (fired via launcher CID112 0x70 = `aim_manager`) | [C:runtime runtime5a] |

## Source files

{{TUS}}

## Key functions

{{KEYFUNCS:max=60 minc=0.75}}

## Open questions
- In-game identity of most placeholder classes (CID 0x47 family, 0x60, 0x62, 0x64-0x66, 0x74, 0x7A). Experiment: break each class's Init at load and look at the level.
- MISSILE vtable length 78 vs 79 (CONFLICTS #4); identity of CID 0x52 (TARMISS/SPLMISS?).
