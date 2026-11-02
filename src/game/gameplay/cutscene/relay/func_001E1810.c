#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1810
 * Purpose: Splice builtin: append a RELAYCK 0x4D (StopMusic) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x4D,fAsync)) and fill its args
 * Info: proposed name AddRelayStopMusic (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1810(s32 arg0, s32 arg1) {
    M2C_FIELD(func_001E01F8(arg0, 0x4D, 1), s32 *, 0x10) = arg1;
}
