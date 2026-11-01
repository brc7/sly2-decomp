#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitXfm();                            /* extern */

void func_00183340(s32 arg0) {
    InitXfm();
    InitGrabspot(arg0 + 0xE0);
}
