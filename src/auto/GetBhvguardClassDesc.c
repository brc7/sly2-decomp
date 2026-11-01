#include "m2c_prelude.h"
extern M2C_UNK D_002BB718;

/** @sly2-annot addr=0x1143D0
 * Purpose: vtable slot 6 of BHV156 (cid 156): returns &D_002BAAE8[156], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhvguardClassDesc (T1); class BHVGUARD slot6; TU crt0_splicewrap
 * Notes: BHV class word normalised: GetBhv156ClassDesc -> GetBhvguardClassDesc | runtime2/4 confirmed the
 *   BHVGUARD role on live guards (S02-S41)
 * Confidence: high [T1 0.97] - evidence: runtime (save-state RAM); sources:
 *   r0_thunks+table-verified(D_002BAAE8 pfnNew/size 185/185; vt slot6 199/199)+runtime(class role)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetBhvguardClassDesc(void) {
    return &D_002BB718;
}
