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

/** @sly2-annot addr=0x1A36D8
 * Purpose: Toggle flag x160 (clears x15C when set), invalidating before/after
 * Info: proposed name SetLightDynamic (T2); TU light
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A36D8(s32 arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x160)) {
        func_001A3B38(arg0);
        M2C_FIELD(arg0, s32 *, 0x160) = arg1;
        if (arg1 != 0) {
            M2C_FIELD(arg0, s32 *, 0x15C) = 0;
        }
        func_001A3B38(arg0);
    }
}
