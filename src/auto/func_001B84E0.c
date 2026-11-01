#include "m2c_prelude.h"
/** @sly2-annot addr=0x1B84E0
 * Purpose: True if no attack overlay running and no attack substate, else the combo-window flag x1820 (anim
 *   event 0x242)
 * Info: proposed name FMurrayCanAttack (T1); TU murray
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
s32 func_001B84E0(void *arg0) {
    if ((M2C_FIELD(arg0, s32 *, 0x190C) != 0) || (M2C_FIELD(arg0, s32 *, 0x1810) != -1)) {
        return M2C_FIELD(arg0, s32 *, 0x1820);
    }
    return 1;
}
