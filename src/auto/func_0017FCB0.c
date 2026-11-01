#include "m2c_prelude.h"
M2C_UNK func_0017F6B0();                            /* extern */
M2C_UNK func_0017FD08(s32, M2C_UNK);                /* extern */
M2C_UNK func_00180100(s32);                         /* extern */
s32 FIsPoActive(s32);                             /* extern */

void func_0017FCB0(s32 arg0) {
    func_0017F6B0();
    if (FIsPoActive(arg0) != 0) {
        func_0017FD08(arg0, 1);
    } else {
        func_0017FD08(arg0, 0);
    }
    func_00180100(arg0);
}
