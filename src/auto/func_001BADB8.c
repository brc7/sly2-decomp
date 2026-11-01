#include "m2c_prelude.h"
M2C_UNK func_0011A220(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001F3EA8();                            /* extern */
extern M2C_UNK D_003E1450;

/** @sly2-annot addr=0x1BADB8
 * Purpose: CID 98 vt23: UnfreezeSo then AppendDlEntry to D_003E1450
 * Info: proposed name OnCid98Add (T2); class CID98 slot23; TU tu_1BAB50
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation) (was UnfreezeCid98)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BADB8(s32 arg0) {
    func_001F3EA8();
    func_0011A220(&D_003E1450, arg0);
}
