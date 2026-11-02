#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoRemove();                            /* extern */
extern M2C_UNK D_003E13CC;

/** @sly2-annot addr=0x1CB9E8
 * Purpose: RAIL vt22: LO base (0x1A6150) then remove from D_003E13CC
 * Info: proposed name OnRailRemove (T1); class RAIL slot22; TU rail
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation)
 * Confidence: high [T1 0.85] - evidence: arbitration; sources: arbitration_slots22_23+verdicts_A2+verify_side
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#rail-0x1cb908-0x1cbeb0
 * @end-sly2-annot */
void func_001CB9E8(s32 arg0) {
    OnLoRemove();
    RemoveDlEntry__FP2DLPv(&D_003E13CC, arg0);
}
