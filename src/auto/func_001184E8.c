#include "m2c_prelude.h"
/** @sly2-annot addr=0x1184E8
 * Purpose: vtable slot 1 override: hands the stored buffer (+0x34 ptr, +0x38 cb) to the read cursor once,
 *   then empties it
 * Info: proposed name CMemInputStream_Refill (T2); class CMemInputStream; TU bis
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void func_001184E8(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x14) = (s32) M2C_FIELD(arg0, s32 *, 0x34);
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) M2C_FIELD(arg0, s32 *, 0x38);
    M2C_FIELD(arg0, s32 *, 0x34) = 0;
}
