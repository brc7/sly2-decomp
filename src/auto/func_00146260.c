#include "m2c_prelude.h"
M2C_UNK func_001EEF78();                            /* extern */

/** @sly2-annot addr=0x146260
 * Purpose: BHV196 slot 9
 * Info: proposed name CloseBhv196 (T2); class BHV196 slot9; TU tu_144B28
 * Notes: superseded: OnBhv196Pop (r2b_rest) | BHV vtable slot 9 role per r9 (CONFLICTS #15); was OnBhv196Pop
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00146260(void *arg0) {
    void *temp_a0;

    func_001EEF78();
    temp_a0 = M2C_FIELD(arg0, void **, 0x10);
    M2C_FIELD(temp_a0, s32 *, 0x10E0) = (s32) (M2C_FIELD(temp_a0, s32 *, 0x10E0) & ~1);
}
