#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1610
 * Purpose: Splice builtin: append a RELAYCK 0x45 (Line) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x45,fAsync)) and fill its args
 * Info: proposed name AddRelayLine (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1610(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_v0;

    temp_v0 = func_001E01F8(arg0, 0x45, arg3);
    M2C_FIELD(temp_v0, s32 *, 0x10) = arg1;
    M2C_FIELD(temp_v0, s32 *, 0x14) = arg2;
}
