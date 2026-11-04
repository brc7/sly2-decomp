# Source tree architecture

The game code under `src/` is grouped into layers, lowest first. A
subsystem may depend on its own layer and on lower layers; dependencies on
higher layers are violations. Because the code must stay byte-identical to
the retail binary, we cannot change who calls whom, only where files live
and how headers are organised. The violations that remain are couplings
built into Sucker Punch's design; each is listed below with the reason it
stays and a header-only remedy.

- Each leaf directory under `src/game/` is one original translation unit
  (source file). Names like `tu_1D7238` (start address) or `empty_126188`
  (a unit holding only compiler-generated stubs) are units not yet named.
  Each leaf will later collapse into a single `.cpp`.
- Every directory has a `README.md` with its layer and responsibility.
- How the graph was built, the alternatives tried and the per-unit
  placement rationale are in the design record
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
4. Every TU has its own leaf directory (it later collapses into `<tu>.cpp`). File stem = linker symbol. No directory contains two
   names differing only by case (the old SetPoCtlState/SetPoctlState pair was renamed for this).
5. A new function goes in its translation unit's directory (`tools/layout/units.csv` maps every function to its unit). A new
   upward dependency should be avoided; if one is unavoidable, add it to the table in section 3 with the reason.
6. Naming reflects layering: all children of a directory are in the same layer (the only exception is `src/game/` itself).
   No two directories share a name except a subsystem and its main TU leaf (`memory/memory`, `sound/sound`, `font/font`,
   `thread/thread`) and the `game` TU (`world/state/game`) - TU leaf names are the tu-map labels and are kept.
   Max depth under `src/game` = 4 directories (`engine/graphics/pipeline/<tu>/`); every leaf directory is one TU, with one
   exception: `platform/memory/heap/` also receives 2 functions of the `dynarray` span because they live in the real file `src/P2/heap.cpp`
   (suggests the heap/dynarray span boundary is off by those functions - HYPOTHESIS).

## 2. The tree (one line per directory; TU leaves with function counts)

