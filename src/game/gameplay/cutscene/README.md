# `src/game/gameplay/cutscene/`

**Layer 9.** relays (scripted event sequences), MPEG FMV, framebuffer capture screen

| Directory | Files |
| --- | --- |
| [`mpeg.cpp`](mpeg.cpp) | 1 |
| [`relay/`](relay/) | 141 |
| [`tu_1471C0.cpp`](tu_1471C0.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
