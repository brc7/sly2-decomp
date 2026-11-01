#include "m2c_prelude.h"
s32 PentryChooseStepguardAttack(void *, s32, s32);                /* extern */
extern f32 D_002BE610;
extern f32 D_002DDED0;

s32 FBhvcombatChooseAttack(void *arg0) {
    s32 temp_v0;
    void *temp_a0;
    void *temp_a0_2;

    temp_a0 = M2C_FIELD(arg0, void **, 0x10);
    temp_v0 = PentryChooseStepguardAttack(temp_a0, M2C_FIELD(arg0, s32 *, 0x30), M2C_FIELD(temp_a0, s32 *, 0x3CC));
    M2C_FIELD(arg0, s32 *, 0x3C) = temp_v0;
    if (temp_v0 != 0) {
        return 1;
    }
    temp_a0_2 = M2C_FIELD(arg0, void **, 0x10);
    if (M2C_FIELD(temp_a0_2, f32 *, 0xF04) < D_002DDED0) {
        M2C_FIELD(temp_a0_2, f32 *, 0xF04) = (f32) (D_002DDED0 + D_002BE610);
    }
    return 0;
}
