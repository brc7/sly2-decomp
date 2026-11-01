#include "m2c_prelude.h"
extern M2C_UNK D_002BB434;

/** @sly2-annot addr=0x114180
 * Purpose: vtable slot 6 of CID119 (cid 119): returns &D_002BAAE8[119], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetCid119ClassDesc (T1); class CID119 slot6; TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00114180(void) {
    return &D_002BB434;
}
