#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitXfm();                            /* extern */

void func_00183F08(void *arg0) {
    InitXfm();
    InitGrabspot(arg0 + 0xE0);
    M2C_FIELD(arg0, s32 *, 0xF0) = 1;
    M2C_FIELD(arg0, f32 *, 0x110) = 200.0f;
    M2C_FIELD(arg0, s32 *, 0xFC) = 0;
}
