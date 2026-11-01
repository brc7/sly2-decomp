#include "m2c_prelude.h"
M2C_UNK func_0013C440();                            /* extern */

/** @sly2-annot addr=0x13C3B8
 * Purpose: Splice: append entry {id, a, b} (max 32, 0x2C bytes) and make it current (+0x158)
 * Info: proposed name AddBootyEntry (T1); TU booty
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/game-progression.md#5-booty-booty-tu-0x13bdf0,
 *   docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013C3B8(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v1;
    void *temp_v0;

    func_0013C440();
    temp_v1 = M2C_FIELD(arg0, s32 *, 0x150);
    if (temp_v1 < 0x20) {
        M2C_FIELD(arg0, s32 *, 0x150) = (s32) (temp_v1 + 1);
        temp_v0 = M2C_FIELD(arg0, s32 *, 0x154) + (temp_v1 * 0x2C);
        M2C_FIELD(arg0, void **, 0x158) = temp_v0;
        M2C_FIELD(temp_v0, s32 *, 0) = arg1;
        M2C_FIELD(temp_v0, s32 *, 4) = arg2;
        M2C_FIELD(temp_v0, s32 *, 0x14) = arg3;
        M2C_FIELD(temp_v0, s32 *, 0x18) = -1;
        M2C_FIELD(temp_v0, s32 *, 0x1C) = 0;
    }
}
