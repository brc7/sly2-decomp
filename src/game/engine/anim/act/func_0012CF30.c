#include "m2c_prelude.h"
s128 func_0022CA18(s32, s32);                       /* extern */

/** @sly2-annot addr=0x12CF30
 * Purpose: Default slot-5: forwards to 0x22CA18
 * Info: proposed name UpdateAct (T2); class ACT; TU act
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012CF30(void *arg0, s128 *arg1, void *arg2) {
    s32 temp_a0;

    if (arg1 != NULL) {
        temp_a0 = M2C_FIELD(arg0, s32 *, 8);
        *arg1 = func_0022CA18(temp_a0, temp_a0);
    }
    if (arg2 != NULL) {
        M2C_FIELD(arg2, s32 *, 8) = 0;
        M2C_FIELD(arg2, s32 *, 4) = 0;
        M2C_FIELD(arg2, s32 *, 0) = 0;
    }
}
