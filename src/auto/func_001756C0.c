#include "m2c_prelude.h"
s32 func_00174898(s32);                             /* extern */
s32 BHV_FHandleMessage(void *, s32, void *);             /* extern */

s32 func_001756C0(void *arg0, s32 arg1, void *arg2) {
    s32 temp_a0;

    if (arg1 == 0xB) {
        temp_a0 = M2C_FIELD(arg2, s32 *, 4);
        if (temp_a0 == M2C_FIELD(arg0, s32 *, 0x30)) {
            if (func_00174898(temp_a0) == M2C_FIELD(arg0, s32 *, 0x10)) {
                M2C_FIELD(arg0, s32 *, 0x40) = 1;
                return 1;
            }
            goto block_5;
        }
    }
block_5:
    return BHV_FHandleMessage(arg0, arg1, arg2);
}
