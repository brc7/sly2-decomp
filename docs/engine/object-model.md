# Object model: BASIC, LO, XFM, classes, vtables, activation, proxies, lookup

How every game object is described, created, linked, activated and found. Tags: see [README](../README.md#how-knowledge-is-marked).
Related: [class tree](../reference/class-tree.md), [world-sw](world-sw.md) (who creates objects), [splice-scripting](splice-scripting.md) (options and events).

## 1. Class descriptors and vtables

- **Descriptor table** D_002BAAE8: 199 records of 0x14 bytes `CLASSINFO {CLASSINFO *pclassinfoSuper; CID cid; CID cidMax; int cb; LO *(*pfnNew)();}` `[C:runtime]`.
  CIDs are numbered in pre-order of the class tree, so `cidMax` (end of the subtree's range) turns "is X derived from
  Y" into a range test: `FIsBasicDerivedFrom` 0x132B10 `[C:verified]`. 73 classes have no factory (abstract or embedded) `[C:static]`.
- **Factories** 0x110AB0-0x1139D8 (126 functions): allocate `cb` bytes from the world heap, store the vtable at +0xC,
  zero the first two words `[C:static]`. **Descriptor getters** 0x113A10 + 0x10*cid (199 three-instruction functions)
  are what vtable slot 6 `Pclassinfo()` points to `[C:runtime]`.
- **vtable entries are 8 bytes** `{s16 delta; s16 pad; pfn}`; a virtual call is `lh 8N(vt); lw 8N+4(vt); jalr; this+=delta`.
  `cid_table.csv`'s `vtable_len` counts 8-byte slots (CONFLICTS #6 is REFUTED) `[C:runtime]`.
- **The vptr sits at +0xC** of every BASIC-derived object (after BASIC's three data words) `[C:runtime]` (8,162/8,162 objects).
  Non-BASIC helper classes (HEAP, CBinaryInputStream, RIP particles, camera policies) put it elsewhere.

### BASIC (0x10 bytes)
| Off | Field | Tag |
|---|---|---|
| +0x0 | `CRefPtr<CFrame> pframe`: the object's Splice frame (script bindings) | [C:static layouts.md] |
| +0x4 | `CRefPtr<CSidebag> psidebag`: extra script options | [C:static] |
| +0x8 | subscriber list for SendMessage (refcounted nodes from pool D_003E1144) | [H r2b basic.md] |
| +0xC | vptr | [C:runtime] |

BASIC virtual slots: 1/2 Incref/Decref-style hooks (empty in BASIC itself; refcounted classes such as SMA, ASEGA,
SND, RIP use them), 3 `HandleSpliceEvent`, 4 `SendMessage`, 5 `HandleMessage`, 6 `Pclassinfo` `[H r2b basic.md]`, slot 6 `[C:runtime]`.

### LO (0x50 bytes), the level object
| Off | Field | Tag |
|---|---|---|
| +0x10 | `SO *psoParent` | [C:runtime] |
| +0x14 | `LOROOT *ploroot` {plo = topmost ancestor, DLEs}; pooled at g_sw+0x398C, shared by a hierarchy | [C:runtime] |
| +0x18 | `OID oid` (32-bit) | [C:runtime] |
| +0x1C / +0x20 | `sleOid` (next in oid hash bucket) / `sleCid` (next in class list) | [C:runtime] |
| +0x24 | `dleChild` (link in the parent's child DL) | [C:runtime] |
| +0x28 | 64-bit flag word: +0x2C cpaloFindSwObjects, +0x2D `grfvis`, bits 48-51 nibble (init 0xF), 52-55 `busyk` (init 2), 56 removed, 58 no-vis-inherit, 60 in-world | [C:runtime] for 48/52/60 |
| +0x30 | tSplice (next Splice update time) | [H r4 lo.md] |
| +0x34 | deferred-callback list (LORUN) | [H r4 lo.md] |
| +0x38 | `sBusy` activation radius (default 1e10; levels set 3000-6000) | [C:runtime] |
| +0x3C | `ppxr` proxy root (default D_003E139C) | [C:runtime] |
| +0x40 | tagged word: odd = flags (init 0x21; bit 2 "wants Update"); even = pointer to its SW update-table entry `{grf, plo}` | [C:runtime] |

## 2. The LO virtual interface

Slot numbers are 8-byte slot indices; LO's table is at 0x2D3540 (48 slots). The same indices hold in every LO
subclass (XFM 50, SO 75, PO 81, STEP 87, STEPPLAYER 105, JT 107, STEPGUARD 89) `[C:static]`.

| Slot | Method | LO base | Tag |
|---|---|---|---|
| 3 | HandleSpliceEvent(symid, fPost, cargs, ppv, pref) | 0x1331F8 (LO override 0x1A61E0 also notifies g_game for 5/6) | [H r2b basic.md] |
| 4 | SendMessage(msg, arg) | 0x133508 | [H r2b basic.md] |
| 5 | HandleMessage | 0x133590 (empty) | [C:static] |
| 6 | Pclassinfo | getter | [C:runtime] |
| 7 | Init | `InitLo` 0x1A5270 | [C:verified] |
| 8 / 9 | Add / Remove (into / out of the world) | `AddLo` 0x1A5478 / `RemoveLo` 0x1A5630 | [C:verified arbitration] |
| 10 | SetParent(SO*, XF*, grf) | 0x1A5900 | [C:verified arbitration] |
| 11 / 12 | AddHierarchy / RemoveHierarchy: set/clear bit 60, send message "added"/"removed" | 0x1A5800 / 0x1A5848 | [C:verified arbitration] |
| 13 | UpdateLoroot | 0x1A5B80 | [H r4 lo.md] |
| 14 | LoadFromBrx(CBinaryInputStream*) | `LoadLoFromBrx` 0x1A5C28 (options loop) | [C:verified] |
| 15 | PostLoad | `PostLoLoad` 0x1A5C48 | [C:verified] |
| 16 | Update (dt read from g_clock) | `UpdateLo` 0x1A5E28 (fires the Splice update event) | [C:verified] |
| 17 | (Sly 1: ProjectTransform) | 0x1A5E80 | LO body = `LO_vt17_InheritFlag4`: copies bit 2 (0x4) of the parent's +0x40 flag word into its own; no matrix math `[C:static v3.1]`. "ProjectTransform" is only a Sly 1 analogy for the slot |
| 19 | RenderSelf(CM*) | 0x1A5ED8 (empty) | [C:static] |
| **22** | **OnRemove**: object stops being live | `OnLoRemove` 0x1A6150 | [C:verified arbitration] |
| **23** | **OnAdd**: object becomes live | `OnLoAdd` 0x1A6178 | [C:verified arbitration] |
| 25 | SetIdProxy | proxy TU | [H r5 proxy.md] |
| 28 | CploChildren(out array) | 0x1A5BE8 (none) | [H r4 lo.md] |
| 29 / 30 | TranslateToPos / RotateToMat | 0x1A6110 / 0x1A6118 | [C:verified] |
| 31 | PmatWorld (plain LO: static identity) | 0x1A62D8 | [C:verified] |
| 33-36 | Get/Set Velocity / AngularVelocity | 0x1A6120.. | [C:verified] (34) |
| 40 | CloneHierarchy | `CloneLoHierarchy` 0x1A6AD0 | [C:runtime] |
| 41 | Clone(plo base): copy the class body past +0x50 | `CloneLo` 0x1A6730 | [C:runtime] |
| 46 / 47 | SetVisMask / SetGrf | 0x1A6DC0 / 0x1A66C8 | [H r4 lo.md] |
| 48+ | class-specific (SO adds 48-74, PO 75-81, ...) | | |

REFUTED: `layouts.md` put Clone at slot 40; it is 41 (40 = CloneHierarchy) `[C:runtime]`. REFUTED: slot 3 = SendMessage (r4) — slot 3 is HandleSpliceEvent, 4 is SendMessage.
Several slots (17, 20, 21, 32, 37-39, 42-45) have shape-only names; see the function table.

## 3. Creation, hierarchy and removal

- `PloNew(cid, psoParent, oid, isplice)` 0x13E3C8: `D_002BAAE8[cid].pfnNew()`, set parent/oid/Splice frame, link into
  the SW oid hash and class list, vt7 Init, vt11 AddHierarchy if the parent is in the world, vt23 OnAdd if now live,
  record in the load list (SW+0x39E4) `[C:static]` (no `psw` argument: SW is the singleton).
- `PloCloneLo` 0x13E510: `PloNew(same cid/parent/oid)` + vt40 `[C:verified]`.
- Hierarchy is kept in DLs: roots in g_sw+0x10, children in SO+0x64 (removed objects at the tail, `ploRemovedFirst`
  SO+0x70) `[C:runtime]`. `SetParent` brackets a reparent with OnRemove/OnAdd like Sly 1's RemoveLo+AddLo `[C:verified arbitration]`.
- Deferred removal: Splice `SplLoRemoveDeferred` -> `DeferLoRemove` 0x1A5D30 queues vt9 on the object's callback list `[H r4 lo.md]`.
- `BindLoOidTable` 0x1A64E8: a Sly 2 data-driven binder that resolves `{grf, id, ib}` tables of OID references into
  pointer fields at PostLoad (used by JT, Murray, many classes) `[H r4 lo.md]`.

## 4. Activation: OnAdd / OnRemove (slots 23 / 22)

Status: **CONFIRMED** `[C:verified]` by the slot-22/23 arbitration, which enumerated all 14 dispatch sites in the ELF
and all 39 implementations. This settles CONFLICTS #1 and #5: names Freeze/Unfreeze and OnLoRemove/OnLoAdd-as-guesses
are retired; slot 12 is RemoveHierarchy.

**Definition.** `FIsLoInWorld` 0x1A61A0 is true only when the in-world bit 60 of LO+0x28 is set **and** the object's
LOROOT is in the active-roots list `g_sw+0x2C` (roots near the player or camera). The engine calls slot 23 exactly when
`FIsLoInWorld(plo)` flips false->true and slot 22 exactly when it flips true->false. The two always alternate and
bracket the interval during which the object is *live*.

**When it flips:**
1. *Structural changes*: `AddLo` 0x1A5478, `RemoveLo` 0x1A5630, `LO::SetParent` 0x1A5900, `PloNew` 0x13E3C8 add or
   remove an object while its root is active.
2. *Proximity changes, every frame*: `UpdateSwBusyList` 0x147F40 (called from `UpdateSw`) walks the busy-candidate
   list g_sw+0x20 with an incremental cursor (SW+0x39A4). Per root, `busyk` (LO+0x28 bits 52-55) decides:
   1 = always active, 2 = never, 0 = active if `grfvis & g_cm+0x14` and the distance to the current player is within
   `sBusy` (LO+0x38), 3 = the same test against the camera position. Joining the active list -> slot 23 on the root;
   leaving -> slot 22. `UpdateLoBusy` 0x148300 makes the same decision for one root (from AddLo, SetParent, and
   ReturnJtToSafePlace, which needs the safe-place object active).
3. *Forced*: two classes force-activate after Add (0x147410, 0x1BEB28); PROXY vt24 0x1CB6D8 force-deactivates.
Only the root receives the proximity call; the SO override passes it to its in-world children.

**What the hooks do.** Base `OnLoAdd` inserts `{grf, plo}` into the SW update table (array at SW+0x3F20, count
SW+0x3F14, compacted when >10% dead; LO+0x40 becomes the entry pointer); `OnLoRemove` clears the entry and restores
the flags. `UpdateSw` calls vt16 and `RenderSw` vt19 **only for entries in this table**. SO adds the sphere tree
(`AddLoToSphereTree` 0x1D6378) and contact-tree registration; TARGET, PIPE, RAIL, LIGHT, the GRABSPOT classes,
STEPGUARD (guard list D_003E1444) and others add themselves to a per-class global list; CID 0x2B starts/stops sounds.

**Why it matters.** Sly 1 had two mechanisms: OnLoAdd/OnLoRemove on world add/remove, and `FreezeAlo(palo, f)` on
busy-list changes. Sly 2 merged them into this pair. A Sly 2 object that is "in the world" but far from the player is
loaded, keeps its state, but gets no Update/Render and is not on gameplay lists. At runtime 1,743 of ~8,160 objects
were live in the Paris hub; guards and pickups are pooled and switched in/out as Sly moves `[C:runtime]`.

## 5. Proxies and spares (object pools)

- A world entry in the level file is a **proxy source** (PROXYSRC, SW+0x3F04 count / +0x3F08 array, 0x6C bytes): a
  set of objects loaded `ccopy` (`c8`) times `[C:static brx.md]`, 287 in the Paris hub `[C:runtime]`.
- Load-time proxies take a copy immediately (`AddProxyCopy`); PROXY objects (CID 0x6/0x7) take a **spare** on demand
  (`PspareAcquire` 0x1CAEC8) and give it back (`ReleasePspare`). `idProxy = (isrc<<16) | icopy` `[H r5 proxy.md]`.
- `PloAcquireProxy` 0x1CB250 is the generic spawn path gameplay uses (thrown objects, markers, coin sprays) `[H r5 proxy.md]`.

## 6. Finding objects

- `PloFindSwObject` 0x15E938 (84 callers) resolves OIDs; `CploFindSwObjectsByClass` enumerates by class `[H r3 find.md]` (Sly 1 find.c order).
- SW tables `[C:runtime]`: oid hash g_sw+0x38 (512 SLs, bucket `(oid*0x95675) & 0x1FF`, link LO+0x1C);
  per-class SLs at g_sw+0x3038 + **12**\*cid (`{first, last, ibNext=0x20}`; REFUTED: "8\*cid DLs" in r3 names);
  a second 512-bucket table at g_sw+0x1838 whose records are not BASIC objects (role unknown, INCONCLUSIVE).
- Runtime-spawned BLIPG/RIPG objects are on class lists but not in the oid hash `[C:runtime]`.

## 7. XFM and transform helpers

- XFM (CID 0x3, 0xE0): LO + `XF *pxfParent` +0x50, stamp +0x54, `mat` +0x60, `matWorld` +0xA0. In Sly 2 it is the base of
  44 lightweight classes that were ALO children in Sly 1 `[C:static layouts.md]`.
- `PosLoWorld` 0x1A62E8 (286 callers) / `PmatLoWorld` 0x1A6318 return vt31's world matrix (position row 3) `[C:verified]`.
- `ConvertAloPos/Vec/Mat` 0x1A6F00.. convert between object frames `[H r4 lo.md]`.
- Transform constraints (dest follows source position/rotation): `LoadXfmConstraintsFromBrx` 0x1603B8, `ApplyXfmConstraint` 0x1604E8 `[H r3 frm.md]`.

## Devtools findings (v3, 2026-11-01)

- **CLASSINFO stride 0x14** `[C:static v3 disassembly]`: the 199 getters at 0x113A10 + 0x10*cid return `g_aclassinfo + 0x14*cid` (e.g. cid 58 -> 0x2BAF70,
  cid 59 -> 0x2BAF84) ([devtools/symbols_export](../guides/devtools.md#symbols_export)).
- **g_sw and g_cm are objects in place, not pointers** `[C:runtime v3]`: the word at +0xC is vt_SW 0x2DC018 at 0x3DD490 and vt_CM 0x2CA4D0 at 0x2DDF40 in every
  state checked (S02, S65, S71-S73).
- **XFM world matrix** (types round 2 correction: matWorld at XFM+0xA0 with its translation row at **+0xD0**; holds for all 3,504 LIGHT instances `[C:runtime]`, but plain XFM/PROXY/TARGET instances have an all-zero matWorld, so do not read object positions there; **SO positions are at pxfRoot (XF) + 0x70**) `[C:runtime level_viewer]`: in Paris S02, 787 of 842 live XFM objects with a non-zero matWorld have a placed instance with
  the same oid within 2 units (LIGHT 248/248); misses are 7 VOLs on moving SOs and 21 runtime-spawned oids. collision_paths' independent check reproduces
  LIGHT 248/248, PROXY 385/385, RAIL 21/21 in S02 ([placement rules](../formats/meshes-scene.md#devtools-findings-v3-2026-11-01)).

## 8. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x132B00-0x1336F0 | basic | medium | tu_table "?" |
| 0x147E20-0x148620 | busy | low | SW busy list |
| 0x15E380-0x15EF00 | find | high | Sly1 find.c order; tu_table said font; was labelled font |
| 0x1603B8-0x1607A0 | tu_1603B8 | low | XFM transform constraints (font<x<frm); sorts font<x<frm |
| 0x1A5270-0x1A7518 | lo | high | tu_table said light; was labelled light |
| 0x1C9940-0x1CB908 | proxy | high |  |
| 0x231100-0x2319E8 | xform | high |  |

## 9. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x132B10](../../src/game/object/basic/FIsBasicDerivedFrom__FP5BASIC3CID.c) | `FIsBasicDerivedFrom` | cid range test: Pclassinfo()->cid in [cid, D_002BAAE8[cid].cidMax) | 0.92 | T1 |  |
| [0x132B78](../../src/game/object/basic/EnsureBasicSidebag__FP5BASIC.c) | `EnsureBasicSidebag` | If psidebag (+4) is null create one (0x257D80) and addref | 0.85 | T1 |  |
| [0x132BC8](../../src/game/object/basic/EnsureLoSidebagBool__FP2LO5OPTIDi.c) | `EnsureLoSidebagBool` | RefEnsureOption(plo, optid, bool CRef (tagk 0x17)) | 0.90 | T1 |  |
| [0x132C20](../../src/game/object/basic/EnsureLoSidebagInt__FP2LO5OPTIDi.c) | `EnsureLoSidebagInt` | RefEnsureOption with S32 CRef (tagk 0x15) | 0.88 | T1 |  |
| [0x132C78](../../src/game/object/basic/EnsureLoSidebagFloat__FP2LO5OPTIDf.c) | `EnsureLoSidebagFloat` | RefEnsureOption with F32 CRef (tagk 0x16) | 0.90 | T1 |  |
| [0x132CC8](../../src/game/object/basic/EnsureLoSidebagClq__FP2LO5OPTIDP3CLQ.c) | `EnsureLoSidebagClq` | RefEnsureOption with CRef(CLQ*) | 0.85 | T1 |  |
| [0x132D28](../../src/game/object/basic/EnsureLoSidebagLm__FP2LO5OPTIDP2LM.c) | `EnsureLoSidebagLm` | RefEnsureOption with CRef(LM*) | 0.85 | T1 |  |
| [0x132D88](../../src/game/object/basic/EnsureLoSidebagOid__FP2LO5OPTID3OID.c) | `EnsureLoSidebagOid` | RefEnsureOption with S32 CRef holding an OID | 0.85 | T1 |  |
| [0x132DE0](../../src/game/object/basic/EnsureLoSidebagVector.c) | `EnsureLoSidebagVector` | RefEnsureOption with a vector CRef (passed by value as qword) | 0.85 | T1 |  |
| [0x1331F8](../../src/game/object/basic/HandleBasicSpliceEvent.c) | `HandleBasicSpliceEvent` | BASIC vt3: if the frame binds symidEvent, build args from s_aotypEvtParm[s_aiEvtParm[ev]] and either schedule (fPost) or evaluate now | 0.75 | T1 | arbitration |
| [0x1333C0](../../src/game/object/basic/SubscribeLoObject.c) | `SubscribeLoObject` | Add ploTarget to the object's subscriber list (BASIC+0x8), refcounted | 0.85 | T1 | verified |
| [0x133508](../../src/game/object/basic/SendLoMessage.c) | `SendLoMessage` | BASIC vt4: own vt5 HandleMessage, then vt5 of every subscriber | 0.85 | T1 |  |
| [0x133590](../../src/game/object/basic/HandleBasicMessage.c) | `HandleBasicMessage` | BASIC vt5 default (empty); PO/others call it as base | 0.75 | T1 |  |
| [0x147F40](../../src/game/object/busy/UpdateSwBusyList.c) | `UpdateSwBusyList` | Per frame: walk objects, test their busy radius against the camera (g_cm pos) and the current player, and move them between busy/idle lists | 0.85 | T1 | runtime |
| [0x148300](../../src/game/object/busy/func_00148300.c) | `UpdateLoBusy` | Decide whether one object is busy (distance to camera/current PO vs its sBusy) and (re)insert it | 0.40 | T2 |  |
| [0x15E380](../../src/game/object/find/PdlFromSwOid.c) | `PdlFromSwOid` | Returns the SW hash bucket DL for an oid: SW+0x38 table, index (oid*0x95675)&0x1FF | 0.85 | T1 | runtime |
| [0x15E3A8](../../src/game/object/find/PdlFromSwCid.c) | `PdlFromSwCid` | Returns the per-class object DL of the SW (SW+0x3038 + 8*cid) | 0.85 | T1 | runtime-corrected, runtime |
| [0x15E3C0](../../src/game/object/find/PdlFromSwPxrOid.c) | `PdlFromSwPxrOid` | Returns a bucket of a second oid-hashed DL table in the SW (SW+0x1838, same hash) | 0.60 | T1 | verified |
| [0x15E650](../../src/game/object/find/CploFindSwObjects_15E650.c) | `CploFindSwObjects_15E650` | Finds up to cploMax LOs with an oid relative to a context object (6-arg form; forwards with extra arg 0) | 0.80 | T1 |  |
| [0x15E678](../../src/game/object/find/func_0015E678.c) | `CploFindSwObjects` | Core object search over the oid hash buckets (and second table), with an extra Sly 2 parameter between ploContext and cploMax | 0.85 | T1 | verified |
| [0x15E938](../../src/game/object/find/PloFindSwObject__FP2SWi3OIDP2LO.c) | `PloFindSwObject` | Returns the first LO matching grffso/oid relative to a context LO (or NULL) | 0.90 | T1 |  |
| [0x15E988](../../src/game/object/find/FIsCidDerivedFrom.c) | `FIsCidDerivedFrom` | Walks the class-descriptor parent chain from cid (D_002BAAE8 records) and returns 1 if it reaches cidAncestor | 0.85 | T1 | verified |
| [0x15E9C8](../../src/game/object/find/CploFindSwObjectsByClass.c) | `CploFindSwObjectsByClass` | Collects up to cploMax LOs of a class (or derived) relative to a context | 0.85 | T1 | verified |
| [0x15ED78](../../src/game/object/find/PloFindSwObjectByClass__FP2SWi3CIDP2LO.c) | `PloFindSwObjectByClass` | First LO of a class: CploFindSwObjectsByClass(grffso\|0x200, cid, ctx, 1, &plo) | 0.85 | T1 |  |
| [0x1603B8](../../src/game/engine/anim/tu_1603B8/func_001603B8.cpp) | `LoadXfmConstraintsFromBrx` | Reads an s8 count of transform-constraint records {source LO, dest LO, fPosition, fRotation} from the BRX stream, links each dest to its record and sw | 0.75 | T1 | parse |
| [0x1A5270](../../src/game/object/lo/InitLo.c) | `InitLo` | LO vt7 Init: busy fields (nibble48=0xF, busyk=2, byte40=0xFF, sBusy x38=1e10), inherit loroot+in-world bit60 from parent or EnsureLoroot, grf x40=0x21 | 0.95 | T1 | runtime, verified |
| [0x1A53E0](../../src/game/object/lo/FreeLoroot.c) | `FreeLoroot` | Unlink plo->ploroot from g_sw+0x2C list and g_sw+0x20 busy list (0x147E20), free it to pool g_sw+0x398C, clear x14 | 0.85 | T1 | verified |
| [0x1A5478](../../src/game/object/lo/AddLo.c) | `AddLo` | LO vt8 Add: if marked removed (bit56) clear it, move into in-world part of parent child DL, call vt11 AddHierarchy if parent in world, root not-busyk2 | 0.85 | T1 | verified |
| [0x1A5630](../../src/game/object/lo/RemoveLo.c) | `RemoveLo` | LO vt9 Remove: if in world vt22 + drop root from g_sw+0x2C; drop root from busy list; move to removed tail of child DL, set bit56, remove children (vt | 0.85 | T1 | verified |
| [0x1A5900](../../src/game/object/lo/func_001A5900.c) | `SetLoParent` | LO vt10: move plo between old/new parent child DLs keeping in-world state, freeze if in world, set psoParent, vt13 UpdateLoroot, recurse to children ( | 0.85 | T1 | verified |
| [0x1A5C28](../../src/game/object/lo/LoadLoFromBrx.c) | `LoadLoFromBrx` | LO vt14: LoadOptionsFromBrx loop (0x13F810) | 0.90 | T1 | parse, verified |
| [0x1A5C48](../../src/game/object/lo/PostLoLoad.c) | `PostLoLoad` | LO vt15: run deferred callbacks, vt3 message 0; if Splice frame has binding 1 -> busyk 2->0, grf x40\|=2 (wants Splice update) else tSplice(x30)=D_002A | 0.85 | T1 | verified |
| [0x1A5D58](../../src/game/object/lo/PdcbAddLo.c) | `PdcbAddLo` | Alloc a 12-byte {pfn,arg,next} node from g_sw array (+0x39D0/+0x39D4 counter) and push on plo->x34 list | 0.85 | T1 | verified |
| [0x1A5FB8](../../src/game/object/lo/func_001A5FB8.c) | `SetLoBeamXf` | Build a camera-facing matrix along segment a->b (width scaling, 0.01 epsilon, uses g_cm pos D_002DDF40+0x90) and apply via 0x1A5EE8 | 0.35 | T2 | verified |
| [0x1A6118](../../src/game/object/lo/RotateLoToMat.c) | `RotateLoToMat` | LO vt30 default no-op | 0.85 | T1 | verified |
| [0x1A6138](../../src/game/object/lo/GetLoAngularVelocity.c) | `GetLoAngularVelocity` | LO vt34: return zero vector | 0.85 | T1 | verified |
| [0x1A6148](../../src/game/object/lo/SetLoAngularVelocity.c) | `SetLoAngularVelocity` | LO vt36 default no-op | 0.85 | T1 | verified |
| [0x1A6150](../../src/game/object/lo/OnLoRemove.c) | `OnLoRemove` | LO vt22: unregister from g_sw per-object update table (0x210268: clear entry, dead++) | 0.85 | T1 | arbitration |
| [0x1A6178](../../src/game/object/lo/OnLoAdd.c) | `OnLoAdd` | LO vt23: register in g_sw update table (0x210140, compacts when >10% dead) | 0.85 | T1 | arbitration |
| [0x1A61A0](../../src/game/object/lo/func_001A61A0.c) | `FIsLoInWorld` | bit60 set and plo->ploroot is in g_sw+0x2C list | 0.70 | T1 |  |
| [0x1A61E0](../../src/game/object/lo/HandleLoSpliceEvent.c) | `HandleLoSpliceEvent` | LO vt3: BASIC Splice-event post (0x1331F8) then for msg 5/6 notify D_002DEB30 with 0x1A/0x1B | 0.85 | T1 | arbitration |
| [0x1A6290](../../src/game/object/lo/FMatchesLoName.c) | `FMatchesLoName` | True if plo->oid==oid or any proxy-alias node owned by plo has that oid | 0.85 | T1 | verified |
| [0x1A62E8](../../src/game/object/lo/func_001A62E8.c) | `PosLoWorld` | Return vt31 PmatWorld()->pos (row 3, +0x30) | 0.90 | T1 | verified |
| [0x1A6318](../../src/game/object/lo/func_001A6318.c) | `PmatLoWorld` | Return vt31 PmatWorld() | 0.90 | T1 | verified |
| [0x1A6730](../../src/game/object/lo/CloneLo.cpp) | `CloneLo` | LO vt41 Clone(ploBase): copy Splice frame/sidebag refs, scale, flag bits, tag word, x34; clone proxy nodes when g_sw phase==-2; CopyAb the class body  | 0.85 | T1 | runtime |
| [0x1A6AD0](../../src/game/object/lo/CloneLoHierarchy.c) | `CloneLoHierarchy` | LO vt40: forwards to vt41 Clone | 0.85 | T1 | runtime |
| [0x1A6BE8](../../src/game/object/lo/SetLoXfFromMat.c) | `SetLoXfFromMat` | vt29 TranslateToPos(mat.pos) then vt30 RotateToMat(mat) | 0.85 | T1 | verified |
| [0x1A6CA8](../../src/game/object/lo/SetLoBusyk.c) | `SetLoBusyk` | Set the LO busy kind (LO+0x28 bits 52-55: 0 busy within sBusy of the camera, 1 always busy, 2 never busy, 3 busy within sBusy of the hero) and re-sort | 0.80 | T1 | top50 |
| [0x1A6EE8](../../src/game/object/lo/func_001A6EE8.c) | `LO_vt43` | LO vt43 default: return 0 | 0.20 | T2 | verified |
| [0x1A6F00](../../src/game/object/lo/ConvertAloPos.c) | `ConvertAloPos` | Transform a point from paloFrom space to world (PmatWorld) then into paloTo space (inverse); vector by value | 0.80 | T1 |  |
| [0x1A6FC0](../../src/game/object/lo/ConvertAloVec.c) | `ConvertAloVec` | Same for direction vectors (rotation only, M3) | 0.80 | T1 |  |
| [0x1C9940](../../src/game/object/proxy/IdProxy__Fii.c) | `IdProxy` | (isrc<<16)\|(icopy&0xFFFF) | 0.85 | T1 |  |
| [0x1C9950](../../src/game/object/proxy/IsrcFromIdProxy__Fi.c) | `IsrcFromIdProxy` | idProxy>>16 | 0.85 | T1 |  |
| [0x1C99D8](../../src/game/object/proxy/func_001C99D8.c) | `FindPxr` | Find (or create, allocating from world heap) the PXR path of ids aid[0..c) under a parent PXR | 0.85 | T1 | verified |
| [0x1C9B08](../../src/game/object/proxy/LoadProxyFromBrx__FP2SWP2SO3CID3OIDiP18CBinaryInputStreamP14CLoadCollector.cpp) | `LoadProxyFromBrx` | read one proxy record (source id/mode/placement) and instantiate copy or PROXY object | 0.95 | T1 | parse |
| [0x1CA118](../../src/game/object/proxy/SetLoIdProxy.cpp) | `SetLoIdProxy` | LO vt25: put object into proxy copy tree under idProxy | 0.90 | T1 |  |
| [0x1CA370](../../src/game/object/proxy/func_001CA370.c) | `AddProxyCopy` | Take source isrc's next copy: SetIdProxy, Add, SetParent, apply placement (vt37) to each object; notify collector; icopyNext++ | 0.85 | T1 | verified |
| [0x1CA500](../../src/game/object/proxy/InitSwProxySpares.c) | `InitSwProxySpares` | After load: turn every unused copy of each proxy source into a spare (rest pose array aposRest, PROXYSPARE array, identity mats) | 0.85 | T1 | verified |
| [0x1CA788](../../src/game/object/proxy/InitProxy.c) | `InitProxy` | PROXY::Init (vt7): InitXfm + flag | 0.85 | T1 |  |
| [0x1CA8E0](../../src/game/object/proxy/OnProxyAdd.c) | `OnProxyAdd` | PROXY vt23: LO::Unfreeze then 0x149AB8 | 0.85 | T1 | verified, arbitration |
| [0x1CA930](../../src/game/object/proxy/IcopyFromPspare__FP5PROXYP10PROXYSPARE.c) | `IcopyFromPspare` | (pspare - aspare)/0x50 + first spare copy index | 0.85 | T1 |  |
| [0x1CAA60](../../src/game/object/proxy/ReleasePspare__FP5PROXYP10PROXYSPARE.cpp) | `ReleasePspare` | return a copy to the spare pool and restore its objects | 0.90 | T1 |  |
| [0x1CB2C0](../../src/game/object/proxy/func_001CB2C0.c) | `FCanAcquireProxy` | PspareChoose(this 2)!=0 | 0.85 | T1 | verified |
| [0x1CB328](../../src/game/object/proxy/EnsureProxyCopy.c) | `EnsureProxyCopy` | if no current copy acquire one | 0.85 | T1 | verified |
| [0x1CB770](../../src/game/object/proxy/FindProxysrc__FP2SWi.c) | `FindProxysrc` | index of proxy source with id or -1 | 0.80 | T1 |  |
| [0x231100](../../src/game/object/xform/InitXfm.c) | `InitXfm` | InitLo; pxfParent from parent+0x60; mat = identity | 0.85 | T1 | verified |
| [0x231170](../../src/game/object/xform/LoadXfmFromBrx.c) | `LoadXfmFromBrx` | 0x231768 (XF ref + matrix) then LoadOptionsFromBrx | 0.90 | T1 | parse, verified |
| [0x2311B0](../../src/game/object/xform/CloneXfm.c) | `CloneXfm` | CloneLo 0x1A6730 then invalidate world | 0.85 | T1 | verified |
| [0x231768](../../src/game/object/xform/LoadXfmXfFromBrx.c) | `LoadXfmXfFromBrx` | S16 XF id -> pxfParent via vt49, matrix read, invalidate, vis mask | 0.75 | T1 | parse |
| [0x2317E0](../../src/game/object/xform/LoadXfmChildObjectsFromBrx.c) | `LoadXfmChildObjectsFromBrx` | pushes XF context, loads child objects (0x13FB40), pops, applies mat to each (vt37) | 0.75 | T1 | parse |

Selected 70 of 166 named functions (T1 with confidence >= 0.75, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/object-model.md](../reference/functions/object-model.md).

## Open questions
- Meaning of LO slots 17, 20, 21, 26/27, 32, 37-39, 42-45 (shape-only names). Compare overriding methods across subclasses.
- What the second oid table at g_sw+0x1838 holds (1,752 non-object records at runtime).
- Real Sucker Punch names of the descriptor struct and LOROOT (team names).
