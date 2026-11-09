# Source tree architecture

The game code under `src/` is grouped into layers, lowest first. A
subsystem may depend on its own layer and on lower layers; dependencies on
higher layers are violations. Because the code must stay byte-identical to
the retail binary, we cannot change who calls whom, only where files live
and how headers are organised. The violations that remain are couplings
built into Sucker Punch's design; each is listed below with the reason it
stays and a header-only remedy.

- Each original translation unit (TU) is one source file. Files are named after the original file where it is known
  (`sw.cpp`, `relay.cpp`); otherwise after the class or system it holds (`safehouse.cpp`, `robottank.cpp`), or
  `empty_<start address>` for a unit that holds only compiler-generated stubs. `tools/layout/units.csv` maps every function to its unit; the file header gives the
  unit's address range.
- How the graph was built, the alternatives tried and the per-unit placement rationale are in the design record
  (`research/layout/LAYOUT.md`, outside the repo).

## 1. Target architecture and layer rules

| layer | prefix | responsibility |
|---|---|---|
| L0 | `sdk` | Sony / 989 SDK libraries (other compilers; replaced by HLE in a port) |
| L1 | `game/platform` | PS2 machine services: boot, heaps, threads, disc (CD/DVD) I/O, pad, printf/string runtime |
| L2 | `game/core` | engine-independent foundation: math, containers, binary streams, clock, locale |
| L3 | `game/splice` | Splice script VM (Sucker Punch library: values/refs, pairs, frames, evaluator, GC); built-ins live in app/ |
| L4 | `game/object` | object model: BASIC/LO base classes, transforms, proxies, lookup, BRX object loading, script peers |
| L5 | `game/world` | the SW world container and its registries; session state (game, save variables, difficulty) |
| L6 | `game/engine` | engine services: graphics, fx, anim, physics, sound, camera, text, presentation (HUD/widgets/screen manager) |
| L7 | `game/level` | level flow: per-frame update, level load glue, transitions, warps |
| L8 | `game/actor` | shared actor bases PO/STEP (player and guards both derive from them) |
| L9 | `game/gameplay` | player, AI, gameplay objects, progression, cutscenes |
| L10 | `game/ui` | front-end screens: pause menu, episode menu, memory-card dialogs |
| L11 | `game/app` | composition root: main loop, boot sequence, class registry/factories, Splice built-ins and binding thunks |
| L11 | `generated` | tool-generated Splice wrappers |

Rules:
1. A file may call / reference / include only its own layer or lower layers ("downward"). Edge kinds counted: call, tail call, virtual
   call (slot attributed to the class that introduces it), function-pointer load, function-pointer table entry, data reference to a
   global defined elsewhere, vtable inheritance (0.25 per inherited slot).
2. Subsystems in the same layer are peers; peer edges are allowed but cycles between peers are reported with a recommended order
   (section 7). Inside a subsystem anything goes (TUs of one subsystem may be mutually dependent).
3. `src/sdk/<lib>` and `src/generated/splicewrap` keep their draft locations. No `.c/.cpp` outside `src/{game,sdk,generated}`.
4. Every TU is one file; file stem = the unit's name. No directory contains two names differing only by case.
5. A new function goes in its translation unit's file (`tools/layout/units.csv` maps every function to its unit). A new
   upward dependency should be avoided; if one is unavoidable, add it to the table in section 3 with the reason.
6. Naming reflects layering: all children of a directory are in the same layer (the only exception is `src/game/` itself).
   Max depth under `src/game` = 3 directories (`engine/graphics/pipeline/`). `platform/memory/heap.cpp` also holds 2 functions
   of the `dynarray` span because the original game keeps them in its `heap.cpp` (suggests the heap/dynarray span boundary is
   off by those functions - HYPOTHESIS).

## 2. The tree (one line per directory, then its files)

