#include "m2c_prelude.h"
s32 IGadgetSlotFromId(s32, M2C_UNK);                    /* extern */
M2C_UNK SetStepplayerGadget(s32, s32, M2C_UNK);           /* extern */
M2C_UNK func_001DAF38(M2C_UNK, M2C_UNK);            /* extern */
extern s32 D_002E1E40;

/** @sly2-annot addr=0x14C258
 * Purpose: If D_002E1E40: find item 0x1F in it (0x141E78), remove it (0x141FC8), then 0x1DAF38(0x1F, 0)
 * Info: proposed name ClearCheatItems (T1); TU cheat
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/cheat
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548
 * @end-sly2-annot */
void ClearCheatItems(void) {
    s32 temp_v0;

    if (D_002E1E40 != 0) {
        temp_v0 = IGadgetSlotFromId(D_002E1E40, 0x1F);
        if (temp_v0 != -1) {
            SetStepplayerGadget(D_002E1E40, temp_v0, -1);
        }
        func_001DAF38(0x1F, 0);
    }
}
