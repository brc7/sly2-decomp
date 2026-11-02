#include "m2c_prelude.h"
/** @sly2-annot addr=0x1D7748
 * Purpose: Append (a,b,c) triple to list at +0x18C (max 12)
 * Info: proposed name AddSafehouseEntry (EXISTING); TU tu_1D7238
 * Notes: agent/verifier proposes AddCharSelectEntry for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> AddSafehouseEntry (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.45] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r6_1D5150+arbitration
 * See: docs/reference/functions/player.md#tu_1d7238-0x1d7238-0x1d7a30
 * @end-sly2-annot */
void AddSafehouseEntry(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 temp_t0;
    void *temp_v0;

    temp_t0 = M2C_FIELD(arg0, u32 *, 0x18C);
    if (temp_t0 < 0xCU) {
        M2C_FIELD(arg0, u32 *, 0x18C) = (u32) (temp_t0 + 1);
        temp_v0 = arg0 + ((temp_t0 * 0xC) + 0x190);
        M2C_FIELD(temp_v0, s32 *, 0) = arg1;
        M2C_FIELD(temp_v0, s32 *, 4) = arg2;
        M2C_FIELD(temp_v0, s32 *, 8) = arg3;
    }
}
