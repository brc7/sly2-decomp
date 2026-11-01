#include "m2c_prelude.h"
void *func_001BBDB0(s32, M2C_UNK);                  /* extern */
s32 func_001BBE18();                                /* extern */
M2C_UNK HideSoGlobsById(s32, s32);                    /* extern */

void func_001BBC98(s32 arg0, s32 arg1) {
    if (func_001BBE18() == 0) {
        HideSoGlobsById(arg0, arg1);
        return;
    }
    M2C_FIELD(func_001BBDB0(arg0, 3), s32 *, 4) = arg1;
}
