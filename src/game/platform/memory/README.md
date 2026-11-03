# `src/game/platform/memory/`

**Layer 1.** heaps (HEAP/CSwHeap, slot heaps, phase memory, scratchpad buffer), PvAllocClear, malloc/free

| Directory | Files |
| --- | --- |
| [`heap.cpp`](heap.cpp) | 1 |
| [`memory.cpp`](memory.cpp) | 1 |
| [`phasemem.cpp`](phasemem.cpp) | 1 |
| [`slotheap.cpp`](slotheap.cpp) | 1 |
| [`sprbuf.cpp`](sprbuf.cpp) | 1 |
| [`tu_1147D8.cpp`](tu_1147D8.cpp) | 1 |
| [`tu_114AE0.cpp`](tu_114AE0.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
