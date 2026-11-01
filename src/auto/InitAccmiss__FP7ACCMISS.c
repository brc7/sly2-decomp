#include "m2c_prelude.h"
M2C_UNK InitMissile__FP7MISSILE();                            /* extern */

void InitAccmiss__FP7ACCMISS(void *arg0) {
    InitMissile__FP7MISSILE();
    M2C_FIELD(arg0, f32 *, 0x270) = 2000.0f;
    M2C_FIELD(arg0, f32 *, 0x274) = 5000.0f;
    M2C_FIELD(arg0, s32 *, 0x264) = 0;
}
