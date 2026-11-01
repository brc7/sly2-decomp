#include "m2c_prelude.h"
void *PvAllocSlotheapClear(M2C_UNK *);                     /* extern */
extern M2C_UNK D_004521A0;

void *func_001292B0(void *arg0, void *arg1) {
    void *temp_a1;
    void *temp_v0;

    M2C_FIELD(arg0, s32 *, 0) = 0x1B;
    temp_v0 = PvAllocSlotheapClear(&D_004521A0);
    M2C_FIELD(temp_v0, s32 *, 0x40) = 0;
    M2C_FIELD(arg0, void **, 4) = temp_v0;
    M2C_FIELD(temp_v0, s32 *, 0x40) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x40) + 1);
    temp_a1 = M2C_FIELD(arg0, void **, 4);
    M2C_FIELD(temp_a1, s128 *, 0) = (s128) M2C_FIELD(arg1, s128 *, 0);
    M2C_FIELD(temp_a1, s128 *, 0x10) = (s128) M2C_FIELD(arg1, s128 *, 0x10);
    M2C_FIELD(temp_a1, s128 *, 0x20) = (s128) M2C_FIELD(arg1, s128 *, 0x20);
    M2C_FIELD(temp_a1, s128 *, 0x30) = (s128) M2C_FIELD(arg1, s128 *, 0x30);
    return arg0;
}
