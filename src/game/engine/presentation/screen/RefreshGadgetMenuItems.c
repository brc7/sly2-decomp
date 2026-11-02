#include "m2c_prelude.h"
s32 func_001DAE58(s32);                             /* extern */
s32 FPowerupAcquired(s32);                             /* extern */

/** @sly2-annot addr=0x1DB010
 * Purpose: Per item: flag 2 = unlocked by episode (FScreenItemUnlocked), flag 4 = acquired
 * Info: proposed name RefreshGadgetMenuItems (T1); TU screen
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void RefreshGadgetMenuItems(void *arg0) {
    s32 temp_v0;
    s32 var_s1;
    void *temp_s0;

    var_s1 = 0;
    if (M2C_FIELD(arg0, s32 *, 0x4C) > 0) {
        do {
            temp_v0 = var_s1 << 5;
            var_s1 += 1;
            temp_s0 = M2C_FIELD(arg0, s32 *, 0x50) + temp_v0;
            M2C_FIELD(temp_s0, s32 *, 0x1C) = (s32) ((M2C_FIELD(temp_s0, s32 *, 0x1C) & ~2) | ((func_001DAE58(M2C_FIELD(temp_s0, s32 *, 0)) & 1) * 2));
            M2C_FIELD(temp_s0, s32 *, 0x1C) = (s32) ((M2C_FIELD(temp_s0, s32 *, 0x1C) & ~4) | ((FPowerupAcquired(M2C_FIELD(temp_s0, s32 *, 0)) & 1) * 4));
        } while (var_s1 < M2C_FIELD(arg0, s32 *, 0x4C));
    }
}
