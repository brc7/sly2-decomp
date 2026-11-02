#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1088
 * Purpose: Splice builtin: append a RELAYCK 0x33 (Rumble) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x33,fAsync)) and fill its args
 * Info: proposed name AddRelayRumble (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayRumble(s32 arg0, s32 arg1, s32 arg2, f32 fparg0) {
    void *temp_v0;

    temp_v0 = PrelaycmdAddRelay(arg0, 0x33, arg2);
    M2C_FIELD(temp_v0, f32 *, 0x10) = fparg0;
    M2C_FIELD(temp_v0, s32 *, 0x14) = arg1;
}
