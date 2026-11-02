#include "m2c_prelude.h"
M2C_UNK InitBhvanim();                            /* extern */

/** @sly2-annot addr=0x17A9D0
 * Purpose: BHVHELD Init: BHVANIM Init with flag 0x17 (carriable/held)
 * Info: proposed name InitBhvjuggled (T1); class BHVJUGGLED slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvheld -> InitBhvjuggled | runtime2/4 confirmed the BHVJUGGLED role
 *   on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void InitBhvjuggled(void *arg0) {
    InitBhvanim();
    M2C_FIELD(arg0, s32 *, 0x3C) = 0x17;
    M2C_FIELD(arg0, s32 *, 0x50) = -1;
}
