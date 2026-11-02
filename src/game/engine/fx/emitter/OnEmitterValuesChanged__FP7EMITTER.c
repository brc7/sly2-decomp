#include "m2c_prelude.h"
M2C_UNK SetRipgEmitb__FP4RIPGP5EMITB(s32, s32);     /* extern */
f32 GRandInRange__Fff(f32, f32);                        /* extern */
M2C_UNK SetBlipgEmitb(s32, s32);                    /* extern */

/** @sly2-annot addr=0x15A1C0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void OnEmitterValuesChanged__FP7EMITTER(void *arg0) {
    f32 temp_f12;
    f32 var_f0;
    s32 temp_a0;
    s32 temp_a0_2;

    M2C_FIELD(arg0, s32 *, 0xE0) = 0;
    temp_a0 = M2C_FIELD(arg0, s32 *, 0x150);
    if (temp_a0 != 0) {
        SetBlipgEmitb(temp_a0, M2C_FIELD(arg0, s32 *, 0xE4));
    }
    temp_a0_2 = M2C_FIELD(arg0, s32 *, 0x14C);
    if (temp_a0_2 != 0) {
        SetRipgEmitb__FP4RIPGP5EMITB(temp_a0_2, M2C_FIELD(arg0, s32 *, 0xE4));
    }
    temp_f12 = M2C_FIELD(arg0, f32 *, 0xF8);
    if (M2C_FIELD(arg0, s32 *, 0xF0) == 3) {
        var_f0 = GRandInRange__Fff(temp_f12, M2C_FIELD(arg0, f32 *, 0xFC)) * 60.0f;
    } else {
        var_f0 = GRandInRange__Fff(temp_f12, M2C_FIELD(arg0, f32 *, 0xFC));
    }
    M2C_FIELD(arg0, f32 *, 0x130) = var_f0;
}
