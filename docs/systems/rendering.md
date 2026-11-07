# Rendering

How a frame gets from live objects to pixels: render lists, globsets (models), shaders and textures, the VU1/GS/DMA
backend, lighting ("relight"), particles and screen effects, and render targets. For the frame loop as a whole see
[engine.md section 4](../engine.md#4-the-frame-loop); for the pass table and vertex formats see
[engine.md section 7.3](../engine.md#73-rendering-enginegraphics); for byte layouts see
[formats/meshes-scene.md](../formats/meshes-scene.md) and [formats/textures.md](../formats/textures.md). The port plan's
render section ([guides/port-plan.md](../guides/port-plan.md#6-rendering--metal)) builds on this page.

## 1. Purpose and shape

Rendering is split into two directories under `src/game/engine/graphics/`:

- **`pipeline/`** is the game-side renderer: which objects draw, in what order, with which model, shader and lights.
  It knows about SOs, globsets, the camera and the world, and it emits *packets* (DMA chains) rather than draw calls.
- **`backend/`** is the PS2 plumbing: DMA chain builders (`DMAS`, `VIFS`, `GIFS`), the GS (display/draw environments,
  GS memory allocation, uploads) and the frame system (`FRM`) that hands finished frames to a render thread.

Particles and effects live in `src/game/engine/fx/`, the HUD pass in `src/game/engine/presentation/hud.cpp`, and the
camera (view/projection, frustum, fog parameters) in `src/game/engine/camera/` (see [camera.md](camera.md)).

PS2 primer, only what this page needs: the EE (main CPU) builds DMA chains in RAM; the **VIF1** channel unpacks data into
**VU1** memory and starts VU1 **microprograms**, which transform vertices and XGKICK finished **GIF** packets to the **GS**
rasteriser. The GS has 4 MB of local memory for framebuffers, Z and textures, a blend equation `(A-B)*C+D`, and **no
stencil buffer**. GIF packets can also go straight to the GS (PATH2 via VIF `DIRECT`, or PATH3 via the GIF channel), which
is how clears, texture uploads and 2-D effects are drawn.

## 2. Main types

| Type | What it is | Where |
|---|---|---|
| `RPL` | one render-list entry: kind (0..25), sort depth, up to two DMA CALL addresses | `pipeline/draw.cpp`, `pipeline/render.cpp` |
| `GLOBSET` / `GLOB` / `SUBGLOB` | an SO's model: a set of globs, each with sub-globs of a type (visible mesh, outline edges, shadow volume, ...) and prebuilt static VIF packets | `pipeline/glob.cpp` |
| `SHD`, `TEX`, `BMP`, `CLUT` | shader, its texture list, a bitmap, a colour table, all loaded per world entry | `pipeline/shd.cpp` |
| `SAA` (+ LOOP, PINGPONG, SHUFFLE, EYES, SCROLLER, CIRCLER) | shader animations: switch bitmap frames / CLUTs or scroll UVs | `pipeline/shdanim.cpp` |
| `LIGHT` | a level light (an XFM): colour, falloff and angle curves fitted into the VU1 lighting terms | `pipeline/light.cpp` |
| `MS` | static mesh: an SO with no fields of its own | `pipeline/ms.cpp` |
| `SKY` | the sky dome, an SO re-centred on the camera each frame | `pipeline/sky.cpp` |
| `VIS` | camera visibility zone (a BSP volume) that selects the camera's `grfvis` mask | `pipeline/vis.cpp` |
| RTCAM (class name not settled) | a camera object that renders a few objects into a 128x128 render target | the render-target unit in `pipeline/` (`InitRenderTargets`) |
| `DMAS` / `VIFS` / `GIFS` | DMA chain builder and its VIF1 and GIF packet subclasses | `backend/dmas.cpp`, `backend/vifs.cpp`, `backend/gifs.cpp` |
| `FRM` | one of the two double-buffered frames handed to the render thread | `backend/frm.cpp` |
| `GSB` | a GS-memory block allocator (size-bucket free lists plus a bump top) | `backend/gs.cpp` |
| `EMITTER`, `RIPG`/`RIP`, `BLIPG`/`BLIP`, `TRAIL`, `EXPL*` | particle emitters, mesh-particle groups, sprite-particle groups, trails, explosions | `src/game/engine/fx/` |

Relationships: an `SO` owns a `GLOBSET`; a sub-glob references `SHD`s; a `SHD` references `TEX` records, which index
`BMP` and `CLUT` arrays; `SAA`s hang off the globset and rewrite the shader's GS words when dirty. `LIGHT`s live in the
world's light table and a plane tree; the relight pass turns them into per-vertex colours stored back in the globset.

## 3. Lifecycle

### Boot

`StartupDma` (DMA channel handles), `StartupGs` (GS reset, double buffers, clear and context packets, vblank and GS
interrupt handlers), `StartupFrame` (VIF1 interrupt handler for the relight readback, frame semaphores, render thread),
`StartupBlips` (blip GIF packets) and `StartupFont` run from the startup table in `app/main.cpp`
([engine.md section 3](../engine.md#3-boot)).

### Level load

Driven by `LoadSwFromBrx` per world entry:

1. `LoadShadersFromBrx` reads colour tables, bitmaps, shaders and shader animations (`LoadSaaFromBrx`), deduplicates
   bitmaps and CLUTs by hash, allocates GS memory (`IgsAllocGsb`) and uploads permanent textures (`UploadPermShaders`,
   `UploadBitmaps`). Optional colour correction (`ColorCorrectRgba`) is applied to colours as they load.
2. Each SO's model comes from `LoadGlobsetFromBrx` (called by the SO loader). Sub-glob type 0, the visible mesh
   (`LoadSubglobType0FromBrx`), is turned into **static VIF batches at load time** with `VIFS::AddVifUnpack` reading
   straight from the stream. These static chains live for the whole level; per-frame work only CALLs them.
3. `BindGlobset` resolves bone references after load; `LIGHT` objects init and register into the light table
   (`AddLightToSw`); `EnsureSwDefaultLight` creates a default light when a level has none; `InitRenderTargets` allocates
   the two render-target textures.

Teardown is implicit: models and packets are in the world heap, which is reset on the next level load; `ResetShaders`
and `ClearTextureHashTables` clear the shader tables.

### Per frame

From `GameMain` (`app/main.cpp`), after the world update:

1. **`OpenFrame`** (`backend/frm.cpp`): wait for a free frame slot, make it the open frame, allocate its VIF and GIF
   packets.
2. **`RenderSw`** (`pipeline/render.cpp`, SW slot 8):
   - `PrepareSwLights` rebuilds the light tree if lights moved;
   - resets the render-list cursor (`AllocRenderListEntry` hands out entries from a per-frame buffer);
   - if the world's illumination stamp changed, `SubmitSwRelightAll` queues relighting for every lit SO;
   - calls **LO slot 19 (RenderSelf)** on every live object that wants rendering. The SO default, `RenderSoSelf`
     (`engine/physics/so.cpp`), checks `grfvis & g_cm.grfvis` and calls **`RenderSoGlobset`**: per sub-glob a VU0 sphere
     cull against the view (which also yields a distance fade), shader-animation update, then `BuildModelDrawPackets`
     builds the per-instance VIF packet (model matrix, bone/skin matrices, light packets from `BuildSoLightPackets`)
     and queues one `RPL` per sub-glob with its kind and sort depth;
   - other classes override RenderSelf (BLIPG, RIPG, WIDGET, RTCAM, ...).
3. **`DrawSw`** (`pipeline/draw.cpp`, SW slot 9): `CprplPartitionArpl` counting-sorts the RPLs into the 26 kinds and
   depth-sorts the blended kinds (`NCmpPrplReverseZ` far-to-near; `NCmpPrplZ` near-to-far for the HUD kind); it emits the
   frame and camera VU1 constants (viewport, world-to-clip, camera matrix, ambient, framebuffer words), then walks the
   entries in kind order. On every **pass** change it calls the old pass's end hook and the new pass's begin hook from
   the pass hook table, then appends each entry's DMA CALLs.
4. **`DrawUi`** (`engine/presentation/screen.cpp`): the screens' draw slots, then the HUD DMA streams.
5. **`CloseFrame`**: `FinalizeFrameVifs` / `FinalizeFrameGifs` append a GS FINISH and END, and the frame is signalled to
   the render thread.

The **render thread** (`FrameRenderLoop`) waits for a closed frame, DMAs its VIF1 chain then its GIF chain, optionally
draws the previous frame back over it (`BlendPrevFrame`, a frame blend), and swaps buffers (`SwapGsBuffers`, which
also handles the field / half-pixel offset of the interlaced display). So the CPU builds frame N+1 while the GS draws
frame N. `FlushFrames` drains it (used before a level load). `RenderFrame` is the synchronous variant.

## 4. Passes, kinds and the hook table

A **kind** is what an object asks for (opaque, cutout, translucent, outline, shadow volume A/B, blip, HUD, render
target slot, foreground...). A **pass** is a VU1 microprogram plus GS state. The kind-to-pass table and the pass hook
table (begin/end pairs) are data in `draw.cpp`; the kind/pass list is in
[engine.md section 7.3](../engine.md#73-rendering-enginegraphics).

The begin/end hooks, by name (all in `pipeline/glob.cpp` unless noted):

- `BeginRpGlob` / `EndRpGlob`: the main glob microprogram, shared by most opaque and translucent kinds;
- `BeginRpBackground` / `EndRpBackground`: sky and background; also uploads per-frame textures parked in Z-buffer memory
  and clears Z afterwards;
- `BeginRpCutout` / `EndRpCutout`: alpha-tested geometry, toggling GS `FBA` so cutout pixels mark destination alpha;
- `BeginRpRelight` / `EndRpRelight`: compute-only relight microprogram (no GS output);
- `BeginRpCelBorder` / `EndRpCelBorder`: outline edges;
- `BeginRpProjVolume`: shadow volumes (both channels), including the full-screen stencil-emulation strips;
- `BeginRpBlip` / `EndRpBlip` (`engine/fx/blip.cpp`): sprite particles;
- `BeginRpHud` / `EndRpHud` (`engine/presentation/hud.cpp`): widgets and text;
- `BeginRpRenderTarget0` / `BeginRpRenderTarget1` (render-target unit): render-to-texture slots;
- `BeginRpForeground` (`draw.cpp`): objects drawn over the HUD after a Z-only clear.

**The kind and pass tables are the main extension point.** A new visual category is a new kind mapped to an existing
or new pass; a native renderer hooks the pass begin/end functions to know which batches belong to which pass.

## 5. Lighting ("relight")

Lighting is **not** computed per frame per vertex. It is event-driven:

1. Designer settings on a `LIGHT` become linear/reciprocal functions (`FitLinearFunction`, `FitRecipFunction`,
   `ConvertAngleStrength`) in `RebuildLight`, which stores the light's shader payload.
2. A change to a non-dynamic light bumps the world's relight stamp (`InvalidateLight`); `RenderSw` notices and
   `SubmitSwRelightAll` -> `SubmitGlobsetRelight` queues kind-0 entries.
3. The relight pass runs the relight microprogram over the vertices; its results are read back from VU1 memory by the
   VIF1 interrupt handler (`StoreRelightColors`) into the glob's EE-side colour buffers (two colour sets per vertex).
4. Every later frame draws with those stored colours. Dynamic per-object lights (up to four, chosen by
   `FindLightsAffectingPoint` through the light plane tree) are added by the B-family glob program through
   `BuildSoLightPackets`.

The formula and the "light fills the headroom first, ambient the rest" rule are in
[engine.md section 7.3](../engine.md#73-rendering-enginegraphics). The cel look comes from the art, the light curve, the
outlines and that fill rule; there is no quantisation step.

## 6. Shaders, textures and shader animation

- A `SHD` has a kind (`GetShaderPassCount` gives 1-3 texture passes), colours and a list of `TEX`; `BuildShaderTex0List`
  precomputes the GS `TEX0` words per frame and pass (`Tex0FromTexIframeCtk`); `PackTexGifs` writes `TEX0`/`TEX1`/`CLAMP`.
- Bitmaps are 4- or 8-bit indexed with CLUTs (decoding: [formats/textures.md](../formats/textures.md)). GS memory is
  managed by `GSB` (`IgsAllocGsb`, `PadGsbToPage`); uploads use `BuildImageGifs` and are sent synchronously at load
  (`SendDmaSyncGsFinish`).
- `UpdateShaders` (from the world update) runs every updatable `SAA`; a changed SAA marks itself dirty and
  `PropagateSais` applies its copy records (VIF unpacks that rewrite GS words inside packets). EYES SAAs implement
  blinking (`SetEyesClosed`), SCROLLER/CIRCLER move UVs (`SetSaiDuDv`), LOOP/PINGPONG/SHUFFLE step frames (`SetSaiIframe`).
- The world background gradient is a generated image (`BuildWorldBackgroundImage`).

## 7. Culling and visibility

- **Per object**: `grfvis` on each LO against `g_cm.grfvis`; the camera's mask comes from the `VIS` zone it is in
  (`UpdateVisInside`, `SetCmVisZone`; see [camera.md](camera.md)). Objects that are not live (see
  [engine.md section 6](../engine.md#6-the-object-model)) never get RenderSelf at all.
- **Per sub-glob**: a VU0 microprogram sphere test (`GCullSphere` in glob.cpp) against the frustum planes CM keeps in a
  transposed form; it also returns the fade used for distance fade-out. `FInsideCmFrustum` / `FInsideCmMrd` are the
  C-side equivalents used by gameplay code.
- **Scene render tree** (the unit holding `CullSceneRenderList`, `AddLoToRenderTree`, `InsertRenderEntry`): a sphere
  tree of render blocks. SOs enter it in their add path (`AddLoToRenderTree` from `engine/physics/so.cpp`); a posed
  static mesh (`RenderMsGlobset`) walks it with an explicit block stack and VU0 culls each entry. Its exact role beside
  the per-object RenderSelf walk is (inferred) a static-world fast path.

## 8. Particles and effects (`engine/fx/`)

- **EMITTER** (an XFM) owns one or more emitter blocks (`EMITB`) describing origin shape (`EMITO`: point, sphere, box,
  skeleton, mesh via `LoadEmitmeshFromBrx`), velocity distribution (`EMITV`) and what to spawn. `UpdateEmitter` decides
  how many particles to emit this frame; `EmitParticles` -> `OriginateParticles` -> `EmitRips` or `EmitBlips`.
  `EmitParticlesAt` emits from code with explicit parameters (used by explosions and splashes such as `WakeSoWater`).
- **BLIPG / BLIP**: sprite particles. Batches are advanced on VU0 (`ProjectBlipgTransformAccel`) and drawn by a VU1
  program as kind 12 (`RenderBlipgSelf`, `AddBlipgVuParams`). Per-particle motion is closed-form in time.
- **RIPG / RIP**: small mesh or custom particles with real per-type virtuals (Init, PostEmit, OnRemove, Project, Update,
  Render, Touch, Bounce). Types include bublets, glints, leaves, matches, shrapnel, smacks, flying and stuck debris.
  `UpdateRipg` advances them, `BounceRipgRips` collides them with nearby SOs, `RenderRip` draws through the SO model path.
- **EXPL family** (same file as EMITTER): `EXPLG` groups child explosions, `EXPLS` fires an emitter setup and an
  optional sound (`EnsureExplsSfx`), all through the explode virtual (`ExplodeExplParams`).
- **TRAIL**: keeps a ring of recent world matrices and places mesh vertices along it, faded by age (`UpdateTrail`,
  `ResetTrailHistory`).
- **Screen-space effects** elsewhere: the screen tint/fade object created by `CreateSwScreenTint`
  (`world/scene/sw.cpp`), the frame-capture split/flip transition (`StartFrameCapture`, `DrawFrameCaptureScreen` in
  `gameplay/cutscene/`), `FillScreenRect` (flat sprite) and `DrawWorldLabel` (projected text label) in render.cpp, the
  letterbox ([ui-and-hud.md](ui-and-hud.md)) and `BlendPrevFrame`.

## 9. Render targets

Two 128x128 GS textures are rendered from RTCAM camera objects (class cid 0x2D; the class name is not settled). The HUD
claims one with `AcquireRenderTarget` to show a 3-D object (an item or character preview: `FindObjectForRenderTarget`
picks the object), `RenderRtcam` draws the camera's objects into the target in the render-target passes, and the glob
renderer samples it **the next frame** through `GetRenderTargetTexture`. `ReleaseRenderTarget` gives it back and
restores the objects it hid from the main scene.

## 10. The backend in brief

- **DMAS**: owns a buffer (frame slot heap via `AllocSw`, or a static array via `AllocStatic`) and writes DMA source-chain
  tags: `AddDmaCnt`, `AddDmaRefs`, `AddDmaCall`, `AddDmaRet`, `AddDmaEnd`; `Detach` / `DetachCopySw` hand the chain out.
- **VIFS** (DMAS subclass): `AddVifUnpack` (copy or `AddVifUnpackRefs` in place), `AddVifMscal` (start a microprogram),
  `AddVifBaseOffset` (VU1 double buffering), `AddVifStcycl` / `AddVifStrow` / `AddVifStmask`, `AddVifFlush(a)`,
  `AddVifDirect` / `AddVifGifs` (GIF packets through PATH2).
- **GIFS** (DMAS subclass): PACKED (`AddPrimPack` + `Pack*`), REGLIST (`AddPrimList` + `List*`) and IMAGE (`AddImage`)
  tags; `EndPrim` fills in NLOOP.
- **GS** (`backend/gs.cpp`): display/draw environments, `SwapGsBuffers`, vblank sync (`SyncVBlank`), clear packets
  (`BuildClearGifs`), framebuffer-as-texture packets, GS memory allocator, synchronous uploads.
- **FRM** (`backend/frm.cpp`): the two frames, `OpenFrame` / `CloseFrame`, the render thread, `EnsureVu1Code` (upload a
  microprogram by REF), `BlendPrevFrame`.
- Microprogram code is binary data referenced by the pass hooks; it is not decompiled C.

## 11. Invariants and gotchas

- **Static packets are long-lived.** Model VIF batches built at load are referenced by address (DMA REF/CALL) every
  frame. Code that moves or frees model data mid-level breaks every frame that CALLs it. `CopyRelocateDmaChain`
  exists for the one sanctioned copy-and-relocate case.
- **Render-list storage is per frame.** RPLs and frame-heap packets are only valid until the frame is closed; never keep
  pointers to them across frames.
- **Relight output is EE data.** The game reads relit colours back from VU1 memory into globset buffers during the
  interrupt; skipping the relight pass leaves stale or black vertex colours rather than just "unlit" geometry.
- **Kind order is the draw order.** Translucency, cutouts, shadows and the HUD rely on the bucket order and on the
  depth sort of the blended kinds; there is no per-material order beyond that.
- **Z is "bigger = nearer"** (32-bit Z, depth test GEQUAL style), and the display is field-rendered at 512x224 per
  field with a half-line offset per field.
- **Shadow volumes count in the framebuffer** (green channel, then destination alpha), because the GS has no stencil.
- **The sky's textures share GS memory with the Z buffer** and are re-uploaded each frame before a Z clear.
- **`FillScreenRect` halves the caller's alpha** (GS alpha 0x80 is opaque) and writes it back.
- Many pass hooks and glob helpers are still marked `(inferred)` in their doc comments; treat their names as provisional.

## 12. How to

**Make an object draw.** Derive from `SO` and give it a `GLOBSET` (loaded by the SO loader); the default `RenderSoSelf`
draws it while the object is live and visible. For a custom draw, override LO slot 19 and queue entries with
`AllocRenderListEntry` (kind, depth, DMA CALL address of a packet built with `VIFS`/`GIFS` in the frame heap).

**Add a new kind of draw.** Pick a kind index whose pass matches the GS state you need, or add a pass: a begin/end
hook pair that uploads its microprogram (`EnsureVu1Code`) and GS state, plus the kind-to-pass entry. Remember the depth
sort rules in `CprplPartitionArpl` if the kind is blended.

**Draw a 2-D overlay.** Use a `GIFS` stream (as `FillScreenRect` and the font code do) and add it to the HUD path
([ui-and-hud.md](ui-and-hud.md)) rather than the 3-D passes.

**Change lighting at run time.** Change the `LIGHT`'s options and call `RebuildLight`; the stamp mechanism re-relights
affected models next frame. Dynamic lights are cheaper for moving sources.

## 13. What a port must replace

Everything in `backend/` and the parts of `pipeline/` that touch VU or GS registers are PS2-specific:

- **VU1 microprograms** (glob A/B families, relight, cel border, projected volume, blip, HUD): become GPU shaders;
  relight can stay CPU-side but must still write the colour buffers the game reads.
- **VIF/DMA packet building** (`DMAS`, `VIFS`, static model batches): either decoded at the frame boundary (the plan's
  interception approach) or replaced by native vertex/index buffers built from `GLOBSET` at load.
- **GS state and memory** (`GSB`, `TEX0`/`CLUT` words, `FBA`, framebuffer-as-texture, Z-buffer-aliased sky textures):
  become pipeline state and real textures; CLUT swaps (SAA) need palette support or re-expansion.
- **Shadow volumes**: a real stencil buffer replaces the green-channel counting.
- **VU0 macro/micro code** (sphere culls, blip update, skinning helpers): plain C++ or SIMD.
- **The frame system and render thread** (`FRM`, semaphores, vblank and VIF1/GS interrupt handlers, `SwapGsBuffers`,
  field offsets, `BlendPrevFrame`): replaced by the host swapchain; frame blend becomes an optional effect.
- **Render targets**: offscreen textures bound where the game samples the target's GS address.

The game-side structure worth keeping: the RPL kinds and their sort, the pass boundaries, the relight trigger
(illumination stamp), shader animation semantics, and the culling/visibility rules.

## 14. Where to look

- `src/game/engine/graphics/pipeline/`: `render.cpp` (RenderSw, RPL allocation), `draw.cpp` (DrawSw, sort, pass hooks
  table), `glob.cpp` (globsets and most pass hooks), `shd.cpp`, `shdanim.cpp`, `light.cpp`, `ms.cpp`, `sky.cpp`, `vis.cpp`,
  plus two units still named by address (render targets; scene render tree).
- `src/game/engine/graphics/backend/`: `dmas.cpp`, `vifs.cpp`, `gifs.cpp`, `gs.cpp`, `frm.cpp`.
- `src/game/engine/fx/`: `emitter.cpp`, `blip.cpp`, `rip.cpp`, `trail.cpp`.
- `src/game/engine/physics/so.cpp`: `RenderSoSelf`, render-tree insertion of SOs.
- `include/vifs.h`, `include/gifs.h`, `include/dmas.h`: the packet builder classes; `include/sly2/emitter.h`.
- `src/sdk/libgraph`, `src/sdk/libdma`: Sony GS/DMA libraries the backend calls.
