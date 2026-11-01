#include "m2c_prelude.h"
M2C_UNK StartJoySelection__FP3JOY(s32);                         /* extern */
M2C_UNK func_001D98A8();                            /* extern */

void func_001AE9A8(void *arg0, s32 arg1) {
    func_001D98A8();
    if (arg1 != 0) {
        StartJoySelection__FP3JOY(M2C_FIELD(arg0, s32 *, 0x10));
    }
}
