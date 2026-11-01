#include "m2c_prelude.h"
M2C_UNK SetJtgoal(s32, s128);                   /* extern */
M2C_UNK func_001789F8();                            /* extern */

void UpdateBhvwalkgoal(void *arg0) {
    func_001789F8();
    SetJtgoal(M2C_FIELD(arg0, s32 *, 0x10) + 0xFD0, M2C_FIELD(arg0, s128 *, 0x80));
}
