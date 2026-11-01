#include "m2c_prelude.h"
M2C_UNK func_001DE5D8(s32);                         /* extern */
M2C_UNK func_001DEDB0(s32);                         /* extern */
M2C_UNK func_001DF260(s32, M2C_UNK);                /* extern */
M2C_UNK func_001F36A8();                            /* extern */

/** @sly2-annot addr=0x1DE568
 * Purpose: LASEN::PostLoad (vt15): PostSoLoad, build beam endpoints/bounds (0x1DE5D8), bind beam animations
 *   (0x1DEDB0), initial zap pass (0x1DF260)
 * Info: proposed name PostLasenLoad (T1); class LASEN slot15; TU sensor
 * Notes: superseded r6_1D5150 proposal: PostLasenLoad? (0.3)
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void func_001DE568(s32 arg0) {
    func_001F36A8();
    func_001DE5D8(arg0);
    func_001DEDB0(arg0);
    func_001DF260(arg0, 0);
}
