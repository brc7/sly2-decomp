#include "m2c_prelude.h"
void *PvAllocSlotheapClear(M2C_UNK *);                     /* extern */
extern M2C_UNK D_003E1144;

void func_001CF578(void *arg0, s32 arg1) {
    void *temp_v0;

    temp_v0 = PvAllocSlotheapClear(&D_003E1144);
    M2C_FIELD(temp_v0, s32 *, 0) = arg1;
    M2C_FIELD(temp_v0, void **, 8) = (void *) M2C_FIELD(arg0, void **, 0xE8);
    M2C_FIELD(arg0, void **, 0xE8) = temp_v0;
}
