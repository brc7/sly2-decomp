#include "m2c_prelude.h"
M2C_UNK SetRipgEmitb__FP4RIPGP5EMITB(s32, s32);     /* extern */
f32 GRandInRange__Fff(f32, f32);                        /* extern */
M2C_UNK func_001394C8(s32, s32);                    /* extern */

void OnEmitterValuesChanged__FP7EMITTER(void *arg0) {
    f32 temp_f12;
    f32 var_f0;
    s32 temp_a0;
    s32 temp_a0_2;

    M2C_FIELD(arg0, s32 *, 0xE0) = 0;
    temp_a0 = M2C_FIELD(arg0, s32 *, 0x150);
    if (temp_a0 != 0) {
        func_001394C8(temp_a0, M2C_FIELD(arg0, s32 *, 0xE4));
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
