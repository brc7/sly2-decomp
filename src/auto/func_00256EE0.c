#include "m2c_prelude.h"
M2C_UNK func_00257078(s32 *, M2C_UNK);              /* extern */

/** @sly2-annot addr=0x256EE0
 * Purpose: Intrusive ref-pointer assignment for CFrame: incref new, decref old (DeleteFrame(p,3) at 0), store
 * Info: proposed name FramePtr_Assign (T1); TU game_linkonce
 * Notes: top50_round (0.60): Body: ++*new; --*old==0 -> 0x257078(old,3); *pp=new. 0x257078 is the CFrame
 *   destructor (frees from pool D_004520F8). Callers CFrame_SetSingleParent/AddParent/RemoveParent (frame TU,
 *   T1) store parent frames; ApplyAseg/New
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: sdk_names+top50_round
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138,
 *   docs/reference/functions/splice-scripting.md#splicemap-0x1ff7e8-0x1ffcf8
 * @end-sly2-annot */
void func_00256EE0(s32 **arg0, s32 *arg1) {
    s32 *temp_a0;
    s32 temp_v0;

    if (arg1 != NULL) {
        *arg1 += 1;
    }
    temp_a0 = *arg0;
    if (temp_a0 != NULL) {
        temp_v0 = *temp_a0 - 1;
        *temp_a0 = temp_v0;
        if (temp_v0 == 0) {
            func_00257078(temp_a0, 3);
        }
    }
    *arg0 = arg1;
}
