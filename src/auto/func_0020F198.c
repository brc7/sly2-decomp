#include "m2c_prelude.h"
/** @sly2-annot addr=0x20F198
 * Purpose: ++SW+0x3D84
 * Info: proposed name IncrementSwInputLock (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020F198(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x3D84) = (s32) (M2C_FIELD(arg0, s32 *, 0x3D84) + 1);
}
