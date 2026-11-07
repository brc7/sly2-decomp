# Recipe: add a new object class

**Goal.** A new kind of level object (a pickup, a hazard, a gadget target, a helper with no model) with its own
C++ class, created by the level loader or at run time, updated every frame while live, and configurable from level data
and Splice.

**Systems.** [object-model.md](../systems/object-model.md) (sections 2-4 and 9 first),
[splice-scripting.md](../systems/splice-scripting.md) section 5 (options),
[world-and-levels.md](../systems/world-and-levels.md) (load order),
[formats/level-objects.md](../formats/level-objects.md) (object records). Mod build: [README.md](README.md#how-a-mod-is-built).

## Before you start: do you need a class?

Many behaviours need no new class. An existing class plus a Splice script covers a lot: any object whose frame binds
event 1 gets per-frame script updates (`PostLoLoad` turns them on), custom properties live in the object's sidebag
(option ids above the record threshold of `GetBasicOption`), and trigger/untrigger events come from buttons and
volumes. That route changes only level data. Add a class when you need native per-frame work, new physics or render
behaviour, or new state the scripts cannot hold.

## What to touch

| Piece | Where | What it is |
|---|---|---|
| class declaration | a header under `include/sly2/` (`classes.h` holds most registered classes) | the struct, deriving from the closest base, with its virtual overrides |
| virtual overrides | a `.cpp` under the subsystem's directory (`src/game/gameplay/objects/` for gameplay props) | `Init`, `LoadFromBrx`, `PostLoad`, `Update`, `OnAdd`/`OnRemove`, ... |
| factory | `app/class_registry.cpp`, the `New<Class>` block | `extern "C" X *New<Class>() { return new X; }` |
| descriptor getter | `app/class_registry.cpp`, the `Get<Class>ClassDesc` block | returns the class's entry in the descriptor table; fills slot 6 |
| descriptor entry | the class descriptor table (`CLASSDESC` rows at the end of `app/class_registry.cpp`) | `{parent entry, cid, end of subtree, instance size, factory}` |
| option array | `app/class_registry_options.inc` | the class's sorted `{option id, record}` list, plus its entry in `g_mpcidarysplopt` |
| per-CID world list | `SW::objectsByClass` (`include/sly2/sw.h`) | one object list per class id, used by `CreateLo` and class lookups |

The model to copy is a small existing class. `BUTTON` (`include/sly2/classes.h`, `gameplay/objects/button.cpp`) is a
good one: an `SO` subclass whose overrides each call the base first and then do the button's part.

```cpp
// gameplay/objects/button.cpp: the shape every override follows
extern "C" void InitButton(BUTTONV *button)
{
    InitSo(button);            // base first
    InitBtn(&button->btn);     // then this class's state
    ...
}
```

## Steps

1. **Pick the base.** `LO` for a logic object with no transform, `XFM` for a positioned helper with no model or
   physics, `SO` for anything with a model, collision or children, `PO`/`STEP` only for controllable characters. The
   base decides which slots exist (object-model.md section 3 lists LO's; `so.h`, `po.h`, `step.h` list the rest).

2. **Declare the class.** Derive from the base and declare only the virtuals you override; gcc 2.95 keeps an override
   in its base's slot whatever order you declare it in, and appends genuinely new virtuals after the base's last
   slot. Existing declarations carry an asm label naming the function in each slot (`__asm__("InitButton")`) and a
   size assert; a new class does not need the labels, but keep a size assert if anything else (an option record's
   byte offset, a pool) depends on the layout.

3. **Choose a class id.** Class ids are numbered in pre-order of the class tree, and `IsBasicDerivedFrom` is a range
   test against the base's end-of-subtree id (object-model.md section 2). Ids are also part of the data format: the
   level files name every object's class by id. So:
   - **Appending** a new id after the last class keeps every existing id (and every level file) valid. It is inside
     the range of a base only if that base's subtree is the last one in the table; today that is `BHV` (see
     [guard-behaviour.md](guard-behaviour.md)). An appended `LO`/`SO` subclass is outside `LO`'s range, so
     `IsBasicDerivedFrom(obj, LO's id)` is false for it, while `IsCidDerivedFrom` (which walks the parent chain) is
     true.
   - **Inserting** inside a base's range renumbers every later class, which invalidates every level file and every
     hard-coded id in the code.
   - For a mod, appending is the practical choice. Then audit the callers of `IsBasicDerivedFrom` that test the new
     class's bases (search for the base's id or its per-file `CID_*` constant), or make `IsBasicDerivedFrom` fall
     back to the parent walk in your build.
   There is no central list of class ids: `CID` is an empty enum in `basic.h`, files use their own `CID_*` enums or
   literals, and the descriptor table (each row commented with the class name) is the list.

