#include "m2c_prelude.h"
M2C_UNK GetStepplayerCpdefi(void *, s32 *);               /* extern */
M2C_UNK func_0019EA00(void *, s32 *);               /* extern */
extern f32 D_002DDED0;

/** @sly2-annot addr=0x1358A8
 * Purpose: BENTLEY vt50: GetStepplayerCpdefi plus per-state tweaks (pipe 13 -> 0x19EA00, state 5 sets flag)
 * Info: proposed name GetBentleyCpdefi (T1); class BENTLEY slot50; TU bentley
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void GetBentleyCpdefi(void *arg0, s32 *arg1) {
    s32 temp_v1;
    s32 temp_v1_2;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x13D0);
    if (temp_v1 != 0) {
        if ((temp_v1 != 9) && (M2C_FIELD(arg0, s32 *, 0x4CC) != 0)) {
            M2C_FIELD(arg0, f32 *, 0x1080) = (f32) D_002DDED0;
        }
    }
    GetStepplayerCpdefi(arg0, arg1);
    temp_v1_2 = M2C_FIELD(arg0, s32 *, 0x13D0);
    switch (temp_v1_2) {                            /* irregular */
    case 13:
        func_0019EA00(arg0 + 0xF50, arg1);
        return;
    case 5:
        *arg1 |= 1;
        return;
    }
}
