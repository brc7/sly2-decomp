#include "m2c_prelude.h"
M2C_UNK func_00155788(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK SetStepguardTarget(s32, M2C_UNK);                /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x155598
 * Purpose: BHV180 slot 9: state -1, clear flag 0x22, 0x208540(guard,0)
 * Info: proposed name CloseBhv180 (T2); class BHV180 slot9; TU tu_1553F0
 * Notes: superseded: OnBhv180Pop (r3b_rest) | BHV vtable slot 9 role per r9 (CONFLICTS #15); was OnBhv180Pop
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
void func_00155598(void *arg0) {
    CloseBhv();
    func_00155788(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x22);
    SetStepguardTarget(M2C_FIELD(arg0, s32 *, 0x10), 0);
}
