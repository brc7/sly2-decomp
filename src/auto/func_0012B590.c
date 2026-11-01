#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, M2C_UNK);         /* extern */

/** @sly2-annot addr=0x12B590
 * Purpose: CopyAb of the record
 * Info: proposed name CloneAcConst (T2); class AcConst; TU act
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012B590(s32 arg0, s32 arg1, void *arg2) {
    if (arg1 != 0) {
        CopyAb__FPvT0Ui(arg1, arg0 + 4, 0xC);
    }
    if (arg2 != NULL) {
        M2C_FIELD(arg2, s32 *, 8) = 0;
        M2C_FIELD(arg2, s32 *, 4) = 0;
        M2C_FIELD(arg2, s32 *, 0) = 0;
    }
}
