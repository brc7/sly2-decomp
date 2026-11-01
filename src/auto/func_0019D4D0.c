#include "m2c_prelude.h"
s32 func_00132B10(s32, M2C_UNK);                    /* extern */
M2C_UNK func_00201D78(s32, s32 *);                  /* extern */

/** @sly2-annot addr=0x19D4D0
 * Purpose: Init mode HUD sub-record: owner, state -1; for JT (CID 0x35) bind the widget at JT+0x1D40
 *   (0x201D78)
 * Info: proposed name InitJtModeHud (T1); TU mode
 * Notes: runtime3: JT+0x1D40 widget, oids 0x35F/0x368
 * Confidence: mid (draft) [T1 0.50] - evidence: runtime (save-state RAM); sources: r4b_rest+runtime
 * See: docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
void func_0019D4D0(void *arg0, s32 *arg1) {
    s32 temp_a0;

    M2C_FIELD(arg0, s32 **, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = -1;
    if (func_00132B10(*arg1, 0x35) != 0) {
        temp_a0 = *M2C_FIELD(arg0, s32 **, 0) + 0x1D40;
        M2C_FIELD(arg0, s32 *, 0x14) = temp_a0;
        func_00201D78(temp_a0, arg1);
        return;
    }
    M2C_FIELD(arg0, s32 *, 0x14) = 0;
}
