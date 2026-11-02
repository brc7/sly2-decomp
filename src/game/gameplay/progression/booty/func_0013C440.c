#include "m2c_prelude.h"
/** @sly2-annot addr=0x13C440
 * Purpose: current entry (+0x158) = 0
 * Info: proposed name ClearBootyCurEntry (T2); TU booty
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013C440(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x158) = 0;
}
