#include "m2c_prelude.h"
M2C_UNK EndJtMode(s32);                         /* extern */

/** @sly2-annot addr=0x1E54B0
 * Purpose: End the STEPPLAYER mode started by StartJtMode (+0xA0 pstepplayerMode -> 0x19CCE8(+0xF00))
 * Info: proposed name ClearJsgJtMode (T2); TU relay
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E54B0(void *arg0) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0xA0);
    if (temp_a0 != 0) {
        EndJtMode(temp_a0 + 0xF00);
        M2C_FIELD(arg0, s32 *, 0xA0) = 0;
    }
}
