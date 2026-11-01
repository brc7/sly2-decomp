#include "m2c_prelude.h"
/** @sly2-annot addr=0x1424D0
 * Purpose: Splice predicate: jtgrab state (+0x4C8) != 9
 * Info: proposed name FStepplayerNotGrabState9 (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
s32 func_001424D0(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x4C8) != 9;
}
