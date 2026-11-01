#include "m2c_prelude.h"
s32 func_001711F0(s32, s32);                        /* extern */
M2C_UNK func_001EF0D8(void *);                      /* extern */

/** @sly2-annot addr=0x171C98
 * Purpose: Pops any running sub-behaviour and jumps the script to a label
 * Info: proposed name JumpBhv169ToLabel (T2); TU tu_1709A8
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00171C98(void *arg0, s32 arg1) {
    if (M2C_FIELD(arg0, s32 *, 0x30) != 0) {
        if (M2C_FIELD(arg0, s32 *, 0x40) != 0) {
            func_001EF0D8(arg0 + 0x18);
            M2C_FIELD(arg0, s32 *, 0x40) = 0;
        }
        M2C_FIELD(arg0, s32 *, 0x38) = func_001711F0(M2C_FIELD(arg0, s32 *, 0x30), arg1);
    }
}
