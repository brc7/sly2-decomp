#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK RemoveLoHierarchy();                            /* extern */
extern M2C_UNK D_003E142C;

/** @sly2-annot addr=0x185C60
 * Purpose: vt12: RemoveLoHierarchy + remove from D_003E142C
 * Info: proposed name RemoveCid9Hierarchy (T2); class CID9 slot12; TU tu_185130
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
void func_00185C60(s32 arg0) {
    RemoveLoHierarchy();
    RemoveDlEntry__FP2DLPv(&D_003E142C, arg0);
}
