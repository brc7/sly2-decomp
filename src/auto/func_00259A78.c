#include "m2c_prelude.h"
/** @sly2-annot addr=0x259A78
 * Purpose: option-record base accessor: return this+0x100 [game linkonce (inline/template)]
 * Info: proposed name PvOptBase_Plus100_00259A78 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s32 func_00259A78(s32 arg0) {
    return arg0 + 0x100;
}
