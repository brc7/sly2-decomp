#include "m2c_prelude.h"
/** @sly2-annot addr=0x177D20
 * Purpose: Guard has designated targets (+0xF94 or +0xFA8 count nonzero)
 * Info: proposed name FBhvguardHasTargetList (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGUARD role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.50] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 FBhvguardHasTargetList(void *arg0) {
    s32 var_a0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x10);
    var_a0 = 0;
    if ((M2C_FIELD(temp_v1, s32 *, 0xFA8) != 0) || (M2C_FIELD(temp_v1, s32 *, 0xF94) != 0)) {
        var_a0 = 1;
    }
    return var_a0;
}
