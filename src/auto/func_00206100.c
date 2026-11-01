#include "m2c_prelude.h"
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A85F8;
extern f32 D_002A8600;
extern f32 D_002A8604;
extern f32 D_002A8610;
extern f32 D_002A8614;
extern f32 D_002A8868;
extern M2C_UNK D_002BBB60;
extern M2C_UNK D_002C3CB8;
extern M2C_UNK D_002C3CC8;
extern s32 D_002C3CE4;
extern f32 D_002C3CE8;
extern s32 D_002C3CF0;
extern M2C_UNK D_002C3CF8;
extern f32 D_002C3D04;
extern f32 D_002C3D08;
extern f32 D_002C3D0C;
extern s32 D_002C3D10;
extern f32 D_002C3D4C;
extern M2C_UNK D_002C3D50;
extern M2C_UNK D_002C3D60;
extern s32 D_002DD5BC;
extern s32 D_002DD648;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern s32 D_002E1E40;
extern s32 D_002F7900;
extern M2C_UNK D_003DD040;
extern s64 D_003DD480;
extern s32 D_003DD488;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E1444;
extern M2C_UNK D_003E1A60;

void func_00206100(s32 arg0, s32 arg1) {
    void *temp_s1;
    void *temp_v0;

    temp_s1 = M2C_FIELD(arg0, void **, 0xF64);
    CloneStep(arg0, arg1);
    func_00209770(arg0 + 0xFD0, arg0);
    M2C_FIELD(arg0, void **, 0xF64) = temp_s1;
    temp_v0 = M2C_FIELD(temp_s1, void **, 0xC);
    M2C_FIELD(temp_v0, void (**)(void *, s32), 0x14C)(temp_s1 + M2C_FIELD(temp_v0, s16 *, 0x148), M2C_FIELD(arg1, s32 *, 0xF64));
    func_00208250(arg0);
}
