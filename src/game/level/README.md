# `src/game/level/`

**Layer 7.** per-frame world update, world-file load glue, warps/exits, wipes (level flow)

| Directory | Files |
| --- | --- |
| [`brx_sw.cpp`](brx_sw.cpp) | 1 |
| [`empty_1FF690.cpp`](empty_1FF690.cpp) | 1 |
| [`empty_226AB8.cpp`](empty_226AB8.cpp) | 1 |
| [`exit_warp/`](exit_warp/) | 22 |
| [`update.cpp`](update.cpp) | 1 |
| [`wipe/`](wipe/) | 8 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
