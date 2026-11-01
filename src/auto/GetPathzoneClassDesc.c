#include "m2c_prelude.h"
extern M2C_UNK D_002BAC3C;

/** @sly2-annot addr=0x113B20
 * Purpose: vtable slot 6 of PATHZONE (cid 17): returns &D_002BAAE8[17], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetPathzoneClassDesc (T1); class PATHZONE slot6; TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetPathzoneClassDesc(void) {
    return &D_002BAC3C;
}
