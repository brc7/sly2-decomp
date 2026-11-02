#include "m2c_prelude.h"
extern M2C_UNK D_002BB498;

/** @sly2-annot addr=0x1141D0
 * Purpose: vtable slot 6 of CID124 (cid 124): returns &D_002BAAE8[124], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetPathlinkClassDesc (T1); class PATHLINK slot6; TU crt0_splicewrap
 * Notes: v3 class identity: GetCid124ClassDesc -> GetPathlinkClassDesc
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+class_review(master_names_v2_1)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetPathlinkClassDesc(void) {
    return &D_002BB498;
}
