# `src/game/core/math/`

**Layer 2.** vectors, matrices, Bezier, Crout solver, BSP/planes, DZG LCP core, misc util

| Directory | Files |
| --- | --- |
| [`bez/`](bez/) | 5 |
| [`bsp/`](bsp/) | 14 |
| [`crout/`](crout/) | 4 |
| [`dzgcore/`](dzgcore/) | 7 |
| [`mat/`](mat/) | 33 |
| [`util/`](util/) | 28 |
| [`vec/`](vec/) | 22 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
