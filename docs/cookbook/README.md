# Cookbook

Practical recipes for changing the game: what to touch, in what order, what breaks, and how to check the result. Each
recipe builds on a systems guide ([../README.md](../README.md#systems-guides)) and names the types, functions and
registration points involved; read the guide's sections it links first.

| Recipe | You want to |
|---|---|
| [new-object-class.md](new-object-class.md) | add a new object class: registry, descriptor, virtual slots, level loading, script options |
| [guard-behaviour.md](guard-behaviour.md) | add a guard behaviour (a new BHV class on the behaviour stack) or a new guard mode |
| [splice-command.md](splice-command.md) | add or change a Splice builtin, an object option (property or method) or an event |
| [hud-widget.md](hud-widget.md) | add a HUD element, a dynamic text, a screen or a pause-menu option |
| [sound-cue.md](sound-cue.md) | play a new sound effect, music change or dialogue line from code or script |
| [player-movement.md](player-movement.md) | change how Sly, Bentley or Murray move: speeds, jumps, gravity, states |
| [level-event-task.md](level-event-task.md) | add a level script event, a mission step (TASK) or job logic, or a scripted sequence |
| [debugging.md](debugging.md) | track down a crash, a hang or a wrong behaviour |

Every recipe has the same parts: goal, systems involved, what to touch, steps, gotchas, how to test.

## Two kinds of change

The repository is a **matching decomp**: every function compiles to the same bytes as the original executable
(`tools/progress.py`, `tools/verify.sh`), and the data tables (vtables, class descriptors, option records, the builtin
table) are defined in source and compared against the ELF too. That shapes every recipe:

- **Changes that keep the match**: renames (`tools/rename.py`, `tools/fieldrename.py`), comments, header types,
  restructuring that the compiler sees as the same code. These are what the repository accepts on its main line; see
  [guides/naming-workflow.md](../guides/naming-workflow.md) and [decisions.md](../decisions.md).
- **Changes to behaviour** (every recipe here): any edit to game code or to its tables changes the bytes. Once you make
  one, `tools/matchtool.py check` reports mismatches for the edited functions, `tools/progress.py` stops reporting
  100%, and the result is no longer the original game. Do this work on a separate branch (or a separate build), never
  on the matching line.

### How a mod is built

The repository does not yet have a build that turns modified sources into a runnable game. The options, in the order
they become available:

1. **Small in-place patches.** `tools/link.py` links the whole ELF from the matched objects, but it places every
   object at the original's addresses ([guides/full-link.md](../guides/full-link.md)), so it is a layout check, not a
   relocating linker. A modified object links only if its sections still fit where the original's were; code that
   grows overlaps the next object (inferred from how link.py places sections). Useful for same-size experiments
   (changing a constant, flipping a branch); not for new classes or functions.
2. **A non-matching relocating link (future).** full-link.md's "End state" describes the final linker script: objects
   on the command line in their original order with the original section rules, and address placement kept only as a
   check. With that script and the address checks off, modified objects can grow and new objects can be added; the
   output is a new ELF that still reads the original disc for its data (inferred: nothing in the code depends on
   absolute code addresses except the blobs listed in full-link.md).
3. **The port.** [guides/port-plan.md](../guides/port-plan.md) section 5.4 (native overrides of game functions by
   name, each with a differential test) and section 7.1 (a tree of mod sources compiled into the port build). This is
   the intended long-term home of mods.

Whatever the route, a mod also needs **data**: new objects in a level, new sounds, new script code. The world files
are compressed, obfuscated binary records ([formats/](../README.md#formats)); the formats are documented byte by byte,
but no tool in the repository writes them yet. Recipes that need new level data say so.

### Testing without a mod build

Several recipes can be tested before any mod build exists:

- **Read the code path and its callers** with the guide; most "where does X happen" questions end there.
- **Live memory edits in PCSX2 over PINE** (the `[C:live]` evidence in [engine.md](../engine.md#1-how-knowledge-is-marked)):
  write a field or a save variable in the running game and watch the reaction. Parameters stored in objects (speeds,
  ranges, timers) can be tuned this way without rebuilding anything. The PINE tooling lives outside this repository
  (port-plan.md section 1.1, K15).
- **Save states** for reproducing a situation: the runtime evidence in the guides (`[C:runtime]`) comes from RAM dumps
  of save states.

## Conventions in the recipes

- Names are the code's current names. Struct field names are still being renamed; where a recipe needs a field it
  names the type and the accessor or function that uses it, so you can find the field in the header.
- `(inferred)` marks a statement that is not confirmed by the code or the running game.
- "Slot N" is a C++ virtual slot as numbered in [systems/object-model.md](../systems/object-model.md#3-layout-and-the-virtual-interface).
