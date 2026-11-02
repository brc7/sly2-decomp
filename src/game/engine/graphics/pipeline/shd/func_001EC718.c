#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */

/** @sly2-annot addr=0x1EC718
 * Purpose: CopyAb(dst=+8, src=+4, cb=+0)
 * Info: proposed name ApplyCopyRecord (T2); TU shd
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001EC718(void *arg0) {
    CopyAb__FPvT0Ui(M2C_FIELD(arg0, s32 *, 8), M2C_FIELD(arg0, s32 *, 4), M2C_FIELD(arg0, s32 *, 0));
}
