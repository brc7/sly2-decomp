#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EEED0
 * Purpose: --pbhvstk->cpbhv
 * Info: proposed name DecrementBhvstk (T1); TU bhvstk
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
void DecrementBhvstk(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = (s32) (M2C_FIELD(arg0, s32 *, 4) - 1);
}
