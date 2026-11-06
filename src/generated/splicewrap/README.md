# `src/generated/splicewrap/`

**Layer 11.** tools/gen_splicewrap.py output (373 wrappers); never edit by hand. Not compiled on their own: each
`.inc` is a fragment of the crt0_splicewrap translation unit, [`src/game/app/class_registry.cpp`](../../game/app/class_registry.cpp),
which includes it at its address (see [tu-merging.md, "class_registry"](../../../docs/guides/tu-merging.md#class_registry)).

Lower layers must not depend on higher ones; see [the architecture doc](../../../docs/architecture.md) for the layer rules and the known exceptions. Each leaf directory under `src/game/` is one original translation unit (source file); names like `tu_1D7238` are units we have not named yet.
