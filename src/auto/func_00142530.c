#include "m2c_prelude.h"
/** @sly2-annot addr=0x142530
 * Purpose: Splice predicate: jtgrab state != 0xC
 * Info: proposed name FStepplayerNotGrabStateC (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
s32 func_00142530(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x4C8) != 0xC;
}
