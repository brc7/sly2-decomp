#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitShape();                            /* extern */

void func_001CB908(void *arg0) {
    InitShape();
    InitGrabspot(arg0 + 0x190);
    M2C_FIELD(arg0, f32 *, 0x1C4) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x1C0) = 1000.0f;
}
