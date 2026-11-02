# `src/game/gameplay/objects/`

**Layer 9.** interactive world objects and minigames: buttons, barriers, bombs, missiles, sensors, turrets, targets, rhythm, hacking, RC tank...

| Directory | Files |
| --- | --- |
| [`barrier/`](barrier/) | 9 |
| [`bomb/`](bomb/) | 22 |
| [`button/`](button/) | 50 |
| [`empty_1D7A30.cpp`](empty_1D7A30.cpp) | 1 |
| [`missile/`](missile/) | 16 |
| [`rhythm/`](rhythm/) | 35 |
| [`sensor/`](sensor/) | 21 |
| [`target/`](target/) | 11 |
| [`tu_14C548.cpp`](tu_14C548.cpp) | 1 |
| [`tu_1539A8.cpp`](tu_1539A8.cpp) | 1 |
| [`tu_153B08.cpp`](tu_153B08.cpp) | 1 |
| [`tu_15DC80.cpp`](tu_15DC80.cpp) | 1 |
| [`tu_17F090/`](tu_17F090/) | 76 |
| [`tu_1B0B78/`](tu_1B0B78/) | 25 |
| [`tu_1BAB50/`](tu_1BAB50/) | 34 |
| [`tu_1C2D20/`](tu_1C2D20/) | 7 |
| [`tu_1D1370/`](tu_1D1370/) | 44 |
| [`tu_1FF3B8/`](tu_1FF3B8/) | 8 |
| [`tu_223F70/`](tu_223F70/) | 28 |
| [`turret/`](turret/) | 12 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
