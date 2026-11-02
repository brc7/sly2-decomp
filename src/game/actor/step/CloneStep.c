#include "m2c_prelude.h"
M2C_UNK ClonePo();                            /* extern */
M2C_UNK CloneBhvstk(s32, s32);                    /* extern */

/** @sly2-annot addr=0x203A08
 * Purpose: STEP vt41 Clone: ClonePo then re-point the BHVSTK owner (+0x26C) at the clone
 * Info: proposed name CloneStep (T1); class STEP slot41; TU step
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void CloneStep(s32 arg0) {
    ClonePo();
    CloneBhvstk(arg0 + 0x26C, arg0);
}
