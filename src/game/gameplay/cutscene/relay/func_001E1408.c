#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1408
 * Purpose: Splice builtin: append a RELAYCK 0x3C (EndJtMode) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x3C,fAsync)) and fill its args
 * Info: proposed name AddRelayEndJtMode (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1408(s32 arg0, s32 arg1) {
    func_001E01F8(arg0, 0x3C, arg1);
}
