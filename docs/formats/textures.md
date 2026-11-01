# Level textures: CLUTs, bitmaps, shaders (shd TU 0x1EA568-0x1ED7A8)

Textures are stored **per world entry** (WE), right after the WE's Splice entries ([level-objects.md](level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx)),
and loaded by `LoadShadersFromBrx` 0x1EB748. The exporter `reports/textures/sly2_textures.py` decodes them to PNG
([guides/asset-tools.md](../guides/asset-tools.md)).

Tags: `[C:static]` = matched decomp code; `[C:parse]` = the walk lands exactly on every following structure in all 43 world files (37,508 bitmaps,
38,750 shaders, 71,774 bitmap x CLUT pairs decoded with 0 errors); `[C:visual]` = the decoded images look right (signs and posters readable,
smooth gradients); `[H]` = inferred.

## 1. Layout inside a world entry `[C:static]` + `[C:parse]`

```
U16 nclut; nclut x CLUT record          LoadColorTablesFromBrx 0x1EAE40 / LoadClutFromBrx 0x1EA8A8
U16 nbmp;  nbmp  x BMP record           LoadBitmapsFromBrx 0x1EB098 / LoadBmpFromBrx 0x1EAF50
U16 nsaa;  nsaa  x {U16 cid (0xFFFF none); 0x20-byte SAA payload}     PsaaLoadFromBrx 0x1ED7A8
U16 nshd;  nshd  x SHD record           LoadShdFromBrx 0x1EB2F0 (0x2C bytes in memory)
Align(0x10); cbBlob bytes = the image blob    0x20F640 -> RelocateSharedColorTables 0x1EA568 -> UploadBitmaps 0x170730
```
The blob is re-read only when the WE's idBlob differs from the previous one; otherwise it is skipped by the same size.

## 2. CLUT record (0x10-byte header) `[C:static]`

| Offset | Field | Notes |
|---|---|---|
| +0 | U32 id | hash key; CLUTs are shared across WEs (`AddClutHash` / `PclutFindHash` 0x1ED560/0x1ED5D0) |
| +4 | U8 kind | 1 = colours in the blob (+0xC = blob offset, 16-aligned); otherwise inline (c x 4 bytes follow). Kind 3 = the "permanent" set uploaded every frame (all four kind-3 CLUTs in L02 belong to `f_skydome`) |
| +5 | S8 cch | if > 0, cch x S16 load-filter ids follow; the record is dropped unless the current filter matches (`FMatchLoadFilter` 0x13FAD0) |
| +8 | U16 c | colour count: <= 16 -> PSMT4 palette, else PSMT8 (`BuildClutTex2` 0x1EC9D0) |
| +0xA | U16 cgs | GS blocks to allocate |
| +0xC | U32 | blob offset (kind 1) |

