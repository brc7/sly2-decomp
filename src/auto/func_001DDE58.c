#include "m2c_prelude.h"
s32 PchzFindSwLangstr__FP2SWi(M2C_UNK *, s32);                  /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
extern void *D_003DA4D0;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1DDE58
 * Purpose: Text callback 0x51C: selected job string +0x6C
 * Info: proposed name BuildJobListJobNameText (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DDE58(s32 arg0) {
    if (D_003DA4D0 != NULL) {
        strcpy(arg0, PchzFindSwLangstr__FP2SWi(&D_003DD490, M2C_FIELD(D_003DA4D0, s32 *, 0x6C)));
    }
}
