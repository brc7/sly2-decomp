#include "m2c_prelude.h"
M2C_UNK PopBhv(s32);                         /* extern */

/** @sly2-annot addr=0x1EEF78
 * Purpose: BHV vt9 base: pop whatever is stacked above (PopBhv(&this->pbhvNext))
 * Info: proposed name CloseBhv (T1); class BHV slot9; TU bhvstk
 * Notes: agent qualifier: (CloseBhv)
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
void CloseBhv(s32 arg0) {
    PopBhv(arg0 + 0x18);
}
