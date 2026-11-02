#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoRemove();                            /* extern */
extern M2C_UNK D_003E13C0;

/** @sly2-annot addr=0x1C3420
 * Purpose: PIPE vt22: base then remove from D_003E13C0
 * Info: proposed name OnPipeRemove (T1); class PIPE slot22; TU pipe
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation)
 * Confidence: high [T1 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#pipe-0x1c3390-0x1c3d70
 * @end-sly2-annot */
void OnPipeRemove(s32 arg0) {
    OnLoRemove();
    RemoveDlEntry__FP2DLPv(&D_003E13C0, arg0);
}
