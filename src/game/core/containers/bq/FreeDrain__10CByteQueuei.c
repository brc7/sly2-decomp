#include "m2c_prelude.h"
/** @sly2-annot addr=0x13D818
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bq-0x13d558-0x13da18
 * @end-sly2-annot */
void FreeDrain__10CByteQueuei(void *arg0, s32 arg1) {
    s32 var_a1;

    var_a1 = arg1;
    if ((var_a1 != 0) || (var_a1 = M2C_FIELD(arg0, s32 *, 0x18), (var_a1 != 0))) {
        M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) + var_a1);
        M2C_FIELD(arg0, s32 *, 0x18) = (s32) (M2C_FIELD(arg0, s32 *, 0x18) - var_a1);
    }
}
