#include "m2c_prelude.h"
/** @sly2-annot addr=0x20F1B8
 * Purpose: SW+0x3D84 > 0
 * Info: proposed name FSwInputLock (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
s32 func_0020F1B8(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x3D84) > 0;
}
