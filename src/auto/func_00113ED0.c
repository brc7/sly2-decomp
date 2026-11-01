#include "m2c_prelude.h"
extern M2C_UNK D_002BB0D8;

/** @sly2-annot addr=0x113ED0
 * Purpose: vtable slot 6 of CID76 (cid 76): returns &D_002BAAE8[76], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetHackwallClassDesc (T1); class HACKWALL slot6; TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: GetCid76ClassDesc -> GetHackwallClassDesc
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
M2C_UNK *func_00113ED0(void) {
    return &D_002BB0D8;
}
