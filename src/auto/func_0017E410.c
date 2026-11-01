#include "m2c_prelude.h"
M2C_UNK func_0017E6B0(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK ClearBhvBusy(void *);                      /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

void func_0017E410(void *arg0) {
    CloseBhv();
    func_0017E6B0(arg0, -1);
    ClearBhvBusy(arg0);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x2C);
}
