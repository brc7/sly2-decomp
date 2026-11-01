#include "m2c_prelude.h"
extern M2C_UNK D_002BB9FC;

/** @sly2-annot addr=0x114620
 * Purpose: vtable slot 6 of BHV193 (cid 193): returns &D_002BAAE8[193], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhv193ClassDesc (T1); class BHV193 slot6; TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetBhv193ClassDesc(void) {
    return &D_002BB9FC;
}
