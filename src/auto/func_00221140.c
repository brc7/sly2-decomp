#include "m2c_prelude.h"
s32 PloCloneLo__FP2LOP2SO(s32, s32);                        /* extern */
M2C_UNK CloneXfm();                            /* extern */

void func_00221140(void *arg0) {
    CloneXfm();
    M2C_FIELD(arg0, s32 *, 0x10C) = PloCloneLo__FP2LOP2SO(M2C_FIELD(arg0, s32 *, 0x10C), M2C_FIELD(arg0, s32 *, 0x10));
}
