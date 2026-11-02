#include "m2c_prelude.h"
/** @sly2-annot addr=0x1B42C0
 * Purpose: returns stream(+0x114)!=0
 * Info: proposed name CMpeg_FActive (T2); class CMpeg; TU mpeg
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
s32 func_001B42C0(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x114) != 0;
}
