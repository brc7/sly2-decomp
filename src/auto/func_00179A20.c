#include "m2c_prelude.h"
M2C_UNK RecalcSwOxfFilterForObject(M2C_UNK *, s32);              /* extern */
M2C_UNK func_00179598();                            /* extern */
extern M2C_UNK D_003DD490;

void func_00179A20(void *arg0) {
    func_00179598();
    RecalcSwOxfFilterForObject(&D_003DD490, M2C_FIELD(arg0, s32 *, 0x10));
}
