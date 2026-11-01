#include "m2c_prelude.h"
/** @sly2-annot addr=0x22C5B8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
void AddOnzOnze__FP3ONZffff(void *arg0, f32 fparg0, f32 fparg1, f32 fparg2, f32 fparg3) {
    u32 temp_v1;
    void *temp_v0;

    temp_v1 = M2C_FIELD(arg0, u32 *, 0xC);
    if (temp_v1 < 4U) {
        M2C_FIELD(arg0, u32 *, 0xC) = (u32) (temp_v1 + 1);
        temp_v0 = arg0 + ((temp_v1 * 0x10) + 0x10);
        M2C_FIELD(temp_v0, f32 *, 0) = fparg0;
        M2C_FIELD(temp_v0, f32 *, 4) = fparg1;
        M2C_FIELD(temp_v0, f32 *, 8) = fparg2;
        M2C_FIELD(temp_v0, f32 *, 0xC) = fparg3;
    }
}
