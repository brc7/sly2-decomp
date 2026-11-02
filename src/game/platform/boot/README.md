# `src/game/platform/boot/`

**Layer 1.** crt0 `_start`

| Directory | Files |
| --- | --- |
| [`crt0/`](crt0/) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
