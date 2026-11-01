#include "m2c_prelude.h"
M2C_UNK func_001306A8(void *);                      /* extern */
s32 FAsegaRetracted(s32);                             /* extern */
M2C_UNK func_00147410(void *, M2C_UNK);             /* extern */
s32 func_001FE848(M2C_UNK *, s32);                  /* extern */
extern M2C_UNK D_003DD040;

void func_00147278(void *arg0) {
    s32 temp_a0;
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x150);
    switch (temp_v1) {                              /* irregular */
    case 0:
        func_00147410(arg0, 1);
        return;
    case 3:
        if (func_001FE848(&D_003DD040, M2C_FIELD(arg0, s32 *, 0x170)) == 0) {
            func_00147410(arg0, 2);
            return;
        }
        return;
    case 2:
        temp_a0 = M2C_FIELD(arg0, s32 *, 0x160);
        if ((temp_a0 == 0) || (FAsegaRetracted(temp_a0) != 0)) {
            func_001306A8(arg0 + 0x160);
            func_00147410(arg0, -1);
        }
        break;
    }
}
