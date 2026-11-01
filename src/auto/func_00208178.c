#include "m2c_prelude.h"
M2C_UNK ReleaseAnimBinding(s32, s32);                    /* extern */

void func_00208178(s32 arg0, s32 arg1) {
    s32 var_s0;
    s32 var_s1;

    var_s0 = arg1;
    var_s1 = 0x19;
    do {
        ReleaseAnimBinding(var_s0, arg0);
        var_s1 -= 1;
        var_s0 += 0x10;
    } while (var_s1 >= 0);
}
