#include "m2c_prelude.h"
M2C_UNK func_001CB518();                            /* extern */
M2C_UNK func_001D4FA0(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x1D4F10
 * Purpose: CID 108 vt26: proxy base then state -1 -> 0 (respawn)
 * Info: proposed name RcPickup_VUnk26Proxy (T2); class CID108 slot26; TU tu_1D1370
 * Notes: names_v2 C proposed GetAloSndRepeat__FP3ALOP2LM (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4F10(s32 arg0) {
    func_001CB518();
    func_001D4FA0(arg0, -1);
    func_001D4FA0(arg0, 0);
}
