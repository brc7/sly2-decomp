#include "m2c_prelude.h"
/** @sly2-annot addr=0x1A3070
 * Purpose: *p = 0
 * Info: proposed name ClearLetterbox (T2); TU letterbox
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/hud-ui.md#letterbox-0x1a3020-0x1a3260
 * @end-sly2-annot */
void func_001A3070(s32 *arg0) {
    *arg0 = 0;
}
