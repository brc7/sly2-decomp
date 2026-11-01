#include "m2c_prelude.h"
M2C_UNK func_001EEF68();                            /* extern */

/** @sly2-annot addr=0x144E20
 * Purpose: BHV193 slot 7: base, defaults 40.0 / 1 / 1.0
 * Info: proposed name InitBhv193 (T2); class BHV193 slot7; TU tu_144B28
 * Notes: superseded: OnBhv193Push (r2b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv193Push
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00144E20(void *arg0) {
    func_001EEF68();
    M2C_FIELD(arg0, f32 *, 0x34) = 40.0f;
    M2C_FIELD(arg0, s32 *, 0x38) = 1;
    M2C_FIELD(arg0, f32 *, 0x3C) = 1.0f;
}
