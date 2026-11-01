#include "m2c_prelude.h"
/** @sly2-annot addr=0x1449C8
 * Purpose: returns fHold (+0x28)
 * Info: proposed name FJtgoalHold (T1); TU jtgoal
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#jtgoal-0x144638-0x144b28
 * @end-sly2-annot */
s32 func_001449C8(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x28);
}
