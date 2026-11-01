#include "m2c_prelude.h"
f32 F32Read__18CBinaryInputStream(s32);             /* extern */

void LoadAcFloatConstFromBrx(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, f32 *, 4) = F32Read__18CBinaryInputStream(arg1);
}
