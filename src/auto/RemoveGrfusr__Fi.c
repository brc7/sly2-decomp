#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, M2C_UNK *, M2C_UNK);   /* extern */
M2C_UNK func_001177A8();                            /* extern */
M2C_UNK func_00117EB0(s32, s32);                    /* extern */
s32 func_00242810();                                /* extern */
s32 func_002432D0(s32, s32, M2C_UNK *);             /* extern */
s32 func_00243388(s32, s32);                        /* extern */
s32 scePadGetState(s32, s32);                       /* extern */
extern s128 D_002A2DC0;
extern f32 D_002A5684;
extern f32 D_002A5688;
extern f32 D_002A568C;
extern f32 D_002A5690;
extern M2C_UNK D_002BBB60;
extern f32 D_002BE908;
extern f32 D_002BE90C;
extern f32 D_002BE910;
extern M2C_UNK D_002BE920;
extern M2C_UNK D_002BE930;
extern M2C_UNK D_002BE950;
extern M2C_UNK D_002BE990;
extern M2C_UNK D_002BF1B0;
extern f32 D_002BF2D0;
extern f32 D_002BF2D4;
extern f32 D_002BF2D8;
extern f32 D_002BF2DC;
extern M2C_UNK D_002BF310;
extern M2C_UNK D_002BF320;
extern M2C_UNK D_002BF330;
extern M2C_UNK D_002D2918;
extern M2C_UNK D_002DDEC0;
extern M2C_UNK D_002DDED0;
extern f32 D_002DDED4;
extern f32 D_002DDF0C;
extern M2C_UNK D_002DDF40;
extern f32 D_002DEB30;
extern s32 D_002DFB84;
extern s32 D_002DFBC0;
extern s32 D_002DFBC4;
extern s32 D_002E0CBC;
extern s32 D_002E1E40;
extern M2C_UNK D_003DD490;

s32 RemoveGrfusr__Fi(s32 arg0) {
    D_002DFBC4 &= ~arg0;
    return UpdateGrfjoytFromGrfusr__Fv();
}
