#include "m2c_prelude.h"
M2C_UNK func_00129EC8(void *);                      /* extern */
s32 func_001AE318(s32 *);                           /* extern */
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
extern f32 D_002DDED0;
extern f32 D_002DDF0C;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE5B0;
extern s128 D_002DE5F0;
extern M2C_UNK D_002DEA28;
extern M2C_UNK D_002DFC00;
extern M2C_UNK D_002E6550;
extern M2C_UNK D_002E65E0;
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

/** @sly2-annot addr=0x15D880
 * Purpose: Plays label 0x5BD/0x5BE when the highlight flag changes
 * Info: proposed name SetWidget131Highlight (T2); TU tu_15CD68
 * Notes: names_v2 v1 proposed HandleExplsMessage__FP5EXPLS5MSGIDPv (0.823)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/hud-ui.md#tu_15cd68-0x15cd68-0x15dc80
 * @end-sly2-annot */
void func_0015D880(s32 arg0, s32 arg1) {
    if (M2C_FIELD(arg0, s32 *, 0x12C) != arg1) {
        if (arg1 != 0) {
            if (arg1 == 1) {
                func_00213B00(arg0, 0x5BD);
            }
        } else {
            func_00213B00(arg0, 0x5BE);
        }
        M2C_FIELD(arg0, s32 *, 0x12C) = arg1;
        M2C_FIELD(arg0, f32 *, 0x130) = (f32) D_002DDF0C;
    }
}
