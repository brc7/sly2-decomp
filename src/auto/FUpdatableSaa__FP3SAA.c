#include "m2c_prelude.h"
extern f32 D_002DDED0;

/** @sly2-annot addr=0x1EDA20
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
s32 FUpdatableSaa__FP3SAA(void *arg0) {
    if (M2C_FIELD(arg0, f32 *, 0x10) != D_002DDED0) {
        M2C_FIELD(arg0, f32 *, 0x10) = (f32) D_002DDED0;
        return 1;
    }
    return 0;
}
