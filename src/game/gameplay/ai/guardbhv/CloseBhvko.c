#include "m2c_prelude.h"
M2C_UNK RecalcSwOxfFilterForObject(M2C_UNK *, s32);              /* extern */
M2C_UNK CloseBhvhit();                            /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x179A20
 * Purpose: BHVKO Close: BHVHIT Close and refresh world filter
 * Info: proposed name CloseBhvko (T1); class BHVKO slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVKO role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void CloseBhvko(void *arg0) {
    CloseBhvhit();
    RecalcSwOxfFilterForObject(&D_003DD490, M2C_FIELD(arg0, s32 *, 0x10));
}
