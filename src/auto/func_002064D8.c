#include "m2c_prelude.h"
M2C_UNK func_00130658(s32);                         /* extern */
M2C_UNK func_001EECD0(s32);                         /* extern */
M2C_UNK func_001F2D48();                            /* extern */
M2C_UNK func_00208540(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x2064D8
 * Purpose: STEPGUARD vt12: base, clear target (0x208540(0)), clear BHV stack, retract held ASEGA +0x34C
 * Info: proposed name RemoveStepguardHierarchy (T1); class STEPGUARD slot12; TU stepguard
 * Notes: slot 12 is RemoveHierarchy (CONFLICTS#5: LO base 0x1A5800/0x1A5848 AddLoHierarchy/RemoveLoHierarchy;
 *   OnAdd/OnRemove are slots 23/22 per arbitration); was OnStepguardRemove
 * Confidence: mid (draft) [T1 0.60] - evidence: arbitration; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002064D8(s32 arg0) {
    func_001F2D48();
    func_00208540(arg0, 0);
    func_001EECD0(arg0 + 0x26C);
    func_00130658(arg0 + 0x34C);
}
