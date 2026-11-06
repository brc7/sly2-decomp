# `src/game/app/`

**Layer 11.** composition root: main loop, boot sequence, class registry and Splice binding thunks (class_registry.cpp: the whole crt0_splicewrap TU, with the generated wrappers of src/generated/splicewrap included), Splice built-ins (bif, bif2)

| Directory | Files |
| --- | --- |
| [`bif.cpp`](bif.cpp) | 1 |
| [`bif2.cpp`](bif2.cpp) | 1 |
| [`class_registry.cpp`](class_registry.cpp) | 1 |
| [`commons.cpp`](commons.cpp) | 1 (data only: the game's commons, not an original TU; docs/guides/tu-merging.md, "Phase 9") |
| [`main.cpp`](main.cpp) | 1 |
| [`tu_1A4E08.cpp`](tu_1A4E08.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
