#include "m2c_prelude.h"
M2C_UNK SetMurrayState(void *, M2C_UNK);             /* extern */
M2C_UNK func_001B8D30(void *, M2C_UNK);             /* extern */
M2C_UNK StartBhv();                            /* extern */

void func_00146E98(void *arg0) {
    StartBhv();
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0x1AC0) = 1;
    SetMurrayState(M2C_FIELD(arg0, void **, 0x10), 0xC);
    func_001B8D30(M2C_FIELD(arg0, void **, 0x10), 0);
}
