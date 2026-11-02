# `src/game/engine/`

**Layer 6.** engine services: graphics, fx, anim, physics, sound, camera, text, presentation (HUD/widgets/screen manager)

| Directory | Files |
| --- | --- |
| [`anim/`](anim/) | 263 |
| [`camera/`](camera/) | 150 |
| [`font/`](font/) | 28 |
| [`fx/`](fx/) | 152 |
| [`graphics/`](graphics/) | 271 |
| [`physics/`](physics/) | 450 |
| [`presentation/`](presentation/) | 241 |
| [`sound/`](sound/) | 121 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
