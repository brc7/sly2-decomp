#include "m2c_prelude.h"
/** @sly2-annot addr=0x15A160
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void UnpauseEmitter__FP7EMITTER(void *arg0) {
    M2C_FIELD(arg0, f32 *, 0x148) = -1.0f;
}
