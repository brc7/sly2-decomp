#include "m2c_prelude.h"
M2C_UNK func_0011A2E8(M2C_UNK *, s32);              /* extern */
M2C_UNK func_00156DE0(s32, M2C_UNK);                /* extern */
M2C_UNK func_001A5848();                            /* extern */
extern M2C_UNK D_003E1468;

/** @sly2-annot addr=0x156D58
 * Purpose: Base remove (0x1A5848), 0x156DE0(p,1), unlink from D_003E1468
 * Info: proposed name RemoveCid47Hierarchy (T2); class CID47 slot12; TU noise
 * Notes: Cid2F->Cid47 (CID 0x2F=47, hex-literal) | slot 12 is RemoveHierarchy (CONFLICTS#5: LO base
 *   0x1A5800/0x1A5848 AddLoHierarchy/RemoveLoHierarchy; OnAdd/OnRemove are slots 23/22 per arbitration); was
 *   OnCid47Remove
 * Confidence: low (draft) [T2 0.55] - evidence: arbitration; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_00156D58(s32 arg0) {
    func_001A5848();
    func_00156DE0(arg0, 1);
    func_0011A2E8(&D_003E1468, arg0);
}
