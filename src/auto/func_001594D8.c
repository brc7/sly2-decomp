#include "m2c_prelude.h"
M2C_UNK func_001C4848(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1594D8
 * Purpose: Forwards to 0x1C4848(&g_sw, p) (SW-level registration of the emitter)
 * Info: TU emitter
 * Notes: no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r3_1539A8+verdicts_A1
 * See: docs/engine/particles-effects.md#1-emitters-emittercpp-0x159118-0x15cd68
 * @end-sly2-annot */
void func_001594D8(s32 arg0) {
    func_001C4848(&D_003DD490, arg0);
}
