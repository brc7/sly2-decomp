#include "m2c_prelude.h"
/** @sly2-annot addr=0x1DA5B0
 * Purpose: Text id being shown or queued
 * Info: proposed name IdsCurNotice (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
s32 func_001DA5B0(void *arg0) {
    s32 var_v1;

    var_v1 = M2C_FIELD(arg0, s32 *, 0x20);
    if (var_v1 == -1) {
        var_v1 = M2C_FIELD(arg0, s32 *, 0x18);
    }
    return var_v1;
}
