#include "m2c_prelude.h"
M2C_UNK func_00176458(void *, M2C_UNK);             /* extern */
M2C_UNK StartBhv();                            /* extern */
M2C_UNK func_00208540(s32, s32);                    /* extern */

void func_00175790(void *arg0) {
    StartBhv();
    func_00176458(arg0, 3);
    func_00208540(M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(arg0, s32 *, 0x30));
}
