#include "m2c_prelude.h"
extern M2C_UNK D_002BB998;

/** @sly2-annot addr=0x1145D0
 * Purpose: vtable slot 6 of BHV188 (cid 188): returns &D_002BAAE8[188], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhv188ClassDesc (T1); class BHV188 slot6; TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_001145D0(void) {
    return &D_002BB998;
}
