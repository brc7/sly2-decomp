#include "m2c_prelude.h"
s32 PchzFindSwLangstr__FP2SWi(M2C_UNK *, s32);                  /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
extern s32 D_002E6590;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1AD7E0
 * Purpose: Dynamic text provider (langstr 0x2F2): strcpy name lookup
 * Info: proposed name TextMcCardName (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD7E0(s32 arg0) {
    strcpy(arg0, PchzFindSwLangstr__FP2SWi(&D_003DD490, D_002E6590));
}
