#include "m2c_prelude.h"
void *PbhvFindBhvstk(s32, M2C_UNK);                  /* extern */

void func_001431D0(void *arg0) {
    s32 temp_a0;
    void *temp_v0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x10DC);
    if (temp_a0 != 0) {
        temp_v0 = PbhvFindBhvstk(temp_a0 + 0x26C, 0xB1);
        if ((temp_v0 != NULL) && (M2C_FIELD(temp_v0, s32 *, 0x5C) == arg0)) {
            M2C_FIELD(temp_v0, s32 *, 0x60) = 0;
        }
    }
}
