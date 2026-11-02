#include "m2c_prelude.h"
M2C_UNK func_001C7A08();                            /* extern */

/** @sly2-annot addr=0x1C7988
 * Purpose: calls 0x1C7A08 stub
 * Info: proposed name AddLootEntry (T1); TU prize
 * Notes: names_v2 C proposed NotifyAlarmSensorsOnTrigger__FP5ALARM (0.167) | superseded: LootUnk7988
 *   (arbitration) | arbitration knock-on rename: CONFIRMED static: LOOT twin of AddBootyEntry 0x13C3B8:
 *   entry=+0x240+0x2C*count (count +0x23C<32), stores (a1,a2), -1,-1, oid count 0, sets current entry +0x244.
 * Confidence: mid (draft) [T1 0.70] - evidence: arbitration; static naming pass; sources:
 *   r5_1AE130+arbitration
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C7988(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_v1;
    void *temp_v0;

    func_001C7A08();
    temp_v1 = M2C_FIELD(arg0, s32 *, 0x23C);
    if (temp_v1 < 0x20) {
        M2C_FIELD(arg0, s32 *, 0x23C) = (s32) (temp_v1 + 1);
        temp_v0 = M2C_FIELD(arg0, s32 *, 0x240) + (temp_v1 * 0x2C);
        M2C_FIELD(arg0, void **, 0x244) = temp_v0;
        M2C_FIELD(temp_v0, s32 *, 0) = arg1;
        M2C_FIELD(temp_v0, s32 *, 4) = arg2;
        M2C_FIELD(temp_v0, s32 *, 0x14) = -1;
        M2C_FIELD(temp_v0, s32 *, 0x18) = -1;
        M2C_FIELD(temp_v0, s32 *, 0x1C) = 0;
    }
}
