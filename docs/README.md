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
- [guides/tu-merging.md](guides/tu-merging.md) — history of the translation-unit merge (kept for reference; current
  rules are in decisions.md).
