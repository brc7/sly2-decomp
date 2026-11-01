#include "m2c_prelude.h"
M2C_UNK func_0017E278(void *, M2C_UNK);             /* extern */
M2C_UNK StartBhv();                            /* extern */
M2C_UNK SetStepguardTarget(s32, s32);                    /* extern */

void func_0017DDA8(void *arg0) {
    StartBhv();
    func_0017E278(arg0, 0);
    SetStepguardTarget(M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(arg0, s32 *, 0x38));
}
