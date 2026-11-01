#include "m2c_prelude.h"
M2C_UNK StaticInit_frame(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x128118
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$frame (EXISTING); TU frame
 * Notes: RENAME proposed -> _GLOBAL_$I$frame (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.85] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   tu_map(structural)+arbitration
 * See: docs/engine/splice-scripting.md#3-the-evaluator
 * @end-sly2-annot */
void _GLOBAL_$I$g_gc(void) {
    StaticInit_frame(1, 0xFFFF);
}
