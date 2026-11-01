#include "m2c_prelude.h"
s32 FBhvpathAtEnd(s32);                             /* extern */

/** @sly2-annot addr=0x17DCB8
 * Purpose: Walking direct (state 1) and at the path end
 * Info: proposed name FBhvgotoArrived (T1); TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloLookAtIgnore (names_v2:B(unverified)) | runtime2/4 confirmed the BHVGOTO role on live guards
 *   (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 FBhvgotoArrived(void *arg0) {
    s32 temp_s0;
    s32 var_v1;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0x30);
    var_v1 = 0;
    if (temp_s0 == 1) {
        var_v1 = (FBhvpathAtEnd(M2C_FIELD(arg0, s32 *, 0x18)) != 0) ? temp_s0 : 0;
    }
    return var_v1;
}
