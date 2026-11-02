#include "m2c_prelude.h"
M2C_UNK PostLoLoad();                            /* extern */
M2C_UNK SetTnTns(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x185CE0
 * Purpose: vt15: PostLoLoad + SetTnTns
 * Info: proposed name PostCid9Load (T2); class CID9 slot15; TU tu_185130
 * Notes: names_v2 C proposed OnStepguardAdd__FP9STEPGUARD (0.200)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
void func_00185CE0(void *arg0) {
    s32 temp_a0;

    PostLoLoad();
    temp_a0 = M2C_FIELD(arg0, s32 *, 0x110);
    if (temp_a0 != 0) {
        SetTnTns(temp_a0, 0);
    }
}
