#include "m2c_prelude.h"
s32 func_0013E368(M2C_UNK *, s32);                  /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
extern void *D_003DA4D0;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1DDE10
 * Purpose: Text callback 0x51D: selected job string +0x70
 * Info: proposed name BuildJobListJobDescText (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DDE10(s32 arg0) {
    if (D_003DA4D0 != NULL) {
        strcpy(arg0, func_0013E368(&D_003DD490, M2C_FIELD(D_003DA4D0, s32 *, 0x70)));
    }
}
