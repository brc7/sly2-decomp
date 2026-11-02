#include "m2c_prelude.h"
M2C_UNK func_001ED8E0();                            /* extern */

/** @sly2-annot addr=0x1EE688
 * Purpose: CIRCLER vt7 Init
 * Info: proposed name InitCircler (T1); class CIRCLER slot7; TU shdanim
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void InitCircler(void *arg0, void *arg1) {
    func_001ED8E0();
    M2C_FIELD(arg0, f32 *, 0x50) = (f32) M2C_FIELD(arg1, f32 *, 0xC);
    M2C_FIELD(arg0, f32 *, 0x54) = (f32) M2C_FIELD(arg1, f32 *, 0x10);
    M2C_FIELD(arg0, f32 *, 0x58) = (f32) M2C_FIELD(arg1, f32 *, 0x14);
    M2C_FIELD(arg0, f32 *, 0x5C) = (f32) M2C_FIELD(arg1, f32 *, 0x18);
}
