# The object model

Every thing the game simulates (a guard, a coin, a light, a camera volume, a mission step) is a C++ object of a class
rooted at `BASIC`. This guide covers the class hierarchy, how classes are registered and described, what the virtual
slots mean, how an object is created, loaded, activated, updated and removed, and how proxies and clones multiply
objects. The world that owns all objects is in [world-and-levels.md](world-and-levels.md); scripting hooks are in
[splice-scripting.md](splice-scripting.md).

Background: Sly 2 descends from Sly Cooper 1, whose symbols shipped. Sly 1 built classes from C function-pointer tables;
Sly 2 uses real gcc 2.95 C++ classes with virtual functions, so most of the work of naming a subclass's Init, Load or
Update is reading its vtable. Sly 1 names (LO, SO, ALO, ...) are a source of names, not of layouts.

## 1. The hierarchy

The top of the tree, by class name (the full tree with sizes lives in the headers named below):

```
BASIC                    script context + message subscribers; the vptr follows its data words
 ├─ SW                   the world (one instance, g_sw) -- not an LO
 ├─ CM                   the camera (one instance, g_cm) -- not an LO
 ├─ ASEGA, SMA, POCTL, BHV family, screens, sound objects, ...   helpers that live outside the level hierarchy
 └─ LO                   level object: oid, parent, hierarchy root, activation, Splice update time
     ├─ XFM              LO + a transform (local and world matrix); lights, proxies, shapes, volumes, warps...
     ├─ ASEG, SM, WIDGET, JOB, TASK, RELAY, ...   LO children with no transform of their own
     └─ SO               solid object: XF array, collision parts, physics, model
         └─ PO           playable object (controller, health)
             └─ STEP     walking character (behaviour stack)
                 ├─ STEPPLAYER -> JT (Sly), MURRAY, BENTLEY
                 └─ STEPGUARD  -> the guard classes
```

**There is no ALO in Sly 2.** Sly 1's ALO ("actor level object": an LO with a transform and children) was split: the
transform part became `XFM`, the child list moved to `SO`. Some code views still say `ALO` (local `ALO` structs, e.g.
inside `lo.cpp`); they operate on any `LO` and the name is historical. Functions Sly 1 named for ALO now say `Lo`
(`ConvertPosBetweenLos`, `ConvertMat4BetweenLos`).

**Where the classes are declared.** `include/sly2/basic.h` (BASIC, CLASSINFO, lists, value types), `lo.h` (LO, LOROOT,
LORUN, PXR), `xfm.h` (XFM, PROXY and other light classes), `so.h`, `po.h`, `step.h`, `stepplayer.h`, `jt.h`,
`players.h`, `guard.h`, `task.h` (JOB, TASK), `sw.h` (SW), `cm.h`. `classes.h` declares every remaining registered
class, each with its real vtable written as member declarations whose asm labels name the function in each slot. Those
headers are generated from the ELF and checked by compile-time size and offset asserts, so they are the authority on
layout.

## 2. Class registry and descriptors

Each class has a numeric class id (`CID`) and a descriptor:

