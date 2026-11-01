#include "m2c_prelude.h"
/** @sly2-annot addr=0x256318
 * Purpose: return s16@0x14 [game linkonce (inline/template)]
 * Info: proposed name GetSoOptS14 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s16 func_00256318(void *arg0) {
    return M2C_FIELD(M2C_FIELD(arg0, void **, 0x24), s16 *, 0x14);
}
