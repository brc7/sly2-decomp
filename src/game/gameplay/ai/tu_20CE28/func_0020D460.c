#include "m2c_prelude.h"
/** @sly2-annot addr=0x20D460
 * Purpose: +0xC = hazard object
 * Info: proposed name SetHazardWatchTarget (T2); TU tu_20CE28
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
void func_0020D460(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0xC) = arg1;
}
