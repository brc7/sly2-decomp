#include "m2c_prelude.h"
/** @sly2-annot addr=0x2599E0
 * Purpose: return v[1] [game linkonce (inline/template)]
 * Info: proposed name VECTOR_GetY (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
f32 func_002599E0(void *arg0) {
    return M2C_FIELD(arg0, f32 *, 4);
}
