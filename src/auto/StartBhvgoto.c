#include "m2c_prelude.h"
M2C_UNK SetBhvgotoState(void *, M2C_UNK);             /* extern */
M2C_UNK PlanBhvgoto(void *);                      /* extern */
M2C_UNK StartBhv(void *);                      /* extern */
M2C_UNK AddStepguardSgf(s32, s32);                    /* extern */

void StartBhvgoto(void *arg0) {
    s32 temp_a1;

    temp_a1 = M2C_FIELD(arg0, s32 *, 0x38);
    if (temp_a1 != -1) {
        AddStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), temp_a1);
    }
    StartBhv(arg0);
    SetBhvgotoState(arg0, 4);
    PlanBhvgoto(arg0);
}
