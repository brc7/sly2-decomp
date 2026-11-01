#include "m2c_prelude.h"
/** @sly2-annot addr=0x11D910
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
s32 FCheckLm__FP2LMf(void *arg0, f32 fparg0) {
    s32 var_v0;

    var_v0 = 0;
    if ((M2C_FIELD(arg0, f32 *, 0) < fparg0) && (fparg0 < M2C_FIELD(arg0, f32 *, 4))) {
        var_v0 = 1;
    }
    return var_v0;
}
