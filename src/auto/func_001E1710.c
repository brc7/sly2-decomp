#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1710
 * Purpose: Splice builtin: append a RELAYCK 0x49 (Unk49) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x49,fAsync)) and fill its args
 * Info: proposed name AddRelayUnk49 (T2); TU relay
 * Notes: verifier PLAUSIBLE: Placeholder (kind number); PrelaycmdAddRelay(prelay, 0x49, 1), +0x10 = arg. The
 *   Lilly enum also has only Unk49. | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.50] - evidence: Sly 1 / Lilly reference; sources: r6b_rest+verdicts_followups
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1710(s32 arg0, s32 arg1) {
    M2C_FIELD(func_001E01F8(arg0, 0x49, 1), s32 *, 0x10) = arg1;
}
