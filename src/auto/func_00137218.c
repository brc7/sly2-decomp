#include "m2c_prelude.h"
M2C_UNK func_002046F0(void *, s32);                 /* extern */

/** @sly2-annot addr=0x137218
 * Purpose: 0x2046F0(owner, owner->+0x10DC)
 * Info: proposed name JtHelperApplyInteract (T2); TU bentley
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00137218(void **arg0) {
    s32 temp_a1;
    void *temp_a0;

    temp_a0 = *arg0;
    temp_a1 = M2C_FIELD(temp_a0, s32 *, 0x10DC);
    if (temp_a1 != 0) {
        func_002046F0(temp_a0, temp_a1);
    }
}
