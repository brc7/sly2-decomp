# `src/game/core/linkonce/`

**Layer 2.** compiler-emitted linkonce instances (inline functions / templates) with no static referrer outside generated code (section 4b)

| Directory | Files |
| --- | --- |
| [`unhomed/`](unhomed/) | 37 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
