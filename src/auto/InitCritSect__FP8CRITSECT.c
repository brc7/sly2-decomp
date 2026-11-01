#include "m2c_prelude.h"
s32 SemaCreate__Fii(M2C_UNK, M2C_UNK);              /* extern */

/** @sly2-annot addr=0x21F028
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#thread-0x21efd0-0x21f2e0
 * @end-sly2-annot */
void InitCritSect__FP8CRITSECT(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = -1;
    M2C_FIELD(arg0, s32 *, 8) = SemaCreate__Fii(1, 1);
}
