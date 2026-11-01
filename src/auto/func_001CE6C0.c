#include "m2c_prelude.h"
/** @sly2-annot addr=0x1CE6C0
 * Purpose: RIPG vt slot 62: returns (state(+0x150) != 1)
 * Info: proposed name FRipgUnk62 (T2); class RIPG slot62; TU rip
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
s32 func_001CE6C0(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x150) != 1;
}
