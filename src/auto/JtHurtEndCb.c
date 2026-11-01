#include "m2c_prelude.h"
s32 func_001AE318(s32 *, M2C_UNK);                  /* extern */
extern s128 D_002A2DA0;
extern s128 D_002A2DB0;
extern s128 D_002A2DD0;
extern M2C_UNK D_002A3868;
extern M2C_UNK D_002A38A8;
extern M2C_UNK D_002BBB60;
extern M2C_UNK D_002BFAA0;
extern M2C_UNK D_002BFAB0;
extern M2C_UNK D_002BFAC0;
extern f32 D_002BFAD4;
extern s128 D_002BFAE0;
extern s128 D_002BFAF0;
extern s128 D_002BFB10;
extern M2C_UNK D_002BFB28;
extern M2C_UNK D_002BFBD0;
extern M2C_UNK D_002BFC60;
extern f32 D_002BFC94;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE578;
extern M2C_UNK D_002DEA28;
extern M2C_UNK D_002DEB30;
extern s32 D_002E1E54;
extern u32 D_002E1E58;
extern M2C_UNK D_002E1E68;
extern M2C_UNK D_003DD490;
extern void *D_003E04F8;
extern s32 D_003E19E4;
extern M2C_UNK D_004435F0;

void JtHurtEndCb(s32 arg0, s32 arg1) {
    if (*M2C_FIELD(arg0, s32 **, 0x10) > 0) {
        SetJtHurtState(arg0, 0);
        return;
    }
    SetJtHurtState(arg0, 8);
}
