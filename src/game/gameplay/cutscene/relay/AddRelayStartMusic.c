#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E17E0
 * Purpose: Splice builtin: append a RELAYCK 0x4C (StartMusic) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x4C,fAsync)) and fill its args
 * Info: proposed name AddRelayStartMusic (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayStartMusic(s32 arg0, s32 arg1) {
    M2C_FIELD(PrelaycmdAddRelay(arg0, 0x4C, 1), s32 *, 0x10) = arg1;
}
