#include "m2c_prelude.h"
M2C_UNK CloseBhvanim();                            /* extern */
M2C_UNK ClearBhvBusy(s32);                         /* extern */

/** @sly2-annot addr=0x17A3D0
 * Purpose: BHVLEAP Close: BHVANIM Close, clear busy
 * Info: proposed name CloseBhvleap (T1); class BHVLEAP slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVLEAP role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void CloseBhvleap(s32 arg0) {
    CloseBhvanim();
    ClearBhvBusy(arg0);
}
