#include "m2c_prelude.h"
/** @sly2-annot addr=0x2562E0
 * Purpose: return (p->x24)->f8 [game linkonce (inline/template)]
 * Info: proposed name GetSoOptF08 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
f32 func_002562E0(void *arg0) {
    return M2C_FIELD(M2C_FIELD(arg0, void **, 0x24), f32 *, 8);
}
