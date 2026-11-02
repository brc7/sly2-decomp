# `src/game/engine/graphics/pipeline/`

**Layer 6.** draw/render passes, globs, shaders (SHD/SHDANIM), lights, sky, visibility zones, culling sphere tree, item render targets

| Directory | Files |
| --- | --- |
| [`draw/`](draw/) | 7 |
| [`glob/`](glob/) | 42 |
| [`light/`](light/) | 32 |
| [`ms.cpp`](ms.cpp) | 1 |
| [`render/`](render/) | 7 |
| [`shd/`](shd/) | 45 |
| [`shdanim/`](shdanim/) | 33 |
| [`sky/`](sky/) | 4 |
| [`tu_1BE398/`](tu_1BE398/) | 16 |
| [`tu_1D57A8/`](tu_1D57A8/) | 10 |
| [`vis.cpp`](vis.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
