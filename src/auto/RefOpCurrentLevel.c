#include "m2c_prelude.h"
s32 IlevelCur__Fv();                                /* extern */

void *RefOpCurrentLevel(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = IlevelCur__Fv();
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return arg0;
}
