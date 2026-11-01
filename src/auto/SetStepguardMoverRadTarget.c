#include "m2c_prelude.h"
M2C_UNK func_00204660(s32);                         /* extern */

void SetStepguardMoverRadTarget(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 4) == 0) {
        func_00204660(M2C_FIELD(arg0, s32 *, 0xC));
    }
}
