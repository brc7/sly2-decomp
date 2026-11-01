#include "m2c_prelude.h"
M2C_UNK func_0011A2E8(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001A6150();                            /* extern */
extern M2C_UNK D_003E13FC;

/** @sly2-annot addr=0x1833A8
 * Purpose: vt22: Freeze + remove from grab list
 * Info: proposed name OnCid34Remove (T2); class CID34 slot22; TU grabspot
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation) (was CID34_Freeze)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001833A8(s32 arg0) {
    func_001A6150();
    func_0011A2E8(&D_003E13FC, arg0);
}
