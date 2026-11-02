# `src/game/engine/anim/`

**Layer 6.** actuators (ACT), animation segments (ASEG/ASEGA), blends, IK, state machines (SM), chains, WR, XF transform/motion slots

| Directory | Files |
| --- | --- |
| [`act/`](act/) | 72 |
| [`aseg/`](aseg/) | 23 |
| [`asega/`](asega/) | 27 |
| [`blend/`](blend/) | 12 |
| [`empty_12F0F0.cpp`](empty_12F0F0.cpp) | 1 |
| [`empty_131F70.cpp`](empty_131F70.cpp) | 1 |
| [`ik/`](ik/) | 8 |
| [`sm/`](sm/) | 43 |
| [`tail/`](tail/) | 6 |
| [`tu_1603B8.cpp`](tu_1603B8.cpp) | 1 |
| [`wr/`](wr/) | 20 |
| [`xf/`](xf/) | 44 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
