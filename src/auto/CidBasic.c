#include "m2c_prelude.h"
s32 CidBasic(void *arg0) {
    s16 temp_a1;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xC);
    temp_a1 = M2C_FIELD(temp_v1, s16 *, 0x30);
    return M2C_FIELD(M2C_FIELD(temp_v1, void *(**)(void *, s16), 0x34)(arg0 + temp_a1, temp_a1), s32 *, 4);
}