4. **Grow everything sized by the number of classes.** Adding an id means growing, together: the descriptor table, the
   root class's (`BASIC`'s) end-of-subtree id and the end id of every base whose subtree you extend, `g_mpcidarysplopt`
   (one entry per class), and `SW::objectsByClass` (one list per class). `SW` is constructed in place at every level load and
   its later fields are found by offset in places (`GetSwClassList` computes the list address from a fixed offset in
   `object/find.cpp`), so growing `objectsByClass` moves every field after it: fix those offset-based accesses, or give the new
   classes a separate list.

5. **Factory and getter.** Add `New<Class>` (a plain `new X`: `operator new` takes zeroed memory from the world heap, and
   the inline constructor stores the vtable) and `Get<Class>ClassDesc` returning the new table row; the getter is the slot 6
   override (`GetClassInfo`). Point the row's factory at `New<Class>`.

6. **Implement the slots.** Call the base first in each, then:
   - slot 7 `Init`: defaults only. Oid references are not resolved yet, other objects may not exist.
   - slot 14 `LoadFromBrx`: read exactly the bytes your record holds after the base's part. Most classes add nothing
     here and take their configuration from options (step 7), which `LoadLoFromBrx` applies.
   - slot 15 `PostLoad`: resolve oid references (`BindLoOidTable`, or the deferred oid fields the option loader
     queued), find related objects, start state machines.
   - slot 16 `Update`: per-frame work. It runs only while the object is live **and** its flag word asks for updates;
     an object without a Splice update handler must set that itself (object-model.md section 4.5).
   - slots 23 / 22 `OnAdd` / `OnRemove`: join and leave any per-class list or engine structure (a `DL` in `SW` is
     the existing pattern, world-and-levels.md section 10).
   - slot 40 / 41 `CloneHierarchy` / `Clone`: copy your fields if the class can appear in a world entry with copies
     or as a proxy source (`CloneButton` is the model). Without it, copies get the base's fields only.
   - slot 26 (reset for proxy): restore the rest state if spare proxy copies of the class are reused.

7. **Options (optional).** To configure the class from level data or scripts, add `EOPID` records to `g_aeopid` and
   the class's option array (splice-scripting.md section 5; [splice-command.md](splice-command.md) has the
   procedure). The class's array must also contain every option of its bases, sorted by option id: copy the base's
   array and add yours.

8. **Create instances.**
   - From level data: an object record with the new class id in a world entry's object list. No tool in the repository
     writes world files yet ([README.md](README.md#how-a-mod-is-built)).
   - At run time: `CreateLo(cid, parent, oid, isplice)` from native code (`object/brx.cpp`). Do it from a point where
     the level is fully loaded, for example the game record's handler for game event 4 (post-load) in
     `HandleGameEvent` (world-and-levels.md section 10). Objects created after load are not post-loaded by
     `PostSwLoad`; call what your `PostLoad` would do yourself. Memory is never freed before the next level load, so
     never create per frame; for repeated spawning use a proxy (object-model.md section 6).

## Gotchas

- **The match.** Every piece above is code or data of the original executable (vtables, the descriptor table, the
  option tables, `SW`'s layout). None of it can change on the matching line; build this as a mod.
- **Exact record size.** A `LoadFromBrx` that reads one byte too few or too many shifts every later object in the
  list. The record header carries the byte size (formats/level-objects.md); the loader trusts your reader.
- **Live is not added.** Do engine registration in `OnAdd`/`OnRemove`, never in `Init`/`Add`: an object can be in
  the world without being live (object-model.md section 4.4).
- **Pointers across levels.** Objects die with the world heap at every level change. Anything global that points at
  your object must be cleared in teardown (`DeleteSw`) or re-found after load.
- **Proxy scoping.** Objects inside a proxy copy find each other by oid only within the same copy unless the search
  says otherwise; a class that looks up "oid N" should use the object-relative lookups (`FindSwObject` with the object
  as context).
- **Per-file views.** Many `.cpp` files declare their own partial view of a class (`BUTTONV` and the like) instead of
  including the full header. If you change a class layout, search for every view of it.

## How to test

- Build the mod and spawn one instance with `CreateLo` from the post-load hook in a level you know; log or draw from
  its `Update` to confirm it is live, then walk away and back to see `OnRemove`/`OnAdd` alternate.
- Check the type tests you rely on: `IsBasicDerivedFrom(obj, base)` for each base the rest of the code tests.
- If instances come from level data, load every level that uses them: a size mismatch in `LoadFromBrx` shows up as
  garbage or a crash in the objects after yours.
- On the matching line nothing of this can be tested; `tools/matchtool.py check` on the touched files only confirms
  what changed.
