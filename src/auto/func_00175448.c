#include "m2c_prelude.h"
M2C_UNK func_00175488(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

void func_00175448(void *arg0) {
    CloseBhv();
    func_00175488(arg0, -1);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x18);
}
