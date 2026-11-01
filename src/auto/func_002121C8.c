#include "m2c_prelude.h"
/** @sly2-annot addr=0x2121C8
 * Purpose: state(+0x58)<2 && request(+0x64)<2
 * Info: proposed name FWidgetIdle (T2); TU hud
 * Notes: names_v2 v1 proposed SetVolbtnRsmg__FP6VOLBTNi3OIDN22 (0.000)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
s32 func_002121C8(void *arg0) {
    s32 var_v1;

    var_v1 = 0;
    if (M2C_FIELD(arg0, s32 *, 0x58) < 2) {
        var_v1 = M2C_FIELD(arg0, s32 *, 0x64) < 2;
    }
    return var_v1;
}
