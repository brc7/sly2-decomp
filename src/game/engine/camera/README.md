# `src/game/engine/camera/`

**Layer 6.** camera (CM), camera policies (CPLCY), TN camera nodes, CPCAMERA

| Directory | Files |
| --- | --- |
| [`cm/`](cm/) | 55 |
| [`cplcy/`](cplcy/) | 51 |
| [`tn/`](tn/) | 31 |
| [`tu_12E600/`](tu_12E600/) | 13 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
