#include "m2c_prelude.h"
M2C_UNK SetPhase__F5PHASE(M2C_UNK);                     /* extern */

/** @sly2-annot addr=0x1C2BA8
 * Purpose: SetPhase(PHASE_None=0)
 * Info: proposed name ClearPhase (T1); TU phasemem
 * Notes: names_v2 C proposed ClearCmCut__FP2CM (0.638)
 * Confidence: high [T1 0.80] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue,
 *   docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#phasemem-0x1c2a78-0x1c2d20
 * @end-sly2-annot */
void ClearPhase(void) {
    SetPhase__F5PHASE(0);
}
