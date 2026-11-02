#include "m2c_prelude.h"
M2C_UNK func_001CB518();                            /* extern */
M2C_UNK ResetRobottankHealth(s32);                         /* extern */

/** @sly2-annot addr=0x1D1910
 * Purpose: CID 77 vt26: SO vt26 base (0x1CB518 proxy reset) then ResetRcCarHealth
 * Info: proposed name Robottank_VUnk26Proxy (T2); class ROBOTTANK slot26; TU tu_1D1370
 * Notes: names_v2 C proposed UpdateAloXfWorld__FP3ALO (0.438) | class identity from whole_game WE
 *   names/object strings: RcCar_VUnk26Proxy -> Robottank_VUnk26Proxy
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D1910(s32 arg0) {
    func_001CB518();
    ResetRobottankHealth(arg0);
}
