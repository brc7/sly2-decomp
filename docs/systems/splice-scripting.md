# Splice scripting

Splice is Sucker Punch's Scheme-like level-scripting language, carried over from Sly 1. Level designers wrote mission
logic, puzzles, ambushes and cutscene triggers in it; the world file carries the compiled scripts, and objects expose
numbered **options** (properties and methods) that scripts read, write and call. This guide covers the virtual
machine (values, frames, evaluator, garbage collector), how scripts are loaded and bound to objects, how events reach
scripts, how game objects expose options, and the builtin library. The serialized form on disc is in
[formats/splice-scripts.md](../formats/splice-scripts.md).

## 1. The pieces

- **The VM** (`src/game/splice/`): values (`CRef`, `ref.cpp`), cons cells (`CPair`, `pair.cpp`), variable bindings
  (`CVarb`, `varb.cpp`), environment frames (`CFrame`, `frame.cpp`), closures (`CProc`, `proc.cpp`), object sidebags
  (`CSidebag`, `sidebag.cpp`), the evaluator (`eval.cpp`), the deserializer (`serialize.cpp`), the garbage collector
  (`gc.cpp`) and VM startup/shutdown (`spliceutils.cpp`).
- **The builtins** (`src/game/app/bif.cpp`, stubs in `bif2.cpp`): the table of native functions scripts call by index.
- **The object bridge** (`src/game/object/`): `spliceobj.cpp` (options as Splice values, argument conversion),
  `splicemap.cpp` (the world's script tables and frames), `basic.cpp` (event dispatch, scheduled callbacks, sidebag
  helpers).
- **Binding thunks and option tables** (`src/game/app/class_registry.cpp`, `class_registry_options.inc`, and the
  generated wrappers in `src/generated/splicewrap/`): the glue from an option id to a native function or field.

## 2. Values

A `CRef` (`include/sly2/cref.h`) is a tag plus one payload word. Tags: integer, float, boolean, symbol id, builtin
index, vector, matrix, CLQ (quadratic curve), LM (range), SMP (smoothing parameters), pair (list), procedure, object
(`BASIC *`), method, void, and nil. Small values live in the word; vectors, matrices, CLQs, LMs and SMPs are boxed in
per-type value heaps; pairs, procedures, frames, bindings, sidebags and methods are garbage-collected nodes. `CRef`'s
constructor, assignment and destructor maintain reference counts on the payload (`Incref` / `Decref`), so C++ code
that holds a `CRef` on the stack is safe across allocations.

Tag numbers (`TAGK`; Sly 2 inserted one special form, so they are shifted from Sly 1) `[C:verified]`: 0x15 integer,
0x16 float, 0x17 boolean, 0x18 symbol id, 0x19 builtin, 0x1A vector, 0x1B matrix, 0x1C CLQ, 0x1D LM, 0x1E SMP, 0x1F pair,
0x20 procedure, 0x21 object, 0x22 method, 0x24 void, -1 nil; 0x14 marks the rest-parameter dot. Special forms are 1-18
(1 `set!`, 2 `define`, 5 `assert`, 6 the unknown Sly 2 form, 7 `if` ... 18 `quote`).

Objects appear in scripts as object values: there are no object literals in level data; scripts find objects at run
time by oid (`find-object` and friends) and pass them around.

## 3. Frames, sidebags and the script tables

- **`CFrame`** (`include/sly2/cframe.h`): a list of bindings (`CVarb`: symbol id -> value) and up to four parent frames
  searched after it (`CFrame::PrefFindBinding`, `FFindBinding`, `RefSetBinding`, `CFrame_RefAddBinding`). Lookup is
  depth-first through the parents with a visited flag against cycles.
- **The world's root frame** is created by `InitSwSpliceFrame` when a level body is loaded and released by
  `DeleteSwSplice` when the world is deleted, which also shuts the VM down (`ShutdownSplice`); `StartupSplice` runs at
  the start of `LoadSwFromBrx`.
- **Script tables.** Each world entry carries a table of serialized entries. `LoadSwSpliceFromBrx` reads it;
  `LoadSpliceEntryFromBrx` builds each entry as a new frame under the root frame and evaluates the entry's expression
  once in it. In practice every entry is a `(begin ...)` of top-level `define`s, so evaluating it fills the frame with
  functions and variables, including event handlers.
