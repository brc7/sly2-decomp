#include "m2c_prelude.h"
M2C_UNK func_00175488(s32, M2C_UNK);                /* extern */
M2C_UNK func_001EEF70();                            /* extern */
M2C_UNK func_001EEFC0(s32);                         /* extern */

/** @sly2-annot addr=0x175410
 * Purpose: BHVATTACK Start: state 0 (strike) and set busy
 * Info: proposed name StartBhvattack (T1); class BHVATTACK slot8; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVATTACK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00175410(s32 arg0) {
    func_001EEF70();
    func_00175488(arg0, 0);
    func_001EEFC0(arg0);
}
