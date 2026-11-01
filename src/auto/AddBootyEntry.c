#include "m2c_prelude.h"
M2C_UNK func_0013C440();                            /* extern */

void AddBootyEntry(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v1;
    void *temp_v0;

    func_0013C440();
    temp_v1 = M2C_FIELD(arg0, s32 *, 0x150);
    if (temp_v1 < 0x20) {
        M2C_FIELD(arg0, s32 *, 0x150) = (s32) (temp_v1 + 1);
        temp_v0 = M2C_FIELD(arg0, s32 *, 0x154) + (temp_v1 * 0x2C);
        M2C_FIELD(arg0, void **, 0x158) = temp_v0;
        M2C_FIELD(temp_v0, s32 *, 0) = arg1;
        M2C_FIELD(temp_v0, s32 *, 4) = arg2;
        M2C_FIELD(temp_v0, s32 *, 0x14) = arg3;
        M2C_FIELD(temp_v0, s32 *, 0x18) = -1;
        M2C_FIELD(temp_v0, s32 *, 0x1C) = 0;
    }
}
