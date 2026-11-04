# `src/game/engine/graphics/pipeline/`

**Layer 6.** draw/render passes, globs, shaders (SHD/SHDANIM), lights, sky, visibility zones, culling sphere tree, item render targets

| Directory | Files |
| --- | --- |
| [`draw.cpp`](draw.cpp) | 1 |
| [`glob/`](glob/) | 42 |
| [`light.cpp`](light.cpp) | 1 |
| [`ms.cpp`](ms.cpp) | 1 |
| [`render.cpp`](render.cpp) | 1 |
| [`shd/`](shd/) | 45 |
| [`shdanim.cpp`](shdanim.cpp) | 1 |
| [`sky.cpp`](sky.cpp) | 1 |
| [`tu_1BE398.cpp`](tu_1BE398.cpp) | 1 |
| [`tu_1D57A8.cpp`](tu_1D57A8.cpp) | 1 |
| [`vis.cpp`](vis.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
