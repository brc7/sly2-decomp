#include "m2c_prelude.h"
/** @sly2-annot addr=0x1FEEC0
 * Purpose: --SNDMGR+0x60
 * Info: proposed name DecrementSndmgrCount60 (T2); TU sound
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FEEC0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x60) = (s32) (M2C_FIELD(arg0, s32 *, 0x60) - 1);
}
