#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1388
 * Purpose: Splice builtin: append a RELAYCK 0x3A (Unk3A) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x3A,fAsync)) and fill its args
 * Info: proposed name AddRelayUnk3A (T2); TU relay
 * Notes: v2: demoted to T2 (placeholder token in name: Unk)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1388(s32 arg0, s32 arg1) {
    M2C_FIELD(PrelaycmdAddRelay(arg0, 0x3A, 1), s32 *, 0x10) = arg1;
}
