#include "m2c_prelude.h"
M2C_UNK func_0011A220(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001A5800();                            /* extern */
extern M2C_UNK D_003E142C;

/** @sly2-annot addr=0x185C28
 * Purpose: vt11: AddLoHierarchy + append to D_003E142C (one of the jump-target candidate lists)
 * Info: proposed name AddCid9Hierarchy (T2); class CID9 slot11; TU tu_185130
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
void func_00185C28(s32 arg0) {
    func_001A5800();
    func_0011A220(&D_003E142C, arg0);
}
