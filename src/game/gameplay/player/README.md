# `src/game/gameplay/player/`

**Layer 9.** STEPPLAYER, Sly/Bentley/Murray (JT, Bentley, Murray), moves (air, grab, hang, pipe, rail, throw, hurt...), grab spots, recon

| Directory | Files |
| --- | --- |
| [`air/`](air/) | 44 |
| [`bentley/`](bentley/) | 48 |
| [`cane.cpp`](cane.cpp) | 1 |
| [`dirblend.cpp`](dirblend.cpp) | 1 |
| [`empty_1344F0.cpp`](empty_1344F0.cpp) | 1 |
| [`empty_1378D8.cpp`](empty_1378D8.cpp) | 1 |
| [`grab/`](grab/) | 28 |
| [`grabspot/`](grabspot/) | 74 |
| [`hang.cpp`](hang.cpp) | 1 |
| [`hnd.cpp`](hnd.cpp) | 1 |
| [`hurt/`](hurt/) | 28 |
| [`jt/`](jt/) | 51 |
| [`jtgoal.cpp`](jtgoal.cpp) | 1 |
| [`jthands/`](jthands/) | 17 |
| [`mode.cpp`](mode.cpp) | 1 |
| [`murray/`](murray/) | 66 |
| [`pipe.cpp`](pipe.cpp) | 1 |
| [`pipe_jt/`](pipe_jt/) | 15 |
| [`places.cpp`](places.cpp) | 1 |
| [`rail.cpp`](rail.cpp) | 1 |
| [`stepplayer/`](stepplayer/) | 96 |
| [`throw.cpp`](throw.cpp) | 1 |
| [`tu_19F0A8.cpp`](tu_19F0A8.cpp) | 1 |
| [`tu_201D78/`](tu_201D78/) | 26 |
| [`tu_20B840/`](tu_20B840/) | 31 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
