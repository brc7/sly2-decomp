#include "m2c_prelude.h"
/** @sly2-annot addr=0x11D5B0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
void PrescaleClq__FP3CLQffT0(void *arg0, void *arg1, f32 fparg0, f32 fparg1) {
    M2C_FIELD(arg1, f32 *, 8) = (f32) (M2C_FIELD(arg0, f32 *, 8) * fparg0 * fparg0);
    M2C_FIELD(arg1, f32 *, 4) = (f32) ((2.0f * M2C_FIELD(arg0, f32 *, 8) * fparg0 * fparg1) + (M2C_FIELD(arg0, f32 *, 4) * fparg0));
    M2C_FIELD(arg1, f32 *, 0) = (f32) ((M2C_FIELD(arg0, f32 *, 8) * fparg1 * fparg1) + (M2C_FIELD(arg0, f32 *, 4) * fparg1) + M2C_FIELD(arg0, f32 *, 0));
}
