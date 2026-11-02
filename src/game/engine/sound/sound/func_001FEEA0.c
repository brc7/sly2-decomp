#include "m2c_prelude.h"
/** @sly2-annot addr=0x1FEEA0
 * Purpose: --SNDMGR+0x5C
 * Info: proposed name DecrementSndmgrCount5C (T2); TU sound
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FEEA0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x5C) = (s32) (M2C_FIELD(arg0, s32 *, 0x5C) - 1);
}
