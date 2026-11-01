#include "m2c_prelude.h"
/** @sly2-annot addr=0x153810
 * Purpose: Append {oid, kind, -1} (max 128, 0x1C each) at +0x2A8
 * Info: proposed name AddRhythmMgrGrade (T2); TU rhythm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_00153810(void *arg0, s32 arg1, s32 arg2) {
    u32 temp_a0;
    void *temp_v0;

    temp_a0 = M2C_FIELD(arg0, u32 *, 0x2A8);
    if (temp_a0 < 0x80U) {
        M2C_FIELD(arg0, u32 *, 0x2A8) = (u32) (temp_a0 + 1);
        temp_v0 = arg0 + ((temp_a0 * 0x1C) + 0x2AC);
        M2C_FIELD(temp_v0, s32 *, 0) = arg1;
        M2C_FIELD(temp_v0, s32 *, 4) = arg2;
        M2C_FIELD(temp_v0, s32 *, 0xC) = -1;
    }
}
