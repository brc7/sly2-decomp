#include "m2c_prelude.h"
M2C_UNK SetBhvanimState(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, s32);                    /* extern */

/** @sly2-annot addr=0x174B28
 * Purpose: BHVANIM Close: base Close, state 1 (stop anim), lower flag +0x3C
 * Info: proposed name CloseBhvanim (T1); class BHVANIM slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVANIM role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void CloseBhvanim(void *arg0) {
    CloseBhv();
    SetBhvanimState(arg0, 1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(arg0, s32 *, 0x3C));
}
