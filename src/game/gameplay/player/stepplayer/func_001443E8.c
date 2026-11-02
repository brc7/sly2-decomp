#include "m2c_prelude.h"
/** @sly2-annot addr=0x1443E8
 * Purpose: jtmode +0xF20 == 0
 * Info: proposed name FStepplayerJtmodeKind0 (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
s32 func_001443E8(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0xF20) == 0;
}
