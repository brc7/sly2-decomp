#include "m2c_prelude.h"
s32 NWorldCur();                                /* extern */

void *RefOpCurrentWorld(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = NWorldCur();
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return arg0;
}
