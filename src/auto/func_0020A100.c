#include "m2c_prelude.h"
void *PbhvFindBhvstk(void *, M2C_UNK);               /* extern */

void func_0020A100(void *arg0, s32 arg1, s128 arg2) {
    s32 temp_a1;
    void *temp_v0;
    void *temp_v1;

    M2C_FIELD(arg0, s32 *, 0xF68) = arg1;
    M2C_FIELD(arg0, s128 *, 0xF70) = arg2;
    temp_v0 = PbhvFindBhvstk(arg0 + 0x26C, 0x9C);
    if (temp_v0 != NULL) {
        temp_a1 = M2C_FIELD(arg0, s32 *, 0xF68);
        if ((temp_a1 != 0) && (M2C_FIELD(temp_v0, s32 *, 0x30) == 0x10)) {
            temp_v1 = M2C_FIELD(temp_v0, void **, 0x18);
            M2C_FIELD(temp_v1, s32 *, 0x30) = temp_a1;
            M2C_FIELD(temp_v1, s128 *, 0x40) = (s128) M2C_FIELD(arg0, s128 *, 0xF70);
        }
    }
}
