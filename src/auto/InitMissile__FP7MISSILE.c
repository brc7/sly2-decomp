#include "m2c_prelude.h"
M2C_UNK InitBomb();                            /* extern */

void InitMissile__FP7MISSILE(void *arg0) {
    InitBomb();
    M2C_FIELD(arg0, s32 *, 0x264) = 1;
    M2C_FIELD(arg0, f32 *, 0x260) = 800.0f;
    M2C_FIELD(arg0, f32 *, 0x26C) = 1.0f;
}
