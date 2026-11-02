#include "m2c_prelude.h"
extern f32 D_002DDF0C;

/** @sly2-annot addr=0x1DAA78
 * Purpose: Gadget menu: state -1, timestamp
 * Info: proposed name ClearGadgetMenu (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DAA78(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x40) = -1;
    M2C_FIELD(arg0, f32 *, 0x44) = (f32) D_002DDF0C;
}
