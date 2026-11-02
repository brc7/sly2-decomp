#include "m2c_prelude.h"
/** @sly2-annot addr=0x12D4D8
 * Purpose: vtable 0x2CA7F0 slot 4
 * Info: proposed name ActLookAt_Slot4 (T2); class ActLookAt; TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012D4D8(void *arg0, s128 *arg1, s128 *arg2) {
    if (arg1 != NULL) {
        *arg1 = M2C_FIELD(M2C_FIELD(arg0, void **, 8), s128 *, 0xC0);
    }
    if (arg2 != NULL) {
        *arg2 = M2C_FIELD(M2C_FIELD(arg0, void **, 8), s128 *, 0xD0);
    }
}
