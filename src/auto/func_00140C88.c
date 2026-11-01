#include "m2c_prelude.h"
void *PpoHero(M2C_UNK *);                     /* extern */
M2C_UNK func_00221268(s32, M2C_UNK);                /* extern */
extern M2C_UNK D_003DD490;

void func_00140C88(s32 arg0, s32 arg1) {
    s32 temp_a0;
    void *temp_v0;

    temp_v0 = PpoHero(&D_003DD490);
    if (temp_v0 == arg0) {
        temp_a0 = M2C_FIELD(temp_v0, s32 *, 0x1024);
        if (temp_a0 != 0) {
            func_00221268(temp_a0, 0);
        }
        M2C_FIELD(temp_v0, s32 *, 0x1024) = arg1;
        if (arg1 != 0) {
            func_00221268(arg1, 1);
        }
    }
}
