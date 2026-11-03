# Rendering (port-oriented)

How a frame becomes pixels, written for someone who has to reproduce it on a modern GPU. Objects queue **render-list entries**
(RPLs); `DrawSw` sorts them into 26 **kinds**, each kind maps to one of 12 **passes**; a pass hook uploads a **VU1 microprogram**
and GS state; per-entry DMA chains feed vertices to the microprogram, which transforms them and kicks GS primitives.
For readers new to the PS2: the EE (main CPU) builds DMA chains; **VIF1** unpacks data into **VU1** data memory (1024 qwords) and
starts microprograms (MSCAL); the microprograms XGKICK GIF packets to the **GS** (rasteriser: 4 MB local memory, two drawing contexts,
**no stencil buffer**, blend `(A-B)*C+D`). A port replaces the microprograms with vertex shaders and the GS register state with
pipeline state.

Sources: `rendering/RENDERING.txt` (code + 23 RAM snapshots), `vu_microcode/MICROCODE.txt` (full disassembly of the seven VU
programs, validated decoder) and `vu_microcode/pseudo_shaders.glsl` (shader sketches). Tags below: `[C:static]` = matched code,
ELF data or the VU disassembly; `[C:runtime]` = measured in RAM snapshot chains; `[H]` = inferred.
Related: [camera](camera.md), [object-model](object-model.md), [mesh and scene format](../formats/meshes-scene.md),
[texture format](../formats/textures.md), [particles-effects](particles-effects.md).

## 1. Frame structure

| Step | Function | What | Tag |
|---|---|---|---|
| open | `OpenFrame` 0x160828 | open this frame's FRM (two FRMs, `g_pfrmOpen` D_002DEA4C) | [C:static] |
| render | `RenderSw` 0x1CDA80 (SW vt8) | light-tree update; SW+0x3D60++; reset `g_prplNext`; if the illumination stamp SW+0x3D5C changed, `SubmitSwRelightAll` 0x1CD9E0 queues a RELIGHT entry per lit model; vt19 `RenderSelf` on live objects with `(grf & 0x88) == 8`; flush `g_gifs`/`g_vifs` | [C:static], counter [C:runtime] |
| draw | `DrawSw` 0x157FA0 (SW vt9) | sort, frame prologue, pass dispatch, emit the VIF1 chain (section 3) | [C:static] |
| UI | `DrawUi` 0x1D9820 | 3 HUD screens + 2 DMAS flushes (existing symbol `UpdateRecyclable__3CGc` is wrong) | [C:verified arbitration] |
| close | `CloseFrame` 0x160A98 | finalise, signal the render thread | [C:static] |
| GPU | `FrameRenderLoop` 0x160B70 (thread) | `PrepareGsForFrameRender`, `RebuildFrameGifs`, DMA VIF1 then GIF, optional `BlendPrevFrame` 0x160F98 (previous frame drawn back in 32-px strips at alpha 0x64/0x80 = frame blend), `SwapGsBuffers` | [C:static] |

`SO::RenderSelf` 0x1F3C70: if `so->grfvis (+0x2D) & g_cm.grfvis (+0x14)`, `DrawGlobset(so+0x80, NULL, alpha +0x12C, alpha, pshd +0x134)` `[C:static]`.
Display: `sceGsSetDefDBuff(CT32, 512x224, ztest 3, Z32)` in `StartupGs` 0x1700D0 = field-rendered 512x224 double buffer `[C:static]`; standard libgraph layout
FB0 0x000 / FB1 0x700 / Z 0xE00 blocks `[H]`. GS pools: GSB0 blocks 0x1800-0x4000 (level textures, uploaded once), GSB1 0x1000-0x1800 (overlaps Z; section 7).
The paused world is still rendered every frame (pause menu S54) `[C:runtime]`.

## 2. Render list entries (RPL)

`g_arpl` D_003B3980[0x2000] of 16-byte entries, bump pointer `g_prplNext` D_003D4A50 (`PvAllocRenderEntry` 0x1CD998) `[C:static]`:
`{int kind; float z; u32 pqwA (per-instance packet, built this frame); u32 pqwB (static load-time chain, may be 0)}`. z = squared camera distance
(or an authored distance with sub grf 0x400; HUD = layer << 18; screen tint = -1.0). DrawSw emits `CALL pqwA`, `CALL pqwB` per entry.

Submitters: `SubmitGlobsetRelight` 0x1681F8 (kind 0), `DrawGlobset` 0x168638, `DrawGlobsetSkinned` 0x169198, `CullSceneRenderList` 0x1D5808 (static scene,
kind from record bits 44-49), `RenderBlipgSelf` 0x13A1E8 (12), `RenderCid74Self` 0x15DD48 (1 or 14), hud widgets/rich text/sprites (21) `[C:static]`.

**Kind remap in DrawGlobset** `[C:static]`: `k = (sub.grf >> 3) & 0x1F`; `alpha = gFirst * min(gVisModel, gVisSub)`; `fFg = model.grf bit 34 && g_sw+0x3BEC == 0`
(no pausing widget); k 3/4 -> 22/23 if foreground, else k when alpha == 1, else 5 (from 3) / 6 (from 4); k 5/6 -> 23 if fFg; 9 -> 24; 10 -> 25.
Render-to-texture slots remap through `g_ampkindRtt` D_002C0BF8 (k 3,4,15,18,22 -> 15; 5,6,16,19,23 -> 16; 9,17,20,24 -> 17; slot 1 uses 18/19/20).

## 3. DrawSw: sort, prologue and pass dispatch (`src/game/engine/graphics/pipeline/draw.cpp`) `[C:static]`

1. `CprplPartitionArpl` 0x157AE0: counting sort into 26 buckets; far-to-near qsort (`NCmpPrplReverseZ`) on buckets 5, 6, 10, 1, 2, 25; near-to-far on 21; others keep submission order.
2. CALL the frame-context packet D_002DF800; if SW+0x3E3C (force clear) or bucket 1 empty, DIRECT the colour+Z clear `g_aqwClearColorZ` D_002DEDC0.
3. Upload VU1 globals (absolute qwords): 0/1 viewport scale/offset + depth map, 2-5 world->clip (CM+0x140), 6-9 camera matrix (CM+0x60, row 3 = position),
   0xA ambient `{r,g,b,shadow}` (g_sw+0x3E28, `UpdateSwIllum` 0x20F1C8), 0xB-0xC framebuffer FRAME/TEX0 words (CT32 and CT16 views of both buffers),
   0xD debug word D_002DE5A4. Then BASE 0x32 / OFFSET 0x1E7 -> VU1 double buffers at 0x032 and 0x219.
