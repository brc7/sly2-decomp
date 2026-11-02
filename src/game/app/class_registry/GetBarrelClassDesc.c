#include "m2c_prelude.h"
extern M2C_UNK D_002BB1C8;

/** @sly2-annot addr=0x113F90
 * Purpose: vtable slot 6 of CID88 (cid 88): returns &D_002BAAE8[88], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBarrelClassDesc (T1); class BARREL slot6; TU crt0_splicewrap
 * Notes: v3 class identity: GetCid88ClassDesc -> GetBarrelClassDesc
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+class_review(master_names_v2_1)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetBarrelClassDesc(void) {
    return &D_002BB1C8;
}
