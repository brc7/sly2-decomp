#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(void *, s32 *, s32);        /* extern */
M2C_UNK func_001291A8(s32 *, s32 *, s32, s32);      /* extern */
M2C_UNK func_00129EC8(s32 *);                       /* extern */
void *func_001AE318(s32 *);                         /* extern */
extern s128 D_002A2DA0;
extern s128 D_002A2DC0;
extern M2C_UNK D_002A4D50;
extern f32 D_002A4D64;
extern f32 D_002A4D68;
extern M2C_UNK D_002A4D88;
extern M2C_UNK D_002A4D90;
extern M2C_UNK D_002A4D98;
extern M2C_UNK D_002A4DA0;
extern M2C_UNK D_002A4DB0;
extern M2C_UNK D_002A4DC0;
extern M2C_UNK D_002A4DC8;
extern M2C_UNK D_002A4E18;
extern M2C_UNK D_002A4E20;
extern f32 D_002A4E30;
extern f32 D_002A4E34;
extern M2C_UNK D_002BBB60;
extern f32 D_002BD268;
extern f32 D_002BD26C;
extern M2C_UNK D_002BD270;
extern M2C_UNK D_002BD280;
extern M2C_UNK D_002BD290;
extern M2C_UNK D_002BD2A0;
extern M2C_UNK D_002BD2B0;
extern f32 D_002BD2D0;
extern f32 D_002BD2D4;
extern f32 D_002BD2D8;
extern M2C_UNK D_002BD2E0;
extern f32 D_002BD2F0;
extern M2C_UNK D_002BD300;
extern M2C_UNK D_002BD310;
extern M2C_UNK D_002BD320;
extern M2C_UNK D_002BD330;
extern M2C_UNK D_002BD338;
extern f32 D_002BD340;
extern s32 D_002DD640;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern s32 D_002DDF54;
extern M2C_UNK D_002DEB30;
extern s32 D_002E1E40;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0EA8;
extern s32 D_003E1084;
extern void *D_003E1474;
extern s32 D_003E1AC8;

void func_001424E0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_a1;

    var_a1 = arg1;
    if (var_a1 == 0) {
        var_a1 = M2C_FIELD(arg0, s32 *, 0x9C8);
    }
    M2C_FIELD(arg0, s32 *, 0x9CC) = var_a1;
    SetJtGrabState(arg0 + 0x4C0, 0xC, arg2, 0);
    M2C_FIELD(arg0, s32 *, 0x1318) = arg3;
}
