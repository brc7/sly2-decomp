#include "m2c_prelude.h"
/** @sly2-annot addr=0x258280
 * Purpose: return *(p->xE00) [game linkonce (inline/template)]
 * Info: proposed name GetPtrE00Value (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s32 func_00258280__10STEPPLAYER(void *arg0) {
    return *M2C_FIELD(arg0, s32 **, 0xE00);
}
