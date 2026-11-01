#include "m2c_prelude.h"
extern s128 D_002A2DA0;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A5674;
extern f32 D_002A5680;
extern M2C_UNK D_002BE888;
extern f32 D_002BE898;
extern f32 D_002BE89C;
extern f32 D_002BE8A0;
extern f32 D_002BE8A4;
extern f32 D_002BE8A8;
extern s32 D_002BE8AC;
extern f32 D_002BE8B0;
extern f32 D_002BE8B4;
extern f32 D_002BE8B8;
extern f32 D_002BE8BC;
extern f32 D_002BE8C0;
extern f32 D_002BE8C4;
extern f32 D_002BE8C8;
extern M2C_UNK D_002BE8D0;
extern f32 D_002BE900;
extern f32 D_002BE904;
extern f32 D_002DDED0;
extern M2C_UNK D_002DEB30;
extern s32 D_002DFB80;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x183A50
 * Purpose: vt61: SO base, and ignore a STEPPLAYER that is holding this object (grab state 1 on it)
 * Info: proposed name BARREL_FIgnoreIntersection (T2); class BARREL slot61; TU grabspot
 * Notes: v3 class identity: CID88_FIgnoreIntersection -> BARREL_FIgnoreIntersection
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
s32 func_00183A50(s32 arg0, s32 arg1) {
    s32 var_s1;
    s32 var_v0;

    var_s1 = func_001F4FC8(arg0, arg1);
    if (func_00132B10(arg1, 0x34) != 0) {
        if ((M2C_FIELD(arg1, s32 *, 0x4C8) == 1) && (arg0 == M2C_FIELD(arg1, s32 *, 0x4D4))) {
            var_s1 = func_00134310(var_s1, 1);
        }
        var_v0 = var_s1;
        if ((u32) (M2C_FIELD(arg1, s32 *, 0x2DC) - 6) < 2U) {
            var_v0 = var_s1;
            if (arg0 == M2C_FIELD(arg1, s32 *, 0x2CC)) {
                var_s1 = func_00134310(var_s1, 1);
                goto block_7;
            }
        }
    } else {
block_7:
        var_v0 = var_s1;
    }
    return var_v0;
}
