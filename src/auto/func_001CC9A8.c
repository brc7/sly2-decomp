#include "m2c_prelude.h"
s32 func_00134310(s32, M2C_UNK);                    /* extern */
void *func_001A5BF0(s32);                           /* extern */

/** @sly2-annot addr=0x1CC9A8
 * Purpose: If hands hold an object (state 1) add ignore bits for its tree
 * Info: proposed name GrfJtHandsIgnoreIntersection (T2); TU jthands
 * Notes: names_v2 C proposed ResetCmLookAtSmooth__FP2CMP6VECTOR (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
s32 func_001CC9A8(void *arg0, void *arg1, s32 arg2) {
    s32 temp_v1;
    s32 var_s0;
    s32 var_v0;
    void *temp_v0;

    var_s0 = arg2;
    if ((M2C_FIELD(arg0, s32 *, 0xA4) == 1) && (temp_v0 = func_001A5BF0(M2C_FIELD(arg0, s32 *, 0x84)), (temp_v0 != NULL))) {
        temp_v1 = M2C_FIELD(temp_v0, s32 *, 0xEC);
        var_v0 = var_s0;
        if (temp_v1 != 0) {
            var_v0 = var_s0;
            if (M2C_FIELD(arg1, s32 *, 0xEC) == temp_v1) {
                var_s0 = func_00134310(var_s0, 3);
                goto block_5;
            }
        }
    } else {
block_5:
        var_v0 = var_s0;
    }
    return var_v0;
}
