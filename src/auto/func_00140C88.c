#include "m2c_prelude.h"
void *func_001C4840(M2C_UNK *);                     /* extern */
M2C_UNK func_00221268(s32, M2C_UNK);                /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x140C88
 * Purpose: If PO is the current player, swap the camera TN override at +0x1024: deactivate old
 *   (SetTnTnsOverride(old,0)), store, activate new
 * Info: proposed name SetStepplayerTnOverride (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140C88(s32 arg0, s32 arg1) {
    s32 temp_a0;
    void *temp_v0;

    temp_v0 = func_001C4840(&D_003DD490);
    if (temp_v0 == arg0) {
        temp_a0 = M2C_FIELD(temp_v0, s32 *, 0x1024);
        if (temp_a0 != 0) {
            func_00221268(temp_a0, 0);
        }
        M2C_FIELD(temp_v0, s32 *, 0x1024) = arg1;
        if (arg1 != 0) {
            func_00221268(arg1, 1);
        }
    }
}
