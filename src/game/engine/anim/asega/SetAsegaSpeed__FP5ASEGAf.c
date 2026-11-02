#include "m2c_prelude.h"
/** @sly2-annot addr=0x131A00
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void SetAsegaSpeed__FP5ASEGAf(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x44) = fparg0;
    M2C_FIELD(arg0, f32 *, 0x40) = (f32) (fparg0 * M2C_FIELD(arg0, f32 *, 0x48) * M2C_FIELD(arg0, f32 *, 0x4C));
}
