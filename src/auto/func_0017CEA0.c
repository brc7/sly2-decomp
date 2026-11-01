#include "m2c_prelude.h"
M2C_UNK func_0017D058(s32, M2C_UNK);                /* extern */
M2C_UNK func_001EEF70();                            /* extern */

/** @sly2-annot addr=0x17CEA0
 * Purpose: BHVPIPE Start: state 0
 * Info: proposed name StartBhvpipe (T1); class BHVPIPE slot8; TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017CEA0(s32 arg0) {
    func_001EEF70();
    func_0017D058(arg0, 0);
}
