#include "m2c_prelude.h"
M2C_UNK SetBhvcombatState(void *, M2C_UNK);             /* extern */
M2C_UNK StartBhv();                            /* extern */
M2C_UNK SetStepguardTarget(s32, s32);                    /* extern */

void StartBhvcombat(void *arg0) {
    StartBhv();
    SetBhvcombatState(arg0, 3);
    SetStepguardTarget(M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(arg0, s32 *, 0x30));
}
