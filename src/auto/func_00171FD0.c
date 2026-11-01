#include "m2c_prelude.h"
M2C_UNK func_00172320(void *, M2C_UNK);             /* extern */
M2C_UNK func_001725E0(void *, M2C_UNK);             /* extern */
M2C_UNK func_001EEF78();                            /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x171FD0
 * Purpose: BHV171 slot 9: base, state -1, clear flag 0x1F, clear watched object
 * Info: proposed name CloseBhv171 (T2); class BHV171 slot9; TU tu_171F58
 * Notes: superseded: OnBhv171Pop (r3b_rest) | BHV vtable slot 9 role per r9 (CONFLICTS #15); was OnBhv171Pop
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
void func_00171FD0(void *arg0) {
    func_001EEF78();
    func_00172320(arg0, -1);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x1F);
    func_001725E0(arg0, 0);
}
