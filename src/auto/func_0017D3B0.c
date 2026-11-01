#include "m2c_prelude.h"
M2C_UNK func_0017D450(void *, M2C_UNK);             /* extern */
M2C_UNK func_0017DA28(void *);                      /* extern */
M2C_UNK StartBhv(void *);                      /* extern */
M2C_UNK func_00209890(s32, s32);                    /* extern */

void func_0017D3B0(void *arg0) {
    s32 temp_a1;

    temp_a1 = M2C_FIELD(arg0, s32 *, 0x38);
    if (temp_a1 != -1) {
        func_00209890(M2C_FIELD(arg0, s32 *, 0x10), temp_a1);
    }
    StartBhv(arg0);
    func_0017D450(arg0, 4);
    func_0017DA28(arg0);
}
