#include "m2c_prelude.h"
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, s32, M2C_UNK); /* extern */
M2C_UNK SetWipeWipes(void *, M2C_UNK);             /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x22B420
 * Purpose: sets wipes 0 or 2 by world lookup
 * Info: TU wipe
 * Notes: descriptive only: wipe from level transition | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/world-sw.md#2-loading-a-level
 * @end-sly2-annot */
void func_0022B420(void *arg0) {
    s32 temp_v0;

    temp_v0 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, M2C_FIELD(arg0, s32 *, 0x58), 0);
    M2C_FIELD(arg0, s32 *, 0x54) = temp_v0;
    if ((temp_v0 == 0) || (M2C_FIELD(arg0, s32 *, 0) != 3)) {
        SetWipeWipes(arg0, 0);
        return;
    }
    SetWipeWipes(arg0, 2);
}
