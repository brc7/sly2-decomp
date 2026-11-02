#include "m2c_prelude.h"
extern f32 D_002DDED0;

/** @sly2-annot addr=0x15A138
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void PauseEmitter__FP7EMITTERf(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x148) = (f32) (D_002DDED0 + fparg0);
}
