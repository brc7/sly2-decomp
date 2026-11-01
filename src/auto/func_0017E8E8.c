#include "m2c_prelude.h"
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

void func_0017E8E8(void *arg0) {
    CloseBhv();
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x2D);
}
