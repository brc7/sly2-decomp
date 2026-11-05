# `src/game/world/state/`

**Layer 5.** session state: GAME (world/level/jobs/events), save variables, difficulty

| Directory | Files |
| --- | --- |
| [`difficulty.cpp`](difficulty.cpp) | 1 |
| [`game.cpp`](game.cpp) | 1 |
| [`save.cpp`](save.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
