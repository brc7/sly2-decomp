#include "m2c_prelude.h"
M2C_UNK func_001DA770();                            /* extern */

/** @sly2-annot addr=0x1DD690
 * Purpose: Screen 0x52D (D_003DA4F0) post-load: base PostScreenLoad, widget +0xB0=1
 * Info: proposed name PostScreen52DLoad (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DD690(void *arg0) {
    void *temp_v1;

    func_001DA770();
    temp_v1 = M2C_FIELD(arg0, void **, 0x24);
    if (temp_v1 != NULL) {
        M2C_FIELD(temp_v1, s32 *, 0xB0) = 1;
    }
}
