# `src/game/app/`

**Layer 11.** composition root: main loop, boot sequence, class registry (New*/Get*ClassDesc, D_002BAAE8), Splice built-ins (bif, bif2) and binding thunks

| Directory | Files |
| --- | --- |
| [`bif/`](bif/) | 161 |
| [`bif2/`](bif2/) | 31 |
| [`class_registry/`](class_registry/) | 424 |
| [`main/`](main/) | 7 |
| [`splice_thunks/`](splice_thunks/) | 31 |
| [`tu_1A4E08.cpp`](tu_1A4E08.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
