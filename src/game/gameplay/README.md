# `src/game/gameplay/`

**Layer 9.** player, AI, gameplay objects, progression, cutscenes

| Directory | Files |
| --- | --- |
| [`ai/`](ai/) | 639 |
| [`cutscene/`](cutscene/) | 186 |
| [`objects/`](objects/) | 413 |
| [`player/`](player/) | 639 |
| [`progression/`](progression/) | 109 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
