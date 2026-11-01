#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32, M2C_UNK *);              /* extern */
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
M2C_UNK func_0012A0B0(s32, M2C_UNK);                /* extern */
M2C_UNK func_00241728(void *, s32, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_002417A0(void *, M2C_UNK, void *, M2C_UNK); /* extern */
M2C_UNK func_002417F0(void *, M2C_UNK, M2C_UNK);    /* extern */
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A8550;
extern f32 D_002A85F0;
extern f32 D_002A85F4;
extern M2C_UNK D_002BBB60;
extern s32 D_002C3588;
extern s32 D_002C3C5C;
extern M2C_UNK D_002C3C60;
extern M2C_UNK D_002C3C68;
extern M2C_UNK D_002C3C80;
extern M2C_UNK32 D_002C3C90;
extern M2C_UNK D_002C3C98;
extern M2C_UNK D_002C3CA8;
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern void *D_002DE59C;
extern void *D_002DE5A0;
extern M2C_UNK D_003DD2C0;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;

/*
Decompilation failure in function RefGetPeopid:

Found jr instruction at chunk.s line 237, but the corresponding jump table is not provided.

Please include it in the input .s file(s), or in an additional file.

*/

/*
Decompilation failure in function RefSetPeopid:

Found jr instruction at chunk.s line 805, but the corresponding jump table is not provided.

Please include it in the input .s file(s), or in an additional file.

*/

void func_00203CB8(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;

    temp_v0 = PsopartFindSo(arg0, arg1);
    if (temp_v0 != 0) {
        SetStepPhys(arg0, temp_v0, arg2);
    }
}
