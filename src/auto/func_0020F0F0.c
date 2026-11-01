#include "m2c_prelude.h"
M2C_UNK RemoveGrfusr__Fi(M2C_UNK);                     /* extern */

void func_0020F0F0(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x3D7C) - 1;
    M2C_FIELD(arg0, s32 *, 0x3D7C) = temp_v0;
    if (temp_v0 == 0) {
        RemoveGrfusr__Fi(4);
    }
}
