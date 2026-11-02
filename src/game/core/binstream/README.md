# `src/game/core/binstream/`

**Layer 2.** CBinaryInputStream (bis), async CD stream (bas), CStreamBis

| Directory | Files |
| --- | --- |
| [`bas.cpp`](bas.cpp) | 1 |
| [`bis.cpp`](bis.cpp) | 1 |
| [`tu_1848A8/`](tu_1848A8/) | 10 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
