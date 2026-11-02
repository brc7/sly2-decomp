#include "m2c_prelude.h"
M2C_UNK OnPoActive();                            /* extern */
M2C_UNK SetRobottankState(void *, M2C_UNK);             /* extern */

/** @sly2-annot addr=0x1D1940
 * Purpose: CID 77 vt78(f): OnPoActive, then state 2 when activated, back to 0 when deactivated from 2
 * Info: proposed name OnRobottankActive (T1); class ROBOTTANK slot78; TU tu_1D1370
 * Notes: names_v2 C proposed DupAloRo__FP3ALOP2ROT1 (0.200) | class identity from whole_game WE names/object
 *   strings: OnRcCarActive -> OnRobottankActive
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r5b_rest+verdicts_followups+whole_game(class identity)
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void OnRobottankActive(void *arg0, s32 arg1) {
    OnPoActive();
    if (arg1 != 0) {
        SetRobottankState(arg0, 2);
    } else if (M2C_FIELD(arg0, s32 *, 0x180) == 2) {
        SetRobottankState(arg0, 0);
    }
}
