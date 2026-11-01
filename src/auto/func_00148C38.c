#include "m2c_prelude.h"
M2C_UNK func_00148740(s32);                         /* extern */
M2C_UNK func_001490A0(s32, M2C_UNK);                /* extern */
M2C_UNK func_001F36A8();                            /* extern */

/** @sly2-annot addr=0x148C38
 * Purpose: vt15: PostSoLoad, PostBtnLoad, SetButtonButtons(0)
 * Info: proposed name PostButtonLoad (T1); class BUTTON slot15; TU button
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00148C38(s32 arg0) {
    func_001F36A8();
    func_00148740(arg0 + 0x150);
    func_001490A0(arg0, 0);
}