- **Binding objects to frames.** Each object record carries a Splice index; `PloNew` attaches the frame of that table
  entry (`GetSpliceFrame`) to the object's `pframe`. The index is local to the world entry being loaded, and **all
  objects with the same index share one frame**, so a `define` executed at run time in a handler is visible to every
  object of that entry.
- **Sidebags** (`CSidebag`, `BASIC::psidebag`): per-object extra bindings, used for script-defined custom properties.
  `EnsureLoSidebag` creates one; the `EnsureLoSidebag*` helpers in `basic.cpp` set typed defaults only when no value
  exists yet.
- **Proxies** add the frames of their init records as extra parents of a copy's frames and remove them when the copy is
  released (`CFrame_AddParent` / `CFrame_RemoveParent`, see [object-model.md](object-model.md)).

## 4. The evaluator

`RefEval(expr, frame)` (`splice/eval.cpp`) is Sly 1's evaluator: atoms evaluate to themselves, symbols are looked up in
the frame chain (unbound symbols give nil), and lists dispatch on their head:

- special forms: `set!`, `define`, `assert`, an extra Sly 2 form (keyword unknown; evaluates its first operand),
  `if`, `or`, `and`, `cond` (with `else`), `case`, `let`, `while`, `lambda`, `begin`, `import` (returns nil),
  `quote`;
- anything else is a call: `RefEvalApply` evaluates the operator and operands and calls a **builtin** (by index into
  the builtin table, checking the minimum argument count and whether extra arguments are allowed), a **method** (a
  native object method, section 5), or a **lambda** (a new frame binding the parameters, with a rest list when the
  lambda has a dotted parameter).

Quirks that matter when reading or writing scripts (all from the code):

- `if` takes the THEN branch only when the test's payload word equals 1, whatever its tag (`#t` is 1, so booleans
  behave; an integer 2 goes to ELSE). The doc comment on `RefEvalIf` says "not #f", which is not what the code does.
- `and`, `or`, `cond` and `not` treat only the boolean false as false.
- `while` evaluates only its first body expression per iteration and returns void (`RefEvalWhile`; its doc comment
  says "the last body value"). Wrap a multi-expression body in `begin`.
- `=` (`RefOpIntEqual`) compares raw payload words, so an integer and a float with the same value are not equal.
- `+` and `-` accumulate in floating point.

## 5. How game objects expose script options

Scripts never touch C++ fields directly. Each class publishes a sorted list of **options**, numbered properties or
methods, and four builtins operate on them: `addo`, `ensureo`, `seto!` and `geto` (all through `RefOpObjectOption` in
`bif.cpp`). A method is called by fetching it and applying it: `((geto obj OPT) args...)`.

**The records.** `g_aeopid` (`app/class_registry_options.inc`, type `EOPID` in `include/sly2/eopid.h`) holds one record
per option binding:

- `otyp`: the value type (bool, float, vector, matrix, CLQ, LM, SMP, the integer kinds, object reference, method);
- `grfeopid`: flags saying how each word is used: a method (a Splice thunk), read directly from a byte offset, written
  directly to a byte offset, reached through a sub-object accessor (`pmfObj`), and which words apply when loading from
  a level file versus at script time;
- three words for get, set and load-from-file, each a byte offset, a pointer to member function (with a typed variant
  per `otyp`) or a thunk; and the sub-object accessor.

**Per-class tables.** For each CID, an array of `{option id, record}` sorted by option id, containing the class's own
options merged with those of all its base classes, indexed through `g_mpcidarysplopt`. `FindOptionRecord(object, id)`
(`object/spliceobj.cpp`) binary-searches the object's class array. Option ids are **polymorphic**: the same id can bind
to different records on different classes (a "trigger" or "state" option means something class-specific), and they do
not line up with Sly 1's ids.

**Access.** `RefGetOption` / `RefSetOption` / `RefAddOption` / `RefEnsureOption` take an object and an option id. Ids up
to a fixed threshold go through the class's record (`RefGetPeopid` / `RefSetPeopid`, which convert between the C type
and a `CRef` by `otyp`); larger ids are custom properties stored in the object's sidebag. Getting a method option
returns a fresh method value (`CMethod`: the object, the thunk, the argument count); applying it calls the thunk.

**Level files use the same records.** An object's option list in the world file is applied by `LoadOptionsFromBrx` ->
`LoadOptionFromBrx` while the object loads, with the record's file-time flags. Two differences from script-time
access: object references (stored as oids) are queued and resolved at post-load, and the "remove" method option is
applied as a deferred removal. The camera and the world itself also take options from the file this way.

