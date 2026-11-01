#include "m2c_prelude.h"
/** @sly2-annot addr=0x1424C0
 * Purpose: Splice predicate: jtthrow state (+0x11B4) == -1
 * Info: proposed name FStepplayerNotThrowing (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
s32 func_001424C0(void *arg0) {
    return ~M2C_FIELD(arg0, s32 *, 0x11B4) == 0;
}
