#include "m2c_prelude.h"
M2C_UNK func_001D4940(void *, M2C_UNK);             /* extern */
M2C_UNK func_001D4C88();                            /* extern */
M2C_UNK func_001F0948(void *, s32);                 /* extern */

/** @sly2-annot addr=0x1D14C0
 * Purpose: health x560 = max x55C, refresh health meter (0x1D4C88), reset body SMA, SetRcCarState(0)
 * Info: proposed name ResetRobottankHealth (T1); TU tu_1D1370
 * Notes: names_v2 v1 proposed UpdateStuck__FP5STUCKf (0.000) | class identity from whole_game WE names/object
 *   strings: ResetRcCarHealth -> ResetRobottankHealth
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D14C0(void *arg0) {
    void *temp_a0;

    M2C_FIELD(arg0, s32 *, 0x560) = (s32) M2C_FIELD(arg0, s32 *, 0x55C);
    func_001D4C88();
    temp_a0 = M2C_FIELD(arg0, void **, 0x570);
    if (temp_a0 != NULL) {
        func_001F0948(temp_a0, *M2C_FIELD(M2C_FIELD(temp_a0, void **, 0x24), s32 **, 0x54));
    }
    func_001D4940(arg0, 0);
}
