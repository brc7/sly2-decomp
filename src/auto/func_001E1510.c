#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1510
 * Purpose: Splice builtin: append a RELAYCK 0x40 (TweenTime) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x40,fAsync)) and fill its args
 * Info: proposed name AddRelayTweenTime (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1510(s32 arg0, s32 arg1, f32 fparg0) {
    M2C_FIELD(func_001E01F8(arg0, 0x40, arg1), f32 *, 0x10) = fparg0;
}
