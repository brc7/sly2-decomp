# `src/game/engine/graphics/`

**Layer 6.** Graphics: backend (GS/DMA/GIF/VIF packet builders, frame system) and pipeline (render passes, globs, shaders, lights, sky, visibility).

| Directory | Files |
| --- | --- |
| [`backend/`](backend/) | 64 |
| [`pipeline/`](pipeline/) | 207 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