```
src/
  sdk/<lib>/                   L0  Sony / 989 SDK (unchanged from the draft: one dir per library; other compilers; replaced by HLE in a port)
                                   989snd, collect2, libc, libcdvd, libdma, libgcc, libgraph, libipu, libkernel, libm, libmc, libmpeg, libpad, libscf
  game/platform/boot/crt0/     L1  crt0 `_start`
                                   files: _start
  game/platform/cdvd/          L1  CD driver, CD file table (fid -> name/size)
                                   files: cd cdutil
  game/platform/input/         L1  pad (JOY) and rumble
                                   files: joy rumble
  game/platform/memory/        L1  heaps (HEAP/CSwHeap, slot heaps, phase memory, scratchpad buffer), AllocZeroedFromHeap, malloc/free
                                   files: heap memory phasemem slotheap sprbuf palette16 palette256
  game/platform/runtime/       L1  Sucker Punch's own printf/string runtime (PrintfToStream, vsprintf, strlen) and OSTRM streams
                                   files: stream text
  game/platform/thread/        L1  EE threads / semaphores wrapper
                                   files: thread
  game/core/binstream/         L2  CBinaryInputStream (bis), async CD stream (bas), CStreamBis
                                   files: bas bis streambis
  game/core/containers/        L2  intrusive lists (DL/SL), CDynArray, byte queues, hashing, small key/string class
                                   files: bq dl dynarray empty_119DA0 empty_119EF8 empty_11B1A8 dynarraystr strhash
  game/core/locale/            L2  language / region selection
                                   files: language
  game/core/math/              L2  vectors, matrices, Bezier, Crout solver, BSP/planes, DZG LCP core, misc util
                                   files: bez bsp crout dzgcore mat util vec
  game/core/time/              L2  clock
                                   files: clock
  game/splice/                 L3  Splice VM: values/refs, pairs, frames, evaluator, GC, procedures, methods, serialisation
                                   files: empty_126188 empty_1262E0 eval frame gc method pair proc ref serialize sidebag spliceutils varb
  game/object/                 L4  BASIC/LO base classes, XFM transforms, proxies, find-by-OID, BRX object deserialisation, busy list, script peers (spliceobj/splicemap)
                                   files: basic brx busy find lo proxy splicemap spliceobj xform
  game/world/scene/            L5  SW: the world container (g_sw, object lists, load queues) and its callback queue
                                   files: callback sw
  game/world/state/            L5  session state: GAME (world/level/jobs/events), save variables, difficulty
                                   files: difficulty game save
  game/engine/anim/            L6  actuators (ACT), animation segments (ASEG/ASEGA), blends, IK, state machines (SM), chains, WR, XF transform/motion slots
                                   files: act aseg asega blend empty_12F0F0 empty_131F70 ik sm tail constraint wr xf
  game/engine/camera/          L6  camera (CM), camera policies (CPLCY), TN camera nodes, CPCAMERA
                                   files: cm cplcy tn camera
  game/engine/font/            L6  font rendering and Shift-JIS
                                   files: font sjis
  game/engine/fx/              L6  particles and effects: emitters, RIPs, blips, trails
                                   files: blip emitter rip trail
  game/engine/graphics/backend/ L6  GS / DMA / GIF / VIF packet builders and the frame (FRM) system
                                   files: dmas empty_1578F8 frm gifs gs vifs
  game/engine/graphics/pipeline/ L6  draw/render passes, globs, shaders (SHD/SHDANIM), lights, sky, visibility zones, culling sphere tree, item render targets
                                   files: draw glob light ms render shd shdanim sky rtcam rendertree vis
  game/engine/physics/         L6  SO solid objects, solver, shapes/geometry, BSP marks/contacts, clipping, paths, volumes, water, launchers, mesh contacts
                                   files: bbmark clip dzg empty_1C2920 geom mark path shape so solve sticky cnvo launcher vol water
  game/engine/presentation/    L6  presentation service: HUD widgets, SCREEN manager/base, letterbox, progress bar + loading screen
                                   files: hud letterbox prog screen
  game/engine/sound/           L6  sound manager, banks/streams, ambient emitters, speakers
                                   files: amb sound subtitle
  game/level/                  L7  per-frame world update, world-file load glue, warps/exits, wipes (level flow)
                                   files: brx_sw empty_1FF690 empty_226AB8 exit_warp update wipe
  game/actor/                  L8  PO and STEP: the shared bases of the player characters and the guards
                                   files: po step
  game/gameplay/ai/            L9  STEPGUARD, guard behaviours (BHV stack, guardbhv), detectors, noise stimuli, pickpocket, named bosses/NPCs, spawners
                                   files: bhvstk detector gomer guardbhv noise pickpocket stepguard stepbhv carmelita contessa dimitri guardscript bhvsearch spawner neela rajan safehouse watchdog
  game/gameplay/cutscene/      L9  relays (scripted event sequences), MPEG FMV, framebuffer capture screen
                                   files: mpeg relay framecapture
  game/gameplay/objects/       L9  interactive world objects and minigames: buttons, barriers, bombs, missiles, sensors, turrets, targets, rhythm, hacking, RC tank...
                                   files: barrier bomb button empty_1D7A30 missile rhythm sensor target empty_14C548 empty_1539A8 filerec screenfade hack critter breakable lootpouch robottank freegrabspot gadget turret
  game/gameplay/player/        L9  STEPPLAYER, Sly/Bentley/Murray (JT, Bentley, Murray), moves (air, grab, hang, pipe, rail, throw, hurt...), grab spots, recon
                                   files: air bentley cane dirblend empty_1344F0 empty_1378D8 grab grabspot hang hnd hurt jt jtgoal jthands mode murray pipe pipe_jt places rail stepplayer throw guardoverride recon gait
  game/gameplay/progression/   L9  prizes/clues, booty, cheats, waypoints
                                   files: booty cheat prize waypoint
  game/ui/                     L10 front-end screens: pause menu, episode menu, memory-card flow and dialogs
                                   files: empty_1AE7D0 memcard menu episodemenu
  game/app/                    L11 composition root: main loop, boot sequence, class registry (New*/Get*ClassDesc, D_002BAAE8), Splice built-ins (bif, bif2) and binding thunks
                                   files: bif bif2 class_registry (the whole crt0_splicewrap TU, generated wrappers included) main startup
                                   class_registry_options.inc (not a TU): crt0_splicewrap's Splice option tables (g_aeopid, the class option
                                   arrays), included by class_registry.cpp (docs/guides/tu-merging.md "The crt0_splicewrap tables"); its
                                   records take the addresses of the inline option accessors of the class headers, so class_registry emits
                                   their linkonce copies (formerly game/core/linkonce/unhomed, then app/eopid_linkonce.cpp)
                                   commons.cpp (not a TU, data only): the game's commons (class/template statics, sentries; all of
                                   .sbss and the end of .bss) in the original's allocation order; here because only the composition
                                   root may name every layer's objects (docs/guides/tu-merging.md "Phase 9: the commons file")
  generated/splicewrap/        L11 tools/gen_splicewrap.py output (373 wrappers, .inc); never edit by hand
                                   not a TU: fragments that app/class_registry.cpp (crt0_splicewrap) includes at their addresses
```

