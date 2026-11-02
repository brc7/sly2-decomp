#include "m2c_prelude.h"
M2C_UNK CloseBhv();                            /* extern */

/** @sly2-annot addr=0x146758
 * Purpose: BHV197 slot 9
 * Info: proposed name CloseBhv197 (T2); class BHV197 slot9; TU tu_144B28
 * Notes: superseded: OnBhv197Pop (r2b_rest) | BHV vtable slot 9 role per r9 (CONFLICTS #15); was OnBhv197Pop
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00146758(void *arg0) {
    void *temp_a0;

    CloseBhv();
    temp_a0 = M2C_FIELD(arg0, void **, 0x10);
    M2C_FIELD(temp_a0, s32 *, 0x10E0) = (s32) (M2C_FIELD(temp_a0, s32 *, 0x10E0) & ~1);
}
