#include "m2c_prelude.h"
extern f32 D_002DDED0;

void SetBhvpathState(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x4C)) {
        M2C_FIELD(arg0, s32 *, 0x4C) = arg1;
        M2C_FIELD(arg0, f32 *, 0x50) = (f32) D_002DDED0;
    }
}
