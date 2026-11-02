#include "m2c_prelude.h"
M2C_UNK SetBhvfollowState(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */

/** @sly2-annot addr=0x145080
 * Purpose: BHV194 slot 8: SetBhv194State(0)
 * Info: proposed name StartBhvfollow (T1); class BHVFOLLOW slot8; TU tu_144B28
 * Notes: superseded: OnBhv194Resume (r2b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv194Resume | v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child):
 *   StartBhv194 -> StartBhvfollow | runtime6 P2: state +0x60 = 0 after Start (S62)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   r2b_rest+bhv-slot-canon(r9)+runtime(class role)+runtime
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void StartBhvfollow(s32 arg0) {
    StartBhv();
    SetBhvfollowState(arg0, 0);
}
