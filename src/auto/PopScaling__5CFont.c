#include "m2c_prelude.h"
/** @sly2-annot addr=0x15F0C0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void PopScaling__5CFont(void *arg0) {
    s32 temp_v0;
    void *temp_v1;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x18) - 1;
    M2C_FIELD(arg0, s32 *, 0x18) = temp_v0;
    temp_v1 = arg0 + ((temp_v0 * 8) + 0x1C);
    M2C_FIELD(arg0, f32 *, 0x10) = (f32) M2C_FIELD(temp_v1, f32 *, 0);
    M2C_FIELD(arg0, f32 *, 0x14) = (f32) M2C_FIELD(temp_v1, f32 *, 4);
}
