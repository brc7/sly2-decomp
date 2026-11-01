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
| 17 | ProjectTransform | 0x1A5E80 | [H r5 SUMMARY] |
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

## 8. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x100000-0x1147D8 | crt0 + Splice thunks + class factories | no static-init boundary inside; thunks in src/splicewrap |
| 0x132B00-0x1336F0 | basic |  |
| 0x147E20-0x148620 | busy? |  |
| 0x15E380-0x15EF00 | find | was labelled font |
| 0x1603B8-0x1607A0 | tu_1603B8 (XFM constraints) | sorts font<x<frm |
| 0x1A5270-0x1A7518 | lo | was labelled light |
| 0x1C9940-0x1CB908 | proxy |  |
| 0x231100-0x2319E8 | xform |  |

## 9. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x132B10](../../src/hand/func_00132B10.c) | `FIsBasicDerivedFrom` | cid range test: Pclassinfo()->cid in [cid, D_002BAAE8[cid].cidMax) | 0.92 | HYP |
| [0x132B78](../../src/hand/func_00132B78.c) | `EnsureBasicSidebag` | If psidebag (+4) is null create one (0x257D80) and addref | 0.85 | HYP |
| [0x132BC8](../../src/hand/func_00132BC8.c) | `EnsureLoSidebagBool` | RefEnsureOption(plo, optid, bool CRef (tagk 0x17)) | 0.90 | HYP |
| [0x132C20](../../src/hand/func_00132C20.c) | `EnsureLoSidebagInt` | RefEnsureOption with S32 CRef (tagk 0x15) | 0.88 | HYP |
| [0x132C78](../../src/hand/func_00132C78.c) | `EnsureLoSidebagFloat` | RefEnsureOption with F32 CRef (tagk 0x16) | 0.90 | HYP |
| [0x132CC8](../../src/auto/func_00132CC8.c) | `EnsureLoSidebagClq` | RefEnsureOption with CRef(CLQ*) | 0.85 | HYP |
| [0x132D28](../../src/auto/func_00132D28.c) | `EnsureLoSidebagLm` | RefEnsureOption with CRef(LM*) | 0.85 | HYP |
| [0x132D88](../../src/hand/func_00132D88.c) | `EnsureLoSidebagOid` | RefEnsureOption with S32 CRef holding an OID | 0.85 | HYP |
| [0x132DE0](../../src/hand/func_00132DE0.c) | `EnsureLoSidebagVector` | RefEnsureOption with a vector CRef (passed by value as qword) | 0.85 | HYP |
| [0x1331F8](../../src/hand/func_001331F8.c) | `HandleLoSpliceEvent` | BASIC vt3: if the frame binds symidEvent, build args from s_aotypEvtParm[s_aiEvtParm[ev]] and either schedule (fPost) or evaluate now | 0.75 | HYP |
| [0x133508](../../src/hand/func_00133508.c) | `SendLoMessage` | BASIC vt4: own vt5 HandleMessage, then vt5 of every subscriber | 0.85 | HYP |
| [0x133590](../../src/auto/func_00133590.c) | `BASIC::HandleMessage` | BASIC vt5 default (empty); PO/others call it as base | 0.75 | HYP |
| [0x147F40](../../src/hand/func_00147F40.c) | `UpdateSwBusyList` | Per frame: walk objects, test their busy radius against the camera (g_cm pos) and the current player, and move them between busy/idle lists | 0.50 | VERIFIED |
| [0x148300](../../src/hand/func_00148300.c) | `UpdateLoBusy` | Decide whether one object is busy (distance to camera/current PO vs its sBusy) and (re)insert it | 0.40 | VERIFIED (arbitration/conflict fix; was UpdateLoBusy?) |
| [0x15E380](../../src/hand/func_0015E380.c) | `PdlFromSwOid` | Returns the SW hash bucket DL for an oid: SW+0x38 table, index (oid*0x95675)&0x1FF | 0.80 | HYP |
| [0x15E650](../../src/hand/func_0015E650.c) | `CploFindSwObjects` | Finds up to cploMax LOs with an oid relative to a context object (6-arg form; forwards with extra arg 0) | 0.80 | HYP |
| [0x15E678](../../src/hand/func_0015E678.c) | `CploFindSwObjects` | Core object search over the oid hash buckets (and second table), with an extra Sly 2 parameter between ploContext and cploMax | 0.65 | VERIFIED |
| [0x15E938](../../src/hand/func_0015E938.c) | `PloFindSwObject` | Returns the first LO matching grffso/oid relative to a context LO (or NULL) | 0.90 | HYP |
| [0x15E988](../../src/hand/func_0015E988.c) | `FIsCidDerivedFrom` | Walks the class-descriptor parent chain from cid (D_002BAAE8 records) and returns 1 if it reaches cidAncestor | 0.85 | VERIFIED |
| [0x15E9C8](../../src/hand/func_0015E9C8.c) | `CploFindSwObjectsByClass` | Collects up to cploMax LOs of a class (or derived) relative to a context | 0.85 | VERIFIED |
| [0x15ED78](../../src/hand/func_0015ED78.c) | `PloFindSwObjectByClass` | First LO of a class: CploFindSwObjectsByClass(grffso\|0x200, cid, ctx, 1, &plo) | 0.85 | HYP |
| [0x1A5270](../../src/hand/func_001A5270.c) | `InitLo` | LO vt7 Init: busy fields (nibble48=0xF, busyk=2, byte40=0xFF, sBusy x38=1e10), inherit loroot+in-world bit60 from parent or EnsureLoroot, grf x40=0x21 | 0.90 | VERIFIED |
| [0x1A53E0](../../src/hand/func_001A53E0.c) | `FreeLoroot` | Unlink plo->ploroot from g_sw+0x2C list and g_sw+0x20 busy list (0x147E20), free it to pool g_sw+0x398C, clear x14 | 0.60 | VERIFIED |
| [0x1A5478](../../src/hand/func_001A5478.c) | `AddLo` | LO vt8 Add: if marked removed (bit56) clear it, move into in-world part of parent child DL, call vt11 AddHierarchy if parent in world, root not-busyk2 | 0.80 | VERIFIED |
| [0x1A5630](../../src/hand/func_001A5630.c) | `RemoveLo` | LO vt9 Remove: if in world vt22 + drop root from g_sw+0x2C; drop root from busy list; move to removed tail of child DL, set bit56, remove children (vt | 0.80 | VERIFIED |
| [0x1A5900](../../src/hand/func_001A5900.c) | `LO::SetParent` | LO vt10: move plo between old/new parent child DLs keeping in-world state, freeze if in world, set psoParent, vt13 UpdateLoroot, recurse to children ( | 0.75 | VERIFIED |
| [0x1A5C28](../../src/auto/func_001A5C28.c) | `LoadLoFromBrx` | LO vt14: LoadOptionsFromBrx loop (0x13F810) | 0.80 | VERIFIED |
| [0x1A5C48](../../src/hand/func_001A5C48.c) | `PostLoLoad` | LO vt15: run deferred callbacks, vt3 message 0; if Splice frame has binding 1 -> busyk 2->0, grf x40\|=2 (wants Splice update) else tSplice(x30)=D_002A | 0.85 | VERIFIED |
| [0x1A5D30](../../src/auto/func_001A5D30.c) | `DeferLoRemove` | Queue Remove of plo to run later (pushes callback 0x1A5D08 on plo deferred list) | 0.75 | HYP |
| [0x1A5D58](../../src/auto/func_001A5D58.c) | `PdcbAddLo` | Alloc a 12-byte {pfn,arg,next} node from g_sw array (+0x39D0/+0x39D4 counter) and push on plo->x34 list | 0.45 | VERIFIED |
| [0x1A6118](../../src/auto/func_001A6118.c) | `LO::RotateToMat` | LO vt30 default no-op | 0.60 | VERIFIED |
| [0x1A6138](../../src/auto/func_001A6138.c) | `LO::GetAngularVelocity` | LO vt34: return zero vector | 0.45 | VERIFIED |
| [0x1A6148](../../src/auto/func_001A6148.c) | `LO::SetAngularVelocity` | LO vt36 default no-op | 0.40 | VERIFIED |
| [0x1A6150](../../src/auto/func_001A6150.c) | `OnLoRemove` | LO vt22: unregister from g_sw per-object update table (0x210268: clear entry, dead++) | 0.45 | VERIFIED (arbitration/conflict fix; was LO::Freeze) |
| [0x1A6178](../../src/auto/func_001A6178.c) | `OnLoAdd` | LO vt23: register in g_sw update table (0x210140, compacts when >10% dead) | 0.45 | VERIFIED (arbitration/conflict fix; was LO::Unfreeze) |
| [0x1A61A0](../../src/hand/func_001A61A0.c) | `FIsLoInWorld` | bit60 set and plo->ploroot is in g_sw+0x2C list | 0.70 | HYP |
| [0x1A6290](../../src/hand/func_001A6290.c) | `FMatchesLoName` | True if plo->oid==oid or any proxy-alias node owned by plo has that oid | 0.75 | VERIFIED |
| [0x1A62D8](../../src/auto/func_001A62D8.c) | `LO::PmatWorld` | LO vt31 default: return static identity D_002A2D30 | 0.75 | HYP |
| [0x1A62E8](../../src/auto/func_001A62E8.c) | `PosLoWorld` | Return vt31 PmatWorld()->pos (row 3, +0x30) | 0.90 | VERIFIED |
| [0x1A6318](../../src/auto/func_001A6318.c) | `PmatLoWorld` | Return vt31 PmatWorld() | 0.90 | VERIFIED |
| [0x1A6730](../../src/hand/func_001A6730.cpp) | `CloneLo` | LO vt41 Clone(ploBase): copy Splice frame/sidebag refs, scale, flag bits, tag word, x34; clone proxy nodes when g_sw phase==-2; CopyAb the class body  | 0.75 | HYP |
| [0x1A6BE8](../../src/auto/func_001A6BE8.c) | `SetLoXfFromMat` | vt29 TranslateToPos(mat.pos) then vt30 RotateToMat(mat) | 0.50 | VERIFIED |
| [0x1A6F00](../../src/hand/func_001A6F00.c) | `ConvertAloPos` | Transform a point from paloFrom space to world (PmatWorld) then into paloTo space (inverse); vector by value | 0.80 | HYP |
| [0x1A6FC0](../../src/hand/func_001A6FC0.c) | `ConvertAloVec` | Same for direction vectors (rotation only, M3) | 0.80 | HYP |
| [0x1C9940](../../src/auto/func_001C9940.c) | `IdProxy` | (isrc<<16)\|(icopy&0xFFFF) | 0.85 | HYP |
| [0x1C9950](../../src/auto/func_001C9950.c) | `IsrcFromIdProxy` | idProxy>>16 | 0.85 | HYP |
| [0x1C99D8](../../src/hand/func_001C99D8.c) | `FindPxr` | Find (or create, allocating from world heap) the PXR path of ids aid[0..c) under a parent PXR | 0.70 | VERIFIED |
| [0x1C9B08](../../src/hand/func_001C9B08.cpp) | `LoadProxyFromBrx` | read one proxy record (source id/mode/placement) and instantiate copy or PROXY object | 0.90 | HYP |
| [0x1CA118](../../src/hand/func_001CA118.cpp) | `LO::SetIdProxy` | LO vt25: put object into proxy copy tree under idProxy | 0.90 | HYP |
| [0x1CA370](../../src/hand/func_001CA370.c) | `AddProxyCopy` | Take source isrc's next copy: SetIdProxy, Add, SetParent, apply placement (vt37) to each object; notify collector; icopyNext++ | 0.80 | VERIFIED |
| [0x1CA500](../../src/hand/func_001CA500.c) | `InitSwProxySpares` | After load: turn every unused copy of each proxy source into a spare (rest pose array aposRest, PROXYSPARE array, identity mats) | 0.50 | VERIFIED |
| [0x1CA788](../../src/auto/func_001CA788.c) | `InitProxy` | PROXY::Init (vt7): InitXfm + flag | 0.85 | HYP |
| [0x1CA7B8](../../src/hand/func_001CA7B8.c) | `CloneProxy` | PROXY::Clone (vt41): clone XFM base and copy init records | 0.75 | HYP |
| [0x1CA8E0](../../src/hand/func_001CA8E0.c) | `OnProxyAdd` | PROXY vt23: LO::Unfreeze then 0x149AB8 | 0.40 | VERIFIED (arbitration/conflict fix; was PROXY::Unfreeze) |
| [0x1CA930](../../src/auto/func_001CA930.c) | `IcopyFromPspare` | (pspare - aspare)/0x50 + first spare copy index | 0.85 | HYP |
| [0x1CAA60](../../src/hand/func_001CAA60.cpp) | `ReleasePspare` | return a copy to the spare pool and restore its objects | 0.90 | HYP |
| [0x1CB2C0](../../src/auto/func_001CB2C0.c) | `FCanAcquirePspare` | PspareChoose(this 2)!=0 | 0.40 | VERIFIED |
| [0x1CB328](../../src/auto/func_001CB328.c) | `EnsureProxyCopy` | if no current copy acquire one | 0.40 | VERIFIED |
| [0x1CB770](../../src/hand/func_001CB770.c) | `FindProxysrc` | index of proxy source with id or -1 | 0.80 | HYP |
| [0x231100](../../src/lib/func_00231100.c) | `InitXfm` | InitLo; pxfParent from parent+0x60; mat = identity | 0.85 | VERIFIED |
| [0x231170](../../src/auto/func_00231170.c) | `LoadXfmFromBrx` | 0x231768 (XF ref + matrix) then LoadOptionsFromBrx | 0.80 | VERIFIED |
| [0x2311B0](../../src/auto/func_002311B0.c) | `CloneXfm` | CloneLo 0x1A6730 then invalidate world | 0.80 | VERIFIED |

Selected 62 of 166 named functions (confidence >= 0.75, verified, or flagged). Full list: [reference/functions/object-model.md](../reference/functions/object-model.md).

## Open questions
- Meaning of LO slots 17, 20, 21, 26/27, 32, 37-39, 42-45 (shape-only names). Compare overriding methods across subclasses.
- What the second oid table at g_sw+0x1838 holds (1,752 non-object records at runtime).
- Real Sucker Punch names of the descriptor struct and LOROOT (team names).
