#include "m2c_prelude.h"
M2C_UNK SetBhvrailState(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */

/** @sly2-annot addr=0x17CB10
 * Purpose: BHVRAIL Start: state 0
 * Info: proposed name StartBhvrail (T1); class BHVRAIL slot8; TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvrail(s32 arg0) {
    StartBhv();
    SetBhvrailState(arg0, 0);
}
