#include "m2c_prelude.h"
M2C_UNK InitBhvanim();                            /* extern */

void InitBhvjuggled(void *arg0) {
    InitBhvanim();
    M2C_FIELD(arg0, s32 *, 0x3C) = 0x17;
    M2C_FIELD(arg0, s32 *, 0x50) = -1;
}
