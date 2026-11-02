# `src/game/splice/`

**Layer 3.** Splice VM: values/refs, pairs, frames, evaluator, GC, procedures, methods, serialisation

| Directory | Files |
| --- | --- |
| [`empty_126188.cpp`](empty_126188.cpp) | 1 |
| [`empty_1262E0.cpp`](empty_1262E0.cpp) | 1 |
| [`eval.cpp`](eval.cpp) | 1 |
| [`frame.cpp`](frame.cpp) | 1 |
| [`gc.cpp`](gc.cpp) | 1 |
| [`method.cpp`](method.cpp) | 1 |
| [`pair.cpp`](pair.cpp) | 1 |
| [`proc.cpp`](proc.cpp) | 1 |
| [`ref/`](ref/) | 38 |
| [`serialize/`](serialize/) | 3 |
| [`sidebag/`](sidebag/) | 12 |
| [`spliceutils.cpp`](spliceutils.cpp) | 1 |
| [`varb.cpp`](varb.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
