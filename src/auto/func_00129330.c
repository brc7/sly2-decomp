#include "m2c_prelude.h"
s128 *PvAllocSlotheapClear(M2C_UNK *);                     /* extern */
extern M2C_UNK D_004521B8;

void *func_00129330(void *arg0, s128 *arg1) {
    s128 *temp_v0;

    M2C_FIELD(arg0, s32 *, 0) = 0x1C;
    temp_v0 = PvAllocSlotheapClear(&D_004521B8);
    M2C_FIELD(temp_v0, s32 *, 0x10) = 0;
    M2C_FIELD(arg0, s128 **, 4) = temp_v0;
    M2C_FIELD(temp_v0, s32 *, 0x10) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x10) + 1);
    *M2C_FIELD(arg0, s128 **, 4) = *arg1;
    return arg0;
}
