#include "m2c_prelude.h"
extern f32 D_002BF660;

/** @sly2-annot addr=0x184238
 * Purpose: *pscore -= radius - (D_002BF660 - 150)
 * Info: proposed name AdjustCid35Score (T2); class CID35; TU grabspot
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00184238(void *arg0, f32 *arg1) {
    *arg1 -= M2C_FIELD(arg0, f32 *, 0x110) - (D_002BF660 - 150.0f);
}
