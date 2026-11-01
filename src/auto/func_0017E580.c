#include "m2c_prelude.h"
M2C_UNK func_0017E6B0(void *, M2C_UNK);             /* extern */
M2C_UNK func_001EF038(void *);                      /* extern */

/** @sly2-annot addr=0x17E580
 * Purpose: Next segment of the pathlink (+0x34 < count +0x50) or finish and clear busy
 * Info: proposed name AdvanceBhvpathlink (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATHLINK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017E580(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x34) + 1;
    M2C_FIELD(arg0, s32 *, 0x34) = temp_v0;
    if (temp_v0 < M2C_FIELD(M2C_FIELD(arg0, void **, 0x30), s32 *, 0x50)) {
        func_0017E6B0(arg0, -1);
        func_0017E6B0(arg0, 0);
        return;
    }
    func_0017E6B0(arg0, -1);
    func_001EF038(arg0);
}
