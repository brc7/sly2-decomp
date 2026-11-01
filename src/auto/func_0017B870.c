#include "m2c_prelude.h"
M2C_UNK func_0017B8B0(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

void func_0017B870(void *arg0) {
    CloseBhv();
    func_0017B8B0(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x20);
}
