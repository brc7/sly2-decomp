# `src/game/object/`

**Layer 4.** BASIC/LO base classes, XFM transforms, proxies, find-by-OID, BRX object deserialisation, busy list, script peers (spliceobj/splicemap)

| Directory | Files |
| --- | --- |
| [`basic/`](basic/) | 31 |
| [`brx/`](brx/) | 16 |
| [`busy/`](busy/) | 8 |
| [`find/`](find/) | 13 |
| [`lo/`](lo/) | 74 |
| [`proxy/`](proxy/) | 38 |
| [`splicemap.cpp`](splicemap.cpp) | 1 |
| [`spliceobj/`](spliceobj/) | 18 |
| [`xform/`](xform/) | 18 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
