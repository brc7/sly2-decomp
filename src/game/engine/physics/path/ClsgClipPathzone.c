#include "m2c_prelude.h"
M2C_UNK ClsgClipEdgeToCbsp(s32);                         /* extern */

/** @sly2-annot addr=0x1C0960
 * Purpose: ClsgClipEdgeToCbsp on pathzone+0xE0
 * Info: proposed name ClsgClipPathzone (T1); TU path
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void ClsgClipPathzone(s32 arg0) {
    ClsgClipEdgeToCbsp(arg0 + 0xE0);
}
