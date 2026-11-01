#include "m2c_prelude.h"
extern M2C_UNK D_002BB970;

/** @sly2-annot addr=0x1145B0
 * Purpose: vtable slot 6 of BHV186 (cid 186): returns &D_002BAAE8[186], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhvgotoClassDesc (T1); class BHVGOTO slot6; TU crt0_splicewrap
 * Notes: BHV class word normalised: GetBhv186ClassDesc -> GetBhvgotoClassDesc | runtime2/4 confirmed the
 *   BHVGOTO role on live guards (S02-S41)
 * Confidence: high [T1 0.97] - evidence: runtime (save-state RAM); sources:
 *   r0_thunks+table-verified(D_002BAAE8 pfnNew/size 185/185; vt slot6 199/199)+runtime(class role)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetBhvgotoClassDesc(void) {
    return &D_002BB970;
}
