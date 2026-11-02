# `src/game/gameplay/cutscene/`

**Layer 9.** relays (scripted event sequences), MPEG FMV, framebuffer capture screen

| Directory | Files |
| --- | --- |
| [`mpeg/`](mpeg/) | 34 |
| [`relay/`](relay/) | 141 |
| [`tu_1471C0/`](tu_1471C0/) | 11 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
