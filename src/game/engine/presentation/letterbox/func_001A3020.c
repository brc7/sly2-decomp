#include "m2c_prelude.h"
M2C_UNK SetWidgetRequest(s32, M2C_UNK); /* extern */
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1A3020
 * Purpose: Find object oid 0x373 (kind 0x405) into *p and SetTimerTimers(1)
 * Info: proposed name InitLetterboxTimer (T2); TU letterbox
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/hud-ui.md#3-fonts-language-letterbox-loading-screen, docs/engine/hud-ui.md#4-source-files,
 *   docs/reference/functions/hud-ui.md#letterbox-0x1a3020-0x1a3260
 * @end-sly2-annot */
void func_001A3020(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x373, 0);
    *arg0 = temp_v0;
    if (temp_v0 != 0) {
        SetWidgetRequest(temp_v0, 1);
    }
}
