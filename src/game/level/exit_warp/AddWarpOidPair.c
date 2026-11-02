#include "m2c_prelude.h"
/** @sly2-annot addr=0x228958
 * Purpose: if count<4 append (a,b) at 0x14C
 * Info: proposed name AddWarpOidPair (EXISTING); TU exit_warp
 * Notes: descriptive only: Add oid pair to 4-slot list at 0x148 | RENAME proposed -> AddWarpOidPair
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.50] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r8_21BB88+arbitration
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void AddWarpOidPair(void *arg0, s32 arg1, s32 arg2) {
    u32 temp_v1;
    void *temp_v0;

    temp_v1 = M2C_FIELD(arg0, u32 *, 0x148);
    if (temp_v1 < 4U) {
        M2C_FIELD(arg0, u32 *, 0x148) = (u32) (temp_v1 + 1);
        temp_v0 = arg0 + ((temp_v1 * 8) + 0x14C);
        M2C_FIELD(temp_v0, s32 *, 0) = arg1;
        M2C_FIELD(temp_v0, s32 *, 4) = arg2;
    }
}
