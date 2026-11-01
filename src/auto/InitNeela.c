#include "m2c_prelude.h"
M2C_UNK func_00205EB8();                            /* extern */

void InitNeela(void *arg0) {
    void *temp_v0;

    func_00205EB8();
    temp_v0 = arg0 + 0x1070;
    M2C_FIELD(arg0, f32 *, 0x1070) = 1.35f;
    M2C_FIELD(temp_v0, f32 *, 4) = -0.0003f;
    M2C_FIELD(temp_v0, s32 *, 8) = 0;
    M2C_FIELD(arg0, f32 *, 0x1080) = 0.9f;
    M2C_FIELD(arg0, f32 *, 0x1084) = 1.2f;
    M2C_FIELD(arg0, f32 *, 0x1088) = 7.0f;
    M2C_FIELD(arg0, s32 *, 0x364) = 0;
    M2C_FIELD(arg0, s32 *, 0xE64) = 0;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xF64), s32 *, 0xF8) = 3;
    M2C_FIELD(arg0, s32 *, 0x4A4) = 1;
}
