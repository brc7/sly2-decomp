#include "m2c_prelude.h"
/** @sly2-annot addr=0x258470
 * Purpose: p->x14 = jh [game linkonce (inline/template)]
 * Info: proposed name CTextBox_SetHorizontalJust (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void CTextBox_SetHorizontalJust(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x14) = arg1;
}
