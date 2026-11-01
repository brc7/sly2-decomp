#include "m2c_prelude.h"
M2C_UNK func_0011A220(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001A6178();                            /* extern */
extern M2C_UNK D_003E1408;

/** @sly2-annot addr=0x1836F8
 * Purpose: vt23: Unfreeze + append to grab list
 * Info: proposed name OnCid38Add (T2); class CID38 slot23; TU grabspot
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation) (was CID38_Unfreeze)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001836F8(s32 arg0) {
    func_001A6178();
    func_0011A220(&D_003E1408, arg0);
}
