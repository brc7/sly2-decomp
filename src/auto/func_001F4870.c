#include "m2c_prelude.h"
M2C_UNK ConvertXfVec(s32, s32, s128);              /* extern */

void func_001F4870(void *arg0) {
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0x5C);
    ConvertXfVec(M2C_FIELD(temp_v0, s32 *, 0xA0), M2C_FIELD(arg0, s32 *, 0x60), M2C_FIELD(temp_v0, s128 *, 0xB0));
}
