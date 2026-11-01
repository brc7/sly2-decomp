#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EEC68
 * Purpose: BHVSTK clone fix-up: re-point pstep at the new step after the object copy
 * Info: proposed name CloneBhvstk (T1); TU bhvstk
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
void func_001EEC68(s32 *arg0, s32 arg1) {
    *arg0 = arg1;
}
