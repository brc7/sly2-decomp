#include "m2c_prelude.h"
s32 func_001F8278(s32);                             /* extern */
M2C_UNK AddTargetRecordGuard(s32, void *);                 /* extern */
M2C_UNK func_0020A790(s32, void *);                 /* extern */

void SetStepguardTarget(void *arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x38C);
    if (arg1 != temp_v0) {
        if (temp_v0 != 0) {
            func_0020A790(M2C_FIELD(arg0, s32 *, 0x3CC), arg0);
            M2C_FIELD(arg0, s32 *, 0x3CC) = 0;
        }
        M2C_FIELD(arg0, s32 *, 0x38C) = arg1;
        if (arg1 != 0) {
            temp_v0_2 = func_001F8278(arg1);
            M2C_FIELD(arg0, s32 *, 0x3CC) = temp_v0_2;
            AddTargetRecordGuard(temp_v0_2, arg0);
        }
    }
}
