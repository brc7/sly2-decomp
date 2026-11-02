#include "m2c_prelude.h"
/** @sly2-annot addr=0x12DFE0
 * Purpose: vtable 0x2CA730 slot 5
 * Info: proposed name ActXform_Slot5 (T2); class ActXform; TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012DFE0(void *arg0, s128 *arg1, void *arg2) {
    if (arg1 != NULL) {
        *arg1 = M2C_FIELD(M2C_FIELD(arg0, void **, 8), s128 *, 0xC0);
    }
    if (arg2 != NULL) {
        M2C_FIELD(arg2, s32 *, 8) = 0;
        M2C_FIELD(arg2, s32 *, 4) = 0;
        M2C_FIELD(arg2, s32 *, 0) = 0;
    }
}
