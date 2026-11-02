#include "m2c_prelude.h"
/** @sly2-annot addr=0x20C0D8
 * Purpose: state == 4
 * Info: proposed name FLeapDone (T1); TU tu_20B840
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
s32 FLeapDone(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x10) == 4;
}
