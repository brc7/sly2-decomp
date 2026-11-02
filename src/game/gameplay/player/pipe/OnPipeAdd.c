#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoAdd();                            /* extern */
extern M2C_UNK D_003E13C0;

/** @sly2-annot addr=0x1C33E8
 * Purpose: PIPE vt23: base then append to pipe list D_003E13C0
 * Info: proposed name OnPipeAdd (T1); class PIPE slot23; TU pipe
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation)
 * Confidence: high [T1 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#pipe-0x1c3390-0x1c3d70
 * @end-sly2-annot */
void OnPipeAdd(s32 arg0) {
    OnLoAdd();
    AppendDlEntry__FP2DLPv(&D_003E13C0, arg0);
}
