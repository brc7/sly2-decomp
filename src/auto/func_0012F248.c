#include "m2c_prelude.h"
M2C_UNK InitDl__FP2DLi(s32, M2C_UNK);                /* extern */
M2C_UNK func_001A5270();                            /* extern */

void func_0012F248(void *arg0) {
    void *temp_v0;

    func_001A5270();
    InitDl__FP2DLi(arg0 + 0x50, 0x2C);
    temp_v0 = arg0 + 0x80;
    M2C_FIELD(arg0, f32 *, 0x78) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x80) = 1.0f;
    M2C_FIELD(temp_v0, s32 *, 4) = 0;
    M2C_FIELD(temp_v0, s32 *, 8) = 0;
    M2C_FIELD(arg0, f32 *, 0x94) = 1.0f;
    M2C_FIELD(arg0, s32 *, 0x90) = 0;
    M2C_FIELD(arg0, s32 *, 0x40) = (s32) (M2C_FIELD(arg0, s32 *, 0x40) | 0x40);
}
