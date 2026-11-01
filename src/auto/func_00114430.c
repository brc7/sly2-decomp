#include "m2c_prelude.h"
extern M2C_UNK D_002BB790;

/** @sly2-annot addr=0x114430
 * Purpose: vtable slot 6 of BHV162 (cid 162): returns &D_002BAAE8[162], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhv162ClassDesc (T1); class BHV162 slot6; TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00114430(void) {
    return &D_002BB790;
}
