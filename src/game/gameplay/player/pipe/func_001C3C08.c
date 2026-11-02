#include "m2c_prelude.h"
/** @sly2-annot addr=0x1C3C08
 * Purpose: +0x1AC = (arg==0)
 * Info: proposed name SetPipeGrabDisabled (T2); class PIPE; TU pipe
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/player.md#pipe-0x1c3390-0x1c3d70
 * @end-sly2-annot */
void func_001C3C08(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x1AC) = (s32) (arg1 == 0);
}
