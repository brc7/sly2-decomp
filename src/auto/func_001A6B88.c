#include "m2c_prelude.h"
M2C_UNK SetLoXfFromMat(s32, s32);                    /* extern */

void func_001A6B88(s32 arg0, void *arg1) {
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg1, void **, 0xC);
    SetLoXfFromMat(arg0, M2C_FIELD(temp_v1, s32 (**)(void *), 0xFC)(arg1 + M2C_FIELD(temp_v1, s16 *, 0xF8)));
}
