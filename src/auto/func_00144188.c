#include "m2c_prelude.h"
/** @sly2-annot addr=0x144188
 * Purpose: *php = min(n, nHpMax)
 * Info: proposed name SetStepplayerHp (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00144188(void *arg0, s32 arg1) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0xDFC);
    *M2C_FIELD(arg0, s32 **, 0xE00) = (arg1 >= temp_v1) ? temp_v1 : arg1;
}
