#include "m2c_prelude.h"
/** @sly2-annot addr=0x17B7F8
 * Purpose: state == -1
 * Info: proposed name FBhvshootDone (T1); TU guardbhv
 * Notes: BHV class word normalised: FBhvshootDone -> FBhv173Done | v3 BHV class word (runtime6 P2: BHV194 =
 *   follow, BHV168 = walk-to-goal child): FBhv173Done -> FBhvshootDone | runtime7a: BHVSHOOT role confirmed
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)+runtime
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 FBhvshootDone(void *arg0) {
    return ~M2C_FIELD(arg0, s32 *, 0x30) == 0;
}
