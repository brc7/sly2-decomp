#include "m2c_prelude.h"
M2C_UNK InitBhv();                            /* extern */
M2C_UNK AddStepguardSgf(s32, M2C_UNK);                /* extern */

void func_0017D2B0(void *arg0) {
    InitBhv();
    AddStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x29);
}
