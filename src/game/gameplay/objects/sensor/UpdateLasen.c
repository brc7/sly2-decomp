#include "m2c_prelude.h"
M2C_UNK func_001DF3D0(s32);                         /* extern */
M2C_UNK UpdateSo();                            /* extern */

/** @sly2-annot addr=0x1DF3A0
 * Purpose: LASEN::Update (vt16): UpdateSo then move path endpoints
 * Info: proposed name UpdateLasen (T1); class LASEN slot16; TU sensor
 * Notes: superseded r6_1D5150 proposal: UpdateLasen? (0.3)
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void UpdateLasen(s32 arg0) {
    UpdateSo();
    func_001DF3D0(arg0);
}
