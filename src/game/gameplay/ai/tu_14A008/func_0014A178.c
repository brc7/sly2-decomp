#include "m2c_prelude.h"
/** @sly2-annot addr=0x14A178
 * Purpose: Splice: append {a, b, -1?} (max 256, 12 bytes)
 * Info: proposed name AddRoutePoint (T2); TU tu_14A008
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014A178(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_a3;
    void *temp_v1;

    temp_a3 = M2C_FIELD(arg0, s32 *, 0x54);
    if (temp_a3 < 0x100) {
        temp_v1 = M2C_FIELD(arg0, s32 *, 0x58) + (temp_a3 * 0xC);
        M2C_FIELD(temp_v1, s32 *, 0) = arg1;
        M2C_FIELD(temp_v1, s32 *, 4) = arg2;
        M2C_FIELD(arg0, s32 *, 0x54) = (s32) (M2C_FIELD(arg0, s32 *, 0x54) + 1);
    }
}
