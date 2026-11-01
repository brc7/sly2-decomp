#include "m2c_prelude.h"
extern M2C_UNK D_002BAB88;

/** @sly2-annot addr=0x113A90
 * Purpose: vtable slot 6 of CID8 (cid 8): returns &D_002BAAE8[8], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetCid8ClassDesc (T1); class CID8 slot6; TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00113A90(void) {
    return &D_002BAB88;
}
