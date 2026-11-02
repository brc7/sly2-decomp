#include "m2c_prelude.h"
/** @sly2-annot addr=0x1BDBC8
 * Purpose: qsort comparator on first float (used to sort hull points by x)
 * Info: proposed name NCmpPosX (EXISTING); TU tu_1BC050
 * Notes: agent/verifier proposes NCmpGX for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> NCmpPosX (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.55] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r5b_rest+arbitration
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
s32 NCmpPosX(f32 *arg0, f32 *arg1) {
    s32 var_v0;

    var_v0 = -1;
    if (*arg0 > *arg1) {
        var_v0 = 1;
    }
    return var_v0;
}
