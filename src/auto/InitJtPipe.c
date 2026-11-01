#include "m2c_prelude.h"
void InitJtPipe(void *arg0, s32 arg1) {
    void *temp_a1;
    void *temp_v0;

    temp_v0 = arg0 + 0x30;
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = -1;
    temp_a1 = arg0 + 0x40;
    M2C_FIELD(arg0, f32 *, 0x30) = 45.0f;
    M2C_FIELD(temp_v0, f32 *, 8) = 40.0f;
    M2C_FIELD(temp_v0, s32 *, 4) = 0;
    M2C_FIELD(arg0, f32 *, 0x40) = 45.0f;
    M2C_FIELD(temp_a1, f32 *, 8) = -20.0f;
    M2C_FIELD(temp_a1, s32 *, 4) = 0;
}
