#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E0A20
 * Purpose: Splice builtin: append a RELAYCK 0x1F (StepCmd1F) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x1F,fAsync)) and fill its args
 * Info: proposed name AddRelayStepCmd1F (T1); TU relay
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayStepCmd1F(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_v0;

    temp_v0 = PrelaycmdAddRelay(arg0, 0x1F, 0);
    M2C_FIELD(temp_v0, s32 *, 0x10) = arg1;
    M2C_FIELD(temp_v0, s32 *, 0x14) = arg2;
}
