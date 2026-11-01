#include "m2c_prelude.h"
M2C_UNK func_0017D450(void *, M2C_UNK);             /* extern */
M2C_UNK func_001EEF78();                            /* extern */
M2C_UNK func_00209970(s32, s32);                    /* extern */

/** @sly2-annot addr=0x17D408
 * Purpose: BHVGOTO Close: state -1, lower its flag
 * Info: proposed name CloseBhvgoto (T1); class BHVGOTO slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGOTO role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017D408(void *arg0) {
    s32 temp_a1;

    func_001EEF78();
    func_0017D450(arg0, -1);
    temp_a1 = M2C_FIELD(arg0, s32 *, 0x38);
    if (temp_a1 != -1) {
        func_00209970(M2C_FIELD(arg0, s32 *, 0x10), temp_a1);
    }
}
