# Rendering

How a frame becomes pixels: objects queue render-list entries, a sorter groups them into passes, pass hooks set up VU1
and the GS, and a render thread ships the DMA chains. For readers new to the PS2: the EE (main CPU) builds DMA chains;
**VIF1** unpacks data into the **VU1** vector unit, whose microprograms transform vertices and emit **GIF** packets to
the **GS** (rasterizer with 4 MB of its own memory). Register packets can also go to the GS directly over the GIF.

Related: [camera](camera.md) (view matrices), [object-model](object-model.md) (RenderSelf), [particles-effects](particles-effects.md).

## 1. Per-frame flow

| Step | Function | What | Tag |
|---|---|---|---|
| open | `OpenFrame` 0x160828 | start this frame's VIF/GIF chains in the open FRM (`g_pfrmOpen` D_002DEA4C; two FRMs) | [H r3 frm.md] |
| render | `RenderSw` 0x1CDA80 (SW slot 8) | bump SW+0x3D60; vt19 `RenderSelf` on live objects with `(grf & 0x88) == 8`; objects queue RPLs via 0x1CD998 | [C:runtime] counter; [H r5 render] |
| draw | `DrawSw` 0x157FA0 (SW slot 9) | partition RPLs (`CprplPartitionArpl` 0x157AE0), emit frame/camera VU constants, call pass hooks, emit each RPL's DMA CALLs | [H r3 draw.md] |
| UI | `DrawUi` 0x1D9820 | flush HUD lists + GIF/VIF (existing symbol `UpdateRecyclable__3CGc` is wrong) | [H r6 SUMMARY, r4] |
| close | `CloseFrame` 0x160A98 | `FinalizeFrameVifs/Gifs`, move deferred-release lists into the frame, signal the render thread | [H r3 frm.md] |
| GPU | `FrameRenderLoop` 0x160B70 (thread) | wait, `PrepareGsForFrameRender`, rebuild frame packets, DMA VIF1 then GIF, optional `BlendPrevFrame`, `SwapGsBuffers` | [H r3 frm.md] |

`FlushFrames` 0x160E68 drains the thread (used by level transitions); `RenderFrame` 0x160C80 renders synchronously;
`ClearPendingFrame` 0x160CE0 releases objects whose destruction waited for the GPU (vt2 Decref) `[H r3 frm.md]`.

## 2. Render list and passes

- **RPL** = 16 bytes `{int kind; float z; u32 qwCall1; u32 qwCall2}`, arrays D_003B3980..D_003D4A50, max 0x2000 `[H r3 draw.md]`.
- `DrawSw` bucket-sorts by kind (26 kinds, counting sort), z-sorts some buckets (`NCmpPrplZ`, `NCmpPrplReverseZ`),
  maps kind -> pass via D_002BDD20, and when the pass changes calls `D_002BDD88[pass].pfnBegin/pfnEnd(VIFS*, kind, kindOther)` `[H r3 draw.md]`.
  This table is **the main extension point for a port's renderer**.

| Pass | Begin / End | Known role | Tag |
|---|---|---|---|
| 0 | 0x16E1A8 / 0x16E228 | main VU1 microprogram (D_0025E680..D_00262320), clear VU constants, MSCAL | [H r3 glob.md] |
| 1 | 0x16EA20 / 0x16EA48 | second VU1 program (D_0025B880..) | [H] |
| 2 | 0x13A830 / 0x13A858 | (in blip.cpp: particle sprites?) | [H] |
| 3 | 0x16E298 / 0x16E380 | bitmap-upload DMA calls | [H] |
| 4 | 0x167FC0 / 0x1681B0 | per-object VU upload of up to 256 class-0x505 objects | [H] |
| 5 | 0x211A80 / 0x211C10 | (hud TU) | [H] |
| 6-7 | 0x16DA48 / 0x16DD58 | screen effects (framebuffer-as-texture) | [H] |
| 8-9 | 0x1BE990, 0x1BE9E8 / 0x16E228 | (tu_1BE398) | [H] |
| 10 | 0x157CF0 / 0x16E228 | re-emit frame camera state | [H] |
| 11 | 0x16E3C8 / 0x16E400 | FBA alpha-correction on/off packets | [H] |

## 3. Models: GLOBSET (glob.cpp, 0x167310-0x16F148)