- `CLASSINFO` (`basic.h`): parent descriptor (`parentClass`), `cid`, `lastDescendantCid` (end of the class's subtree), instance size `instanceSize`. The
  registry's table extends it with the factory (`CLASSDESC` in `app/class_registry.cpp`: `CLASSINFO` + `pfnNew`). The
  table is indexed by CID.
- **Factories** `New<Class>` (one per constructible class) allocate the instance with `new` (which takes zeroed memory
  from the world heap) and run the inline constructor, which stores the vtable. Abstract and embedded classes have no
  factory.
- **Descriptor getters** `Get<Class>ClassDesc` return the class's table entry; they sit in vtable slot 6
  (`GetClassInfo`) of every class, so `plo->GetClassInfo()->cid` is the runtime type.
- Factories, getters, the Splice binding thunks and the option tables all belong to one translation unit, the crt0
  block, which is why they live together in `app/class_registry.cpp` (with `class_registry_options.inc` and the
  generated wrappers in `src/generated/splicewrap/`).

**Type tests.** CIDs are numbered in pre-order of the class tree, so a subtree occupies a contiguous CID range.
`IsBasicDerivedFrom(object, cid)` (`object/basic.cpp`) is a range test against the base's `lastDescendantCid`;
`IsCidDerivedFrom(cid, cidBase)` (`object/find.cpp`) walks the parent chain instead. `GetBasicCid` returns an object's
CID. Level files name classes by CID, so CIDs are part of the data format.

## 3. Layout and the virtual interface

**Layout.** `BASIC` holds three data words, then the vptr: a counted pointer to the object's Splice frame
(`CRefPtr<CFrame> pframe`), a counted pointer to its sidebag of extra script variables (`CRefPtr<CSidebag>
psidebag`), and the head of its subscriber list. gcc 2.95 places the vptr after the base's data, so every
BASIC-derived object has it at the same place, after those three words. Helpers that are not BASIC-derived (heaps,
streams, camera policies, particles) put their vptr elsewhere.

**Slot numbering.** A gcc 2.95 vtable entry is `{short delta; short pad; function}` and entry 0 is a null header, so
"slot N" means entry N. The code's comments, the `LO_SLOT_*` enum in `lo.cpp` and the hand-made calls in m2c views
(`object->vt[N]`) all use this numbering; the headers mark it on each declaration (`/* slot N */`), and the asm label
on a declaration names the function that fills the slot.

**BASIC's slots**: 1 and 2 empty defaults (`AddRef` / `Release` in basic.h; role unknown); 3 `HandleSpliceEvent`;
4 `SendMessage`; 5 `HandleMessage`; 6 `GetClassInfo`.

**LO's slots** (they keep their meaning in every LO subclass; a subclass appends its own after the last LO slot):

- 3 `HandleLoSpliceEvent` (overrides BASIC: the BASIC handling, then trigger/untrigger are also reported as game events);
- 7 `Init` (`InitLo`); 8 `Add` / 9 `Remove` (`AddLo` / `RemoveLo`: into / out of the world);
- 10 `SetParent` (`LO_SetParent`); 11 / 12 `AddHierarchy` / `RemoveHierarchy` (set / clear the in-world bit and
  notify subscribers); 13 `UpdateLoroot`;
- 14 `LoadFromBrx` (`LoadLoFromBrx`); 15 post-load (`PostLoLoad`); 16 per-frame update (`UpdateLo`); 17 inherits a flag
  from the parent (`InheritLoFlag4`);
- 19 `RenderSelf` (empty for LO); 20 render-all;
- **22 / 23 `OnLoRemove` / `OnLoAdd`**: the object stops / starts being live (lo.h declares them as `OnRemove` /
  `OnAdd`; Sly 1 called the pair Freeze / Unfreeze);
- 25 `SetIdProxy`; 26 reset-for-proxy (run when a spare proxy copy is placed); 28 child list (`LO_GetChildren`);
- 29 / 30 / 31 `TranslateToPos` / `RotateToMat` / world matrix; 33-36 velocity and angular velocity get/set;
- 40 / 41 `CloneHierarchy` / `Clone`; 42 set busy distance; 46 `SetVisMask`; 47 / 48 set / clear flags.

`SO` overrides most of these (it has children, a model and physics) and adds its own block; `PO`, `STEP`,
`STEPPLAYER` and `STEPGUARD` each append more. Their headers list the slots with the functions that fill them.

## 4. Lifecycle of an object

### 4.1 Creation: `CreateLo`

`CreateLo(cid, psoParent, oid, isplice)` (`object/brx.cpp`) is the general factory, used by the level loader and by
run-time spawning alike:

1. calls the class's factory through the descriptor table;
2. sets the parent and oid, and attaches the Splice frame of script-table entry `isplice` (`GetSpliceFrame`, see
   [splice-scripting.md](splice-scripting.md));
3. links the object into the world's oid hash and its class list;
4. calls slot 7 `Init`;
5. if the in-world bit is set, calls slot 11 `AddHierarchy`; if the object is now live (`IsLoInWorld`), slot 23
   `OnAdd`;
6. records it in the world's "new objects" array while a level is loading (the post-load pass walks that array).

`InitLo` (every class's `Init` calls its base `Init` first) sets the defaults: busy kind "never", an effectively
infinite busy distance, all visibility bits, the default flag word; it inherits the hierarchy root (`LOROOT`) and the
in-world bit from the parent, or allocates its own root when it has no parent; it inherits the proxy record pointer;
and it links the object into the parent's child list. New objects are therefore born "added", not removed.

`CloneLoWithParent(plo, psoParent)` makes a new object of the same class and oid with `CreateLo` and then calls slot 40
`CloneHierarchy`, which copies the body and, for an SO, clones the children.

### 4.2 Load: `LoadFromBrx` and options

The level loader (`LoadSwObjectsFromBrx`) reads each object record and calls slot 14 `LoadFromBrx` with the stream.
The rule every override must follow: **consume exactly the record's payload size**. Overrides call the base class's
load first; `LoadLoFromBrx` reads the object's option list through `LoadOptionsFromBrx`, which applies each value
through the option table (section 5 of [splice-scripting.md](splice-scripting.md)). Option values that name other
objects by oid cannot be resolved yet (the referenced object may not exist): `LoadOptionFromBrx` queues them on the
object's deferred-callback list (`LORUN`) as `PostLoadOidField` / `PostLoadOidSetter`. Record format:
[formats/level-objects.md](../formats/level-objects.md).

### 4.3 Post-load

After the whole level is read, `PostSwLoad` (`world/scene/sw.cpp`) runs slot 15 on every object recorded during the
load, repeating until no new objects appear (post-load handlers may create objects). `PostLoLoad`:

1. runs the object's deferred callbacks in the order they were queued (`RunLoDeferredCallbacks`), which resolves the
   oid references now that every object exists (`FindSwObject` relative to the object);
2. sends Splice event 0 (post-load) to itself through slot 3;
3. if the object's frame binds a handler for event 1 (update), switches busy kind "never" to "by distance" and sets
   the flag that asks for per-frame updates; otherwise it parks the next-Splice-update time.

Classes resolve tables of oid fields with `BindLoOidTable` (parts by id, SO sub-objects, or world objects by oid).

### 4.4 Activation: in the world, live, busy

Three notions, easy to confuse:

- **Added / removed.** `AddLo` / `RemoveLo` (slots 8/9) move the object between the two parts of its parent's child
  list (in-world part first, removed tail after) and set or clear the removed bit, recursing over children. A removed
  object still exists and keeps its state.
- **In the hierarchy.** Slots 11/12 set or clear the in-world bit and send message 0 (added) / 1 (removed) to
  subscribers. The bit is inherited down the tree.
- **Live.** `IsLoInWorld(plo)` is true only when the in-world bit is set **and** the object's `LOROOT` is in the
  world's active (busy) root list. Slot 23 `OnAdd` fires exactly when this becomes true, slot 22 `OnRemove` when it
  becomes false; they always alternate.

What makes a root active is the **busy list** (`object/busy.cpp`). Each root has a busy kind (in the LO flag word):
always, never, within its busy distance of the camera, or within its busy distance of the current player, gated by the
object's visibility mask against the camera's. `UpdateSwBusyList` re-tests a few roots per frame, skipping roots that
are far from their zone edge until the camera or player has moved enough, and calls slot 23 / slot 22 on the roots
that crossed. `SetLoBusyKind` and `SetLoBusyDistance` change the settings and re-sort the root so it is tested soon.

Base `OnLoAdd` puts the object in the world's **update table** and `OnLoRemove` takes it out. Subclasses add their
own registration: SO joins the render and physics structures; targets, volumes, guards, rails, pipes, lights and
others join per-class lists kept in `SW`. Guards, pickups and other pooled objects are toggled this way rather than
created and destroyed.

### 4.5 Per-frame update

The world update (see [world-and-levels.md](world-and-levels.md)) walks the update table and calls slot 16 `Update`
on entries whose flag word asks for updates; while the game clock is stopped only entries flagged to update while
paused are called. Being live is necessary but not sufficient: the "wants update" flag must be set (by
`PostLoLoad` for scripted objects, by class code otherwise (inferred)). `UpdateLo` itself only checks the object's
next Splice update time and, when it has come, sends Splice event 1 (update). Subclasses override `Update`, call the
base, and do their own work. The builtin `defer-object-update` moves the next Splice update time forward.

### 4.6 Render

`RenderSw` calls slot 19 `RenderSelf` on live objects; `RenderLoSelf` is empty and `SO` draws its model. Details
belong to the rendering system (`engine/graphics/`).

### 4.7 Removal and teardown

There is no per-object destruction during play. `RemoveLo` takes an object out of the world (and fires `OnRemove` if
it was live); `DeferLoRemove` queues the removal as a deferred callback (level files use this for their "remove"
option, so removals happen after the load finishes). Objects themselves are never freed individually: the world heap
is a bump allocator released as a whole when the level is deleted (`DeleteSw` -> `FreeSw`). Anything that must
survive a level change lives in the save blob ([progression-and-saves.md](progression-and-saves.md)).

## 5. Hierarchy, lookup and communication

**Hierarchy.** `parent` points to the parent SO (null for a top-level object); children live in the parent's child
list (top-level objects in the world's child list), linked through `siblingLink`. `LOROOT` is shared by a whole
hierarchy: it names the topmost ancestor and links the hierarchy into the world's root and busy lists. Reparenting goes
through slot 10 `SetParent` (`LO_SetParent`, wrapped by `SetLoParent`), which keeps the in-world state consistent,
updates the root record (slot 13) and fires the activation callbacks as needed.

**Lookup.** The world keeps an oid hash and one list per CID (`SW::objectsByOid`, `SW::objectsByClass`; hash and bucket helpers in
`object/find.cpp`).

- `FindSwObject(world, grf, oid, ploContext)` returns the first match; `FindSwObjects` collects several. The low
  byte of `grf` picks the scope relative to the context object (scope 4 = the matches nearest the context in the
  hierarchy; the level loader's oid fixups use it); higher bits restrict to live objects, lift the result clamp, or
  ignore proxy scoping.
- `FindSwObjectsByClass` / the "first of class" variant walk the class lists (derived classes included).
- Objects inside a proxy copy are found only from the same copy unless the search says otherwise, so two copies of a
  template that refer to "oid N" each find their own N.
- Objects spawned at run time without an oid (particle groups, for example) are on class lists but not in the oid hash.

**Three ways to talk to an object** (do not mix them up):

- **Messages**: `SendLoMessage` (slot 4) delivers a numeric `MSGID` with a pointer to the object's own slot 5
  `HandleMessage` and then to every subscriber (`SubscribeLoObject` / `UnsubscribeLoObject`; subscriptions are
  counted). Used for added/removed notifications and many engine signals.
- **Splice events**: slot 3 `HandleSpliceEvent(event, fPost, ...)` runs or schedules the script handler bound for that
  event in the object's frame. Event 5 is trigger and 6 untrigger (`SendTriggerEvent` / `SendUntriggerEvent`); buttons,
  exits, warps and volumes send them. See [splice-scripting.md](splice-scripting.md).
- **Game events**: `BroadcastGameEvent` reaches the game record, difficulty, tasks and the current job, not arbitrary
  objects. See [progression-and-saves.md](progression-and-saves.md).

## 6. Proxies and clones

Levels are built from **world entries**, each a block of objects in the world file
([formats/world-file.md](../formats/world-file.md)). Two mechanisms multiply them:

**Copies of a world entry.** `LoadSwFromBrx` loads an entry's object list once and then calls `CloneLoWithParent` on every
loaded object as many times as the entry asks for copies. Depending on a per-entry flag, the copies stay in the world,
or all of them are removed right after loading and serve only as a pool for proxies (the in-memory field name says
which is which only by inference).

**Proxies** (`PROXY`, an `XFM` subclass; `object/proxy.cpp`):

- A **proxy source** (`PROXYSRC`, in `SW::aproxysrc`) is a world entry used as a template; each of its copies has a
  **proxy id** `MakeProxyId(isrc, icopy)` = source index in the high half, copy index in the low half (`GetSourceIndexFromProxyId`,
  `GetCopyIndexFromProxyId`).
- `LoadProxyFromBrx` reads a proxy record from an object list. In one mode it immediately takes the source's next copy
  (`AddProxyCopy`), places it at the record's transform, applies the record's `PROXYINIT` overrides (class, Splice
  frame, option bytes) and loads a nested object list under it. In the other mode it creates a `PROXY` object that
  stands at the placement and acquires copies at run time.
- After the load, `InitSwProxySpares` turns every unused copy into a **spare** (`PROXYSPARE`) and records its objects'
  rest poses.
- At run time `AcquireProxySpare` rates the spares (free, locked, live...), takes the best from its previous owner,
  moves its objects to the proxy, re-applies the init records (slot 26 on each object) and adds them to the world;
  `ReleaseProxySpare` undoes all of it (removes the objects, restores the rest pose, detaches the proxy's frames).
  `AcquireProxyObject` returns the single object of a one-object source; `LockProxyObject` /
  `UnlockProxyObject` pin a copy. Thrown objects, coin sprays and respawning enemies come from here.
- Each object that came from a proxy keeps a proxy record (`PXR`, `LO::proxyRecord`): a node in a tree of proxy ids rooted at
  `SW::ppxrRoot`, which is what scoped lookup matches against. `SetIdProxy` (slot 25) moves an object (and optionally
  its children) to a new place in that tree.

## 7. How it connects

- **World** (`world/scene/sw.cpp`): owns every list the objects link into (oid hash, class lists, roots, busy list,
  update table, per-class lists, slot heaps for `LOROOT`s and callbacks). [world-and-levels.md](world-and-levels.md).
- **Level loading** (`object/brx.cpp`, `level/brx_sw.cpp`): creates objects with `CreateLo` and calls `LoadFromBrx`.
- **Splice**: every object may have a frame and a sidebag; options expose fields and methods to scripts; slot 3 runs
  event handlers. [splice-scripting.md](splice-scripting.md).
- **Engine services**: SO's slot overrides connect objects to physics, rendering and animation; XF transforms are
  maintained by `engine/anim/xf.cpp` and `object/xform.cpp`.

## 8. Invariants and gotchas

- `LoadFromBrx` must consume exactly the record's payload, or every later object in the list is misread. Skipped
  objects are skipped by that size.
- Never test "is this object active" with the in-world bit alone; use `IsLoInWorld`, which also checks the root's busy
  state. `OnAdd`/`OnRemove` are the only reliable activation edges.
- Activation can change in the middle of the update pass (an `Update` may add or remove other objects); the update
  table tolerates holes and compacts itself when adding, so do not cache entry indices.
- `IsBasicDerivedFrom` relies on pre-order CID numbering. A class whose CID lies outside its parent's range (for
  example one appended after all existing CIDs) fails that test even though `IsCidDerivedFrom` succeeds.
- `isRemoved` and "not live" are different states: a live object's parent can still be removed later, and a removed
  object keeps its frame, sidebag and fields for when it is added again.
- Option-based oid references are resolved only at post-load; reading such a pointer field in `LoadFromBrx` or `Init`
  gives null.
- `SW` and `CM` are BASIC children, not LOs: no oid, no hierarchy, no `IsLoInWorld`; `SW` has its own slots and
  `UpdateSw` is called directly by the main loop.
- Matching constraint: vtables and descriptor tables are data that must match the original byte for byte. New
  classes, slots or descriptor entries are possible only in a non-matching (port or mod) build.

## 9. Recipes

**Find a class's Init/Load/Update.** Look up the class in `include/sly2/` (or `classes.h`); the asm labels of its
virtual declarations name the functions in slots 7, 14, 15, 16, 22 and 23. Search `src/game/` for those symbols.

**Spawn an object at run time.** Prefer a proxy: give the level a `PROXY` whose source is the template, then call
`AcquireProxySpare` (or the one-object helper) and release it with `ReleaseProxySpare`. `CreateLo` also works, but the
object's memory is never reclaimed before the next level load, so do not create objects per frame.

**Add a new object class (port or mod build).**

1. Declare it in a header under `include/sly2/` deriving from the closest base (`LO` if it needs no transform, `XFM`
   for a positioned helper, `SO` for something physical), overriding the slots you need (`Init`, `LoadFromBrx`,
   `Update`, `OnAdd`/`OnRemove`...). Call the base implementation first in each override.
2. Give it a factory and a descriptor getter and add a descriptor entry, as `app/class_registry.cpp` does for the
   existing classes. Mind the CID range rule above if code will test derivation with `IsBasicDerivedFrom`.
3. If levels should place it, the level data must carry records with its CID, and its `LoadFromBrx` must read exactly
   what the record holds.
4. If scripts should see its properties, add option records (see [splice-scripting.md](splice-scripting.md)).

**React to an object entering or leaving the active set.** Override slots 23/22, call the base, then register or
unregister in your structure. Do not do this in `Init` or `Add`: an object can be added but not live.

## 10. Where to look

- `src/game/object/basic.cpp`: BASIC, type tests, sidebag option helpers, Splice event dispatch, messages, scheduled
  Splice callbacks.
- `src/game/object/lo.cpp`: LO virtuals, roots, child lists, deferred callbacks, visibility and busy setters,
  coordinate conversions.
- `src/game/object/xform.cpp`: XFM.
- `src/game/object/brx.cpp`: `CreateLo`, `CloneLoWithParent`, `LoadSwObjectsFromBrx`, option loading, load collectors.
- `src/game/object/busy.cpp`: the busy list. `src/game/object/find.cpp`: lookup by oid and class.
- `src/game/object/proxy.cpp`: proxies, spares and proxy ids.
- `src/game/app/class_registry.cpp`: factories, descriptor getters, descriptor table.
- `include/sly2/basic.h`, `lo.h`, `xfm.h`, `so.h`, `classes.h`: layouts and vtables.
- [engine.md](../engine.md) section 6 for the summary and section 8 for the Sly 1 comparison.
