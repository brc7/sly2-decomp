#include "m2c_prelude.h"
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

void func_0017D330(void *arg0) {
    CloseBhv();
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x29);
}
