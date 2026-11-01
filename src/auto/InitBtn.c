#include "m2c_prelude.h"
void InitBtn(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x90) = -1;
    M2C_FIELD(arg0, f32 *, 0x8C) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x98) = 1.0f;
}
