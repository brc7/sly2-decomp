#include "m2c_prelude.h"
s32 func_001AE318(s32 *);                           /* extern */
extern s128 D_002A2DD0;
extern f32 D_002A5370;
extern f32 D_002A53B8;
extern M2C_UNK D_002A53C0;
extern M2C_UNK D_002A53C8;
extern f32 D_002A53D0;
extern f32 D_002A53D4;
extern f32 D_002BE5A0;
extern M2C_UNK D_002BE5A8;
extern M2C_UNK D_002BE5B0;
extern f32 D_002BE5B8;
extern f32 D_002BE5BC;
extern f32 D_002BE5C0;
extern f32 D_002BE5C4;
extern f32 D_002BE5C8;
extern f32 D_002BE5CC;
extern f32 D_002BE5D0;
extern f32 D_002BE5D4;
extern f32 D_002BE5D8;
extern f32 D_002BE5DC;
extern f32 D_002BE5E0;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DD490;
extern void *D_003E13B4;
extern s32 D_003E13C0;
extern s32 D_003E13CC;
extern s32 D_003E13D8;
extern s32 D_003E13E4;
extern s32 D_003E13F0;
extern s32 D_003E13FC;
extern s32 D_003E1408;
extern s32 D_003E1450;

/** @sly2-annot addr=0x171DA8
 * Purpose: True if the guard's BHV156 is in mode 5 with this script
 * Info: proposed name FGuardRunningScript (T2); TU tu_1709A8
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
s32 func_00171DA8(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 var_a1;

    temp_v0 = func_00205458(arg0, 0x9C);
    var_a1 = 0;
    if ((temp_v0 != 0) && (M2C_FIELD(temp_v0, s32 *, 0x30) == 5)) {
        var_a1 = M2C_FIELD(temp_v0, s32 *, 0x40) == arg1;
    }
    return var_a1;
}
