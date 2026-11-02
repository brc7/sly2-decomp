#include "m2c_prelude.h"
s32 PchzFindSwLangstr__FP2SWi(M2C_UNK *, s32);                  /* extern */
s32 IdPauseTopPage(M2C_UNK *);                       /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
extern M2C_UNK D_003D9ED0;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1AD928
 * Purpose: Dynamic text provider (0x5CB)
 * Info: proposed name TextSaveA (T2); TU memcard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD928(s32 arg0) {
    strcpy(arg0, PchzFindSwLangstr__FP2SWi(&D_003DD490, IdPauseTopPage(&D_003D9ED0)));
}
