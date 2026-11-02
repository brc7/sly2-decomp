#include "m2c_prelude.h"
/** @sly2-annot addr=0x1FEE90
 * Purpose: ++SNDMGR+0x5C
 * Info: proposed name IncrementSndmgrCount5C (T2); TU sound
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FEE90(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x5C) = (s32) (M2C_FIELD(arg0, s32 *, 0x5C) + 1);
}
