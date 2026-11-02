#include "m2c_prelude.h"
extern M2C_UNK D_002BB060;

/** @sly2-annot addr=0x113E70
 * Purpose: vtable slot 6 of CID70 (cid 70): returns &D_002BAAE8[70], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetSafehouseClassDesc (T1); class SAFEHOUSE slot6; TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: GetCid70ClassDesc -> GetSafehouseClassDesc
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *GetSafehouseClassDesc(void) {
    return &D_002BB060;
}
