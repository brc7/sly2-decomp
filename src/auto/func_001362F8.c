#include "m2c_prelude.h"
M2C_UNK func_00136F48(void *, M2C_UNK);             /* extern */
extern f32 D_002DDED0;

/** @sly2-annot addr=0x1362F8
 * Purpose: Set +0x16BC aim state with timestamp; state 0 stops crosshair helper (0x136F48(-1))
 * Info: proposed name SetBentleyAimState (T2); TU bentley
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_001362F8(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x16BC)) {
        M2C_FIELD(arg0, s32 *, 0x16BC) = arg1;
        M2C_FIELD(arg0, f32 *, 0x16C0) = (f32) D_002DDED0;
        if (arg1 == 0) {
            func_00136F48(arg0 + 0x1710, -1);
        }
    }
}
