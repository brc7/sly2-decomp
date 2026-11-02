#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EDD30
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
f32 UCompleteLoop__FP4LOOP(void *arg0) {
    f32 temp_f2;

    temp_f2 = M2C_FIELD(arg0, f32 *, 0x60);
    return (M2C_FIELD(arg0, f32 *, 0x64) / temp_f2) / (((f32) M2C_FIELD(arg0, s32 *, 0x1C) / temp_f2) + M2C_FIELD(arg0, f32 *, 0x68));
}
