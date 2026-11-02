#include "m2c_prelude.h"
extern M2C_UNK D_002BB8F8;

/** @sly2-annot addr=0x114550
 * Purpose: vtable slot 6 of BHV180 (cid 180): returns &D_002BAAE8[180], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBhv180ClassDesc (T1); class BHV180 slot6; TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetBhv180ClassDesc(void) {
    return &D_002BB8F8;
}
