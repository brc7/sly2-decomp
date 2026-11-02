#include "m2c_prelude.h"
s32 NewSfx();                                /* extern */

/** @sly2-annot addr=0x15C650
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Info: TU emitter
 * Notes: names_v2 v1 hint not used: CloneExplo__FP5EXPLOT0
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * @end-sly2-annot */
s32 func_0015C650(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0xF0) == 0) {
        M2C_FIELD(arg0, s32 *, 0xF0) = NewSfx();
    }
    return M2C_FIELD(arg0, s32 *, 0xF0);
}
