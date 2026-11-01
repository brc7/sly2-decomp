#include "m2c_prelude.h"
s32 func_00174898(s32);                             /* extern */
s32 func_001EF0B8(void *, s32, void *);             /* extern */

/** @sly2-annot addr=0x1756C0
 * Purpose: BHVATTACK message handler: msg 0xB from the victim naming this guard = hit landed (+0x40=1)
 * Info: proposed name BHVATTACK_FHandleMessage (T1); class BHVATTACK slot13; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVATTACK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 func_001756C0(void *arg0, s32 arg1, void *arg2) {
    s32 temp_a0;

    if (arg1 == 0xB) {
        temp_a0 = M2C_FIELD(arg2, s32 *, 4);
        if (temp_a0 == M2C_FIELD(arg0, s32 *, 0x30)) {
            if (func_00174898(temp_a0) == M2C_FIELD(arg0, s32 *, 0x10)) {
                M2C_FIELD(arg0, s32 *, 0x40) = 1;
                return 1;
            }
            goto block_5;
        }
    }
block_5:
    return func_001EF0B8(arg0, arg1, arg2);
}
