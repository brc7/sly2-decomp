#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
s32 func_001AE318(s32 *);                           /* extern */
s32 func_0023E1C8(f32 *, f32);                      /* extern */
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern M2C_UNK D_002A3928;
extern M2C_UNK D_002BBB60;
extern M2C_UNK D_002BFDA8;
extern M2C_UNK D_002BFE08;
extern M2C_UNK D_002BFE68;
extern s32 D_002BFEC8;
extern s32 D_002C00E8;
extern f32 D_002C01C8;
extern f32 D_002C01CC;
extern s32 D_002C0230;
extern M2C_UNK D_002C0248;
extern f32 D_002DDED0;
extern f32 D_002DDF10;
extern s32 D_002DE598;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_002DEB80;
extern M2C_UNK D_002E6550;
extern M2C_UNK D_002E65E0;
extern f32 D_002F6468;
extern f32 D_002F646C;
extern M2C_UNK D_002F6470;
extern M2C_UNK D_002F65D0;
extern M2C_UNK D_002F66B0;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003D9ED0;
extern M2C_UNK D_003DA3C0;
extern M2C_UNK D_003DA530;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E0F9C;
extern s32 D_003E0FE4;

/** @sly2-annot addr=0x1B1B98
 * Purpose: PloFindSwObject by id and call its vt (slot 9)
 * Info: proposed name PloFindCid96Link (T2); class CID96; TU tu_1B0B78
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
s32 func_001B1B98(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 temp_v0;
    void *temp_v0_2;

    temp_v0 = PloFindSwObject__FP2SWi3OIDP2LO((s32) &D_003DD490, 1, arg1, arg0, arg4, arg5, arg6);
    if (temp_v0 != 0) {
        temp_v0_2 = M2C_FIELD(temp_v0, void **, 0xC);
        M2C_FIELD(temp_v0_2, void (**)(s32), 0x4C)(temp_v0 + M2C_FIELD(temp_v0_2, s16 *, 0x48));
    }
    return temp_v0;
}
