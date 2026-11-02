#include "m2c_prelude.h"
extern M2C_UNK D_002BB86C;

/** @sly2-annot addr=0x1144E0
 * Purpose: vtable slot 6 of BHV173 (cid 173): returns &D_002BAAE8[173], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhvshootClassDesc (T1); class BHVSHOOT slot6; TU crt0_splicewrap
 * Notes: v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): GetBhv173ClassDesc ->
 *   GetBhvshootClassDesc (BHV173 = BHVSHOOT, runtime7a)
 * Confidence: high [T1 0.95] - evidence: runtime (save-state RAM); sources:
 *   r0_thunks+table-verified(D_002BAAE8 pfnNew/size 185/185; vt slot6 199/199)+runtime(class role)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetBhvshootClassDesc(void) {
    return &D_002BB86C;
}
