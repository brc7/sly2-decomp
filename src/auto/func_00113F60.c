#include "m2c_prelude.h"
extern M2C_UNK D_002BB18C;

/** @sly2-annot addr=0x113F60
 * Purpose: vtable slot 6 of CID85 (cid 85): returns &D_002BAAE8[85], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetLasenClassDesc (T1); class LASEN slot6; TU crt0_splicewrap
 * Notes: v3 class identity: GetCid85ClassDesc -> GetLasenClassDesc
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+class_review(master_names_v2_1)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00113F60(void) {
    return &D_002BB18C;
}
