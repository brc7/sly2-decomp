#include "m2c_prelude.h"
M2C_UNK CloneXfm();                            /* extern */

void func_002590B8(void *arg0) {
    s32 *temp_v1;

    CloneXfm();
    temp_v1 = M2C_FIELD(arg0, s32 **, 0xE4);
    *temp_v1 += 1;
}
