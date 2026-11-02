#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EEFA0
 * Purpose: BHV vt12 base: return pstep->+0x204 ? 0 : 7 -- the step's ground/collision flag mask (bit0 snap to
 *   ground, bit1 alt probe point, bit2 count in solver contacts)
 * Info: proposed name BHV_GrfGroundProbe (T2); class BHV slot12; TU bhvstk
 * Notes: agent qualifier:
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
s32 func_001EEFA0(void *arg0) {
    return (M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0x204) != 0) ? 0 : 7;
}
