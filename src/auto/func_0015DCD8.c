#include "m2c_prelude.h"
M2C_UNK func_00117EB0(s32, s32);                    /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
s32 *func_001AE318(s32 *, s32, s32);                /* extern */
extern M2C_UNK D_002A2D30;
extern s128 D_002A2DA0;
extern s128 D_002A2DB0;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A5148;
extern f32 D_002A5150;
extern M2C_UNK D_002A5160;
extern M2C_UNK D_002A5168;
extern M2C_UNK D_002BDEB0;
extern M2C_UNK D_002BDEC0;
extern M2C_UNK D_002BE080;
extern M2C_UNK D_002BE1B8;
extern M2C_UNK D_002BE200;
extern M2C_UNK D_002BE248;
extern s32 D_002BE26C;
extern s32 D_002BE270;
extern M2C_UNK D_002BE278;
extern M2C_UNK D_002BE2A8;
extern M2C_UNK D_002BE2D0;
extern M2C_UNK D_002BE2F8;
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern void *D_002DD2FC;
extern f32 D_002DDED0;
extern f32 D_002DDF0C;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE5B0;
extern s128 D_002DE5F0;
extern M2C_UNK D_002DEA28;
extern M2C_UNK D_002DFC00;
extern M2C_UNK D_002E6550;
extern M2C_UNK D_002F66B0;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003D9ED0;
extern M2C_UNK D_003DA4F0;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0FE8;
extern s32 D_003E1C04;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;

void func_0015DCD8(s32 arg0) {
    void *temp_v0;

    UpdateLo(arg0);
    if ((D_002DDED0 - M2C_FIELD(arg0, f32 *, 0x68)) > (M2C_FIELD(arg0, f32 *, 0x5C) + M2C_FIELD(arg0, f32 *, 0x60) + M2C_FIELD(arg0, f32 *, 0x64))) {
        temp_v0 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_v0, void (**)(s32), 0x4C)(arg0 + M2C_FIELD(temp_v0, s16 *, 0x48));
    }
}
