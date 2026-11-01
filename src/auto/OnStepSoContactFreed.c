#include "m2c_prelude.h"
M2C_UNK func_001F5988();                            /* extern */

/** @sly2-annot addr=0x204C60
 * Purpose: STEP vt54: base no-op then drop the cached ground contact +0x208 if it is the freed one
 * Info: proposed name OnStepSoContactFreed (T1); class STEP slot54; TU step
 * Notes: superseded: OnStepXpFreed (verdicts_followups) | verifier better_name (CORRECT row): Slot 54 of the
 *   STEP-family vtables: SO base no-op, then clears +0x208 if (+0x208)->+0x48 == arg. Slot 54 is the
 *   contact-freed notify (FreeSoContacts 0x1F4E28 an
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions, docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void OnStepSoContactFreed(void *arg0, s32 arg1) {
    void *temp_v0;

    func_001F5988();
    temp_v0 = M2C_FIELD(arg0, void **, 0x208);
    if ((temp_v0 != NULL) && (M2C_FIELD(temp_v0, s32 *, 0x48) == arg1)) {
        M2C_FIELD(arg0, void **, 0x208) = NULL;
    }
}
