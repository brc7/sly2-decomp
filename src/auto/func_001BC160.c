#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK AddLoHierarchy();                            /* extern */
extern M2C_UNK D_003E1A4C;

/** @sly2-annot addr=0x1BC160
 * Purpose: CID 14 vt11: AddLoHierarchy, append to pending-obstacle list D_003E1A4C
 * Info: proposed name AddCid14Hierarchy (T2); class CID14 slot11; TU tu_1BC050
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BC160(s32 arg0) {
    AddLoHierarchy();
    AppendDlEntry__FP2DLPv(&D_003E1A4C, arg0);
}
