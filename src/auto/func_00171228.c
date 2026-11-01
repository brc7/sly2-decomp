#include "m2c_prelude.h"
M2C_UNK InitBhv();                            /* extern */
M2C_UNK AddStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x171228
 * Purpose: BHV169 slot 7: base slot 7, then flag 0x21 on the guard (0x209890)
 * Info: proposed name InitBhv169 (T2); class BHV169 slot7; TU tu_1709A8
 * Notes: superseded: OnBhv169Push (r3b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv169Push
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00171228(void *arg0) {
    InitBhv();
    AddStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x21);
}
