#include "m2c_prelude.h"
M2C_UNK func_001BF900(s32);                         /* extern */

/** @sly2-annot addr=0x1C0980
 * Purpose: FClipEdgeToCbsp on pathzone+0xE0
 * Info: proposed name FClipPathzone (T1); TU path
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C0980(s32 arg0) {
    func_001BF900(arg0 + 0xE0);
}