4. For each sorted entry: `pass = g_mpkindpass[kind]` (D_002BDD20); on a pass change call `g_arpk[prev].pfnEnd(vifs, kindPrev, kind)` then
   `g_arpk[new].pfnBegin(vifs, kind, kindPrev)` (D_002BDD88); emit the entry's CALLs. Finally the last pfnEnd, reset, and an optional background gradient rebuild.

**This table is the main extension point for a native renderer**: keep the 26 buckets and sort rules, implement the 12 pass hooks below.

## 4. The 26 kinds and 12 passes `[C:static]` (tables dumped from the ELF); roles `[C:static]` unless marked

| Kind | Pass | Role | Notes |
|---|---|---|---|
| 0 | P4 RELIGHT | compute-only vertex relighting (section 5) | when lights/ambient change |
| 1 | P3 BACKGROUND | sky dome (SKY, CID 0x61) and CID116 (0x74) mode-0 colour fill; far->near | TEST 0x317E1: no depth test or write |
| 2 | P0 | unused (no producer in 23 snapshots) | `[H]` background translucent |
| 3 | P0 GLOB | **opaque world** | layer 1 TEST 0x717EB (ATST GEQUAL 0x7E, ZTST GREATER), ALPHA 0x44, fog; optional additive layer 2 |
| 4 | P11 CUTOUT | alpha-tested geometry with FBA_1 = 1 (marks dest alpha "solid") | |
| 5 | P0 | translucent (authored k5 + faded k3); far->near | |
| 6 | P11 | translucent cutout (faded k4); far->near | |
| 7 | P6 VOLUME | **shadow volumes**, channel A (section 7) | on nearly every character/prop |
| 8 | P7 VOLUME | shadow volumes, channel B (subtype 3 only) | second stencil channel |
| 9 | P1 CEL BORDER | **outlines** (section 6) | after opaque, cutout, translucent, shadows |
| 10 | P0 | late translucent (glass, water, decals, light cones); far->near | `[H]` roles |
| 11 | P0 | late opaque (CID98/CID101, BOMB, ACCMISS) | `[H]` "no projected volumes" |
| 12 | P2 BLIP | BLIP particles (chain A only) | |
| 13 | P0 | post-blip (a few SO/GUARD subs) | `[H]` |
| 14 | P0 | screen tint/flash/fade (CID116 mode 1, full-screen sprite) | |
| 15-17 / 18-20 | P8 / P9 (+P0, P1) | render-to-texture slot 0 / 1: opaque+cutout, translucent, outlines into a 128x128 CT32 target | owners CID45 (<= 4 objects); sampled next frame; item preview `[H]` |
| 21 | P5 HUD | widgets, rich text, sprites; z = layer << 18; near->far | |
| 22-25 | P10 FOREGROUND (+P0, P1) | model grf bit 34 objects while no pausing widget is open: Z cleared, drawn over the HUD | none in snapshots |

Per-frame counts in gameplay `[C:runtime]`: k0 0-12, k1 4, k3 300-530, k4 110-290, k5 56-300, k6 30-112, k7 73-156, k9 29-88, k10 108-178, k12 42-153, k21 12-25.

| Pass | Begin / End | Microprogram (data symbol) | Hook behaviour |
|---|---|---|---|
| P0 GLOB | 0x16E1A8 / 0x16E228 | `g_vuMain` D_0025E680 (1932 instr) | zero the batch-header block, MSCAL 0x0; end MSCAL 0x3AB0 (flush), FLUSHA. All GS state comes from per-batch material blocks |
| P1 CEL BORDER | 0x16EA20 / 0x16EA48 | `g_vuCelBorder` D_0025B880 (292) | |
| P2 BLIP | 0x13A830 / 0x13A858 | `g_vuBlip` D_0025A0C0 (472) | |
| P3 BACKGROUND | 0x16E298 / 0x16E380 | main | + replay the per-frame texture uploads `g_apqwFrameUpload` D_003DCFA8 (class-3 bitmaps); end + Z-only clear `g_aqwClearZ` D_002DF010 |
| P4 RELIGHT | 0x167FC0 / 0x1681B0 | `g_vuRelight` D_0025E090 (186, no XGKICK) | upload up to 256 lights (3 qw each from LIGHT+0x120, `FindSwObjects(g_sw, 0x505, ...)`) |
| P5 HUD | 0x211A80 / 0x211C10 | `g_vuHud` D_0025AFA0 (280) | HUD constants, DIRECT 0x27 qw HUD GS state, MSCAL 0xC0/0x100 |
| P6/P7 VOLUME | 0x16DA48 / 0x16DD58 | `g_vuProjVolume` D_0025C1C0 (972) | stencil init / resolve / apply (section 7) |
| P8/P9 RTT | 0x1BE990, 0x1BE9E8 / 0x16E228 | main | `SetupItemRenderView` 0x1BE6F0: target setup + slot camera, then P0 |
| P10 FOREGROUND | 0x157CF0 / 0x16E228 | main | re-CALL frame context, Z-only clear, re-upload globals, then P0 |
| P11 CUTOUT | 0x16E3C8 / 0x16E400 | main | P0 + FBA_1 = 1 on / off |

Every pass begin re-uploads its whole microprogram (`EnsureVu1Code` 0x160918 appends a DMA REF; no caching). VU0 runs a separate helper set
(`g_vu0Micro` D_00259DF0, 84 instr: 0x1C0 sphere visibility with a fade over the last 400*scale units, matrix and point helpers) `[C:static]`.

## 5. Vertex formats and the main glob program (P0)

**Batch header** (VU data, per batch) `[C:static]` + `[C:runtime S04]`: 0-3 model (or world, for boned subs) matrix; 4-7 model->clip; 8 params
`{alpha*gFirst (fade), alpha*gSecond, pmodel+0x50 (tint amount, 0 in samples), grfMode}`; 9 fit params `{W base magic float, fog K, fog d0}`
(`CalcRecipFitParams`); 0xB.x flags (0x2 per-object lights, 0x10/0x20/0x40 skinning, 0x80 double-sided, 0x100/0x200 billboards); 0xD `{vtxBegin, vtxSplit,
vtxEnd, NLOOP|EOP}`; 0x15.. material layer blocks of 9 qw: two GIF tags (PRIM 0x7C tri-strip and 0x7D fan, IIP+TME+FGE+ABE, PACKED, 4 regs) + A+D
`TEX0, TEX1 (0x61 bilinear), CLAMP, TEST, ALPHA, FOGCOL`.

