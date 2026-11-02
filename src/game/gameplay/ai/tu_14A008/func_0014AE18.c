#include "m2c_prelude.h"
/** @sly2-annot addr=0x14AE18
 * Purpose: kind field of the current route point
 * Info: proposed name KindBhv182RoutePoint (T2); TU tu_14A008
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
s32 func_0014AE18(void *arg0) {
    return M2C_FIELD(((M2C_FIELD(arg0, s32 *, 0x3C) * 0xC) + M2C_FIELD(M2C_FIELD(arg0, void **, 0x38), s32 *, 0x58)), s32 *, 8);
}
