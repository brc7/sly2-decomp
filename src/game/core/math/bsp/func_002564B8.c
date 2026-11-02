#include "m2c_prelude.h"
/** @sly2-annot addr=0x2564B8
 * Purpose: return arg0 (out-of-line inline accessor/cast in the linkonce block)
 * Info: proposed name Identity_002564B8 (T2); TU game_linkonce
 * Notes: top50_round (0.15): Body only; 4 BSP/light/plane callers (PbspPointInBspQuick, InsertLightInTree,
 *   GDistPlanePos). No evidence for a better name; leave as draft
 * Confidence: low (draft) [T2 0.15] - evidence: semantic review; sources: sdk_names+top50_round
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s32 func_002564B8(s32 arg0) {
    return arg0;
}
