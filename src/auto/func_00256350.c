#include "m2c_prelude.h"
/** @sly2-annot addr=0x256350
 * Purpose: return int@0x18 [game linkonce (inline/template)]
 * Info: proposed name GetSoOptI18 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s32 func_00256350(void *arg0) {
    return M2C_FIELD(M2C_FIELD(arg0, void **, 0x24), s32 *, 0x18);
}
