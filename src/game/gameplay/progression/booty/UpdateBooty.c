#include "m2c_prelude.h"
M2C_UNK func_0013C4E0(void *, M2C_UNK);             /* extern */
M2C_UNK func_00143380(s32);                         /* extern */
M2C_UNK UpdateSo();                            /* extern */
extern f32 D_002DDED0;
extern f32 D_003E1BFC;

/** @sly2-annot addr=0x13BF50
 * Purpose: cid 0x67 vt16: UpdateSo; state 5 (dropped) returns to its source after 5 s; state 4 (carried)
 *   forces a drop (DropStepplayerBooty) once the level timer D_003E1BFC passes
 * Info: proposed name UpdateBooty (T1); class BOOTY slot16; TU booty
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void UpdateBooty(void *arg0) {
    s32 temp_v1;

    UpdateSo();
    temp_v1 = M2C_FIELD(arg0, s32 *, 0x184);
    switch (temp_v1) {                              /* irregular */
    case 5:
        if ((D_002DDED0 - M2C_FIELD(arg0, f32 *, 0x188)) >= 5.0f) {
            func_0013C4E0(arg0, 0);
            return;
        }
        return;
    case 4:
        if ((M2C_FIELD(M2C_FIELD(arg0, void **, 0x158), s32 *, 0xC) != 0) && (D_002DDED0 >= D_003E1BFC)) {
            func_00143380(M2C_FIELD(arg0, s32 *, 0x160));
        }
        break;
    }
}
