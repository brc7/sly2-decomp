# `src/game/engine/presentation/`

**Layer 6.** presentation service: HUD widgets, SCREEN manager/base, letterbox, progress bar + loading screen

| Directory | Files |
| --- | --- |
| [`hud/`](hud/) | 124 |
| [`letterbox.cpp`](letterbox.cpp) | 1 |
| [`prog.cpp`](prog.cpp) | 1 |
| [`screen.cpp`](screen.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
