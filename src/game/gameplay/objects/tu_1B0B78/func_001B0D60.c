#include "m2c_prelude.h"
M2C_UNK func_001B0D90(s32);                         /* extern */
M2C_UNK func_001CB518();                            /* extern */

/** @sly2-annot addr=0x1B0D60
 * Purpose: CID 96 vt26: proxy base then reset
 * Info: proposed name Cid96_VUnk26Proxy (T2); class CID96 slot26; TU tu_1B0B78
 * Notes: names_v2 v1 proposed UpdateStepMatTarget__FP4STEP (0.004)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B0D60(s32 arg0) {
    func_001CB518();
    func_001B0D90(arg0);
}
