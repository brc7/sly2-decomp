# `src/game/engine/physics/`

**Layer 6.** SO solid objects, solver, shapes/geometry, BSP marks/contacts, clipping, paths, volumes, water, launchers, mesh contacts

| Directory | Files |
| --- | --- |
| [`bbmark/`](bbmark/) | 13 |
| [`clip/`](clip/) | 8 |
| [`dzg/`](dzg/) | 9 |
| [`empty_1C2920.cpp`](empty_1C2920.cpp) | 1 |
| [`geom.cpp`](geom.cpp) | 1 |
| [`mark/`](mark/) | 22 |
| [`path/`](path/) | 51 |
| [`shape/`](shape/) | 56 |
| [`so/`](so/) | 177 |
| [`solve/`](solve/) | 24 |
| [`tu_185130/`](tu_185130/) | 16 |
| [`tu_1BC050/`](tu_1BC050/) | 31 |
| [`tu_1D6848.cpp`](tu_1D6848.cpp) | 1 |
| [`vol.cpp`](vol.cpp) | 1 |
| [`water/`](water/) | 22 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
