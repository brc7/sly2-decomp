#include "m2c_prelude.h"
/** @sly2-annot addr=0x1D4D18
 * Purpose: Set field +0x574 of 12-byte slot i
 * Info: proposed name SetRobottankSlotValue (T2); TU tu_1D1370
 * Notes: verifier PLAUSIBLE: Placeholder. p[+0x574 + 12*i] = v; caller is a splice wrapper. | class identity
 *   from whole_game WE names/object strings: SetRcCarSlotValue -> SetRobottankSlotValue | T2 forced:
 *   verifier: placeholder name
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources:
 *   r5b_rest+verdicts_followups+whole_game(class identity)
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4D18(s32 arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(((arg1 * 0xC) + arg0), s32 *, 0x574) = arg2;
}
