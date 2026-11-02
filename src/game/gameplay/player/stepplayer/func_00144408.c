#include "m2c_prelude.h"
/** @sly2-annot addr=0x144408
 * Purpose: jtmode +0xF20 == 2
 * Info: proposed name FStepplayerJtmodeKind2 (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
s32 func_00144408(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0xF20) == 2;
}
