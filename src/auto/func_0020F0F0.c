#include "m2c_prelude.h"
M2C_UNK func_001870A0(M2C_UNK);                     /* extern */

/** @sly2-annot addr=0x20F0F0
 * Purpose: --SW+0x3D7C no-pause counter (last -> RemoveGrfusr(4))
 * Info: proposed name DecrementSwNoPause (T1); TU sw
 * Notes: names_v2 B proposed DecrementSwHandsOff__FP2SW (0.846) | v3: superseded DecrementSwUsrLock |
 *   runtime6 N1 pair; v2 DecrementSwUsrLock
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7_1F8DD8+runtime6
 * See: docs/engine/world-sw.md#5-hands-off-and-locks,
 *   docs/guides/naming-workflow.md#1-where-things-stand-master_names_v2,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_0020F0F0(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x3D7C) - 1;
    M2C_FIELD(arg0, s32 *, 0x3D7C) = temp_v0;
    if (temp_v0 == 0) {
        func_001870A0(4);
    }
}