**A family** (static, unskinned, no shader animation; entries 0x2E20 / 0x2E68 / 0x2EB0 for material layer 0 / 1 / 2+, closed by 0x2EE8), 4 qw per vertex,
transformed **in place** into the PACKED GIF stream `[C:static]`:

| qw | Unpack | Content | Becomes |
|---|---|---|---|
| +0 | V2_16 | ST, s16 4.12 fixed | (S/w, T/w, 1/w): perspective-correct via Q |
| +1 | V4_5 + masked S_8 | colour 0 (RGBA5551 from relight output A), alpha from a byte stream | RGBAQ (layer using colour 0) |
| +2 | V4_5 + masked S_8 | colour 1 (relight output B) | RGBAQ (layer using colour 1) |
| +3 | V4_32 | position xyz; **w = magic float flag/fog word** | XYZF2 |

The material tag's REGS picks the colour: 0x4F12 = ST,RGBAQ,NOP,XYZF2 (colour 0) or 0x41F2 = ST,NOP,RGBAQ,XYZF2 (colour 1). Typical opaque batch: layer 0
colour 0, TEST 0x717EB, ALPHA 0x44; layer 1 colour 1, TEST 0x51001 (no Z write, ZTST GEQUAL), ALPHA 0x48 (`Cs*As + Cd`, additive), FOGCOL 0
`[C:runtime S04]`. Layers 1/2 re-kick the already transformed vertices with the next material block. Textures: PSMT4/PSMT8 with CT32 CLUTs, CLD = 1 per
batch, TFX MODULATE, TCC = 1.

**B family** (skinned, shader-animated, multi; entries 0x2EF8 / 0x2F80 / 0x2FC8, closed by 0x3358): V3_16 positions, V4_8 signed normals, colours; optional
stages before the same transform `[C:static]`, purposes `[H]` where noted: billboards (0x70; spherical 0x100, cylindrical about Z 0x200), UV offset
(0xD28; UV scroll / SAA), 4-matrix skinning (0x16B0 positions, 0x1940 positions+normals; weights V4_8/255), offset blend with 4 delta vectors (0xE30;
morph), view-angle alpha `clamp(a + b d + c d^2)` (0x13F0; rim/fresnel fade), per-object dynamic lighting for <= 4 lights (0x298 + 0x1050), flip of the
back-face mark (0xFC0; second side of double-sided geometry).

**Per-vertex maths (A family)** `[C:static]`:
```
clip = M * vec4(p, 1);  q = 1/clip.w;  ST = (s, t, 1) * q;  XYZ = viewport(clip.xyz * q)
W    = Wvtx + K * min(clip.w, d0) / clip.w                 (K = MEM[9].y, d0 = MEM[9].z; S04: K = 1360, d0 = 3000)
fog F (8 bits) = ((Wvtx >> 4) & 0xFF) + K/16 * min(1, d0/clip.w)    (GS: F = 255 no fog, 0 = full FOGCOL)
fade (params.x < 1): colour0.a *= params.x; colour1.rgb *= params.x
```
**Vertex W flag word** = 2^23 magic float + integer bits: 15 = ADC (strip restart / triangle not drawn), 12-14 culling marks (drawable vertices carry 7 so that
adding 4096 / 8192 / 16384 carries into bit 15: rejected off-screen / needs clipping / back-face culled), 4-11 fog F, 0-3 = 0xF. Authored strip restarts
arrive as 0x8xxx `[C:static]`.
**Projection**: the viewport constants are "magic floats" whose bit pattern is the GS XYZF2 fixed-point word; the clip cube is a +-2027 px guard band
around the 4096x4096 GS centre (the 512x224 screen is |ndc.x| < 0.126, |ndc.y| < 0.055). Z is "bigger = nearer" (ZTST GREATER/GEQUAL) `[C:static]`.
**Culling/clipping**: per-triangle back-face test against the double-sided flag (MEM[11].x & 0x80); triangles crossing the guard band are clipped later
against 6 planes and kicked as fans `[C:static]`. PORT: normal projection with reversed-Z; hardware clipping; honour double-sided.

Batch scheduling details (buffer flip by MSCAL 0x0, continuation "yield" 0x2D38 for split batches, per-object texture swap via params.w) can be ignored by a port.
The debug word D_002DE5A4 is consumed only by unreachable debug-break hooks (glob 0x2DD0, cel 0x10) `[C:static]`.

## 6. Lighting ("relight") and outlines: the cel look

**Relight (kind 0, P4, prog5)** computes two colour sets per vertex whenever lights or ambient change; the main program never lights A-family vertices per
frame `[C:static]`. Lights are LIGHT objects (CID 4), 3 qw each: `{int type (0 directional, 1 point), k0, k1, kAtt0}`, colour `{r,g,b,w}`, direction of travel
(directional) or `{position, attenuation slope}` (point). Formula (prog5 0x10-0x548):
```
n = normalize(inverse_transpose(M3) * n_model);  p = M * p_model;  acc = vec4(0, 0, 0, wBias + ambient.w)
for each light:
  directional: d = dot(n, L.dir)
  point:       skip the light for the whole model if kAtt0 + slope*(|sphere.c - L.pos| - sphere.r) <= 0
               v = p - L.pos; d = dot(n, v/|v|); att = clamp(kAtt0 + slope*|v|, 0, 1)
  I = clamp(k0 + k1*(d + d^3), 0, 1) * att;   acc += L.colour * I       (four lanes)
m = max(min(acc.rgb, 1));  s = min(acc.w, 1 - m);  u = 1 - m - s
colour A.rgb = (min(acc.rgb, 1) + ambient.rgb * u) * max(vtx.rgb);   A.a = vtx.a
colour B.rgb = vtx.rgb * s * vtx.a / (127 * (1 + 1/127 - u));        B.a = 128 - 127u   ("how directly lit")
```
`StoreRelightColors` 0x167E58 (VU1 interrupt 0x167F78) packs A and B to RGBA5551 into the glob's colour buffers, which are exactly colour 0 / colour 1 of the
A-family vertex format `[C:static]` + `[C:runtime]` chain decode. **There is no banding or quantisation anywhere in the VU code**: the cel look comes from the
art, the steep `k0 + k1(d + d^3)` curve (k1 < 0 against the direction of travel), the "light fills the headroom, ambient only the rest" rule (u), and the
outlines `[C:static]` (whole program read). B-family objects get the same maths per frame for <= 4 lights on a 0..2 scale (0x298 + 0x1050).
Sample values `[C:runtime S04]`: point k0 0.08-0.13, k1 -0.2..-0.35, kAtt0 1.3-1.8, slope -0.002..-0.007; directional k0 0.163, k1 -0.261. The world
night look: SW `vecLightRgb` (0.094, 0.176, 0.341) at g_sw+0x3E28, fog RGBA 2a3d5c over 3000-30000 `[C:runtime]`.

