#include "m2c_prelude.h"
/** @sly2-annot addr=0x172550
 * Purpose: True if state == -1
 * Info: proposed name FBhv171Idle (T2); TU tu_171F58
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
s32 func_00172550(void *arg0) {
    return ~M2C_FIELD(arg0, s32 *, 0x30) == 0;
}
