#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E0ED8
 * Purpose: Splice builtin: append a RELAYCK 0x2C (AddObject) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x2C,fAsync)) and fill its args
 * Info: proposed name AddRelayAddObject (T1); TU relay
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6b_rest+verdicts_followups
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayAddObject(s32 arg0, s32 arg1, s32 arg2, f32 fparg0) {
    void *temp_v0;

    temp_v0 = PrelaycmdAddRelay(arg0, 0x2C, arg2);
    M2C_FIELD(temp_v0, s32 *, 0x10) = arg1;
    M2C_FIELD(temp_v0, f32 *, 0x14) = fparg0;
}
