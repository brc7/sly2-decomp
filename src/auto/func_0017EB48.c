#include "m2c_prelude.h"
M2C_UNK InitBhv();                            /* extern */
M2C_UNK func_00209890(s32, M2C_UNK);                /* extern */

void func_0017EB48(void *arg0) {
    InitBhv();
    M2C_FIELD(arg0, f32 *, 0x38) = -1.0f;
    func_00209890(M2C_FIELD(arg0, s32 *, 0x10), 0x2E);
}