Each SO has a GLOBSET (+0x80 in SO): bounds, shader animations (SAA), and sub-globs of several geometry types.
`LoadGlobsetFromBrx` 0x167320 (Lilly-matched) reads it; `BindGlobset` resolves bones; `CloneGlobset` copies for cloned SOs
`[C:verified]` (LoadGlobsetFromBrx), rest `[H r3 glob.md]`. Drawing (`DrawGlobset` 0x168638, matcher header): sphere-cull each
sub-model on **VU0** (microprogram at 0x1C0), update SAAs, build a VIF packet per sub-model (DMA CNT + UNPACK to VU1 0x301
+ RET) and queue it as an RPL. Bone/skin matrices are uploaded separately (0x1684D0). The VIF1/VU1 interrupt handler
0x167F78 reads VU1 results back (colour to 1555) `[H r3 glob.md]`. MS (CID 0x31) is the static-mesh SO whose render slot
calls a scene cull `CullSceneRenderList` 0x1D5808 `[H r5 ms.md, r6]`.

## 4. DMA, VIF, GIF helper classes

`src/P2/dmas.cpp`, `vifs.cpp`, `gifs.cpp` are fully typed and named (DMAS, VIFS, GIFS builder classes) `[C:static]`.
Globals: `g_vifs` D_002DEA28, `g_gifs` D_002DEA00 (constructed in frm.cpp's static init) `[H r3 frm.md]`. Note
`AllocSw__4DMASi` 0x1572E0 is really `AllocStack__4DMASi` (allocates from the stack heap) `[C:verified]` (verify disputes).

## 5. GS setup and GS memory (gs.cpp, 0x16F2D0-0x1709A8)

`StartupGs` 0x1700D0: reset, double buffers, clear/FBA/frame-context packets, VBlank semaphores, interrupt handlers
`[H r3 gs.md]`. GS memory is allocated from a GSB with **bucketed free lists** (`IgsAllocGsb` 0x170548, new vs Sly 1's
bump allocator) `[C:verified]`. `UploadBitmaps` 0x170730 sends every level bitmap/CLUT to GS memory; fonts are uploaded to
a GSB at 0x1000..0x1800 `[H r3 gs.md, font.md]`. Fix: `InitZpr` at 0x1703F8 is really `InitGsb`.

## 6. Textures, shaders, colour (shd.cpp 0x1EA568, shdanim.cpp 0x1ED7A8)

Level textures: `LoadShadersFromBrx` (called by LoadSwFromBrx) loads colour tables, bitmaps, textures and shaders, then
shader animations; GIF packets via `BuildClutGifs`, `BuildBmpTex0` (verified Sly 1 gs.c builders) `[H r6b shd.md]`.
New in Sly 2: shared textures through two 0x3FFD-slot hash tables (D_003DD010 bitmaps, D_003DD018 CLUTs), and a global
**colour-correction pass** (`ColorCorrectRgba/Vector`, flag D_003E12C8) applied to every loaded colour, fog colour and light `[H r6b shd.md]`.
Shader animations (SAA family CID 0x8A-0x90: LOOP, PINGPONG, SHUFFLE, EYES, SCROLLER, CIRCLER) are ticked by `UpdateShaders` from UpdateSw `[H r6b shd.md]`.

## 7. Lights (light.cpp 0x1A3260)

LIGHT = CID 0x4 (254 instances in the Paris hub) `[C:runtime]`. Lights live in a plane tree with a 256-slot bitmap;
each object gets its 4 best lights as VIF packets (`BuildSoLightPackets`). OnAdd/OnRemove register lights with the SW `[H r4b light.md]`.

## 8. Visibility helpers

- Sphere tree (tu_1D57A8): SOs register bounding spheres in a 4-ary tree on OnAdd/OnRemove and busy changes (LO+0x140/+0x144) `[H r6 small_1D57A8]`.
- CID 0x5 (XFM + BSP volume, "vis zone"?) queried by camera code 0x14E078 `[H r8 vis_cid5.md]`.
- Frame blend: `BlendPrevFrame` 0x160F98 draws the previous framebuffer back as 32-px strips (motion blur), unless D_002DEB24/D_003E12C8 disable it `[H r3 frm.md]`.

## 9. Source files

| Span | File (label) | Notes |
|---|---|---|
| 0x157200-0x1578F8 | dmas |  |
| 0x157A50-0x1585D8 | draw | was labelled render |
| 0x1607A0-0x1613A8 | frm |  |
| 0x166920-0x167310 | gifs |  |
| 0x167310-0x16F148 | glob | was labelled ref |
| 0x16F2D0-0x1709A8 | gs |  |
| 0x1A3260-0x1A4E08 | light |  |
| 0x1B4438-0x1B4728 | ms |  |
| 0x1BE398-0x1BF110 | tu_1BE398 (GS texture setup + cid 0x2D) |  |
| 0x1CD5F0-0x1CDCD0 | render |  |
| 0x1D57A8-0x1D6848 | tu_1D57A8 (cull + sphere tree) |  |
| 0x1EA568-0x1ED7A8 | shd | was labelled gs |
| 0x1ED7A8-0x1EE988 | shdanim |  |
| 0x1EF850-0x1EFA90 | sky |  |
| 0x226C10-0x227350 | vifs |  |
| 0x227350-0x2275B8 | vis? (cid 5) |  |

## 10. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x157AE0](../../src/hand/func_00157AE0.c) | `CprplPartitionArpl` | Bucket-sorts the frame RPL array by kind (26 kinds, counting sort) into aprpl, z-sorts some buckets, returns end index | 0.80 | VERIFIED |
| [0x157FA0](../../src/hand/func_00157FA0.c) | `DrawSw` | Builds the frame VIF1 chain: partitions RPLs, emits frame/camera VU constants, then for each RPL kind-change calls the pass table begin/end hooks D_00 | 0.70 | HYP |
| [0x1607A0](../../src/hand/func_001607A0.cpp) | `StartupFrame` | Render-thread startup: installs INTC 5 (VIF1) handler 0x167F78, creates the two frame semaphores (0,2)/(2,2), clears the two deferred-release DLs, sta | 0.85 | HYP |
| [0x160918](../../src/hand/func_00160918.cpp) | `EnsureVu1Code` | Appends a DMA REF chain that uploads the VU1 microprogram [pvStart,pvEnd) | 0.85 | VERIFIED |
| [0x160A98](../../src/hand/func_00160A98.cpp) | `CloseFrame` | Finalizes the open frame: closes its VIF and GIF chains into g_pfrmOpen, moves the two deferred-release DLs into the frame, clears g_pfrmOpen and sign | 0.85 | HYP |
| [0x160B70](../../src/hand/func_00160B70.cpp) | `FrameRenderLoop` | Render-thread body: wait for a closed frame, PrepareGsForFrameRender, rebuild frame packets, DMA VIF1 then GIF chains, optional BlendPrevFrame, signal | 0.90 | VERIFIED |
| [0x160C80](../../src/hand/func_00160C80.c) | `RenderFrame` | Synchronously renders one frame: PrepareGsForFrameRender, rebuild frame packets, send VIF chain, and GIF chain if fRenderGifs | 0.90 | VERIFIED |
| [0x160CE0](../../src/hand/func_00160CE0.cpp) | `ClearPendingFrame` | Releases objects whose destruction was deferred until the frame finished: calls vtable slot 2 (Decref) on every LO in frm->dl34 and frees the pool blo | 0.75 | HYP |
| [0x160E68](../../src/hand/func_00160E68.cpp) | `FlushFrames` | Drains the render thread (wait/signal sema twice), optionally re-renders the last frame 3 times to both buffers and clears pending frames | 0.90 | HYP |
| [0x160F98](../../src/hand/func_00160F98.cpp) | `BlendPrevFrame` | Draws the other (previous) framebuffer back over the current one as 32-px textured strips with alpha 0x64/0x80 (motion-blur/frame blend), unless disab | 0.80 | VERIFIED |
| [0x167320](../../src/hand/func_00167320.cpp) | `LoadGlobsetFromBrx` | Reads an SO model (GLOBSET): bounds, shader animations (SAA), sub-globs and their per-type geometry | 0.90 | VERIFIED |
| [0x168638](../../src/hand/func_00168638.cpp) | `DrawGlobset` | Per-model draw: sphere-culls each sub-model against the camera via VU0 microprogram 0x1C0, updates shader animations, builds/queues VIF packets (0x168 | 0.50 | PLAUSIBLE |
| [0x170048](../../src/hand/func_00170048.c) | `RebuildFrameGifs` | Rebuilds the three per-frame context packets (different FBMSK masks) and the framebuffer-texture packets after a buffer swap | 0.55 | VERIFIED |
| [0x1700D0](../../src/hand/func_001700D0.c) | `StartupGs` | GS startup: crit-sect, sceGsResetGraph, double buffers, build clear packets and FBA packets, frame context packets, semaphores, VBlank/GS interrupt ha | 0.90 | HYP |
| [0x1703F8](../../src/hand/InitZpr__FP3ZPR3ZPKP2LO.cpp) | `InitZpr__FP3ZPR3ZPKP2LO` |  |  | existing (dispute won); fix: InitGsb__FP3GSBii |
| [0x170548](../../src/hand/func_00170548.cpp) | `IgsAllocGsb` | Allocates c units of GS memory from a GSB: first-fit from the size-bucket free lists, else bump igsMac (padding to a 0x80 boundary if it would straddl | 0.80 | VERIFIED |
| [0x1A3360](../../src/hand/func_001A3360.c) | `InitLight` | LIGHT vt7 Init: InitXfm, kind 0, slot -1, falloff 200/2000, angle 240, ... | 0.85 | HYP |
| [0x1A3400](../../src/auto/func_001A3400.c) | `PostLightLoad` | LIGHT vt15: PostLoLoad then RebuildLight | 0.80 | HYP |
| [0x1A34E0](../../src/auto/func_001A34E0.c) | `OnLightRemove` | LIGHT vt22: LO Freeze then RemoveLightFromSw (free tree slot) | 0.60 | VERIFIED (arbitration/conflict fix; was LIGHT::Freeze) |
| [0x1A3518](../../src/auto/func_001A3518.c) | `OnLightAdd` | LIGHT vt23: LO Unfreeze then AddLightToSw (alloc slot, insert into light tree) | 0.60 | VERIFIED (arbitration/conflict fix; was LIGHT::Unfreeze) |
| [0x1A3968](../../src/hand/func_001A3968.c) | `RebuildLight` | Recompute the light's shading parameters: ConvertAngleStrength, HSV->RGB color, FitLinearFunction falloff, position; then InvalidateLight | 0.75 | HYP |
| [0x1B4438](../../src/hand/func_001B4438.c) | `InitMs` | MS::Init (vt7): InitSo then clear flag bit 35 of +0x148 | 0.80 | HYP |
| [0x1B4478](../../src/hand/func_001B4478.c) | `RenderMsGlobset` | MS::RenderSelf (vt19): draw globset +0x80 via 0x1D57A8/0x1D5808 path if +0xC8 else DrawGlobset(0x168638) | 0.75 | HYP |
| [0x1CD5F0](../../src/hand/func_001CD5F0.cpp) | `FillScreenRect` | GIF sprite rectangle in screen space with RGBA (alpha-blended if a<0xFF) | 0.80 | VERIFIED |
| [0x1CDA80](../../src/hand/func_001CDA80.c) | `RenderSw` | SW vt8: per-frame world render entry (called from main loop) | 0.50 | HYP |
| [0x1EAE40](../../src/hand/func_001EAE40.c) | `LoadColorTablesFromBrx` | Read count, allocate 0x20-byte CLUT array (D_003DCFFC), LoadClutFromBrx each, track shared count | 0.75 | HYP |
| [0x1EB098](../../src/hand/func_001EB098.c) | `LoadBitmapsFromBrx` | Read count, allocate 0x28-byte BMP array (D_003DCFF4), LoadBmpFromBrx each | 0.75 | HYP |
| [0x1EC9D0](../../src/hand/func_001EC9D0.c) | `BuildClutTex2` | Fill TEX2-style CLUT fields | 0.85 | VERIFIED |
| [0x1ECAB0](../../src/hand/BuildClutGifs__FP4CLUTiP4GIFS.cpp) | `BuildClutGifs` | existing name kept | 0.80 | VERIFIED |
| [0x1ECB18](../../src/hand/BuildBmpTex0__FP3BMPi.cpp) | `BuildBmpTex0` | existing name kept | 0.80 | VERIFIED |
| [0x1ECC28](../../src/hand/func_001ECC28.c) | `FBuildUploadBitmapGifs` | Upload CLUTs/BMPs of kind k into a GS block; returns whether anything was added | 0.80 | VERIFIED |
| [0x1EDB98](../../src/hand/func_001EDB98.c) | `InitLoop` | LOOP vt7 Init | 0.60 | VERIFIED |
| [0x1EF8C8](../../src/hand/func_001EF8C8.c) | `UpdateSky` | SKY vt16: UpdateSo, then TranslateToPos(camera pos D_002DDFA0, keeping own Z unless fFollowZ +0x150) | 0.85 | VERIFIED |

Selected 33 of 247 named functions (confidence >= 0.75, verified, or flagged). Full list: [reference/functions/rendering.md](../reference/functions/rendering.md).

## Open questions
- Meaning of each of the 26 RPL kinds and the 12 passes. Experiment: break 0x157FA0, dump D_003D4A50-D_003B3980 and the
  sorted order; patch a kind's pass to 0 and see what disappears; no-op pass 11 to find FBA effects.
- Numbering and dispatch of the sub-glob type loaders 0x16CAA0/0x16CF78/0x16E438 (probably a jump table).
- Which class is "0x505" in pass 4 (lights/shadows?).
- D_003E12C8 colour correction: option or per-level flag?
