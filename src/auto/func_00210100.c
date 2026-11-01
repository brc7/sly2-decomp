#include "m2c_prelude.h"
/** @sly2-annot addr=0x210100
 * Purpose: Return &array(+0x39D0)[count(+0x39D4)++] (12-byte records)
 * Info: proposed name PAllocSwLoQueue (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
s32 func_00210100(void *arg0) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x39D4);
    M2C_FIELD(arg0, s32 *, 0x39D4) = (s32) (temp_v1 + 1);
    return M2C_FIELD(arg0, s32 *, 0x39D0) + (temp_v1 * 0xC);
}
