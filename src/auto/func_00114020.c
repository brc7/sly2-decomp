#include "m2c_prelude.h"
extern M2C_UNK D_002BB27C;

/** @sly2-annot addr=0x114020
 * Purpose: vtable slot 6 of CID97 (cid 97): returns &D_002BAAE8[97], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetSkyClassDesc (T1); class SKY slot6; TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: GetCid97ClassDesc -> GetSkyClassDesc
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00114020(void) {
    return &D_002BB27C;
}
