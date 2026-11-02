#include "m2c_prelude.h"
s32 NCompletionPercentGs(M2C_UNK *, M2C_UNK *);            /* extern */
M2C_UNK sprintf(s32, M2C_UNK *, s32);               /* extern */
extern M2C_UNK D_002A7AB8;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003E0EA8;

/** @sly2-annot addr=0x1DDFB0
 * Purpose: Text callback 0x5D5: "%d%%" game completion
 * Info: proposed name BuildTitleCompletionText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildTitleCompletionText(s32 arg0) {
    sprintf(arg0, &D_002A7AB8, NCompletionPercentGs(&D_003E0EA8, &D_003D4A60));
}