**Outlines (kinds 9/17/20/24, P1, prog3)** `[C:static]`: per object, V3_32 model-space positions and V4_8 edge records `(opposite vertex A, edge start, edge end,
opposite vertex B)` (Sly 1 CELPOINT[4]; S04 sample 83 vertices, 178 edges). Per edge: project the four points; skip if any is outside the clip cube;
`a = cross2(P1-P0, P2-P1)`, `b = cross2(P3-P2, P2-P1)`; **draw iff a*b < 0** (the two faces sharing the edge face opposite ways = silhouette). Line from
P1 to P2 offset by 0.00024 NDC (~0.49 px) toward the side given by sign(a), extended by the same amount. Primitive: **one GS LINE**, pass 1 antialiased
(PRIM 0xC1, ZTST GREATER, colour only, ALPHA 0x44), pass 2 plain line writing Z only. Colour from the batch header (sample (16,16,16,128)); with flag 1:
`rgb = lerp(header.rgb, (160,0,0), params.z)` (red flash, `[H]` damage/alert) and `a = header.a * 0.6 * params.y` when faded. PORT: expand each edge into a
thin quad (geometry/compute shader or instanced quads), ~1 px at 448 lines, scale with resolution, depth test "nearer-or-equal" without depth write, then a depth-only pass.

## 7. Shadow volumes (kinds 7/8, P6/P7, prog4): Z-fail counting in the green channel `[C:static]`

The GS has no stencil buffer, so Sly 2 counts in the **green channel** of the framebuffer:
- Volume geometry uses the glob skeleton with 2-qw vertices. Each triangle's colour G is +2 or -2 (0xFE) by its screen winding. The vertex list is kicked twice:
  material 1 `ZTST GREATER, ALPHA (0 - Cs) + Cd`, material 2 `ZTST ALWAYS, ALPHA Cs + Cd`, both with FBMSK 0xFFFF00FF (only G written) and COLCLAMP 0 (wrap).
  Net per pixel: +2 per front face **behind** the scene surface, -2 per back face behind it = Carmack's-reverse **Z-fail**, in steps of 2 so bit 0 stays free.
- Frame sequence (hooks 0x16DA48 / 0x16DD58): (a) save the current G into the *other* buffer's alpha (CT16 "texture shuffle" sprites); (b) init G = 0x7E
  (kind 7; a following kind-8 pass uses 0x3E and keeps bit 7); volumes: inside one kind-7 volume ends at 0x80, inside a kind-8 volume sets bit 6;
  (c) kind 8: G bit 0 <- bit 6; (d) dest alpha bits 0/7 <- G bits 0/7; (e) restore G from the other buffer's alpha; (f) **apply**: full-screen sprites
  sampling the framebuffer as PSMT8H (index = alpha byte) through a CLUT at block 0x1800, `pixel = lerp(pixel, CLUT[A].rgb, CLUT[A].a / 128)`.
- The CLUT (`BuildSwBackgroundGifs` 0x1EA9F0, D_003E1330 = g_sw+0x3EA0): for index n, `g = max((n & 0x7F) - 1, 0) / 126`; `n & 0x81`: 0 = not shadowed,
  0x80 = in kind-7 volume (colour[0]), 0x01 = kind 8 (colour[1]), 0x81 = both (colour[2]); alpha = g * gain[i] * 128; colours/gains per world (g_sw+0x3E64).
  The low 7 bits come from the colour-1 layer alpha `128 - 127u`, so **shadows darken a surface in proportion to its direct light**; ambient-only surfaces
  are not darkened `[C:static]` path, artistic intent `[H]`.
- Volumes with subtype 2 prepend a per-SO reference chain from the shader; subtype 3 uploads fNear (camera inside the bounding sphere) for near-plane handling `[H]`.

PORT: a real stencil buffer. Draw volumes stencil-only with Z-fail (front faces increment on depth fail, back faces decrement), kind 7 and kind 8 in separate
bits; then one full-screen pass: `shadowType = (s7 != 0) | (s8 != 0) << 1`, colour/alpha from the 3-entry table, alpha scaled by the lit fraction (write
`128 - 127u` into an extra render target during the mesh pass). Drop the alpha save/restore and the CT16 shuffles. A GS dump (rendering/EXPERIMENTS E2/E3)
is the remaining end-to-end check of the face-sign convention.

## 8. Blips, HUD, screen effects

- **Blips** (prog1, kind 12): one particle loop in three variants (0x0, 0x390, 0x870); per particle t = now - birth; curves = coefficient vectors . (1, t, t^2);
  4 corners in clip space; flip-book table; colour key lerp; one XGKICK per batch `[C:static]`, curve meanings `[H]`.
- **HUD** (prog2, kind 21): constant packets plus a 2-D batch draw (0x140); V2_32 positions, V3_8/V4_8 colours, V2_16 UVs. PORT: a 2-D sprite renderer.
- **Screen tint/flash/fade**: CID116 (0x74): k14 over the world (colour lerp over 3 durations), k1 behind it. **Clears**: `BuildClearGifs` 0x16F960 modes 3
  colour+Z, 2 Z only, 1 colour only. **Background gradient**: `BuildSwBackgroundGifs` when SW+0x3EA8. **Frame blend**: `BlendPrevFrame`.
- **Anaglyph stereo** (SW+0x3E38 = D_003E12C8): one eye per FRM (`BuildCmEyeViewProj` 0x14F768), FBMSK masks per eye, every colour through
  `ColorCorrectVector` 0x1ED2D8 (desaturation), frame blend off. This is the only use of the "colour correction" flag; whether retail turns it on is unknown
  (low priority for a port; experiment E8) `[C:static]`.
