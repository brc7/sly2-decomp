# `src/game/engine/fx/`

**Layer 6.** particles and effects: emitters, RIPs, blips, trails

| Directory | Files |
| --- | --- |
| [`blip/`](blip/) | 21 |
| [`emitter/`](emitter/) | 54 |
| [`rip/`](rip/) | 64 |
| [`trail/`](trail/) | 13 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
