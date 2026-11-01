#include "m2c_prelude.h"
M2C_UNK func_00172320(s32, M2C_UNK);                /* extern */
M2C_UNK func_001EEF70();                            /* extern */

/** @sly2-annot addr=0x171FA0
 * Purpose: BHV171 slot 8: base, SetBhv171State(0)
 * Info: proposed name StartBhv171 (T2); class BHV171 slot8; TU tu_171F58
 * Notes: superseded: OnBhv171Resume (r3b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv171Resume
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
void func_00171FA0(s32 arg0) {
    func_001EEF70();
    func_00172320(arg0, 0);
}
