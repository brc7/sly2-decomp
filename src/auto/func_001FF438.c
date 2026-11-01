#include "m2c_prelude.h"
M2C_UNK func_0011A220(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001A6178();                            /* extern */
extern M2C_UNK D_003E13D8;

/** @sly2-annot addr=0x1FF438
 * Purpose: vt23: add to SW update array (0x1A6178) and to global DL D_003E13D8
 * Info: proposed name OnCid39Add (T2); class CID39 slot23; TU tu_1FF3B8
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1ff3b8-0x1ff3b8-0x1ff690
 * @end-sly2-annot */
void func_001FF438(s32 arg0) {
    func_001A6178();
    func_0011A220(&D_003E13D8, arg0);
}
