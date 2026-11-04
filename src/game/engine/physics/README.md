# `src/game/engine/physics/`

**Layer 6.** SO solid objects, solver, shapes/geometry, BSP marks/contacts, clipping, paths, volumes, water, launchers, mesh contacts

| Directory | Files |
| --- | --- |
| [`bbmark.cpp`](bbmark.cpp) | 1 |
| [`clip.cpp`](clip.cpp) | 1 |
| [`dzg.cpp`](dzg.cpp) | 1 |
| [`empty_1C2920.cpp`](empty_1C2920.cpp) | 1 |
| [`geom.cpp`](geom.cpp) | 1 |
| [`mark.cpp`](mark.cpp) | 1 |
| [`path.cpp`](path.cpp) | 1 |
| [`shape/`](shape/) | 56 |
| [`so/`](so/) | 174 |
| [`solve.cpp`](solve.cpp) | 1 |
| [`tu_185130.cpp`](tu_185130.cpp) | 1 |
| [`tu_1BC050.cpp`](tu_1BC050.cpp) | 1 |
| [`tu_1D6848.cpp`](tu_1D6848.cpp) | 1 |
| [`vol.cpp`](vol.cpp) | 1 |
| [`water.cpp`](water.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
