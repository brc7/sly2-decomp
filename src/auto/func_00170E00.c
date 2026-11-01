#include "m2c_prelude.h"
M2C_UNK func_0011A070(s32, void *);                 /* extern */
void *PvAllocSlotheapClear(M2C_UNK *);                     /* extern */
extern M2C_UNK D_003E1548;

void *func_00170E00(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_v0;

    temp_v0 = PvAllocSlotheapClear(&D_003E1548);
    M2C_FIELD(temp_v0, s32 *, 0) = arg1;
    M2C_FIELD(temp_v0, s32 *, 4) = arg2;
    func_0011A070(arg0 + 0x50, temp_v0);
    return temp_v0;
}
