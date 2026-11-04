# `src/game/object/`

**Layer 4.** BASIC/LO base classes, XFM transforms, proxies, find-by-OID, BRX object deserialisation, busy list, script peers (spliceobj/splicemap)

| Directory | Files |
| --- | --- |
| [`basic.cpp`](basic.cpp) | 1 |
| [`brx.cpp`](brx.cpp) | 1 |
| [`busy.cpp`](busy.cpp) | 1 |
| [`find.cpp`](find.cpp) | 1 |
| [`lo/`](lo/) | 74 |
| [`proxy/`](proxy/) | 38 |
| [`splicemap.cpp`](splicemap.cpp) | 1 |
| [`spliceobj.cpp`](spliceobj.cpp) | 1 |
| [`xform.cpp`](xform.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