**Thunks.** A method record points to a thunk `CRef thunk(BASIC *self, int cargs, CRef *args)` that converts the
arguments with `RefSetArgsFromSplice` (one `OTYP` per argument) and calls the native function, for example
`RefThunkBombPRIME` calling the bomb's prime function. Most thunks are generated wrappers in
`src/generated/splicewrap/` (one `.inc` per thunk, named `RefThunk<Class><SCRIPT_NAME>`, included by
`app/class_registry.cpp`); the rest are hand-written in `class_registry.cpp`. The script-side names in the wrapper file
names (`DETONATE`, `SEEK`, `CUSTOM_INT_PROPERTY`, ...) are the best available names for the options.

## 6. Events: how the engine calls scripts

An **event** is a small symbol id; a script handles it by binding that symbol in the object's frame to a procedure
taking the object first: `(define <event> (lambda (self args...) ...))`. Handlers commonly call the previous binding
first to chain behaviour (`find-binding-or-noop`).

- `HandleBasicSpliceEvent` (`object/basic.cpp`, vtable slot 3) checks whether the object's frame binds the event; if so
  it builds the call `(event self args...)`, with argument types taken from a per-event parameter table, and either
  evaluates it now in the object's frame or, when posted, schedules it on the world for the current time (it then runs
  in the next `ProcessSwCallbacks`). LO overrides slot 3 (`HandleLoSpliceEvent`) to also broadcast trigger and untrigger
  as game events.
- Events with known meanings: 0 post-load (sent by `PostLoLoad`), 1 update (sent by `UpdateLo` when the object's next
  Splice update time has come), 4 animation/relay label callbacks, 5 trigger and 6 untrigger (`SendTriggerEvent` /
  `SendUntriggerEvent`, sent by buttons, exits, warps, volumes), 17 game event (jobs), 19 / 20 activate / deactivate
  (tasks and jobs, inferred). The rest are listed as open in [formats/splice-scripts.md](../formats/splice-scripts.md).
- **Update scheduling.** `PostLoLoad` gives an object that binds event 1 per-frame updates; the builtin
  `defer-object-update` sets the object's next Splice update time, so a script throttles its own update handler.
- **Scheduled callbacks.** `schedule-callback` builds a call and schedules it at a time on the world
  (`ScheduleSwSpliceCallback`), returning a handle for `cancel-callback` (`CancelSwSpliceCallback`). Due callbacks run
  from `ProcessSwCallbacks` in the world update.

Messages (`SendLoMessage`) are a separate, purely native mechanism; see [object-model.md](object-model.md).

## 7. The builtin library

The builtin table in `app/bif.cpp` holds `{function, minimum argument count, accepts rest arguments}` per index; every
builtin has the signature `CRef RefOpX(int cargs, CRef *args, CFrame *frame)`. Groups, in table order:

- the Sly 1 core: arithmetic, comparisons, type predicates, list operations (`cons`, `car`, `map`, `filter`,
  `for-each`, ...), `eval`, constructors for vectors, matrices, CLQ, LM and SMP, math, vector and matrix algebra, CLQ
  and LM helpers, rounding;
- world queries: current time, current level index, episode, world, current job, current task, pending task,
  difficulty;
- scheduling: `schedule-callback`, `cancel-callback`, `defer-object-update`;
- options: `addo`, `ensureo`, `seto!`, `geto`;
- finding objects: by oid, by oid list, nearest, the world, the sound manager, the camera, by class, in a box or
  sphere, hit tests, coordinate conversion between objects;
- sound and rumble, save variables (`RefOpGetSaveVar` / `RefOpSetSaveVar`, keyed by a list of save-schema symbols, see
  [progression-and-saves.md](progression-and-saves.md)), bitwise and/or, the hint box, power-up flags;
- a block of stubs at the end (`bif2.cpp`) whose bodies were compiled out of the release build; they return nil.

Script spellings come from Sly 1's builtin names where the slot is shared; Sly 2-only builtins have team names.

## 8. Memory: reference counts and the collector

Collected node kinds (pairs, bindings, frames, procedures, sidebags, methods) each live in a slot pool and derive from
`CGcObj<T>` (`include/sly2/gc.h`). Two mechanisms free them:

