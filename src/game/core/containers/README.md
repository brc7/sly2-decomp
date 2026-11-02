# `src/game/core/containers/`

**Layer 2.** intrusive lists (DL/SL), CDynArray, byte queues, hashing, small key/string class

| Directory | Files |
| --- | --- |
| [`bq.cpp`](bq.cpp) | 1 |
| [`dl/`](dl/) | 21 |
| [`dynarray.cpp`](dynarray.cpp) | 1 |
| [`empty_119DA0.cpp`](empty_119DA0.cpp) | 1 |
| [`empty_119EF8.cpp`](empty_119EF8.cpp) | 1 |
| [`empty_11B1A8.cpp`](empty_11B1A8.cpp) | 1 |
| [`tu_119BC8/`](tu_119BC8/) | 16 |
| [`tu_11AFE0.cpp`](tu_11AFE0.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
