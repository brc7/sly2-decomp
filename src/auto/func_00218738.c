#include "m2c_prelude.h"
/** @sly2-annot addr=0x218738
 * Purpose: returns pt(+8)->x10
 * Info: proposed name GetSpriteMainState (T2); TU hud
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
s32 func_00218738(void *arg0) {
    return M2C_FIELD(M2C_FIELD(arg0, void **, 8), s32 *, 0x10);
}
