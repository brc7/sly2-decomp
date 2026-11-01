#include "m2c_prelude.h"
extern M2C_UNK D_002BB808;

/** @sly2-annot addr=0x114490
 * Purpose: vtable slot 6 of BHV168 (cid 168): returns &D_002BAAE8[168], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhvwalkgoalClassDesc (T1); class BHVWALKGOAL slot6; TU crt0_splicewrap
 * Notes: v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): GetBhv168ClassDesc ->
 *   GetBhvwalkgoalClassDesc
 * Confidence: high [T1 0.95] - evidence: runtime (save-state RAM); sources:
 *   r0_thunks+table-verified(D_002BAAE8 pfnNew/size 185/185; vt slot6 199/199)+runtime(class role)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00114490(void) {
    return &D_002BB808;
}
