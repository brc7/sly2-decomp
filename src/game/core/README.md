# `src/game/core/`

**Layer 2.** engine-independent foundation: math, containers, binary streams, clock, locale, linkonce instances

| Directory | Files |
| --- | --- |
| [`binstream/`](binstream/) | 45 |
| [`containers/`](containers/) | 63 |
| [`linkonce/`](linkonce/) | 56 |
| [`locale/`](locale/) | 8 |
| [`math/`](math/) | 113 |
| [`time/`](time/) | 8 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
