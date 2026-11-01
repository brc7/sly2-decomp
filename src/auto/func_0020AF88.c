#include "m2c_prelude.h"
M2C_UNK ReleaseAnimBinding(s32, s32);                    /* extern */
M2C_UNK ResolveAnimBinding(s32, s32);                    /* extern */

void func_0020AF88(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_s1;

    temp_s1 = arg0 + 0xB0;
    ReleaseAnimBinding(temp_s1, M2C_FIELD(arg0, s32 *, 0xAC));
    M2C_FIELD(arg0, s32 *, 0xB4) = arg1;
    M2C_FIELD(arg0, s32 *, 0xBC) = arg2;
    ResolveAnimBinding(temp_s1, M2C_FIELD(arg0, s32 *, 0xAC));
}
