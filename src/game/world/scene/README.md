# `src/game/world/scene/`

**Layer 5.** SW: the world container (g_sw, object lists, load queues) and its callback queue

| Directory | Files |
| --- | --- |
| [`callback.cpp`](callback.cpp) | 1 |
| [`sw.cpp`](sw.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
