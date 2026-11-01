#include "m2c_prelude.h"
/** @sly2-annot addr=0x22C4A0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
void *PwreGetWrCur__FP2WR4ENSK4WREK(void *arg0, s32 arg1) {
    if (M2C_FIELD(arg0, s32 *, 0x54) == 0) {
        return NULL;
    }
    if (arg1 == 1) {
        M2C_FIELD(arg0, s32 *, 0x8FC) = arg1;
    }
    return arg0 + ((M2C_FIELD(arg0, s32 *, 0x6E0) * 0xD0) + 0x60);
}
