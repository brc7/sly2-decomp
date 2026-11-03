# `src/game/gameplay/ai/`

**Layer 9.** STEPGUARD, guard behaviours (BHV stack, guardbhv), detectors, noise stimuli, pickpocket, named bosses/NPCs, spawners

| Directory | Files |
| --- | --- |
| [`bhvstk/`](bhvstk/) | 25 |
| [`detector/`](detector/) | 19 |
| [`gomer.cpp`](gomer.cpp) | 1 |
| [`guardbhv/`](guardbhv/) | 190 |
| [`noise/`](noise/) | 23 |
| [`pickpocket.cpp`](pickpocket.cpp) | 1 |
| [`stepguard/`](stepguard/) | 158 |
| [`tu_144B28/`](tu_144B28/) | 43 |
| [`tu_14A008/`](tu_14A008/) | 21 |
| [`tu_14FAC8.cpp`](tu_14FAC8.cpp) | 1 |
| [`tu_1553F0/`](tu_1553F0/) | 19 |
| [`tu_1709A8/`](tu_1709A8/) | 42 |
| [`tu_171F58/`](tu_171F58/) | 16 |
| [`tu_173158/`](tu_173158/) | 21 |
| [`tu_1B9F80.cpp`](tu_1B9F80.cpp) | 1 |
| [`tu_1CBEB0.cpp`](tu_1CBEB0.cpp) | 1 |
| [`tu_1D7238.cpp`](tu_1D7238.cpp) | 1 |
| [`tu_20CE28.cpp`](tu_20CE28.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
