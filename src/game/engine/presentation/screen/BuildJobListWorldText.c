#include "m2c_prelude.h"
s32 PchzFindSwLangstr__FP2SWi(M2C_UNK *, s32);                  /* extern */
s32 IdsFromWorld(s32);                             /* extern */
s32 NWorldCur();                                /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1DDDB8
 * Purpose: Text callback 0x51B: current episode name
 * Info: proposed name BuildJobListWorldText (T1); TU screen
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildJobListWorldText(s32 arg0) {
    strcpy(arg0, PchzFindSwLangstr__FP2SWi(&D_003DD490, IdsFromWorld(NWorldCur())));
}