- **Reference counts**: counted pointers (`CRefPtr<T>`) and `CRef` release a node when its count drops to zero.
- **Mark and sweep** (`CGc::Collect`, global `g_gc`): flips the mark bit, marks from the roots (frames made roots,
  object sidebags, live values) and frees unmarked nodes, which reclaims cycles (closures that capture their own
  frame, for example). A collection runs when a pool has no free slot at allocation time (`CGcObj<T>::PtNew`), unless
  a world flag forbids it and the pool may still grow by another block.

`CGc_UpdateRecyclable`, called at the end of every world update, resets each pool's reuse cursor. Everything the VM
owns is discarded at world teardown (`ShutdownSplice`), so no Splice value survives a level change; persistent script
state goes through save variables.

## 9. How it connects

- **Object model**: frames and sidebags live in `BASIC`; slot 3 is the event entry; `PloNew` binds frames; options are
  per class. [object-model.md](object-model.md).
- **World**: the root frame, the per-entry tables and the scheduled-callback list are world state, rebuilt per level.
  [world-and-levels.md](world-and-levels.md).
- **Progression**: jobs and tasks are configured by script calls at load (visibility records, completion events) and
  driven by script handlers for events 17, 19 and 20; scripts read and write save variables.
  [progression-and-saves.md](progression-and-saves.md).
- **Cutscenes**: RELAY command lists are built from Splice through builtins and options (`gameplay/cutscene/relay.cpp`).

## 10. Invariants and gotchas

- A script handler runs synchronously inside whatever native code sent the event, unless the event was posted. Native
  code that sends an event must tolerate the handler adding or removing objects, firing relays or requesting a warp.
- Frame sharing: handlers of all objects with the same script index share variables. Per-object state belongs in the
  sidebag (custom properties) or in options.
- Option ids are per class. A script that calls option N on the wrong class reaches an unrelated record or none.
- Holding a raw `CPair *` or `CFrame *` across a call that can allocate is unsafe unless something holds a reference;
  use `CRefPtr` or `CRef`.
- The generator for the wrapper files (`tools/gen_splicewrap.py`) depends on matching-era helpers no longer in the
  repository; new thunks are written by hand following an existing `.inc`.
- Matching constraint: the option tables, builtin table and thunks are data and code of the original crt0 block.
  Adding entries is only possible in a non-matching build.

## 11. Recipes

**Find what a script option does.** Find the option id in the decoded script, look up the object's class, open
`class_registry_options.inc`, find the class's option array (commented with the class name), follow the id to its
`g_aeopid` record and read its get/set/thunk; a thunk's `.inc` names the native function it calls.

**Expose a new property or method to scripts (port or mod build).**

1. Write the native accessor or method on the class.
2. For a method, write a thunk that converts its arguments with `RefSetArgsFromSplice`, modelled on an existing
   wrapper.
3. Add an `EOPID` record to `g_aeopid` with the right `otyp` and flags, and add `{id, record}` to the option arrays of
   the class **and of every class derived from it**, keeping each array sorted by id. Pick an id unused in that
   subtree; ids above the sidebag threshold will never reach a record.

**Call a script handler from native code.** Call the object's slot 3 with the event id (`plo->HandleSpliceEvent(...)`,
or `SendTriggerEvent` for event 5); pass the post flag to defer the call to the next callback pass.

**Read or write a save variable from native code.** Use `RefGetSaveVarV` / `RefSetSaveVarV` with the variable's path
(see [progression-and-saves.md](progression-and-saves.md)); scripts use the save-variable builtins with the same
paths.

## 12. Where to look

- `src/game/splice/eval.cpp`: `RefEval`, the special forms, `RefEvalApply`.
- `src/game/splice/frame.cpp`, `sidebag.cpp`, `ref.cpp`, `gc.cpp`, `spliceutils.cpp`: the VM.
- `src/game/object/spliceobj.cpp`: `FindOptionRecord`, `RefGetPeopid`, `RefSetPeopid`, `RefGetOption` and friends,
  argument conversion.
- `src/game/object/splicemap.cpp`: script tables and frames. `src/game/object/basic.cpp`: event dispatch and scheduled
  callbacks.
- `src/game/object/brx.cpp`: `LoadOptionsFromBrx`, `LoadOptionFromBrx`.
- `src/game/app/bif.cpp`, `bif2.cpp`: builtins. `src/game/app/class_registry_options.inc`: option records and per-class
  arrays. `src/generated/splicewrap/`: thunks.
- `include/sly2/cref.h`, `cframe.h`, `eopid.h`, `gc.h`.
- [formats/splice-scripts.md](../formats/splice-scripts.md): serialized form, symbol and event names, option usage.
