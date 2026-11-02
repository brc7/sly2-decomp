#include "m2c_prelude.h"
/** @sly2-annot addr=0x1FE5E0
 * Purpose: +0x68 == 3
 * Info: proposed name FSndmgrMuted (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
s32 func_001FE5E0(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x68) == 3;
}
