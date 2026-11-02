#include "m2c_prelude.h"
M2C_UNK SetBootyState(void *, M2C_UNK);             /* extern */
s32 func_001F7588();                                /* extern */

/** @sly2-annot addr=0x13C368
 * Purpose: cid 0x67 vt75: if zap qualifies and dropped, reset to state 0 and return 1
 * Info: proposed name BOOTY_FTakeDamage (T2); class BOOTY slot75; TU booty
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
s32 func_0013C368(void *arg0) {
    if ((func_001F7588() == 0) || (M2C_FIELD(arg0, s32 *, 0x184) != 5)) {
        return 0;
    }
    SetBootyState(arg0, 0);
    return 1;
}
