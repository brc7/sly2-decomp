#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1058
 * Purpose: Splice builtin: append a RELAYCK 0x32 (Unk32) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x32,fAsync)) and fill its args
 * Info: proposed name AddRelayUnk32 (T2); TU relay
 * Notes: r0 hint: optid = JOLT | v2: demoted to T2 (placeholder token in name: Unk)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1058(s32 arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(func_001E01F8(arg0, 0x32, arg2), s32 *, 0x10) = arg1;
}
