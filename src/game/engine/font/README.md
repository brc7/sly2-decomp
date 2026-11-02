# `src/game/engine/font/`

**Layer 6.** font rendering and Shift-JIS

| Directory | Files |
| --- | --- |
| [`font/`](font/) | 24 |
| [`sjis/`](sjis/) | 4 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
