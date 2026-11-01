#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK AddLoHierarchy();                            /* extern */
extern M2C_UNK D_003E1468;

/** @sly2-annot addr=0x156D20
 * Purpose: Base add (0x1A5800) then appends this to the global class list D_003E1468
 * Info: proposed name AddCid47Hierarchy (T2); class CID47 slot11; TU noise
 * Notes: Cid2F->Cid47 (CID 0x2F=47, hex-literal) | slot 11 is AddHierarchy (CONFLICTS#5: LO base
 *   0x1A5800/0x1A5848 AddLoHierarchy/RemoveLoHierarchy; OnAdd/OnRemove are slots 23/22 per arbitration); was
 *   OnCid47Add
 * Confidence: low (draft) [T2 0.55] - evidence: arbitration; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_00156D20(s32 arg0) {
    AddLoHierarchy();
    AppendDlEntry__FP2DLPv(&D_003E1468, arg0);
}
