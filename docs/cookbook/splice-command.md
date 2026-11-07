# Recipe: add or change a Splice builtin, object option or event

**Goal.** Give level scripts something new to call (a builtin such as `current-time`, or an object option such as a
button's TRIGGER method), change what an existing one does, or let native code call into scripts with a new event.

**Systems.** [splice-scripting.md](../systems/splice-scripting.md) (sections 4-7 and 11 first),
[formats/splice-scripts.md](../formats/splice-scripts.md) (how compiled scripts refer to builtins, options and
events), [object-model.md](../systems/object-model.md) section 5 (the three ways to talk to an object). Mod build:
[README.md](README.md#how-a-mod-is-built).

## How scripts reach native code

| Mechanism | How a compiled script names it | Table | Native signature |
|---|---|---|---|
| builtin | a builtin index, stored in the script (value tag 25) | the builtin table at the end of `app/bif.cpp` (`BIF` rows `{function, minimum argument count, accepts rest}`) | `CRef Builtin<Name>(int cargs, CRef *args, CFrame *frame)` |
| object option | an option id, the second argument of `geto` / `seto!` / `addo` / `ensureo` | `g_aeopid` records and the per-class option arrays in `app/class_registry_options.inc` | a getter/setter member function, a byte offset, or a method thunk `CRef thunk(BASIC *self, int cargs, CRef *args)` |
| event (native to script) | a symbol id the script binds with `define` in the object's frame | the event parameter-type tables in `class_registry_options.inc` | sent through slot 3, `HandleSpliceEvent` |

Compiled scripts store **indices and ids, not names**. There is no symbol lookup that turns `current-time` into
builtin N at run time; the script compiler did that. The spellings in the docs and in the research decoder come from
Sly 1's names and team names. This is the main constraint on adding anything: new script code that calls a new
builtin or option must be produced by a tool that can write the index or id into world-file data, and no such tool is
in the repository yet (the research decoder that re-encodes scripts byte-identically lives outside it; see
formats/splice-scripts.md).

## Change what an existing builtin or option does

The simplest case, and the one that needs no new data.

1. Find it. Builtins: the `Builtin<Name>` function in `app/bif.cpp` (the table at the end of the file gives the index
   order). Options: find the option id in the decoded script, the object's class, then the class's option array in
   `class_registry_options.inc` (each array is commented with its class), the record it points to in `g_aeopid`, and
   the getter, setter or thunk the record names. A thunk's `.inc` in `src/generated/splicewrap/` names the native
   function it calls.
2. Change the native function (or the builtin's body). Keep the argument count and types: `EvalApply` checks the
   minimum argument count from the table before calling, and the thunk converts arguments with the `OTYP` list it
   passes to `RefSetArgsFromSplice`.
3. Remember that options are **polymorphic**: the same id can bind to different records on different classes, and
   the same record can be listed by many classes (the record comment says `+n`). Changing a record changes it for every
   class that lists it.

## Add a builtin

1. Write `CRef Builtin<Name>(int cargs, CRef *args, CFrame *frame)` in `app/bif.cpp` (matched builtins spell the
   hidden return pointer explicitly, `CRef *Builtin...(CRef *ret, ...)`; a new one can use the plain C++ form). Read
   arguments from `args[0..cargs-1]` by tag; with "accepts rest" set, the last argument is a list of the extra values.
2. **Append** a row to the builtin table. Never reorder or insert: every compiled script in every world file refers to
   builtins by index. The table ends with a block of stubs whose bodies were compiled out (`bif2.cpp`, `BuiltinStub*`);
   no retail script calls them (formats/splice-scripts.md), so a mod may also reuse one of those indices instead of
   appending.
3. Emit the index from your script tool (value tag 25 with the new index).

## Add an object option

Use this to expose a property or a method of a class to scripts and to level data.

1. **Native side.** For a property, a getter and/or setter member function (or a plain field read/written by byte
   offset). For a method, a thunk modelled on an existing one: convert the arguments with `RefSetArgsFromSplice`
   (one `OTYP` per argument), call the native function, return a `CRef` (a method with no result returns the empty value
   `RefThunkBombDETONATE` builds). Generated thunks are `.inc` files included by `app/class_registry.cpp` between
   `SLY2_SPLICEWRAP_BEGIN` / `SLY2_SPLICEWRAP_END`; their generator (`tools/gen_splicewrap.py`) depends on helpers
   no longer in the repository, so write new ones by hand, either as an `.inc` following that pattern or as a
   plain function in your own file.
2. **The record.** Add an `EOPID` row to `g_aeopid` (`include/sly2/eopid.h` describes the fields; the `EOPID_*`
   macros in the `.inc` build each word): `otyp` the value type, `grfeopid` the flags, then the get, set,
   file/argument-count and sub-object words. Only some flag bits are understood (method, read directly, written
   directly, through the sub-object accessor); **copy the flags of an existing record of the same kind** (a
   script-and-file property, a script-only method, a file-only setter) rather than composing them.
3. **The arrays.** Add `{option id, &record}` to the option array of the class **and of every class derived from it**,
   each array kept sorted by option id (`FindOptionRecord` binary-searches it), and update the count in
   `g_mpcidarysplopt` for each array you grew. Choose an id unused in that subtree and below the threshold in
   `GetBasicOption` (larger ids are sidebag variables and never reach a record).
4. **Level files** apply the same records when an object loads (`LoadOptionsFromBrx` -> `LoadOptionFromBrx`, using the
   record's file word). Object references are queued and resolved at post-load, so a setter that takes an object is
   called after every object exists.

## Add an event (native code calls a script)

1. Pick the event symbol. Symbols 0-28 are the events the parameter-type tables cover (formats/splice-scripts.md
   lists their known meanings). A **new** event id needs the parameter-type tables (`HandleBasicSpliceEvent` indexes
   them by event id) extended to cover it; reusing an id with no known sender avoids that, but check the decoded
   scripts first that no level binds it.
2. Send it: call the object's slot 3, `HandleSpliceEvent(event, post, ...)`, as `SendTriggerEvent` does for event 5.
   With the post flag set, the call is scheduled on the world and runs in the next `ProcessSwCallbacks`; without it, the
   handler runs inside your native call.
3. Scripts handle it with `(define <event> (lambda (self args...) ...))` in the object's frame. Objects without a
   binding ignore the event (`HandleBasicSpliceEvent` returns when the frame has no binding).

## Gotchas

- **The match.** The builtin table, `g_aeopid`, the option arrays and the thunks are original data and code (the
  crt0 block and `bif.cpp`); all additions are mod-only.
- **Indices are data format.** Builtin indices, option ids and event symbols are baked into every world file. Append;
  never renumber.
- **Synchronous handlers.** An unposted event runs the script inside your native code. The handler may add or remove
  objects, fire relays or request a warp; post the event if your caller is in the middle of iterating world lists.
- **Shared frames.** All objects with the same script index share one frame, so a handler's `set!` of a top-level
  variable is seen by every such object. Per-object state belongs in the sidebag (custom properties) or in options.
- **Truthiness.** `if` takes the THEN branch only when the test's payload word is 1 (splice-scripting.md section 4);
  return real booleans from new builtins and option getters.
- **Raw pointers.** Holding a raw `CPair *` or `CFrame *` across anything that can allocate is unsafe (the collector
  may run); use `CRef` / `CRefPtr`.

## How to test

- Changes to an existing builtin or option: build the mod and visit a level whose scripts use it (the decoded scripts
  tell you which); compare with the original in PCSX2.
- New builtins, options and events need a script that uses them, so they need a world-file writer; until then, call
  the native side directly from a debug hook to test it, and test the dispatch by calling `GetBasicOption` /
  `SetBasicOption` with the new id from native code.
- After adding an option, check that `FindOptionRecord` finds it on the class and on each derived class (an unsorted
  array or a missing derived class makes the lookup fail silently).
