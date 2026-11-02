# `src/game/core/containers/`

**Layer 2.** intrusive lists (DL/SL), CDynArray, byte queues, hashing, small key/string class

| Directory | Files |
| --- | --- |
| [`bq/`](bq/) | 10 |
| [`dl/`](dl/) | 21 |
| [`dynarray/`](dynarray/) | 7 |
| [`empty_119DA0.cpp`](empty_119DA0.cpp) | 1 |
| [`empty_119EF8/`](empty_119EF8/) | 2 |
| [`empty_11B1A8/`](empty_11B1A8/) | 2 |
| [`tu_119BC8/`](tu_119BC8/) | 16 |
| [`tu_11AFE0/`](tu_11AFE0/) | 3 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
