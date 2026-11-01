#include "m2c_prelude.h"
M2C_UNK func_001749C0(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK func_00209970(s32, s32);                    /* extern */

void func_00174B28(void *arg0) {
    CloseBhv();
    func_001749C0(arg0, 1);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(arg0, s32 *, 0x3C));
}
