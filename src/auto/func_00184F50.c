#include "m2c_prelude.h"
M2C_UNK func_00182C70(s32);                         /* extern */
M2C_UNK InitTarget();                            /* extern */

void func_00184F50(void *arg0) {
    InitTarget();
    func_00182C70(arg0 + 0x100);
    M2C_FIELD(arg0, s32 *, 0xE8) = 1;
    M2C_FIELD(arg0, s32 *, 0x12C) = 8;
    M2C_FIELD(arg0, s32 *, 0x130) = 9;
}
