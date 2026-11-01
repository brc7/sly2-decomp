#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(s32, M2C_UNK);                    /* extern */
M2C_UNK func_00184130();                            /* extern */
M2C_UNK func_001B7000(s32, s32);                    /* extern */

void func_00184330(s32 arg0, s32 arg1) {
    func_00184130();
    if (FIsBasicDerivedFrom__FP5BASIC3CID(arg1, 0x36) != 0) {
        func_001B7000(arg1, arg0);
    }
}
