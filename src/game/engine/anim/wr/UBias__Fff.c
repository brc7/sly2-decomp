#include "m2c_prelude.h"
/** @sly2-annot addr=0x22BF40
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
f32 UBias__Fff(f32 fparg0, f32 fparg1) {
    return fparg0 / ((((1.0f / fparg1) - 2.0f) * (1.0f - fparg0)) + 1.0f);
}
