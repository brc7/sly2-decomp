#include "m2c_prelude.h"
/** @sly2-annot addr=0x142860
 * Purpose: +0x1100 = 0
 * Info: proposed name UnlockStepplayerLeanTarget (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00142860(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x1100) = 0;
}