- **Particles**: RIPs have no RPL of their own; `RenderRip` 0x1CF518 positions the rip's LO and draws through the SO globset path `[H]`.

## 9. Textures, CLUTs and GS memory (shd.cpp 0x1EA568, gs.cpp 0x16F2D0) `[C:static]`

- Level textures are loaded per world entry by `LoadShadersFromBrx` 0x1EB748 (CLUTs, bitmaps, SAAs, shaders + an image blob); layout in
  [formats/textures.md](../formats/textures.md). Records live in D_003DCFF4/D_003DCFF0 (BMP 0x28 B) and D_003DCFFC/D_003DCFF8 (CLUT 0x20 B); both are
  zeroed after the load (`ResetShaders` 0x1EC7A0, `ClearTexHashTables`), so RAM dumps cannot show them `[C:runtime]`.
- `FBuildUploadBitmapGifs` 0x1ECC28 allocates GS blocks (`IgsAllocGsb` 0x170548, bucketed free lists) and builds `BuildBmpTex0`/`BuildClutTex2` + image
  transfers; shared textures are deduplicated by two hash tables (D_003DD010 bitmaps, D_003DD018 CLUTs).
- Class 1 bitmaps go to GSB0 once per level (`UploadBitmaps` 0x170730). Class 3 (`UploadPermShaders` 0x1ECE58: the sky's) go to GSB1 (0x1000-0x1800), which
  overlaps the Z buffer, and are re-uploaded every frame at the start of P3, followed by a Z-only clear: textures parked in Z memory until the world writes
  depth `[H]` (strong). PORT: keep every texture resident; honour the Z clears.
- Shader animations (SAA family CID 0x8A-0x90: LOOP, PINGPONG, SHUFFLE, EYES, SCROLLER, CIRCLER) switch bitmap frames / CLUTs (`UpdateShaders` from UpdateSw).
  If they rewrite CLUTs, palette lookup in the shader is simpler than pre-expanded RGBA.
- `InitGsb` 0x1703F8 (was `InitZpr`), `AllocStack__4DMASi` 0x1572E0 (was `AllocSw`) (arbitration). DMA/VIF/GIF builder classes are in `src/game/engine/graphics/backend/dmas.cpp`,
  `vifs.cpp`, `gifs.cpp` (typed); `g_vifs` D_002DEA28, `g_gifs` D_002DEA00.

## 10. Models: GLOBSET at render time `[C:static]`

GLOBSET (SO+0x80): +0x10 centre, +0x20 radius, +0x38 csub, +0x3C sub descriptors (0x14 B), +0x40 subs (0x28 B), +0x44 per-sub spheres, +0x4C relight
stamp, +0x50 u64 grf (bit 33 expand cull sphere for shadow projection, bit 34 foreground). SUB grf: bits 0-2 subtype (0 mesh, 1 cel-border edges,
2 projected volume with SHD reference chain, 3 volume with near test), bits 3-7 authored kind, 0x100 prepend SHD call, 0x400 authored z, 0x800 hidden,
0x1000 absolute matrix. Culling: VU0 sphere test (model, then each sub) gives visibility 0..1 (used as fade) and the z key; skinned subs use
`FInsideCmMrd` 0x14F898. Per-object lights for dynamically lit objects come from `BuildSoLightPackets` (light.cpp). The file layout of GLOBSETs, the
sub-glob types (0 mesh, 1 outline/shadow data, 2/3 shader-bound strips) and scene placement are in [formats/meshes-scene.md](../formats/meshes-scene.md).

## 11. Port checklist

1. Decode level textures to RGBA8 (or keep palettes for SAA CLUT animation); load GLOBSETs as vertex/index buffers.
2. Compute relight colours A/B on the CPU whenever lights/ambient change; keep B.a.
3. Draw in kind order with the bucket sort rules; map TEST/ALPHA/FOGCOL per material layer to pipeline state; reversed-Z.
4. Per-vertex fog from the fit parameters; fade alpha from DrawGlobset.
5. Outlines as silhouette-tested thin quads; shadow volumes with a real stencil and the 3-entry shadow table.
6. HUD as a 2-D sprite pass; ignore VU double buffering, GSB aliasing, CT16 shuffles and the debug hooks.

Shader sketches for all of the above: `vu_microcode/pseudo_shaders.glsl`.

## Devtools findings (v3, 2026-11-01; tool still in progress)

- render_lab (no README yet; out/S02/relight_check.json): relighting the S02 render list with the documented formula matches the game's own lit vertex
  colours for 5,797 matched vertices: colour A exact in 97.5 % (mean abs error 0.5-0.6 of 255), colour B within one step in 99.0 % `[C:runtime render_lab, preliminary]`.
  Treat as preliminary until the tool's README lands.
- Textures: no mip chains are stored (every bitmap has cb = w*h*bpp/8) `[C:parse texture_browser]`; see [formats/textures](../formats/textures.md#devtools-findings-v3-2026-11-01).

## 12. Source files

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x157200-0x1578F8 | dmas | high |  |
| 0x157A50-0x1585D8 | draw | medium | tu_table said render; was labelled render |
| 0x1607A0-0x1613A8 | frm | high |  |
| 0x166920-0x167310 | gifs | high |  |
| 0x167310-0x16F148 | glob | medium | matcher headers cite glob.c; was labelled ref |
| 0x16F2D0-0x1709A8 | gs | high |  |
| 0x1A3260-0x1A4E08 | light | high |  |
| 0x1B4438-0x1B4728 | ms | medium |  |
| 0x1BE398-0x1BF110 | tu_1BE398 | low | CID45 + GS render slots |
| 0x1CD5F0-0x1CDCD0 | render | high |  |
| 0x1D57A8-0x1D6848 | tu_1D57A8 | low | render-list cull + world sphere tree |
| 0x1EA568-0x1ED7A8 | shd | high | tu_table said gs; was labelled gs |
| 0x1ED7A8-0x1EE988 | shdanim | high |  |
| 0x1EF850-0x1EFA90 | sky | high |  |
| 0x226C10-0x227350 | vifs | high |  |
| 0x227350-0x2275B8 | vis | low | CID5 XFM + BSP zone |

## 13. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x1572E0](../../src/game/engine/graphics/backend/dmas.cpp) | `AllocStack__4DMASi` |  | 0.90 | existing | arbitration; replaces existing AllocSw__4DMASi |
| [0x157AE0](../../src/game/engine/graphics/pipeline/draw.cpp) | `CprplPartitionArpl` | Bucket-sorts the frame RPL array by kind (26 kinds, counting sort) into aprpl, z-sorts some buckets, returns end index | 0.80 | T1 | verified |
| [0x157FA0](../../src/game/engine/graphics/pipeline/draw.cpp) | `DrawSw` | Builds the frame VIF1 chain: partitions RPLs, emits frame/camera VU constants, then for each RPL kind-change calls the pass table begin/end hooks D_00 | 0.70 | T1 |  |
| [0x1607A0](../../src/game/engine/graphics/backend/frm.cpp) | `StartupFrame` | Render-thread startup: installs INTC 5 (VIF1) handler 0x167F78, creates the two frame semaphores (0,2)/(2,2), clears the two deferred-release DLs, sta | 0.85 | T1 |  |
| [0x160918](../../src/game/engine/graphics/backend/frm.cpp) | `EnsureVu1Code` | Appends a DMA REF chain that uploads the VU1 microprogram [pvStart,pvEnd) | 0.85 | T1 | verified |
| [0x160A98](../../src/game/engine/graphics/backend/frm.cpp) | `CloseFrame` | Finalizes the open frame: closes its VIF and GIF chains into g_pfrmOpen, moves the two deferred-release DLs into the frame, clears g_pfrmOpen and sign | 0.85 | T1 |  |
| [0x160B70](../../src/game/engine/graphics/backend/frm.cpp) | `FrameRenderLoop` | Render-thread body: wait for a closed frame, PrepareGsForFrameRender, rebuild frame packets, DMA VIF1 then GIF chains, optional BlendPrevFrame, signal | 0.90 | T1 | verified |
| [0x160C80](../../src/game/engine/graphics/backend/frm.cpp) | `RenderFrame` | Synchronously renders one frame: PrepareGsForFrameRender, rebuild frame packets, send VIF chain, and GIF chain if fRenderGifs | 0.90 | T1 | verified |
| [0x160CE0](../../src/game/engine/graphics/backend/frm.cpp) | `ClearPendingFrame` | Releases objects whose destruction was deferred until the frame finished: calls vtable slot 2 (Decref) on every LO in frm->dl34 and frees the pool blo | 0.75 | T1 |  |
| [0x160E68](../../src/game/engine/graphics/backend/frm.cpp) | `FlushFrames` | Drains the render thread (wait/signal sema twice), optionally re-renders the last frame 3 times to both buffers and clears pending frames | 0.90 | T1 |  |
| [0x160F98](../../src/game/engine/graphics/backend/frm.cpp) | `BlendPrevFrame` | Draws the other (previous) framebuffer back over the current one as 32-px textured strips with alpha 0x64/0x80 (motion-blur/frame blend), unless disab | 0.85 | T1 | verified |
| [0x167320](../../src/game/engine/graphics/pipeline/glob/LoadGlobsetFromBrx__FP7GLOBSETP2SOP18CBinaryInputStream.cpp) | `LoadGlobsetFromBrx` | Reads an SO model (GLOBSET): bounds, shader animations (SAA), sub-globs and their per-type geometry | 0.95 | T1 | parse, verified |
| [0x167870](../../src/game/engine/graphics/pipeline/glob/func_00167870.c) | `LoadPoseSetsFromBrx` | Reads groups of up to 4 distance/LOD records (kind, position, radius, limit<=1e10, count) per globset descriptor | 0.75 | T1 | parse, format-doc |
| [0x167E58](../../src/game/engine/graphics/pipeline/glob/StoreRelightColors.c) | `StoreRelightColors` | Relight readback: converts the two RGBA results per vertex in VU1 data memory to RGBA5551 into the glob's EE colour buffers (+alpha bytes) | 0.65 | T1 |  |
| [0x167FC0](../../src/game/engine/graphics/pipeline/glob/BeginRpRelight.cpp) | `BeginRpRelight` | Pass 4 begin (kind 0): upload relight microprogram D_0025E090..D_0025E668 (compute only), BASE 0x32 OFFSET 0, upload up to 256 lights x 3 qw | 0.70 | T1 | verified |
| [0x168638](../../src/game/engine/graphics/pipeline/glob/RenderSoGlobset.cpp) | `RenderSoGlobset` | Per-model draw: sphere-culls each sub-model against the camera via VU0 microprogram 0x1C0, updates shader animations, builds/queues VIF packets (0x168 | 0.60 | T1 | verified |
| [0x169AC8](../../src/game/engine/graphics/pipeline/glob/LoadSubglobInstanceFromBrx.cpp) | `LoadSubglobInstanceFromBrx` | Reads one sub-glob instance: transform owner lookup (vtable slot 51), matrix, flags, optional binding table, relocates packet data | 0.75 | T1 | parse |
| [0x169EB8](../../src/game/engine/graphics/pipeline/glob/CopyRelocateDmaChain.c) | `CopyRelocateDmaChain` | Copies a VIF/DMA packet into the SW heap and optionally relocates DMA REF addresses (tag id 4) by a delta | 0.60 | T1 | verified |
| [0x16A000](../../src/game/engine/graphics/pipeline/glob/func_0016A000.cpp) | `LoadSubglobType0FromBrx` | Sub-glob type 0 loader: textured lit triangle strips (the visible mesh); builds VIF batches | 0.85 | T1 | parse, format-doc |
| [0x16C8E0](../../src/game/engine/graphics/pipeline/glob/func_0016C8E0.cpp) | `LoadClutsetFromBrx` | Reads a u16-counted table of vertex-binding groups (u16/u16/u8 records) and fixes up indices (0x1ED1A8) | 0.75 | T2 | parse, format-doc |
| [0x16CAA0](../../src/game/engine/graphics/pipeline/glob/LoadSubglobType2FromBrx.cpp) | `LoadSubglobType2FromBrx` | Sub-glob type 2 loader: U16 shader + vertices + colours; HYPOTHESIS cel/outline | 0.80 | T1 | parse, format-doc |
| [0x16CF78](../../src/game/engine/graphics/pipeline/glob/func_0016CF78.cpp) | `LoadSubglobType3FromBrx` | Sub-glob type 3 loader: U16 shader + indexed vectors + strip bits; HYPOTHESIS cel edge strips | 0.80 | T1 | parse, format-doc |
| [0x16DA48](../../src/game/engine/graphics/pipeline/glob/BeginRpProjVolume.cpp) | `BeginRpProjVolume` | Pass 6/7 begin (kinds 7/8): upload projected-volume microprogram D_0025C1C0..D_0025E040, build/kick (MSCAL 0x1AA0) the masked full-screen stencil-init | 0.60 | T1 |  |
| [0x16DD58](../../src/game/engine/graphics/pipeline/glob/EndRpProjVolume.cpp) | `EndRpProjVolume` | Pass 6/7 end: apply pass reading the framebuffer as an indexed texture (TEX2 D_003E1330) with alpha blend over full-screen strips (MSCAL 0x1C48) | 0.60 | T1 |  |
| [0x16E1A8](../../src/game/engine/graphics/pipeline/glob/BeginRpGlob.cpp) | `BeginRpGlob` | Pass 0 begin: upload main glob microprogram D_0025E680..D_00262320, zero the 11-qw batch descriptor at TOPS+0xA, MSCAL 0 | 0.75 | T1 |  |
| [0x16E228](../../src/game/engine/graphics/pipeline/glob/EndRpGlob.cpp) | `EndRpGlob` | Pass 0 end (shared by passes 8-10): zero batch descriptor, MSCAL 0x3AB0 (flush entry), FLUSHA | 0.75 | T1 |  |
| [0x16E438](../../src/game/engine/graphics/pipeline/glob/LoadSubglobType1FromBrx.cpp) | `LoadSubglobType1FromBrx` | Sub-glob type 1 loader: vertices + colours, no shader (GIF prims); HYPOTHESIS shadow/cel-no-shader | 0.80 | T1 | parse, format-doc |
| [0x16EA20](../../src/game/engine/graphics/pipeline/glob/BeginRpCelBorder.c) | `BeginRpCelBorder` | Pass 1 begin (kinds 9/17/20/24): upload the cel-border microprogram D_0025B880..D_0025C1B0 | 0.55 | T1 | verified |
| [0x170048](../../src/game/engine/graphics/backend/gs/RebuildFrameGifs__Fv.c) | `RebuildFrameGifs` | Rebuilds the three per-frame context packets (different FBMSK masks) and the framebuffer-texture packets after a buffer swap | 0.85 | T1 | verified |
| [0x1700D0](../../src/game/engine/graphics/backend/gs/StartupGs__Fv.c) | `StartupGs` | GS startup: crit-sect, sceGsResetGraph, double buffers, build clear packets and FBA packets, frame context packets, semaphores, VBlank/GS interrupt ha | 0.90 | T1 |  |
| [0x1703F8](../../src/game/engine/graphics/backend/gs/InitZpr__FP3ZPR3ZPKP2LO.cpp) | `InitGsb__FP3GSBii` |  | 0.85 | existing | arbitration; replaces existing InitZpr__FP3ZPR3ZPKP2LO |
| [0x170548](../../src/game/engine/graphics/backend/gs/IgsAllocGsb__FP3GSBi.cpp) | `IgsAllocGsb` | Allocates c units of GS memory from a GSB: first-fit from the size-bucket free lists, else bump igsMac (padding to a 0x80 boundary if it would straddl | 0.85 | T1 | verified |
| [0x1A3360](../../src/game/engine/graphics/pipeline/light/InitLight.c) | `InitLight` | LIGHT vt7 Init: InitXfm, kind 0, slot -1, falloff 200/2000, angle 240, ... | 0.85 | T1 |  |
| [0x1A3400](../../src/game/engine/graphics/pipeline/light/PostLightLoad.c) | `PostLightLoad` | LIGHT vt15: PostLoLoad then RebuildLight | 0.80 | T1 |  |
| [0x1A34E0](../../src/game/engine/graphics/pipeline/light/OnLightRemove.c) | `OnLightRemove` | LIGHT vt22: LO Freeze then RemoveLightFromSw (free tree slot) | 0.85 | T1 | arbitration |
| [0x1A3518](../../src/game/engine/graphics/pipeline/light/OnLightAdd.c) | `OnLightAdd` | LIGHT vt23: LO Unfreeze then AddLightToSw (alloc slot, insert into light tree) | 0.85 | T1 | arbitration |
| [0x1A3968](../../src/game/engine/graphics/pipeline/light/RebuildLight.c) | `RebuildLight` | Recompute the light's shading parameters: ConvertAngleStrength, HSV->RGB color, FitLinearFunction falloff, position; then InvalidateLight | 0.75 | T1 |  |
| [0x1A4258](../../src/game/engine/graphics/pipeline/light/FSphereHitsSpheres.cpp) | `FSphereHitsSpheres` | True if a sphere overlaps any of a list of spheres (or the capsules between consecutive ones) | 0.85 | T1 | verified |
| [0x1B4438](../../src/game/engine/graphics/pipeline/ms.cpp) | `InitMs` | MS::Init (vt7): InitSo then clear flag bit 35 of +0x148 | 0.80 | T1 |  |
| [0x1B4478](../../src/game/engine/graphics/pipeline/ms.cpp) | `RenderMsGlobset` | MS::RenderSelf (vt19): draw globset +0x80 via 0x1D57A8/0x1D5808 path if +0xC8 else DrawGlobset(0x168638) | 0.75 | T1 |  |
| [0x1BEA88](../../src/game/engine/graphics/pipeline/tu_1BE398/func_001BEA88.c) | `LoadCid45FromBrx` | CID 45 vt14: LoadXfmFromBrx | 0.75 | T2 | parse, verified |
| [0x1BEE10](../../src/game/engine/graphics/pipeline/tu_1BE398/func_001BEE10.c) | `IRenderSlotTexture` | Table lookup D_002C0BF8[slot][n] or -1 | 0.30 | T2 | verified |
| [0x1CD5F0](../../src/game/engine/graphics/pipeline/render.cpp) | `FillScreenRect` | GIF sprite rectangle in screen space with RGBA (alpha-blended if a<0xFF) | 0.80 | T1 | verified |
| [0x1CDA80](../../src/game/engine/graphics/pipeline/render.cpp) | `RenderSw` | SW vt8: per-frame world render entry (called from main loop) | 0.85 | T1 | runtime |
| [0x1EA568](../../src/game/engine/graphics/pipeline/shd/RelocateSharedColorTables.c) | `RelocateSharedColorTables` | After shared-data load: for each color table/bitmap record flagged shared (b4==1) add the load offset to its data pointer and color-correct its entrie | 0.65 | T1 | parse, format-doc |
| [0x1EA8A8](../../src/game/engine/graphics/pipeline/shd/LoadClutFromBrx.c) | `LoadClutFromBrx` | Read one color table header (0x10) and colors; skip if its name fails the load filter; shared entries resolved via PclutFindHash; color-correct entrie | 0.80 | T1 | parse, format-doc |
| [0x1EA9F0](../../src/game/engine/graphics/pipeline/shd/func_001EA9F0.cpp) | `BuildSwBackgroundGifs` | Build a GS packet that draws a gradient/solid image from HSV colors into an allocated GS block (tex0 saved at g_sw+0x3EA0) | 0.30 | T2 |  |
| [0x1EAE40](../../src/game/engine/graphics/pipeline/shd/LoadColorTablesFromBrx__FP18CBinaryInputStream.c) | `LoadColorTablesFromBrx` | Read count, allocate 0x20-byte CLUT array (D_003DCFFC), LoadClutFromBrx each, track shared count | 0.85 | T1 | parse, format-doc |
| [0x1EAF50](../../src/game/engine/graphics/pipeline/shd/LoadBmpFromBrx.c) | `LoadBmpFromBrx` | Read one bitmap header (0x18) and pixels (filtered by name; shared via PbmpFindHash) | 0.80 | T1 | parse, format-doc |
| [0x1EB098](../../src/game/engine/graphics/pipeline/shd/LoadBitmapsFromBrx__FP18CBinaryInputStream.c) | `LoadBitmapsFromBrx` | Read count, allocate 0x28-byte BMP array (D_003DCFF4), LoadBmpFromBrx each | 0.85 | T1 | parse, format-doc |
| [0x1EB1A8](../../src/game/engine/graphics/pipeline/shd/LoadTexFromBrx.c) | `LoadTexFromBrx` | Read a TEX: header then bitmap indices (into D_003DCFF4) and clut indices (0xFFFF = none) | 0.75 | T1 | parse |
| [0x1EB2F0](../../src/game/engine/graphics/pipeline/shd/LoadShdFromBrx.c) | `LoadShdFromBrx` | Read one shader: header, colors (color-corrected), texture list (LoadTexFromBrx) and per-frame data | 0.80 | T1 | parse, format-doc |
| [0x1EB748](../../src/game/engine/graphics/pipeline/shd/LoadShadersFromBrx.c) | `LoadShadersFromBrx` | Load color tables, bitmaps, shaders and shader animations (PsaaLoadFromBrx 0x1ED7A8); upload permanent textures (0x1ECE58); build shader packets | 0.80 | T1 | parse, format-doc |
| [0x1EC820](../../src/game/engine/graphics/pipeline/shd/PshdFindShader.c) | `PshdFindShader` | Find shader by oid in D_003DCFEC[D_003DCFE8] (stride 0x2C); -1 -> NULL | 0.75 | T1 | format-doc |
| [0x1EC8F8](../../src/game/engine/graphics/pipeline/shd/func_001EC8F8.c) | `PshdFromIshd` | &D_003DCFEC[i] (stride 0x2C) | 0.85 | T1 | verified |
| [0x1EC9D0](../../src/game/engine/graphics/pipeline/shd/BuildClutTex2__FP4CLUTi.c) | `BuildClutTex2` | Fill TEX2-style CLUT fields | 0.90 | T1 | verified, format-doc |
| [0x1ECAB0](../../src/game/engine/graphics/pipeline/shd/BuildClutGifs__FP4CLUTiP4GIFS.cpp) | `BuildClutGifs__FP4CLUTiP4GIFS` | existing name kept | 0.85 | existing | verified |
| [0x1ECB18](../../src/game/engine/graphics/pipeline/shd/BuildBmpTex0__FP3BMPi.cpp) | `BuildBmpTex0__FP3BMPi` | existing name kept | 0.85 | existing | verified |
| [0x1ECC28](../../src/game/engine/graphics/pipeline/shd/FBuildUploadBitmapGifs__FiP3GSBP4GIFS.c) | `FBuildUploadBitmapGifs` | Upload CLUTs/BMPs of kind k into a GS block; returns whether anything was added | 0.80 | T1 | verified |
| [0x1ECE58](../../src/game/engine/graphics/pipeline/shd/UploadPermShaders.c) | `UploadPermShaders` | Build and send DMA uploading the permanent (always-resident) bitmaps/CLUTs | 0.65 | T1 | format-doc |
| [0x1ED7A8](../../src/game/engine/graphics/pipeline/shdanim/func_001ED7A8.c) | `PsaaLoadFromBrx` | Read an SAA class index, construct it via the class table factory (D_002BAAE8[cid].pfnNew), set owner (+0x30), read 0x20-byte SAAF and call vt7 Init(p | 0.85 | T1 | parse, verified, format-doc |
| [0x1EDB98](../../src/game/engine/graphics/pipeline/shdanim/InitLoop.c) | `InitLoop` | LOOP vt7 Init | 0.85 | T1 | verified |
| [0x1EF8C8](../../src/game/engine/graphics/pipeline/sky.cpp) | `UpdateSky` | SKY vt16: UpdateSo, then TranslateToPos(camera pos D_002DDFA0, keeping own Z unless fFollowZ +0x150) | 0.85 | T1 | verified |
| [0x227350](../../src/game/engine/graphics/pipeline/vis.cpp) | `InitCid5` | InitXfm; state 0x110 = -1 | 0.60 | T2 | verified |
| [0x227380](../../src/game/engine/graphics/pipeline/vis.cpp) | `LoadCid5ZoneFromBrx` | XFM XF read 0x231768, U32, ReadGeom, ReadBspc, LoadOptionsFromBrx | 0.75 | T2 | parse |

Selected 65 of 271 named functions (T1 with confidence >= 0.75, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/rendering.md](../reference/functions/rendering.md).

## Open questions
- End-to-end GS check of the shadow-volume face-sign convention (E2/E3) and the TEX2 CLUT contents.
- Producers of kinds 2, 8, 11, 13 and which models set the foreground bit 34 (E1, E5).
- When the render-to-texture slots are used (E6); whether anaglyph stereo is reachable in retail (E8).
- Blip curve meanings and the three variants; the B-family optional stages' exact uses.
