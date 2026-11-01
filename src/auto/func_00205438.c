#include "m2c_prelude.h"
M2C_UNK func_001EEE00(s32);                         /* extern */

/** @sly2-annot addr=0x205438
 * Purpose: Push a new behaviour of class cid on the step's BHVSTK (+0x26C) and Init it (0x1EEE00)
 * Info: proposed name PbhvPushStep (T1); TU step
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/player.md#3-step-stepcpp-0x203948, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_00205438(s32 arg0) {
    func_001EEE00(arg0 + 0x26C);
}
