#include "m2c_prelude.h"
M2C_UNK func_00155AE0(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x1559F8
 * Purpose: BHV181 slot 9
 * Info: proposed name CloseBhv181 (T2); class BHV181 slot9; TU tu_1553F0
 * Notes: superseded: OnBhv181Pop (verdicts_followups) | BHV vtable slot 9 role per r9 (CONFLICTS #15); was
 *   OnBhv181Pop
 * Confidence: low (draft) [T2 0.80] - evidence: static analysis; sources:
 *   r3b_rest+verdicts_followups+bhv-slot-canon(r9)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
void func_001559F8(void *arg0) {
    CloseBhv();
    func_00155AE0(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x23);
}
