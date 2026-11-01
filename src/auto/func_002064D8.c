#include "m2c_prelude.h"
M2C_UNK func_00130658(s32);                         /* extern */
M2C_UNK ClearBhvstk__FP6BHVSTK(s32);                         /* extern */
M2C_UNK RemoveSoHierarchy();                            /* extern */
M2C_UNK func_00208540(s32, M2C_UNK);                /* extern */

void func_002064D8(s32 arg0) {
    RemoveSoHierarchy();
    func_00208540(arg0, 0);
    ClearBhvstk__FP6BHVSTK(arg0 + 0x26C);
    func_00130658(arg0 + 0x34C);
}
