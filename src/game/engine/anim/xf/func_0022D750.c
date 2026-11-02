#include "m2c_prelude.h"
M2C_UNK func_0022D6B0(s32, s32, M2C_UNK);           /* extern */

/** @sly2-annot addr=0x22D750
 * Purpose: PxfResolveId(id, parent table, 0)
 * Info: proposed name XFM_PxfFromId (T2); class XFM slot49; TU xf
 * Notes: agent qualifier: (slot49)
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_0022D750(void *arg0, s32 arg1) {
    func_0022D6B0(arg1, M2C_FIELD(arg0, s32 *, 0x10), 0);
}
