#include "m2c_prelude.h"
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
extern M2C_UNK D_0027A10C;
extern M2C_UNK D_0027A1D8;
extern M2C_UNK D_002BAAE8;
extern f32 D_002DDED0;
extern M2C_UNK D_003DD490;
extern f32 *D_003E0E80;
extern M2C_UNK D_003E1144;
extern M2C_UNK D_004520A8;

s32 PoxAddSw(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_v0;

    temp_v0 = PvAllocSlotheapClear(arg0 + 0x3CF8);
    M2C_FIELD(temp_v0, s32 *, 0xC) = (s32) M2C_FIELD(arg2, s32 *, 0x60);
    M2C_FIELD(temp_v0, void **, 4) = (void *) M2C_FIELD(arg1, void **, 0x64);
    M2C_FIELD(arg1, void **, 0x64) = temp_v0;
    return (s32) temp_v0;
}
