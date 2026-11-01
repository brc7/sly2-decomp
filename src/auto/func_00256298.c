#include "m2c_prelude.h"
/** @sly2-annot addr=0x256298
 * Purpose: return qword at +0x90 [game linkonce (inline/template)]
 * Info: proposed name CmIface_GetQw90 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s128 func_00256298(void *arg0) {
    return M2C_FIELD(arg0, s128 *, 0x90);
}