```
src/
  sdk/<lib>/                   L0  Sony / 989 SDK (unchanged from the draft: one dir per library; other compilers; replaced by HLE in a port)
                                   989snd, collect2, libc, libcdvd, libdma, libgcc, libgraph, libipu, libkernel, libm, libmc, libmpeg, libpad, libscf
  game/platform/boot/          L1  crt0 `_start`
                                   TU leaves: crt0(1)
  game/platform/cdvd/          L1  CD driver, CD file table (fid -> name/size)
                                   TU leaves: cd(10), tu_14B660(5)
  game/platform/input/         L1  pad (JOY) and rumble
                                   TU leaves: joy(19), rumble(11)
  game/platform/memory/        L1  heaps (HEAP/CSwHeap, slot heaps, phase memory, scratchpad buffer), PvAllocClear, malloc/free
                                   TU leaves: heap(12), memory(21), phasemem(4), slotheap(7), sprbuf(5), tu_1147D8(2), tu_114AE0(2)
  game/platform/runtime/       L1  Sucker Punch's own printf/string runtime (CchOstrmPrintf, vsprintf, strlen) and OSTRM streams
                                   TU leaves: stream(7), text(24)
  game/platform/thread/        L1  EE threads / semaphores wrapper
                                   TU leaves: thread(7)
  game/core/binstream/         L2  CBinaryInputStream (bis), async CD stream (bas), CStreamBis
                                   TU leaves: bas(13), bis(43), tu_1848A8(10)
  game/core/containers/        L2  intrusive lists (DL/SL), CDynArray, byte queues, hashing, small key/string class
                                   TU leaves: bq(10), dl(21), dynarray(9), empty_119DA0(2), empty_119EF8(2), empty_11B1A8(2), tu_119BC8(16), tu_11AFE0(3)
  game/core/locale/            L2  language / region selection
                                   TU leaves: language(8)
  game/core/math/              L2  vectors, matrices, Bezier, Crout solver, BSP/planes, DZG LCP core, misc util
                                   TU leaves: bez(5), bsp(14), crout(4), dzgcore(7), mat(33), util(28), vec(22)
  game/core/time/              L2  clock
                                   TU leaves: clock(8)
  game/splice/                 L3  Splice VM: values/refs, pairs, frames, evaluator, GC, procedures, methods, serialisation
                                   TU leaves: empty_126188(2), empty_1262E0(2), eval(27), frame(15), gc(19), method(2), pair(3), proc(4), ref(38), serialize(3), sidebag(12), spliceutils(10), varb(4)
  game/object/                 L4  BASIC/LO base classes, XFM transforms, proxies, find-by-OID, BRX object deserialisation, busy list, script peers (spliceobj/splicemap)
                                   TU leaves: basic(31), brx(16), busy(8), find(13), lo(74), proxy(38), splicemap(10), spliceobj(18), xform(18)
  game/world/scene/            L5  SW: the world container (g_sw, object lists, load queues) and its callback queue
                                   TU leaves: callback(6), sw(68)
  game/world/state/            L5  session state: GAME (world/level/jobs/events), save variables, difficulty
                                   TU leaves: difficulty(10), game(101), save(24)
  game/engine/anim/            L6  actuators (ACT), animation segments (ASEG/ASEGA), blends, IK, state machines (SM), chains, WR, XF transform/motion slots
                                   TU leaves: act(72), aseg(23), asega(27), blend(12), empty_12F0F0(2), empty_131F70(2), ik(8), sm(43), tail(6), tu_1603B8(4), wr(20), xf(44)
  game/engine/camera/          L6  camera (CM), camera policies (CPLCY), TN camera nodes, CPCAMERA
                                   TU leaves: cm(55), cplcy(51), tn(31), tu_12E600(13)
  game/engine/font/            L6  font rendering and Shift-JIS
                                   TU leaves: font(24), sjis(4)
  game/engine/fx/              L6  particles and effects: emitters, RIPs, blips, trails
                                   TU leaves: blip(21), emitter(54), rip(64), trail(13)
  game/engine/graphics/backend/ L6  GS / DMA / GIF / VIF packet builders and the frame (FRM) system
                                   TU leaves: dmas(28), empty_1578F8(2), frm(13), gifs(24), gs(26), vifs(21)
  game/engine/graphics/pipeline/ L6  draw/render passes, globs, shaders (SHD/SHDANIM), lights, sky, visibility zones, culling sphere tree, item render targets
                                   TU leaves: draw(7), glob(42), light(32), ms(5), render(7), shd(45), shdanim(33), sky(4), tu_1BE398(16), tu_1D57A8(10), vis(6)
  game/engine/physics/         L6  SO solid objects, solver, shapes/geometry, BSP marks/contacts, clipping, paths, volumes, water, launchers, mesh contacts
                                   TU leaves: bbmark(13), clip(8), dzg(9), empty_1C2920(2), geom(6), mark(22), path(51), shape(56), so(177), solve(24), tu_185130(16), tu_1BC050(31), tu_1D6848(6), vol(7), water(22)
  game/engine/presentation/    L6  presentation service: HUD widgets, SCREEN manager/base, letterbox, progress bar + loading screen
                                   TU leaves: hud(124), letterbox(6), prog(13), screen(98)
  game/engine/sound/           L6  sound manager, banks/streams, ambient emitters, speakers
                                   TU leaves: amb(10), sound(103), tu_2275B8(8)
  game/level/                  L7  per-frame world update, world-file load glue, warps/exits, wipes (level flow)
                                   TU leaves: brx_sw(5), empty_1FF690(2), empty_226AB8(2), exit_warp(22), update(4), wipe(8)
  game/actor/                  L8  PO and STEP: the shared bases of the player characters and the guards
                                   TU leaves: po(38), step(46)
  game/gameplay/ai/            L9  STEPGUARD, guard behaviours (BHV stack, guardbhv), detectors, noise stimuli, pickpocket, named bosses/NPCs, spawners
                                   TU leaves: bhvstk(25), detector(19), gomer(3), guardbhv(190), noise(23), pickpocket(11), stepguard(158), tu_144B28(43), tu_14A008(21), tu_14FAC8(4), tu_1553F0(19), tu_1709A8(42), tu_171F58(16), tu_173158(21), tu_1B9F80(14), tu_1CBEB0(5), tu_1D7238(10), tu_20CE28(15)
  game/gameplay/cutscene/      L9  relays (scripted event sequences), MPEG FMV, framebuffer capture screen
                                   TU leaves: mpeg(34), relay(141), tu_1471C0(11)
  game/gameplay/objects/       L9  interactive world objects and minigames: buttons, barriers, bombs, missiles, sensors, turrets, targets, rhythm, hacking, RC tank...
                                   TU leaves: barrier(9), bomb(22), button(50), empty_1D7A30(2), missile(16), rhythm(35), sensor(21), target(11), tu_14C548(2), tu_1539A8(2), tu_153B08(3), tu_15DC80(6), tu_17F090(76), tu_1B0B78(25), tu_1BAB50(34), tu_1C2D20(7), tu_1D1370(44), tu_1FF3B8(8), tu_223F70(28), turret(12)
  game/gameplay/player/        L9  STEPPLAYER, Sly/Bentley/Murray (JT, Bentley, Murray), moves (air, grab, hang, pipe, rail, throw, hurt...), grab spots, recon
                                   TU leaves: air(44), bentley(48), cane(12), dirblend(5), empty_1344F0(2), empty_1378D8(2), grab(28), grabspot(74), hang(12), hnd(4), hurt(28), jt(51), jtgoal(6), jthands(17), mode(14), murray(66), pipe(14), pipe_jt(15), places(14), rail(12), stepplayer(96), throw(12), tu_19F0A8(6), tu_201D78(26), tu_20B840(31)
  game/gameplay/progression/   L9  prizes/clues, booty, cheats, waypoints
                                   TU leaves: booty(28), cheat(13), prize(61), waypoint(7)
  game/ui/                     L10 front-end screens: pause menu, episode menu, memory-card flow and dialogs
                                   TU leaves: empty_1AE7D0(2), memcard(81), menu(38), tu_15CD68(12)
  game/app/                    L11 composition root: main loop, boot sequence, class registry (New*/Get*ClassDesc, D_002BAAE8), Splice built-ins (bif, bif2) and binding thunks
                                   TU leaves: bif(161), bif2(31), class_registry(424), main(7), splice_thunks(31), tu_1A4E08(9)
                                   eopid_linkonce.cpp (not a TU): takes the addresses of the inline option accessors defined in the
                                   class headers (include/sly2/*.h), as the option table g_aeopid does, so g++ emits their linkonce copies
                                   (formerly game/core/linkonce/unhomed; see docs/guides/tu-merging.md "Inline functions and linkonce copies")
  generated/splicewrap/        L11 tools/gen_splicewrap.py output (373 wrappers); never edit by hand
                                   TU leaves: splicewrap(373)
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
| ONE-HIERARCHY | 7 | 31 | BASIC/LO/XFM base methods call SO/XF/ACT/CPLCY services (BindLoOidTable -> PsopartFindSo, TranslateXfmToPos -> ConvertXfPos). The object model and engine classes are one C++ hierarchy split over many files; xf could join engine/object at the cost of xf->act/so up-edges (tried: no gain). |
| SCRIPT-API | 1 | 15 | Splice is the game's scripting API: values wrap BASIC objects (CRef_SetBasic -> BASIC vtable), frames/eval resolve proxies, built-ins query the world. The VM part could be decoupled by a vtable-only interface; the built-ins cannot. |
| BINDING | 4 | 6 | Option/property binding records in the crt0-block .data prefix (0x2AC880-0x2BAAE8) point at generated wrappers; brx/proxy/emitter/so read them by address. |
| REGISTRY | 1 | 1 | Class descriptor table D_002BAAE8 / factories live in the crt0-block registry; FIsBasicDerivedFrom (basic) and PloNew (brx) read it. Header fix: declare the descriptor record type and an extern for the table in object/; only the definition stays in app/. |

All upward subsystem pairs with weight >= 3 (54 of 134; the other 80 pairs total 112 weight, mostly single vtable slots
or one call each; all are listed in the design record, research/layout/):

| # | from (layer) | to (layer) | weight | main TU pairs | category |
|---|---|---|---|---|---|
| 1 | splice (L3) | app (L11) | 51.0 | eval->bif 21, serialize->bif 11, pair->bif 8, ref->bif 8 | SCRIPT-BUILTINS |
| 2 | engine/presentation (L6) | ui (L10) | 39.5 | screen->memcard 14, hud->memcard 11, screen->menu 9, hud->tu_15CD68 4 | PRESENTATION-READS |
| 3 | object (L4) | app (L11) | 34.0 | spliceobj->bif 15, basic->bif 13, basic->class_registry 2, splicemap->bif 2 | SCRIPT-BUILTINS |
| 4 | object (L4) | world/scene (L5) | 32.2 | lo->sw 16, brx->sw 8, proxy->sw 4, basic->sw 2 | WORLD |
| 5 | engine/presentation (L6) | gameplay/player (L9) | 21.0 | screen->stepplayer 18, hud->tu_201D78 2, screen->tu_19F0A8 1 | PRESENTATION-READS |
| 6 | world/scene (L5) | engine/physics (L6) | 18.2 | sw->so 11, sw->bbmark 5, sw->path 2 | WORLD-UP |
| 7 | world/state (L5) | engine/presentation (L6) | 16.7 | game->hud 10, game->screen 7 | UI-PUSH |
| 8 | object (L4) | engine/anim (L6) | 15.5 | xform->xf 8, proxy->xf 4, lo->xf 2, basic->asega 1 | ONE-HIERARCHY |
| 9 | core/containers (L2) | object (L4) | 15.0 | tu_119BC8->basic 10, bq->basic 3, dynarray->basic 2 | SPECIFIC |
| 10 | splice (L3) | object (L4) | 15.0 | frame->proxy 4, ref->basic 4, eval->proxy 3, proc->proxy 2 | SCRIPT-API |
| 11 | sdk/989snd (L0) | platform/runtime (L1) | 13.0 | sdk:989snd->text 13 | C-RUNTIME |
| 12 | world/scene (L5) | engine/sound (L6) | 12.4 | sw->sound 12 | WORLD-UP |
| 13 | world/scene (L5) | engine/presentation (L6) | 12.0 | sw->screen 7, sw->hud 3, sw->letterbox 2 | UI-PUSH |
| 14 | engine/presentation (L6) | gameplay/ai (L9) | 11.0 | screen->bhvstk 5, screen->guardbhv 3, hud->guardbhv 2, screen->tu_1D7238 1 | PRESENTATION-READS |
| 15 | engine/presentation (L6) | actor (L8) | 10.0 | screen->po 6, hud->po 4 | PRESENTATION-READS |
| 16 | world/scene (L5) | engine/camera (L6) | 10.0 | sw->cm 5, sw->cplcy 5 | WORLD-UP |
| 17 | gameplay/ai (L9) | ui (L10) | 9.0 | guardbhv->menu 5, tu_173158->menu 3, tu_144B28->menu 1 | UI-PUSH |
| 18 | actor (L8) | gameplay/ai (L9) | 9.0 | step->bhvstk 5, po->bhvstk 1, po->guardbhv 1, po->tu_1D7238 1 | SPECIFIC |
| 19 | platform/input (L1) | splice (L3) | 9.0 | rumble->ref 9 | SPECIFIC |
| 20 | world/scene (L5) | engine/graphics/pipeline (L6) | 8.2 | sw->shd 4, sw->shdanim 2, sw->light 1, sw->tu_1BE398 1 | WORLD-UP |
| 21 | world/state (L5) | level (L7) | 8.0 | game->wipe 6, game->exit_warp 2 | WORLD-UP |
| 22 | object (L4) | engine/physics (L6) | 8.0 | lo->so 4, proxy->so 3, proxy->path 1 | ONE-HIERARCHY |
| 23 | world/state (L5) | ui (L10) | 7.0 | game->memcard 6, save->menu 1 | UI-PUSH |
| 24 | sdk/libgcc (L0) | platform/memory (L1) | 7.0 | sdk:libgcc->memory 7 | C-RUNTIME |
| 25 | world/scene (L5) | level (L7) | 6.2 | sw->update 2, sw->exit_warp 2, sw->brx_sw 1, sw->wipe 1 | WORLD-UP |
| 26 | core/linkonce (L2) | engine/fx (L6) | 6.0 | game_linkonce->emitter 6 | SPECIFIC |
| 27 | engine/physics (L6) | gameplay/player (L9) | 6.0 | tu_185130->jt 2, so->places 1, so->stepplayer 1, tu_185130->dirblend 1 | HERO-HOOK |
| 28 | engine/sound (L6) | actor (L8) | 6.0 | sound->po 6 | HERO-HOOK |
| 29 | world/scene (L5) | actor (L8) | 6.0 | sw->po 6 | HERO-HOOK |
| 30 | world/scene (L5) | gameplay/ai (L9) | 5.7 | sw->guardbhv 1, sw->bhvstk 1, sw->noise 1, sw->stepguard 1 | HERO-HOOK |
| 31 | core/binstream (L2) | object (L4) | 5.0 | bis->basic 5 | SPECIFIC |
| 32 | platform/input (L1) | world/state (L5) | 5.0 | rumble->game 3, joy->game 2 | WORLD |
| 33 | engine/graphics/pipeline (L6) | gameplay/objects (L9) | 5.0 | render->rhythm 5 | SPECIFIC |
| 34 | world/state (L5) | engine/sound (L6) | 5.0 | save->sound 5 | WORLD-UP |
| 35 | engine/presentation (L6) | gameplay/objects (L9) | 5.0 | screen->rhythm 5 | PRESENTATION-READS |
| 36 | sdk/libgcc (L0) | core/containers (L2) | 5.0 | sdk:libgcc->tu_119BC8 5 | C-RUNTIME |
| 37 | platform/cdvd (L1) | engine/presentation (L6) | 4.0 | cd->screen 4 | UI-PUSH |
| 38 | engine/fx (L6) | app (L11) | 4.0 | emitter->bif 3, emitter->class_registry 1 | SCRIPT-BUILTINS |
| 39 | world/state (L5) | app (L11) | 4.0 | game->tu_1A4E08 4 | WORLD-UP |
| 40 | platform/memory (L1) | object (L4) | 4.0 | heap->basic 4 | SPECIFIC |
| 41 | gameplay/cutscene (L9) | ui (L10) | 4.0 | mpeg->memcard 4 | UI-PUSH |
| 42 | world/scene (L5) | engine/graphics/backend (L6) | 4.0 | sw->gs 3, sw->frm 1 | WORLD-UP |
| 43 | platform/memory (L1) | engine/graphics/pipeline (L6) | 4.0 | tu_1147D8->shd 2, tu_114AE0->shd 2 | SPECIFIC |
| 44 | gameplay/ai (L9) | app (L11) | 4.0 | tu_1553F0->bif 4 | SCRIPT-BUILTINS |
| 45 | world/state (L5) | engine/physics (L6) | 3.7 | game->so 4 | WORLD-UP |
| 46 | level (L7) | ui (L10) | 3.0 | update->memcard 2, brx_sw->memcard 1 | UI-PUSH |
| 47 | object (L4) | engine/camera (L6) | 3.0 | busy->cplcy 2, lo->cplcy 1 | ONE-HIERARCHY |
| 48 | world/scene (L5) | app (L11) | 3.0 | callback->bif 2, sw->tu_1A4E08 1 | WORLD-UP |
| 49 | world/state (L5) | gameplay/progression (L9) | 3.0 | difficulty->prize 2, game->booty 1 | WORLD-UP |
| 50 | level (L7) | actor (L8) | 3.0 | exit_warp->po 2, update->po 1 | HERO-HOOK |
| 51 | core/linkonce (L2) | actor (L8) | 3.0 | game_linkonce->po 2, game_linkonce->step 1 | HERO-HOOK |
| 52 | core/linkonce (L2) | engine/physics (L6) | 3.0 | game_linkonce->so 3 | SPECIFIC |
| 53 | object (L4) | world/state (L5) | 3.0 | lo->game 3 | WORLD |
| 54 | world/scene (L5) | gameplay/progression (L9) | 3.0 | sw->prize 2, sw->cheat 1 | WORLD-UP |

Rows 26, 51 and 52 were measured when the unhomed linkonce copies sat in game/core/linkonce/. That directory is gone
(inline passes 1 and 3): the copies are inline functions or function templates in the class headers (include/sly2/),
emitted through src/game/app/eopid_linkonce.cpp, so those edges now come from app/ (L11) and are downward.

Specific notes (CONFIRMED from edge examples):
- core/containers -> object: `tu_119BC8` (util library) defines a class whose vtable inherits BASIC slots; `bq`/`dynarray` call
  through BASIC-shaped vtables. The util library therefore already depended on BASIC in the original source.
- core/binstream -> object: `bis` calls virtual slots on a BASIC-derived owner while decompressing (Decompress/Refill).
- platform/input -> splice: rumble's "enabled" flag is a save variable read through Splice refs.
- actor -> gameplay/ai: STEP owns a behaviour stack (InitStep -> InitBhvstk); BHVSTK lives with the guard behaviours. Moving
  `bhvstk` to actor would trade 9 for ~30 weight (ai->bhvstk is heavy); left as is.
- world/scene -> engine/*: DeleteSw/PostSwLoad/ExecuteSwTransition reset camera, sound, shaders, physics filters.
- platform/memory -> engine/graphics/pipeline: two static-init-only TUs (`tu_1147D8`, `tu_114AE0`) touch shd tables; unknown purpose.

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

