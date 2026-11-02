# `src/game/platform/thread/`

**Layer 1.** EE threads / semaphores wrapper

| Directory | Files |
| --- | --- |
| [`thread.cpp`](thread.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
