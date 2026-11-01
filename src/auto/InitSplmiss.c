#include "m2c_prelude.h"
M2C_UNK InitMissile__FP7MISSILE();                            /* extern */

void InitSplmiss(void *arg0) {
    InitMissile__FP7MISSILE();
    M2C_FIELD(arg0, f32 *, 0x274) = 1500.0f;
}
