#include "m2c_prelude.h"
M2C_UNK func_0020C030(s32);                         /* extern */
M2C_UNK func_0020CA20(s32);                         /* extern */

void ReleaseAnimBinding(void *arg0) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0);
    if (temp_v1 != 2) {
        if (temp_v1 == 3) {
            func_0020CA20(M2C_FIELD(arg0, s32 *, 4));
            goto block_4;
        }
    } else {
        func_0020C030(M2C_FIELD(arg0, s32 *, 4));
block_4:
        M2C_FIELD(arg0, s32 *, 4) = 0;
    }
    M2C_FIELD(arg0, s32 *, 4) = -1;
    M2C_FIELD(arg0, s32 *, 0) = -1;
}
