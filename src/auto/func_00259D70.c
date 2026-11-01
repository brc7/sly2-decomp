#include "m2c_prelude.h"
/** @sly2-annot addr=0x259D70
 * Purpose: return p->xF64->xEC [game linkonce (inline/template)]
 * Info: proposed name GetF64_EC (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s32 func_00259D70(void *arg0) {
    return M2C_FIELD(M2C_FIELD(arg0, void **, 0xF64), s32 *, 0xEC);
}
