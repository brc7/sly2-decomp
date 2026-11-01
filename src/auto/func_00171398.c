#include "m2c_prelude.h"
M2C_UNK func_00171258(void *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_001EEF78();                            /* extern */
M2C_UNK func_001EF038(void *);                      /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x171398
 * Purpose: BHV169 slot 9: base pop, ClearBhvBusy, clear the script, drop flag 0x21
 * Info: proposed name CloseBhv169 (T2); class BHV169 slot9; TU tu_1709A8
 * Notes: superseded: OnBhv169Pop (r3b_rest) | BHV vtable slot 9 role per r9 (CONFLICTS #15); was OnBhv169Pop
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00171398(void *arg0) {
    func_001EEF78();
    func_001EF038(arg0);
    func_00171258(arg0, 0, 0, 0);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x21);
}
