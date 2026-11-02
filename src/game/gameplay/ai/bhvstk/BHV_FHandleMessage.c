#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EF0B8
 * Purpose: BHV vt13 base: return 0 (message not consumed)
 * Info: proposed name BHV_FHandleMessage (T1); class BHV slot13; TU bhvstk
 * Notes: agent qualifier: (FHandleBhvMessage)
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
s32 BHV_FHandleMessage(void) {
    return 0;
}
