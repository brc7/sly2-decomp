#include "m2c_prelude.h"
/** @sly2-annot addr=0x259CF8
 * Purpose: refptr store with incref [game linkonce (inline/template)]
 * Info: proposed name Pool6Ptr_Init (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00259CF8(s32 **arg0, s32 *arg1) {
    if (arg1 != NULL) {
        *arg1 += 1;
    }
    *arg0 = arg1;
}
