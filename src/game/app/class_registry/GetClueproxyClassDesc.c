#include "m2c_prelude.h"
extern M2C_UNK D_002BAB74;

/** @sly2-annot addr=0x113A80
 * Purpose: vtable slot 6 of CID7 (cid 7): returns &D_002BAAE8[7], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetClueproxyClassDesc (T1); class CLUEPROXY slot6; TU crt0_splicewrap
 * Notes: v3 class identity: GetCid7ClassDesc -> GetClueproxyClassDesc
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+class_review(master_names_v2_1)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetClueproxyClassDesc(void) {
    return &D_002BAB74;
}
