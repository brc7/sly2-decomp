#include "m2c_prelude.h"
M2C_UNK func_001448F0(s32, s128);                   /* extern */
M2C_UNK func_00177EF0();                            /* extern */

/** @sly2-annot addr=0x144B58
 * Purpose: BHV168 slot 8: base, then SetJtgoal(owner+0xFD0, goal +0x80)
 * Info: proposed name StartBhvwalkgoal (T1); class BHVWALKGOAL slot8; TU tu_144B28
 * Notes: superseded: OnBhv168Resume (r2b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv168Resume | v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child):
 *   StartBhv168 -> StartBhvwalkgoal | runtime6 P2: goal +0x80 = (13803,-6465) passed to SetJtgoal (S62)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r2b_rest+bhv-slot-canon(r9)+runtime(class role)+runtime
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00144B58(void *arg0) {
    func_00177EF0();
    func_001448F0(M2C_FIELD(arg0, s32 *, 0x10) + 0xFD0, M2C_FIELD(arg0, s128 *, 0x80));
}
