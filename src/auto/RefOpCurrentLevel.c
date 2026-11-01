#include "m2c_prelude.h"
s32 func_00210A88();                                /* extern */

void *RefOpCurrentLevel(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = func_00210A88();
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return arg0;
}
