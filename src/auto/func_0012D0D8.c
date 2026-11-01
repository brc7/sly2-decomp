#include "m2c_prelude.h"
/** @sly2-annot addr=0x12D0D8
 * Purpose: vtable 0x2CA830 slot 5
 * Info: proposed name ActUnk_2CA830_5 (T2); TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012D0D8(void *arg0, s128 *arg1, void *arg2) {
    if (arg1 != NULL) {
        *arg1 = M2C_FIELD(arg0, s128 *, 0x20);
    }
    if (arg2 != NULL) {
        M2C_FIELD(arg2, s32 *, 8) = 0;
        M2C_FIELD(arg2, s32 *, 4) = 0;
        M2C_FIELD(arg2, s32 *, 0) = 0;
    }
}
