# `src/game/engine/sound/`

**Layer 6.** sound manager, banks/streams, ambient emitters, speakers

| Directory | Files |
| --- | --- |
| [`amb/`](amb/) | 10 |
| [`sound/`](sound/) | 103 |
| [`tu_2275B8.cpp`](tu_2275B8.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
