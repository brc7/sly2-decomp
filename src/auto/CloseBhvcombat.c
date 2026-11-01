#include "m2c_prelude.h"
M2C_UNK SetBhvcombatState(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

void CloseBhvcombat(void *arg0) {
    CloseBhv();
    SetBhvcombatState(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x1D);
}
