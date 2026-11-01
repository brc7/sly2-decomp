#include "m2c_prelude.h"
M2C_UNK func_0011A050(s32, M2C_UNK);                /* extern */
M2C_UNK InitLo();                            /* extern */

void InitJob(void *arg0) {
    InitLo();
    M2C_FIELD(arg0, f32 *, 0x9C) = 480.0f;
    M2C_FIELD(arg0, f32 *, 0xA0) = 720.0f;
    M2C_FIELD(arg0, s32 *, 0x6C) = -1;
    M2C_FIELD(arg0, s32 *, 0x70) = -1;
    M2C_FIELD(arg0, s32 *, 0xAC) = -1;
    M2C_FIELD(arg0, s32 *, 0xB0) = -1;
    M2C_FIELD(arg0, s32 *, 0xB4) = -1;
    func_0011A050(arg0 + 0x80, 0x84);
}
