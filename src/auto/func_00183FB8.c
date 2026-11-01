#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoAdd();                            /* extern */
extern M2C_UNK D_003E1414;

/** @sly2-annot addr=0x183FB8
 * Purpose: vt23 (shared with CID 0x24): Unfreeze + list
 * Info: proposed name OnCid35Add (T2); class CID35 slot23; TU grabspot
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation) (was CID35_Unfreeze)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/game-progression.md#8-key-functions, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_00183FB8(s32 arg0) {
    OnLoAdd();
    AppendDlEntry__FP2DLPv(&D_003E1414, arg0);
}
