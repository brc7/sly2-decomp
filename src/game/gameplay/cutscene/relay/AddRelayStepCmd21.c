#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E0AA0
 * Purpose: Splice builtin: append a RELAYCK 0x21 (StepCmd21) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x21,fAsync)) and fill its args
 * Info: proposed name AddRelayStepCmd21 (T1); TU relay
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayStepCmd21(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_v0;

    temp_v0 = PrelaycmdAddRelay(arg0, 0x21, 0);
    M2C_FIELD(temp_v0, s32 *, 0x10) = arg1;
    M2C_FIELD(temp_v0, s32 *, 0x14) = arg2;
}
