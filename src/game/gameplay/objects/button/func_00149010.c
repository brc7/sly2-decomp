#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(void *, M2C_UNK);                 /* extern */
M2C_UNK SetButtonButtons(void *, M2C_UNK);             /* extern */
extern f32 D_002DDED0;

/** @sly2-annot addr=0x149010
 * Purpose: if a STEPPLAYER (+0x2F8) is in a particular grab state, press the button
 * Info: proposed name UpdateButtonPlayerPush (T2); TU button
 * Notes: names_v2 C proposed DisablePrsen__FP5PRSEN (0.668)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00149010(void *arg0) {
    void *temp_a0;

    if (FIsBasicDerivedFrom__FP5BASIC3CID(M2C_FIELD(arg0, void **, 0x2F8), 0x34) != 0) {
        temp_a0 = M2C_FIELD(arg0, void **, 0x2F8);
        if (M2C_FIELD(temp_a0, s32 *, 0x2DC) != 0x11) {
            if (M2C_FIELD(temp_a0, s32 *, 0x2C4) == 3) {
                if ((D_002DDED0 - M2C_FIELD(temp_a0, f32 *, 0x2E0)) < 0.15f) {
                    goto block_4;
                }
                goto block_6;
            }
            goto block_5;
        }
block_4:
        M2C_FIELD(arg0, void **, 0x2F8) = NULL;
        return;
    }
block_5:
block_6:
    SetButtonButtons(arg0, 1);
}
