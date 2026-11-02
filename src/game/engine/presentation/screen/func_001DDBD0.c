#include "m2c_prelude.h"
extern f32 D_002DDF0C;

/** @sly2-annot addr=0x1DDBD0
 * Purpose: Title screen slot 14: store direction and time
 * Info: proposed name SetTitleScreenStickDir (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DDBD0(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x28)) {
        M2C_FIELD(arg0, s32 *, 0x28) = arg1;
        M2C_FIELD(arg0, f32 *, 0x2C) = (f32) D_002DDF0C;
    }
}
