#include "m2c_prelude.h"
M2C_UNK SetBhvattackState(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

void CloseBhvattack(void *arg0) {
    CloseBhv();
    SetBhvattackState(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x18);
}
