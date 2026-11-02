#include "m2c_prelude.h"
extern f32 D_002BF900;

/** @sly2-annot addr=0x19B088
 * Purpose: g + gPenalty, + D_002BF900 if candidate is the jump source
 * Info: proposed name GAdjustJtLandingScore (EXISTING); TU air
 * Notes: agent/verifier proposes GAdjustJtLandingScore for this existing name (see existing_name_changes.csv)
 *   | RENAME proposed -> GAdjustJtLandingScore (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.65] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4b_rest+arbitration
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
f32 GAdjustJtLandingScore(void *arg0, s32 arg1, f32 fparg0, f32 fparg1) {
    f32 var_f12;

    var_f12 = fparg0 + fparg1;
    if (arg1 == M2C_FIELD(arg0, s32 *, 0xBC)) {
        var_f12 += D_002BF900;
    }
    return var_f12;
}
