#include "m2c_prelude.h"
M2C_UNK InitBhv();                            /* extern */

void func_00144E20(void *arg0) {
    InitBhv();
    M2C_FIELD(arg0, f32 *, 0x34) = 40.0f;
    M2C_FIELD(arg0, s32 *, 0x38) = 1;
    M2C_FIELD(arg0, f32 *, 0x3C) = 1.0f;
}
