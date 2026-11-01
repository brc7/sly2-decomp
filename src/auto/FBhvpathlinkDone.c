#include "m2c_prelude.h"
/** @sly2-annot addr=0x17E570
 * Purpose: state == -1
 * Info: proposed name FBhvpathlinkDone (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATHLINK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 FBhvpathlinkDone(void *arg0) {
    return ~M2C_FIELD(arg0, s32 *, 0x38) == 0;
}