## 3. Remaining violations and why they stay

| category | subsystem pairs | weight | what it is / remedy |
|---|---|---|---|
| WORLD-UP | 20 | 101 | The world container and session state orchestrate higher subsystems: DeleteSw/ExecuteSwTransition reset sound/camera/shaders, call prizes, warps and FMV; GAME triggers wipes and the boot sequence. SW is a god object; whichever layer it sits in, ~150 weight crosses it. |
| SCRIPT-BUILTINS | 4 | 93 | The Splice evaluator (eval->bif 21, serialize 11, pair 8) and the BASIC script peers (spliceobj/basic->bif 28) dispatch into the built-in table, which now sits in app/ as the binding layer. Header fix: the VM sees the built-in table only as an array of function pointers declared in splice/. |
| PRESENTATION-READS | 7 | 91 | The HUD/screen manager (engine/presentation, L6) reads game state (screen->stepplayer 18, guard behaviours) and opens front-end screens (screen/hud->memcard 25, screen->menu 9). The price of making HUD a service; header fix: HUD pulls through PO*/BASIC queries only. |
| SPECIFIC | 23 | 77 | Small, individually explained couplings (see table). |
| HERO-HOOK | 33 | 70 | Engine/world code asks for the current player (PpoCur/PpoHero, 0x1C4840/0x1C4A48: sound mix, listener, camera, warps, busy list) or special-cases a player/guard class. Header fix: expose PpoCur/PpoHero through an `actor/po_query.h` that only returns PO* (an opaque base). |
| UI-PUSH | 16 | 69 | Lower layers (world/state, world/scene, level, object, platform) call HUD widget animation (PlayWidgetAnim, SetWidgetState), Show/Hide screens and memcard UI directly. Sucker Punch had no event bus; the HUD reads game state and the game pokes the HUD. Since review round 1 the HUD is a L6 service, so gameplay->HUD is downward; what remains comes from L5 and below (game->hud 10, sw->screen 7) and gameplay->front-end screens. |
| WORLD | 5 | 43 | Code below the world layer touches g_sw (LO add/remove, BRX load queues, splice built-ins asking for the current level/world/episode) or session state. Inherent: g_sw is the global context of the engine. Header fix: declare `struct SW` + the `g_sw` extern (0x3DD490) in `world/scene/sw.h` and allow it as a sanctioned include for engine/object and splice/builtins. |
| C-RUNTIME | 13 | 36 | SDK code calls game-provided runtime functions (libgcc EH -> malloc/free in memory; 989snd -> printf-family in text; libc -> 0x100218 in the crt0 block). Inherent to how the ELF was linked; in a port these become host libc. |
| ONE-HIERARCHY | 7 | 31 | BASIC/LO/XFM base methods call SO/XF/ACT/CPLCY services (BindLoOidTable -> FindSoPart, TranslateXfmToPos -> ConvertXfPos). The object model and engine classes are one C++ hierarchy split over many files; xf could join engine/object at the cost of xf->act/so up-edges (tried: no gain). |
| SCRIPT-API | 1 | 15 | Splice is the game's scripting API: values wrap BASIC objects (CRef_SetBasic -> BASIC vtable), frames/eval resolve proxies, built-ins query the world. The VM part could be decoupled by a vtable-only interface; the built-ins cannot. |
| BINDING | 4 | 6 | Option/property binding records in the crt0-block .data prefix (0x2AC880-0x2BAAE8, app/class_registry_options.inc) point at generated wrappers; brx/proxy/emitter/so read them by address. |
| REGISTRY | 1 | 1 | Class descriptor table D_002BAAE8 / factories live in the crt0-block registry; IsBasicDerivedFrom (basic) and CreateLo (brx) read it. Header fix: declare the descriptor record type and an extern for the table in object/; only the definition stays in app/. |

