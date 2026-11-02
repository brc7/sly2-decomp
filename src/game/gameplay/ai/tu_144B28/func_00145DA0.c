#include "m2c_prelude.h"
/** @sly2-annot addr=0x145DA0
 * Purpose: state == -1
 * Info: proposed name FBhv195Idle (T2); TU tu_144B28
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
s32 func_00145DA0(void *arg0) {
    return ~M2C_FIELD(arg0, s32 *, 0x30) == 0;
}
