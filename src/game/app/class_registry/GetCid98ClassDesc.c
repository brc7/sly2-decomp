#include "m2c_prelude.h"
extern M2C_UNK D_002BB290;

/** @sly2-annot addr=0x114030
 * Purpose: vtable slot 6 of CID98 (cid 98): returns &D_002BAAE8[98], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetCid98ClassDesc (T1); class CID98 slot6; TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetCid98ClassDesc(void) {
    return &D_002BB290;
}
