#include "m2c_prelude.h"
/** @sly2-annot addr=0x15A2B0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void SetEmitterAutoPause__FP7EMITTERi(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x114) = arg1;
    M2C_FIELD(arg0, s32 *, 0xE0) = 1;
}
