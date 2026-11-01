#include "m2c_prelude.h"
M2C_UNK func_00130658(s32);                         /* extern */
M2C_UNK ClearBhvstk__FP6BHVSTK(s32);                         /* extern */
M2C_UNK RemoveSoHierarchy();                            /* extern */

void func_00141928(s32 arg0) {
    RemoveSoHierarchy();
    func_00130658(arg0 + 0xEFC);
    func_00130658(arg0 + 0xF14);
    ClearBhvstk__FP6BHVSTK(arg0 + 0x26C);
}
