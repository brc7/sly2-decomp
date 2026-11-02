#include "m2c_prelude.h"
/** @sly2-annot addr=0x20CA98
 * Purpose: job +0x24 == 0
 * Info: proposed name FDirAnimStopped (T2); TU tu_20B840
 * Notes: T2 forced: verifier: coined class word without descriptor
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
s32 func_0020CA98(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x24) == 0;
}
