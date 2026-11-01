#include "m2c_prelude.h"
s32 func_00174FB8(s32);                             /* extern */
M2C_UNK func_0017B750(void *, M2C_UNK);             /* extern */
M2C_UNK UpdateBhv();                            /* extern */
s32 FBhvBusyAbove(void *);                          /* extern */

void func_0017B620(void *arg0) {
    s32 temp_s1;

    UpdateBhv();
loop_1:
    if (FBhvBusyAbove(arg0) == 0) {
        temp_s1 = M2C_FIELD(arg0, s32 *, 0x30);
        if ((temp_s1 == 0) && (func_00174FB8(M2C_FIELD(arg0, s32 *, 0x18)) != 0)) {
            func_0017B750(arg0, -1);
        }
        if (temp_s1 != M2C_FIELD(arg0, s32 *, 0x30)) {
            goto loop_1;
        }
    }
}
