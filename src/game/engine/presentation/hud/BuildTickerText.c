#include "m2c_prelude.h"
s32 PchzFindSwLangstr__FP2SWi(M2C_UNK *, s32);                  /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
extern s32 D_003DA0D8;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x2113E0
 * Purpose: STRINGGEN id 0x2F1: langstr(D_003DA0D8) = message of the scrolling ticker D_003DA0C0
 * Info: proposed name BuildTickerText (T1); TU hud
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void BuildTickerText(s32 arg0) {
    strcpy(arg0, PchzFindSwLangstr__FP2SWi(&D_003DD490, D_003DA0D8));
}
