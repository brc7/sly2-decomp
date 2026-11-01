#include "m2c_prelude.h"
/** @sly2-annot addr=0x1800A0
 * Purpose: Append (int, OID) to a 4-entry table at +0x234 (stride 0x18, count +0x230)
 * Info: proposed name AddHackshipOidPair (EXISTING); class CID72; TU tu_17F090
 * Notes: Cid48->Cid72 (CID 0x48=72, evidence) | agent/verifier proposes AddCid72Entry for this existing name
 *   (see existing_name_changes.csv) | RENAME proposed -> AddHackshipOidPair (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.50] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4b_rest+arbitration
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void UseStepguardDeathAnimation__FP9STEPGUARDi3OID(void *arg0, s32 arg1, s32 arg2) {
    u32 temp_a3;
    void *temp_v0;

    temp_a3 = M2C_FIELD(arg0, u32 *, 0x230);
    if (temp_a3 < 4U) {
        M2C_FIELD(arg0, u32 *, 0x230) = (u32) (temp_a3 + 1);
        temp_v0 = arg0 + ((temp_a3 * 0x18) + 0x234);
        M2C_FIELD(temp_v0, s32 *, 0) = arg1;
        M2C_FIELD(temp_v0, s32 *, 4) = arg2;
    }
}
