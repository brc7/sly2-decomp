# `src/game/splice/`

**Layer 3.** Splice VM: values/refs, pairs, frames, evaluator, GC, procedures, methods, serialisation

| Directory | Files |
| --- | --- |
| [`empty_126188.cpp`](empty_126188.cpp) | 1 |
| [`empty_1262E0.cpp`](empty_1262E0.cpp) | 1 |
| [`eval/`](eval/) | 27 |
| [`frame/`](frame/) | 15 |
| [`gc/`](gc/) | 19 |
| [`method.cpp`](method.cpp) | 1 |
| [`pair.cpp`](pair.cpp) | 1 |
| [`proc/`](proc/) | 4 |
| [`ref/`](ref/) | 38 |
| [`serialize/`](serialize/) | 3 |
| [`sidebag/`](sidebag/) | 12 |
| [`spliceutils/`](spliceutils/) | 10 |
| [`varb/`](varb/) | 4 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
