#include "m2c_prelude.h"
M2C_UNK InitBhvpath();                            /* extern */

/** @sly2-annot addr=0x144B28
 * Purpose: BHV168 slot 7: BHV166 base (0x177E80), speed scale +0xD0 = 1
 * Info: proposed name InitBhvwalkgoal (T1); class BHVWALKGOAL slot7; TU tu_144B28
 * Notes: superseded: OnBhv168Push (r2b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv168Push | v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): InitBhv168
 *   -> InitBhvwalkgoal | runtime6 P2: class role confirmed (walk-to-goal child)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r2b_rest+bhv-slot-canon(r9)+runtime(class role)+runtime
 * See: docs/engine/player.md#10-source-files, docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void InitBhvwalkgoal(void *arg0) {
    InitBhvpath();
    M2C_FIELD(arg0, f32 *, 0xD0) = 1.0f;
}
