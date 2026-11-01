#include "m2c_prelude.h"
M2C_UNK func_001448F0(s32, s128);                   /* extern */
M2C_UNK func_001789F8();                            /* extern */

/** @sly2-annot addr=0x144B90
 * Purpose: BHV168 slot 10: BHV166 update (0x1789F8), then refresh SetJtgoal
 * Info: proposed name UpdateBhvwalkgoal (T1); class BHVWALKGOAL slot10; TU tu_144B28
 * Notes: BHV slot 10 (Update): canonical form would be UpdateBhv168 | v3 BHV class word (runtime6 P2: BHV194
 *   = follow, BHV168 = walk-to-goal child): UpdateBhv168Goal -> UpdateBhvwalkgoal | runtime6 P2: walk-to-goal
 *   update; state +0x4C = 2 (arrived) in S62
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime(class
 *   role)+runtime
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00144B90(void *arg0) {
    func_001789F8();
    func_001448F0(M2C_FIELD(arg0, s32 *, 0x10) + 0xFD0, M2C_FIELD(arg0, s128 *, 0x80));
}
