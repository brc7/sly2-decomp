#include "m2c_prelude.h"
void *PbhvFindBhvstk(s32, M2C_UNK);                  /* extern */

/** @sly2-annot addr=0x1431D0
 * Purpose: If +0x10DC target has behaviour 0xB1 pointing at the player, clear its +0x60
 * Info: proposed name ReleaseInteractTargetMark (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001431D0(void *arg0) {
    s32 temp_a0;
    void *temp_v0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x10DC);
    if (temp_a0 != 0) {
        temp_v0 = PbhvFindBhvstk(temp_a0 + 0x26C, 0xB1);
        if ((temp_v0 != NULL) && (M2C_FIELD(temp_v0, s32 *, 0x5C) == arg0)) {
            M2C_FIELD(temp_v0, s32 *, 0x60) = 0;
        }
    }
}
