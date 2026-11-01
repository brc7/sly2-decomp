#include "m2c_prelude.h"
s32 func_00207B10(s32);                             /* extern */
M2C_UNK func_00207B70(s32, s32, s32, s32);          /* extern */
M2C_UNK ResolveAnimBinding(s32, s32);                    /* extern */
s32 func_00209C18(s32, s32);                        /* extern */

void func_00207BC8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_s1;

    temp_s1 = func_00209C18(arg0, func_00207B10(arg1));
    func_00207B70(arg0, arg1, arg2, arg3);
    ResolveAnimBinding(temp_s1, arg0);
}
