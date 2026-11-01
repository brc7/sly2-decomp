#include "m2c_prelude.h"
/** @sly2-annot addr=0x174FB8
 * Purpose: BHVANIM state == 1 (animation finished)
 * Info: proposed name FBhvanimDone (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVANIM role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 FBhvanimDone(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x30) == 1;
}
