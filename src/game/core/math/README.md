# `src/game/core/math/`

**Layer 2.** vectors, matrices, Bezier, Crout solver, BSP/planes, DZG LCP core, misc util

| Directory | Files |
| --- | --- |
| [`bez.cpp`](bez.cpp) | 1 |
| [`bsp.cpp`](bsp.cpp) | 1 |
| [`crout.cpp`](crout.cpp) | 1 |
| [`dzgcore.cpp`](dzgcore.cpp) | 1 |
| [`mat/`](mat/) | 33 |
| [`util.cpp`](util.cpp) | 1 |
| [`vec.cpp`](vec.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
