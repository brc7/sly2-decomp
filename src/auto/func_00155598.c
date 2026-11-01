#include "m2c_prelude.h"
M2C_UNK func_00155788(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK SetStepguardTarget(s32, M2C_UNK);                /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

void func_00155598(void *arg0) {
    CloseBhv();
    func_00155788(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x22);
    SetStepguardTarget(M2C_FIELD(arg0, s32 *, 0x10), 0);
}
