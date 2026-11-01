#include "m2c_prelude.h"
extern M2C_UNK D_002BB4D4;

/** @sly2-annot addr=0x114200
 * Purpose: vtable slot 6 of WIDGET127 (cid 127): returns &D_002BAAE8[127], the class descriptor {pcdParent,
 *   cid, cidEnd, cb, pfnNew}
 * Info: proposed name GetWidget127ClassDesc (T1); class WIDGET127 slot6; TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00114200(void) {
    return &D_002BB4D4;
}
