# `src/game/world/`

**Layer 5.** the SW world container and its registries; session state (game, save variables, difficulty)

| Directory | Files |
| --- | --- |
| [`scene/`](scene/) | 74 |
| [`state/`](state/) | 135 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
