#include "m2c_prelude.h"
M2C_UNK RenderSoSelf();                            /* extern */

/** @sly2-annot addr=0x203AD0
 * Purpose: STEP vt19 RenderSelf: just RenderSoSelf
 * Info: proposed name RenderStepSelf (T1); class STEP slot19; TU step
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void RenderStepSelf(void) {
    RenderSoSelf();
}
