#include "m2c_prelude.h"
M2C_UNK func_001DA770();                            /* extern */

/** @sly2-annot addr=0x1DCCA0
 * Purpose: Job list (D_003DA3C0, widget 0x4E0) post-load: base PostScreenLoad, widget +0xB0=1
 * Info: proposed name PostJobListLoad (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void PostJobListLoad(void *arg0) {
    void *temp_v1;

    func_001DA770();
    temp_v1 = M2C_FIELD(arg0, void **, 0x24);
    if (temp_v1 != NULL) {
        M2C_FIELD(temp_v1, s32 *, 0xB0) = 1;
    }
}
