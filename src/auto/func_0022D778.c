#include "m2c_prelude.h"
M2C_UNK func_0022D6B0(s32, s32, void *);            /* extern */

/** @sly2-annot addr=0x22D778
 * Purpose: PxfResolveId(id, parent, this)
 * Info: proposed name SO_PxfFromId (T2); class SO slot51; TU xf
 * Notes: agent qualifier: (slot51)
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_0022D778(void *arg0, s32 arg1) {
    func_0022D6B0(arg1, M2C_FIELD(arg0, s32 *, 0x10), arg0);
}
