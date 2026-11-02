#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1458
 * Purpose: Splice builtin: append a RELAYCK 0x3E (TweenAimObj) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x3E,fAsync)) and fill its args
 * Info: proposed name AddRelayTweenAimObj (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayTweenAimObj(s32 arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(PrelaycmdAddRelay(arg0, 0x3E, arg2), s32 *, 0x10) = arg1;
}
