#include "m2c_prelude.h"
M2C_UNK func_001BA828(s32, M2C_UNK);                /* extern */
M2C_UNK func_001EEF70();                            /* extern */

/** @sly2-annot addr=0x1BA598
 * Purpose: BHV174 s8: StartBhv then state 0
 * Info: proposed name StartBhv174 (T2); class BHV174 slot8; TU tu_1B9F80
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1b9f80-0x1b9f80-0x1bab50
 * @end-sly2-annot */
void func_001BA598(s32 arg0) {
    func_001EEF70();
    func_001BA828(arg0, 0);
}
