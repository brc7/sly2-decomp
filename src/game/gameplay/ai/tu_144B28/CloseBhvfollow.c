#include "m2c_prelude.h"
M2C_UNK SetBhvfollowState(s32, M2C_UNK);                /* extern */
M2C_UNK CloseBhv();                            /* extern */

/** @sly2-annot addr=0x1450B0
 * Purpose: BHV194 slot 9: SetBhv194State(-1)
 * Info: proposed name CloseBhvfollow (T1); class BHVFOLLOW slot9; TU tu_144B28
 * Notes: superseded: OnBhv194Pop (r2b_rest) | BHV vtable slot 9 role per r9 (CONFLICTS #15); was OnBhv194Pop
 *   | v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): CloseBhv194 ->
 *   CloseBhvfollow | runtime6 P2: class role confirmed (follow)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r2b_rest+bhv-slot-canon(r9)+runtime(class role)+runtime
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void CloseBhvfollow(s32 arg0) {
    CloseBhv();
    SetBhvfollowState(arg0, -1);
}
