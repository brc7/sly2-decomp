#include "m2c_prelude.h"
/** @sly2-annot addr=0x2562A0
 * Purpose: return this+0x60 [game linkonce (inline/template)]
 * Info: proposed name CmIface_Ptr60 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s32 func_002562A0__2CM(s32 arg0) {
    return arg0 + 0x60;
}
