#include "m2c_prelude.h"
M2C_UNK func_001CC2E8(void *);                      /* extern */

/** @sly2-annot addr=0x18ED70
 * Purpose: If jtcs (x18D8)==0 (reach) run the reach tracker 0x1CC2E8 on JT+0x17D0
 * Info: proposed name UpdateJtCane (T1); TU cane
 * Notes: names_v2 B proposed EnsureAloSfx__FP3ALO (0.856)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#cane-0x18e540-0x18f908
 * @end-sly2-annot */
void func_0018ED70(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x18D8) == 0) {
        func_001CC2E8(arg0 + 0x17D0);
    }
}
