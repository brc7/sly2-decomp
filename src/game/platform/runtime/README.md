# `src/game/platform/runtime/`

**Layer 1.** Sucker Punch's own printf/string runtime (CchOstrmPrintf, vsprintf, strlen) and OSTRM streams

| Directory | Files |
| --- | --- |
| [`stream/`](stream/) | 7 |
| [`text/`](text/) | 24 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
