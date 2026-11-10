# Documentation map

Start here. Each page links onward; this list is the full set.

## Start here
- [SETUP.md](../SETUP.md) — build the repo and check the match (Linux); [guides/setup.md](guides/setup.md) for macOS
  and Docker.
- [architecture.md](architecture.md) — how `src/` is organised: layers, subsystems, one file per original source file.
- [engine.md](engine.md) — engine overview: boot, frame loop, level loading, object model, and a short section per
  subsystem that points to its systems guide.

## Systems guides
One per subsystem, for programmers fixing bugs or adding features. Each names the classes and functions involved and
ends with how to extend the system.
- [object-model.md](systems/object-model.md) — the `BASIC` class tree, class registration, virtual slots, object
  lifecycle, proxies and clones.
- [world-and-levels.md](systems/world-and-levels.md) — `SW`, the frame loop and per-frame update order, level load,
  activation, teardown and level transitions.
- [splice-scripting.md](systems/splice-scripting.md) — the Splice VM, how scripts bind to objects, events, option
  tables, builtins, garbage collection.
- [physics-and-collision.md](systems/physics-and-collision.md) — `SO` physics, broad phase, contacts, the solver,
  trigger volumes, paths, navigation, damage.
- [animation.md](systems/animation.md) — transform slots, actions, clips and blends, state machines, IK, ropes,
  constraints.
- [rendering.md](systems/rendering.md) — render lists, models, shaders and textures, the VU1/GS backend, lighting,
  particles, render targets.
- [camera.md](systems/camera.md) — the priority stack of camera policies and the view, frustum and fog it produces.
- [sound-and-music.md](systems/sound-and-music.md) — effects, ambience, music, dialogue and subtitles; channels and
  the USB-headset microphone.
- [player.md](systems/player.md) — Sly, Bentley and Murray: class chain, state machines, input, movement components,
  gadgets, the AI partner.
- [ai-guards.md](systems/ai-guards.md) — guards: behaviour stack, perception, combat, patrols, spawners.
- [ui-and-hud.md](systems/ui-and-hud.md) — HUD widgets, menu screens, text and fonts, full-screen presentation.
- [progression-and-saves.md](systems/progression-and-saves.md) — missions and steps, the game event bus, save
  variables, memory card, dynamic difficulty.
- [cutscenes-and-sequences.md](systems/cutscenes-and-sequences.md) — RELAY command lists and the JSG scripts that run
  them, movies, the wipe, frame-capture and letterbox transitions.
- [bosses-and-minigames.md](systems/bosses-and-minigames.md) — how the boss fights and the minigames (hacking, turret,
  RC combat, dance, safehouse) reuse guards, scripts, relays and controller hand-off.

## Cookbook
Practical, step-by-step recipes for changing the game, each naming the systems, types, functions and registration
points to touch, the matching-build constraints and how to test: [cookbook/README.md](cookbook/README.md) (how mods are
built and tested), [new object class](cookbook/new-object-class.md), [guard behaviour](cookbook/guard-behaviour.md),
[Splice builtins, options and events](cookbook/splice-command.md), [HUD widgets and screens](cookbook/hud-widget.md),
[sound and music cues](cookbook/sound-cue.md), [player movement](cookbook/player-movement.md),
[level events, tasks and sequences](cookbook/level-event-task.md), [debugging](cookbook/debugging.md).

## Formats
On-disc data, for tools and the port: [disc records](formats/disc-records.md),
[LZSS compression](formats/compression-lzss.md), [world file](formats/world-file.md) and its
[object records](formats/level-objects.md), [textures](formats/textures.md), [meshes and scene](formats/meshes-scene.md),
[audio](formats/audio.md), [Splice scripts](formats/splice-scripts.md), [save variables](formats/save-variables.md).

## Contributor guides
- [decisions.md](decisions.md) — the active design decisions and compromises the matching code depends on. Read before
  restructuring source or headers.
- [guides/naming-workflow.md](guides/naming-workflow.md) — naming rules, readable-code conventions, rename tools.
- [MATCHING_NOTES.md](MATCHING_NOTES.md) — compiler facts and matching techniques.
- [guides/full-link.md](guides/full-link.md) — linking the whole ELF from source and what it still takes from the
  original.
- [guides/port-plan.md](guides/port-plan.md) — the plan for a native port.
- [guides/cleanup-playbook.md](guides/cleanup-playbook.md) — the end-to-end cleanup method (names, TU merging, view
  unification, sweeps, verification, integration, agent orchestration) with the lessons from this decomp, for the next
  one; the agent checklist is [skills/decomp-cleanup/SKILL.md](../skills/decomp-cleanup/SKILL.md) and the scripts are
  in tools/cleanup/.
- [guides/tu-merging.md](guides/tu-merging.md) — editing a translation unit without breaking the match: mismatch
  causes, vtable emission, unifying private views with the shared headers, islands, linkonce copies, data layout.
- [upstream/ONBOARDING.md](upstream/ONBOARDING.md) — for contributors coming from upstream main: what this tree
  changed and how to port per-function work and verify it; [upstream/NAME_CONFLICTS.md](upstream/NAME_CONFLICTS.md)
  compares upstream readable's original names with ours, with the crosswalk
  [upstream/name_crosswalk.csv](upstream/name_crosswalk.csv) and three reconciliation strategies.
- [archive/view-unification-history.md](archive/view-unification-history.md) — the per-round records of vtable
  emission and view unification (what each round changed and left), kept for reference.
- [archive/tu-merge-history.md](archive/tu-merge-history.md) — the per-wave notes of the translation-unit merge, kept
  for reference.