Colours are RGBA32 (R,G,B,A bytes) `[C:visual]`. Every loaded colour also goes through `ColorCorrectRgba` 0x1ED278 when D_003E12C8 is set; that flag is the
anaglyph-stereo flag ([rendering](../engine/rendering.md#8-blips-hud-screen-effects)), so normal rendering does not correct colours `[C:static]`.

## 3. BMP record (0x18-byte header) `[C:static]`

`+0 U32 id; +4 U8 kind (1 = pixels in the blob at +0x14, else inline cb bytes); +5 S8 cch (filter ids follow); +6 U16 (0); +8 U16 width; +0xA U16 height;
+0xC U8 psm (GS PSM: 0x14 PSMT4, 0x13 PSMT8); +0xD U8 cbw (TBW = max(2, width/64)); +0xE U16 cgs; +0x10 U32 cb (= w*h*bpp/8); +0x14 U32 blob offset`.
**Only PSMT4 and PSMT8 occur in the whole game** `[C:parse]`. All L02 bitmaps are power-of-two. The image is uploaded with BITBLTBUF DPSM = its own psm, so the
data is in **linear host order, not pre-swizzled** into GS block layout `[C:static]` + `[C:visual]`.

## 4. SHD record (shader) and TEX `[C:static]`

0x18-byte header: `+0 U32 oid; +4 U8 shdk; +5 U8 grf; +8/+0xC/+0x10 three RGBA colours (colour-corrected); +0x16 render priority (13 = translucent [H]); +0x17 U8 ctex`;
then `S16 isaa` (index into the SAA list, -1 none) and `ctex x TEX`. TEX: 8-byte header `{+0 U32 ?, +4 U16 ?, +6 U8 cbmp, +7 U8 cclut}`, then cbmp x U16 bitmap
index and cclut x U16 CLUT index (0xFFFF none).

- **Passes per frame**: `CpassShd` 0x1EA688: shdk 1 -> 2 CLUTs, shdk 7 -> 3 (2 if grf & 0x20), else 1. TEX0 for frame i, pass j = `bmp[i].TEX0 | clut[i*cpass + j].TEX0`
  (`Tex0FromTexIframeCtk` 0x1EA6C8). A TEX with several bitmaps is a flip-book animation driven by the SAA (EYES, LOOP, ...); each frame has cpass palettes over the
  same index image.
- L02 shdk counts: 1:1303, 0:153, 2:73, 3:29, 7:16, 4:4; ctex = 1 everywhere. shdk 3 batches are flat-colour FX volumes (blast domes, glows, blob shadows) `[H]`.
- Pass meaning `[H]`: for shdk 1, pass 0 = the colour texture, pass 1 = a greyscale palette over the same indices (lighting/shadow, cf. Sly 1's three-way CLUTs).
- Every CLUT is used by exactly one (bmp, clut) pair in L02 (2766 pairs = 2766 CLUTs) `[C:parse]`.
- Meshes refer to shaders by index into **the shader list of the WE whose object list holds the SO** (`PshdFromIshd` 0x1EC8F8 = &D_003DCFEC[ishd], stride 0x2C);
  a proxy copy uses its source WE's shaders `[C:static]`.

## 5. Pixel decode (what `sly2_textures.decode` does)

1. Indices: PSMT4 2 px/byte, **low nibble = left pixel** `[C:visual]` (smoother in 241/256 test images); PSMT8 1 byte/px.
2. 8-bit CLUT un-swizzle (CSM1 storage): logical index i reads stored entry `(i & ~0x18) | ((i & 8) << 1) | ((i & 0x10) >> 1)` (0x08-0x0F <-> 0x10-0x17 in every
   32-entry group), only when c > 16 `[C:visual]` (roughness 17.6 vs 44.0).
3. Alpha: PS2 0x80 = opaque -> `a' = min(255, a*255/128)`.
4. RGB: every stored component is <= 0x80 `[C:parse]`; GS MODULATE computes `Cv = Cf*Ct >> 7`, so 0x80 = 1.0; the exporter scales by 255/128 (`--raw` keeps the stored values) `[H]` (matches the GS maths).
5. Rows are stored bottom-up: flip vertically (`--no-flip` to keep) `[C:visual]`. Mesh UVs are stored without a flip, so on top-down PNGs glTF uses `(u, 1 - v)` ([meshes-scene.md](meshes-scene.md)).

## 6. Runtime notes

After every load the texture-table counts and list heads are reset: `ResetShaders` 0x1EC7A0 is 17 instructions that zero the 8 words 0x3DCFE8-0x3DD004 and memset nothing `[C:static v3.1]` (plus `ClearTexHashTables`). The BMP/CLUT array contents may survive, so a save-state RAM cross-check is still worth retrying (v3 said the arrays are zeroed: PARTLY wrong, red-team #27).
GS upload, GS memory pools and per-frame re-uploads of kind-3 textures: [rendering](../engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic).

## Devtools findings (v3, 2026-11-01)

From devtools/texture_browser (all 43 levels, 37,508 bitmaps, 71,774 bitmap x CLUT pairs, counts equal whole_game/media_counts.csv):
- **No stored mips**: every bitmap has cb == w*h*bpp/8; only T4/T8 occur; 0 bitmaps unreferenced by a shader; 0 out-of-range shader indices `[C:parse texture_browser]`.
- **Dedupe**: 71,774 pair uses -> 6,708 unique RGBA images (10.7x); 3,452 unique index bitmaps; 6,715 unique CLUTs. RGBA8 1,005 MB -> 223 MB unique; unique
  indexed bitmaps + CLUTs = 22.8 MB. A port should dedupe by hash and can keep textures indexed `[C:parse texture_browser]`.
- Vectorised decode is pixel-identical to `sly2_textures.decode` on 2,103 sampled pairs. 19 images flagged by noise/stride heuristics look like real art `[H visual]`.

## 7. Open

- SAA payloads (0x20 bytes per animation) are not decoded; frames are exported as f0..fN.
- TEX header +0/+4 and shader header bytes 0x14-0x15.
- Fonts (`CFontBrx` 0x15F710) and "ZRB." streamed sprite files (0x2124F0) use other paths.
