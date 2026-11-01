#include "m2c_prelude.h"
M2C_UNK CloseBhv();                            /* extern */

void func_00146260(void *arg0) {
    void *temp_a0;

    CloseBhv();
    temp_a0 = M2C_FIELD(arg0, void **, 0x10);
    M2C_FIELD(temp_a0, s32 *, 0x10E0) = (s32) (M2C_FIELD(temp_a0, s32 *, 0x10E0) & ~1);
}
