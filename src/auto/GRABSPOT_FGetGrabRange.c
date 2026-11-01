#include "m2c_prelude.h"
/** @sly2-annot addr=0x182CC0
 * Purpose: Default: no wrap range (return 0)
 * Info: proposed name GRABSPOT_FGetGrabRange (T1); class GRABSPOT; TU grabspot
 * Notes: superseded: GRABSPOT_FGetWrapRange (verdicts_followups) | verifier better_name (CORRECT row):
 *   Default slot 1 of the GRABSPOT secondary vtable (7 vtables). Overrides return a [min,max] parameter range
 *   and 1: CID34 +-pi*k (angular wrap), RAIL/PIPE shape s-
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4b_rest+verdicts_followups
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
s32 GRABSPOT_FGetGrabRange(void) {
    return 0;
}
