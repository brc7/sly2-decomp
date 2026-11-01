#include "m2c_prelude.h"
extern M2C_UNK D_002BAC64;

/** @sly2-annot addr=0x113B40
 * Purpose: vtable slot 6 of CID19 (cid 19): returns &D_002BAAE8[19], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetHndClassDesc (T1); class HND slot6; TU crt0_splicewrap
 * Notes: v3 class identity: GetCid19ClassDesc -> GetHndClassDesc
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+class_review(master_names_v2_1)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00113B40(void) {
    return &D_002BAC64;
}
