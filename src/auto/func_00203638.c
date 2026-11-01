#include "m2c_prelude.h"
M2C_UNK SubscribeLoObject(s32, void *);                 /* extern */
M2C_UNK PostLoLoad();                            /* extern */

void func_00203638(void *arg0) {
    s32 temp_a0;

    PostLoLoad();
    temp_a0 = M2C_FIELD(arg0, s32 *, 0xEC);
    if (temp_a0 != 0) {
        SubscribeLoObject(temp_a0, arg0);
    }
}
