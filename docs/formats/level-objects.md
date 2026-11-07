# World file, part 2: world entries and object records (all classes)

The part of a decompressed world file after the head (the head is in [world-file.md](world-file.md)). Every layout here is read from the matched
loader named in the table and exercised by the level parser (`reports/level_parser/sly2_assets`), which follows **100.000% of the stream bytes of
all 43 world files on the disc** (121,153 object records, every payload ends exactly at its stored `cb`, 0 skipped bytes, 116 classes) `[C:parse]`.

Tags: `[C:parse]` = read from the loader and proven by consuming every byte of the game's data (a wrong width anywhere breaks the walk);
`[C:static]` = read from the loader, but the class or branch never occurs in the data; `[H]` = meaning inferred.
Types: all little-endian; `VECTOR` = 3 x F32; `MATRIX4` = 4 x VECTOR (48 B, rows 0-2 + translation; `ReadMatrix4` 0x117F18); `MATRIX3` = 36 B;
`string16` = U16 cch + cch bytes, no NUL; `FILEREC` = 64-byte disc record ([disc-records.md](disc-records.md)). Signed counts that are negative read nothing.
How to run the parser: guides/asset-tools.md.

## 1. Where the stream ends

`LoadSwFromBrx` 0x13DA18 reads the head, then per world entry (WE) the block below + its object list + Align(0x10), and stops. Every file has
567-2,468 trailing bytes after that; they equal the bytes 0x1FFF earlier (LZSS ring echo, decompressor run-off) and are never read `[C:parse]`.
The draft's "unexplained trailer" is closed.

## 2. Per-world-entry header (loop body of LoadSwFromBrx)

| Field | Type | Loader | Tag |
|---|---|---|---|
| cbBlob, idBlob | S32, S32 (blob re-uploaded only when idBlob changes, D_003E1C58) | `LoadSwSharedHeaderFromBrx` 0x20F5D0 | [C:parse] |
| fExtra | S32 -> D_003E1C34 (debug info; 0 in all 7,753 WEs) | 0x13DA18 | [C:parse] |
| cref; cref x Ref | U32; Splice entries ([splice-scripts.md](splice-scripts.md)); +3 string16 each if fExtra | `LoadSwSpliceFromBrx` 0x1FF858 | [C:parse] |
| CLUT table | U16 n; n x CLUT | `LoadColorTablesFromBrx` 0x1EAE40 | [C:parse] |
| bitmap table | U16 n; n x BMP | `LoadBitmapsFromBrx` 0x1EB098 | [C:parse] |
| SAA table | U16 n; n x {U16 cid (0xFFFF none); 0x20 B to SAA Init} | `PsaaLoadFromBrx` 0x1ED7A8 | [C:parse] |
| shader table | U16 n; n x SHD | `LoadShdFromBrx` 0x1EB2F0 | [C:parse] |
| Align(0x10); cbBlob bytes | texture image blob (Read if reloading, else Skip) | `LoadSwSharedDataFromBrx` 0x20F640 | [C:parse] |
| zone BSP | U32 n; [n: U32 c8; U32 c10; n x {U16, U16, U16}; c8 x VECTOR4; c10 x S32] | 0x1C0EC0 / 0x1C0F30 | [C:parse] |
| object list | section 3 | `LoadSwObjectsFromBrx` 0x13FB40 | [C:parse] |
| Align(0x10) | | 0x117988 | [C:parse] |

Texture records (CLUT, BMP, SHD, TEX) are described in [textures.md](textures.md).

