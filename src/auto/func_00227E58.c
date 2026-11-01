#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoRemove();                            /* extern */
extern M2C_UNK D_003E1480;

/** @sly2-annot addr=0x227E58
 * Purpose: OnLoRemove + RemoveDlEntry(&D_003E1480)
 * Info: proposed name OnCid23Remove (T2); class CID23 slot22; TU vol
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#vol-0x227bb8-0x227fe8
 * @end-sly2-annot */
void func_00227E58(s32 arg0) {
    OnLoRemove();
    RemoveDlEntry__FP2DLPv(&D_003E1480, arg0);
}
