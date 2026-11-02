# `src/game/engine/graphics/backend/`

**Layer 6.** GS / DMA / GIF / VIF packet builders and the frame (FRM) system

| Directory | Files |
| --- | --- |
| [`dmas.cpp`](dmas.cpp) | 1 |
| [`empty_1578F8.cpp`](empty_1578F8.cpp) | 1 |
| [`frm/`](frm/) | 13 |
| [`gifs.cpp`](gifs.cpp) | 1 |
| [`gs/`](gs/) | 26 |
| [`vifs.cpp`](vifs.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
