# `src/game/platform/input/`

**Layer 1.** pad (JOY) and rumble

| Directory | Files |
| --- | --- |
| [`joy/`](joy/) | 19 |
| [`rumble.cpp`](rumble.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
