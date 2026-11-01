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

/** @sly2-annot addr=0x1AF660
 * Purpose: In state 7: keep only root page, state 8
 * Info: proposed name PopPauseMenuToRoot (T2); TU menu
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AF660(s32 arg0, s32 arg1) {
    if (M2C_FIELD(arg0, s32 *, 0x1E8) == 7) {
        if (M2C_FIELD(arg0, s32 *, 0x24) >= 2) {
            M2C_FIELD(arg0, s32 *, 0x24) = 1;
        }
        func_001AEAA8(arg0, 8);
    }
}
