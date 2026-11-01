#include "m2c_prelude.h"
/** @sly2-annot addr=0x1441C0
 * Purpose: *pmpCur = min(n, mpMax)
 * Info: proposed name SetStepplayerMp (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void SetStepplayerMp(void *arg0, s32 arg1) {
    s32 *temp_a2;
    s32 temp_v1;

    temp_a2 = M2C_FIELD(arg0, s32 **, 0x12FC);
    if (temp_a2 != NULL) {
        temp_v1 = M2C_FIELD(arg0, s32 *, 0x1300);
        *temp_a2 = (arg1 >= temp_v1) ? temp_v1 : arg1;
    }
}
