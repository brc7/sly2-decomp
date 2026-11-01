#include "m2c_prelude.h"
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A8280;
extern f32 D_002A8284;
extern M2C_UNK D_002CA280;
extern M2C_UNK D_002CA2C0;
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern M2C_UNK D_002DDED0;
extern M2C_UNK D_003DD020;
extern M2C_UNK D_003DD490;
extern s32 D_003E12EC;
extern void *D_003E145C;
extern M2C_UNK D_003E15CC;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;

void func_001F7D10(s32 arg0, s32 arg1, f32 fparg0) {
    void *temp_v0;
    void *temp_v0_2;

    if (fparg0 <= 0.0f) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(arg0, 0x69) == 0) {
            temp_v0 = M2C_FIELD(arg0, void **, 0xC);
            M2C_FIELD(temp_v0, void (**)(s32), 0x4C)(arg0 + M2C_FIELD(temp_v0, s16 *, 0x48));
        }
    } else {
        temp_v0_2 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_v0_2, void (**)(s32), 0x44)(arg0 + M2C_FIELD(temp_v0_2, s16 *, 0x40));
    }
    M2C_FIELD(arg0, f32 *, 0x12C) = fparg0;
}
