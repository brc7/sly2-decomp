#include "m2c_prelude.h"
/** @sly2-annot addr=0x144068
 * Purpose: True if c270>0 (STEP behaviour stack) or ride object +0x1084 set
 * Info: proposed name FStepplayerPushing (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
s32 func_00144068(void *arg0) {
    s32 var_v1;

    var_v1 = 0;
    if ((M2C_FIELD(arg0, s32 *, 0x270) > 0) || (M2C_FIELD(arg0, s32 *, 0x1084) != 0)) {
        var_v1 = 1;
    }
    return var_v1;
}
