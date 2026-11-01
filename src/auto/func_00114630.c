#include "m2c_prelude.h"
extern M2C_UNK D_002BBA10;

/** @sly2-annot addr=0x114630
 * Purpose: vtable slot 6 of BHV194 (cid 194): returns &D_002BAAE8[194], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhvfollowClassDesc (T1); class BHVFOLLOW slot6; TU crt0_splicewrap
 * Notes: v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): GetBhv194ClassDesc ->
 *   GetBhvfollowClassDesc
 * Confidence: high [T1 0.95] - evidence: runtime (save-state RAM); sources:
 *   r0_thunks+table-verified(D_002BAAE8 pfnNew/size 185/185; vt slot6 199/199)+runtime(class role)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00114630(void) {
    return &D_002BBA10;
}
