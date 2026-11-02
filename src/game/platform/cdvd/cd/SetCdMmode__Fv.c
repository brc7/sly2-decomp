#include "m2c_prelude.h"
M2C_UNK sceCdMmode(M2C_UNK);                        /* extern */
extern s32 D_002BD4C4;

/** @sly2-annot addr=0x14BA00
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#cd-0x14b7f8-0x14bea8
 * @end-sly2-annot */
void SetCdMmode__Fv(void) {
    if (D_002BD4C4 == 0x14) {
        sceCdMmode(2);
    }
}
