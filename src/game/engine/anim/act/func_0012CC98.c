#include "m2c_prelude.h"
/** @sly2-annot addr=0x12CC98
 * Purpose: Returns the stored float
 * Info: proposed name EvaluateAcFloatConst (T2); class AcFloatConst; TU act
 * Notes: names_v2 C proposed PtnfnFromTn__FP2TN (0.638)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012CC98(void *arg0, f32 *arg1, s32 *arg2) {
    if (arg1 != NULL) {
        *arg1 = M2C_FIELD(arg0, f32 *, 4);
    }
    if (arg2 != NULL) {
        *arg2 = 0;
    }
}
