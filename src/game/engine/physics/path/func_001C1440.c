#include "m2c_prelude.h"
M2C_UNK memset(void *, M2C_UNK, M2C_UNK);           /* extern */

/** @sly2-annot addr=0x1C1440
 * Purpose: Reset a 0x40-byte sweep cursor for the zone obstacle lists
 * Info: proposed name InitObstacleQueryCursor (T2); TU path
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C1440(void *arg0, void *arg1) {
    s32 temp_v0;
    s32 temp_v1;

    memset(arg1, 0, 0x40);
    temp_v1 = M2C_FIELD(arg0, s32 *, 0x100) - 2;
    M2C_FIELD(arg1, s32 *, 0x28) = temp_v1;
    M2C_FIELD(arg1, s32 *, 0x24) = temp_v1;
    temp_v0 = M2C_FIELD(arg0, s32 *, 0x100) - 2;
    M2C_FIELD(arg1, s32 *, 0x20) = -1;
    M2C_FIELD(arg1, s32 *, 0x30) = temp_v0;
    M2C_FIELD(arg1, s32 *, 0x2C) = temp_v0;
}
