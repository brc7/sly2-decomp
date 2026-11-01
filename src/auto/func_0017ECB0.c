#include "m2c_prelude.h"
s32 func_0017DCB8(s32);                             /* extern */
M2C_UNK func_0017ECF0(void *, M2C_UNK);             /* extern */
M2C_UNK UpdateBhv();                            /* extern */

void func_0017ECB0(void *arg0) {
    UpdateBhv();
    if (func_0017DCB8(M2C_FIELD(arg0, s32 *, 0x18)) != 0) {
        func_0017ECF0(arg0, 0);
    }
}
