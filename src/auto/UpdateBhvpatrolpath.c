#include "m2c_prelude.h"
s32 FBhvgotoArrived(s32);                             /* extern */
M2C_UNK AdvanceBhvpatrolpathGoal(void *, M2C_UNK);             /* extern */
M2C_UNK UpdateBhv();                            /* extern */

/** @sly2-annot addr=0x17ECB0
 * Purpose: BHVPATROLPATH Update: on arrival advance the goal along the shape
 * Info: proposed name UpdateBhvpatrolpath (T1); class BHVPATROLPATH slot10; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATROLPATH role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void UpdateBhvpatrolpath(void *arg0) {
    UpdateBhv();
    if (FBhvgotoArrived(M2C_FIELD(arg0, s32 *, 0x18)) != 0) {
        AdvanceBhvpatrolpathGoal(arg0, 0);
    }
}