All upward subsystem pairs with weight >= 3 (54 of 134; the other 80 pairs total 112 weight, mostly single vtable slots
or one call each; all are listed in the design record, research/layout/):

| # | from (layer) | to (layer) | weight | main TU pairs | category |
|---|---|---|---|---|---|
| 1 | splice (L3) | app (L11) | 51.0 | eval->bif 21, serialize->bif 11, pair->bif 8, ref->bif 8 | SCRIPT-BUILTINS |
| 2 | engine/presentation (L6) | ui (L10) | 39.5 | screen->memcard 14, hud->memcard 11, screen->menu 9, hud->episodemenu 4 | PRESENTATION-READS |
| 3 | object (L4) | app (L11) | 34.0 | spliceobj->bif 15, basic->bif 13, basic->class_registry 2, splicemap->bif 2 | SCRIPT-BUILTINS |
| 4 | object (L4) | world/scene (L5) | 32.2 | lo->sw 16, brx->sw 8, proxy->sw 4, basic->sw 2 | WORLD |
| 5 | engine/presentation (L6) | gameplay/player (L9) | 21.0 | screen->stepplayer 18, hud->recon 2, screen->guardoverride 1 | PRESENTATION-READS |
| 6 | world/scene (L5) | engine/physics (L6) | 18.2 | sw->so 11, sw->bbmark 5, sw->path 2 | WORLD-UP |
| 7 | world/state (L5) | engine/presentation (L6) | 16.7 | game->hud 10, game->screen 7 | UI-PUSH |
| 8 | object (L4) | engine/anim (L6) | 15.5 | xform->xf 8, proxy->xf 4, lo->xf 2, basic->asega 1 | ONE-HIERARCHY |
| 9 | core/containers (L2) | object (L4) | 15.0 | dynarraystr->basic 10, bq->basic 3, dynarray->basic 2 | SPECIFIC |
| 10 | splice (L3) | object (L4) | 15.0 | frame->proxy 4, ref->basic 4, eval->proxy 3, proc->proxy 2 | SCRIPT-API |
| 11 | sdk/989snd (L0) | platform/runtime (L1) | 13.0 | sdk:989snd->text 13 | C-RUNTIME |
| 12 | world/scene (L5) | engine/sound (L6) | 12.4 | sw->sound 12 | WORLD-UP |
| 13 | world/scene (L5) | engine/presentation (L6) | 12.0 | sw->screen 7, sw->hud 3, sw->letterbox 2 | UI-PUSH |
| 14 | engine/presentation (L6) | gameplay/ai (L9) | 11.0 | screen->bhvstk 5, screen->guardbhv 3, hud->guardbhv 2, screen->safehouse 1 | PRESENTATION-READS |
| 15 | engine/presentation (L6) | actor (L8) | 10.0 | screen->po 6, hud->po 4 | PRESENTATION-READS |
| 16 | world/scene (L5) | engine/camera (L6) | 10.0 | sw->cm 5, sw->cplcy 5 | WORLD-UP |
| 17 | gameplay/ai (L9) | ui (L10) | 9.0 | guardbhv->menu 5, spawner->menu 3, stepbhv->menu 1 | UI-PUSH |
| 18 | actor (L8) | gameplay/ai (L9) | 9.0 | step->bhvstk 5, po->bhvstk 1, po->guardbhv 1, po->safehouse 1 | SPECIFIC |
| 19 | platform/input (L1) | splice (L3) | 9.0 | rumble->ref 9 | SPECIFIC |
| 20 | world/scene (L5) | engine/graphics/pipeline (L6) | 8.2 | sw->shd 4, sw->shdanim 2, sw->light 1, sw->rtcam 1 | WORLD-UP |
| 21 | world/state (L5) | level (L7) | 8.0 | game->wipe 6, game->exit_warp 2 | WORLD-UP |
| 22 | object (L4) | engine/physics (L6) | 8.0 | lo->so 4, proxy->so 3, proxy->path 1 | ONE-HIERARCHY |
| 23 | world/state (L5) | ui (L10) | 7.0 | game->memcard 6, save->menu 1 | UI-PUSH |
| 24 | sdk/libgcc (L0) | platform/memory (L1) | 7.0 | sdk:libgcc->memory 7 | C-RUNTIME |
| 25 | world/scene (L5) | level (L7) | 6.2 | sw->update 2, sw->exit_warp 2, sw->brx_sw 1, sw->wipe 1 | WORLD-UP |
| 26 | core/linkonce (L2) | engine/fx (L6) | 6.0 | game_linkonce->emitter 6 | SPECIFIC |
| 27 | engine/physics (L6) | gameplay/player (L9) | 6.0 | sticky->jt 2, so->places 1, so->stepplayer 1, sticky->dirblend 1 | HERO-HOOK |
| 28 | engine/sound (L6) | actor (L8) | 6.0 | sound->po 6 | HERO-HOOK |
| 29 | world/scene (L5) | actor (L8) | 6.0 | sw->po 6 | HERO-HOOK |
| 30 | world/scene (L5) | gameplay/ai (L9) | 5.7 | sw->guardbhv 1, sw->bhvstk 1, sw->noise 1, sw->stepguard 1 | HERO-HOOK |
| 31 | core/binstream (L2) | object (L4) | 5.0 | bis->basic 5 | SPECIFIC |
| 32 | platform/input (L1) | world/state (L5) | 5.0 | rumble->game 3, joy->game 2 | WORLD |
| 33 | engine/graphics/pipeline (L6) | gameplay/objects (L9) | 5.0 | render->rhythm 5 | SPECIFIC |
| 34 | world/state (L5) | engine/sound (L6) | 5.0 | save->sound 5 | WORLD-UP |
| 35 | engine/presentation (L6) | gameplay/objects (L9) | 5.0 | screen->rhythm 5 | PRESENTATION-READS |
| 36 | sdk/libgcc (L0) | core/containers (L2) | 5.0 | sdk:libgcc->dynarraystr 5 | C-RUNTIME |
| 37 | platform/cdvd (L1) | engine/presentation (L6) | 4.0 | cd->screen 4 | UI-PUSH |
| 38 | engine/fx (L6) | app (L11) | 4.0 | emitter->bif 3, emitter->class_registry 1 | SCRIPT-BUILTINS |
| 39 | world/state (L5) | app (L11) | 4.0 | game->startup 4 | WORLD-UP |
| 40 | platform/memory (L1) | object (L4) | 4.0 | heap->basic 4 | SPECIFIC |
| 41 | gameplay/cutscene (L9) | ui (L10) | 4.0 | mpeg->memcard 4 | UI-PUSH |
| 42 | world/scene (L5) | engine/graphics/backend (L6) | 4.0 | sw->gs 3, sw->frm 1 | WORLD-UP |
| 43 | platform/memory (L1) | engine/graphics/pipeline (L6) | 4.0 | palette16->shd 2, palette256->shd 2 | SPECIFIC |
| 44 | gameplay/ai (L9) | app (L11) | 4.0 | dimitri->bif 4 | SCRIPT-BUILTINS |
| 45 | world/state (L5) | engine/physics (L6) | 3.7 | game->so 4 | WORLD-UP |
| 46 | level (L7) | ui (L10) | 3.0 | update->memcard 2, brx_sw->memcard 1 | UI-PUSH |
| 47 | object (L4) | engine/camera (L6) | 3.0 | busy->cplcy 2, lo->cplcy 1 | ONE-HIERARCHY |
| 48 | world/scene (L5) | app (L11) | 3.0 | callback->bif 2, sw->startup 1 | WORLD-UP |
| 49 | world/state (L5) | gameplay/progression (L9) | 3.0 | difficulty->prize 2, game->booty 1 | WORLD-UP |
| 50 | level (L7) | actor (L8) | 3.0 | exit_warp->po 2, update->po 1 | HERO-HOOK |
| 51 | core/linkonce (L2) | actor (L8) | 3.0 | game_linkonce->po 2, game_linkonce->step 1 | HERO-HOOK |
| 52 | core/linkonce (L2) | engine/physics (L6) | 3.0 | game_linkonce->so 3 | SPECIFIC |
| 53 | object (L4) | world/state (L5) | 3.0 | lo->game 3 | WORLD |
| 54 | world/scene (L5) | gameplay/progression (L9) | 3.0 | sw->prize 2, sw->cheat 1 | WORLD-UP |

