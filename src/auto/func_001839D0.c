#include "m2c_prelude.h"
M2C_UNK func_0011A2E8(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001F3E00();                            /* extern */
extern M2C_UNK D_003E13E4;

/** @sly2-annot addr=0x1839D0
 * Purpose: vt22: FreezeSo + remove from grab list
 * Info: proposed name OnBarrelRemove (T1); class BARREL slot22; TU grabspot
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation) (was CID88_Freeze) | v3 class identity: OnCid88Remove -> OnBarrelRemove
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources:
 *   arbitration_slots22_23+class_review(master_names_v2_1)
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001839D0(s32 arg0) {
    func_001F3E00();
    func_0011A2E8(&D_003E13E4, arg0);
}
