#include "m2c_prelude.h"
M2C_UNK func_0011A2E8(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001F3E00();                            /* extern */
extern M2C_UNK D_003E1450;

/** @sly2-annot addr=0x1BAD80
 * Purpose: CID 98 vt22: FreezeSo then RemoveDlEntry from class list D_003E1450
 * Info: proposed name OnCid98Remove (T2); class CID98 slot22; TU tu_1BAB50
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation) (was FreezeCid98)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BAD80(s32 arg0) {
    func_001F3E00();
    func_0011A2E8(&D_003E1450, arg0);
}
