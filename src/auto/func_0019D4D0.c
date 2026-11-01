#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(s32, M2C_UNK);                    /* extern */
M2C_UNK func_00201D78(s32, s32 *);                  /* extern */

void func_0019D4D0(void *arg0, s32 *arg1) {
    s32 temp_a0;

    M2C_FIELD(arg0, s32 **, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = -1;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(*arg1, 0x35) != 0) {
        temp_a0 = *M2C_FIELD(arg0, s32 **, 0) + 0x1D40;
        M2C_FIELD(arg0, s32 *, 0x14) = temp_a0;
        func_00201D78(temp_a0, arg1);
        return;
    }
    M2C_FIELD(arg0, s32 *, 0x14) = 0;
}
