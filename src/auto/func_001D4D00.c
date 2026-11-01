#include "m2c_prelude.h"
/** @sly2-annot addr=0x1D4D00
 * Purpose: Get field +0x574 of 12-byte slot i
 * Info: proposed name GetRobottankSlotValue (T2); TU tu_1D1370
 * Notes: names_v2 C proposed GetAloLookAtIgnore__FP3ALOPf (0.638) | class identity from whole_game WE
 *   names/object strings: GetRcCarSlotValue -> GetRobottankSlotValue
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
s32 func_001D4D00(s32 arg0, s32 arg1) {
    return M2C_FIELD(((arg1 * 0xC) + arg0), s32 *, 0x574);
}
