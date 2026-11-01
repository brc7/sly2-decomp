#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E15B0
 * Purpose: Splice builtin: append a RELAYCK 0x43 (Unk43) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x43,fAsync)) and fill its args
 * Info: proposed name AddRelayUnk43 (T2); TU relay
 * Notes: v2: demoted to T2 (placeholder token in name: Unk)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E15B0(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_v0;

    temp_v0 = func_001E01F8(arg0, 0x43, 0);
    M2C_FIELD(temp_v0, s32 *, 0x10) = arg1;
    M2C_FIELD(temp_v0, s32 *, 0x14) = arg2;
}
