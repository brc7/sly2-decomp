#include "m2c_prelude.h"
/** @sly2-annot addr=0x13C4D8
 * Purpose: returns current entry pointer
 * Info: proposed name PentryBootyCur (T2); TU booty
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
s32 func_0013C4D8(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x158);
}
