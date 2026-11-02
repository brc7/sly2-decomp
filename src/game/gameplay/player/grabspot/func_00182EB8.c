#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoAdd();                            /* extern */
extern M2C_UNK D_003E13F0;

/** @sly2-annot addr=0x182EB8
 * Purpose: vt23: LO Unfreeze + append to grab list D_003E13F0
 * Info: proposed name OnCid12Add (T2); class CID12 slot23; TU grabspot
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation) (was CID12_Unfreeze)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00182EB8(s32 arg0) {
    OnLoAdd();
    AppendDlEntry__FP2DLPv(&D_003E13F0, arg0);
}
