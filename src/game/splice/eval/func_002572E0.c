#include "m2c_prelude.h"
M2C_UNK __builtin_delete(s32 **);                   /* extern */
M2C_UNK func_00257078(s32 *, M2C_UNK);              /* extern */

/** @sly2-annot addr=0x2572E0
 * Purpose: release CFrame refptr (DeleteFrame when 0), flags&1 -> delete [game linkonce (inline/template)]
 * Info: proposed name FramePtr_Release (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002572E0(s32 **arg0, s32 arg1) {
    s32 *temp_a0;
    s32 temp_v0;

    temp_a0 = *arg0;
    if (temp_a0 != NULL) {
        temp_v0 = *temp_a0 - 1;
        *temp_a0 = temp_v0;
        if (temp_v0 == 0) {
            func_00257078(temp_a0, 3);
        }
    }
    if (arg1 & 1) {
        __builtin_delete(arg0);
    }
}
