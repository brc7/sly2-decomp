# `src/sdk/`

**Layer 0.** Sony / 989 SDK libraries (other compilers; replaced by HLE in a port)

| Directory | Files |
| --- | --- |
| [`989snd/`](989snd/) | 40 |
| [`collect2/`](collect2/) | 2 |
| [`libc/`](libc/) | 17 |
| [`libcdvd/`](libcdvd/) | 21 |
| [`libdma/`](libdma/) | 10 |
| [`libgcc/`](libgcc/) | 68 |
| [`libgraph/`](libgraph/) | 17 |
| [`libipu/`](libipu/) | 7 |
| [`libkernel/`](libkernel/) | 281 |
| [`libm/`](libm/) | 40 |
| [`libmc/`](libmc/) | 15 |
| [`libmpeg/`](libmpeg/) | 104 |
| [`libpad/`](libpad/) | 21 |
| [`libscf/`](libscf/) | 3 |

Lower layers must not depend on higher ones; see [the architecture doc](../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
