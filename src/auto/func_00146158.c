#include "m2c_prelude.h"
M2C_UNK SetBhvBusy(void *);                      /* extern */
M2C_UNK ClearBhvBusy();                            /* extern */
extern f32 D_002DDED0;

void func_00146158(void *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x30);
    if (arg1 != temp_v0) {
        if (temp_v0 == 2) {
            ClearBhvBusy();
        }
        M2C_FIELD(arg0, s32 *, 0x30) = arg1;
        M2C_FIELD(arg0, f32 *, 0x34) = (f32) D_002DDED0;
        if (arg1 == 2) {
            SetBhvBusy(arg0);
        }
    }
}
