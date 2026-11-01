#include "m2c_prelude.h"
M2C_UNK func_0011A2E8(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001A6150();                            /* extern */
extern M2C_UNK D_003E1408;

/** @sly2-annot addr=0x183730
 * Purpose: vt22: Freeze + remove from grab list
 * Info: proposed name OnCid38Remove (T2); class CID38 slot22; TU grabspot
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation) (was CID38_Freeze)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183730(s32 arg0) {
    func_001A6150();
    func_0011A2E8(&D_003E1408, arg0);
}
