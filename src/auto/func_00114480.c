#include "m2c_prelude.h"
extern M2C_UNK D_002BB7F4;

/** @sly2-annot addr=0x114480
 * Purpose: vtable slot 6 of BHV167 (cid 167): returns &D_002BAAE8[167], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhvwalkpathClassDesc (T1); class BHVWALKPATH slot6; TU crt0_splicewrap
 * Notes: BHV class word normalised: GetBhv167ClassDesc -> GetBhvwalkpathClassDesc | runtime2/4 confirmed the
 *   BHVWALKPATH role on live guards (S02-S41)
 * Confidence: high [T1 0.97] - evidence: runtime (save-state RAM); sources:
 *   r0_thunks+table-verified(D_002BAAE8 pfnNew/size 185/185; vt slot6 199/199)+runtime(class role)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00114480(void) {
    return &D_002BB7F4;
}
