# `src/game/platform/cdvd/`

**Layer 1.** CD driver, CD file table (fid -> name/size)

| Directory | Files |
| --- | --- |
| [`cd/`](cd/) | 10 |
| [`tu_14B660.cpp`](tu_14B660.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
