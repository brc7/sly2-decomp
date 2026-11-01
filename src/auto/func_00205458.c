#include "m2c_prelude.h"
M2C_UNK func_001EEEE0(s32);                         /* extern */

/** @sly2-annot addr=0x205458
 * Purpose: Find the behaviour of class cid in the step's BHVSTK (0x1EEEE0, FIsBasicDerivedFrom)
 * Info: proposed name PbhvFindStep (T1); TU step
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/player.md#3-step-stepcpp-0x203948,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_00205458(s32 arg0) {
    func_001EEEE0(arg0 + 0x26C);
}
