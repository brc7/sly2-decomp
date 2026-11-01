#include "m2c_prelude.h"
M2C_UNK SetBhvanimState(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, s32);                    /* extern */

void CloseBhvanim(void *arg0) {
    CloseBhv();
    SetBhvanimState(arg0, 1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(arg0, s32 *, 0x3C));
}
