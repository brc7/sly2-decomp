#include "m2c_prelude.h"
M2C_UNK SetLoBusyk(void *, M2C_UNK);             /* extern */
M2C_UNK InitXfm();                            /* extern */

void InitTarget(void *arg0) {
    InitXfm();
    SetLoBusyk(arg0, 3);
    M2C_FIELD(arg0, s32 *, 0xE8) = 0x17E;
    M2C_FIELD(arg0, f32 *, 0xEC) = 25.0f;
    M2C_FIELD(arg0, s32 *, 0xF4) = -1;
}
