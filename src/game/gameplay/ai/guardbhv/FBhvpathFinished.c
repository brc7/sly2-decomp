#include "m2c_prelude.h"
/** @sly2-annot addr=0x178D80
 * Purpose: BHVPATH state == 3
 * Info: proposed name FBhvpathFinished (T1); TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 FBhvpathFinished(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x4C) == 3;
}