**WE == PROXYSRC** `[C:parse]` (structure) + `[C:runtime]`: the in-memory WE record (0x6C bytes at SW+0x3F08) is the PROXYSRC layout
`{cplo, aplo, ccopy, icopyNext, id, nUnk14, aoid, achzName[0x40], nUnk5c, fUnk60, ...}`: WE.x10 = proxy-source id (= PROXY.idSource; resolves for 1549 of
1577 proxies in L02), WE.c8 = number of copies, WE.x60 = fUnk60, WE.x5C != 0 = copies stay in the world at their own coordinates. Predicting live class
counts from this model matches 58 of 89 classes exactly and the total within 0.1% (8165 vs 8157 live in PCSX2 S02); the rest are runtime spawns
(BLIPG, RIPG, ASEGBL) or pool shuffles `[C:parse]`. The scene-assembly rules (which WEs are visible, how proxies place copies) are in
[meshes-scene.md](meshes-scene.md#4-scene-assembly-and-placement).

## 3. Object list and object record

```
S16 cobj; cobj x {
  S8  k            k > 0: k x S16 load-filter ids follow; k = 0: always; k = -1/-2: created unless the save state says the object is gone
                   (0x805 / 0x405 queries); k < -2: nothing read
  [k x S16 filter]
  S16 cid; S32 oid; S16 isplice (-1 none; index into this WE's Splice table); S32 cb
  cb bytes -> LoadProxyFromBrx 0x1C9B08 for cids derived from 6 (PROXY 6, CID7), else vtable slot 14 (LoadFromBrx) of class cid;
              Skip(cb) when filtered out
}
```
`[C:parse]`. The same list format nests inside payloads (SO children, TN/EXPLG children, SM's ASEGs, proxy lists; depth <= 4); `cb` always covers the
nested records. The filter matches the current level index (D_003E1110; 2 in the Paris hub) `[C:runtime]`. L02: k 0 x4351, 1 x265, -1 x72, -2 x50, 2..17 x85;
isplice set on 353 objects.

## 4. Common building blocks `[C:parse]`

**Options** (`LoadOptionsFromBrx` 0x13F810 -> `LoadOptionFromBrx` 0x13E818): `repeat { S16 eopid; eopid < 0 -> end; value }`. EOPID table `g_aeopid` D_002AC880
(stride 0x28: otyp +0, grf +4, crefReq +0x18); the loader masks grf & 0x2580. Value by otyp: 0 U8 | 5,6 S32 | 1 F32 | 10 2xF32 | 2,9,11 3xF32 | 4 3xF32
(Euler -> MATRIX3) | 3,12 4xF32 | 13 U32 RGBA | 22 U16 c + c x S16 | 7,14-17,19-21,23 S16 | (otyp & 0x7FFF) >> 12 == 1: U32 | == 2: S32 oid | other: nothing.
**METHOD options** (grf & 0x400) carry crefReq typed arguments: each `U16 atyp` (atyp & 0x8000 -> U8 n + n values), value by `RefFromBrx` 0x13E690: 0 U8 |
5,6 S32 | 1 F32 | 7,14-17,19-21,23 S16 | 2,9 3xF32 | 10 2xF32 | (atyp >> 12) == 1 U32 | everything else (incl. 3,4,11,12,13,22 and oids): nothing.
Method options are common (2,822 in L02) and appear in the head too (option 934 on g_sw, 44 instances game-wide); the original head-only parser stopped at
the first one. RELAY command lists arrive this way.

**XF part** (`LoadXfmXfFromBrx` 0x231768): `S16 ixf (parent transform index); MATRIX4 local; U32 grfvis`.
**GEOM** (`ReadGeom` 0x117FD0): `U16 cpos; cpos x VECTOR; U16 cface; U16 cedge; cface x {3 x U16 ipos; 4 x F32 (discarded, plane recomputed); U8 c; c x {U16 ipos0, U16 ipos1, U16 ifaceOther}}`.
**TREE** (`ReadBspc` 0x118220): `U32 cnode; U32; U32; cnode x {U16 iitem; iitem != 0xFFFF -> U32 left, U32 right}`.

## 5. Per-class payloads (vtable slot 14)

| Class (cid) | Loader | Payload | Tag |
|---|---|---|---|
| LO base: LO 0x02, CID112 0x70 (aim manager), CID113 0x71 (guard script), WR 0x73, CID116 0x74, CID117 0x75, CID120 0x78, PATHLINK 0x7C, CID121 0x79 | `LoadLoFromBrx` 0x1A5C28 | options | [C:parse] |
| RELAY 0x72; CID119 0x77; route CID123 0x7B | `LoadRelayFromBrx` 0x1DFB10 / `LoadCid119FromBrx` 0x1731B8 / `LoadRouteFromBrx` 0x14A008 | = LoadLoFromBrx (relay commands arrive via METHOD options; route copies its points to the heap) | [C:parse] |
| XFM base: XFM 0x03, LIGHT 0x04, TARGET 0x12, 0x13-0x15, 0x23-0x25, 0x27, 0x2C, CID43 0x2B, CID45 0x2D, recon target 0x2E, 0x10, 0x28, 0x2F | `LoadXfmFromBrx` 0x231170 | XF part; options | [C:parse] |
| PROXY 0x06, CID7 0x07 | `LoadProxyFromBrx` 0x1C9B08 (instead of slot 14) | `S32 idSource; S8 mode; S16 ixf; MATRIX4; U32 grfvis; U8 cinit; cinit x {S32 kind (5/6 = the copy's first object, else an oid); S16 cid; S16 isplice; U16 cb; cb bytes options}; S32 cbList; nested object list` (cbList includes its S16 count; 5,979/5,979 init blocks parse exactly) | [C:parse] |
| SHAPE 0x0A, PIPE 0x0B, CID12 0x0C, RAIL 0x0D, CID14 0x0E | `LoadShapeFromBrx` 0x1E5DF8 (CID14: `LoadCid14FromBrx` 0x1BC088) | XF part; S8 fTangents; S8 n; U16 c; c x 4 B; c x VECTOR; [fTangents: 2 x c x VECTOR]; options (spline control points `[H]`) | [C:parse] |
| CID5 0x05 (vis zone, `*_vz`) | `LoadCid5ZoneFromBrx` 0x227380 | XF part; U32; GEOM; TREE; options | [C:parse] |
| CID8/9 0x08/0x09 | `LoadCid8FromBrx` 0x185150 | XF part; S16 cv; cv x VECTOR; S16 cx; 2 x cx x 3 x S16; options | [C:parse] |
| TN 0x0F | `LoadTnFromBrx` 0x220FD0 | XF part; GEOM; TREE; options; nested list (`LoadXfmChildObjectsFromBrx` 0x2317E0) | [C:parse] |
| PATHZONE 0x11 | `LoadPathzoneFromBrx` 0x1C03C0 | XFM; U16 cpt; U16 ctri; cpt x {VECTOR; U8 c; c x U16 neighbour}; ctri x 3 x U16; U16 c3; c3 x {3 x F32, U16, U16} (navigation mesh `[H]`) | [C:parse] |
| VOL 0x16, CID23 0x17, CID27 0x1B; VOLBTN 0x18/0x19 | `LoadVolFromBrx` 0x227BB8 (0x18/0x19: 0x149358) | XFM; GEOM; TREE | [C:parse] |
| EXIT 0x1A | `LoadExitFromBrx` 0x2282B0 | VOL; FILEREC; U8 c; c x FILEREC (`LoadWarpBlobFromBrx` 0x228250) | [C:parse] |
| EMITTER 0x1D; EXPLO/EXPLS 0x20/0x21 | 0x258598 / 0x259030 (linkonce: `LoadEmitterFromBrx`, `LoadExploFromBrx`) | XFM; [EMITB+0x10 == 3 (set by option 483 / 536): U16 n; n x VECTOR; U16 m; m x {3 x U16, F32}; VECTOR]; U16 n; [n: U8; n x U32 RGBA] (3 = emit from a triangle mesh `[H]`; 14 objects, all and only those with the option = 3) | [C:parse] |
| EXPLG 0x1F | `LoadExplgFromBrx` 0x15C1E8 | XFM; nested list | [C:parse] |
| CID41 0x29, WARP 0x2A | `LoadCid41FromBrx` 0x228140 / `LoadWarpFromBrx` 0x228720 | XFM; string16 (skipped by the game: `WARP_*` / `entrance_*`) | [C:parse] |
| SO base and direct users (SO 0x30, MS 0x31, BOMB 0x4F, BARRIER 0x56, WATER 0x59, SKY 0x61, CID99 0x63, CID104 0x68, CID105 0x69, CID106 0x6A, WAYPT 0x6B, CID88 0x58) | `LoadSoFromBrx` 0x1F31E0 | section 6 | [C:parse] |
| SO + own wrapper: BOOTY 0x67, DPRIZE kinds 0x5B-0x5D/0x5F, LOOT 0x5E, CID98 0x62, CID100-102 0x64-0x66, BUTTON 0x53, MISSILE family 0x50-0x52, CID85 0x55, CID96 0x60 | `LoadBootyFromBrx` 0x13BE58, `LoadDprizeFromBrx` 0x1C4EF8, `LoadLootFromBrx` 0x1C76D0, `LoadCid98FromBrx` 0x1BABA0, `LoadCid100FromBrx` 0x223FE0, `LoadButtonFromBrx` 0x148C08, `LoadMissileFromBrx` 0x1B1E00, `LoadLasenFromBrx` 0x1DE478, `LoadCid96FromBrx` 0x1B0CA8 | LoadSoFromBrx; no further file reads | [C:parse] |
| PO family: PO 0x32, TURRET 0x45, SAFEHOUSE 0x46, 0x47-0x4E | `LoadPoFromBrx` 0x1C3DC8 | SO; string16 (skipped by the game) | [C:parse] |
| STEP family: GOMER 0x39, GUARD 0x3A, characters 0x3B-0x44, JT 0x35, MURRAY 0x36, BENTLEY 0x37 | `LoadStepFromBrx` 0x2039C0, `LoadJtFromBrx` 0x1885B8, `LoadMurrayFromBrx` 0x1B4888, `LoadBentleyFromBrx` 0x134770, `LoadCid59FromBrx` 0x1553F0 | = LoadPoFromBrx (the rest is runtime-only) | [C:parse] |
| ASEG 0x6D | `LoadAsegFromBrx` 0x12F2A8 | section 7 | [C:parse] |
| SM 0x6F | `SM::LoadFromBrx` 0x1EFAC0 | nested list (its ASEGs, then removed from the world); U8 csms; csms x {S32 oid, S32 oidNext}; U8 csmt; csmt x {S32 from, S32 to, S32 grf, S32 grfBreak, F32 probability, S32 oidAseg}; options | [C:parse] |
| WIDGET family 0x7E-0x83 | `LoadWidgetFromBrx` 0x212298 | section 8 | [C:parse] |
| JOB 0x84 | `LoadJobFromBrx` 0x1624A8 | options; string16 (save name, skipped by the game) | [C:parse] |
| TASK 0x85 | `LoadTaskFromBrx` 0x1635F8 | options; U8 c; c x S32; U8 c; c x S32; string16 (two oid lists + save name) | [C:parse] |
| CID122 0x7A | `LoadCid122FromBrx` 0x153B08 | options; U8 c; c x {string16; S8; S32; S32; FILEREC} | [C:static] |

The 26 classes that appear only outside the four original samples all go through these loaders `[C:parse whole_game]`.

## 6. SO payload (`LoadSoFromBrx` 0x1F31E0) `[C:parse]`

1. `LoadSoXfsFromBrx` 0x22D7A0: `S16 cxf; cxf x {S16 iparent; S16 kind; S32 tag; U8 fl; fl&1: F32 | else fl&2: 2 x F32; fl&4: S8; fl&8: S8, F32, VECTOR, 8 B, VECTOR, 8 B, S8, S8, S32; fl&16: S8, F32, F32; fl&32: VECTOR; kind 8/14: S16 origin ref | other kinds: MATRIX4}`;
   then `LoadSoIkFromBrx` 0x186670 (`U8 c; c x {S16, S16, S16, VECTOR, S16, VECTOR}`), `LoadXfmConstraintsFromBrx` 0x1603B8 (`S8 c; c x {S16, S16, S8, S8}`),
   `LoadSoChainsFromBrx` 0x21C888 (`S8 c; c x {U8, 4 x F32, VECTOR, VECTOR, S8 n, n x S16}`), 3 x S16 XF refs (the third = the SO's world XF, SO+0x60).
2. `U32 grfvis`.
3. GLOBSET = the mesh ([meshes-scene.md](meshes-scene.md)).
4. `S16 cpart; cpart x {S16 ixf; S32; S8; S8; F32 radius; GEOM; TREE; U32; S16; S16; S8}` = collision parts (SOPART).
5. `[cpart > 0: F32 mass; MATRIX3 inertia]`.
6. options.
7. nested object list (the SO's children).

## 7. Animation segment (`LoadAsegFromBrx` 0x12F2A8) `[C:parse]`

`F32 tMax; S16 cchan; cchan x {S32; S16; S8 k1; [k1 != -1: AC(f1, k1)]; S8 k2; [k2 != -1: AC(f2, k2)]; U8}; U8 c2; c2 x {U8; S32; S8 k; AC(f2, k)};`
`events(timed 0); events(timed 1); events(timed 0)` (`LoadAsegEventsFromBrx` 0x12F590); options.

Actuator curves (factory 1 `PacNewKind` 0x12B3B0, factory 2 0x12CB98; load = vtable slot 1; t in 1/60 s):

| Kind | Loader | Payload |
|---|---|---|
| f1/0 | `LoadAcConstFromBrx` 0x12B4F8 | 12 B |
| f1/1 | `LoadAcVecAFromBrx` 0x12B5E0 | U16 c; c x {U16 t; 3 x VECTOR} |
| f1/2 | `LoadAcVecBFromBrx` 0x12BA98 | U16 c; c x {U16 t; 2 x VECTOR} |
| f1/3 | `LoadAcFloatAFromBrx` 0x12BF30 | VECTOR; U16 c; c x U16 t; U8 grf; per set bit 0..2: c x 3 x F32 |
| f1/4 | `LoadAcFloatBFromBrx` 0x12C570 | same with c x 2 x F32 |
| f1/5, f2/2 | 0x12B4F0, 0x12CC60 | empty |
| f2/0 | `LoadAcFloatConstFromBrx` 0x12CC68 | F32 |
| f2/1 | `LoadAcFloatKeysFromBrx` 0x12CCB8 | U16 c; c x {U16 t; 3 x F32} |

`[H]`: f1 = position/rotation channels, f2 = scalar channels; A/B = Bezier vs linear keys.
Events: `U8 c; c x {S8 kind; [timed: S16 t]; kind 0: S16, 3 x S32, 3 x F32 | 3: 2 x S16 | 5: S16, F32 | 7, 8: S32, F32 | 13-16: 2 x S32 | 17: 3 x S32, F32 | 20: 2 x S32, F32 | 1, 2, 21: S16 | 4, 11, 12, 22: S32 | 24: 3 x 8 B | other: nothing}`.

## 8. WIDGET payload (`LoadWidgetFromBrx` 0x212298) `[C:parse]`

`options; S8 fStreamed; F32; U32; U32; U32;` then `fStreamed: FILEREC` (a "ZRB." sprite file on disc) or the inline sprite data:
- `LoadSpriteTable` 0x214398: `U16 n; n x {U16 id; U32 kind; 4: U16 | 8: SpriteTable (recursive), U32 c, c x U32, U32 c, c x F32 | 3: U32, U32, F32, F32, F32, S8, S8 mode, [mode 0/1: S32] | 2: MATRIX3, U16 c, c x 0x18 B | 6, 7: 4 x S16, S32 texture | 1: U16 c; c x {U8 n, U8, U8, U8, U8; n x {U32, U8, S16, S16}} | 0, 5, 9, other: nothing}` (`LoadSpriteDescriptor` 0x2149D8).
- `LoadSpriteGroups` 0x214460: `U16 ng; ng x {U16 creuse; U16 cframe; U16 canim; canim x {F32; S32; U16 ckey; ckey x {F32 t; U8 ci; ci x {U16, U16, U16, S16, U8, U8, S32, MATRIX3}; U8 cc; cc x 2 x U32 RGBA; U8 cs; cs x U16; U8 cb; cb x U8; U8 ct; ct x {U8 kind, S32}}}}`.

`[H]`: a Flash-like 2D UI (sprites, shapes, keyframed groups); WIDGET131 in the front-end world is 3.5 MB.

## 9. Statistics and object inventory

- Byte budget (L02): meshes 41%, texture blobs 20%, animation (ASEG) 19%, collision 9.6%, UI sprites 3.3%, rest 7% `[C:parse]`.
- Whole game: 488,840,496 stream bytes, 7,753 WEs (1,900 distinct names; 66 names in >= 42 levels = the shared character/FX/pickup kit), 121,153 object
  records, 10,930 Splice entries `[C:parse]`. Per-level x class counts and every WE: `reports/whole_game/{objects_by_level,objects_matrix,we_names_by_level}.csv`.
- Class identities from the object data (WE names and object strings; `whole_game/class_name_hints.csv`) are the source of the v2 class names in
  [`include/sly2/classes.h`](../../include/sly2/classes.h).

## 10. Open

- 10 PROXY records in L02 whose idSource matches no WE.x10 of the file.
- Meaning of several fixed fields (zone BSP, SOPART trailing fields, ASEG channel header).
