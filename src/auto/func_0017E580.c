#include "m2c_prelude.h"
M2C_UNK func_0017E6B0(void *, M2C_UNK);             /* extern */
M2C_UNK ClearBhvBusy(void *);                      /* extern */

void func_0017E580(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x34) + 1;
    M2C_FIELD(arg0, s32 *, 0x34) = temp_v0;
    if (temp_v0 < M2C_FIELD(M2C_FIELD(arg0, void **, 0x30), s32 *, 0x50)) {
        func_0017E6B0(arg0, -1);
        func_0017E6B0(arg0, 0);
        return;
    }
    func_0017E6B0(arg0, -1);
    ClearBhvBusy(arg0);
}
