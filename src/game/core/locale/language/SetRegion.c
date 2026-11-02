#include "m2c_prelude.h"
extern s32 D_002E1E58;

/** @sly2-annot addr=0x1A2C20
 * Purpose: Set region index D_002E1E58 (-1 -> 0)
 * Info: proposed name SetRegion (T1); TU language
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/hud-ui.md#language-0x1a2bd0-0x1a3020
 * @end-sly2-annot */
void SetRegion(s32 arg0) {
    if (arg0 == -1) {
        D_002E1E58 = 0;
        return;
    }
    D_002E1E58 = arg0;
}
