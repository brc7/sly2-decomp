#include "m2c_prelude.h"
M2C_UNK SetLoParent_1A58D0(void *, M2C_UNK, M2C_UNK);    /* extern */
s32 PpoCur(M2C_UNK *);                       /* extern */
M2C_UNK func_00221268(void *, M2C_UNK);             /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1C4130
 * Purpose: Set PO camera TN (+0x16C, Lilly ptnCam): deactivate old TN, store new, tn+0x128=-1, unparent
 *   (0x1A58D0(tn,0,0)), activate it if this PO is the effective current
 * Info: proposed name SetPoTnCam (T1); TU po
 * Notes: names_v2 B proposed ChooseRobWanderLocation__FP3ROBP3ROH (0.856)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void SetPoTnCam(void *arg0, void *arg1) {
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg0, void **, 0x16C);
    if (arg1 != temp_a0) {
        if (temp_a0 != NULL) {
            func_00221268(temp_a0, 0);
        }
        M2C_FIELD(arg0, void **, 0x16C) = arg1;
        if (arg1 != NULL) {
            M2C_FIELD(arg1, s32 *, 0x128) = -1;
            SetLoParent_1A58D0(M2C_FIELD(arg0, void **, 0x16C), 0, 0);
            if (PpoCur(&D_003DD490) == arg0) {
                func_00221268(M2C_FIELD(arg0, void **, 0x16C), 1);
            }
        }
    }
}
