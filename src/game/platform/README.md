# `src/game/platform/`

**Layer 1.** PS2 machine services: boot, heaps, threads, disc (CD/DVD) I/O, pad, printf/string runtime

| Directory | Files |
| --- | --- |
| [`boot/`](boot/) | 1 |
| [`cdvd/`](cdvd/) | 15 |
| [`input/`](input/) | 30 |
| [`memory/`](memory/) | 50 |
| [`runtime/`](runtime/) | 31 |
| [`thread/`](thread/) | 7 |

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
