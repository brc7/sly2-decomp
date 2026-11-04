# `src/game/engine/anim/`

**Layer 6.** actuators (ACT), animation segments (ASEG/ASEGA), blends, IK, state machines (SM), chains, WR, XF transform/motion slots

| Directory | Files |
| --- | --- |
| [`act/`](act/) | 72 |
| [`aseg.cpp`](aseg.cpp) | 1 |
| [`asega.cpp`](asega.cpp) | 1 |
| [`blend.cpp`](blend.cpp) | 1 |
| [`empty_12F0F0.cpp`](empty_12F0F0.cpp) | 1 |
| [`empty_131F70.cpp`](empty_131F70.cpp) | 1 |
| [`ik.cpp`](ik.cpp) | 1 |
| [`sm.cpp`](sm.cpp) | 1 |
| [`tail.cpp`](tail.cpp) | 1 |
| [`tu_1603B8.cpp`](tu_1603B8.cpp) | 1 |
| [`wr.cpp`](wr.cpp) | 1 |
| [`xf.cpp`](xf.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
