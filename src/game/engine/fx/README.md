# `src/game/engine/fx/`

**Layer 6.** particles and effects: emitters, RIPs, blips, trails

| Directory | Files |
| --- | --- |
| [`blip.cpp`](blip.cpp) | 1 |
| [`emitter.cpp`](emitter.cpp) | 1 |
| [`rip.cpp`](rip.cpp) | 1 |
| [`trail.cpp`](trail.cpp) | 1 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
