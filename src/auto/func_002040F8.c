#include "m2c_prelude.h"
/** @sly2-annot addr=0x2040F8
 * Purpose: STEP vt85: ground-probe flags for AddStepCustomXps: 0 if +0x204 else 7 (1 probe, 2 use top point,
 *   4 ...)
 * Info: proposed name GrfStepGroundProbe (T2); class STEP slot85; TU step
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
s32 func_002040F8(void *arg0) {
    return (M2C_FIELD(arg0, s32 *, 0x204) != 0) ? 0 : 7;
}
