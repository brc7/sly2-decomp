# `src/game/actor/`

**Layer 8.** PO and STEP: the shared bases of the player characters and the guards

| Directory | Files |
| --- | --- |
| [`po/`](po/) | 38 |
| [`step/`](step/) | 46 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
