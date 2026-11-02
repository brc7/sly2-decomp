# `src/game/`

**Layer -.** Game code, grouped by layer: platform (1), core (2), splice (3), object (4), world (5), engine (6), level (7), actor (8), gameplay (9), ui (10), app (11).

| Directory | Files |
| --- | --- |
| [`actor/`](actor/) | 84 |
| [`app/`](app/) | 663 |
| [`core/`](core/) | 293 |
| [`engine/`](engine/) | 1676 |
| [`gameplay/`](gameplay/) | 1986 |
| [`level/`](level/) | 43 |
| [`object/`](object/) | 226 |
| [`platform/`](platform/) | 134 |
| [`splice/`](splice/) | 141 |
| [`ui/`](ui/) | 133 |
| [`world/`](world/) | 209 |

Lower layers must not depend on higher ones; see [the architecture doc](../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
