#include "m2c_prelude.h"
extern f32 D_002DDF0C;

/** @sly2-annot addr=0x1DD7B8
 * Purpose: slot 14 of 0x2D83E8: if state(+0x28) changes, store it and the time
 * Info: proposed name SetScreenState (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DD7B8(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x28)) {
        M2C_FIELD(arg0, s32 *, 0x28) = arg1;
        M2C_FIELD(arg0, f32 *, 0x2C) = (f32) D_002DDF0C;
    }
}
