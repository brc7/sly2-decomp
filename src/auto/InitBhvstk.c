#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EEC60
 * Purpose: BHVSTK init: pbhvstk->pstep = pstep
 * Info: proposed name InitBhvstk (T1); TU bhvstk
 * Notes: runtime: BHVSTK at STEP+0x26C {pstep,cpbhv,apbhv[8]} confirmed
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/guards-ai.md#9-key-functions,
 *   docs/engine/hud-ui.md#4-source-files
 * @end-sly2-annot */
void InitBhvstk(s32 *arg0, s32 arg1) {
    *arg0 = arg1;
}
