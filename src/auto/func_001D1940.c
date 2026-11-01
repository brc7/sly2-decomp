#include "m2c_prelude.h"
M2C_UNK OnPoActive();                            /* extern */
M2C_UNK func_001D4940(void *, M2C_UNK);             /* extern */

void func_001D1940(void *arg0, s32 arg1) {
    OnPoActive();
    if (arg1 != 0) {
        func_001D4940(arg0, 2);
    } else if (M2C_FIELD(arg0, s32 *, 0x180) == 2) {
        func_001D4940(arg0, 0);
    }
}
