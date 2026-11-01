#include "m2c_prelude.h"
extern M2C_UNK D_002BAF70;

/** @sly2-annot addr=0x113DB0
 * Purpose: vtable slot 6 of CID58 (cid 58): returns &D_002BAAE8[58], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetGuardClassDesc (T1); class GUARD slot6; TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: GetCid58ClassDesc -> GetGuardClassDesc
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetGuardClassDesc(void) {
    return &D_002BAF70;
}
