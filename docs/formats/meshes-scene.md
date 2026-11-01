# Meshes (GLOBSET) and scene assembly

How models are stored inside SO-family object payloads and how a level's objects are placed to rebuild the scene. Exporters:
`reports/meshes/sly2_mesh.py` (OBJ) and `reports/textured/sly2_export.py` (textured, placed glTF/OBJ); see [guides/asset-tools.md](../guides/asset-tools.md).
Render-time behaviour of the same data: [rendering](../engine/rendering.md).

Tags: `[C:static]` = matched loader; `[C:parse]` = consumed exactly by the parsers over all 43 world files (11,244 SOs, 4,326,803 type-0 triangles including
instances, 0 errors; every filtered sub-glob's skip size equals the bytes parsed); `[C:runtime]` = checked against the S02 RAM dump; `[C:visual]` = verified
in renders against game screenshots; `[H]` = inferred.

## 1. Where models live

Each SO-family object record (CID 0x30-0x6C) carries a GLOBSET as item 3 of its payload ([level-objects.md](level-objects.md#6-so-payload-loadsofrombrx-0x1f31e0-cparse)):
SO transforms (XFs), visibility mask, GLOBSET, collision parts (GEOM + TREE), mass, options, children.

## 2. GLOBSET (`LoadGlobsetFromBrx` 0x167320, matched) `[C:parse]`

```
S8 fCompact; VECTOR center; F32 radius; U8 csaa; csaa x SAA (U16 cid, 0xFFFF none, else 0x20 B); U8 n; n x S32; S16 cglob (slot count)
sub-globs until cglob slots are used (a sub-glob with csub extra instances uses 1 + csub slots):
  S8 cfilter; [cfilter x S16; S32 cbSkip]          (load filter; cbSkip = size of the rest of this sub-glob)
  S8 type; U16 csub; S16 ixf; S8 kind; F32 g4 (FLT_MAX = none); F32 g0; S32 oid; S8; S8 fHide
  [fCompact == 0: VECTOR center; F32 radius; U32 grf; F32 dist]
  body by type; then csub instance records
pose sets (0x167870): S16 c; c x up to 4 {S8 kind (4-bit, -1 ends the group); VECTOR; F32; F32; U8; kind 0/1: S16}
```
**Type dispatch** (compare chain on the type byte at +3: `lb $3,3($19)` at 0x167748, then beq/slti/beqz; no jal, no jump table) `[C:static v3.1]` + `[C:parse]`: type 0 -> 0x16A000, type 1 -> `LoadSubglobType1FromBrx` 0x16E438, type 2 -> 0x16CAA0,
type 3 -> 0x16CF78. (The v1 names were permuted; master_names_v2 fixed them.) Samples L02: type 0 x2022, 1 x254, 2 x40, 3 x99; fCompact on 6 of 848 globsets.
The `kind` byte: 1 = sky dome, 3 = buildings/backdrop, 4, 5, 10, 11, 13 other categories (11/13 select other VU1 entries) `[H]`.

### Type 0: the visible textured mesh (`LoadSubglobType0FromBrx` 0x16A000) `[C:parse]`

```
U8 flags (1 lit/multi path, 2, 4 has WRBG warp, 8 keep batch records, 0x10); S16 isaa; U8 cbnd; cbnd x {S32 oid; S16 ixf} (bones)
S8 iSpec; [iSpec != -1: 12 B + 8 B + VECTOR]
CLUTSET (0x16C8E0): U16 c; [c: c x {U16 n; n x {U16, U16, U8}}; U16 m; m x S16]
[flags & 4: S32 oid; S8 wek; [wek != -1: 4 x F32 + MATRIX4]; U8 cmat; U8 fDpos; U8 fDuv]
S16 cgroup; per group:
   U8 nbatch; nbatch x {S32 cvtx; S32 ishd (shader index in the WE's list); U32 grf}
   U8 nv; VECTOR pos[nv]; F32 uv[nv][2]; VECTOR normal[nv]; U8 rgba[nv][4]
   S8 fAlpha; [cbnd: U8 bone[4]; U8 weight[nv][min(cbnd, 4)]]
   U8 vidx[sum cvtx]                      (strip vertex -> index into the group's arrays)
   U32 bits[ceil(sum cvtx / 32)]          (one bit per strip vertex, LSB first, running across the group's batches)
csub instance records (0x169AC8): S8; [fCompact == 0: U32]; S16 ixf; MATRIX4; S32; S8; CLUTSET
```
- Each batch is one **triangle strip** of cvtx vertices; a set bit = ADC (the triangle ending at that vertex is not drawn; the first two vertices of every strip
  carry it) `[C:visual]`, matching the VU W-word bit 15 ([rendering](../engine/rendering.md#5-vertex-formats-and-the-main-glob-program-p0)). Winding alternates.
- UVs are floats; the loader stores `(int)(u*4096 + 0.5)` with no flip `[C:static]` (0x16A000 l.1227-1233). Since texture rows are stored bottom-up, glTF uses
  `(u, 1 - v)` on top-down PNGs and OBJ `(u, v)` `[C:visual]`.
- Vertex colour is multiplied by the shader colour at load for unlit shaders (`vtx * shd.rgba / 256`, alpha `vA * shdA / 510`, 0x80 = opaque) `[C:static]`; for lit
  shaders (shdk 1, 7) the relight program produces the final colours ([rendering](../engine/rendering.md#6-lighting-relight-and-outlines-the-cel-look)).
- Skinned meshes (cbnd > 0) are stored in **bind pose**: Sly, Murray and Bentley render correctly un-skinned `[C:visual]`. Weights: D_002BE558 = {-1, S8, V2_8, V3_8, V4_8}
  bytes per vertex by bone count.

### Types 1-3 (byte layouts `[C:parse]`, roles `[H]` except where noted)

| Type | Loader | Layout | Role |
|---|---|---|---|
| 1 | 0x16E438 | U8 cbone (signed); cbone x {S32, S16}; S16 cpart; cpart x {U8 v; v x VECTOR; [cbone: U8 nb; nb x U8; v x weights]; U8 c; c x U32 RGBA} | no shader, GIF prims, default dist 2000: shadow / volume data |
| 2 | 0x16CAA0 | bones; S16 cpart; cpart x {U16 ishd; U8 v; v x VECTOR; [cbone: U8 nb; nb x U8; v x weights]; U8 c; c x U32 RGBA} | shader-bound vertex sets |
| 3 | 0x16CF78 | bones; S16 cpart; cpart x {U16 ishd; U8 v; U8 cvec; cvec x VECTOR; [cbone: 4 x U8; min(cbone,4) x cvec weights]; v x U8 index; ceil(v/32) x U32 bits} | indexed strips |

At render time the sub-glob **subtype** (SUB grf bits 0-2) distinguishes mesh (0), cel-border edge records (1), projected/shadow volumes (2, 3) `[C:static rendering]`;
how file types 1-3 map onto subtypes 1-3 is not yet pinned down. The noun of 0x16C8E0 (CLUTSET vs "subglob bindings") is disputed (low stakes).

## 3. Transforms `[C:static]`

- XF world = `local * parent` in row-vector storage (`UpdateSoXfWorldMatrices` 0x22FAA0 uses VU0 `MultiplyMatrix4` with (parent, local)). The root has a NULL parent.
- XF parent reference (0x22D6B0): -1 -> NULL; 0x0nnn own XF n; 0x1nnn XF n of the **parent SO** (child SOs); 0x2000 -> XF-context stack top. In L02, XF[0] is the root and every
  other XF points at an earlier own XF. Kinds 8/14 (pivot = -origin) do not occur in L02.
- Sub-glob geometry is in XF[ixf] space; extra instances = instance MATRIX4 * XF[inst.ixf].
- **The SO's world position is XF[ref3]** (the third trailing XF reference, SO+0x60 = what `PmatWorld` 0x1F3FA0 returns) `[C:runtime]`.
- Coordinates: right-handed, **Z up** (camera basis rows at g_cm+0x60 = forward, left, up) `[C:runtime]`. 1 unit = 1 cm `[H]` (Sly is 196 units tall with ears).
  The exporter writes Y-up metres: `(x, y, z) -> (x, z, -y) * 0.01`.

## 4. Scene assembly and placement

- **World entries are proxy sources** (PROXYSRC.id = WE.x10). After loading, the objects of WEs with x5C = 0 are removed (vt9) and enter the world only through PROXY
  records; WEs with x5C = 1 (e.g. WE 286 `f_nightclub_exterior` = the Paris hub mesh) stay at their own coordinates. A scene export = the x5C WEs + everything their
  proxies pull in, recursively `[C:static]` + `[C:runtime]`.
- **PROXY placement** (`LoadProxyFromBrx` 0x1C9B08 record: `S32 idSource, S8 mode, S16 ixf, MATRIX4, U32 grfvis, init blocks, nested list`): `AddProxyCopy` 0x1CA370 ->
  `SetSoParent` 0x1F21B8 re-points every NULL-parent XF of the copy to pxf (or the parent SO's XF[0]); `SO::ApplyProxy` 0x1F4010 sets `XF[0].local = matProxy * XF[0].local`.
  So `world(XF0) = local0 . matProxy . pxfWorld` (row form). A top-level proxy inside a WE that is itself copied inherits the outer copy's transform
  (`matProxy_inner . matProxy_outer`; needed for `f_tree_ring -> f_tree`) `[C:static]`.
- Mode 1 (or -1 when the source WE has x60): a PROXY object stands at the placement and borrows one of the c8 spare copies at runtime (`PspareAcquire` calls LO/SO slot 26
  `ResetLoForProxy`/`ResetSoForProxy` after slot 37 `ApplySoProxy`) `[H top50]`; every live spare copy sits exactly on one of the PROXY placements `[C:runtime]`.
- **Shaders** come from the WE whose object list holds the SO; a proxy copy uses its source WE's shaders. Texture = SHD.atex[0], frame 0, pass 0 ([textures.md](textures.md)).
- **Load filter** = level index (2 in the Paris hub; D_003E1110) applied to object records (k > 0, k < -2) and sub-glob cfilter lists exactly as `FMatchLoadFilter` 0x13FAD0 `[C:runtime]`.
- **Runtime check** (`textured/verify_runtime_S02.txt`) `[C:runtime]`: all 31 directly loaded and 142 of 143 proxy-copied root SOs coincide with a live object of the same oid
  within 2 units (the exception, `f_skydome`, follows the camera in x/y); all 67 in-world child SOs match; 450 of 464 live in-world SOs have an exported instance
  within 2 units (misses: patrolling guards, Sly, carried dispensers, the sky).
- Editorial filter used by the exporter `[H]`: proxies nested in actor classes (players, guards, BOMB, ACCMISS, pickups, dispensers, gadget objects) are pooled or carried
  items and are skipped by default; fHide sub-globs, shdk-3 FX batches and types 1-3 are dropped. The pipe-room interior sits high above the hub (z ~ 4-6k), as in the game.

## Devtools findings (v3, 2026-11-01)

- **Placement rules for every record** (devtools/level_viewer, lib/scene.py), extending the SO rules above: XFM family (LIGHT, SHAPE/RAIL/PIPE, VOL, TN,
  EMITTER, CID24/27/36/43, ...) world = `LoadXfmXfFromBrx` 0x231768 matrix x resolve(ixf) with the same ref rules; top-level XFMs of a proxy copy get
  matProxy + repoint; LO-only classes (TASK, JOB, RELAY, ASEG, SM) have no transform. Runtime check: matWorld translation at **XFM+0xD0** (types round 2: confirmed only for LIGHT; plain XFM/PROXY/TARGET have it all zero; SO positions are at pxfRoot+0x70); 787/842 live XFMs in S02 match
  a placed instance within 2 units `[C:runtime level_viewer]` (see [object-model](../engine/object-model.md#devtools-findings-v3-2026-11-01)).
- Load filter: records with k > 0 are kept iff the level index (D_003E1110 = Lnn) is in the filter (`FMatchLoadFilter` 0x13FAD0) `[C:runtime]` (re-checked v3, and `[C:live]`).
- All 43 levels place with 0 mesh-parse failures and 0 missing proxy sources; an SO placed >= 3 times with identical XF structure can be instanced
  (Paris: 148 records / 1,799 copies) `[C:parse level_viewer]`.

## 5. Materials in the exporter

baseColorFactor = SHD.rgba/255 (exact for unlit shdk 0/2/4); COLOR_0 = vertex RGBA/255 (already includes the PS2 "x2" convention); alphaMode BLEND if SHD alpha < 1 or
render priority 13, MASK if the texture has binary holes, else OPAQUE; double-sided; wrap REPEAT (CLAMP per shader not decoded). Lit shaders are written as
`KHR_materials_unlit` (the relight lighting is not reproduced) `[H]`. Results: Paris hub 1,630 nodes, 862 materials, 322,875 triangles; Sly 1,787 triangles
(bind pose, faces +X) `[C:visual]` (screenshot comparisons in `reports/textured/compare_*.png`).

## 6. Open

- Which sub-globs are state-dependent (alternative heads, gadget meshes) beyond fHide and filters.
- Skinning export (bones/weights), shader animation, the other CLUT passes, WRBG warps.
- Mapping of file types 1-3 to render subtypes; collision GEOM export.
