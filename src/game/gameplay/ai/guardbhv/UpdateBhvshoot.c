#include "m2c_prelude.h"
s32 FBhvanimDone(s32);                             /* extern */
M2C_UNK SetBhvshootState(void *, M2C_UNK);             /* extern */
M2C_UNK UpdateBhv();                            /* extern */
s32 FBhvBusyAbove(void *);                          /* extern */

/** @sly2-annot addr=0x17B620
 * Purpose: BHVSHOOT Update: finish when the anim child is done
 * Info: proposed name UpdateBhvshoot (T1); class BHVSHOOT slot10; TU guardbhv
 * Notes: BHV class word normalised: UpdateBhvshoot -> UpdateBhv173 | v3 BHV class word (runtime6 P2: BHV194 =
 *   follow, BHV168 = walk-to-goal child): UpdateBhv173 -> UpdateBhvshoot | runtime7a: BHVSHOOT +0x30 state,
 *   +0x34 tStart, +0x38 target (S67)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)+runtime
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void UpdateBhvshoot(void *arg0) {
    s32 temp_s1;

    UpdateBhv();
loop_1:
    if (FBhvBusyAbove(arg0) == 0) {
        temp_s1 = M2C_FIELD(arg0, s32 *, 0x30);
        if ((temp_s1 == 0) && (FBhvanimDone(M2C_FIELD(arg0, s32 *, 0x18)) != 0)) {
            SetBhvshootState(arg0, -1);
        }
        if (temp_s1 != M2C_FIELD(arg0, s32 *, 0x30)) {
            goto loop_1;
        }
    }
}
