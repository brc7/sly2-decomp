#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EF6F0
 * Purpose: return 0xF0: slot size for any BHV (largest subclass, BHV167 = 0xF0)
 * Info: proposed name CbBhvMax (T1); TU bhvstk
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
s32 func_001EF6F0(void) {
    return 0xF0;
}
