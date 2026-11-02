#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E0C78
 * Purpose: Splice builtin: append a RELAYCK 0x25 (RunOn) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x25,fAsync)) and fill its args
 * Info: proposed name AddRelayRunOn (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayRunOn(s32 arg0, s32 arg1, s32 arg2) {
    PrelaycmdAddRelay(arg0, 0x25, 1);
}
