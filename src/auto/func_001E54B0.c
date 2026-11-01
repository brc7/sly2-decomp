#include "m2c_prelude.h"
M2C_UNK EndJtMode(s32);                         /* extern */

void func_001E54B0(void *arg0) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0xA0);
    if (temp_a0 != 0) {
        EndJtMode(temp_a0 + 0xF00);
        M2C_FIELD(arg0, s32 *, 0xA0) = 0;
    }
}
