#include "m2c_prelude.h"
M2C_UNK func_00171CF8(s32, s32);                    /* extern */

/** @sly2-annot addr=0x170E68
 * Purpose: If a guard is given, StartGuardScript(guard, this)
 * Info: proposed name StartGuardCid113 (T2); class CID113; TU tu_1709A8
 * Notes: names_v2 C proposed AddJsgUnknown__FP3JSG (0.438) | Cid71->Cid113 (CID 0x71=113, agent-default(hex))
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00170E68(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        func_00171CF8(arg1, arg0);
    }
}
