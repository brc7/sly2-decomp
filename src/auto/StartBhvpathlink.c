#include "m2c_prelude.h"
M2C_UNK AdvanceBhvpathlink(s32);                         /* extern */
M2C_UNK StartBhv();                            /* extern */
M2C_UNK SetBhvBusy(s32);                         /* extern */

/** @sly2-annot addr=0x17E3D8
 * Purpose: BHVPATHLINK Start: busy, begin the first link segment
 * Info: proposed name StartBhvpathlink (T1); class BHVPATHLINK slot8; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATHLINK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvpathlink(s32 arg0) {
    StartBhv();
    SetBhvBusy(arg0);
    AdvanceBhvpathlink(arg0);
}