Rows 26, 51 and 52 were measured when the unhomed linkonce copies sat in game/core/linkonce/. That directory is gone
(inline passes 1 and 3): the copies are inline functions or function templates in the class headers (include/sly2/),
emitted by app/class_registry.cpp (the option table g_aeopid takes their addresses; the grab-spot owner getters by their
key TUs in gameplay/), so those edges now come from app/ (L11) and are downward.

Specific notes (CONFIRMED from edge examples):
- core/containers -> object: `dynarraystr` (util library) defines a class whose vtable inherits BASIC slots; `bq`/`dynarray` call
  through BASIC-shaped vtables. The util library therefore already depended on BASIC in the original source.
- core/binstream -> object: `bis` calls virtual slots on a BASIC-derived owner while decompressing (Decompress/Refill).
- platform/input -> splice: rumble's "enabled" flag is a save variable read through Splice refs.
- actor -> gameplay/ai: STEP owns a behaviour stack (InitStep -> InitBhvstk); BHVSTK lives with the guard behaviours. Moving
  `bhvstk` to actor would trade 9 for ~30 weight (ai->bhvstk is heavy); left as is.
- world/scene -> engine/*: DeleteSw/PostSwLoad/ExecuteSwTransition reset camera, sound, shaders, physics filters.
- platform/memory -> engine/graphics/pipeline: two static-init-only TUs (`palette16`, `palette256`) touch shd tables; unknown purpose.

**Header organisation** (ours to choose) can make most of the remaining couplings compile-time clean without moving code: put the
*declarations* that lower layers need into lower headers - `struct SW` + the `g_sw` extern (world/scene/sw.h), the class-descriptor type +
table extern (object/), PpoCur/PpoHero returning `PO*` (actor/), the built-in table as a function-pointer array declared in splice/ - and
mark those four headers as the only sanctioned upward includes. The link-level edges stay, but the include graph becomes a DAG.

## 4. Peer cycles inside a layer (allowed; recommended order)

| layer | members, recommended order (low -> high) | back-edge weight | heaviest back edges |
|---|---|---|---|
| L0 | sdk/collect2 < sdk/libc < sdk/libgcc | 5 | sdk/libc->sdk/libgcc 3, sdk/collect2->sdk/libgcc 2 |
| L1 | platform/memory < platform/runtime | 1 | platform/memory->platform/runtime 1 |
| L2 | core/binstream < core/containers < core/math | 1 | core/binstream->core/math 1 |
| L5 | world/scene < world/state | 37 | world/scene->world/state 37 |
| L6 | engine/anim < engine/sound < engine/physics < engine/camera < engine/graphics/backend < engine/graphics/pipeline < engine/fx < engine/presentation | 70 | engine/physics->engine/graphics/pipeline 19, engine/sound->engine/presentation 7, engine/graphics/pipeline->engine/presentation 7, engine/graphics/backend->engine/graphics/pipeline 7 |
| L9 | gameplay/ai < gameplay/objects < gameplay/player < gameplay/cutscene < gameplay/progression | 81 | gameplay/ai->gameplay/player 49, gameplay/player->gameplay/progression 12, gameplay/ai->gameplay/objects 8, gameplay/objects->gameplay/progression 4 |
| L11 | generated/splicewrap < app | 24 | generated/splicewrap->app 24 |

Reading: in L6 the data says anim (incl. XF) is the base of the engine services, then sound, physics, camera, the render backend, the
scene renderer and fx on top; graphics/backend -> graphics/pipeline back edges (7) are gs->shd and frm->shdanim calls. In L9 player <-> ai is genuinely
mutual (guards query the player; the player pickpockets/takes down guards): treat L9 as peers. world/scene -> state (37) is
ResolveSwSaveVars/events. (The L3 splice/vm <-> builtins cycle disappeared with review round 1.)

